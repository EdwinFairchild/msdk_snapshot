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
#include "max32665.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Size of buffer for stdio functions */
#define WSF_BUF_POOLS               6
#define WSF_BUF_SIZE                0x1048

#define SLEEP_MIN                   0x1000

#define MXC_DBB_EVT_STATUS          (*((volatile uint16_t*)(MXC_BASE_BTLE_DBB_CTRL + 0x084)))
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

/*! \brief  Stack initialization */
extern void StackInit(void);

/*************************************************************************************************/
static bool_t myTrace(const uint8_t *pBuf, uint32_t len)
{
    extern uint8_t wsfCsNesting;

    if (wsfCsNesting == 0)
    {
        printf("%.*s",len,pBuf);
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
void UART1_IRQHandler(void) __interrupt
{
    // Do nothing, we're using the IPC for LHCI
}

/*************************************************************************************************/
void TMRB_IRQHandler(void) __interrupt
{
    WsfTimerUpdate(WSF_MS_PER_TICK);
}

/*************************************************************************************************/
void TMR0_IRQHandler(void) __interrupt
{

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

    WsfTimerInit();

    SystemHeapStart = (uint32_t)&SystemHeap;
    memset(SystemHeap, 0, sizeof(SystemHeap));
    // printf("SystemHeapStart = 0x%x\r\n", SystemHeapStart);
    // printf("SystemHeapSize = 0x%x\r\n", SystemHeapSize);
    bytesUsed = WsfBufInit(WSF_BUF_POOLS, mainPoolDesc);
    // printf("bytesUsed = 0x%x\r\n", bytesUsed);
    
#if WSF_TRACE_ENABLED
    WsfTraceRegisterHandler(myTrace);
#endif
    WsfTraceEnable(TRUE);
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
    // printf("***** ME14 SDMA LHCI *****\r\n");

    /* Initialize Radio */
    WsfInit();
    StackInit();

    // printf("SDMA Setup complete\r\n");

    // Signal that we're ready 
    interrupt_ARM();

    while(1)
    {
        // Poll for BLE interrupts
        BTLE_IRQHandler();

        wsfOsDispatcher();
    }
}
