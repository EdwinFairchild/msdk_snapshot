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
 * $Date: 2018-08-10 09:36:39 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   development test for DMA
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "led.h"
#include "tmr_utils.h"
#include "gpio.h"
#include "dma.h"

/***** Definitions *****/

#define DMA_CHANNELS     16

#define DATA_SIZE     256
#define BUF_SIZE     4096
#define TIMES          10

/***** Globals *****/

static const uint8_t src_flash[DATA_SIZE] = {
  0x00,0x00,0x00,0x00,0x00,0xe0,0x00,0x00,0x00,0x00,0x00,0xe0,0x00,0x80,0x08,0x00,
  0x00,0xe0,0x00,0xf0,0x54,0x01,0x00,0xe0,0x00,0xfe,0x90,0x04,0x00,0xe0,0x00,0xff,
  0xa9,0x2a,0x00,0xe0,0xc0,0xff,0x54,0x92,0x00,0xe0,0xe0,0xff,0x24,0x55,0x00,0xe0,
  0xe0,0xfb,0x48,0xa5,0x02,0xe0,0xe0,0xff,0xa9,0x52,0x01,0xe0,0x60,0xdf,0x54,0x4a,
  0x02,0xe0,0xe0,0xff,0x20,0xa9,0x0a,0xe0,0xe0,0xff,0x4c,0x95,0x0a,0xe0,0xe0,0xfb,
  0xa9,0x52,0x12,0xe0,0xe0,0xff,0x00,0x00,0x00,0xe0,0x60,0xcf,0x00,0x00,0x00,0xe0,
  0xe0,0xcf,0x54,0x55,0x15,0xe0,0xe0,0xcf,0x29,0x55,0x55,0xe0,0xe0,0xcf,0x50,0x49,
  0x52,0xe0,0xe0,0xce,0x94,0x54,0x15,0xe0,0xe0,0xcf,0xa8,0x92,0x24,0xe0,0xe0,0xcf,
  0x49,0x55,0x55,0xe0,0x00,0xc0,0x94,0xa4,0x2a,0xe0,0x00,0xc0,0xa4,0x2a,0x49,0xe0,
  0xe0,0xcf,0x28,0x55,0x52,0xe0,0xe0,0xdf,0xa9,0x92,0x2a,0xe0,0xe0,0xcf,0x24,0xa5,
  0x4a,0xe0,0xe0,0xcf,0x50,0xa9,0x24,0xe0,0xe0,0xce,0x94,0x4a,0x55,0xe0,0xe0,0xcf,
  0x01,0x00,0x00,0xe0,0xe0,0xdf,0x00,0x00,0x00,0xe0,0xc0,0xff,0x54,0x55,0x15,0xe0,
  0xe0,0xff,0xa8,0xaa,0x0a,0xe0,0xe0,0xfb,0x55,0x92,0x04,0xe0,0xe0,0xff,0x48,0x55,
  0x09,0xe0,0x60,0xdf,0xa4,0x4a,0x02,0xe0,0xe0,0xff,0x28,0xa9,0x02,0xe0,0xe0,0xff,
  0x94,0x94,0x00,0xe0,0xc0,0xfb,0xa8,0x52,0x00,0xe0,0x80,0xff,0xa5,0x2a,0x00,0xe0,
  0x00,0xfe,0x48,0x0a,0x00,0xe0,0x00,0xf8,0xa8,0x02,0x00,0xe0,0x00,0xc0,0x94,0x00,
};
  
uint8_t src_sram[BUF_SIZE];
volatile uint8_t dst_sram[BUF_SIZE];
volatile uint8_t dst_sram_chained[BUF_SIZE*TIMES];


volatile int dma_done;

/***** Functions *****/

void dma_cb(int ch, int err)
{
  unsigned int flags;
  
  if (DMA_GetFlags(ch, &flags) != E_NO_ERROR) {
    printf("Can't get flags in ISR!\n");
    while (1);
  }
  if (flags & (MXC_F_DMA_ST_BUS_ERR | MXC_F_DMA_ST_TO_ST)) {
    printf("Channel error detected!\n");
    while (1);
  }

  DMA_ClearFlags(ch);

  dma_done = 1;
}

