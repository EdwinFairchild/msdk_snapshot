/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
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
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "spi.h"
#include "dma.h"

#define SPIRAM_READ_CMD     0x03
#define SPIRAM_WRITE_CMD    0x02
#define SPIRAM_EQIO_CMD     0x38
#define SPIRAM_RSTQIO_CMD   0xFF

#define SPIRAM_ADDR         0

#define SPI_BAUD            400000

#define TRANS_LENGTH        400

volatile int out_dma_complete = 1;
volatile int in_dma_complete = 1;

const gpio_cfg_t ssel_pin = {PORT_0, PIN_8, GPIO_FUNC_OUT, GPIO_PAD_NONE};

// DMA IRQ Handlers - there needs to be one handler for each channel used.  This example uses 2.
void DMA0_IRQHandler()
{
    DMA_Handler(0);
}

void DMA1_IRQHandler()
{
    DMA_Handler(1);
}

// A callback function that will be called when the RX DMA completes.
void in_callback(int ch, int reason)
{
    in_dma_complete = 1;
}

// A callback function that will be called when the TX DMA completes.
void out_callback(int ch, int reason)
{
    out_dma_complete = 1;
}

// Sends/Receives a SPI packet using DMA.
//  out   - the buffer containing data to send to the slave.  May be NULL if there is 
//          no data to send or this the send portion of a quad mode transaction.
//  in    - the buffer to receive data from the slave.  May be NULL if thre is no
//          data coming from the slave or if this is the receive phase of a quad mode transaction.
//  len   - the number of bytes to send/receive.
//  deass - Whether to deassert the SSEL line at the end of this transaction.
//  quad  - Whether to send this packet using SPI quad mode or traditional full-duplex SPI.
void send_packet(uint8_t* out, uint8_t* in, unsigned int len, int deass, int quad)
{
    int in_ch = -1;
    int out_ch = -1;

    // Assert SSEL
    GPIO_OutClr(&ssel_pin);

    // Initialize the CTRL0 register
    MXC_SPI0->ctrl0 = MXC_F_SPI17Y_CTRL0_MASTER |   // Enable master mode
                      MXC_F_SPI17Y_CTRL0_EN;        // Enable the peripheral

    // Initialize the CTRL1 register
    MXC_SPI0->ctrl1 = 0;
    if(out || quad) {
        // Set how many to characters to send, or when in quad mode how many characters to receive
        MXC_SPI0->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_TX_NUM_CHAR_POS); 
    }

    if(in) {
        // Set how many characters to receive
        MXC_SPI0->ctrl1 |= (len << MXC_F_SPI17Y_CTRL1_RX_NUM_CHAR_POS);
    }

    // Initialize the CTRL2 register
    MXC_SPI0->ctrl2 = (quad ? MXC_S_SPI17Y_CTRL2_DATA_WIDTH_QUAD : MXC_S_SPI17Y_CTRL2_DATA_WIDTH_MONO) |
                      (8 << MXC_F_SPI17Y_CTRL2_NUMBITS_POS);

    // Initialize the DMA register
    MXC_SPI0->dma = 0;
    if(out) {
        MXC_SPI0->dma |= MXC_F_SPI17Y_DMA_TX_DMA_EN |                   // Enable DMA for transmit
                         MXC_F_SPI17Y_DMA_TX_FIFO_EN |                  // Enable the TX FIFO
                         (31 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);    // Set the threshold of when to add more data to TX FIFO
    }
    if(in) {
        MXC_SPI0->dma |= MXC_F_SPI17Y_DMA_RX_DMA_EN |                   // Enable DMA for receive
                         MXC_F_SPI17Y_DMA_RX_FIFO_EN |                  // Enable the RX FIFO
                         (0 << MXC_F_SPI17Y_DMA_TX_FIFO_LEVEL_POS);     // Set the threshold of when to read data from RX FIFO
    }

    // Prepare DMA for unloading RX FIFO.
    if(in) {
        in_ch = DMA_AcquireChannel();
        DMA_ConfigChannel(in_ch, DMA_PRIO_LOW, DMA_REQSEL_SPI0RX, 0,
                          DMA_TIMEOUT_4_CLK, DMA_PRESCALE_DISABLE,
                          DMA_WIDTH_BYTE, 0, DMA_WIDTH_BYTE, 1, 1, 1, 0);
        DMA_SetCallback(in_ch, in_callback);
        DMA_EnableInterrupt(in_ch);
        DMA_SetSrcDstCnt(in_ch, 0, in, len);
        in_dma_complete = 0;
        DMA_Start(in_ch);
    }
    else {
        in_dma_complete = 1;    // Not using the RX DMA, mark it as completed.
    }

    // Prepare DMA to fill TX FIFO.
    if(out) {
        out_ch = DMA_AcquireChannel();
        DMA_ConfigChannel(out_ch, DMA_PRIO_LOW, DMA_REQSEL_SPI0TX, 0,
                          DMA_TIMEOUT_4_CLK, DMA_PRESCALE_DISABLE,
                          DMA_WIDTH_BYTE, 1, DMA_WIDTH_BYTE, 0, 1, 1, 0);
        DMA_SetCallback(out_ch, out_callback);
        DMA_EnableInterrupt(out_ch);
        DMA_SetSrcDstCnt(out_ch, out, 0, len);
        out_dma_complete = 0;
        DMA_Start(out_ch);
    }
    else {
        out_dma_complete = 1;    // Not using the TX DMA, mark it as completed.
    }

    // Start the SPI transaction
    MXC_SPI0->ctrl0 |= MXC_F_SPI17Y_CTRL0_START;

    // Wait here for all DMA transactions to complete.
    // This would normally be where this function would
    // return so the micro can go do other things.  The
    // callbacks will be triggered when the DMA completes
    // so the application knows its time to do more work.
    // For this simple example, we'll just block here.
    while(!in_dma_complete || !out_dma_complete);

    if(in) {
        DMA_ReleaseChannel(in_ch);
    }
    if(out) {
        DMA_ReleaseChannel(out_ch);
    }

    // DMA has completed, but that just means DMA has loaded all of its data to/from
    //  the FIFOs.  We now need to wait for the SPI transaction to fully complete.
    if(MXC_SPI0->ctrl0 & MXC_F_SPI17Y_CTRL0_EN) {
        while(MXC_SPI0->stat & MXC_F_SPI17Y_STAT_BUSY);
    }

    // If the callee wanted SSEL deasserted at the end of the transaction, do that now.
    if(deass) {
        GPIO_OutSet(&ssel_pin);
    }
}

