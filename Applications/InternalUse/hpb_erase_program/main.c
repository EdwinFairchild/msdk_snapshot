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
#include <stdio.h>
#include "mxc_config.h"
#include "hpb.h"
#include "icc_regs.h"
#include "dcc_regs.h"
#include "s26k.h"
#include "tmr_utils.h"

/***** Definitions *****/
#define BASE_ADDRESS            (MXC_HPB_MEM_BASE + 0x10000000)
#define TEST_LEN                0x10
#define TEST_ADDR               0x0

/***** Globals *****/
uint16_t tx_data[TEST_LEN];

/***** Functions *****/


// *****************************************************************************
int main(void)
{
    uint16_t status;
    unsigned i;

    // Initialize the data
    for(i = 0; i < TEST_LEN; i++) {
        tx_data[i] = i;
    }


    TMR_Delay(MXC_TMR0, MSEC(100));

    printf("\n***** HyperFlash Erase and program test *****\n");    

    // Disable the instruction cache
    MXC_ICC->cache_ctrl = 0;
    MXC_DCC->cache_ctrl = 0;
    MXC_ICX->cache_ctrl = 0;

    // Bypass the line fill buffer
    MXC_GCR->scon |= (0x1 << 9);

    // Initialize the S26K
    S26K_Init(1, BASE_ADDRESS);

    status = S26K_GetSectorProtection(TEST_ADDR);
    printf("Sector Protection Status = 0x%x\n", status);

    status = S26K_GetSectorPPB(TEST_ADDR);
    printf("Sector PPB = 0x%x\n", status);

    status = S26K_GetASPStatus();
    printf("ASP Status = 0x%x\n", status);

    // Erase the Hyper flash sector
    if((status = S26K_SectorErase(TEST_ADDR)) != E_NO_ERROR) {
        printf("Error erasing 0x%x\n", status);
    } else {
        printf("Sector erased\n");
    }
    if((status = S26K_Write16(TEST_ADDR, 0x1234)) != E_NO_ERROR) {
        printf("Error programming = 0x%x\n", status);
    } else {
        printf("Sector programmed\n");
    }

    printf("Done testing\n");
    while (1) {}
}
