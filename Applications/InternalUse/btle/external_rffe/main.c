/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Id: main.c 42272 2019-04-09 19:28:04Z kevin.gillespie $
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "mcr_regs.h"
#include "mxc_config.h"
#include "gcr_regs.h"
#include "board.h"
#include "gpio.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE 128

#define MXC_R_SIR_SHR4          *((uint32_t*)(0x40000410))
#define MXC_R_TM                *((uint32_t*)(0x40000C00))
#define MXC_R_TMR3              *((uint32_t*)(0x40000C18))

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

/*************************************************************************************************/
/*!
 *  \brief  Initialize platform.
 *
 *  \param  msPerTick   Milliseconds per timer tick.
 *
 *  \return None.
 */
/*************************************************************************************************/
void mainPlatformInit(void)
{
    /* Change the pullup on the RST pin to 25K */
    MXC_MCR->ctrl = 0x202;
    
    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D | MXC_F_GCR_PERCKCN0_SPI1D);  // Clear GPIO0 and GPIO1 Disable
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD );  // Clear BTLE and ICACHE0 disable
    
    /* Added to prevent printf() and friends from using malloc() */
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);

    // Enable SPI on GPIO0.16-18
    const gpio_cfg_t rffe_spi =  { PORT_0, (PIN_16 | PIN_17 | PIN_18 | PIN_19), GPIO_FUNC_ALT2, GPIO_PAD_NONE};
    GPIO_Init();
    GPIO_Config(&rffe_spi);

    // Enable test mode
    MXC_R_TM = 0x1;
    if((MXC_R_TM & 0x1) != 0x1) {
        printf("Error enabling test mode! You are not prepared..\n");
        while(1) {}
    }

    /* Enable BLE if disabled by test */
    MXC_R_SIR_SHR4 = 0x00100010;

    // Set TMR3 bit 21 and 19
    MXC_R_TMR3 |= ((0x1 << 21) | (0x1 << 19));

    // Set the drive strength of the GPIO
    MXC_GPIO0->ds |= (0xFFFFFFFF);
    MXC_GPIO0->ds1 &= ~(0xFFFFFFFF);
    MXC_GPIO1->ds |= (0xFFFFFFFF);
    MXC_GPIO1->ds1 &= ~(0xFFFFFFFF);

}

/*************************************************************************************************/
/*!
 *  \fn     main
 *
 *  \brief  Entry point for demo software.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
int main(void)
{

    printf("\n***** MAX32665 External RFFE *****\n");

    printf("Starting init\n");

    mainPlatformInit();

    printf("Initialization complete\n");

    while (1) {}
}

/*****************************************************************/
void HardFault_Handler(void)
{
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1) {}
}
