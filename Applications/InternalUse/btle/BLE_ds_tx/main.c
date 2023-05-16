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
#include "ll_api.h"
#include "gpio.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

#define TX_CHANNEL          20
#define TX_PHY              LL_PHY_LE_1M
#define TX_LEN              37
#define TX_PKT_TYPE         LL_TEST_PKT_TYPE_F0
#define TX_PACKETS          1500

#define SLEEP_US            5000
#define SCH_DELAY_US        300

#define GPIO_PORT_OUT       PORT_1
#define GPIO_PIN_OUT        PIN_14

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

static volatile bool_t txDone;


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

        /* save DBB, WUT clocks, arm WUT for wakeup */
        WUT_SetWakeup(sleep_ticks - powerup_ticks - US_TO_BBTICKS(WAKEUP_TIME_US));

        /* power off unused hardware */
        DisableUnused();

        /* enterDeepSleep mode */
        WsfTaskUnlock();
        EnterDeepsleep();

        /* initial restore */
        ExitDeepsleep();
        WsfTaskLock();

        /* Restore BLE hardware state */
        BbDrvInitWake();

        /* Restore BB clock */
        WUT_RestoreBBClock();
    }
}

/*************************************************************************************************/
/*!
 *  \brief      Fill buffer with random payload values.
 *
 *  \param      pBuf        Buffer to fill.
 *  \param      len         Number of bytes to fill.
 *
 *  \return     None.
 *
 *  Fill payload with random numbers.
 */
/*************************************************************************************************/
static void llPrbs9Payload(uint8_t *pBuf, uint8_t len)
{
  const uint32_t lfsrInit = 0x1FF;
  const uint32_t bitsPerByte = 8;

  unsigned int bit;
  unsigned int bitNum;
  uint32_t lfsr = lfsrInit;

  while (len-- > 0)
  {
    /* Empty buffer (initial value contains first byte). */
    *(pBuf++) = (uint8_t)lfsr;

    /* Calculate next 8 bits. */
    bitNum = bitsPerByte;
    while (bitNum-- > 0)
    {
      /* Cycle PRBS9 sequence; taps are 5th (bit 4) and 9th (bit 0) position. */
      bit  = ((lfsr >> 4) ^ (lfsr >> 0)) & 1;
      lfsr =  (lfsr >> 1) | (bit << 8);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Buikld transmit buffer.
 *
 *  \param      len         Length of test data.
 *  \param      pktType     Test packet payload type.
 *  \param      pBuf        Tx buffer.
 *
 *  Build DTM packet header and payload buffer.
 */
/*************************************************************************************************/
static void llBuildTxPkt(uint8_t len, uint8_t pktType, uint8_t *pBuf)
{
  /* Fill header. */
  *pBuf++ = pktType;
  *pBuf++ = len;

  /* Fill payload. */
  switch (pktType)
  {
    case LL_TEST_PKT_TYPE_0F:
      memset(pBuf, 0x0F, len);
      break;
    case LL_TEST_PKT_TYPE_55:
      memset(pBuf, 0x55, len);
      break;
    case LL_TEST_PKT_TYPE_FF:
      memset(pBuf, 0xFF, len);
      break;
    case LL_TEST_PKT_TYPE_00:
      memset(pBuf, 0x00, len);
      break;
    case LL_TEST_PKT_TYPE_F0:
      memset(pBuf, 0xF0, len);
      break;
    case LL_TEST_PKT_TYPE_AA:
      memset(pBuf, 0xAA, len);
      break;
    case LL_TEST_PKT_TYPE_PRBS9:
      llPrbs9Payload(pBuf, len);
      break;
    case LL_TEST_PKT_TYPE_PRBS15:
    default:
      break;
  }
}

/*************************************************************************************************/
void txCback(uint8_t status)
{
    txDone = TRUE;
}

/*************************************************************************************************/
void TX_Packet(void)
{
    /* Setup the channel, phy ... parameters */
    BbBleDrvChan_t chanParams;
    chanParams.chanIdx          = llConvertRfChanToChanIdx(TX_CHANNEL);
    chanParams.txPhy            = TX_PHY;
    chanParams.accAddr          = LL_DTM_SYNC_WORD;
    chanParams.crcInit          = LL_DTM_CRC_INIT;
    BbBleDrvSetChannelParam(&chanParams);

    BbBleDrvOpParam_t opParams;
    opParams.ifsSetup           = FALSE;
    BbBleDrvSetOpParams(&opParams);

    BbBleDrvDataParam_t dataParams;
    dataParams.due              = BbDrvGetCurrentTime() + US_TO_BBTICKS(SCH_DELAY_US);
    dataParams.txCback          = txCback;
    BbBleDrvSetDataParams(&dataParams);

    /* Disable whitening */
    BbBleDrvEnableDataWhitening(FALSE);

    /* Setup the callback */
    txDone = FALSE;

    /* Build the payload */
    uint8_t txBuf[TX_LEN+2];
    llBuildTxPkt(TX_LEN, TX_PKT_TYPE, txBuf);

    /* Start the TX operation */
    BbBleDrvTxBufDesc_t txBufDesc;
    txBufDesc.len               = TX_LEN+2;
    txBufDesc.pBuf              = txBuf;
    BbBleDrvTxData(&txBufDesc, 1);

    /* Wait for the callback */
    while(!txDone) {}
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

    printf("\n\n***** MAX32665 Deep Sleep TX *****\n");

    /* Delay before continuing with deep sleep code */
    WUT_Init();

    WsfInit();
    StackInit();

    BbDrvInit();
    WUT_Delay_MS(10);
    BbDrvEnable();
    BbBleDrvInit();

    /* Setup outpt pin to receiver */
    gpio_cfg_t gpio_out;
    gpio_out.port = GPIO_PORT_OUT;
    gpio_out.mask = GPIO_PIN_OUT;
    gpio_out.pad = GPIO_PAD_NONE;
    gpio_out.func = GPIO_FUNC_OUT;
    GPIO_Config(&gpio_out);
    GPIO_OutSet(&gpio_out);

    int txPackets = 0;

    WUT_Delay_MS(3000);
    printf("Setup Complete\n");

    /* Signal to receiver that we're about to start */
    GPIO_OutClr(&gpio_out);
    WUT_Delay_MS(1);

    while (1)
    {
        TX_Packet();

        // DeepSleep();
        WUT_Delay_100US(SLEEP_US / 100);

        if(++txPackets == TX_PACKETS) {
          GPIO_OutSet(&gpio_out);
          printf("Done transmitting\n");

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
