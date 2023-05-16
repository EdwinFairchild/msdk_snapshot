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
 * $Date: 2021-09-20 09:03:42 -0500 (Mon, 20 Sep 2021) $
 * $Revision: 59800 $
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mxc_config.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_cs.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "app_ui.h"
#include "fit/fit_api.h"
#include "app_ui.h"
#include "hci_vs.h"
#include "hci_core.h"
#include "hci_drv_sdma.h"
#include "ipc_defs.h"
#include "pb.h"
#include "tmr.h"
#include "wakeup.h"
#include "uart.h"
#include "sch_api.h"
#include "wut.h"
#include "rtc.h"
#include "simo.h"
#include "board.h"
#include "gcr_regs.h"
#include "sch_api_ble.h"
#include "lp.h"
#include "bb_drv.h"
#include "led.h"
#include "tmr_utils.h"
#include "pal_rtc.h"
#include "demo_config.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define WAKEUP_32M_US       (3200)
#define WUT_FREQ            (32768)
#define US_TO_WUTTICKS(x)   (((x)*WUT_FREQ)/1000000)
#define WUT_MIN_TICKS       (10)

// OST_TIMER defined in demo_config.h

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

/* Definitions for push button handling */
#define BUTTON0_TMR         MXC_TMR0
#define BUTTON1_TMR         MXC_TMR1
#define BUTTON_SHORT_MS     200
#define BUTTON_MED_MS       500
#define BUTTON_LONG_MS      1000

/**
 * @brief Disable SDMA interrupts when going into background mode. */
//#define ENABLE_BACKGROUND_SDMA_INT_WORKAROUND

/**
 * @brief Defined if the WUT is disabled by the ARM
 * immediately prior to entering background mode. */
//#define USE_BG_WUT_DISABLE

#define ENABLE_BACKGROUND_HARDFAULT_WORKAROUND

/**
 * @brief How many WUT ticks to delay entering Deep Sleep after being given the go ahead by the SDMA. */
#define DEEPSLEEP_DELAY (20)

/**************************************************************************************************
  Data Types
**************************************************************************************************/

#ifdef ENABLE_SDMA
typedef struct {
    /**
     * @brief Maintains the most recently read SDMA flag. Used to detect
     * state transitions in the SDMA. */
    uint8_t prevSdmaFlag;

    /**
     * @brief Set whenever the SDMA signals a new state. */
    int8_t bHaveSignaledFlag;

    /**
     * @brief The WUT counter value when the SDMA signals a new state. */
    uint32_t u32FlagWUTTimestamp;

} SDMASleepState_t;
#else /* ENABLE_SDMA */
typedef void SDMASleepState_t;
#endif /* ENABLE_SDMA */

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

uint32_t SystemHeapSize = WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE / 4];
uint32_t SystemHeapStart;

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

// *INDENT-OFF*
/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] = {
    {  16,  8 },
    {  32,  4 },
    {  64,  4 },
    { 128,  4 },
    { 256,  4 },
    { 512,  4 }
};
// *INDENT-ON*

#ifdef ENABLE_SDMA
static SDMASleepState_t stSDMASleepState;
#endif /* ENABLE_SDMA */

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitFit(void);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
static bool_t myTrace(const uint8_t * pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0) {
        fwrite(pBuf, len, 1, stdout);
        return TRUE;
    }

    return FALSE;
}

/*************************************************/
/* Get the time delay expected on powerup */
uint32_t get_powerup_delay(uint32_t wait_ticks)
{
    uint32_t ret;

    ret = US_TO_WUTTICKS(WAKEUP_32M_US + (wait_ticks / 32));

    return ret;
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize WSF.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void WsfInit(void)
{
    uint32_t bytesUsed;

    /* setup the systick for 1MS timer */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |=
            (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk |
            SysTick_CTRL_TICKINT_Msk);

    WsfTimerInit();

    SystemHeapStart = (uint32_t) & SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));
    printf("SystemHeapStart = 0x%x\n", SystemHeapStart);
    printf("SystemHeapSize = 0x%x\n", SystemHeapSize);
    bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    printf("bytesUsed = 0x%x\n", bytesUsed);

    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
}

