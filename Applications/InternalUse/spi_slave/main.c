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
#define TEST_SPI        MXC_SPI3
#define TEST_SPI_GPIO   &gpio_cfg_spi3

#define TEST_LEN        32
#define TEST_WIDTH      4

/***** Globals *****/
uint8_t rx_data[TEST_LEN];
uint8_t tx_data[TEST_LEN];

uint32_t irq_flags;
/***** Functions *****/

// *****************************************************************************
void SPI3_IRQHandler(void)
{
    uint32_t temp_flags;

    temp_flags = TEST_SPI->irq;
    TEST_SPI->irq = temp_flags;

    irq_flags |= temp_flags;
}

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

    printf("\n***** New SPI slave test *****\n");

    // Enable PINS for this SPI
    if(GPIO_Config(TEST_SPI_GPIO) != E_NO_ERROR) {
        printf("Error configuring pins\n");
        while(1) {}
    }

    // Configure the peripheral
    if(SPI_Config(TEST_SPI, 0, 1000000) != 0) {
        printf("Error configuring SPI\n");
        while(1) {}
    }

    NVIC_EnableIRQ(SPI3_IRQn);
    TEST_SPI->irqe = 0xFFFFFFFF;

    memset(rx_data, 0x0, TEST_LEN);
    if(SPI_SlaveTrans(TEST_SPI, NULL, rx_data, TEST_LEN, 8, TEST_WIDTH) != 0) {
        printf("Error with SPI_SlaveTrans\n");
    }

    if(memcmp(rx_data, tx_data, TEST_LEN) != 0) {
        printf("Error verifying rx_data\n");
    }

    printf("irq_flags = 0x%x\n", irq_flags);
    printf("Done testing\n");        


    while(1) {}
}
