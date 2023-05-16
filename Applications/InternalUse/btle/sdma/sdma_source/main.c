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
 * $Id: main.c 48553 2019-11-08 17:13:40Z kevin.gillespie $
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "wsf_os.h"
#include "wsf_buf.h"
#include "wsf_trace.h"
#include "wsf_timer.h"
#include "lhci_api.h"
#include "bb_ble_api.h"
#include "sch_api.h"
#include "ll_api.h"
#include "bb_ble_api.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "max32665.h"
#include "gpio_regs.h"
#include "tmr_regs.h"
#include "ipc_def.h"
#include "lhci_drv.h"
#include "sdma_regs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS               6

#define MXC_DBB_EVT_STATUS                          (*((volatile uint16_t*)(MXC_BASE_BTLE_DBB_CTRL + 0x084)))

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! Free memory for pool buffers (use word elements for word alignment). */
static uint32_t mainBufMem[3584/sizeof(uint32_t) + 96];

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

/*************************************************************************************************/
void mainPlatformInit(void)
{
    WsfTimerInit();
    WsfBufInit(sizeof(mainBufMem), (uint8_t*)mainBufMem, WSF_BUF_POOLS, mainPoolDesc);
}

extern void LHCI_Handler(void);
/*************************************************************************************************/
void UART1_IRQHandler(void) __interrupt
{
    LHCI_Handler();
}

void INT_IN_IRQHandler(void) __interrupt
{

}

void TMR0_IRQHandler(void) __interrupt
{
  
}

void TMRB_IRQHandler(void) __interrupt
{
    SDMA_GPIO_PORT->out ^= SDMA_GPIO_PIN0;
    WsfTimerUpdate(WSF_MS_PER_TICK);
    wsfOsDispatcher();
}

/*************************************************************************************************/
void mainMacInit(void)
{
    wsfHandlerId_t handlerId;

    /* Initialize link layer. */
    BbInit();
    handlerId = WsfOsSetNextHandler(SchHandler);
    SchInit(handlerId);
    LlAdvSlaveInit();
    LlConnSlaveInit();
    LlScanMasterInit();
    LlConnMasterInit();
    handlerId = WsfOsSetNextHandler(LlHandler);
    LlHandlerInit(handlerId);

    handlerId = WsfOsSetNextHandler(LhciHandler);
    LhciHandlerInit(handlerId);
    LhciAdvSlaveInit();
    LhciScanMasterInit();
}

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

    __enable_irq();
}

/*************************************************************************************************/
void main(void)
{
    bdAddr_t bdAddr = {0x56,0x34,0x12,0x8B,0x05,0x00};

    printf("***** ME14 SDMA LHCI *****\r\n");

    mainPlatformInit();
    mainMacInit();
    LlSetBdAddr((uint8_t*)&bdAddr);

    printf("Setup complete\r\n");

    while(1) {
        BTLE_IRQHandler();
    }
}
