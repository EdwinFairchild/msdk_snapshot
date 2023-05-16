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
 * $Date: 2018-08-29 15:05:41 -0500 (Wed, 29 Aug 2018) $
 * $Revision: 37503 $
 *
 ******************************************************************************/
/* * As an example, given the key @a 0x139A35422F1D61DE3C91787FE0507AFD, the proper storage order is:
 * ~~~~~
 * uint8_t key[16] = { 0xFD, 0x7A, 0x50, 0xE0,
 *                     0x7F, 0x78, 0x91, 0x3C,
 *                     0xDE, 0x61, 0x1D, 0x2F,
 *                     0x42, 0x35, 0x9A, 0x13 };*/
/**
 * @file    main.c
 * @brief   GPIO test.
 *
 * @detail  First, turn off the conflicting pins/ports on the EV Kit, per the "masks" array
 *          in the "Globals" section.
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "max32650.h"
#include "board.h"
#include "maa.h"
#include "tpu_regs.h"

/***** Definitions *****/

char temp[] = {0x00, 0x00, 0x00};

void ascii_to_byte(const char *src, char *dst, int len){
  int i;
  for(i=0; i<len; i++) {
    int val;
    temp[0] = *src; src++;
    temp[1] = *src; src++;
    sscanf(temp, "%0x", &val);
    dst[i] = val;
  }
}

unsigned int findLength(int num){
  unsigned int len = 0;
  len = num/8;
  if(num%8 != 0)
    len++;
  return len;
}

 
void rvereseArray(unsigned char arr[], int start, int end)
{
    int temp;
    while (start < end)
    {
        temp = arr[start];   
        arr[start] = arr[end-1];
        arr[end] = temp;
        start++;
        end--;
    }   
}     

char exponent_data[] = {
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f };
   //unsigned char *exponent_data = NULL;  
   
char multiplier_data[] = {
  0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
  0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12 };

char multiplicand_data[] = {
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


char modulus_data[] = {
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f };

char result_data[] = {
      0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe,
      0xf0, 0xde, 0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12 };


int result[256];
					
int main(){
	
	
	printf("\r\n************************** Crypto Test MAA *************************************\r\n");
	int len,i=0;
	len = 127;
	//result = (unsigned int*)malloc(20);
	printf("MAA Config status: 0x%x\r\n", MAA_Init(len));
	len = findLength(len);
	printf("len: %d\r\n", len);
	
	
	if((len % 4) !=0) {
	  len = (len + 4 - (len%4));
	}
	printf("len: %d\r\n", len);
	//rvereseArray(multiplier_data, 0, len-1);
	//rvereseArray(multiplicand_data, 0, len-1);
	//rvereseArray(modulus_data, 0, len-1);
	
	printf("EXP status: %d\r\n",MAA_Compute(CRYPTO_MAA_EXP, multiplier_data, multiplicand_data, exponent_data, modulus_data, result, len));
	
	while(i<len/4) {
	  printf(" result[%d] = 0x%x\r\n", i, *(result+i));
	  i++;
	}
	
	return 0;
}
