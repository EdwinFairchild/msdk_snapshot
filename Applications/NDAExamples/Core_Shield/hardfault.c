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
 * $Date: 2018-08-27 15:25:00 -0500 (Mon, 27 Aug 2018) $
 * $Revision: 37359 $
 *
 ******************************************************************************/

/* This set of utility routines decodes the HardFault handler for easier debug */

extern unsigned int _data;
extern unsigned int __StackTop;

#include <stdio.h>
#include "mxc_config.h"

void HardFault_Handler(void)
{
  __asm(
	" TST LR, #4\n"
        " ITE EQ \n"
        " MRSEQ R0, MSP \n"
        " MRSNE R0, PSP \n"
        " B HardFault_Decoder \n");
}

void HardFault_Decoder(unsigned int *f)
{
  printf("\n-- HardFault --\n");
  printf("HFSR 0x%08x CFSR 0x%08X", (unsigned int)SCB->HFSR, (unsigned int)SCB->CFSR);
  if (SCB->CFSR & 0x80) {
    printf(", MMFAR 0x%08X", (unsigned int)SCB->MMFAR);
  }
  if (SCB->CFSR & 0x8000) {
    printf(", BFAR 0x%08X", (unsigned int)SCB->BFAR);
  }
  printf("\n");
  if ((f < &_data) || (f > &__StackTop)) {
    printf("Invalid fault stack address (0x%08X). Decode is likely incorrect!\n", (unsigned int)f);
  }
  printf("stacked pc=0x%08X\tlr=0x%08X\tpsr=0x%08X\n", f[6], f[5], f[7]);
  
  /* Hang here */
  while(1);
}
