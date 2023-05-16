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
 * @brief   SPIM test file.
 * @details Put a jumper across P0.5 and P0.6 to loopback MOSI to MISO
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "uart_regs.h"
#include "spim.h"
#include "mxc_lock.h"
#include "tmr_utils.h"

/***** Definitions *****/
#define BUFF_SIZE   512

/***** Globals *****/
volatile int spim_flag;

uint8_t txdata[BUFF_SIZE];
uint8_t rxdata[BUFF_SIZE];

/***** Functions *****/

/******************************************************************************/
void spim_cb(spim_req_t* req, int error)
{
    spim_flag = error;
}

/******************************************************************************/
void SPI0_IRQHandler(void)
{
    SPIM_Handler(MXC_SPIM0);
}


/******************************************************************************/
int init_test(void)
{
    int retval = 0;

    // Setup the interrupt
    NVIC_ClearPendingIRQ(MXC_SPIM_GET_IRQ(0));
    NVIC_DisableIRQ(MXC_SPIM_GET_IRQ(0));
    NVIC_SetPriority(MXC_SPIM_GET_IRQ(0), 1);
    NVIC_EnableIRQ(MXC_SPIM_GET_IRQ(0));

    // Test SPIM_Shutdown
    SPIM_Shutdown(MXC_SPIM0);
    // Test SPIM_Init
    spim_cfg_t cfg;
    cfg.mode = 0;
    cfg.ssel_pol = 0;
    cfg.baud = 1000000;
    if (SPIM_Init(MXC_SPIM0, &cfg, NULL) != E_NO_ERROR) {
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int read_write_blocking(void)
{
    int test_size = 68;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = txdata;
    req.rx_data     = rxdata;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = NULL;

    if(SPIM_Trans(MXC_SPIM0, &req) != test_size) {
        printf("Read error\n");
        return -1;
    }

    if(req.read_num != test_size) {
        printf("Data read_num incorrect %d\n", req.read_num);
        retval = -1;
    }

    if(req.write_num != test_size) {
        printf("Data write_num incorrect %d\n", req.write_num);
        retval = -1;
    }

    if(memcmp(txdata, rxdata, test_size) != 0) {
        printf("Data mismatch\n");
        retval = -1;
    }
    printf("DONE\n" );
    return retval;
}

/******************************************************************************/
int read_blocking(void)
{
    int test_size = 36;
    int error;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = NULL;
    req.rx_data     = rxdata;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = NULL;

    if((error = SPIM_Trans(MXC_SPIM0, &req)) != test_size) {
        printf("Read error %d\n", error);
        return -1;
    }

    if(req.write_num != 0) {
        printf("Data num incorrect %d\n", req.write_num);
        retval = -1;
    }

    if(req.read_num != test_size) {
        printf("Data num incorrect %d\n", req.read_num);
        retval = -1;
    }

    int i;
    for(i = 0; i < test_size; i++) {
        if(rxdata[i] != 0xFF) {
            printf("Data mismatch\n");
            retval = -1;
        }
    }

    return retval;
}

/******************************************************************************/
int write_blocking(void)
{
    int test_size = 500;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = txdata;
    req.rx_data     = NULL;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = NULL;

    if(SPIM_Trans(MXC_SPIM0, &req) != test_size) {
        printf("Read error\n");
        return -1;
    }

    if(req.read_num != 0) {
        printf("Data num incorrect %d\n", req.read_num);
        retval = -1;
    }

    if(req.write_num != test_size) {
        printf("Data num incorrect %d\n", req.write_num);
        retval = -1;
    }

    int i;
    for(i = 0; i < test_size; i++) {
        if(rxdata[i] != 0) {
            printf("Data mismatch\n");
            retval = -1;
        }
    }

    return retval;
}

/******************************************************************************/
int read_write_async(void)
{
    int test_size = 8;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = txdata;
    req.rx_data     = rxdata;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = spim_cb;

    spim_flag = 1;
    if(SPIM_TransAsync(MXC_SPIM0, &req) != E_NO_ERROR) {
        printf("SPIM_TransAsync error\n");
        return -1;
    }

    while(spim_flag == 1) {}
    if(spim_flag != E_NO_ERROR) {
        printf("Error with SPIM_TransAsync callback %d\n", spim_flag);
        retval = -1;
    }


    if(req.write_num != test_size) {
        printf("Data num incorrect %d\n", req.write_num);
        retval = -1;
    }

    if(req.read_num != test_size) {
        printf("Data num incorrect %d\n", req.read_num);
        retval = -1;
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
    int test_size = 36;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = txdata;
    req.rx_data     = NULL;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = spim_cb;

    spim_flag = 1;
    if(SPIM_TransAsync(MXC_SPIM0, &req) != E_NO_ERROR) {
        printf("SPIM_TransAsync error\n");
        return -1;
    }

    while(spim_flag == 1) {}
    if(spim_flag != E_NO_ERROR) {
        printf("Error with SPIM_TransAsync callback\n");
        retval = -1;
    }


    if(req.write_num != test_size) {
        printf("Data num incorrect %d\n", req.write_num);
        retval = -1;
    }

    if(req.read_num != 0) {
        printf("Data num incorrect %d\n", req.read_num);
        retval = -1;
    }

    int i;
    for(i = 0; i < test_size; i++) {
        if(rxdata[i] != 0) {
            printf("Data mismatch\n");
            retval = -1;
        }
    }

    return retval;
}


/******************************************************************************/
int read_async(void)
{
    int test_size = 56;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = NULL;
    req.rx_data     = rxdata;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = spim_cb;

    spim_flag = 1;
    if(SPIM_TransAsync(MXC_SPIM0, &req) != E_NO_ERROR) {
        printf("SPIM_TransAsync error\n");
        return -1;
    }

    while(spim_flag == 1) {}
    if(spim_flag != E_NO_ERROR) {
        printf("Error with SPIM_TransAsync callback\n");
        retval = -1;
    }


    if(req.write_num != 0) {
        printf("Data num incorrect %d\n", req.write_num);
        retval = -1;
    }

    if(req.read_num != test_size) {
        printf("Data num incorrect %d\n", req.read_num);
        retval = -1;
    }

    int i;
    for(i = 0; i < test_size; i++) {
        if(rxdata[i] != 0xFF) {
            printf("Data mismatch\n");
            retval = -1;
        }
    }

    return retval;
}

/******************************************************************************/
int long_async(void)
{
    int test_size = 200;
    int retval = 0;

    memset(rxdata, 0x0, BUFF_SIZE);

    spim_req_t req;
    req.ssel        = 0;
    req.deass       = 1;
    req.tx_data     = txdata;
    req.rx_data     = rxdata;
    req.len         = test_size;
    req.read_num    = 0;
    req.write_num   = 0;
    req.width       = SPIM_WIDTH_1;
    req.callback    = spim_cb;

    spim_flag = 1;
    if(SPIM_TransAsync(MXC_SPIM0, &req) != E_NO_ERROR) {
        printf("SPIM_TransAsync error\n");
        return -1;
    }

    while(spim_flag == 1) {}
    if(spim_flag != E_NO_ERROR) {
        printf("Error with SPIM_TransAsync callback\n");
        retval = -1;
    }


    if(req.write_num != test_size) {
        printf("Data write_num incorrect %d\n", req.write_num);
        retval = -1;
    }

    if(req.read_num != test_size) {
        printf("Data read_num incorrect %d\n", req.read_num);
        retval = -1;
    }

    if(memcmp(txdata, rxdata, test_size) != 0) {
        printf("Data mismatch\n");
        retval = -1;
    }

    return retval;
}

/******************************************************************************/
int main(void)
{
    int retval = 0;
    int i;

    printf("***** SPIM Driver test *****\n");

    // Initialize test data
    for(i = 0; i < BUFF_SIZE; i++) {
        txdata[i] = i;
    }

    uint32_t lock;
    mxc_get_lock(&lock, 1);
    mxc_free_lock(&lock);

#if 1
    if(init_test() != 0) {
        retval = -1;
        while(1) {}
    }
#endif

#if 1
    if(read_write_blocking() != 0) {
        retval = -1;
        printf("read_write_blocking error\n");
    }
#endif

#if 1
    TMR_Delay(MXC_TMR0, USEC(100));
    if(read_blocking() != 0) {
        retval = -1;
        printf("read_blocking error\n");
    }
#endif

#if 1
    TMR_Delay(MXC_TMR0, USEC(100));
    if(write_blocking() != 0) {
        retval = -1;
        printf("write_blocking error\n");
    }
#endif

#if 1
    TMR_Delay(MXC_TMR0, USEC(100));
    if(read_write_async() != 0) {
        retval = -1;
        printf("read_write_async error\n");
    }
#endif

#if 1
    TMR_Delay(MXC_TMR0, USEC(100));
    if(write_async() != 0) {
        retval = -1;
        printf("write_async error\n");
    }
#endif

#if 1
    TMR_Delay(MXC_TMR0, USEC(100));
    if(read_async() != 0) {
        retval = -1;
        printf("read_async error\n");
    }
#endif

#if 1
    TMR_Delay(MXC_TMR0, USEC(100));
    if(long_async() != 0) {
        retval = -1;
        printf("long_async error\n");
    }
#endif

    if(retval != 0) {
        printf("You are not prepared..\n");
    } else {
        printf("Tests Passed!\n");
    }

    while(1) {}
    return 0;
}
