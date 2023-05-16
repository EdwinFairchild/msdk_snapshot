/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Id: main.c 58927 2021-08-03 15:16:23Z nathan.goldstick $
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "max32665.h"
#include "max32665_sdma.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_trace.h"
#include "wsf_timer.h"
#include "lhci_api.h"
#include "sch_api.h"
#include "wakeup_sdma.h"
#include "bb_drv.h"
#include "bb_ble_drv.h"
#include "gcr_regs.h"
#include "sch_api_ble.h"
#include "pwrseq_regs.h"
#include "simo_regs.h"
#include "ll_debug.h"
#include "ipc_defs.h"
#include "sdma_regs.h"
#include "bb_ble_drv.h"
#include "bb_drv.h"
#include "ll_api.h"
#include "wut.h"
#include "hci_drv_sdma.h"

#define US_TO_BBTICKS(x)        (((x)*(BB_CLK_RATE_HZ/100000)+9)/10)
#define WUT_FREQ                (32768)   /* nominal */
#define wait_wut_edge()         {uint32_t tmp=MXC_WUT->cnt; while(tmp==MXC_WUT->cnt){} }

//#define BOD_DELAY_WAKEUP_THRESHOLD (3U * 1000000U / 625U) // 3ms
#define RESET_DELAY_TICKS (1 * 1000000U / 625U) // 1ms

/* If we take too long to be put into deepsleep,
 * interrupt the ARM again. */
#define ENABLE_PERSISTANT_DEEPSLEEP_INTERRUPTIONS

/* Defined in the startup file */
extern void SDMA_EnterStopMode(void);
extern void SDMA_EnterIdleMode(void);

extern void INT_IN_IRQHandler(void);

extern void TMRB_IRQHandler_clear(void);
extern void TMRB_IRQHandler_poll(void);
extern int TMRB_IRQPoll_haveOverflow(void);

/*! \brief  Stack initialization */
extern void StackInit(void);

extern void TMRB_WSF_Init(void);
extern void TMRB_sleepUs(uint32_t us);
extern void TMRB_delayUs(uint32_t us);

extern bool_t BbGetNextRelativeDueTime(int32_t *pi32RelDueTime);

void TMRB_IRQPoll(void);

const SDMACfg_t _sdma_cfg = {
    /*cbSize*/                  sizeof(SDMACfg_t),
    /*version*/                 0,
    /*flags*/                   SDMA_CFG_S_SLEEP_SYNC_ENABLED
};

SDMADiag_t _sdma_diag;

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS               6
#define WSF_BUF_SIZE                0x1048
#define MXC_DBB_EVT_STATUS          (*((volatile uint16_t*)(MXC_BASE_BTLE_DBB_CTRL + 0x084)))

/**
 * @brief Defined if the WUT is disabled by the ARM
 * immediately prior to entering background mode. */
//#define USE_BG_WUT_DISABLE

/**************************************************************************************************
  Local Variables
**************************************************************************************************/
uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] = {
    {  16,  8 },
    {  32,  4 },
    {  64,  4 },
    { 128,  4 },
    { 256,  4 },
    { 512,  4 }
};

/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0) {
        printf("%.*s",len,pBuf);
        return TRUE;
    }

    return FALSE;
}

/*************************************************************************************************/
void PalSysAssertTrap(void)
{
    // printf("Assertion detected\n");
    while(1) {}
}

void Catch_IRQHandler(void) __interrupt 
{
    /* Wake the ARM core to handle unserviced interrupts */
    wake_ARM();

    // Poll TMRB IRQ.
    __disable_irq();
    TMRB_IRQPoll();
    __enable_irq();
}

volatile bool_t tmrb_wsf_enable = FALSE;

void TMRB_WSF_Enable(void)
{
    tmrb_wsf_enable = TRUE;
    TMRB_WSF_Init();
}

/*************************************************************************************************/
static void TMRB_IRQEvent(void)  {
    ll_dbg_pin_assert(LL_DBG_PIN_SDMA_TMRB);

    if(tmrb_wsf_enable) {
        WsfTimerUpdate(WSF_MS_PER_TICK);
    }

    TMRB_IRQHandler_clear();

    ll_dbg_pin_deassert(LL_DBG_PIN_SDMA_TMRB);
}

