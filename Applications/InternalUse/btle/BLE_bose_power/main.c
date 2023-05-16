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
#include "sdma_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS       6
#define WSF_BUF_SIZE        0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE     128

#define RX_CHANNEL          20
#define RX_PHY              LL_PHY_LE_2M
#define RX_LEN              100
#define RX_ITERS            3000

#define SLEEP_US            20300
#define SCH_DELAY_US        105
#define RX_TIMEOUT          1400

#define GPIO_PORT_IN        PORT_1
#define GPIO_PIN_IN         PIN_14

volatile unsigned sdma_code = 0x0cfeda3a;  // jump $

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
void Sleep(void)
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

        BbDrvInitWake();
        
        /* Restore BB clock */
        WUT_RestoreBBClock();
    }
}

/*************************************************************************************************/
void rxCback(uint8_t status, int8_t rssi, uint32_t crc, uint32_t timestamp, uint8_t rxPhyOptions)
{
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
    dataParams.due              = BbDrvGetCurrentTime() + US_TO_BBTICKS(SCH_DELAY_US);
    dataParams.rxTimeoutUsec    = RX_TIMEOUT;
    dataParams.rxCback          = rxCback;
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
#include "simo_regs.h"
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

    // Disable SDMA, assert reset
    MXC_SDMA->ctrl &= ~(MXC_F_SDMA_CTRL_EN);

    // Enable SDMA
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_SDMAD);

    // Setup code address
    MXC_SDMA->ip_addr = (uint32_t)&sdma_code;

    MXC_SDMA->ctrl |= MXC_F_SDMA_CTRL_EN;

    /* Delay before continuing with deep sleep code */
    WUT_Delay_MS(3000);

    printf("Setup Complete\n");
    WUT_Delay_MS(100);


    while (1)
    {
        RX_Packet();
        
        PowerDownBLE();

        SIMO_setVregO_B(900);
        SIMO_setVregO_C(900);

        while(!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB)) {}
        MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
        MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV1;

        // Delay to emulate LC3 processing
        static int i;
        if(++i%100 == 0)
            printf("%d\n", i);
        WUT_Delay_MS(8);

        Sleep();
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

extern unsigned int _data;
extern unsigned int __StackTop;

/*****************************************************************/
void HardFault_Decoder(unsigned int *f)
{

    SystemCoreClockUpdate();
    Console_Init();

  printf("\n-- HardFault --\n");
  printf("HFSR 0x%08x CFSR 0x%08X", (unsigned int)SCB->HFSR, (unsigned int)SCB->CFSR);
  if (SCB->CFSR & 0x80) {
    printf(", MMFAR 0x%08X", (unsigned int)SCB->MMFAR);
  }
  if (SCB->CFSR & 0x8000) {
    printf(", BFAR 0x%08X", (unsigned int)SCB->BFAR);
  }
  printf("\n");
  if ((f < &_data) || (f > &__StackTop)) {
    printf("Invalid fault stack address (0x%08X). Decode is likely incorrect!\n", (unsigned int)f);
  }
  printf("stacked pc=0x%08X\tlr=0x%08X\tpsr=0x%08X\n", f[6], f[5], f[7]);
  
  /* Hang here */
  while(1);
}
