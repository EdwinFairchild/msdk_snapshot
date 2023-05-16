/**
 * @file    main.c
 * @brief   iRhythm routine to re-program information block contents
 */

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
 * $Date: 2019-04-18 12:22:26 -0500 (Thu, 18 Apr 2019) $
 * $Revision: 42667 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "led.h"
#include "flc.h"
#include "gpio.h"

#include "device1_data.h"
#include "device2_data.h"
#include "device3_data.h"

/***** Definitions *****/
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define FLASH_BUSY (MXC_FLC0->cn & (MXC_F_FLC_CN_WR | MXC_F_FLC_CN_ME | MXC_F_FLC_CN_PGE))

uint32_t crc32(const void *buf, unsigned size);

/***** Globals *****/

/***** Functions *****/
void reprogram_info(const uint32_t *device)
{
  uint32_t addr, idx;
  
  /* Set flash clock divider to generate a 1MHz clock from the APB clock */
  MXC_FLC0->clkdiv = SystemCoreClock / 1000000;
  
  /* Clear stale errors */
  if (MXC_FLC0->intr & MXC_F_FLC_INTR_AF) {
    MXC_FLC0->intr &= ~MXC_F_FLC_INTR_AF;
  }
  
  /* Unlock flash */
  MXC_FLC0->cn = (MXC_FLC0->cn & ~MXC_F_FLC_CN_UNLOCK) | MXC_S_FLC_CN_UNLOCK_UNLOCKED;

  /* Write page erase code, address, and start */
  printf("Erasing info block ..\n");
  MXC_FLC0->cn = (MXC_FLC0->cn & ~MXC_F_FLC_CN_ERASE_CODE) | MXC_S_FLC_CN_ERASE_CODE_ERASEPAGE;
  MXC_FLC0->addr = 0x80000;
  MXC_FLC0->cn |= MXC_F_FLC_CN_PGE;

  /* Wait for completion */
  while (FLASH_BUSY);
  printf(" ... done.\n");

  /* Wait a bit more */
  for (idx = 0; idx < 0x1000000; idx++) {
    __NOP();
  }
  
  /* Unlock flash */
  MXC_FLC0->cn = (MXC_FLC0->cn & ~MXC_F_FLC_CN_UNLOCK) | MXC_S_FLC_CN_UNLOCK_UNLOCKED;
  
  /* Program contents */
  printf("Programming info block ..\n");
  for (addr = 0; addr < 0x1000; addr += 16) {
    idx = addr / 4;
    MXC_FLC0->addr = 0x80000 + addr;
    MXC_FLC0->data[0] = device[idx];
    MXC_FLC0->data[1] = device[idx+1];
    MXC_FLC0->data[2] = device[idx+2];
    MXC_FLC0->data[3] = device[idx+3];
    MXC_FLC0->cn |= MXC_F_FLC_CN_WR;
    while (FLASH_BUSY);
  }
  printf(" ... done.\n");

  /* Wait a bit more */
  for (idx = 0; idx < 0x1000000; idx++) {
    __NOP();
  }
  
  /* Lock flash */
  MXC_FLC0->cn &= ~MXC_F_FLC_CN_UNLOCK;
}

int main(void)
{
  uint32_t *tme = (uint32_t *)0x40000C00;
  gpio_cfg_t button =
    {
     .port = PORT_1,
     .mask = PIN_0,
    };
  
  printf("\n\n***** MAX" TOSTRING(TARGET) " Information Block Re-program *****\n");

  /* Turn on test mode */
  *tme |= 1;
  if ((*tme & 1) == 0) {
    printf(" FAIL: This device is test-mode locked and cannot be reprogrammed.\n");
    while(1);
  }
  
  /* Unlock information block for readback */
  MXC_FLC0->acntl = 0x3a7f5ca3;
  MXC_FLC0->acntl = 0xa1e34f20;
  MXC_FLC0->acntl = 0x9608b2c1;

  printf("Press button to reprogram device trim data\n");
  
  while (!GPIO_InGet(&button));
  reprogram_info(device3_data);

  printf("Programming complete. Please power-cycle board to reload info block.\n");
  
  return 0;
}
