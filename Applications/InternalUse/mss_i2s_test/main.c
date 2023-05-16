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
 * @brief   MSS I2S Test
 * @details Generates some data to shove out of the I2S interface
 */

/* -- Notes --
 *
 * SPI2 I/O is used for testing on ME10
 * 
 * Port/Pin  SPI Mode  I2S Mode
 * P2.2      CLK       BCLK
 * P2.3      MISO      (n/a)
 * P2.4      MOSI      SD
 * P2.5      /SS       LRCLK
 *
 * SPI1 Master/Slave is used to send/receive I2S data
 *
 * Port/Pin  SPI Mode
 * P1.23     /SS
 * P1.26     CLK
 * P1.28     MISO
 * P1.29     MOSI
 *
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "led.h"
#include "dma_regs.h"
#include "spimss_regs.h"
#include "spis.h"

/***** Definitions *****/

#define AUDIO_RATE 44100  /* Valid values: 11025, 22050, 44100 */
#define DMA_BURST_SIZE 3  /* Set this to (# bytes - 1) burst value */
#define FIFO_TX_LEVEL  1  /* Set this to (# bytes - 1) FIFO DMA request */
#undef LEFT_JUSTIFY       /* Test left-justify (with SPI Mode 0) */
#undef INVERT_LRCLK     /* Invert the polarity of the LRCLK for test, since SPI slave can only receive /SS */

/***** Globals *****/
#define NUM_WORDS_I2S_DATA 512
#define NUM_BYTES_I2S_DATA (NUM_WORDS_I2S_DATA * 2)
unsigned short srcdata[NUM_WORDS_I2S_DATA];
unsigned short dstdata[NUM_WORDS_I2S_DATA];
unsigned char spi_data[NUM_WORDS_I2S_DATA];

/***** Functions *****/

void generate_data(unsigned short *ptr, unsigned int num)
{
  if (ptr && num) {
    while (num--) {
      *ptr++ = (unsigned short)num;
    }
  }
}

void init_dma(void)
{
  /* TX path */
  
  /* Clear flags */
  MXC_DMA->ch[0].st = MXC_F_DMA_DMA_ST_CTZ_ST | MXC_F_DMA_DMA_ST_RLD_ST |
    MXC_F_DMA_DMA_ST_BUS_ERR | MXC_F_DMA_DMA_ST_TO_ST;

  /* Configure transfer characteristics */
  MXC_DMA->ch[0].cfg =
    (2 << MXC_F_DMA_DMA_CFG_SRCWD_POS) |     /* 32-bit source */
    MXC_F_DMA_DMA_CFG_SRCINC |              /* Source auto-increment */
    (1 << MXC_F_DMA_DMA_CFG_DSTWD_POS) |    /* 16-bit destination */
    (DMA_BURST_SIZE << MXC_F_DMA_DMA_CFG_BRST_POS) |     /* 4-byte bursts */
    (0x30 << MXC_F_DMA_DMA_CFG_REQSEL_POS); /* DMA request source: SPI_MSS TX (0x30) */

  /* RX path */

  /* Clear flags */
  MXC_DMA->ch[1].st = MXC_F_DMA_DMA_ST_CTZ_ST | MXC_F_DMA_DMA_ST_RLD_ST |
    MXC_F_DMA_DMA_ST_BUS_ERR | MXC_F_DMA_DMA_ST_TO_ST;

  /* Configure transfer characteristics */
  MXC_DMA->ch[1].cfg =
    (1 << MXC_F_DMA_DMA_CFG_SRCWD_POS) |     /* 16-bit source */
    MXC_F_DMA_DMA_CFG_DSTINC |              /* Destination auto-increment */
    (1 << MXC_F_DMA_DMA_CFG_DSTWD_POS) |    /* 32-bit destination */
    (DMA_BURST_SIZE << MXC_F_DMA_DMA_CFG_BRST_POS) |     /* 4-byte bursts */
    (0x10 << MXC_F_DMA_DMA_CFG_REQSEL_POS); /* DMA request source: SPI_MSS RX (0x10) */  
}

int set_tx_dma_range(void *src, unsigned int cnt)
{
  if (src && cnt) {
    MXC_DMA->ch[0].src = (unsigned int)src;
    MXC_DMA->ch[0].dst = (unsigned int)&MXC_SPIMSS->data;
    MXC_DMA->ch[0].cnt = cnt & MXC_F_DMA_DMA_CNT_CNT;
  } else {
    return -1;
  }
  
  return 0;
}

int set_rx_dma_range(void *dst, unsigned int cnt)
{
  if (dst && cnt) {
    MXC_DMA->ch[1].src = (unsigned int)&MXC_SPIMSS->data;
    MXC_DMA->ch[1].dst = (unsigned int)dst;
    MXC_DMA->ch[1].cnt = cnt & MXC_F_DMA_DMA_CNT_CNT;
  } else {
    return -1;
  }
  
  return 0;
}

