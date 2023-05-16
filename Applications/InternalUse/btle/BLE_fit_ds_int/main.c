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
 * $Date: 2021-09-08 15:46:34 -0500 (Wed, 08 Sep 2021) $
 * $Revision: 59633 $
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_cs.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "wsf_cs.h"
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
#include "dm_api.h"
#include "att_api.h"
#include "ecc.h"
#include "demo_config.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define WAKEUP_32M_US       (3200)
#define WUT_FREQ            (32768)
#define US_TO_WUTTICKS(x)   (((x)*WUT_FREQ)/1000000)
#define POWERUP_DELAY(sleep_ticks) (US_TO_WUTTICKS(WAKEUP_32M_US) + (sleep_ticks >> 6))
#define SLEEP_MIN_DELAY     (306) ///< Minimum delay period (us) required to attempt sleeping.
#define WSF_MIN_DELAY       (65535) ///< Minimum delay period (us) required to avoid waking SDMA.

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

uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] = {
    {  16,  8 },
    {  32,  4 },
    {  64,  4 },
    { 128,  4 },
    { 256,  4 },
    { 512,  4 }
};

// *INDENT-OFF*
/**
 * @brief User pin configuration. */
const gpio_cfg_t user_pin[USER_PIN_COUNT] = {
    /* USER_PIN_IN: */
    {
        USER_PIN_PORT,
        USER_PIN_PIN,
        GPIO_FUNC_IN,
        USER_PIN_PAD},
    /* USER_PIN_OUT: */
    {0}                                   // Default to using led_pin[USER_GPIO_LED].
};
// *INDENT-ON*

#ifdef ENABLE_SDMA
static SDMASleepState_t stSDMASleepState;
#endif /* ENABLE_SDMA */

#ifdef ENABLE_SDMA
/**
 * @brief Set whenever an SDMA interrupt occurs. */
static bool_t bHaveSDMAInterrupt = 0;
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
/*!
 *  \brief  Trace function to print to stdout.
 *  \param  pBuf    Data to write.
 *  \param  len     Length of data in bytes.
 *  \return TRUE if written to stdout.
 */
