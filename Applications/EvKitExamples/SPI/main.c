/**
 * @file    main.c
 * @brief   SPI Master Demo
 * @details Shows Master loopback demo for QSPI0
 *          Read the printf() for instructions
 */

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
 * $Date: 2021-09-07 14:40:37 -0500 (Tue, 07 Sep 2021) $
 * $Revision: 59600 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pb.h"
#include "mxc_config.h"
#include "led.h"
#include "mxc_pins.h"
#include "uart.h"
#include "pb.h"
#include "spi.h"
#include "hexdump.h"

/***** Definitions *****/
#define TEST_LEN        0x40      // Words
#define TEST_VALUE      0xA5A5  // This is for master mode only...
#define VALUE           0xFFFF
#define SPI_SPEED       100000  // Bit Rate


#define SPI SPI1
#define SPI_IRQ SPI1_IRQn

// SELECT FROM BELOW
#define ASYNC         // Comment out line for Async operation

/***** Globals *****/
uint16_t rx_data[TEST_LEN];
uint16_t tx_data[TEST_LEN];
volatile int spi_flag;
volatile int buttonPressed;

/***** Functions *****/

void spi_cb(void *req, int error)
{
    spi_flag = error;
}

void SPI1_IRQHandler(void)
{
    SPI_Handler(SPI);
}

void buttonHandler()
{
    buttonPressed = 1;
}

int main(void)
{
    int byteWidth;
    int byteIndex;
    uint16_t tx_word;
    uint8_t tx_byte;
    sys_cfg_spi_t spi17y_master_cfg;
    spi_req_t req;
    
    spi17y_master_cfg.map = MAP_A;
    spi17y_master_cfg.ss0 = Disable;
    spi17y_master_cfg.ss1 = Disable;
    spi17y_master_cfg.ss2 = Disable;
    spi17y_master_cfg.num_io = 2;
#ifdef ASYNC
    // Setup the interrupt
    NVIC_EnableIRQ(SPI_IRQ);
#endif
    
    printf("\n************** SPI Loopback Demo ****************\n");
    printf("This example configures the SPI to send data between the MISO (P0.18, aka AIN2)\n");
    printf("and MOSI (P0.17 aka AIN1) pins.  Connect these two pins together.\n");
    printf("This demo shows SPI sending different bit sizes each run through. \n");
    printf("Push SW2 to continue\n");
    
    buttonPressed = 0;
    PB_RegisterCallback(0, (pb_callback)buttonHandler);
    while(!buttonPressed);
    
    for (byteWidth=1; byteWidth<17; byteWidth++) {
        if (byteWidth == 1 || byteWidth ==9) { // Sending out 2 to 16 bits.
            continue;
        }

        // Initialize TX Data buffer.
        memset(tx_data, 0x0, sizeof(tx_data));
        if (byteWidth <= 8) {
            for (byteIndex = 0; byteIndex < (TEST_LEN << 1); byteIndex++) {
                tx_byte = (((1U << byteWidth) - 1) & TEST_VALUE);
                ((uint8_t*)tx_data)[byteIndex] = tx_byte;
            }
        } else {
            for (byteIndex = 0; byteIndex < TEST_LEN; byteIndex++) {
                tx_word = (((1U << byteWidth) - 1) & TEST_VALUE);
                tx_data[byteIndex] = tx_word;
            }
        }

        // Clear RX Data buffer.
        memset(rx_data, 0x0, sizeof(rx_data));

        // Configure the peripheral
        if (SPI_Init(SPI, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
        
            Console_Init();
            printf("Error configuring SPI\n");
            while (1) {}
        }
        
        // Issue SPI transfer request.
        req.tx_data = tx_data;
        req.rx_data = rx_data;
        req.len = (sizeof(tx_data) >> ((byteWidth > 8) ? 1 : 0));
        req.bits = byteWidth;
        req.width = SPI17Y_WIDTH_1;
        req.ssel = 0;
        req.deass = 1;
        req.ssel_pol = SPI17Y_POL_LOW;
        req.tx_num = 0;
        req.rx_num = 0;
        req.callback = spi_cb;
        spi_flag = 1;
#ifdef ASYNC
        SPI_MasterTransAsync(SPI, &req);
        while (spi_flag == 1);
#else
        SPI_MasterTrans(SPI, &req);
#endif
        
        // Compare Sent data vs Received data
        // Printf needs the Uart turned on since they share the same pins
        if (memcmp(rx_data, tx_data, sizeof(tx_data)) != 0) {
            printf("\nError verifying rx_data %d\n",byteWidth);
            printf("\n");
            printf("RX Data:\n");
            hexdump((const char *)rx_data, sizeof(rx_data));
            printf("TX Data:\n");
            hexdump((const char *)tx_data, sizeof(tx_data));
        } else {
            printf("\nSent %d bits per transaction\n",byteWidth);
        }

        SPI_Shutdown(SPI);
    }
    
    printf("\n Done testing \n");
    
    while(1);
}
