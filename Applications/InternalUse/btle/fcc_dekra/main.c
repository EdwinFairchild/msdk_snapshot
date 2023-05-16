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
#include "ll_api.h"
#include "hci_vs.h"

#include "uart.h"
#include "board.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/* Number of WSF buffer pools */
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
static wsfBufPoolDesc_t mainPoolDesc[WSF_BUF_POOLS] =
{
  {  16,  8 },
  {  32,  4 },
  {  64,  4 },
  { 128,  4 },
  { 256,  4 },
  { 512,  4 }
};

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief  Stack initialization for app. */
extern void StackInit(void);

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

void usage(void)
{
    printf("Usage: \n\n");
    printf(" (0) Transmit on RF channel 0 (2402 MHz)\n");
    printf(" (1) Transmit on RF channel 19 (2440 MHz)\n");
    printf(" (2) Transmit on RF channel 39 (2480 MHz)\n");
    printf(" (3) Set Transmit power\n");
    printf(" (e) End transmission -- MUST be used between tests\n\n");
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
    uint8_t res;
    unsigned char inp;
    mxc_uart_regs_t * ConsoleUART = MXC_UART_GET_UART(CONSOLE_UART);

#ifndef __IAR_SYSTEMS_ICC__
    setvbuf(stdout, printf_buffer, _IOLBF, PRINTF_BUF_SIZE);
#endif

    printf("\n\n***** MAX32665 BLE HCI UART *****\n");

    WsfInit();
    StackInit();

    printf("Setup Complete\n");
    usage();

    while (1)
    {
        wsfOsDispatcher();
        if (UART_NumReadAvail(ConsoleUART)) {
            /* Process key press */
            inp = UART_ReadByte(ConsoleUART);
            switch (inp) {
                case '0':
                    printf("Transmit RF channel 0, 255 bytes/pkt, 0xAA, 1Msym/sec, forever ..\n");
                    res = LlEnhancedTxTest(0, 255, LL_TEST_PKT_TYPE_AA, LL_PHY_LE_1M, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;

                case '1':
                    printf("Transmit RF channel 19, 255 bytes/pkt, 0xAA, 1Msym/sec, forever ..\n");
                    res = LlEnhancedTxTest(19, 255, LL_TEST_PKT_TYPE_AA, LL_PHY_LE_1M, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;
                    
                case '2':
                    printf("Transmit RF channel 39, 255 bytes/pkt, 0xAA, 1Msym/sec, forever ..\n");
                    res = LlEnhancedTxTest(39, 255, LL_TEST_PKT_TYPE_AA, LL_PHY_LE_1M, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;

                case '3':
                    printf("Select transmit power\n");
                    printf(" 0: -10 dBm\n");
                    printf(" 1:   0 dBm\n");
                    printf(" 2: 4.5 dBm\n");
                    inp = UART_ReadByte(ConsoleUART);

                    switch (inp) {
                        case '0':
                            dbb_set_rfpower(-10);
                            printf("Power set to -10 dBm\n");
                            break;
                        case '1':
                            dbb_set_rfpower(0);
                            printf("Power set to 0 dBm\n");
                            break;
                        case '2':
                            dbb_set_rfpower(4);
                            printf("Power set to 4.5 dBm\n");
                            break;
                        default:
                            printf("Invalid selection\n");
                            break;
                    }
                    break;
                    
                case 'E':
                case 'e':
                    printf("End test\n");
                    res = LlEndTest(NULL);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;
                    
                default:
                    break;
            }

            usage();
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