/******************************************************************************/
int main(void)
{
  int ch[DMA_CHANNELS], i;

  DMA_Init();

  /* Allocate all channels */
  i = 0;
  while (i < DMA_CHANNELS) {
    if ((ch[i++] = DMA_AcquireChannel()) < 0) {
      printf("Failure to allocate channel %u (out of %u possible)\n", i-1, DMA_CHANNELS);
      goto bail_out;
    }
  }

  for (i = 0; i < DMA_CHANNELS; i++) {
    printf("ch[%u] = %u\n", i, ch[i]);
  }

  /* Try to allocate one more, which should fail */
  if (DMA_AcquireChannel() != E_NONE_AVAIL) {
    printf("Aquire channel gave us a channel when it shouldn't have\n");
    goto bail_out;
  }

  /* Free one channel, then try again */
  if (DMA_ReleaseChannel(ch[0]) != E_NO_ERROR) {
    printf("Can't release channel\n");
    goto bail_out;
  }
  if ((ch[0] = DMA_AcquireChannel()) < 0) {
    printf("Unable to get free channel\n");
    goto bail_out;
  }

  /* Configure one channel */
  if (DMA_ConfigChannel(ch[1], MXC_V_DMA_CFG_PRI_HIGH,
			MXC_V_DMA_CFG_REQSEL_MEMTOMEM, 0,
			DMA_TIMEOUT_4_CLK, DMA_PRESCALE_DISABLE,
			DMA_WIDTH_BYTE, 1,
			DMA_WIDTH_BYTE, 1,
			4, 1, 1) != E_NO_ERROR) {
    printf("Configure channel failed\n");
    goto bail_out;
  }

  /* Source, destination, and count */
  printf("SRAM to SRAM test\n");
  if (DMA_SetSrcDstCnt(ch[1], src_sram, (void *)dst_sram, BUF_SIZE) != E_NO_ERROR) {
    printf("Failed to set source/destination/count\n");
    goto bail_out;
  }
  for (i = 0; i < (BUF_SIZE/DATA_SIZE); i++) {
    memcpy(src_sram+i, src_flash, BUF_SIZE);
  }
  bzero((void *)dst_sram, BUF_SIZE);

  dma_done = 0;
  DMA_SetCallback(ch[1], dma_cb);
  DMA_EnableInterrupt(ch[1]);
  NVIC_EnableIRQ(DMA1_IRQn);

  if (dma_done) {
    printf("Early dma_done\n");
    goto bail_out;
  }
  
  DMA_Start(ch[1]);

  while (!dma_done);

  if (memcmp((void *)src_sram, (void *)dst_sram, BUF_SIZE)) {
    printf("Memory mismatch on compare\n");
    goto bail_out;
  }
  
  /* Source, destination, and count */
  printf("Chained SRAM to SRAM test\n");
  if (DMA_SetSrcDstCnt(ch[1], src_sram, (void *)dst_sram_chained, BUF_SIZE) != E_NO_ERROR) {
    printf("Failed to set source/destination/count\n");
    goto bail_out;
  }
  if (DMA_SetReload(ch[1], src_sram, (void *)dst_sram_chained+BUF_SIZE, BUF_SIZE) != E_NO_ERROR) {
    printf("Failed to set reload source/destination/count\n");
    goto bail_out;
  }

  for (i = 0; i < (BUF_SIZE/DATA_SIZE); i++) {
    memcpy(src_sram+i, src_flash, BUF_SIZE);
  }
  bzero((void *)dst_sram_chained, BUF_SIZE*TIMES);

  dma_done = 0;
  DMA_SetCallback(ch[1], dma_cb);
  DMA_EnableInterrupt(ch[1]);
  NVIC_EnableIRQ(DMA1_IRQn);

  if (dma_done) {
    printf("Early dma_done\n");
    goto bail_out;
  }

  DMA_Start(ch[1]);
  for (i = 1; i < TIMES; i++) {
    while (!dma_done);
    dma_done = 0;
    /* Next reload */
    if (DMA_SetReload(ch[1], src_sram, (void *)dst_sram_chained+(i*BUF_SIZE), BUF_SIZE) != E_NO_ERROR) {
      printf("Failed to set reload source/destination/count\n");
      goto bail_out;
    }
  }

  for (i = 0; i < TIMES; i++) {
    if (memcmp(src_sram, (void *)dst_sram_chained+(i*BUF_SIZE), BUF_SIZE)) {
      printf("Memory mismatch on compare\n");
      goto bail_out;
    }
  }
  
  printf("Test OK!\n");
  
 bail_out:  
  return 0;
}

void DMA1_IRQHandler(void)
{
  DMA_Handler(1);
}