/*
 * In two-chip solutions, setting the address must wait until the HCI interface is initialized.
 * This handler can also catch other Application events, but none are currently implemented.
 * See ble-profiles/sources/apps/app/common/app_ui.c for further details.
 *
 */
void SetAddress(uint8_t event)
{
    uint8_t bdAddr[6] = { 0x02, 0x02, 0x44, 0x8B, 0x05, 0x00 };

    switch (event) {
    case APP_UI_RESET_CMPL:
        printf("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X\n",
                bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1],
                bdAddr[0]);
        HciVsSetBdAddr(bdAddr);
        break;
    default:
        break;
    }
}

#ifdef NORMAL_SLEEP
/**
 * @brief  Wakeup timer hook.
 */
void WUT_IRQHook(void *pHookArg)
{
    // This argument isn't used for this application.
    // Explicitly ignore.
    (void)pHookArg;

    bHaveWUTEvent = -1;
}
#endif /* NORMAL_SLEEP */

/*************************************************************************************************/
void HandleButton(int button)
{
    mxc_tmr_regs_t *button_tmr = MXC_TMR_GET_TMR(button);

    // Check if rising or falling
    if (PB_Get(button)) {
        // Start timer
        TMR_Enable(button_tmr);
    } else {
        uint32_t time;
        tmr_unit_t unit;

        // Get the elapsed time since the button was pressed
        TMR_GetTime(button_tmr, TMR_GetCount(button_tmr), &time, &unit);
        TMR_Disable(button_tmr);
        TMR_SetCount(button_tmr, 0);

        if (unit == TMR_UNIT_NANOSEC) {
            time /= 1000000;
        } else if (unit == TMR_UNIT_MICROSEC) {
            time /= 1000;
        }

        if (time < BUTTON_SHORT_MS) {
            AppUiBtnTest(button ? APP_UI_BTN_2_SHORT : APP_UI_BTN_1_SHORT);
        } else if (time < BUTTON_MED_MS) {
            AppUiBtnTest(button ? APP_UI_BTN_2_MED : APP_UI_BTN_1_MED);
        } else if (time < BUTTON_LONG_MS) {
            AppUiBtnTest(button ? APP_UI_BTN_2_LONG : APP_UI_BTN_1_LONG);
        } else {
            AppUiBtnTest(button ? APP_UI_BTN_2_EX_LONG : APP_UI_BTN_1_EX_LONG);
        }
    }
}

/*************************************************************************************************/
void Button0Pressed(void *arg)
{
    HandleButton(0);
}

/*************************************************************************************************/
void Button1Pressed(void *arg)
{
    HandleButton(1);
}

