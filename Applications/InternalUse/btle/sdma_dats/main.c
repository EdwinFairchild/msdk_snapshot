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
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "app_ui.h"
#include "ll_api.h"
#include "sch_api.h"
#include "dats/dats_api.h"
#include "app_ui.h"
#include "hci_core.h"
#include "mxc_config.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "sdma_regs.h"
#include "mxc_pins.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "tmr.h"
#include "hci_vs.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS           6
#define WSF_BUF_SIZE            0x1048

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE         128

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

uint32_t SystemHeapSize=WSF_BUF_SIZE;
uint32_t SystemHeap[WSF_BUF_SIZE/4];
uint32_t SystemHeapStart;

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

/*! \brief  Stack initialization for app. */
extern void StackInitDats(void);

/*************************************************************************************************/
void print_sdma(void)
{
    printf("MXC_SDMA->ip = 0x%x\n", MXC_SDMA->ip);
    printf("MXC_SDMA->sp = 0x%x\n", MXC_SDMA->sp);
    printf("MXC_SDMA->dp0 = 0x%x\n", MXC_SDMA->dp0);
    printf("MXC_SDMA->dp1 = 0x%x\n", MXC_SDMA->dp1);
    printf("MXC_SDMA->bp = 0x%x\n", MXC_SDMA->bp);
    printf("MXC_SDMA->offs = 0x%x\n", MXC_SDMA->offs);
    printf("MXC_SDMA->lc0 = 0x%x\n", MXC_SDMA->lc0);
    printf("MXC_SDMA->lc1 = 0x%x\n", MXC_SDMA->lc1);
    printf("MXC_SDMA->a0 = 0x%x\n", MXC_SDMA->a0);
    printf("MXC_SDMA->a1 = 0x%x\n", MXC_SDMA->a1);
    printf("MXC_SDMA->a2 = 0x%x\n", MXC_SDMA->a2);
    printf("MXC_SDMA->a3 = 0x%x\n", MXC_SDMA->a3);
    printf("MXC_SDMA->wdcn = 0x%x\n", MXC_SDMA->wdcn);
    printf("MXC_SDMA->int_in_ctrl = 0x%x\n", MXC_SDMA->int_in_ctrl);
    printf("MXC_SDMA->int_in_flag = 0x%x\n", MXC_SDMA->int_in_flag);
    printf("MXC_SDMA->int_in_ie = 0x%x\n", MXC_SDMA->int_in_ie);
    printf("MXC_SDMA->irq_flag = 0x%x\n", MXC_SDMA->irq_flag);
    printf("MXC_SDMA->irq_ie = 0x%x\n\n", MXC_SDMA->irq_ie);
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
void wsf_assertion(void)
{
    printf("WSF assertion detected\r\n");
    while(1) {}
}

/*************************************************************************************************/
void TMR0_IRQHandler(void)
{
    TMR_IntClear(MXC_TMR0);
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
void PlatformInit(void)
{
    tmr_cfg_t tmr;
    uint32_t ticks, bytesUsed;

    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));
    printf("SystemHeapStart = 0x%x\n", SystemHeapStart);
    printf("SystemHeapSize = 0x%x\n", SystemHeapSize);
    bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    printf("bytesUsed = 0x%x\n", bytesUsed);
    
    WsfTraceRegisterHandler(myTrace);
    WsfTraceEnable(TRUE);

    // Initialize timer interrupt
    TMR_Init(MXC_TMR0, TMR_PRES_16, NULL);
    TMR_GetTicks(MXC_TMR0, 1, TMR_UNIT_MILLISEC, &ticks);
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.cmp_cnt = ticks;
    tmr.pol = 0;
    TMR_Config(MXC_TMR0, &tmr);

    NVIC_EnableIRQ(TMR0_IRQn);
    TMR_Enable(MXC_TMR0);
}

/*
 * In two-chip solutions, setting the address must wait until the HCI interface is initialized.
 * This handler can also catch other Application events, but none are currently implemented.
 * See ble-profiles/sources/apps/app/common/app_ui.c for further details.
 *
 */
void SetAddress(uint8_t event)
{
    uint8_t bdAddr[6] = {0x02, 0x00, 0x44, 0x8B, 0x05, 0x00};
    
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
int main(void)
{
    printf("\n***** MAX32665 SDMA, ARM Core *****\n");

    printf("Starting init\n");

    PlatformInit();
    StackInitDats();
    DatsStart();

    /* Register a handler for Application events */
    AppUiActionRegister(SetAddress);
    

    printf("ARM Setup complete\n");

    while (1) {
        wsfOsDispatcher();
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1) {}
}
