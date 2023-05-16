/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2018-08-10 09:36:39 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Template main file
 * @details Use this as a starting point for new API testing files. Describe how
 *          to execute the test if there is any hardware setup.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "max32665.h"
#include "uart.h"

/***** Definitions *****/

// #define printf(...)  // Using this until printf working
#define UARTp MXC_UART1
#define UARTs MXC_UART2

/***** Globals *****/
sys_cfg_uart_t uartp_sys_cfg;
sys_cfg_uart_t uarts_sys_cfg;

uart_cfg_t uartp_cfg;
uart_cfg_t uarts_cfg;

uint8_t datap[256];
uint8_t datas[256];

volatile int UARTs_AsyncFlag = 0;
volatile int UARTs_AsyncRetVal = 0;

volatile int tx_threshold_flag = 0;

/***** Functions *****/

/******************************************************************************/
void UART1_IRQHandler(void) 
{
    if(UART_GetFlags(MXC_UART1) & MXC_F_UART_INT_FL_TX_FIFO_THRESH) {
        tx_threshold_flag++;
    }

    UART_Handler(MXC_UART1);
}

/******************************************************************************/
void UART2_IRQHandler(void) 
{
    UART_Handler(MXC_UART2);
}

/******************************************************************************/
void UARTp_Callback(uart_req_t* req, int error)
{
    return;
}

/******************************************************************************/
void UARTs_Callback(uart_req_t* req, int error)
{
    UARTs_AsyncFlag++;
    UARTs_AsyncRetVal = error;
    return;
}

/******************************************************************************/
int init(void)
{
    int retval = 0;

    // Setup the interrupt
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_EnableIRQ(UART2_IRQn);

    // Enable and configure the peripheral
    uartp_sys_cfg.map = MAP_A;
    uartp_sys_cfg.flow = Disable;

    uartp_cfg.parity = UART_PARITY_DISABLE;
    uartp_cfg.size = UART_DATA_SIZE_8_BITS;
    uartp_cfg.stop = UART_STOP_1;
    uartp_cfg.flow = UART_FLOW_CTRL_DIS;
    uartp_cfg.pol = UART_FLOW_POL_DIS;
    uartp_cfg.baud = 115200;
    uartp_cfg.clksel = UART_CLKSEL_SYSTEM;

    retval += UART_Init(UARTp, &uartp_cfg, &uartp_sys_cfg);

    uarts_sys_cfg.map = MAP_A;
    uarts_sys_cfg.flow = Disable;

    uarts_cfg.parity = UART_PARITY_DISABLE;
    uarts_cfg.size = UART_DATA_SIZE_8_BITS;
    uarts_cfg.stop = UART_STOP_1;
    uarts_cfg.flow = UART_FLOW_CTRL_DIS;
    uarts_cfg.pol = UART_FLOW_POL_DIS;
    uarts_cfg.baud = 115200;
    uarts_cfg.clksel = UART_CLKSEL_SYSTEM;

    retval += UART_Init(UARTs, &uarts_cfg, &uarts_sys_cfg);

    return retval;
}

/******************************************************************************/
int test0(void)
{
    int retval = 0;
    int err = 0;
    int timeout = 0;
    int tx_threshold = 5;
    int test_len = 256;

    uart_req_t uarts_rx;
    uarts_rx.data = datas;
    uarts_rx.len = test_len;
    uarts_rx.callback = UARTs_Callback;

    uart_req_t uartp_tx;
    uartp_tx.data = datap;
    uartp_tx.len = test_len;

    for(int i = 0; i < 256; i++) {
        datap[i] = 255-i;
    }

    tx_threshold_flag = 0;
    MXC_UART1->thresh_ctrl &= ~MXC_F_UART_THRESH_CTRL_TX_FIFO_THRESH;
    MXC_UART1->thresh_ctrl |= (tx_threshold << MXC_F_UART_THRESH_CTRL_TX_FIFO_THRESH_POS) & MXC_F_UART_THRESH_CTRL_TX_FIFO_THRESH;
    // MXC_UART1->int_en |= MXC_F_UART_INT_EN_TX_FIFO_THRESH;

    printf("test0: Starting Read Async request\n");
    retval += UART_ReadAsync(UARTs, &uarts_rx);
    printf("test0: Starting write request\n");
    retval += (err = UART_WriteAsync(UARTp, &uartp_tx)) == 256 ? 0 : err;
    printf("test0: Write Complete\n");

    for(timeout = 0; timeout < 10000; timeout++) {
        if(UARTs_AsyncFlag != 0) {
            retval += (err = UARTs_AsyncRetVal) == 256 ? 0 : err;
            break;
        }
    }

    if(retval != 0 || timeout == 10000) {
        printf("test0: UART Read/Write Failed\n");
        UART_AbortAsync(&uarts_rx);
        return retval - 50;
    }

    retval += memcmp(datap, datas, test_len) == 0 ? 0 : -100;

    retval += tx_threshold_flag > 0 ? 0 : -500;
    printf("TX Threshold Fired %d times\n", tx_threshold_flag);

    return retval;
}

// *****************************************************************************
int main(void)
{

    int error = 0;

    printf("***** UART Driver test *****\n");

    // Use init to initialize the peripherals
    if(init() == E_NO_ERROR) {

        // Partition the test into smaller units to make debugging easier
        error += test0();
        if(error) {
            printf("test0 failed with %d!\n\n", error);
        } else {
            printf("test0 passed\n\n");
        }

    } else {
        error = -1;        
    }

    if(error) {
        printf("Test Failed!\n");        
    }
    else {
        printf("Test Passed\n");
    }

    while(1) {}

}
