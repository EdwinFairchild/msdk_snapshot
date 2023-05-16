/*************************************************************************************************/
/*!
 *  \file   main.c
 *
 *  \brief  Main module.
 *
 *          $Date: 2019-11-08 11:13:40 -0600 (Fri, 08 Nov 2019) $
 *          $Revision: 48553 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "ll_api.h"
#include "sch_api.h"
#include "datc/datc_api.h"
#include "mxc_config.h"
#include "gcr_regs.h"
#include "mcr_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS              6

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! Free memory for pool buffers (use word elements for word alignment). */
static uint32_t mainBufMem[3584/sizeof(uint32_t)+96];

/*! Default pool descriptor. */
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 384,  4 }
};

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInitDatc(void);

/*************************************************************************************************/
void SysTick_Handler(void)
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
static void myTrace(const char *pStr, va_list args)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        vprintf(pStr, args);
        printf("\r\n");
    }
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
    WsfTimerInit();
    WsfBufInit(sizeof(mainBufMem), (uint8_t*)mainBufMem, WSF_BUF_POOLS, mainPoolDesc);
    WsfTraceRegister(myTrace);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize platform.
 *
 *  \param  msPerTick   Milliseconds per timer tick.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PlatformInit(void)
{
    /* Enable peripheral clocks */
    MXC_MCR->ctrl = 0x202;

    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);  // Clear GPIO0 and GPIO1 Disable
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD );  // Clear BTLE and ICACHE0 disable

    /* setup the systick */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize MAC layer.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void MacInit(void)
{
    wsfHandlerId_t handlerId;

    /* Initialize link layer. */
    BbInit();
    handlerId = WsfOsSetNextHandler(SchHandler);
    SchInit(handlerId);
    LlScanMasterInit();
    LlConnMasterInit();
    handlerId = WsfOsSetNextHandler(LlHandler);
    LlHandlerInit(handlerId);
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

  printf("\n\n***** MAX32665 Data Collector *****\n");

#ifdef AFE_BOARD
    printf("AFE_BOARD %u\n", AFE_BOARD);
    bdAddr_t bdAddr = {AFE_BOARD,0x01,0x44,0x33,0x22,0x11};
#else
    bdAddr_t bdAddr = {0x00,0x01,0x44,0x33,0x22,0x11};
#endif
    printf("MAC %02X:%02X:%02X:%02X:%02X:%02X\n", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);

    PlatformInit();
    WsfInit();
    MacInit();
    StackInitDatc();
    DatcStart();

    /* TODO replace with static address */
    //LlGetRandAddr((uint8_t*)&bdAddr);
    LlSetBdAddr((uint8_t*)&bdAddr);

    printf("Setup Complete\n");

    while (1)
    {
        wsfOsDispatcher();
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1);
}
