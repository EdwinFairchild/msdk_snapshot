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
 * $Date: 2019-12-06 15:25:52 -0600 (Fri, 06 Dec 2019) $
 * $Revision: 49605 $
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
#include "tmr.h"
#include "tmr_utils.h"
#include "led.h"

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

int pa_value = -1;

static uint8_t phy = LL_PHY_LE_1M;
static uint8_t txFreqHopCh;

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

/*! \brief Sets the transmit power. */
extern void llc_api_set_txpower(int power);

void TMR1_IRQHandler(void)
{
    int res;

    TMR_IntClear(MXC_TMR1);

    res = LlEnhancedTxTest(txFreqHopCh++, 255, LL_TEST_PKT_TYPE_AA, phy, 0);
    if(res != LL_SUCCESS) {
        printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");        
    }

    if(txFreqHopCh == 40) {
        txFreqHopCh = 0;
    }
    TMR_TO_Start(MXC_TMR1, 20000, NULL);
}

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

/*************************************************************************************************/
/*!
 *  \fn     usage
 *
 *  \brief  Prints the usage statement.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void usage(void)
{
    printf("Usage: \n\n");
    printf(" (0) Transmit on RF channel 0 (2402 MHz)\n");
    printf(" (1) Transmit on RF channel 19 (2440 MHz)\n");
    printf(" (2) Transmit on RF channel 39 (2480 MHz)\n");
    printf(" (3) Receive  on RF channel 39 (2480 MHz)\n");
    printf(" (4) Set Transmit power\n");
    printf(" (5) Enable constant TX\n");
    printf(" (6) Disable constant TX -- MUST be called after (5)\n");
    printf(" (7) Set PA value\n");
    printf(" (8) Set PHY\n");
    printf(" (9) TX Frequency Hop\n");
    printf(" (e) End transmission -- MUST be used after each (0-3, 9)\n");
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

    printf("\n\n***** MAX32665 BLE FCC *****\n");

    /* Drive GPIO low to prevent accidental RF radiation */
    MXC_GPIO0->out = 0;
    MXC_GPIO0->en = 0xFFFFFFFF;
    MXC_GPIO0->out_en = 0xFFFFFFFF;

    MXC_GPIO1->out = 0;
    MXC_GPIO1->en = 0xFFFFFFFF;
    MXC_GPIO1->out_en = 0xFFFFFFFF;

    Console_Init();
    
    /* Initialize Radio */
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
                    res = LlEnhancedTxTest(0, 255, LL_TEST_PKT_TYPE_AA, phy, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;

                case '1':

                    printf("Transmit RF channel 19, 255 bytes/pkt, 0xAA, %dMsym/sec, forever ..\n",  phy);
                    res = LlEnhancedTxTest(19, 255, LL_TEST_PKT_TYPE_AA, phy, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;
                    
                case '2':

                    printf("Transmit RF channel 39, 255 bytes/pkt, 0xAA, %dMsym/sec, forever ..\n", phy);
                    res = LlEnhancedTxTest(39, 255, LL_TEST_PKT_TYPE_AA, phy, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;

                case '3':

                    printf("Receive RF channel 39, 1Msym/sec, forever ..\n");
                    res = LlEnhancedRxTest(39, phy, 0, 0);
                    printf("res = %u %s\n", res, res == LL_SUCCESS ? "(SUCCESS)" : "(FAIL)");
                    break;

                case '4':
                    printf("Select transmit power\n");
                    printf(" 0: -10 dBm\n");
                    printf(" 1:   0 dBm\n");
                    printf(" 2: 4.5 dBm\n");
                    inp = UART_ReadByte(ConsoleUART);

                    switch (inp) {
                        case '0':
                            llc_api_set_txpower(-10);
                            printf("Power set to -10 dBm\n");
                            break;
                        case '1':
                            llc_api_set_txpower(0);
                            printf("Power set to 0 dBm\n");
                            break;
                        case '2':
                            llc_api_set_txpower(4);
                            printf("Power set to 4.5 dBm\n");
                            break;
                        default:
                            printf("Invalid selection\n");
                            break;
                    }
                    break;

                case '5':
                    printf("Select transmit channel\n");
                    printf(" 0: 0\n");
                    printf(" 1: 19\n");
                    printf(" 2: 39\n");
                    inp = UART_ReadByte(ConsoleUART);

                    switch (inp) {
                        case '0':
                            dbb_seq_select_rf_channel(0);
                            printf("Channel set to 0\n");
                            break;
                        case '1':
                            dbb_seq_select_rf_channel(19);
                            printf("Channel set to 19\n");
                            break;
                        case '2':
                            dbb_seq_select_rf_channel(39);
                            printf("Channel set to 39\n");
                            break;
                        default:
                            printf("Invalid selection\n");
                            break;
                    }
					
                    llc_api_tx_ldo_setup();

					/* Enable constant TX */
                    dbb_seq_tx_enable();
                    break;

                case '6':
					
					/* Disable constant TX */
                    dbb_seq_tx_disable();
                    break;

                case '7':
                    /* Set PA value */
                    printf("Select transmit PA value, one hex digit 0-f\n");
                    inp = UART_ReadByte(ConsoleUART);

                    switch (inp) {
                        case '0': pa_value = 0x0; break;
                        case '1': pa_value = 0x1; break;
                        case '2': pa_value = 0x2; break;
                        case '3': pa_value = 0x3; break;
                        case '4': pa_value = 0x4; break;
                        case '5': pa_value = 0x5; break;
                        case '6': pa_value = 0x6; break;
                        case '7': pa_value = 0x7; break;
                        case '8': pa_value = 0x8; break;
                        case '9': pa_value = 0x9; break;
                        case 'a': pa_value = 0xa; break;
                        case 'b': pa_value = 0xb; break;
                        case 'c': pa_value = 0xc; break;
                        case 'd': pa_value = 0xd; break;
                        case 'e': pa_value = 0xe; break;
                        case 'f': pa_value = 0xf; break;
                        default:
                            printf("Invalid selection\n");
                            break;
                    }
                    printf("pa_value = 0x%x\n", pa_value);

                    dbb_seq_set_pa_value(pa_value);
                    
                    break;

                case '8':
                    /* Set the PHY */
                    printf("Select PHY\n");
                    printf("1: 1M\n");
                    printf("2: 2M\n");
                    printf("3: S2\n");
                    printf("4: S8\n");


                    inp = UART_ReadByte(ConsoleUART);

                    switch (inp) {
                        case '1':
                            phy = LL_TEST_PHY_LE_1M;
                            printf("PHY set to 1M\n");
                            break;
                        case '2':
                            phy = LL_TEST_PHY_LE_2M;
                            printf("PHY set to 2M\n");
                            break;
                        case '3':
                            phy = LL_TEST_PHY_LE_CODED_S2;
                            printf("PHY set to S2\n");
                            break;
                        case '4':
                            phy = LL_TEST_PHY_LE_CODED_S8;
                            printf("PHY set to S8\n");
                            break;
                        default:
                            printf("Invalid selection\n");
                            break;
                    }
                    
                    break;
                case '9':
                    /* Frequency hopping TX */
                    printf("Starting frequency hopping\n");
                    NVIC_EnableIRQ(TMR1_IRQn);
                    TMR_TO_Start(MXC_TMR1, 20000, NULL);
                    break;
                    
                case 'E':
                case 'e':

                    printf("End test\n");
                    TMR_TO_Stop(MXC_TMR1);
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