/*************************************************************************************************/
#ifndef ENABLE_SDMA
void Sleep(void)
{
    int bGoBackToSleep = -1;

    WsfTaskLock();

#ifdef DEEPSLEEP_LED
    LED_On(DEEPSLEEP_LED);
#endif /* DEEPSLEEP_LED */

    /* WSF and UART are not busy ? */
    if (wsfOsReadyToSleep() &&
            UART_PrepForSleep(MXC_UART_GET_UART(CONSOLE_UART)) == E_NO_ERROR) {
        /* get next due time and sleep time */
        uint32_t nextDbbEventDue;
        bool_t dueValid = SchBleGetNextDueTime(&nextDbbEventDue);
        int sleep_ticks = nextDbbEventDue - BbDrvGetCurrentTime();

        /* timing valid ? */
        if (dueValid && (sleep_ticks > 0)) {
            /* have enough time to deep sleep ? */
            if (sleep_ticks >
                    US_TO_BBTICKS(DS_WAKEUP_TIME_US + MIN_DEEPSLEEP_US)) {

                /* Stop SysTick */
                SysTick->CTRL = 0;

                /* save DBB, WUT clocks, arm WUT for wakeup */
                WUT_SetWakeup(sleep_ticks - US_TO_BBTICKS(DS_WAKEUP_TIME_US));

                /* unschedule next BLE operation */
                SchSleep();

                /* Only count WUT interrupts that follow. */
                bHaveWUTEvent = 0;

                /* Enter the appropriate sleep mode. */
                do {
#ifdef NORMAL_SLEEP
                    // Enter normal sleep.
                    LP_EnterSleepMode();

                    // If a wakeup interrupt has happened.
                    if (bHaveWUTEvent) {
                        bGoBackToSleep = 0;
                    }
#else /* NORMAL_SLEEP */
                    WsfCsEnter();
                    
                    /* enterDeepSleep mode */
                    EnterDeepsleep();

                    /* initial restore */
                    ExitDeepsleep();

                    WsfCsExit();
                    
                    // Leave the sleep function regardless of
                    // how much time is remaining.
                    // Only wake up events can bring us here.
                    // Normal interrupts will not bring us out of deepsleep.
                    bGoBackToSleep = 0;
#endif /* NORMAL_SLEEP */
                } while (bGoBackToSleep);

                /* Restore BLE hardware state */
                BbDrvInitWake();

                /* Restore BB clock */
                WsfTaskLock();
                WUT_RestoreBBClock(BB_CLK_RATE_HZ);
                WsfTimerUpdate((WUT_GetSleepTicks() * 1000) /
                        SYS_WUT_GetFreq() / WSF_MS_PER_TICK);
                WsfTaskUnlock();

                /* setup the systick */
                SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
                SysTick->VAL = 0;
                SysTick->CTRL |=
                        (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk |
                        SysTick_CTRL_TICKINT_Msk);

                /* Re-schedule next BLE operation */
                SchWake();

                /* have enough time to sleep ? */
            } else {
                LP_EnterSleepMode();
            }
            /* Nothing scheduled, wait for interrupt */
        } else {
            LP_EnterSleepMode();
        }
    }                                     /* if(not busy) */

#ifdef DEEPSLEEP_LED
    LED_Off(DEEPSLEEP_LED);
#endif /* DEEPSLEEP_LED */

    WsfTaskUnlock();
}
#else /* ENABLE_SDMA */
#define Sleep(...) SDMASleep(__VA_ARGS__)
#endif /* ENABLE_SDMA */

