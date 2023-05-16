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
 * $Date: 2022-05-11 09:20:55 -0500 (Wed, 11 May 2022) $
 * $Revision: 62404 $
 *
 ******************************************************************************/

#define MAIN_C

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mxc_config.h"
#include "wsf_types.h"
#include "wsf_os.h"
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
#include "mxc_delay.h"

#include "user_event.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

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

static volatile bool_t bHaveWUTEvent;

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

gpio_cfg_t user_interrupt_status;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitFit(void);


/*************************************************************************************************/
/*!
 *  \brief  SysTick interrupt service routine.
 *
 *  \return None.
 */
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
    uint8_t bdAddr[6] = {0x02, 0x12, 0x44, 0x8B, 0x05, 0x00};

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
 *  \brief  Register's the occurance of a WUT interrupt.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WUT_IRQHandler(void)
{
  bHaveWUTEvent = TRUE;
  WUT_IntClear();
  NVIC_ClearPendingIRQ(WUT_IRQn);
}

/*************************************************************************************************/
/*!
 *  \brief  Enable user defined GPIO interrupts/wake events.
 *
 *  \return None.
 */
/*************************************************************************************************/
void RegisterUserEvent()
{
    // Register callback
    GPIO_RegisterCallback(&user_pin[USER_PIN_IN], HandleUserEventInterrupt, &user_interrupt_status);

    // Configure and enable interrupt
    GPIO_IntConfig(&user_pin[USER_PIN_IN], GPIO_INT_EDGE, GPIO_INT_BOTH);
    GPIO_IntEnable(&user_pin[USER_PIN_IN]);
    GPIO_WakeEnable(&user_pin[USER_PIN_IN]);
    NVIC_EnableIRQ((IRQn_Type) MXC_GPIO_GET_IRQ(user_pin[USER_PIN_IN].port));
}

void SetUserLED(int bEnabled)
{
    const gpio_cfg_t *pUserPinOutCfg;
    gpio_cfg_t zeroCfg = { 0 };
    mxc_gpio_regs_t *pUserPinOutReg;

    int bUseSet;

    // If an output GPIO is provided, use that.
    // Otherwise, just use the gpio configuration
    // associated with the LED specifed by USER_GPIO_LED.
    pUserPinOutCfg = &user_pin[USER_PIN_OUT];
    if (memcmp(pUserPinOutCfg, &zeroCfg, sizeof(zeroCfg)) == 0) {
        pUserPinOutCfg = &led_pin[USER_GPIO_LED];
    }
    pUserPinOutReg = MXC_GPIO_GET_GPIO(pUserPinOutCfg->port);

    // Are LEDs active low or active high on this board?
#if (LED_ON == 0)
    bUseSet = FALSE;
    if (bEnabled == FALSE)
        bUseSet = TRUE;
#else
    bUseSet = FALSE;
    if (bEnabled != FALSE)
        bUseSet = TRUE;
#endif

    // Set GPIO output accordingly.
    if (bUseSet == FALSE)
        pUserPinOutReg->out_clr = pUserPinOutCfg->mask;
    else
        pUserPinOutReg->out_set = pUserPinOutCfg->mask;
}

/*************************************************************************************************/
/*!
 *  \brief  Restart BLE hardware and timers.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void BLEWake(void)
{
    WsfTaskLock();

    /* Restore BLE power */
    BbDrvEnable();

    /* Restore BLE hardware state */
    BbDrvInitWake();

    /* Restore BB clock */
    WUT_RestoreBBClock(BB_CLK_RATE_HZ);
    WsfTimerUpdate((WUT_GetSleepTicks() * 1000) / SYS_WUT_GetFreq() / WSF_MS_PER_TICK );

    /* setup the systick */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);

    WsfTaskUnlock();

    /* Re-schedule next BLE operation */
    SchWake();
}

/*************************************************************************************************/
/*!
 *  \brief  Attempt to sleep.
 *
 *  \return None.
 */
