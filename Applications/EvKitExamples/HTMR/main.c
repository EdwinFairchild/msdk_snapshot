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
 * $Date: 2020-01-17 08:41:53 -0600 (Fri, 17 Jan 2020) $
 * $Revision: 50773 $
 *
 ******************************************************************************/

/**
 * @file        main.c
 * @brief       Configures and starts the HTMR and demonstrates the use of the alarms.
 * @details     The HTMR is enabled and the sub-second alarm set to trigger every 250 ms.
 *              P2.25 (LED0) is toggled each time the sub-second alarm triggers.  The
 *              time-of-day alarm is set to 2 seconds.  When the time-of-day alarm
 *              triggers, the rate of the sub-second alarm is switched to 500 ms.  The
 *              time-of-day alarm is then rearmed for another 2 sec.  Pressing SW2 will
 *              output the current value of the HTMR to the console UART.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "board.h"
#include "htmr.h"
#include "led.h"
#include "pb.h"
#include "tmr_utils.h"

/***** Definitions *****/
#define LED_FLASH   0
#define LED_ALARM   1

#define PORT_PWM   PORT_0 
#define PIN_PWM    PIN_13

/***** Globals *****/
volatile int buttonPressed = 0;
gpio_cfg_t pwm_out;

/***** Functions *****/
void SysTick_Handler(void) {
    // Do nothing here
}

void HTMR0_IRQHandler(void)
{
    if(HTMR_GetFlags(MXC_HTMR0) & MXC_F_HTMR_CTRL_ALDF) {
        LED_On(LED_ALARM);
        HTMR_ClearFlags(MXC_HTMR0, MXC_F_HTMR_CTRL_ALDF);
    }
    if(HTMR_GetFlags(MXC_HTMR0) & MXC_F_HTMR_CTRL_ALSF) {
        LED_Toggle(LED_FLASH);
        HTMR_ClearFlags(MXC_HTMR0, MXC_F_HTMR_CTRL_ALSF);
    }

    return;
}

void buttonHandler()
{
    buttonPressed = 1;
}

void alarmSetHandler()
{
    HTMR_Disable(MXC_HTMR0);

    if (HTMR_SetLongIntAlarm(MXC_HTMR0, HTMR_GetLongCount(MXC_HTMR0)+10000) != E_NO_ERROR) {
        printf("Failed to set Long Interval Alarm.\n");
    }

    HTMR_Enable(MXC_HTMR0);
}

void printTime()
{
    double count;
    
    /* The short count register is 20 bits long, however 
     * the long count counter is incremented every 4096 (12 bits) short counts
     * so we need to take short count mod 4096 and add long count to 
     * get a float representing the total number of long counts 
     */
    count = (HTMR_GetShortCount(MXC_HTMR0)%4096) / 4096.0;
    count += HTMR_GetLongCount(MXC_HTMR0);
    
    printf("Current Count %f\n", count);
}

// *****************************************************************************
int main(void)
{
    sys_cfg_htmr_t sys_htmr_cfg = NULL; //No system specific settings needed for the timers.
    sys_cfg_tmr_t sys_tmr_cfg = NULL;

    pwm_out.func = GPIO_FUNC_OUT;
    pwm_out.port = PORT_PWM;
    pwm_out.mask = PIN_PWM;
    pwm_out.pad = GPIO_PAD_PULL_DOWN;


    printf("\n***** High Speed Timer Example *****\n\n");
    printf("This example enables the HTMR and sets the short interval\n");
    printf("alarm to trigger every ~2^22 short interval counts (0.5sec)\n");
    printf("Pressing PB0 will print the current count to the console\n");
    printf("Pressing PB1 will set the Long Interval alarm to light LED1 in 10000 counts\n\n");
    
    NVIC_EnableIRQ(HTMR0_IRQn);
    
    /* Setup callback to receive notification of when button is pressed. */
    PB_RegisterCallback(0, (pb_callback)buttonHandler);
    PB_RegisterCallback(1, (pb_callback)alarmSetHandler);
    
    /* Turn LED off initially */
    LED_Off(LED_ALARM);
    
    if (HTMR_Init(MXC_HTMR0, 0, 0, &sys_htmr_cfg) != E_NO_ERROR) {
        printf("Failed HTMR Initialization.\n");
        while(1);
    }

    if (HTMR_SetShortIntAlarm(MXC_HTMR0, 0xFFC7BFFF) != E_NO_ERROR) {
        printf("Failed to set short interval alarm\n");
    }
    
    HTMR_Enable(MXC_HTMR0);
    printf("Timer started.\n");
    printTime();
    
    while (1) {
        if (buttonPressed) {
            /* Show the time elapsed. */
            printTime();
            /* Delay for switch debouncing. */
            TMR_Delay(MXC_TMR0, MSEC(100), &sys_tmr_cfg);
            /* Re-arm switch detection. */
            buttonPressed = 0;
        }
    }
}