/*************************************************************************************************/
#ifdef ENABLE_SDMA
void SDMASleep(void)
{
    int bGoBackToSleep = -1;

    uint8_t curSdmaFlag;
    uint8_t signaledSdmaFlag;
    uint8_t sdmaFlagChanged;

    uint32_t u32sdmaWakeupCount;
    uint32_t u32sdmaWakeupTime;
    uint32_t u32sdmaSleepTicks;

    WsfTaskLock();

    /* If the ARM core is idle, attempt to go to sleep or request to go to sleep. */
    if (wsfOsReadyToSleep() &&
            UART_PrepForSleep(MXC_UART_GET_UART(CONSOLE_UART)) == E_NO_ERROR) {

        /* Check whether the SDMA has already told the ARM its decision. */
        if (stSDMASleepState.bHaveSignaledFlag == 0) {

            /* Signal to the SDMA that it is being put to sleep, not being reset outright. */
            SDMASetARMFlag(ARM_FLAG_SLEEP | ARM_FLAG_CHANGED);

            /* Check for a state change from the SDMA. */
            curSdmaFlag = SDMAGetSDMAFlag();
            sdmaFlagChanged =
                    ((curSdmaFlag ^ stSDMASleepState.prevSdmaFlag) &
                    SDMA_FLAG_CHANGED);

            // Determine what state transitions have been signaled.
            if (sdmaFlagChanged == SDMA_FLAG_CHANGED) {
                signaledSdmaFlag = (curSdmaFlag & SDMA_FLAG_STATE);
            } else if ((curSdmaFlag & SDMA_FLAG_STATE) == SDMA_FLAG_SLEEPING) {
                signaledSdmaFlag = SDMA_FLAG_SLEEPING;
            } else {
                signaledSdmaFlag = 0;
            }

            // Save current state for future reference.
            stSDMASleepState.prevSdmaFlag = curSdmaFlag;
            if (sdmaFlagChanged == SDMA_FLAG_CHANGED) {
                stSDMASleepState.bHaveSignaledFlag = TRUE;
                u32sdmaWakeupCount = WUT_GetCount();
                stSDMASleepState.u32FlagWUTTimestamp = u32sdmaWakeupCount;
            }
        }
        else {
            signaledSdmaFlag = (stSDMASleepState.prevSdmaFlag & SDMA_FLAG_STATE);
        }

        // If enough ticks haven't passed till we go to sleep,
        // then just disregard signaled flag for now.
        if (stSDMASleepState.bHaveSignaledFlag != 0) {
            u32sdmaWakeupCount = WUT_GetCount();
            if ((u32sdmaWakeupCount - stSDMASleepState.u32FlagWUTTimestamp) <
                    DEEPSLEEP_DELAY) {
                signaledSdmaFlag = 0;
            }
        }

        // If the SDMA has responded to an offer to sleep from the ARM.
        if ((signaledSdmaFlag == SDMA_FLAG_SLEEPING) ||
                (signaledSdmaFlag == SDMA_FLAG_RUNNING)) {

            /* Signaled SDMA flag is being consumed. */
            stSDMASleepState.bHaveSignaledFlag = 0;

            /* Store the wakeup timer value to restore the WSF timer once we wakeup */
            WUT_Store();

            /* Stop the Systick interrupt */
            SysTick->CTRL = 0;

            /* Set the wakeup compare value from the SDMA (deepsleep only) */
            u32sdmaWakeupCount = WUT_GetCount();
            if (signaledSdmaFlag == SDMA_FLAG_SLEEPING)
                u32sdmaWakeupTime = SDMAGetWakeupTime();
            else
                u32sdmaWakeupTime = u32sdmaWakeupCount - 1;
            WUT_SetCompare(u32sdmaWakeupTime);

            /* Only count WUT interrupts that follow. */
            bHaveWUTEvent = 0;

            /* Enter the appropriate sleep mode. */
            do {
                if ((signaledSdmaFlag == SDMA_FLAG_SLEEPING) &&
                        ((signed)(u32sdmaWakeupTime - u32sdmaWakeupCount) <
                        WUT_MIN_TICKS)) {
                    // printf("Missed wakeup time by %u ticks\r\n", (u32sdmaWakeupCount + WUT_MIN_TICKS + 1) - u32sdmaWakeupTime);
                    while (((signed)(u32sdmaWakeupTime -
                                            u32sdmaWakeupCount)) > 0) {
                        u32sdmaWakeupTime = WUT_GetCompare();
                        u32sdmaWakeupCount = WUT_GetCount();
                    }

                    WUT_IntClear();
                    NVIC_ClearPendingIRQ(WUT_IRQn);
                } else if ((signaledSdmaFlag == SDMA_FLAG_SLEEPING) &&
                        ((signed)(u32sdmaWakeupTime - (u32sdmaWakeupCount +
                        get_powerup_delay(u32sdmaWakeupTime - u32sdmaWakeupCount))) < 0)) {
#if 1
                    LP_EnterSleepMode();
#else
                    while (((signed)(u32sdmaWakeupTime -
                                            u32sdmaWakeupCount)) > 0) {
                        u32sdmaWakeupTime = WUT_GetCompare();
                        u32sdmaWakeupCount = WUT_GetCount();
                    }
#endif

                    WUT_IntClear();
                    NVIC_ClearPendingIRQ(WUT_IRQn);
                } else {
                    if (signaledSdmaFlag == SDMA_FLAG_SLEEPING) {
#ifdef DEEPSLEEP_LED
                        LED_On(DEEPSLEEP_LED);
#endif /* DEEPSLEEP_LED */
                    } else {
#ifdef BACKGROUND_LED
                        LED_On(BACKGROUND_LED);
#endif /* BACKGROUND_LED */
                    }

                    if (signaledSdmaFlag == SDMA_FLAG_SLEEPING) {
                        /* Enter Deep Sleep if the SDMA is sleeping */
                        EnterDeepsleepSDMA();
                        ExitDeepsleepSDMA();
                    } else {
#if 0
                        /* Enter Background mode if SDMA stays awake */
#ifdef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
                        /* SDMA will get stuck waiting for ARM to clear GPIO interrupt
                        * flag so disable here and poll GPIO on next wakeup. */
                        UserPinSetEnableInterrupt(TRUE);
                        NVIC_DisableIRQ((IRQn_Type)
                                MXC_GPIO_GET_IRQ(user_pin[USER_PIN_IN].port));
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifdef ENABLE_BACKGROUND_SDMA_INT_WORKAROUND
                        NVIC_DisableIRQ((IRQn_Type) SDMA_IRQn);
#endif /* ENABLE_BACKGROUND_SDMA_INT_WORKAROUND */

#ifdef ENABLE_BG_WUT_INT_DISABLE
                        /* Don't handle WUT interrupts from here on out. */
                        // TODO: Assert that there is enough time to enter background.
                        NVIC_DisableIRQ((IRQn_Type) WUT_IRQn);
#endif /* ENABLE_BG_WUT_INT_DISABLE */

                        // Don't touch WUT if background mode isn't being used.
                        // It isn't required.
#ifndef ENABLE_BACKGROUND_SDMA_INT_WORKAROUND
                        // Disable WUT before entering background mode.
                        // SDMA will reenable.
#ifdef USE_BG_WUT_DISABLE
                        WUT_Disable();
#else /* USE_BG_WUT_DISABLE */
                        WUT_SetCompare(MXC_WUT->cnt - 1);
#endif /* USE_BG_WUT_DISABLE */
#endif /* ENABLE_BACKGROUND_SDMA_INT_WORKAROUND */

                        /* Enter Background mode if the SDMA is running */
                        EnterBackground();
                        ExitBackground();

#ifdef ENABLE_BACKGROUND_SDMA_INT_WORKAROUND
                        NVIC_EnableIRQ((IRQn_Type) SDMA_IRQn);
#endif /* ENABLE_BACKGROUND_SDMA_INT_WORKAROUND */

#ifdef ENABLE_BG_WUT_INT_DISABLE
                        /* Handle pending WUT interrupts after leaving ExitBackground() */
                        NVIC_EnableIRQ((IRQn_Type) WUT_IRQn);
#endif /* ENABLE_BG_WUT_INT_DISABLE */

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
                        /* Check to see if GPIO level has changed on wakeup.
                        * Reenable GPIO interrupts going forward where required. */
                        UserEventPoll();
                        NVIC_EnableIRQ((IRQn_Type)
                                MXC_GPIO_GET_IRQ(user_pin[USER_PIN_IN].port));
                        UserPinSetEnableInterrupt(TRUE);
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */
#endif /* 0 */
                    }

                    if (signaledSdmaFlag == SDMA_FLAG_SLEEPING) {
#ifdef DEEPSLEEP_LED
                        LED_Off(DEEPSLEEP_LED);
#endif /* DEEPSLEEP_LED */
                    } else {
#ifdef BACKGROUND_LED
                        LED_Off(BACKGROUND_LED);
#endif /* BACKGROUND_LED */
                    }

                    // Leave the sleep function regardless of
                    // how much time is remaining. Wake SDMA
                    // back up and allow SDMA to decide whether
                    // returning to sleep makes sense.
                    bGoBackToSleep = 0;
                }
            } while (bGoBackToSleep);

            /* If the SDMA was stopped, restart here. */
            if (signaledSdmaFlag == SDMA_FLAG_SLEEPING) {
                SDMARestart();
            }

            /* Restore the WSF timer from sleep */
            u32sdmaSleepTicks = WUT_GetSleepTicks();
            WsfTimerUpdate((u32sdmaSleepTicks * 1000) / SYS_WUT_GetFreq() /
                    WSF_MS_PER_TICK);

            /* setup the systick */
            SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
            SysTick->VAL = 0;
            SysTick->CTRL |=
                    (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk |
                    SysTick_CTRL_TICKINT_Msk);
        }
    }

    WsfTaskUnlock();
}
#endif /* ENABLE_SDMA */