void start_dma(void)
{
  MXC_DMA->ch[0].cfg |= MXC_F_DMA_DMA_CFG_CHEN;
  MXC_DMA->ch[1].cfg |= MXC_F_DMA_DMA_CFG_CHEN;
}

void stop_dma(void)
{
  MXC_DMA->ch[0].cfg = 0;
  MXC_DMA->ch[1].cfg = 0;
  /* Wait for DMA to stop */
  while(MXC_DMA->ch[0].st & MXC_F_DMA_DMA_ST_CH_ST);
  while(MXC_DMA->ch[1].st & MXC_F_DMA_DMA_ST_CH_ST);
}

void setup_i2s(void)
{
  /* Disable master */
  MXC_SPIMSS->cntl = 0;

  /* Configure I2S */
  MXC_SPIMSS->i2s_cntl = MXC_F_SPIMSS_I2S_CNTL_I2S_EN; /* Enable I2S */
#ifdef LEFT_JUSTIFY
  MXC_SPIMSS->i2s_cntl |= MXC_F_SPIMSS_I2S_CNTL_I2S_LJ ;  /* Left-justify to emulate SPI */
#endif
#ifdef INVERT_LRCLK
  MXC_SPIMSS->i2s_cntl |= 0x20; /* UNDOCUMENTED: Invert output LRCLK, only works if we are Master */
#endif

  MXC_SPIMSS->cntl =
    MXC_F_SPIMSS_CNTL_SPIEN |                           /* Enable SPI */
    MXC_F_SPIMSS_CNTL_MMEN;                             /* Master mode */

  MXC_SPIMSS->mod = MXC_F_SPIMSS_MOD_SSIO;              /* SS is output */
  MXC_SPIMSS->mod |= MXC_F_SPIMSS_MOD_SSV;              /* Inverted (active high) SS */
  
  /* Emulator: HCLK is 40 MHz, PCLK is 20 MHz, SPI_MSS has a built-in /2, so effective SCLK is at max 10MHz */
#if (AUDIO_RATE == 44100)
  MXC_SPIMSS->brg = 7;                                  /* Within 1% of 44100 * 16 * 2 */
#elif (AUDIO_RATE == 22050)
  MXC_SPIMSS->brg = 14;                                 /* Within 1% of 22050 * 16 * 2 */
#elif (AUDIO_RATE == 11025)
  MXC_SPIMSS->brg = 28;                                 /* Within 1% of 11025 * 16 * 2 */
#else
  #error "Unsupported audio rate!"
#endif

  MXC_SPIMSS->dma =
    (FIFO_TX_LEVEL << MXC_F_SPIMSS_DMA_TX_FIFO_LEVEL_POS) |       /* TX DMA request FIFO level */
    MXC_F_SPIMSS_DMA_TX_FIFO_CLEAR |                    /* Clear TX FIFO */
    (0x1 << MXC_F_SPIMSS_DMA_RX_FIFO_LEVEL_POS) |       /* RX DMA request FIFO level */
    MXC_F_SPIMSS_DMA_RX_FIFO_CLEAR;                     /* Clear RX FIFO */
  
}

void start_i2s(void)
{
  /* Begin DMA transfers */
  MXC_SPIMSS->dma |= MXC_F_SPIMSS_DMA_TX_DMA_EN;
  MXC_SPIMSS->dma |= MXC_F_SPIMSS_DMA_RX_DMA_EN;
}

void spi_slave_complete(spis_req_t *req, int error_code)
{
    if (error_code != E_NO_ERROR) {
	printf("Error during SPI slave transaction: %u\n", error_code);
    } 

    printf("SPI slave transfer complete.\n");
}

