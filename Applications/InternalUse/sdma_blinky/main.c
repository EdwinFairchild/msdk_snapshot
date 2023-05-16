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
 * $Date: 2019-04-11 14:11:48 -0500 (Thu, 11 Apr 2019) $
 * $Revision: 42389 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "gcr_regs.h"
#include "sdma_regs.h"
#include "mxc_pins.h"
#include "gpio.h"
#include "led.h"

/***** Definitions *****/

/***** Globals *****/

// Make sure this is in SRAM section
volatile unsigned sdma_code = 0x0cfeda3a;  // jump $

/***** Functions *****/

// *****************************************************************************
int main(void)
{

	printf("\nSDMA blinky\n");

    // Disable SDMA, assert reset
    MXC_SDMA->ctrl &= ~(MXC_F_SDMA_CTRL_EN);

    // Enable SDMA
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_SDMAD);

    // Setup code address
    MXC_SDMA->ip_addr = (uint32_t)&sdma_code;

    MXC_SDMA->ctrl |= MXC_F_SDMA_CTRL_EN;
  
    printf("Setup complete\n");

    while(1) {}
}
