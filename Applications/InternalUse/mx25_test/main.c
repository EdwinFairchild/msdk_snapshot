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
 * $Date: 2018-08-27 11:13:26 -0500 (Mon, 27 Aug 2018) $
 * $Revision: 37347 $
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
#define TEST_SPI        MXC_SPI3
#define TEST_SPI_GPIO   &gpio_cfg_spi3
#define TEST_ADDR       0x1000
#define TEST_LEN        32

/***** Globals *****/
uint8_t rx_data[TEST_LEN];
uint8_t tx_data[TEST_LEN];

/***** Functions *****/

// *****************************************************************************
int main(void)
{
    int i;

    // Initialize the tx_data
    for(i = 0; i < TEST_LEN; i++) {
        tx_data[i] = i;
    }

    // Delay to avoid code running while loading
    TMR_Delay(MXC_TMR0, MSEC(100));

    printf("\n***** QSPI mx25 test *****\n");

    // Enable PINS for this SPI
    if(GPIO_Config(TEST_SPI_GPIO) != E_NO_ERROR) {
        printf("Error configuring pins\n");
        while(1) {}
    }

    // Configure the peripheral
    if(SPI_Config(TEST_SPI, 0, 1000000) != 0) {
        printf("Erro configuring SPI\n");
        while(1) {}
    }

    // Initialize the MX25
    MX25_Init(TEST_SPI, 0);
    MX25_Reset();

    // Get the ID of the MX25
    uint32_t id = MX25_ID();
    printf("id = 0x%x\n", id);

    // Erase the test sector
    printf("Erasing test sector\n");
    MX25_Erase(TEST_ADDR, MX25_Erase_4K);

    // Program the MX25
    printf("Programming in 4-wire mode\n");
    MX25_Program_Page(TEST_ADDR, tx_data, TEST_LEN, 1);

    // Read back the memory
    printf("Verifying in 4-wire mode\n");
    memset(rx_data, 0x0, TEST_LEN);
    MX25_Read(TEST_ADDR, rx_data, TEST_LEN, 1);

    if(memcmp(rx_data, tx_data, TEST_LEN) != 0) {
        printf("Error verifying data in 4-wire mode\n");
    }

    // Erase the test sector
    printf("Erasing test sector\n");
    MX25_Erase(TEST_ADDR, MX25_Erase_4K);

    printf("Enabling quad mode\n");
    MX25_Quad(1);

    // Program the MX25
    printf("Programming in quad mode\n");
    MX25_Program_Page(TEST_ADDR, tx_data, TEST_LEN, 4);

    // Read back the memory
    printf("Verifying in quad mode\n");
    memset(rx_data, 0x0, TEST_LEN);
    MX25_Read(TEST_ADDR, rx_data, TEST_LEN, 4);

    if(memcmp(rx_data, tx_data, TEST_LEN) != 0) {
        printf("Error verifying data in 4-wire mode\n");
    }

    printf("Done testing\n");        
    while(1) {}
}
