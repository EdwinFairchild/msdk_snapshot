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
 * $Date: 2019-10-17 07:37:39 -0500 (Thu, 17 Oct 2019) $
 * $Revision: 47812 $
 *
 ******************************************************************************/

#ifndef WAKEUP_H
#define WAKEUP_H

#include "lp.h"
#include "icc.h"
#include "simo.h"
#include "board.h"
#include "sch_api_ble.h"
#include "wut_regs.h"
#include "wsf_timer.h"
#include "usbhs_regs.h"
#include "bb_drv.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "tmr_regs.h"

#define US_TO_BBTICKS(x)    (((x)*(BB_CLK_RATE_HZ/100000)+9)/10)
#define WAKEUP_TIME_US      (1800)
#define MIN_DEEPSLEEP_US    (0)
#define WUT_FREQ            (32768)  /* nominal */

void WUT_Init(void);
void WUT_SetWakeup(uint32_t sleep_time);
void WUT_RestoreBBClock(void);
void WUT_Delay_MS(uint32_t wait_ms);
void WUT_Delay_100US(uint32_t wait_100us);
uint32_t GetWakeDelay(uint32_t wait_ticks);
void DisableUnused(void);
void EnterDeepsleep(void);
void ExitDeepsleep(void);

#endif
