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
 * @brief   New SPIM3 basic test!
 * @details Having to delay between AHB transactions when configuring SPI3.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "led.h"
#include "mxc_pins.h"
#include "spi_regs.h"
#include "tmr_utils.h"
#include "spi.h"
#include "mx25.h"

/***** Definitions *****/
#define TEST_SPI        MXC_SPI1
#define TEST_SPI_GPIO   &gpio_cfg_spi1

#define TEST_LEN        60
#define TEST_VALUE      0xFFFF
#define ASYNC           1
/***** Globals *****/
uint16_t rx_data[TEST_LEN];
uint16_t tx_data[TEST_LEN];
volatile int spi_flag;
volatile int irqe;
/***** Functions *****/
/******************************************************************************/
void spi_cb(spi_req_t* req, int error)
{
    spi_flag = error;
}

/******************************************************************************/
void SPI1_IRQHandler(void)
{   
    SPI_Handler(TEST_SPI);
}
// *****************************************************************************
int main(void)
{
    int i,j;
    uint16_t temp;
    spi_req_t req;
    
    // // Setup the interrupt
    NVIC_EnableIRQ(SPI1_IRQn);

    // Delay to avoid code running while loading
    TMR_Delay(MXC_TMR0, MSEC(100));

    printf("\n***** New SPI loopback test *****\n");

    // Enable PINS for this SPI
    if(GPIO_Config(TEST_SPI_GPIO) != E_NO_ERROR) {
        printf("Error configuring pins\n");
        while(1) {}
    }
   for (i=2;i<17;i++){

    TEST_SPI->ctrl1 = (MXC_F_SPI_CTRL1_SPIEN);
    
    // Initialize the tx_data
    for(j = 0; j < TEST_LEN; j++) {
        tx_data[j] = TEST_VALUE;
    }
    // Configure the peripheral
    if(SPI_Init(TEST_SPI, 0, 100000) != 0) {
        printf("Error configuring SPI\n");
        while(1) {}
    }

    memset(rx_data, 0x0, TEST_LEN);
    req.write = tx_data;
    req.read = rx_data;
    req.len = TEST_LEN;
    req.bits = i;
    req.width = SPI_WIDTH_1;
    req.slave = 0;
    req.deass = 1;
    req.tx_num = 0;
    req.rx_num = 0;
    req.callback = spi_cb;
    spi_flag =1;

    #if ASYNC
        SPI_MasterTransAsync(TEST_SPI, &req);
        while(spi_flag == 1);
    #else
        SPI_MasterTrans(TEST_SPI, &req);
    #endif

    for(j=0; j<TEST_LEN; j++){
        if(req.bits <=8 ){
            if(j<(TEST_LEN/2)){

                temp =  TEST_VALUE >>(16-req.bits);
                temp = (temp<<8)|temp;
                tx_data[j] = temp;
            }
            else
            {
                tx_data[j] = 0x0000;
            }
        }
        else{

            tx_data[j] = TEST_VALUE >>(16-req.bits);
        }
    }

    if(memcmp(rx_data, tx_data, sizeof(tx_data)) != 0) {
        printf("Error verifying rx_data %d\n",i);
        // asm volatile ("mov R7, 0x55"); // Fail
    } else {
        printf("pass %d\n",i);
        // asm volatile ("mov R7, 0xAA"); // Pass
    }

    TEST_SPI->dma |= MXC_F_SPI_DMA_TX_FIFO_CLEAR | MXC_F_SPI_DMA_RX_FIFO_CLEAR;

    TEST_SPI->ctrl1 = ~(MXC_F_SPI_CTRL1_SPIEN);


}
   
    printf("Done testing\n");        
    while(1) {}
}
