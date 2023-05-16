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
#include "hci_handler.h"
#include "lhci_api.h"
#include "app_api.h"
#include "bb_ble_api.h"
#include "sch_api.h"
#include "ll_api.h"
#include "mcr_regs.h"
#include "mxc_config.h"
#include "gcr_regs.h"
#include "uart_regs.h"
#include "ll_debug.h"
#include "board.h"
#include "uart.h"
#include "bb_ble_api.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
#define WSF_BUF_POOLS              6

/* Size of buffer for stdio functions */
#define PRINTF_BUF_SIZE 128

#define LED_COUNT                   400000

#define MXC_R_SIR_SHR4          *((volatile uint32_t*)(0x40000410))
#define MXC_R_TM                *((volatile uint32_t*)(0x40000C00))
#define MXC_R_TMR3              *((volatile uint32_t*)(0x40000C18))

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! Buffer for stdio functions */
char printf_buffer[PRINTF_BUF_SIZE];

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
 *  \brief  Initialize platform.
 *
 *  \param  msPerTick   Milliseconds per timer tick.
 *
 *  \return None.
 */
/*************************************************************************************************/
void mainPlatformInit(void)
{
    /* Change the pullup on the RST pin to 25K */
    MXC_MCR->ctrl = 0x202;
    
    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D | MXC_F_GCR_PERCKCN0_SPI1D);  // Clear GPIO0 and GPIO1 Disable
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD );  // Clear BTLE and ICACHE0 disable
    
    /* setup the systick */
    SysTick->LOAD = (SystemCoreClock / 1000) * WSF_MS_PER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL |= (SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
    WsfTimerInit();
    WsfBufInit(sizeof(mainBufMem), (uint8_t*)mainBufMem, WSF_BUF_POOLS, mainPoolDesc);
    WsfTraceRegister(myTrace);

    /* Added to prevent printf() and friends from using malloc() */
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);

    // Setup UART1 to use 3.3V on VDDIOH
    MXC_GPIO0->vssel |= ((0x1 << 21) | (0x1 << 20));

    // Enable test mode
    MXC_R_TM = 0x1;

    // Enable BLE  if disabled by test
    MXC_R_SIR_SHR4 = 0x00100010;

    MXC_R_TMR3 = 0;

    // Setup GPIO for LHCI UART1, AF3
    MXC_GPIO0->en_clr =     ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->en1_clr =    ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->en2_set =    ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->pad_cfg1 &= ~((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->pad_cfg2 &= ~((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));

    // Enable GPIO0 and UART1 clocks
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_UART1D);

    NVIC_EnableIRQ(UART1_IRQn);
}

/*************************************************************************************************/
extern void LHCI_Handler(void);
void UART1_IRQHandler(void)
{
  LHCI_Handler();
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
void mainMacInit(void)
{
    wsfHandlerId_t handlerId;

    HciVsSetTxPower(-10);

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
    bdAddr_t bdAddr = {0x56,0x34,0x12,0x8B,0x05,0x00};

    printf("\n***** MAX32665 IUT *****\n");

    printf("Starting init\n");

    mainPlatformInit();

    mainMacInit();

    LlSetBdAddr((uint8_t*)&bdAddr);
    printf("MAC %02X:%02X:%02X:%02X:%02X:%02X\n", bdAddr[5], bdAddr[4], bdAddr[3], bdAddr[2], bdAddr[1], bdAddr[0]);
    
    printf("\nSetup Complete\n");

    mxc_uart_regs_t* debug_uart = MXC_UART_GET_UART(CONSOLE_UART);
    unsigned num_avail;

    while (1)
    {
        wsfOsDispatcher();

        // Some kind of non-blocking char get to print the ll_trace
        // if uart_getchar, print trace CONSOLE_UART
        num_avail = UART_NumReadAvail(debug_uart);
        if(num_avail) {
            UART_DrainRX(debug_uart);
            num_avail = 0;

            ll_print_trace();
        }
    }
}

/*****************************************************************/
void HardFault_Handler(void)
{
    volatile int led_counter;
    printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

    // Loop forever
    while(1) {
        if(led_counter++ == LED_COUNT) {
            led_counter = 0;
        }
    }
}
