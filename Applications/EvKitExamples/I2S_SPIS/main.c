/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated document  ation files (the "Software"),
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
 * $Date: 2018-09-04 17:13:32 -0500 (Tue, 04 Sep 2018) $
 * $Revision: 37649 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   I2S over SPIS
 * @details This examples uses the SPIS to emulate an I2S peripheral. The codec must bet setup
            in TDM/PCM mode so the LRCLK operates as a frame marker, or slave select in
            the SPI sense. The codec must also operate in master mode to drive LRCLK(SS) and 
            BCLK(SCK).
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "max9867.h"
#include "i2c.h"
#include "spi.h"
#include "spi17y_regs.h"
#include "nvic_table.h"
#include "led.h"

/***** Definitions *****/
#define I2C_CODEC       MXC_I2C0_BUS0
#define CODEC_MCLK_FREQ 12288000

#define I2S_SPI         SPI2
#define I2S_SPI_REG     MXC_SPI2
#define I2C_SPI_IRQn    SPI2_IRQn
#define SPI_MODE        0
#define SPI_FREQ        5000000

#define DMA_CH          0

/*  Define this if we have DIN/DOUT swapped on the EvKit.
    Without the board modification, we will only be able to do half-duplex communication.
      SPIS 3-wire configuration to transmit.
      SPIS in dual mode to receive. Will have to post process and  layer the bytes.
*/
// #define SPIS_PCB_MOD

/***** Globals *****/
spi17y_req_t spisReq;

// Test data
int32_t tone[] = {
    0x00000000, 0x0C8B0000, 0x18F80000, 0x25270000, 0x30FB0000, 0x3C560000, 0x471C0000, 0x51330000,
    0x5A810000, 0x62F10000, 0x6A6C0000, 0x70E10000, 0x76400000, 0x7A7C0000, 0x7D890000, 0x7F610000,
    0x7FFF0000, 0x7F610000, 0x7D890000, 0x7A7C0000, 0x76400000, 0x70E10000, 0x6A6C0000, 0x62F10000,
    0x5A810000, 0x51330000, 0x471C0000, 0x3C560000, 0x30FB0000, 0x25270000, 0x18F80000, 0x0C8B0000,
    0x00000000, 0xF3750000, 0xE7080000, 0xDAD90000, 0xCF050000, 0xC3AA0000, 0xB8E40000, 0xAECD0000,
    0xA57F0000, 0x9D0F0000, 0x95940000, 0x8F1F0000, 0x89C00000, 0x85840000, 0x82770000, 0x809F0000,
    0x80010000, 0x809F0000, 0x82770000, 0x85840000, 0x89C00000, 0x8F1F0000, 0x95940000, 0x9D0F0000,
    0xA57F0000, 0xAECD0000, 0xB8E40000, 0xC3AA0000, 0xCF050000, 0xDAD90000, 0xE7080000, 0xF3750000,
    0x00000000, 0x0C8B0000, 0x18F80000, 0x25270000, 0x30FB0000, 0x3C560000, 0x471C0000, 0x51330000,
    0x5A810000, 0x62F10000, 0x6A6C0000, 0x70E10000, 0x76400000, 0x7A7C0000, 0x7D890000, 0x7F610000,
    0x7FFF0000, 0x7F610000, 0x7D890000, 0x7A7C0000, 0x76400000, 0x70E10000, 0x6A6C0000, 0x62F10000,
    0x5A810000, 0x51330000, 0x471C0000, 0x3C560000, 0x30FB0000, 0x25270000, 0x18F80000, 0x0C8B0000,
    0x00000000, 0xF3750000, 0xE7080000, 0xDAD90000, 0xCF050000, 0xC3AA0000, 0xB8E40000, 0xAECD0000,
    0xA57F0000, 0x9D0F0000, 0x95940000, 0x8F1F0000, 0x89C00000, 0x85840000, 0x82770000, 0x809F0000,
    0x80010000, 0x809F0000, 0x82770000, 0x85840000, 0x89C00000, 0x8F1F0000, 0x95940000, 0x9D0F0000,
    0xA57F0000, 0xAECD0000, 0xB8E40000, 0xC3AA0000, 0xCF050000, 0xDAD90000, 0xE7080000, 0xF3750000,
};

/***** Functions *****/

// *****************************************************************************
void SPI_IRQHandler(void)
{
    /* Save and clear the interrupts */
    uint32_t flags = I2S_SPI_REG->int_fl;
    I2S_SPI_REG->int_fl = flags;

    /* Check for errors */
    if(flags & MXC_F_SPI17Y_INT_FL_TX_OVR) {
        printf("SPI TX Overrun error\n");
    }
    if(flags & MXC_F_SPI17Y_INT_FL_TX_UND) {
        printf("SPI TX Underrun error\n");
    }
}

// *****************************************************************************
void DMA0_IRQHandler(void)
{
    LED_On(0);
    DMA_Handler(DMA_CH);
    LED_Off(0);
}

// *****************************************************************************
void DMA_Callback(int ch, int error)
{
    unsigned int dma_flag;

    /* Clear the interrupt flags */
    DMA_GetFlags(DMA_CH, &dma_flag);
    DMA_ClearFlags(DMA_CH);

    if(ch != DMA_CH) {
        printf("Error, callback with wrong DMA channel\n");
        while(1) {};
    }

    if(error != E_NO_ERROR) {
        printf("Error in DMA callback error code %d\n", error);
        while(1) {}
    }

    /* Restart the DMA channel */
    DMA_Start(DMA_CH);
}

