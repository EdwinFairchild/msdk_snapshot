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
 * $Date: 2019-11-08 11:13:40 -0600 (Fri, 08 Nov 2019) $
 * $Revision: 48553 $
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "wakeup.h"
#include "sch_api.h"
#include "uart.h"
#include "ll_debug.h"
#include "bb_ble_drv.h"
#include "bb_drv.h"
#include "ll_api.h"
#include "gpio.h"
#include "lp.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

#define RX_CHANNEL          20
#define RX_PHY              LL_PHY_LE_1M
#define RX_LEN              100
#define RX_ITERS            3000

#define SLEEP_US            5000
#define SCH_DELAY_US        105
#define RX_TIMEOUT          10000
#define IFS_US              (SLEEP_US+654)      // Measured emperically from transmitter

#define GPIO_PORT_IN        PORT_1
#define GPIO_PIN_IN         PIN_14

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 512,  4 }
};

static volatile bool_t rxDone;
static uint8_t rxBuf[RX_LEN];
static int rxOk;
static int rxTo;
static int rxCRC;
static int failCnt;
uint32_t anchor;

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInit(void);

extern uint8_t llConvertRfChanToChanIdx(uint8_t rfChan);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        fwrite(pBuf, len, 1, stdout);
        return TRUE;
    }

    return FALSE;
}

/*************************************************************************************************/
void PalSysAssertTrap(void)
{
    printf("Assertion detected\n");
    while(1) {}
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
    if((bytesUsed = WsfBufCalcSize(WSF_BUF_POOLS, mainPoolDesc)) != SystemHeapSize) {
        printf("SystemHeapSize 0x%x != bytesUsed 0x%x\n", SystemHeapSize, bytesUsed);
        printf("  Adjust SystmeHeap size to match mainPoolDesc\n");
        while(1){}
    }
    WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    
    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);
}

/*************************************************************************************************/
void DeepSleep(void)
{
    uint32_t sleep_ticks = US_TO_BBTICKS(SLEEP_US);

    /* need extra time to restore voltages */
    uint32_t powerup_ticks = GetWakeDelay(sleep_ticks);

    /* have enough time to sleep ? */
    if(sleep_ticks > (US_TO_BBTICKS(WAKEUP_TIME_US + MIN_DEEPSLEEP_US) + powerup_ticks)) {

        /* Stop SysTick */
        SysTick->CTRL = 0;

        // sleep_ticks = 0x7FFFFFFF;

        /* save DBB, WUT clocks, arm WUT for wakeup */
        WUT_SetWakeup(sleep_ticks - powerup_ticks - US_TO_BBTICKS(WAKEUP_TIME_US));

        /* power off unused hardware */
        DisableUnused();

        /* enterDeepSleep mode */
        WsfTaskUnlock();
        PowerDown();

        LP_EnterBackgroundMode();
        // LP_EnterDeepSleepMode();
        // EnterBackup();

        /* initial restore */
        ExitSleep();
        WsfTaskLock();

        /* Restore BLE hardware state */
        ll_dbg_pin_assert(LL_DBG_PIN_RX_CRC);

        BbDrvInitWake();
        ll_dbg_pin_assert(LL_DBG_PIN_RX_CRC);


        /* Restore BB clock */
        WUT_RestoreBBClock();
    }
}

/*************************************************************************************************/
void rxCback(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions)
{
    if(status == BB_STATUS_SUCCESS) {
      anchor = timestamp;
      failCnt = 0;
      rxOk++;
    } else if(status == BB_STATUS_CRC_FAILED) {
      anchor = timestamp;
      rxCRC++;
      failCnt = 0;
    } else if(anchor != 0){
      failCnt++;
      rxTo++;
    }

    rxDone = TRUE;
}

/*************************************************************************************************/
void RX_Packet(void)
{
    /* Setup the channel, phy ... parameters */
    BbBleDrvChan_t chanParams;
    chanParams.chanIdx          = llConvertRfChanToChanIdx(RX_CHANNEL);
    chanParams.rxPhy            = RX_PHY;
    chanParams.accAddr          = LL_DTM_SYNC_WORD;
    chanParams.crcInit          = LL_DTM_CRC_INIT;
    BbBleDrvSetChannelParam(&chanParams);

    BbBleDrvOpParam_t opParams;
    opParams.ifsSetup           = FALSE;
    BbBleDrvSetOpParams(&opParams);

    /* Disable whitening */
    BbBleDrvEnableDataWhitening(FALSE);

    BbBleDrvDataParam_t dataParams;
    if(anchor == 0) {
      dataParams.due              = BbDrvGetCurrentTime() + US_TO_BBTICKS(SCH_DELAY_US);
      dataParams.rxTimeoutUsec    = 1000;
    } else {
      /* Use the fail count for window widening */
      dataParams.due              = anchor + (US_TO_BBTICKS(IFS_US)*(failCnt+1)) - US_TO_BBTICKS(SCH_DELAY_US) - US_TO_BBTICKS(failCnt*10);
      dataParams.rxTimeoutUsec    = 500 + failCnt*10;
    }

    dataParams.rxCback            = rxCback;
    BbBleDrvSetDataParams(&dataParams);

    /* Setup the callback */
    rxDone = FALSE;

    /* Start the RX operation */
    BbBleDrvRxData(rxBuf, RX_LEN);

    /* Wait for the callback */
    while(!rxDone) {
      LP_EnterSleepMode();
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

#ifndef __IAR_SYSTEMS_ICC__
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);
#endif

    printf("\n\n***** MAX32665 Deep Sleep RX *****\n");

    WUT_Init();
    WsfInit();
    StackInit();
    BbDrvEnable();

    /* Delay before continuing with deep sleep code */
    WUT_Delay_MS(2000);

    /* Init the RX packet counters */
    rxOk = 0;
    rxCRC = 0;
    rxTo = 0;

    /* Init the anchor point */
    anchor = 0;
    failCnt = 0;

    /* Setup GPIO to receive signal from transmitter */
    gpio_cfg_t gpio_in;
    gpio_in.port = GPIO_PORT_IN;
    gpio_in.mask = GPIO_PIN_IN;
    gpio_in.pad = GPIO_PAD_PULL_UP;
    gpio_in.func = GPIO_FUNC_IN;
    GPIO_Config(&gpio_in);

    printf("Setup Complete\n");

    /* Wait for signal to go low before starting the RX */
    // while(GPIO_InGet(&gpio_in)) {}

    while (1)
    {
        RX_Packet();

        DeepSleep();
        // WUT_Delay_100US((SLEEP_US - 1500) / 100);

        if(GPIO_InGet(&gpio_in)) {
          printf("rxOk  = %d\n", rxOk);
          printf("rxCRC = %d\n", rxCRC);
          printf("rxTo  = %d\n", rxTo);
          while(1) {}
        }
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1);
}