void TMRB_IRQHandler(void) __interrupt {
    __disable_irq();

    TMRB_IRQEvent();

    __enable_irq();
}

void TMRB_IRQPoll(void) {
    if (TMRB_IRQPoll_haveOverflow()) {
        TMRB_IRQEvent();
    }
}

/*************************************************************************************************/
void TMR0_IRQHandler(void) __interrupt {

}

/*************************************************************************************************/
/*!
 *  \brief  Initialize WSF.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void WsfInitSh(void)
#else /* __CROSSWORKS */
static void WsfInit(void)
#endif /* __CROSSWORKS */
{

    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));
    WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);

#if WSF_TRACE_ENABLED
    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
#endif

}
#ifdef __CROSSWORKS
#pragma codeseg(default)
static void WsfInit(void)
{
    _sdma_diag.pfnLastShCall = ((size_t)&WsfInitSh << 1);
    WsfInitSh();
}
#endif /* __CROSSWORKS */

#define DELAY_SLEEP_US          50         
/*************************************************************************************************/
#ifdef DELAY_SLEEP_US
void WUT_sleepUs(uint32_t us)
{
    /* Have to delay before sleeping */
    if(us > DELAY_SLEEP_US) {
        us -= DELAY_SLEEP_US;
        TMRB_delayUs(DELAY_SLEEP_US);
    } else {
        TMRB_delayUs(us);
        return;
    }

    ll_dbg_pin_assert(LL_DBG_PIN_SDMA_SLEEP);
    TMRB_sleepUs(us);
    ll_dbg_pin_deassert(LL_DBG_PIN_SDMA_SLEEP);
}
#else /* DELAY_SLEEP_US */
void WUT_sleepUs(uint32_t us)
{
    TMRB_delayUs(us);
}
#endif /* DELAY_SLEEP_US */

/*************************************************************************************************/
extern void BTLE_TX_DONE_IRQHandler(void);
extern void BTLE_RX_RCVD_IRQHandler(void);
extern void BTLE_RX_ENG_DET_IRQHandler(void);
extern void BTLE_SFD_DET_IRQHandler(void);
extern void BTLE_SFD_TO_IRQHandler(void);
extern void BTLE_RFFE_SPIM_IRQHandler(void);
extern void BTLE_GP_EVENT_IRQHandler(void);
void BTLE_IRQHandler(void)
{
    volatile uint16_t irq_status;

    __disable_irq();

    irq_status = MXC_DBB_EVT_STATUS;

    if(irq_status & 0x1) {
        BTLE_TX_DONE_IRQHandler();
    }
    if(irq_status & 0x2) {
        BTLE_RX_RCVD_IRQHandler();
    }
    if(irq_status & 0x4) {
        BTLE_RX_ENG_DET_IRQHandler();
    }
    if(irq_status & 0x8) {
        BTLE_SFD_DET_IRQHandler();
    }
    if(irq_status & 0x10) {
        BTLE_SFD_TO_IRQHandler();
    }
    if(irq_status & 0x20) {
        BTLE_RFFE_SPIM_IRQHandler();
    }
    if(irq_status & 0x80) {
        BTLE_GP_EVENT_IRQHandler();
    }

    // Poll TMRB IRQ.
    TMRB_IRQPoll();

    __enable_irq();
}

