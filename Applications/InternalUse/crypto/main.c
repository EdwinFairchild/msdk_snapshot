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
#include "crypto.h"
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

char result[512];
					
int main(){
	
	#if 1
	printf("\r\n**************************Crypto Test DES/TDES/AES*************************************\r\n");
	const char *keyin = "6f419b4c683a44d67d234eaa6b57f622f912de657dddb280a14d0cb967ed951f";
	char key[32];
	ascii_to_byte(keyin,key, 32);
	
	
		
	const char *iv_src = "19b888800ff1d0116124f79dfae54ffe";
	char iv_dst[16];
	ascii_to_byte(iv_src,iv_dst, 16);
	
	const char *msg2 = "3d12989faf41ba75bfa70e2bcc2fa222";
	char msg[16];
	ascii_to_byte(msg2,msg, 16);
	
	printf("Cipher Config Status: %d\r\n", CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES256));
	
	printf("Encryption status: %d\r\n\n", CRYPTO_AES_Encrypt(msg, iv_dst, key,CRYPTO_CIPHER_AES256, CRYPTO_MODE_OFB, 16, result));
	int j=0;
	for(;j<16;j++) {
	  printf("result[%d]: 0x%x \r\n", j, result[j]);
	}
	
	const char *ciphertext ="2d6b005e8d3bc6ea9f62dca36d47aea5";
	
	char out[16];
	ascii_to_byte(ciphertext,out, 16);
	
	printf("Decryption status: %d\r\n", CRYPTO_AES_Decrypt(out, iv_dst, key,CRYPTO_CIPHER_AES256, CRYPTO_MODE_OFB, 16, result));
	
	for(j=0;j<16;j++) {
	  printf("result[%d]: 0x%x \r\n", j, result[j]);
	}
	
	#endif
	#if 0
	printf("\r\n************************** Crypto Test HASH *************************************\r\n");
	int len,i;
	len = findLength(41696); 
	printf("len=%d\r\n", len); 
	const char *msg = "";
	char msg2[len];
	ascii_to_byte(msg, msg2, len);
	for(i=0; i<len; i++) {
  printf("msg[%d] = 0x%x\r\n	", i, *(msg2+i));
    }
  
    printf("Hash Config Status: %d\r\n", CRYPTO_Hash_Config(CRYPTO_HASH_SHA224));
	printf("SHA status: %d\r\n", CRYPTO_SHA(msg2, CRYPTO_HASH_SHA224, len, result));
	#endif
	#if 0
	printf("\r\n************************** Crypto Test CRC *************************************\r\n");
	int len,i;
	uint32_t crc=0;
	len = 8;
	//printf("len=%d\r\n", len);
    const char *msg = "46b9bd634418b6e9";
	uint8_t msg2[len];
	ascii_to_byte(msg, msg2, len);
	for(i=0; i<len; i++) {
	  printf("msg2[%d] = 0x%02x\r\n", i, msg2[i]);
	}
	CRYPTO_Crc_Config();
	CRYPTO_CRC(msg2, len, CRYPTO_CRC32_ETHERNET, &crc);
	printf("CRC value: 0x%8x; \r\n", crc);
	#endif
	#if 0
	printf("\r\n************************** Crypto Test HAMMING *************************************\r\n");
	int len,i;
	uint32_t *ecc;
	len =4;
	//printf("len=%d\r\n", len);
    const char *msg = "a73a264";
	uint8_t msg2[len];
	ascii_to_byte(msg, msg2, len);
	for(i=0; i<len; i++) {
	  printf("msg2[%d] = 0x%02x\r\n", i, msg2[i]);
	}
	CRYPTO_Ham_Config();
	CRYPTO_HAM(msg2, len, ecc);
	printf("ECC value: 0x%x; Expected: 0x14\r\n", *ecc);
	#endif
	while(1);
	return 0;
}
