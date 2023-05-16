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
 * @brief   WDT main file
 * @details Runs the WDT test if the previous reset was not caused by the watchdog
 */

/***** Includes *****/
#include <stdio.h>
#include "mxc_config.h"
#include "wdt.h"
#include "tmr_utils.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/

// *****************************************************************************
int main(void)
{
    if(WDT_GetResetFlag(MXC_WDT0)) {
        WDT_ClearResetFlag(MXC_WDT0);
        WDT_EnableReset(MXC_WDT0, 0);
        WDT_Enable(MXC_WDT0, 0);
        printf("  Watchdog reset\n");
        while(1) {}
    }

    printf("\n***** WDT Timer Example *****\n");
    TMR_Delay(MXC_TMR0, MSEC(100));

    // Configure the period
    WDT_SetResetPeriod(MXC_WDT0, WDT_PERIOD_2_25);

    // Enable the Reset
    WDT_EnableReset(MXC_WDT0, 1);

    // Start the timer
    WDT_Enable(MXC_WDT0, 1);

    // Reset the watchdog for 5 seconds
    printf("Resetting the watchdog\n");
    TMR_TO_Start(MXC_TMR0, SEC(5));
    while(TMR_TO_Check(MXC_TMR0) == E_NO_ERROR) {
        WDT_ResetTimer(MXC_WDT0);
    }

    printf("Waiting for reset\n");

    while(1) {}
    return 0;
}