// *****************************************************************************
int main(void)
{
    int retval;
    printf("\n***** I2S Emulation w/ SPIS *****\n");

    /* Setup the I2C for the codec register interface */
    I2C_Shutdown(I2C_CODEC);
    if (I2C_Init(I2C_CODEC, I2C_FAST_MODE, NULL) != E_NO_ERROR) {
        printf("Failed to setup the I2C interface\n");
        while(1) {}
    }

    /* Setup the codec */
    if (codecInit(I2C_CODEC, CODEC_MCLK_FREQ) < 0) {
        printf("Failed to setup the codec\n");
        while(1) {}
    }

    /* Set the volume to something low */
    codecSetVolume(0x20, 0x20);


#ifdef SPIS_PCB_MOD
    /* TODO: Setup the SPIS in 4-wire slave mode */

#else
    /* Setup the SPIS in 3-wire slave mode, TX to codec only. */
    sys_cfg_spi17y_t spi_cfg;
    spi_cfg.map = MAP_A;
    spi_cfg.ss0 = Enable;
    spi_cfg.ss1 = Disable;
    spi_cfg.ss2 = Disable;
    spi_cfg.num_io = 1;
    SPI_Init(I2S_SPI, SPI_MODE, SPI_FREQ, spi_cfg);

    /* Enable 3-wire mode so the SPIS transmits on the MOSI pin */
    I2S_SPI_REG->ctrl2 |= MXC_F_SPI17Y_CTRL2_THREE_WIRE;
#endif

    /* Enable SPI DMA */
    I2S_SPI_REG->dma |= (MXC_F_SPI17Y_DMA_TX_DMA_EN | 
        MXC_F_SPI17Y_DMA_TX_FIFO_EN);

    /* Enable SPIS interrupts */
    NVIC_EnableIRQ(I2C_SPI_IRQn);
    NVIC_SetVector(I2C_SPI_IRQn, SPI_IRQHandler);
    I2S_SPI_REG->int_en |= (MXC_F_SPI17Y_INT_EN_TX_OVR | MXC_F_SPI17Y_INT_EN_TX_UND);

    /* Enable SPI */
    SPI_Enable(I2S_SPI);

    /* enable DMA interrupt */
    NVIC_EnableIRQ(DMA0_IRQn);

    /* Setup the DMA channel to feed the TX buffer */
    if((retval = DMA_Init()) != E_NO_ERROR) {
        printf("Failed to initialize DMA\n");
        while(1) {}
    }

    /* Acquire a DMA channel */
    if((retval = DMA_AcquireChannel()) == E_BAD_STATE) {
        printf("Error acquiring DMA channel\n");
        while(1) {}
    } 

    if(retval != DMA_CH) {
        /* Hardcoded to only work with DMA CH 0 */
        printf("Acqquired DMA channel %d when we expect %d\n", retval, DMA_CH);
        while(1) {}
    }

    /* Configure the DMA channel */
    retval = DMA_ConfigChannel(
        DMA_CH,                              //ch
        DMA_PRIO_HIGH,                      //prio
        DMA_REQSEL_SPI2TX,                  //reqsel
        0,                                  //reqwait_en
        DMA_TIMEOUT_4_CLK,                  //tosel
        DMA_PRESCALE_DISABLE,               //pssel
        DMA_WIDTH_WORD,                     //srcwd
        1,                                  //srcinc_en
        DMA_WIDTH_WORD,                     //dstwd
        0,                                  //dstinc_en
        4,                                  //burst_size (bytes-1)
        1,                                  //chdis_inten
        0                                   //ctz_inten
        );

    if(retval != E_NO_ERROR) {
        printf("Error configuring the DMA channel\n");
        while(1) {}
    }

    retval = DMA_SetSrcDstCnt(DMA_CH, tone, NULL, sizeof(tone));
    if(retval != E_NO_ERROR) {
        printf("Error setting the DMA src, dst, and count\n");
        while(1) {}
    }

    /* Setup the DMA to reload at the start of the tone */
    retval = DMA_SetReload(DMA_CH, tone, NULL, sizeof(tone));
    if(retval != E_NO_ERROR) {
        printf("Error setting the DMA src, dst, and count\n");
        while(1) {}
    }

    if((retval = DMA_SetCallback(DMA_CH, DMA_Callback)) != E_NO_ERROR) {
        printf("Error setting callback\n");
        while(1) {}
    }

    if((retval = DMA_EnableInterrupt(DMA_CH)) != E_NO_ERROR) {
        printf("Error enabling interrupt\n");
        while(1) {}
    }

    /* Start the DMA to start filling the SPI TX FIFO */
    if((retval = DMA_Start(DMA_CH)) != E_NO_ERROR) {
        printf("Error starting DMA Channel\n");
        while(1) {}
    }

    /* Set the bit clock divider */
    if((retval = codecUpdate(MAX9867_REG_09_IFC1B, 0x7, MAX9867_BSEL_48X)) != 2) {
        printf("Error setting BSEL %d\n", retval);
        while(1) {}
    }

    /* Put the codec into master mode to start the data transfer */
    if((retval = codecUpdate(MAX9867_REG_08_IFC1A, MAX9867_MAS, MAX9867_MAS)) != 2) {
        printf("Error setting the master mode bit %d\n", retval);
        while(1) {}
    }

    printf("Setup complete\n");
    while(1) {}
}
