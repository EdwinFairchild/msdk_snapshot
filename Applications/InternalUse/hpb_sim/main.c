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
 * @brief   Simulation test.
 * @details Testing putting functions in different memory segments.
 */

/***** Includes *****/
#include <string.h>
#include "mxc_config.h"
#include "hpb.h"
#include "icc_regs.h"
#include "dcc_regs.h"

/***** Definitions *****/
#define BASE0_ADDRESS            0x60000000
#define BASE1_ADDRESS            (BASE0_ADDRESS + 0x10000000)

/***** Globals *****/

/***** Functions *****/

void HardFault_Handler(void)
{
    while(1) {}
}

// *****************************************************************************
int main(void)
{

    volatile int i;
    for(i = 0; i < 0xFFFF; i++) {}


    // Disable the instruction cache
    MXC_ICC->cache_ctrl = 0;
    MXC_DCC->cache_ctrl = 0;
    MXC_ICX->cache_ctrl = 0;

    // MXC_HPB->mcr[0] &= ~MXC_F_HPB_MCR_CRT;
    // MXC_HPB->mcr[0] &= ~MXC_F_HPB_MCR_DEVTYPE;
    // MXC_HPB->mbr[0] = BASE1_ADDRESS;

    MXC_HPB->mcr[1] &= ~MXC_F_HPB_MCR_CRT;
    MXC_HPB->mcr[1] &= ~MXC_F_HPB_MCR_DEVTYPE;
    MXC_HPB->mbr[1] = BASE1_ADDRESS;

    for(i = 0; i < 0xFFFF; i++) {} 

    while (1) {}
}