// *****************************************************************************
int main(void)
{
    unsigned int i, failed = 0;
    uint16_t check_word, desired_word;
    spis_req_t spis_req;
    int error;
  
    printf("\n\nMAX32650 I2S test\n");

    generate_data(srcdata, NUM_WORDS_I2S_DATA);

    /* Setup I/O */

    MXC_GPIO2->en &= ~(0x0000003c); /* Clear GPIO enables */
    MXC_GPIO2->en1 &= ~(0x0000003c); /* Enable AF1,3,4 */

    /* Configure SPI slave */
    
    if ((error = SPIS_Init(MXC_SPIS, 0, NULL)) != E_NO_ERROR) {
      printf("Error while configuring SPI slave\n");
      while(1);
    }

    memset(&spis_req, 0, sizeof(spis_req));
    spis_req.deass = 0;
    spis_req.tx_data = NULL;
    spis_req.rx_data = spi_data;
    spis_req.width = 1;
    spis_req.len = NUM_WORDS_I2S_DATA;
    spis_req.callback = spi_slave_complete;

    NVIC_DisableIRQ(MXC_SPIS_GET_IRQ(0));
    NVIC_ClearPendingIRQ(MXC_SPIS_GET_IRQ(0));
    NVIC_EnableIRQ(MXC_SPIS_GET_IRQ(0));

    if ((error = SPIS_TransAsync(MXC_SPIS, &spis_req)) != E_NO_ERROR) {
        printf("Error with Slave Transferring Data Asynchronously: %d\n", error);
        while (1) {}
    }

    /* Clear any old data */
    memset(spi_data, 0, NUM_WORDS_I2S_DATA);
    memset(dstdata, 0, NUM_BYTES_I2S_DATA);
    
    /* DMA configuration */
    
    stop_dma();
    
    init_dma();

#if 0
    printf("cfg: 0x%08x\n", MXC_DMA->ch[0].cfg);
    printf("st: 0x%08x\n", MXC_DMA->ch[0].st);
#endif

    set_tx_dma_range(srcdata, NUM_BYTES_I2S_DATA);
    set_rx_dma_range(dstdata, NUM_BYTES_I2S_DATA);

    /* I2S configuration */
    
    setup_i2s();

    /* Start test */
    
    start_dma();

    start_i2s();
  
    /* Wait for DMA to complete all CNT bytes */
    while (!(MXC_DMA->ch[0].st & MXC_F_DMA_DMA_ST_CTZ_ST));

    if (MXC_SPIMSS->status & MXC_F_SPIMSS_STATUS_TUND) {
      printf("FAIL: DMA did not keep up with I2S (underflow)\n");
      failed = 1;
    }

    /* Catch I2S before transmitting garbage */
    while (!(MXC_SPIMSS->status & MXC_F_SPIMSS_STATUS_TUND));

    /* Stop I2S */
    MXC_SPIMSS->i2s_cntl = 0;

    printf("DMA operation complete\n");
#if 0
    printf("st: 0x%08x\n", MXC_DMA->ch[0].st);
#endif

    printf("Waiting for SPI slave transaction to complete\n");
    while (SPIS_Busy(MXC_SPIS) == E_BUSY);

    printf("Checking result\n");
    
    /* We received only one channel, which is 0.5 * NUM_WORDS_I2S_DATA, but correct for words -> bytes */
    for (i = 0; i < (NUM_WORDS_I2S_DATA / 2); i++) {
      /* Each is a 16 bit word shipped to us in big-endian format */
#ifdef LEFT_JUSTIFY
      check_word = (spi_data[2*i] << 8) + (spi_data[(2*i)+1]);
#ifdef INVERT_LRCLK
      desired_word = (NUM_WORDS_I2S_DATA - ((i * 2) + 2));
#else
      desired_word = (NUM_WORDS_I2S_DATA - ((i * 2) + 1));
#endif /* INVERT_LRCLK */
      if (check_word != desired_word) {
	printf("FAIL: Mismatch at %u: Got 0x%04x, expected 0x%04x\n", i, check_word, desired_word);
	failed = 1;
      }
#else
      /* Can't check LSB of this word .. */
      check_word = ((spi_data[2*i] << 9) + (spi_data[(2*i)+1] << 1)) & 0xffff;
#ifdef INVERT_LRCLK
      desired_word = (NUM_WORDS_I2S_DATA - ((i * 2) + 2));
#else
      desired_word = (NUM_WORDS_I2S_DATA - ((i * 2) + 1));
#endif
      if (check_word != (desired_word & 0xfffe)) {
	printf("FAIL: Mismatch at %u: Got 0x%04x, expected 0x%04x\n", i, check_word, desired_word & 0xfffe);
	failed = 1;
      }
      /* .. but can check the LSB of the previous, other channel word, which should be opposite */
      if (i) {
	/* Only check this if we are past the first sample */
	if ((spi_data[(2*i)] & 0x80) == ((desired_word & 1) << 7)) {
	  printf("FAIL: Mismatch at %u: previous, other channel LSB incorrect!\n", i);
	  failed = 1;
	}
      }
#endif
    }

    /* Check the data received by the I2S receiver, activated simultaneously */
    if (memcmp(srcdata, dstdata, NUM_BYTES_I2S_DATA)) {
      printf("FAIL: Loop-back from I2S failed. Check connections and data\n");
      failed = 1;
    } else {
      printf("Loop-back I2S compared OK\n");
    }
    
    /* Done */
    
    printf("Test complete: %s\n", failed ? "FAIL" : "PASS");

    return 0;
}

/* This interrupt doubles as the SPI slave */
void SPI1_IRQHandler(void)
{
    SPIS_Handler(MXC_SPIS);
}
