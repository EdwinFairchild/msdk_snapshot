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
 * $Date: 2021-08-03 10:16:23 -0500 (Tue, 03 Aug 2021) $
 * $Revision: 58927 $
 *
 ******************************************************************************/

#ifndef _WAKEUP_H_
#define _WAKEUP_H_

#define US_TO_DBBTICKS(xx)    (((xx)*16+9)/10)
#define WAKEUP_TIME_US        (1420)
#define MIN_DEEPSLEEP_US      (10000)
#define WUT_FREQ              (32768)

void WUT_Delay_MS(uint32_t waitMs);
uint32_t get_powerup_delay(uint32_t wait_ticks);
void wait_wut_edge(void);
void set_ahead_wakeup(uint32_t sleep_time);
void restore_dbb_clock(void);
void wake_ARM(void);

uint8_t GetARMFlag(void);
void SetSDMAFlagDirect(uint8_t value);
void SetSDMAFlag(uint8_t value);
void SetSDMAWakeupTime(uint32_t value);

#endif
