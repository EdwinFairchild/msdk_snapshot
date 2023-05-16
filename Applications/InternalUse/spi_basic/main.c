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

/***** Definitions *****/
#define TEST_SPI        MXC_SPI3
#define TEST_GPIO       &gpio_cfg_spi3
#define TEST_IRQN       SPI3_IRQn
#define TEST_LEN        4


/***** Globals *****/
volatile uint32_t irq_flags;

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
void SPI1_IRQHandler(void)
{
    uint32_t temp_flags;

    temp_flags = MXC_SPI1->irq;
    MXC_SPI1->irq = temp_flags;

    irq_flags |= temp_flags;
}

uint8_t txdata[TEST_LEN];
uint8_t rxdata[TEST_LEN];

// *****************************************************************************
int main(void)
{


    int i;
    for(i = 0; i < TEST_LEN; i++) {
        txdata[i] = i;
    }

    // // Delay to avoid code running while loading
    TMR_Delay(MXC_TMR0, MSEC(100));

    printf("\n***** New SPI basic test *****\n");

    // Enable PINS for this SPI
    if(GPIO_Config(TEST_GPIO) != E_NO_ERROR) {
        printf("Error configuring pins\n");
        while(1) {}
    }

    // Setup SPI for transaction
    TEST_SPI->ctrl1 |= (MXC_F_SPI_CTRL1_SPIEN | MXC_F_SPI_CTRL1_MMEN | MXC_S_SPI_CTRL1_SS_SS0);

    TEST_SPI->ctrl2 |= (TEST_LEN << MXC_F_SPI_CTRL2_TX_NUM_CHAR_POS);
    TEST_SPI->ctrl2 |= (TEST_LEN << MXC_F_SPI_CTRL2_RX_NUM_CHAR_POS);

    TEST_SPI->ctrl3 |= (0x8 << MXC_F_SPI_CTRL3_NUMBITS_POS);
    TEST_SPI->ctrl3 |= MXC_S_SPI_CTRL3_DATA_WIDTH_MONO;

    TEST_SPI->ctrl4 |= ((0x1 << MXC_F_SPI_CTRL4_SSACT1_POS) |
        (0x1 << MXC_F_SPI_CTRL4_SSACT2_POS) |
        (0x1 << MXC_F_SPI_CTRL4_SSINACT_POS));

    TEST_SPI->brg_ctrl |= ((0x1 << MXC_F_SPI_BRG_CTRL_LOW_POS) |
        (0x1 << MXC_F_SPI_BRG_CTRL_HI_POS));

    TEST_SPI->brg_ctrl |= (0x1 << MXC_F_SPI_BRG_CTRL_SCALE_POS);

    TEST_SPI->dma |= (MXC_F_SPI_DMA_TX_FIFO_EN | MXC_F_SPI_DMA_RX_FIFO_EN);

    // Clear the interrupts
    TEST_SPI->irq = TEST_SPI->irq;

    // Enable the interrupts
    NVIC_EnableIRQ(TEST_IRQN);
    TEST_SPI->irqe = 0xFFFFFFFF;

    for(i = 0; i < TEST_LEN; i++) {
        TEST_SPI->data8[0] = txdata[i];
    }

    // Start the transaction    
    TEST_SPI->ctrl1 |= MXC_F_SPI_CTRL1_TX_START;

    // Wait for the done flags
    while(!(irq_flags & MXC_F_SPI_IRQ_M_DONE)) {}

    // Read back the data
    for(i = 0; i < TEST_LEN; i++) {
        while(!(TEST_SPI->dma & MXC_F_SPI_DMA_RX_FIFO_CNT)) {}
        rxdata[i] = TEST_SPI->data8[0];
    }

    if(memcmp(txdata, rxdata, TEST_LEN) != 0) {
        printf("Error verifying RX Data\n");
    }

    printf("irq_flags = 0x%x\n", irq_flags);
    printf("Done testing\n");

    while(1) {}
}