int main()
{
    int i;
    uint8_t cmd_buffer[5];
    uint8_t out_buffer[TRANS_LENGTH];
    uint8_t in_buffer[TRANS_LENGTH];

    printf("\n************** SPI DMA Demo ********************\n");
    printf("This example configures SPI0 to communicate with\n");
    printf("the external SPI RAM on the evaluation kit. When\n");
    printf("running this example make sure the SPI RAM's\n");
    printf("communication pins are connected to the micro by\n");
    printf("properly placing the JP7 jumper.\n");
    printf("************************************************\n\n");

    // Initialize DMA peripheral
    DMA_Init();
    NVIC_EnableIRQ(DMA0_IRQn);
    NVIC_EnableIRQ(DMA1_IRQn);

    // Initialize SPI peripheral
    sys_cfg_spi_t cfg = {MAP_B, Disable, Disable, Disable, 4};
    SPI_Init(SPI0, 0, SPI_BAUD, cfg);
    MXC_SPI0->ctrl0 |= MXC_F_SPI17Y_CTRL0_MASTER; // Switch SPI to master mode, else the state of the SS pin could cause the SPI periph to appear 'BUSY';

    MXC_SPI0->ss_time = (4 << MXC_F_SPI17Y_SS_TIME_PRE_POS) |
                        (8 << MXC_F_SPI17Y_SS_TIME_POST_POS) |
                        (16 << MXC_F_SPI17Y_SS_TIME_INACT_POS);

    GPIO_OutSet(&ssel_pin);
    GPIO_Config(&ssel_pin);

    // Send 'Reset' command to external SPI RAM
    printf("Resetting the SPI communication mode...\n");
    cmd_buffer[0] = SPIRAM_RSTQIO_CMD;
    send_packet(cmd_buffer, NULL, 1, 1, 0);

    // Send 'Enable QUAD Mode' command to external SPI RAM
    printf("Enabling SPI quad mode...\n");
    cmd_buffer[0] = SPIRAM_EQIO_CMD;
    send_packet(cmd_buffer, NULL, 1, 1, 0);

    // Send 'Write' command to external SPI RAM
    printf("Writing test data to the SPI RAM...\n");
    cmd_buffer[0] = SPIRAM_WRITE_CMD;
    cmd_buffer[1] = SPIRAM_ADDR >> 16;
    cmd_buffer[2] = SPIRAM_ADDR >> 8;
    cmd_buffer[3] = SPIRAM_ADDR;
    send_packet(cmd_buffer, NULL, 4, 0, 1);

    // Send the data to the external SPI RAM
    for(i = 0; i < TRANS_LENGTH; i++) {
        out_buffer[i] = i;
    }
    send_packet(out_buffer, NULL, TRANS_LENGTH, 1, 1);

    // Send 'Read' command to external SPI RAM
    printf("Reading data from the SPI RAM...\n");
    cmd_buffer[0] = SPIRAM_READ_CMD;
    cmd_buffer[1] = SPIRAM_ADDR >> 16;
    cmd_buffer[2] = SPIRAM_ADDR >> 8;
    cmd_buffer[3] = SPIRAM_ADDR;
    cmd_buffer[4] = 0xFF;
    send_packet(cmd_buffer, NULL, 5, 0, 1);

    // Read data from the external SPI RAM
    memset(in_buffer, 0, TRANS_LENGTH);
    send_packet(NULL, in_buffer, TRANS_LENGTH, 1, 1);

    // Verify the data read matches what was expected
    printf("Verifying the expected data was received...\n");
    for(i = 0; i < TRANS_LENGTH; i++) {
        if(in_buffer[i] != out_buffer[i]) {
            printf("Data mismatch at offset %d: Exp = 0x%02X Got = 0x%02X\n", i, out_buffer[i], in_buffer[i]);
        }
    }

    printf("Example complete.\n");
}
