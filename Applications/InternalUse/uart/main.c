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
 * @brief   UART test file.
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "uart_regs.h"
#include "uart.h"
#include "mxc_lock.h"
#include "gpio.h"
#include "mxc_pins.h"

//#include "led.h"

/***** Definitions *****/

#ifdef EMULATOR
#define UART_IDX    1
#else
#define UART_IDX    0
#endif

#define MXC_UARTn   MXC_UART_GET_UART(UART_IDX)
#define UARTn_IRQn  MXC_UART_GET_IRQ(UART_IDX)

#define UART_TEST_BAUD      115200
#define BUFF_SIZE           128

/***** Globals *****/
const uart_cfg_t uart_cfg = {
    .parity = UART_PARITY_DISABLE,
    .size = UART_DATA_SIZE_8_BITS,
    .stop = UART_STOP_1,
    .flow =UART_FLOW_CTRL_DIS,
    .pol =UART_FLOW_POL_DIS,
    .baud = UART_TEST_BAUD
};

volatile int read_flag;
volatile int write_flag;

uint8_t txdata[BUFF_SIZE];
uint8_t rxdata[BUFF_SIZE];

/***** Functions *****/

/******************************************************************************/
void read_cb(uart_req_t* req, int error)
{
    read_flag = error;
}

/******************************************************************************/
void write_cb(uart_req_t* req, int error)
{
    write_flag = error;
}

/******************************************************************************/
void UART0_IRQHandler(void)
{
    UART_Handler(MXC_UART0);
}

/******************************************************************************/
void UART1_IRQHandler(void)
{
    UART_Handler(MXC_UART1);
}

