/*******************************************************************************
 * Copyright (C) 2020 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date$
 * $Revision$
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "wsf_types.h"
#include "led.h"
#include "board.h"
#include "mcr_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
   Local Variables
**************************************************************************************************/
static volatile uint32_t eccFlag;
static volatile uint32_t eccErr;
static volatile uint32_t eccDErr;
static volatile uint32_t eccAddr;

/**************************************************************************************************
  Functions
**************************************************************************************************/
void ECC_IRQHandler(void) 
{
    eccErr = MXC_GCR->eccerr;
    eccDErr = MXC_GCR->ecc_ced;
    eccAddr = MXC_GCR->eccerrad;
    eccFlag = 1;

    MXC_GCR->eccerr = MXC_GCR->eccerr;
    MXC_GCR->ecc_ced = MXC_GCR->ecc_ced;

    printf("ECC Error:              0x%08x\n", eccErr);
    printf("ECC Not Double Error:   0x%08x\n", eccDErr);
    printf("ECC Error Address:      0x%08x\n", eccAddr);

    while(1)
    {
    }
}

void ECC_SetEnable(bool_t bEnable)
{
    if (bEnable)
    {
        MXC_GCR->eccirqen |=
                MXC_F_GCR_ECCIRQEN_SYSRAM0ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM1ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM2ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM3ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM4ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM5ECCEN;
        NVIC_EnableIRQ(ECC_IRQn);
    }
    else
    {
        MXC_GCR->eccirqen &= ~(
                MXC_F_GCR_ECCIRQEN_SYSRAM0ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM1ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM2ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM3ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM4ECCEN |
                MXC_F_GCR_ECCIRQEN_SYSRAM5ECCEN);
        NVIC_DisableIRQ(ECC_IRQn);
    }
}