/*************************************************************************************************/
#ifdef ENABLE_SDMA
void SDMASleepStateInit()
{
    stSDMASleepState.prevSdmaFlag = SDMA_FLAG_INIT;
}
#else
#define SDMASleepStateInit(pstSDMASleepState)
#endif /* ENABLE_SDMA */

/*************************************************************************************************/
#if defined(ENABLE_SDMA) && defined(ENABLE_SDMA_ERROR_REPORTING)
/**
 * @brief Checks whether the SDMA was able to be initialized.
 *
 * @return A non-zero error code if something went wrong. */
int SDMACheckForError(void)
{
    uint8_t sdmaFlagState;
    uint8_t sdmaError;

    // Check if the SDMA is still initializing.
    sdmaFlagState = (stSDMASleepState.sdmaFlag & SDMA_FLAG_STATE);
    if (sdmaFlagState == SDMA_FLAG_INIT) {
        stSDMASleepState.sdmaFlag = SDMAGetSDMAFlag();
    }

    // Return any errors.
    sdmaError = 0;
    switch (sdmaFlagState) {
    case SDMA_FLAG_ENOMEM:
        sdmaError = sdmaFlagState;
        break;
    }

    // Also make sure the SDMA is operating within
    // the bounds of its assigned memory region.
    if (sdmaError == 0)
        sdmaError = SDMACheckIP();

    return (sdmaError);
}
#else /* ENABLE_SDMA_ERROR_REPORTING && ENABLE_SDMA */
#define SDMACheckForError() (0)
#endif /* ENABLE_SDMA_ERROR_REPORTING && ENABLE_SDMA */

