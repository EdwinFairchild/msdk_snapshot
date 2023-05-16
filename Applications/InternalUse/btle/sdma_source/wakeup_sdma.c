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

#include "max32665.h"
#include "max32665_sdma.h"
#include "wsf_types.h"
#include "wsf_timer.h"
#include "wsf_os.h"
#include "wsf_cs.h"
#include "wut_regs.h"
#include "wakeup_sdma.h"
#include "sdma_regs.h"
#include "sema_regs.h"
#include "ipc_defs.h"
#include "ll_debug.h"
#include "gpio_regs.h"
#include "bb_drv.h"

#define WAKEUP_32M_US           (3200)

#define POWERUP_DELAY(sleep_ticks) (US_TO_DBBTICKS(WAKEUP_32M_US) + (sleep_ticks >> 6))

#define MIN_DELAY (10UL)
#define MAX_DELAY (1000UL)

#define MXC_WUT_MIN_DELAY (3)

#define ENABLE_READ_BACKOFF

uint32_t wut_count;
uint32_t wut_snapshot;

/* Defined in the startup file */
extern shared_mem_t arm_shared_mem;
extern shared_mem_t sdma_shared_mem;

#define ENABLE_BACKGROUND_HARDFAULT_WORKAROUND

/*************************************************/
/* Get the ARM state flag */
uint8_t GetARMFlag(void)
{
    bool_t retval = FALSE;
    uint32_t sema_val;
    uint8_t value;

    while(retval == FALSE) {

        WsfTaskLock();

        // Attempt to lock the ARM_SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];

        if (!sema_val) {
            value = arm_shared_mem.state.arm.fsm;
            retval = TRUE;
            MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0x0;
        }

        WsfTaskUnlock();
    }

    return value;
}

/*************************************************/
/* Set the SDMA state flag without semaphore protection. */
void SetSDMAFlagDirect(uint8_t value)
{
    sdma_shared_mem.state.sdma.fsm = (value ^ (sdma_shared_mem.state.sdma.fsm & SDMA_FLAG_CHANGED));
}

/*************************************************/
/* Set the SDMA state flag */
void SetSDMAFlag(uint8_t value)
{
    bool_t retval = FALSE;
    uint32_t sema_val;
#if defined(ENABLE_READ_BACKOFF)
    unsigned int uDelay = 0;
    volatile unsigned int iterDelay;
#endif /* ENABLE_READ_BACKOFF */

    while(retval == FALSE) {

        WsfTaskLock();

        // Attempt to lock the ARM_SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];

        if (!sema_val) {
            SetSDMAFlagDirect(value);
            retval = TRUE;
            MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0x0;
        }

        WsfTaskUnlock();

#if defined(ENABLE_READ_BACKOFF)
        // If still waiting to write, back off so that SDMA can clear dirty bit.
        if (retval == FALSE) {
            if (uDelay == 0)
                uDelay = MIN_DELAY;
            else
                uDelay <<= 1;
            if (uDelay > MAX_DELAY)
                uDelay = MAX_DELAY;
            for (iterDelay = 0; iterDelay < uDelay; iterDelay ++);
        }
#endif /* ENABLE_READ_BACKOFF */
    }
}

/*************************************************/
/* Set the future MXC_WUT->cmp value to wakeup from Deep Sleep */
void SetSDMAWakeupTime(uint32_t value)
{
    bool_t retval = FALSE;
    uint32_t sema_val;
#if defined(ENABLE_READ_BACKOFF)
    unsigned int uDelay = 0;
    volatile unsigned int iterDelay;
#endif /* ENABLE_READ_BACKOFF */
    
    while(retval == FALSE) {

        WsfTaskLock();

        // Attempt to lock the ARM_SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];

        if (!sema_val) {
            sdma_shared_mem.state.sdma.wakeupTime = value;
            retval = TRUE;
            MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0x0;
        }

        WsfTaskUnlock();

#if defined(ENABLE_READ_BACKOFF)
        // If still waiting to write, back off so that SDMA can clear dirty bit.
        if (retval == FALSE) {
            if (uDelay == 0)
                uDelay = MIN_DELAY;
            else
                uDelay <<= 1;
            if (uDelay > MAX_DELAY)
                uDelay = MAX_DELAY;
            for (iterDelay = 0; iterDelay < uDelay; iterDelay ++);
        }
#endif /* ENABLE_READ_BACKOFF */
    }
}

