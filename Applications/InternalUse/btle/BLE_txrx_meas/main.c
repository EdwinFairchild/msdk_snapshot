/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-06-07 07:33:41 -0700 (Fri, 07 Jun 2019) $
 * $Revision: 43735 $
 *
 ******************************************************************************/

#include <stdio.h>
#include "mxc_config.h"
#include "lp.h"
#include "gcr_regs.h"
#include "board.h"
#include "simo.h"
#include "bb_int.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#ifndef RUN_VOLTAGE
#define RUN_VOLTAGE       900
#endif

#ifndef TX_POWER
#define TX_POWER            0
#endif

#ifndef MEAS_TX
#ifndef MEAS_RX
#ifndef MEAS_BLE_IDLE
#ifndef MEAS_SLEEP
#error Must define one of the measurement options
#endif
#endif
#endif
#endif

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

const BbRtCfg_t _bb_cfg = {
    /*clkPpm*/                  20,
    /*rfSetupDelayUsec*/        BB_RF_SETUP_DELAY_US,
    /*defaultTxPower*/          TX_POWER,
    /*maxScanPeriodMsec*/       BB_MAX_SCAN_PERIOD_MS,
    /*schSetupDelayUsec*/       BB_SCH_SETUP_DELAY_US
};

/**************************************************************************************************
  Functions
**************************************************************************************************/

static void switchToHIRC(void)
{
    MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
    MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV1;
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    /* Disable unused clocks */
    while(!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY)); /* Wait for the switch to occur */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);
    SystemCoreClockUpdate();
}

int main(void)
{
    /* Use the 60 MHz clock */
    switchToHIRC();
    Console_Init();

    printf("\n\n***** MAX32665 BLE TX RX *****\n");;

#ifndef MEAS_SLEEP
    BbInitRunTimeCfg(&_bb_cfg);
    BbDrvInit();
    BbDrvEnable();

#ifndef MEAS_BLE_IDLE
#ifdef MEAS_TX
    llc_api_tx_ldo_setup();
    dbb_seq_tx_enable();
#endif
#ifdef MEAS_RX
    llc_api_tx_ldo_done();
    dbb_seq_rx_enable();
#endif
#endif
#endif

    printf("Disabling and Sleeping\n");

    /* Delay before sleeping so we can debug and load again */
    volatile int i;
    for(i = 0; i < 0x8FFFFF; i++) {}

    /* Disable periphal clock gates */
    MXC_GCR->perckcn0 = 0xFFFFFFFF;
    MXC_GCR->perckcn1 = 0xFFFFFFFF;

    __disable_irq();

    SIMO_setVregO_B(RUN_VOLTAGE);
    SIMO_setVregO_C(RUN_VOLTAGE);

    while (1) {
      LP_EnterSleepMode();
    }
}