/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
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
uint32_t get_powerup_delay(uint32_t sleep_ticks)
{
    uint32_t ret;

    ret = POWERUP_DELAY(sleep_ticks);

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
    /* setup the systick for 1MS timer*/
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
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
    uint8_t bdAddr[6] = {0x02, 0x02, 0x44, 0x8B, 0x05, 0x00};

    switch (event) {
        case APP_UI_RESET_CMPL:
            printf("Setting address -- MAC %02X:%02X:%02X:%02X:%02X:%02X\n", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);
            HciVsSetBdAddr(bdAddr);
            break;
        default:
            break;
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Restart BLE hardware and timers.
 *
 *  \return None.
 */
void WUT_IRQHook(void *pHookArg)
{
    // This argument isn't used for this application.
    // Explicitly ignore.
    (void)pHookArg;

    bHaveWUTEvent = -1;
}

#ifdef ENABLE_SDMA
/**
 * @brief  SDMA interrupt hook.
 */
void SDMA0_IRQHook(void *pHookArg)
{
    // This argument isn't used for this application.
    // Explicitly ignore.
    (void)pHookArg;

    bHaveSDMAInterrupt = -1;
}
#endif /* ENABLE_SDMA */

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
void UserEvent(void)
{
#ifdef USER_GPIO_LED
    const gpio_cfg_t *pUserPinOutCfg;
    gpio_cfg_t zeroCfg = { 0 };
    mxc_gpio_regs_t *pUserPinOutReg;
#endif /* USER_GPIO_LED */

    // Get the gpio configuration for the output pin.
#ifdef USER_GPIO_LED
    pUserPinOutCfg = &user_pin[USER_PIN_OUT];
    if (memcmp(pUserPinOutCfg, &zeroCfg, sizeof(zeroCfg)) == 0) {
        pUserPinOutCfg = &led_pin[USER_GPIO_LED];
    }
    pUserPinOutReg = MXC_GPIO_GET_GPIO(pUserPinOutCfg->port);
#endif /* USER_GPIO_LED */

#if defined(MAIN_LOOP_HANDLER) || defined(SLEEP_LOOP_HANDLER)
    bHaveUserEvent = -1;
#endif /* defined(MAIN_LOOP_HANDLER) || defined(SLEEP_LOOP_HANDLER) */
#ifdef USER_GPIO_LED
#ifndef USER_GPIO_TOGGLE_DEMO
#ifdef USER_GPIO_TIMING_DEMO
    LED_On(USER_GPIO_LED);
    pUserPinOutReg->out_clr = pUserPinOutCfg->mask;
#endif /* USER_GPIO_TIMING_DEMO */
#endif /* USER_GPIO_TOGGLE_DEMO */
#endif /* USER_GPIO_LED */
}

void UserPinSetEnableInterrupt(bool_t bEnabled)
{
    const gpio_cfg_t *pUserPinInCfg;
    mxc_gpio_regs_t *pUserPinInReg;
    __IO uint32_t *pu32UserPinInRegField;

    // Get the gpio register for the input pin.
    pUserPinInCfg = &user_pin[USER_PIN_IN];
    pUserPinInReg = MXC_GPIO_GET_GPIO(pUserPinInCfg->port);

    // Get the appropriate offset for
    // setting/clearing interrupts on this GPIO port.
    pu32UserPinInRegField = (bEnabled ?
            &pUserPinInReg->int_en_clr:
            &pUserPinInReg->int_en_set);
        
    // Set/clear interrupts.
    *pu32UserPinInRegField = pUserPinInCfg->mask;
}

void UserEventPoll(void)
{
    int bCurUserLevel;

    const gpio_cfg_t *pUserPinInCfg;
    mxc_gpio_regs_t *pUserPinInReg;

    // Get the gpio configuration for the input pin.
    pUserPinInCfg = &user_pin[USER_PIN_IN];
    pUserPinInReg = MXC_GPIO_GET_GPIO(pUserPinInCfg->port);

    // Get the current user pin level.
    bCurUserLevel = (((pUserPinInReg->in & pUserPinInCfg->mask) == 0) ? 0 : 1);

    // If the level hasn't changed, then this is a non-event.
    if (bLastUserLevel == bCurUserLevel)
        return;

    // Process event.
    if (bCurUserLevel == 0) {
        UserEvent();
    }

    // Update prior observed pin level.
    bLastUserLevel = bCurUserLevel;
}

void UserEventInterrupt(void *arg)
{
    int bCurUserLevel;

    const gpio_cfg_t *pUserPinInCfg;
    mxc_gpio_regs_t *pUserPinInReg;

    // If the last interrupt is still pending, don't handle this interrupt.
    if (bHaveUserEvent != 0)
        return;

    // Get the gpio configuration for the input pin.
    pUserPinInCfg = &user_pin[USER_PIN_IN];
    pUserPinInReg = MXC_GPIO_GET_GPIO(pUserPinInCfg->port);

    // Get the current user pin level.
    bCurUserLevel = (((pUserPinInReg->in & pUserPinInCfg->mask) == 0) ? 0 : 1);

    // Process event.
    if (bCurUserLevel == 0) {
        UserEvent();
    }

    // Update prior observed pin level.
    bLastUserLevel = bCurUserLevel;
}

/*************************************************************************************************/
void RegisterUserEvent()
{
    // Register callback
    GPIO_RegisterCallback(&user_pin[USER_PIN_IN], UserEventInterrupt,
            (void *)0);

    // Configure and enable interrupt
    GPIO_IntConfig(&user_pin[USER_PIN_IN], GPIO_INT_EDGE, GPIO_INT_BOTH);
    GPIO_IntEnable(&user_pin[USER_PIN_IN]);
    GPIO_WakeEnable(&user_pin[USER_PIN_IN]);
    NVIC_EnableIRQ((IRQn_Type) MXC_GPIO_GET_IRQ(user_pin[USER_PIN_IN].port));
}

/*************************************************************************************************/
#if defined(MAIN_LOOP_HANDLER) || defined(SLEEP_LOOP_HANDLER)
void __attribute__((optimize("O0"))) HandleUserEvent(void)
{
#ifdef USER_GPIO_LED
    const gpio_cfg_t *pUserPinOutCfg;
    gpio_cfg_t zeroCfg = { 0 };
    mxc_gpio_regs_t *pUserPinOutReg;
#endif /* USER_GPIO_LED */

    bool_t dueValid;
    int sleep_ticks_available;
    int sleep_ticks_required;

#ifdef EVENT_TIME_REQUIREMENT
    uint32_t nextDbbEventDue;
#endif /* EVENT_TIME_REQUIREMENT */

    // Get the gpio configuration for the output pin.
#ifdef USER_GPIO_LED
    pUserPinOutCfg = &user_pin[USER_PIN_OUT];
    if (memcmp(pUserPinOutCfg, &zeroCfg, sizeof(zeroCfg)) == 0) {
        pUserPinOutCfg = &led_pin[USER_GPIO_LED];
    }
    pUserPinOutReg = MXC_GPIO_GET_GPIO(pUserPinOutCfg->port);
#endif /* USER_GPIO_LED */

    if (bHaveUserEvent) {
        // How much time is available to handle event?
#ifdef EVENT_TIME_REQUIREMENT
        dueValid = SchBleGetNextDueTime(&nextDbbEventDue);
        sleep_ticks_available = nextDbbEventDue - BbDrvGetCurrentTime();
        sleep_ticks_required = US_TO_BBTICKS(EVENT_TIME_REQUIREMENT);
#else /* EVENT_TIME_REQUIREMENT */
        dueValid = 0;
        sleep_ticks_available = 0;
        sleep_ticks_required = 0;
#endif /* EVENT_TIME_REQUIREMENT */

        // If there is enough time, handle event.
        if (!dueValid || (sleep_ticks_available > sleep_ticks_required)) {

#ifdef EVENT_TIME_REQUIREMENT
#ifdef EVENT_TIME_SIM_DELAY
#if EVENT_TIME_SIM_DELAY > 0
            // Simulate delay required to handle event.
            TMR_Delay(OST_TIMER, EVENT_TIME_SIM_DELAY, 0);
#else /* EVENT_TIME_SIM_DELAY > 0 */
            for (volatile int iter = 0; iter < -(EVENT_TIME_SIM_DELAY); iter++) {
            }
#endif /* EVENT_TIME_SIM_DELAY > 0 */
#endif /* EVENT_TIME_SIM_DELAY */
#endif /* EVENT_TIME_REQUIREMENT */

            // If there is enough time, handle event.
#ifdef USER_GPIO_LED
#ifdef USER_GPIO_TOGGLE_DEMO
            //LED_Toggle(USER_GPIO_LED);
            if (pUserPinOutReg->out & pUserPinOutCfg->mask)
                pUserPinOutReg->out_clr = pUserPinOutCfg->mask;
            else
                pUserPinOutReg->out_set = pUserPinOutCfg->mask;
#elif defined(USER_GPIO_TIMING_DEMO)
            LED_Off(USER_GPIO_LED);
            pUserPinOutReg->out_set = pUserPinOutCfg->mask;
#endif /* USER_GPIO_GPIO_DEMO */
#endif /* USER_GPIO_LED */

            // Clear pending event flag.
            bHaveUserEvent = 0;
        }
    }
}
#endif /* defined(MAIN_LOOP_HANDLER) || defined(SLEEP_LOOP_HANDLER) */

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
                    
#ifdef USE_FORCE_WSF_DISPATCH
                    // Leave the sleep function regardless of
                    // how much time is remaining. Wake SDMA
                    // back up and allow SDMA to decide whether
                    // returning to sleep makes sense.
                    bGoBackToSleep = 0;
#else /* USE_FORCE_WSF_DISPATCH */
                    // If a wakeup interrupt has happened.
                    if (bHaveWUTEvent) {
                        bGoBackToSleep = 0;
                    }
#endif /* USE_FORCE_WSF_DISPATCH */

#ifdef SLEEP_LOOP_HANDLER
                    // Handle user event signaled by GPIO interrupt.
                    HandleUserEvent();
#endif /* SLEEP_LOOP_HANDLER */
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

            /* Only count WUT interrupts that follow. */
            bHaveWUTEvent = 0;

            /* Only count SDMA interrupts that follow. */
            bHaveSDMAInterrupt = 0;

            /* Enter the appropriate sleep mode. */
            do {
                // Leave the sleep function (do loop) unless we
                // have a user wake event unrelated to anything BLE.
                // and USE_FORCE_WSF_DISPATCH is disabled.
                bGoBackToSleep = 0;

                /* Set the wakeup compare value from the SDMA (deepsleep only) */
                u32sdmaWakeupCount = WUT_GetCount();
                if (signaledSdmaFlag == SDMA_FLAG_SLEEPING)
                    u32sdmaWakeupTime = SDMAGetWakeupTime();
                else
                    u32sdmaWakeupTime = u32sdmaWakeupCount - 1;
                WUT_SetCompare(u32sdmaWakeupTime);

                /* Enter appropriate low power state. */
                if ((signaledSdmaFlag == SDMA_FLAG_SLEEPING) &&
                        ((signed)(u32sdmaWakeupTime - u32sdmaWakeupCount) <
                        US_TO_WUTTICKS(SLEEP_MIN_DELAY))) {
                    // printf("Missed wakeup time by %u ticks\r\n", (u32sdmaWakeupCount + SLEEP_MIN_DELAY + 1) - u32sdmaWakeupTime);
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

#ifndef USE_FORCE_WSF_DISPATCH
                        if (bHaveUserEvent)
                        {
                            u32sdmaWakeupTime = WUT_GetCompare();
                            u32sdmaWakeupCount = WUT_GetCount();

                            if (((signed)(u32sdmaWakeupTime - u32sdmaWakeupCount)) >
                                    US_TO_WUTTICKS(WSF_MIN_DELAY))
                                bGoBackToSleep = -1;
                        }

#endif /* USE_FORCE_WSF_DISPATCH */
                    } else {
                        /* Enter Background mode if SDMA stays awake */
#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
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
                }

#ifdef SLEEP_LOOP_HANDLER
                // Handle user event signaled by GPIO interrupt.
                HandleUserEvent();
#endif /* SLEEP_LOOP_HANDLER */
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

#define ENABLE_TRIMRTC_SWITCH_CLOCK
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

    int xtalEnabled;

    /* Initialize VREGO_D and BTLELDOCN for 32 MHz crystal */
    MXC_SIMO->vrego_d = (0x50 << MXC_F_SIMO_VREGO_D_VSETD_POS);
    while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYD)) {
    }

    MXC_GCR->btleldocn = 0xD5;

    TMR_Delay(OST_TIMER, MSEC(1000), 0);

    /* Note the state of the 32M clock. */
    xtalEnabled = SYS_IsClockEnabled(SYS_CLOCK_XTAL32M);

    /* Enable the 32.00 MHz crystal oscillator (XO) as the system clock source */
    if (!xtalEnabled) {
        if (SYS_ClockSourceEnable(SYS_CLOCK_XTAL32M) != E_NO_ERROR) {
            printf("Clock enable unsuccessful\n");
            return;
        }
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

    if (!xtalEnabled) {
        if (SYS_ClockSourceEnable(SYS_CLOCK_XTAL32M) != E_NO_ERROR) {
            printf("Clock disable unsuccessful\n");
            return;
        }
    }
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

#ifdef ENABLE_ECC
    ECC_SetEnable(TRUE);
#endif /* ENABLE_ECC */

    printf(" A 10sec delay before Adv. (..to allow re-flash)\n");

    TMR_Delay(OST_TIMER, INTERVAL_TIME_OST_STARTUP, 0);

    /* Restoring VREGOD when reset out of deep sleep */
    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VREGODPD;

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

    /* Initialize Wakeup timer */
    WUT_Init(WUT_PRES_1);

    wut_cfg.mode = WUT_MODE_COMPARE;
    wut_cfg.cmp_cnt = 0xFFFFFFFF;

    WUT_Config(&wut_cfg);
    WUT_Enable();

    /* Enable WUT and GPIO as a wakup source */
    MXC_GCR->pm |= (MXC_F_GCR_PM_WUTWKEN | MXC_F_GCR_PM_GPIOWKEN);
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

    /* Setup user event (may override PB pin). */
    RegisterUserEvent();
    PalRtcRegisterHook(WUT_IRQHook, NULL);

    /* Initialize sleep states. */
    SDMASleepStateInit(&stSDMASleepState);

#ifdef ENABLE_SDMA
    /* Register for notifications of SDMA interrupts. */
    SDMA0RegisterHook(SDMA0_IRQHook, NULL);
#endif /* ENABLE_SDMA */

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

#ifdef MAIN_LOOP_HANDLER
        /* Handle non-BLE low priority events. */
        HandleUserEvent();
#endif /* MAIN_LOOP_HANDLER */

        Sleep();

#ifdef ENABLE_SDMA
        /* Check to see if SDMA has sent an HCI message. */
        SDMA0_PollHCI();
#endif /* ENABLE_SDMA */
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
  __asm(
    " TST LR, #4\n"
    " ITE EQ \n"
    " MRSEQ R0, MSP \n"
    " MRSNE R0, PSP \n"
    " B HardFault_Decoder \n");
}


/*****************************************************************/
void HardFault_Decoder(unsigned int *f)
{

  printf("\n-- HardFault --\n");
  printf("HFSR 0x%08x CFSR 0x%08X", (unsigned int)SCB->HFSR, (unsigned int)SCB->CFSR);
  if (SCB->CFSR & 0x80) {
    printf(", MMFAR 0x%08X", (unsigned int)SCB->MMFAR);
  }
  if (SCB->CFSR & 0x8000) {
    printf(", BFAR 0x%08X", (unsigned int)SCB->BFAR);
  }
  printf("\n");

  /* Hang here */
  while(1);
}
