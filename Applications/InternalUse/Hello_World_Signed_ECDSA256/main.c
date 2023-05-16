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
 * $Date: 2019-02-21 15:14:01 -0600 (Thu, 21 Feb 2019) $
 * $Revision: 41164 $
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
#include "led.h"
#include "tmr_utils.h"

/***** Definitions *****/

// Enable define below to create 128K image for ROM load testing.
#define USE_LOTS_OF_CODE_SPACE
// Use Test Mode to turn off classic UART so I can use existing SDK code.
//#define TURN_ON_CLASSIC_UART

/***** Globals *****/

#define SUB_ARRAY_4(x) (x),(x+1),(x+2),(x+3)
#define SUB_ARRAY_8(x) SUB_ARRAY_4(x), SUB_ARRAY_4(x+1)
#define SUB_ARRAY_16(x) SUB_ARRAY_8(x), SUB_ARRAY_8(x+1)
#define SUB_ARRAY_32(x) SUB_ARRAY_16(x), SUB_ARRAY_16(x+1)
#define SUB_ARRAY_64(x) SUB_ARRAY_32(x), SUB_ARRAY_32(x+1)
#define SUB_ARRAY_128(x) SUB_ARRAY_64(x), SUB_ARRAY_64(x+1)
#define SUB_ARRAY_256(x) SUB_ARRAY_128(x), SUB_ARRAY_128(x+1)
#define SUB_ARRAY_512(x) SUB_ARRAY_256(x), SUB_ARRAY_256(x+1)
#define SUB_ARRAY_1024(x) SUB_ARRAY_512(x), SUB_ARRAY_512(x+1)
#define SUB_ARRAY_2K(x) SUB_ARRAY_1024(x), SUB_ARRAY_1024(x+1)
#define SUB_ARRAY_4K(x) SUB_ARRAY_2K(x), SUB_ARRAY_2K(x+1)
#define SUB_ARRAY_8K(x) SUB_ARRAY_4K(x), SUB_ARRAY_4K(x+1)
#define SUB_ARRAY_16K(x) SUB_ARRAY_8K(x), SUB_ARRAY_8K(x+1)
#define SUB_ARRAY_32K(x) SUB_ARRAY_16K(x), SUB_ARRAY_16K(x+1)
#define SUB_ARRAY_64K(x) SUB_ARRAY_32K(x), SUB_ARRAY_32K(x+1)
#define SUB_ARRAY_128K(x) SUB_ARRAY_64K(x), SUB_ARRAY_64K(x+1)
#define SUB_ARRAY_256K(x) SUB_ARRAY_128K(x), SUB_ARRAY_128K(x+1)
#define SUB_ARRAY_512K(x) SUB_ARRAY_256K(x), SUB_ARRAY_256K(x+1)

#ifdef USE_LOTS_OF_CODE_SPACE
// 98K + (other code) ~ 127K
const uint8_t func_array[] = { SUB_ARRAY_64K(5), SUB_ARRAY_32K(1), SUB_ARRAY_2K(1) };
// Use 512K, cannot use in Emulator, memory too small
//const uint8_t func_array[] = { SUB_ARRAY_512K(5) };
volatile int trick_volatile = 0;
#else // USE_LOTS_OF_CODE_SPACE
const uint8_t func_array[] = { SUB_ARRAY_4(1) };
volatile int trick_volatile = 0;
#endif // USE_LOTS_OF_CODE_SPACE

/***** Functions *****/

#ifdef TURN_ON_CLASSIC_UART
int PreInit(void)
{
    volatile uint32_t *tm_ptr = (uint32_t *)0x40000C00;
    volatile uint32_t *uartselect_ptr = (uint32_t *)0x40000410;
    uint32_t UART_SELECT_MASK = (1 << 14) | (1 << 30);

    // Turn on test mode TESTMODE.TME (bit 0)
    *tm_ptr = 1;
    // Turn on classic UART
    *uartselect_ptr |= UART_SELECT_MASK;
    // Turn off test mode
    *tm_ptr = 0;

    return 0;
}
#endif // TURN_ON_CLASSIC_UART

// *****************************************************************************
int main(void)
{
    int count = 0;
    const sys_cfg_tmr_t sys_tmr_cfg = NULL; /* No system specific configuration needed. */

    printf("\n\n\n");
    printf("**********Hello World Example**********\n");
    printf("Array size: %d\n",sizeof(func_array));

    while(1) {
//        LED_On(0);
        TMR_Delay(MXC_TMR0, MSEC(500), &sys_tmr_cfg);
//        LED_Off(0);
        TMR_Delay(MXC_TMR0, MSEC(500), &sys_tmr_cfg);
        printf("count = %d\n", count++);
#ifdef USE_LOTS_OF_CODE_SPACE
        trick_volatile = func_array[count];
#endif // USE_LOTS_OF_CODE_SPACE
    }
}