/*************************************************************************************************/
/*!
 *  \fn     trimRTC
 *
 *  \brief  Trim the internal RTC load capacitors to increase the 32 kHz accuracy.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void trimRTC(void)
{
    int freq;

    /* Initialize VREGO_D and BTLELDOCN for 32 MHz crystal */
    MXC_SIMO->vrego_d = (0x50 << MXC_F_SIMO_VREGO_D_VSETD_POS);
    while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYD)) {
    }

    MXC_GCR->btleldocn = 0xD5;

    TMR_Delay(OST_TIMER, MSEC(1000), 0);

    /* Select the 32.00 MHz crystal oscillator (XO) as the system clock source */
    if (SYS_Clock_Select(SYS_CLOCK_XTAL32M, NULL) != E_NO_ERROR) {
        printf("Clock switch unsuccessful\n");
        return;
    } else {
        Console_Init();
    }

    sys_cfg_rtc_t sys_cfg = NULL;         /*  No Specific system configuration object */

    if (RTC_Init(MXC_RTC, 0, 0, &sys_cfg) != E_NO_ERROR) {
        printf("Failed RTC_Setup().\n");
        return;
    }

    /* Clear out any previous configuration */
    RTC_DisableTimeofdayInterrupt(MXC_RTC);
    RTC_DisableSubsecondInterrupt(MXC_RTC);
    RTC_ClearFlags(RTC_GetFlags());

    if (RTC_EnableRTCE(MXC_RTC) != E_NO_ERROR) {
        printf("Failed RTC_EnableRTCE()\n");
        return;
    }

    /* Run the trim, save the freq */
    freq = RTC_LoadTrim();
    if (freq < 0) {
        printf("Failed to trim RTC %d\n", freq);
    } else {
        printf("RTC Trimmed to %d Hz\n", freq);
    }

    RTC_DisableRTCE(MXC_RTC);

    /* Apply the freq */
    SYS_WUT_SetFreq(freq);
}

/*************************************************************************************************/
/*!
 *  \fn     main
 *
 *  \brief  Entry point for demo software.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
int main(void)
{
    wut_cfg_t wut_cfg;

    tmr_cfg_t button_config;

#ifdef ENABLE_SDMA
#ifdef ENABLE_SDMA_ERROR_REPORTING
    int sdmaError;
#endif /* ENABLE_SDMA_ERROR_REPORTING */
#endif /* ENABLE_SDMA */

    printf("\n\n***** MAX32665 BLE Fitness Profile, Deep Sleep *****\n");

    printf(" A 10sec delay before starting. (..to allow re-flash)\n");

    TMR_Delay(OST_TIMER, INTERVAL_TIME_OST_STARTUP, 0);

    /* Trim 32 kHz trim */
    trimRTC();
    while (UART_PrepForSleep(MXC_UART_GET_UART(CONSOLE_UART)) != E_NO_ERROR) {
    }

