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
 * @brief   Memory Dump
 * @details This application prints the memory contents to the serial port.
 */

/***** Includes *****/
#include <stdio.h>
#include "mxc_config.h"
#include "tmr_utils.h"

/***** Definitions *****/
#define START_ADDR          0x10000007
#define END_ADDR            0x10000102

/***** Globals *****/

/***** Functions *****/

// *****************************************************************************
int main(void)
{
    unsigned i, start, end;

    // Delay to clean up the output as the device is being reset
    TMR_Delay(MXC_TMR0, MSEC(100));
    printf("\n\n");

    // 16-bit align the start and end addr
    start = START_ADDR - (START_ADDR % 0x10);
    end = END_ADDR + (0x10 - (END_ADDR % 0x10));

    for(i = start; i < end; i+=4) {
        if((i % 16) == 0) {
            printf("0x%08x:", i);
        }

        printf("    0x%08x", (uint32_t)*(uint32_t*)i);

        if((i % 16) == 12) {
            printf("\n");
        }
    }

    while(1) {}
    return 0;
}
