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

/***** Includes *****/
#include <stdio.h>
#include "mxc_config.h"
#include "uart.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/

// *****************************************************************************
int main(void)
{

    // Delay loop to prevent code from running while resetting
    volatile int k;
    for(k = 0; k < 0xFFFF; k++) {}

#if 0
    // Nothing abmormal observed with this loop 
    int i;
    for (i = 0; i < 256; i++) {
        UART_WriteByte(MXC_UART0, '.');
    }

#else

    // Putting this newline before the fflush seems to fix the problem
    // putchar('\n');

    int i;
    for (i = 0; i < 256; i++) {
        if (i && !(i & 0x1f)) {
            putchar('\n');
        } else {
            fflush(stdout);
        }
        putchar('.');

        // Delaying between character writes also seems to fix the problem
        // volatile int j;
        // for(j = 0; j < 0xFF; j++) {}
    }

    // putchar('\n');
#endif

    putchar('\n');

    
    while(1) {}

}