#ifdef USE_96M_CLOCK
    /* Run from the 96MHz oscillator */
    switchTo96M();
#else /* USE_96M_CLOCK */
    /* Run from the 60MHz oscillator */
    switchToHIRC();
#endif /* USE_96M_CLOCK */

    Console_Init();

    printf("SystemCoreClock = %d\n", SystemCoreClock);

    /* Make sure VRego C uses run voltage. */
    SIMO_setVregO_C(RUN_VOLTAGE);
    while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYC)) {
    }

#if (BACKUP_MODE == 1)
    printf("Entering backup mode instead of deep sleep\n");
#endif

    /* Initialize Wakeup timer */
    WUT_Init(WUT_PRES_1);

    wut_cfg.mode = WUT_MODE_COMPARE;
    wut_cfg.cmp_cnt = 0xFFFFFFFF;

    WUT_Config(&wut_cfg);
    WUT_Enable();

    /* Enable WUT as a wakup source */
    MXC_GCR->pm |= MXC_F_GCR_PM_WUTWKEN;
    NVIC_EnableIRQ(WUT_IRQn);

    /* Delay before continuing with deep sleep code */
    WUT_Delay_MS(3000);

    /* power off unused hardware */
    DisableUnused();

    /* Initialize Radio */
    WsfInit();
    StackInitFit();
    FitStart();

    /* Setup pushbuttons and timers */
    PB_RegisterRiseFallCallback(0, Button0Pressed);
    PB_RegisterRiseFallCallback(1, Button1Pressed);
    PB_IntEnable(0);

    TMR_Init(BUTTON0_TMR, TMR_PRES_16, NULL);
    TMR_Init(BUTTON1_TMR, TMR_PRES_16, NULL);

    button_config.mode = TMR_MODE_CONTINUOUS;
    TMR_Config(BUTTON0_TMR, &button_config);
    TMR_Config(BUTTON1_TMR, &button_config);

    /* Initialize sleep states. */
#ifdef NORMAL_SLEEP
    /* Hook WUT_IRQHook for NORMAL_SLEEP. */
    PalRtcRegisterHook(WUT_IRQHook, NULL);
#endif /* NORMAL_SLEEP */

    SDMASleepStateInit(&stSDMASleepState);

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);

    printf("Setup Complete\n");

    while (1) {

        wsfOsDispatcher();

#ifdef ENABLE_SDMA
#ifdef ENABLE_SDMA_ERROR_REPORTING
        sdmaError = SDMACheckForError();
        if (sdmaError) {
            switch (sdmaError) {
            case SDMA_FLAG_ENOMEM:
                printf("SDMA core ran out of allocatable memory.");
                break;
            case SDMA_FLAG_EINVALIDIP:
                printf("SDMA invalid instruction pointer.");
                break;
            }

            fflush(stdout);
            while (1) {
            }
        }
#endif /* ENABLE_SDMA_ERROR_REPORTING */
#endif /* ENABLE_SDMA */

        Sleep();
    }
}

/*****************************************************************/
void HardFault_Decoder(unsigned int *f)
{

    printf("\n-- HardFault --\n");
    printf("HFSR 0x%08x CFSR 0x%08X", (unsigned int)SCB->HFSR,
            (unsigned int)SCB->CFSR);
    if (SCB->CFSR & 0x80) {
        printf(", MMFAR 0x%08X", (unsigned int)SCB->MMFAR);
    }
    if (SCB->CFSR & 0x8000) {
        printf(", BFAR 0x%08X", (unsigned int)SCB->BFAR);
    }
    printf("\n");

    /* Hang here */
    while (1);
}