/******************************************************************************/
int init_test(void)
{
    int retval = 0;
    sys_cfg_uart_t uart_sys_cfg = NULL;    // There is no special system configuration parameters for UART

    // Setup the interrupt
    NVIC_ClearPendingIRQ(UARTn_IRQn);
    NVIC_DisableIRQ(UARTn_IRQn);
    NVIC_SetPriority(UARTn_IRQn, 1);
    NVIC_EnableIRQ(UARTn_IRQn);
  if (MXC_UARTn == MXC_UART0) {
    GPIO_Config(&gpio_cfg_uart0);
  } else if (MXC_UARTn == MXC_UART1) {
    GPIO_Config(&gpio_cfg_uart1);
  } else {
    return E_NO_DEVICE;
  }
    // Test UART_Shutdown
    UART_Shutdown(MXC_UARTn);

    // Test UART_Init
    if (UART_Init(MXC_UARTn, &uart_cfg, &uart_sys_cfg) != E_NO_ERROR) {
        printf("UART_Init() failed\n");
        retval = -1;
    }

    // Test UART_NumWriteAvail
    if(UART_NumWriteAvail(MXC_UARTn) != MXC_UART_FIFO_DEPTH) {
        printf("TXFIFO not initialized\n");
        retval = -1;
    }

    // Test UART_NumReadAvail
    if(UART_NumReadAvail(MXC_UARTn) != 0) {
        printf("RXFIFO not initialized\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int basic_blocking(void)
{
    int test_size = 8;
    int retval = 0;
    int error;
    UART_Write(MXC_UARTn, txdata, test_size);

    int num = 0;
    memset(rxdata, 0x0, test_size);
    if((error = UART_Read(MXC_UARTn, rxdata, test_size, &num)) != test_size) {
        printf("Read error : %d\n",error);
        retval = -1;
    }

    if(num != test_size) {
        printf("Data num incorrect %d\n", num);
    }

    if(memcmp(txdata, rxdata, test_size) != 0) {
        printf("Data mismatch\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int write_async(void)
{
    int test_size = 8;
    int retval = 0;

    uart_req_t write_req;
    write_req.data = txdata;
    write_req.len = test_size;
    write_req.callback = write_cb;
    write_flag = 1;

    int error = UART_WriteAsync(MXC_UARTn, &write_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async write %d\n", error);
        retval = -1;
    }

    while(write_flag == 1) {}
    if(write_flag != E_NO_ERROR) {
        printf("Error with UART_WriteAsync callback\n");
    }

    int num = 0;
    memset(rxdata, 0x0, test_size);
    error = UART_Read(MXC_UARTn, rxdata, test_size, &num);
    if(error != test_size) {
        printf("Read error %d\n", error);
        retval = -1;
    }

    if(num != test_size) {
        printf("Data num incorrect %d\n", num);
    }

    if(memcmp(txdata, rxdata, test_size) != 0) {
        printf("Data mismatch\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int read_async(void)
{
    int test_size = 7;
    int retval = 0;

    uart_req_t read_req;
    read_req.data = rxdata;
    read_req.len = test_size;
    read_req.callback = read_cb;

    read_flag = 1;

    memset(rxdata, 0x0, BUFF_SIZE);
    int error = UART_ReadAsync(MXC_UARTn, &read_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        retval = -1;
    }

    UART_Write(MXC_UARTn, txdata, test_size);

    while(read_flag == 1) {}
    if(read_flag != E_NO_ERROR) {
        printf("Error with UART_ReadAsync callback\n");
    }

    if(read_req.num != test_size) {
        printf("Data num incorrect %d\n", read_req.num);
        retval = -1;
    }

    if(memcmp(txdata, rxdata, test_size) != 0) {
        printf("Data mismatch\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int long_async(void)
{
    int test_size = BUFF_SIZE;
    int retval = 0;

    uart_req_t read_req;
    read_req.data = rxdata;
    read_req.len = test_size;
    read_req.callback = read_cb;

    uart_req_t write_req;
    write_req.data = txdata;
    write_req.len = test_size;
    write_req.callback = write_cb;

    read_flag = 1;
    write_flag = 1;
    memset(rxdata, 0xff, BUFF_SIZE);

    int error = UART_ReadAsync(MXC_UARTn, &read_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        retval = -1;
    }

    error = UART_WriteAsync(MXC_UARTn, &write_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async write %d\n", error);
        retval = -1;
    }

    while(write_flag == 1) {}
    if(write_flag != E_NO_ERROR) {
        printf("Error with UART_WriteAsync callback\n");
    }

    while(read_flag == 1) {}
    if(read_flag != E_NO_ERROR) {
        printf("Error with UART_ReadAsync callback %d\n", read_flag);
    }

    if(read_req.num != test_size) {
        printf("Data read num incorrect %d\n", read_req.num);
        retval = -1;
    }

    if(write_req.num != test_size) {
        printf("Data write num incorrect %d\n", read_req.num);
        retval = -1;
    }

    if(memcmp(txdata, rxdata, test_size) != 0) {
        printf("Data mismatch\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int abort_async(void)
{
    int test_size = 60;
    int retval = 0;

    uart_req_t read_req;
    read_req.data = rxdata;
    read_req.len = test_size;
    read_req.callback = read_cb;

    uart_req_t write_req;
    write_req.data = txdata;
    write_req.len = test_size;
    write_req.callback = write_cb;

    read_flag = 1;
    write_flag = 1;

    int error = UART_ReadAsync(MXC_UARTn, &read_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        retval = -1;
    }

    error = UART_WriteAsync(MXC_UARTn, &write_req);
    if(error != E_NO_ERROR) {
        printf("Error starting async write %d\n", error);
        retval = -1;
    }

    error = UART_AbortAsync(&write_req);
    if(error != E_NO_ERROR) {
        printf("Error aborting write %d\n", error);
        retval = -1;
    }

    error = UART_AbortAsync(&read_req);
    if(error != E_NO_ERROR) {
        printf("Error aborting read %d\n", error);
        retval = -1;
    }

    if(read_flag != E_ABORT) {
        printf("Error aborting UART_ReadAsync %d\n", read_flag);
        retval = -1;
    }

    if(write_flag != E_ABORT) {
        printf("Error aborting UART_WriteAsync %d\n", read_flag);
        retval = -1;
    }
    while(UART_Busy(MXC_UARTn));
    UART_DrainRX(MXC_UARTn);
    UART_DrainTX(MXC_UARTn);

    // Check to make sure that FIFOs are empty
    if(UART_NumReadAvail(MXC_UARTn) != 0) {
        printf("Error bytes still in RXFIFO\n");
        retval = -1;
    }

    if(UART_NumWriteAvail(MXC_UARTn) != MXC_UART_FIFO_DEPTH) {
        printf("Error bytes in TXFIFO\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int main(void)
{
    int retval = 0;
    int i;

    printf("\n\n***** UART Driver test *****\n");
    printf("Testing UART%u @%08x with IRQ%u\n", UART_IDX, MXC_UARTn, UARTn_IRQn);

    // Initialize test data
    for(i = 0; i < BUFF_SIZE; i++) {
        txdata[i] = 0x30 + i;
    }
    uint32_t lock;
    mxc_get_lock(&lock, 1);
    mxc_free_lock(&lock);

#if 1
    if(init_test() != 0) {
        retval = -1;
    }
#endif

#if 1
    if(basic_blocking() != 0) {
        retval = -1;
    }
#endif

#if 1
    if(write_async() != 0) {
        retval = -1;
    }
#endif

#if 1
    if(read_async() != 0) {
        retval = -1;
    }
#endif

#if 1
    if(abort_async() != 0) {
        retval = -1;
    }
#endif

#if 1
    if(long_async() != 0) {
        retval = -1;
    }
#endif

    if(retval != 0) {
        printf("You are not prepared..\n");
        //LED_On(2);
    } else {
        printf("Tests Passed!\n");
        //LED_On(3);
    }
    while(1) {}
    return 0;
}