/*************************************************************************************************/
void Sleep(void)
{
    uint32_t u32cnt;
    uint32_t u32cmp;
    
    /* WSF and UART are not busy ? */
    if (wsfOsReadyToSleep() && UART_PrepForSleep(MXC_UART_GET_UART(CONSOLE_UART)) == E_NO_ERROR) {

        WsfTaskLock();

        /* get next due time and sleep time */
        uint32_t  nextDbbEventDue;
        bool_t dueValid = SchBleGetNextDueTime(&nextDbbEventDue);
        int sleep_ticks = nextDbbEventDue - BbDrvGetCurrentTime();

        WsfTaskUnlock();


        /* timing valid ? */
        if(dueValid && (sleep_ticks > 0 )) {
            /* have enough time to deep sleep ? */
            if(sleep_ticks > US_TO_BBTICKS(DS_WAKEUP_TIME_US + MIN_DEEPSLEEP_US)) {

                WsfTaskLock();

                /* Stop SysTick */
                SysTick->CTRL = 0;

                /* save DBB, WUT clocks, arm WUT for wakeup */
                WUT_SetWakeup(sleep_ticks - GetWakeDelay(sleep_ticks));

                WsfTaskUnlock();

                /* unschedule next BLE operation */
                SchSleep();

                /* Attempt to detect subsequent WUT events. */
                bHaveWUTEvent = FALSE;

                /*  Service GPIO interrupts without restarting BLE, 
                    don't deep sleep if we're too close to the WUT wakeup time */
                bool_t bHaveSufficientTime;
                do {
                    bHaveSufficientTime = FALSE;

                    LED_Off(SLEEP_GPIO_LED);

                    /* enterDeepSleep mode */
                    EnterDeepsleep();

                    /* initial restore */
                    ExitDeepsleep();

                    LED_On(SLEEP_GPIO_LED);

                    /* Process the GPIO interrupt message */
                    if (bHaveWUTEvent == FALSE) {

                        /* If there are pending WSF events (presumably GPIO events), then we need
                         * to make sure that an event handler can block for USER_EVENT_DELAY us
                         * plus the overhead imposed by any additional WSF processing. */
                        if (!wsfOsReadyToSleep())
                        {
                            /* Do we need to restart BLE before handling GPIO event? */
                            /* Note that if (USER_EVENT_DELAY + WSF_DELAY_MARGIN) >
                             *     (DS_WAKEUP_TIME_US + MIN_DEEPSLEEP_US), then
                             * we will have to wake up earlier than we otherwise would. This will result
                             * in increased power consumption. */
                            u32cmp = MXC_WUT->cmp;
                            u32cnt = MXC_WUT->cnt;
                            if ((u32cmp - u32cnt) <
                                    (US_TO_WUTTICKS(USER_EVENT_DELAY + WSF_DELAY_MARGIN + WUT_DS_MARGIN)))
                            {
                                /* Don't handle GPIO event without first restarting BLE.
                                 * There isn't enough time to accomodate the handler. */
                                MXC_ASSERT(bHaveSufficientTime == FALSE);
                                break;
                            }

                            /* Handle GPIO wake event */
                            do {
                                /* We should block in this loop for no longer than
                                 * (USER_EVENT_DELAY + WSF_DELAY_MARGIN) us. */
                                wsfOsDispatcher();
                            } while(!wsfOsReadyToSleep());
                        }

                        /* Is there sufficient time to go back to sleep? */
                        u32cmp = MXC_WUT->cmp;
                        u32cnt = MXC_WUT->cnt;
                        if ((u32cmp - u32cnt) >
                                (US_TO_WUTTICKS(DS_WAKEUP_TIME_US + MIN_DEEPSLEEP_US + WUT_DS_MARGIN)))
                            bHaveSufficientTime = TRUE;
                    }
                } while ((bHaveWUTEvent == FALSE) && (bHaveSufficientTime != FALSE));

                BLEWake();

                /* have enough time to sleep ?*/
            } else {
                LED_Off(SLEEP_GPIO_LED);
                LP_EnterSleepMode();
                LED_On(SLEEP_GPIO_LED);
            }
            /* Nothing scheduled, wait for interrupt */
        } else {
            LED_Off(SLEEP_GPIO_LED);
            LP_EnterSleepMode();
            LED_On(SLEEP_GPIO_LED);
        }
    } /* if(not busy) */
}

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
    while(!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYD)) {}

    MXC_GCR->btleldocn = 0xD5;

    mxc_delay(1000000);

    /* Select the 32.00 MHz crystal oscillator (XO) as the system clock source */
    if (SYS_Clock_Select(SYS_CLOCK_XTAL32M, NULL) != E_NO_ERROR) {
        printf("Clock switch unsuccessful\n");
        return;
    } else {
        Console_Init();
    }

    sys_cfg_rtc_t sys_cfg = NULL; /*  No Specific system configuration object */
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
    if(freq < 0) {
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
    LED_On(SLEEP_GPIO_LED);
    printf("\n\n***** MAX32665 BLE Fitness Profile, Deep Sleep w/ GPIO interrupt *****\n");

    // Delay to prevent bricks
    mxc_delay(STARTUP_DELAY);

    printf("Starting application\n");

    /* Trim 32 kHz trim */
    trimRTC();
    while(UART_PrepForSleep(MXC_UART_GET_UART(CONSOLE_UART)) != E_NO_ERROR) {}

    /* Run from the 60MHz oscillator */
    switchToHIRC();
    Console_Init();

    printf("SystemCoreClock = %d\n", SystemCoreClock);

    /* Make sure VRego C uses run voltage. */
    SIMO_setVregO_C(RUN_VOLTAGE);
    while(!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYC)) {}