/*************************************************************************************************/
void main(void)
{
    uint8_t armFlag;
    uint8_t armOldFlag;
    int bChooseReset;
#ifdef RESET_DELAY_TICKS
    uint32_t u32ChooseResetTime;
#endif /* RESET_DELAY_TICKS */
    int bStartReset;
    int bNeedWakeup;
    int bHaveBackground;
    uint32_t nBootCountNext;
    uint32_t nSleepTickNext;

    uint32_t  nextDbbEventDue, currentTime;
    bool_t dueValid;
    uint32_t sleep_ticks;
    uint32_t powerup_ticks;

    int32_t i32RelDueTime;
    bool_t bHaveRelDueTime;

    /* Set the SDMA flag to signal to the ARM that the SDMA is in the INIT state */
    SetSDMAFlag(SDMA_FLAG_INIT);

    /* The SDMA makes the decision as to whether */
    /* the part goes into background or deepsleep mode. */
    bChooseReset = FALSE;

    /* Determine if the ARM is in the INIT or SLEEP state */
    armFlag = GetARMFlag();
    armOldFlag = armFlag;
    if(armFlag & ARM_FLAG_STATE)  {
        goto restart_from_sleep;
    }

    // printf("***** ME14 SDMA LHCI *****\r\n");
    // printf("WSF_MS_PER_TICK = %d\r\n", WSF_MS_PER_TICK);

    /* Initialize Radio */
    WsfInit();
    StackInit();
    TMRB_WSF_Enable();

    // printf("SDMA Setup complete\r\n");

    // Not in background mode.
    bHaveBackground = FALSE;

    // Signal that we're ready
    interrupt_ARM();

    while (1) {
        wsfOsDispatcher();

#if (MXC_MIN_CHIP_REVISION <= 1)
        if(MXC_GCR->revision == 0xA1) {
            BTLE_IRQHandler();
        }
#endif /* (MXC_MIN_CHIP_REVISION <= 1) */

#ifdef ENABLE_RESTART_FROM_SLEEP
        /* WSF not busy ? */
        armFlag = GetARMFlag();
        if (_sdma_cfg.flags == SDMA_CFG_S_SLEEP_SYNC_ENABLED)
        {
            armOldFlag = armFlag;
        }
        else if (((armFlag ^ armOldFlag) & ARM_FLAG_CHANGED) == ARM_FLAG_CHANGED)
        {
            armOldFlag = armFlag;
            armFlag &= ARM_FLAG_STATE;
        }
        else
        {
            armOldFlag = armFlag;
            armFlag = 0;
        }

        if (armFlag & ARM_FLAG_STATE)
        {
            // Only need to send a wakeup event if the ARM is waiting.
            // The arm signals this when it transitions to the sleep state,
            // even if it makes this transition from the sleep state.
            bNeedWakeup = TRUE;

            // If the ARM has signaled us, we are not in background mode.
            bHaveBackground = FALSE;
        }

        /* Is the SDMA ready to sleep? as well as the ARM? */
        if (wsfOsReadyToSleep()) {

            /* Get the time pending to the next event in BLE ticks. */
            dueValid = SchBleGetNextDueTime(&nextDbbEventDue);
            currentTime = BbDrvGetCurrentTime();
            sleep_ticks = nextDbbEventDue - currentTime;

            /* Reschedule events that are in the past */
            if (dueValid && ((int)sleep_ticks < -10000)) {
                SchHandler(0, NULL);
                dueValid = FALSE;
            }

            /* Then, determine whether the schedule leaves
               enough time for the SDMA to be restarted? */
            if (dueValid && (int)sleep_ticks > 0 ) {

                /* need extra time to restore voltages from sleep */
                powerup_ticks = get_powerup_delay(sleep_ticks);

                /* have enough time to sleep ? */
                if ( (int)sleep_ticks >
                        US_TO_DBBTICKS(WAKEUP_TIME_US + MIN_DEEPSLEEP_US)
                        + powerup_ticks) {
                    if (!bChooseReset)
                    {
#ifdef RESET_DELAY_TICKS
                        u32ChooseResetTime = currentTime;
#endif /* RESET_DELAY_TICKS */
                        bChooseReset = TRUE;
                    }
                }
            }

            /* Don't reset just as soon as possible. Wait a tiny bit to avoid waking up too soon. */
            if (bChooseReset != FALSE)
            {
#ifdef RESET_DELAY_TICKS
                if ((currentTime - u32ChooseResetTime) > RESET_DELAY_TICKS) {
                    bStartReset = TRUE;
                }
#else /* RESET_DELAY_TICKS */
                bStartReset = TRUE;
#endif /* RESET_DELAY_TICKS */
            }

            /* If the ARM is asleep, and there is enough time for the SDMA to be
               shutdown, wake the ARM so that it can shut the SDMA down here. */
            if (bHaveBackground && bStartReset) {
#ifdef BOD_DELAY_WAKEUP_THRESHOLD
                bHaveRelDueTime = BbGetNextRelativeDueTime(&i32RelDueTime);
                if (!bHaveRelDueTime ||
                        (i32RelDueTime > BOD_DELAY_WAKEUP_THRESHOLD)) {
                    //bHaveBackground = FALSE;
                    wake_ARM();
                    interrupt_ARM();
                }
#endif /* BOD_DELAY_WAKEUP_THRESHOLD */
            }

            /* Has the ARM requested to be put to sleep? */
            if (bNeedWakeup) {

                /* If we need to reset the part, */
                if ( bStartReset != FALSE) {
                    tmrb_wsf_enable = FALSE;
                    TMRB_Disable();

                    /* Go to Sleep here */
                    /* save DBB, WUT clocks, arm WUT for wakeup */
                    sleep_ticks = nextDbbEventDue - BbDrvGetCurrentTime();
                    set_ahead_wakeup( sleep_ticks - powerup_ticks - US_TO_DBBTICKS(WAKEUP_TIME_US) );

                    /* unschedule next operation */
                    SchSleep();

                    BbDrvDisable();

                    ll_dbg_pin_assert(LL_DBG_PIN_SDMA_SLEEP);

                    /* Divide down the systme clock to prevent current spikes when the
                    ARM wakes from background */
                    MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV8;

                    /* Signal to the ARM that we're ready to sleep */
                    SetSDMAFlag(SDMA_FLAG_CHANGED | SDMA_FLAG_SLEEPING);

                    /* If we are going into deepsleep, we can not be in background mode. */
                    bHaveBackground = FALSE;

                    /* Notify ARM of pending message. */
                    if (bNeedWakeup)
                    {
                        bNeedWakeup = 0;
                        interrupt_ARM();
                    }

                    /* ARM core will disable SDMA */
                    _sdma_diag.nSleepTick = 0;
                    while(1)
                    {
                        // Increment diaglostic counter,
                        // while being careful not to ever set to zero.
                        nSleepTickNext = _sdma_diag.nSleepTick + 1;
                        if (nSleepTickNext != 0) _sdma_diag.nSleepTick = nSleepTickNext;
#ifdef ENABLE_PERSISTANT_DEEPSLEEP_INTERRUPTIONS
                        if ((nSleepTickNext & 0xFFFF) == 0) {
                            interrupt_ARM();
                            SetSDMAFlag(SDMA_FLAG_CHANGED | SDMA_FLAG_SLEEPING);
                        }
#endif /* ENABLE_PERSISTANT_DEEPSLEEP_INTERRUPTIONS */
                    }
    restart_from_sleep:
                    /* will jump here when restarting after sleep */

                    ll_dbg_pin_deassert(LL_DBG_PIN_SDMA_SLEEP);

                    /* Restore bNeedWakeup. Was cleared immediately before entering
                    * while loop, but is not explicitly set on entry to main. */
                    bNeedWakeup = 0;

                    /* Clear diagnostic sleep tick counter. */
                    _sdma_diag.nSleepTick = 0;

                    /* Set diagnostic reboot counter. */
                    nBootCountNext = _sdma_diag.nBootCount + 1;
                    if (nBootCountNext != 0) _sdma_diag.nBootCount = nBootCountNext;

                    /* Disable TMRB until we finish BLE init */
                    tmrb_wsf_enable = FALSE;
                    TMRB_Disable();

                    SetSDMAFlag(SDMA_FLAG_CHANGED | SDMA_FLAG_RUNNING);
                    interrupt_ARM();

                    BbDrvEnable();

                    /* Restore BLE hardware state */
                    BbDrvInitWake();

                    /* Restore BLE clock */
                    restore_dbb_clock();

                    /* Enable TMRB for WSF timer */
                    TMRB_WSF_Enable();

                    /* Re schedule the next operation */
                    SchWake();

                    // Enable interrupt from ARM
                    MXC_SDMA->int_in_ie = 1;

                    INT_IN_IRQHandler();

                } else if (bChooseReset == FALSE) {
                    SetSDMAFlag(SDMA_FLAG_CHANGED | SDMA_FLAG_RUNNING);

                    // Let ARM know we've made our decision.
                    if (bNeedWakeup)
                    {
                        bNeedWakeup = 0;
                        interrupt_ARM();
                    }
                    
                    // We will soon be in background mode.
                    bHaveBackground = TRUE;
                }
                // if we choose to reset, but not just yet, just don't do anything.
            }
        }
#else /* ENABLE_RESTART_FROM_SLEEP */

restart_from_sleep:
        (void)0; /*NOP*/
#endif /* ENABLE_RESTART_FROM_SLEEP */
    }

}
