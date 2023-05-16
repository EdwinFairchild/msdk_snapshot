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
 * @brief   Test SPID API
 * @details Initialize, enable, transact, disable, then shut down the SPID periph.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mxc_config.h"
#include "spid.h"

/***** Definitions *****/
// RAM Vendor specific commands
#define A1024_READ  0x03
#define A1024_WRITE 0x02
// #define A1024_EDIO  0x3B

// RAM Vendor specific values
#define DEVICE_SIZE 0x20 /*32*/
#define FIFO_SIZE 8

/***** Globals *****/
spid_cfg_t init_cfg = {
    MXC_V_SPID_CTRL1_SS_SS0, // ssel

    0x08, // numbits
    SPID_MONO, // data_width

    0x04, //ssel_act_1, number of system clocks between SS active and first serial clock edge
    0x08, //ssel_act_2, number of system clocks between last serial clock edge and SS inactive
    0x10, //ssel_inact, number of system clocks between transactions

    0x1, // baud_freq
    0x1 // baud_scale
};

int pass;
int fail;

/***** Functions *****/
// *****************************************************************************
void Pass(void) {pass++;}

// *****************************************************************************
void Fail(void) {fail++;}

// *****************************************************************************
void setup(void)
{
    /* First Set SPID to Mono SPI mode to talk to RAM*/
    SPID_Enable();
    SPID_Config(&init_cfg);

    /*Send command to RAM to configure into dual mode*/
    uint8_t dual_cmd[2] = {0x3,0xB}; /*Defined by the RAM A1024*/
    SPID_Send_Command(dual_cmd, 4, 1);

    /*Set the SPID module to dual mode*/
    MXC_SPID->ctrl3 |= MXC_S_SPID_CTRL3_DATA_WIDTH_DUAL;
}

// *****************************************************************************
void spid_test(void)
{
    int i,r; /*Indices*/
    uint8_t tmp, random;

    /*Generate and write pseudorandom data to RAM*/
    for (i = 0; i < DEVICE_SIZE; i += FIFO_SIZE) {
        MXC_SPID->dma = (MXC_F_SPID_DMA_TX_FIFO_CLEAR | MXC_F_SPID_DMA_RX_FIFO_CLEAR | MXC_F_SPID_DMA_TX_FIFO_EN);

        uint8_t write_cmd[4] = {A1024_WRITE, (i>>16), (i>>8), i};
        SPID_Send_Command(write_cmd, 4, (4 + FIFO_SIZE));

        // Create and send randomish payload
        srand(i);
        for (r = 0; r < FIFO_SIZE; r++) {
            MXC_SPID->data8[0] = rand();
        }

        while (SPID_Busy());
    }

    /* Read and compare pseudorandom data from RAM*/
    for (i = 0; i < DEVICE_SIZE; i += FIFO_SIZE) {
        /*Send read command*/
        MXC_SPID->dma = (MXC_F_SPID_DMA_TX_FIFO_CLEAR | MXC_F_SPID_DMA_RX_FIFO_CLEAR | MXC_F_SPID_DMA_TX_FIFO_EN);
        MXC_SPID->ctrl1 |= MXC_S_SPID_CTRL1_SS_CTRL_ASSERT;
        MXC_SPID->ctrl2 = (FIFO_SIZE << MXC_F_SPID_CTRL2_RX_NUM_CHAR_POS);

        uint8_t read_cmd[5] = {A1024_READ, (i>>16), (i>>8), i, 0x00};
        SPID_Send_Command(read_cmd, 5, 5);
        while (SPID_Busy());


        /*Fill RXFIFO*/
        MXC_SPID->dma = MXC_S_SPID_DMA_RX_FIFO_EN_EN | MXC_S_SPID_DMA_RX_DMA_EN_EN;
        MXC_SPID->ctrl1 |= MXC_F_SPID_CTRL1_TX_START;

        while (((MXC_SPID->dma & MXC_F_SPID_DMA_RX_FIFO_CNT) >> MXC_F_SPID_DMA_RX_FIFO_CNT_POS) < (FIFO_SIZE >> 1));
        MXC_SPID->ctrl1 &= ~MXC_S_SPID_CTRL1_SS_CTRL_ASSERT;
        while (((MXC_SPID->dma & MXC_F_SPID_DMA_RX_FIFO_CNT) >> MXC_F_SPID_DMA_RX_FIFO_CNT_POS) != FIFO_SIZE);

        /*Regenerate pseudorandom sequence and compare against values read from RAM*/
        srand(i);
        for (r = 0; r < FIFO_SIZE; r++) {
            random = rand();
            tmp = MXC_SPID->data8[0];

            if (tmp != random) {
                printf("%08X fail %02X != %02X\n", i, tmp, random);
                Fail();
            } else {
                printf("%02X == %02X\n", tmp, random);
                Pass();
            }
        }

        while (SPID_Busy()); // Wait for transaction to complete
    }
}

// *****************************************************************************
int main(void)
{
    printf("********** QSPI_XMEM test **********\n");
    /*Globals*/
    pass=0;
    fail=0;

    setup();
    spid_test();

    SPID_Disable();

    while(1);
    return 0;
}