#if (BACKUP_MODE == 1)
    printf("Entering backup mode instead of deep sleep\n");
#endif

    /* Initialize Wakeup timer */
    WUT_Init(WUT_PRES_1);
    wut_cfg_t wut_cfg;
    wut_cfg.mode = WUT_MODE_COMPARE;
    wut_cfg.cmp_cnt = 0xFFFFFFFF;

    WUT_Config(&wut_cfg);
    WUT_Enable();

    /* Enable WUT as a wakup source */
    MXC_GCR->pm |= MXC_F_GCR_PM_WUTWKEN;
    NVIC_SetPriority(WUT_IRQn, 0x0);    /* highest priority */
    NVIC_EnableIRQ(WUT_IRQn);

    /* Setting interrupt priorities */
    NVIC_SetPriority(BTLE_TX_DONE_IRQn,1);
    NVIC_SetPriority(BTLE_RX_RCVD_IRQn,1);
    NVIC_SetPriority(BTLE_RX_ENG_DET_IRQn,1);
    NVIC_SetPriority(BTLE_SFD_DET_IRQn,1);
    NVIC_SetPriority(BTLE_SFD_TO_IRQn,1);
    NVIC_SetPriority(BTLE_GP_EVENT_IRQn,1);
    NVIC_SetPriority(BTLE_CFO_IRQn,1);
    NVIC_SetPriority(BTLE_SIG_DET_IRQn,1);
    NVIC_SetPriority(BTLE_AGC_EVENT_IRQn,1);
    NVIC_SetPriority(BTLE_RFFE_SPIM_IRQn,1);
    NVIC_SetPriority(BTLE_TX_AES_IRQn,1);
    NVIC_SetPriority(BTLE_RX_AES_IRQn,1);
    NVIC_SetPriority(BTLE_INV_APB_ADDR_IRQn,1);
    NVIC_SetPriority(BTLE_IQ_DATA_VALID_IRQn,1);

    NVIC_SetPriority(WUT_IRQn, 2);

    NVIC_SetPriority(GPIO0_IRQn, 3);
    NVIC_SetPriority(GPIO1_IRQn, 3);
    NVIC_EnableIRQ(GPIO0_IRQn);
    NVIC_EnableIRQ(GPIO1_IRQn);

    /* power off unused hardware */
    DisableUnused();

    /* Initialize Radio */
    WsfInit();
    StackInitFit();
    FitStart();

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);

    /* Setup user event (may override PB pin). */
    RegisterUserEvent();

    printf("Setup Complete\n");

    while (1) {
        wsfOsDispatcher();
        Sleep();
    }
}

/*****************************************************************/
void HardFault_Decoder(unsigned int *f)
{
  printf("\n-- HardFault --\n");
  printf("HFSR 0x%08X ", (unsigned int)SCB->HFSR);
  printf("CFSR 0x%08X", (unsigned int)SCB->CFSR);
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
