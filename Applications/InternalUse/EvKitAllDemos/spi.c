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
 * @brief   SPIS example using SPI1 & SPI2
 * @details Uses the on board master and slave to show the SPIS. It sends data from 
 *          master to slave and salve to master. This example is showing full duplex 
 *          duplex communication which can be changed to half duplex by changing the 
 *          spi_req_t write & read pointer to buffer to NULL.  
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
#define TEST_MASTER        MXC_SPI1
#define TEST_MASTER_GPIO   &gpio_cfg_spi1

#define TEST_SLAVE         MXC_SPI2
#define TEST_SLAVE_GPIO    &gpio_cfg_spi2

#define TEST_LEN           60
#define TEST_LEN_BYTE      120

#define CLOCK_RATE         4000000
/***** Globals *****/
uint16_t master_rx_data[TEST_LEN];
uint16_t master_tx_data[TEST_LEN];
uint16_t slave_rx_data[TEST_LEN];
uint16_t slave_tx_data[TEST_LEN];
volatile int spis_flag;
volatile int spim_flag;

/***** Functions *****/
/******************************************************************************/

void spis_cb(spi_req_t* req, int error) 
{
    spis_flag = error;
}
//******************************************************************************

void spim_cb(spi_req_t* req, int error) 
{
    spim_flag = error;
}
//******************************************************************************

void SPI1_IRQHandler(void)
{      
    SPI_Handler(TEST_MASTER);
}
//******************************************************************************

void SPI2_IRQHandler(void)
{   
    SPI_Handler(TEST_SLAVE);
}
// *****************************************************************************

void spi_ctrl_bit_enable(mxc_spi_regs_t* regs) 
{
    regs->ctrl1 = (MXC_F_SPI_CTRL1_SPIEN);
}
// *****************************************************************************

void spi_ctrl_bit_disable(mxc_spi_regs_t* regs) 
{
    regs->ctrl1 = ~(MXC_F_SPI_CTRL1_SPIEN);
}
// *****************************************************************************

void clear_fifo(mxc_spi_regs_t* regs) 
{
    regs->dma |= MXC_F_SPI_DMA_TX_FIFO_CLEAR | MXC_F_SPI_DMA_RX_FIFO_CLEAR;
}
// *****************************************************************************

int spi_main(void)
{
    spi_req_t spi_master_req; // Initialize SPI Request struct for Master
    spi_req_t spi_slave_req;  // Initialize SPI Request struct for Slave
    
    // // Setup the interrupt
    NVIC_EnableIRQ(SPI1_IRQn);
    NVIC_EnableIRQ(SPI2_IRQn);

    // Delay to avoid code running while loading
    // TMR_Delay(MXC_TMR0, MSEC(100));

    printf("\n***** SPI-Slave Example *****\n\n");

    // Both Master & Slave has ctrl bit has to be enabled
    spi_ctrl_bit_enable(TEST_MASTER);
    spi_ctrl_bit_enable(TEST_SLAVE);

    // Initialize the tx_data
    for(int j = 0; j < TEST_LEN; j++) { 
        master_tx_data[j] = j;
        slave_tx_data[j] = TEST_LEN-j;
    }

    // Configure the peripheral
    if(SPI_Init(TEST_MASTER, 0, CLOCK_RATE) != E_NO_ERROR) { 
        // printf("Error configuring SPI\n");
        while(1) {} 
    }

    if(SPI_Init(TEST_SLAVE, 0, CLOCK_RATE) != E_NO_ERROR) { 
        // printf("Error configuring SPI\n");
        while(1) {}
    }

    // Initialize RX buffer to store data
    memset(master_rx_data, 0x0, TEST_LEN_BYTE);
    memset(slave_rx_data,  0x0, TEST_LEN_BYTE);

    // Initialize spi_reqest struct for Master 
    spi_master_req.slave = 0;           
    spi_master_req.deass = 1;           
    spi_master_req.write = master_tx_data; 
    spi_master_req.read = master_rx_data;    
    spi_master_req.len = TEST_LEN;      
    spi_master_req.width = SPI_WIDTH_1; 
    spi_master_req.bits = 16;        
    spi_master_req.rx_num = 0;          
    spi_master_req.tx_num = 0;          
    spi_master_req.callback = spim_cb;    

    // Initialize spi_request struct for slave
    spi_slave_req.write = slave_tx_data;
    spi_slave_req.read = slave_rx_data;
    spi_slave_req.len = TEST_LEN;
    spi_slave_req.width = SPI_WIDTH_1;
    spi_slave_req.bits = 16; 
    spi_slave_req.tx_num = 0;
    spi_slave_req.rx_num = 0;
    spi_slave_req.callback = spis_cb;

    spim_flag =1;
    spis_flag =1;

    // Initiate the Slave Async first before Master Snync to make sure
    // data can be received and no data lost when master initiates com
    // -munication

    // fix: add also timeout, use timer utils function & also return value of spi_flag. check whether error / timeout happened
    // If this is done, it will add delay between slave trans asnc & master trans initialization which will cause problem when 
    // they communicate. Even with just if statement
    SPI_SlaveTransAsync(TEST_SLAVE, &spi_slave_req);
    SPI_MasterTrans(TEST_MASTER, &spi_master_req);
    while(spis_flag == 1); 

    // Print the data
    for(int j = 0; j < TEST_LEN; j++) { 
        printf("master data %u  | slave data %u \n", master_tx_data[j], slave_rx_data[j]);
    }

    // // Check the data sent from Master to Slave 
    if (memcmp(slave_rx_data, master_tx_data, TEST_LEN_BYTE) == E_NO_ERROR) {
        printf("Data is verified.\n\n");
    } else {
        // while(1){}
        printf("Data mismatch between slave_rx and master_tx\n\n");
    }
    
    // Print the data
    for(int j = 0; j < TEST_LEN; j++) { 
        printf("master data %u  | slave data %u \n", slave_tx_data[j], master_rx_data[j]);
    }

    // Check the data sent from Slave to Master
    if (memcmp(master_rx_data, slave_tx_data, TEST_LEN_BYTE) == E_NO_ERROR) {
        printf("Data is verified.\n\n");
    } else {
        // return 0;
        printf("Data mismatch between master_rx and slave_tx_data.\n\n");
    }

    // Clear the FIFO inside SPI for further communication
    clear_fifo(TEST_MASTER);
    clear_fifo(TEST_SLAVE);


    spi_ctrl_bit_disable(TEST_SLAVE);
    spi_ctrl_bit_disable(TEST_MASTER);

    // Shutdown SPI Peripheral
    SPI_Shutdown(TEST_SLAVE);
    SPI_Shutdown(TEST_MASTER);

    return 0;

}