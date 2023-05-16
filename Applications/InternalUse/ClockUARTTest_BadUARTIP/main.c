/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-08-10 09:36:39 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Timer example
 * @details PWM Timer        - Outputs a PWM signal (2Hz, 30% duty cycle) on 3.7
 *          Continuous Timer - Outputs a continuous 1s timer on LED0 (GPIO toggles every 500s)
 *          One Shot Timer   - Starts a one shot timer - LED1 turns on when one shot time (1 sec) is complete
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "nvic_table.h"
#include "gpio.h"
#include "board.h"
#include "tmr.h"
#include "tmr_utils.h"
#include "led.h"
#include "uart.h"

/***** Definitions *****/

// Parameters for PWM output
#define PORT_PWM   PORT_0  // port
#define PIN_PWM    PIN_13   // pin
#define FREQ       200 // (Hz) 
#define DUTY_CYCLE 75  // (%)  
#define PWM_TIMER  MXC_TMR1  // must change PORT_PWM and PIN_PWM if changed

// Parameters for Continuous timer
#define CONT_LED_IDX       0
#define INTERVAL_TIME_CONT 1  // (s) will toggle after every interval 
//#define CONT_TIMER MXC_TMR0   // Can be MXC_TMR0 through MXC_TMR5
// Use Timer 5, it comes out on P0.29/AF3 or AF4 depending on how it is counted.
#define CONT_TIMER MXC_TMR5   // Can be MXC_TMR0 through MXC_TMR5

// Parameter for One shot timer
#define OST_LED_IDX        1
#define INTERVAL_TIME_OST  3  // (s) before turns LED on, then off
#define OST_TIMER  MXC_TMR4  // Can be MXC_TMR0 through MXC_TMR5

// Check Frequency bounds
#if (FREQ == 0)
#error "Frequency cannot be 0."
#elif (FREQ > 100000)
#error "Frequency cannot be over 10000."
#endif

// Check duty cycle bounds
#if (DUTY_CYCLE < 0) || (DUTY_CYCLE > 100)
#error "Duty Cycle must be between 0 and 100."
#endif

/***** Globals *****/
gpio_cfg_t gpio_cont; //for ContinuousTimer()
volatile int led_status = 0;  // to toggle continuous timer


/***** Functions *****/

void ContinuousTimer()
{
    // Declare variables
    tmr_cfg_t tmr; 
    uint32_t period_ticks = PeripheralClock/4*INTERVAL_TIME_CONT;

    // Initial state is off
//    LED_Off(CONT_LED_IDX);

    /*    
    Steps for configuring a timer for PWM mode:
    1. Disable the timer
    2. Set the prescale value
    3  Configure the timer for continuous mode
    4. Set polarity, timer parameters
    5. Enable Timer
    */

    TMR_Disable(CONT_TIMER);
    
    TMR_Init(CONT_TIMER, TMR_PRES_4, 0);
//    TMR_Init(CONT_TIMER, TMR_PRES_1, 0);
    
    tmr.mode = TMR_MODE_CONTINUOUS;
//    tmr.cmp_cnt = period_ticks; //SystemCoreClock*(1/interval_time);
    tmr.cmp_cnt = 1;
    tmr.pol = 0;
    TMR_Config(CONT_TIMER, &tmr);

    TMR_Enable(CONT_TIMER);

    //Configure port pin output.
    gpio_cfg_t gpio_out;    // to configure GPIO

    // Congfigure GPIO port and pin for PWM
    gpio_out.func = GPIO_FUNC_ALT4;
    gpio_out.port = PORT_0;
    gpio_out.mask = PIN_29; 
    gpio_out.pad = GPIO_PAD_NONE;
    
    if (GPIO_Config(&gpio_out) != E_NO_ERROR) {
        printf("Failed GPIO_Config for out.\n");
    }
    
    printf("Continuous timer started.\n");
}

extern mxc_uart_regs_t *ConsoleUart;

/***** File Scope Variables *****/
const uart_cfg_t uart_cfg_main = {
    .parity = UART_PARITY_DISABLE,
    .size   = UART_DATA_SIZE_8_BITS,
    .stop   = UART_STOP_1,
    .flow   = UART_FLOW_CTRL_DIS,
    .pol    = UART_FLOW_POL_DIS,
    .baud   = CONSOLE_BAUD,
/*    .clksel = UART_CLKSEL_ALTERNATE*/
    .clksel = UART_CLKSEL_SYSTEM
};

const sys_cfg_uart_t uart_sys_cfg_main = {
    .map = MAP_A,
    .flow = Disable
};

#define UART_ER_IF (MXC_F_UART_INT_FL_RX_FRAME_ERROR | \
                    MXC_F_UART_INT_FL_RX_PARITY_ERROR | \
                    MXC_F_UART_INT_FL_RX_OVERRUN)

/* ************************************************************************* */
uint32_t uart_error_check(mxc_uart_regs_t *uart)
{
    return (uart->int_fl & UART_ER_IF);
}

// How many bytes to receive before turning off LED.
#define LED_DELAY_ITERATION 20

void Echo(void)
{
    static int led0_delay = 0;
    static int led1_delay = 0;
           
    while (1)
    {
        uint8_t onechar;
        uint32_t error_flags;

        onechar = UART_ReadByte(ConsoleUart);
        error_flags = uart_error_check(ConsoleUart);
        if (error_flags == 0)
        {
            if (led0_delay > 0)
            {
                led0_delay--;
                if (led0_delay == 0)
                {
                    LED_Off(1);
                }
            }
            if (led1_delay > 0)
            {
                led1_delay--;
                if (led1_delay == 0)
                {
                    LED_Off(1);
                }
            }

        }
        else
        {
            UART_ClearFlags(ConsoleUart,error_flags);
            if (error_flags & MXC_F_UART_INT_FL_RX_OVERRUN)
            {
                LED_On(0);
                led0_delay = LED_DELAY_ITERATION;
            }
            if (error_flags & MXC_F_UART_INT_FL_RX_FRAME_ERROR)
            {
                LED_On(1);
                led1_delay = LED_DELAY_ITERATION;
            }
//            printf("Error on RX %08X\n",error_flags);
        }
        UART_WriteByte(ConsoleUart,onechar);
    }
}


// *****************************************************************************
int main(void)
{
    // Go to 60 MHz HIRC clock.
    SYS_Clock_Select(SYS_CLOCK_HIRC, MXC_TMR0);
    SystemCoreClockUpdate();
    // Start 7.3728MHz clock
    MXC_GCR->clkcn |= (1 << 20);
    // Reinit the UART
    UART_Init(ConsoleUart, &uart_cfg_main, &uart_sys_cfg_main);

    //Exact timer operations can be found in tmr_utils.c

    LED_On(0);
    LED_Off(0);
    printf("\n\n\n");
    printf("***** Output divided system clock on P0.29. *****\n\n");
    printf("System Clock set to SYS_CLOCK_HIRC\n");
    printf("UART using %s clock.\n",(uart_cfg_main.clksel == UART_CLKSEL_SYSTEM)?"System":"Alternate");
    printf("UART set to %d bps.\n",(uart_cfg_main.baud));
    printf("baud0 0x%08X\n",ConsoleUart->baud0);
    printf("baud1 0x%08X\n",ConsoleUart->baud1);

    ContinuousTimer();

    Echo();

    while(1);

    return 0;
}