/*************************************************/
/* Delay for the given number of ms */
void WUT_Delay_MS(uint32_t waitMs)
{
    /* assume WUT is already running */
    uint32_t  tmp = MXC_WUT->cnt;

    tmp += (waitMs * (WUT_FREQ / 1)
            + 500) / 1000 ;

    while(MXC_WUT->cnt < tmp) {}
}

/*************************************************/
/* Get the time delay expected on powerup */
uint32_t get_powerup_delay(uint32_t sleep_ticks)
{
    uint32_t ret;

    ret = POWERUP_DELAY(sleep_ticks);

    return ret;
}

/*************************************************/
/* Wait for an edge on the WUT->cnt */
void wait_wut_edge(void)
{
    uint32_t  tmp = MXC_WUT->cnt;
    while (tmp == MXC_WUT->cnt) {}
}

/*************************************************/
/* This will save DBB clock, WUT clock,
  arm WUT for wakeup from Backup mode */
void set_ahead_wakeup(uint32_t sleep_time)
{
    uint32_t u32WUTCount;
    uint32_t u32WUTTicks;
    
    /* save clocks */
    wait_wut_edge();
    wut_count = u32WUTCount = MXC_WUT->cnt;
    wut_snapshot = BbDrvGetCurrentTime();

    /* Calculate WUT ticks required to sleep for specified time. */
    u32WUTTicks = ((uint64_t)(sleep_time) * WUT_FREQ / BB_CLK_RATE_HZ);

    /* arm WUT for wakeup */
    SetSDMAWakeupTime(u32WUTCount + u32WUTTicks);
}

/*************************************************/
/* This will restore all clocks after wakeup */
void restore_dbb_clock(void)
{
    /* restore DBB clock from WUT */
    wait_wut_edge();

#if BB_CLK_RATE_HZ == 1600000
    // Avoid 64-mul/div
    MXC_WUT->preset = wut_snapshot + ((1 + MXC_WUT->cnt - wut_count)
                      * 5*5*5*5*5 / 64);
#else
    MXC_WUT->preset = wut_snapshot + (uint64_t)(MXC_WUT->cnt - wut_count + 1)
                      * BB_CLK_RATE_HZ / WUT_FREQ;
#endif

    MXC_WUT->reload =  1;  // arm DBB_CNT update on the next rising WUT clock
    wait_wut_edge();
    wait_wut_edge();
    /* Update WSF timers */
    WsfTimerUpdate( (MXC_WUT->cnt - wut_count) * 1000 / WUT_FREQ / WSF_MS_PER_TICK );
    MXC_WUT->reload = 0;
}

/*************************************************/
/* Wake the ARM from background mode with the WUT */
void wake_ARM(void)
{
#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    uint32_t cnt;
    uint32_t cn;

    // Nobody else should be messing with WUT.
    WsfCsEnter();

    /* If the wakeup timer is disabled. */
    cn = MXC_WUT->cn;

    if (1) { //((cn & MXC_F_WUT_CN_TEN) == 0) {
        // ARM must be in background mode.
        
        /* Prevent endless loop if this function is being called multiple times
        WUT fails to wake ARM if cmp margin is less than MXC_WUT_MIN_DELAY */
        cnt = MXC_WUT->cnt;
        if((MXC_WUT->cmp - cnt) > MXC_WUT_MIN_DELAY) {
            MXC_WUT->cmp = cnt + MXC_WUT_MIN_DELAY;
        }

        /* Reenable the WUT. */
        MXC_WUT->cn = (cn | MXC_F_WUT_CN_TEN);
    }

    WsfCsExit();
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */
}

/*************************************************/
/* Used by the startup code */
uint8_t get_ARM_flag(void)
{
    return arm_shared_mem.state.arm.fsm;
}

