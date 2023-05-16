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
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "uart_regs.h"
#include "tmr_utils.h"

/***** Definitions *****/

// #define printf(...)  // Using this until printf working

/***** Globals *****/
volatile int written = 0;
volatile uint8_t txdata[256];
volatile int cb_flag = 0;

mxc_uart_regs_t *uart = MXC_UART1;

/***** Functions *****/

/******************************************************************************/
void UART1_IRQHandler(void) 
{
    if(uart->int_fl & MXC_F_UART_INT_FL_TX_FIFO_THRESH) {
        int spots = 32-((uart->status & MXC_F_UART_STATUS_TX_FIFO_CNT) >> MXC_F_UART_STATUS_TX_FIFO_CNT_POS);

        cb_flag++;

        while(written != 256 && spots > 0) {
            uart->fifo = txdata[written];
            written++;
            spots--;
        }

    }
    uart->int_fl = uart->int_fl;
    return;
}

// *****************************************************************************
void do_transaction(int threshold) {

    uart->thresh_ctrl &= ~MXC_F_UART_THRESH_CTRL_TX_FIFO_THRESH;
    uart->thresh_ctrl |= (threshold << MXC_F_UART_THRESH_CTRL_TX_FIFO_THRESH_POS) & MXC_F_UART_THRESH_CTRL_TX_FIFO_THRESH;

    uart->int_fl = uart->int_fl;

    for(written = 0; written<32; written++) {
        while(uart->status & MXC_F_UART_STATUS_TX_FULL);
        uart->fifo = txdata[written];
    }

    NVIC_EnableIRQ(UART1_IRQn);
}

// *****************************************************************************
int main(void)
{
    printf("\n\n");

    sys_cfg_uart_t uart_sys_cfg;
    int tx_thresh = 15;

    uart_sys_cfg.map = MAP_A;
    uart_sys_cfg.flow = Disable;

    // Initialize UART1 to send data
    SYS_UART_Init(MXC_UART1, &uart_sys_cfg);

    uart->ctrl = 0;
    uart->ctrl |= MXC_S_UART_CTRL_CHAR_SIZE_8;
    uart->ctrl |= MXC_F_UART_CTRL_TX_FLUSH | MXC_F_UART_CTRL_RX_FLUSH;

    uart->baud0 = ((2 << MXC_F_UART_BAUD0_FACTOR_POS) | 1);
    uart->baud1 = 2;

    uart->ctrl |= MXC_F_UART_CTRL_ENABLE;
    
    uart->int_en = MXC_F_UART_INT_FL_TX_FIFO_THRESH;
    NVIC_EnableIRQ(UART1_IRQn);

    for(int i = 0; i < 256; i++) {
        txdata[i] = 255-i;
    }

    for(tx_thresh = 1; tx_thresh < 31; tx_thresh++) {
        cb_flag = 0;
        do_transaction(tx_thresh);
        while(written != 256);
        NVIC_DisableIRQ(UART1_IRQn);
        TMR_Delay(MXC_TMR0, USEC(10000), NULL);
        printf("Did tx_thresh = %02d - Called %02d times\n", tx_thresh, cb_flag);
    }

    printf("Complete\n");

    while(1) {}
}