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

/**
 * @file    	main.c
 * @brief   	AES Example
 * @details 	Encryption and decryption of AES on different modes (ECB and OFB) with different bit sizes (128, 192, and 256)
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "max32650.h"
#include "board.h"
#include "crypto.h"
#include "tpu_regs.h"

/***** Definitions *****/
#define MXC_AES_DATA_LEN 	(128 / 8)	/**< Number of bytes in an AES plaintext or cyphertext block, which are always 128-bits long. */
#define MXC_AES_KEY_128_LEN 	(128 / 8)	/**< Number of bytes in a AES-128 key. */
#define MXC_AES_KEY_192_LEN 	(192 / 8)	/**< Number of bytes in a AES-192 key. */
#define MXC_AES_KEY_256_LEN 	(256 / 8)	/**< Number of bytes in a AES-256 key. */

/***** Globals *****/
char temp[] = {0x00, 0x00, 0x00}; //for aes_ascii_to_byte()
char result[512];

/***** Functions *****/
//aes_print human-friendly readable output of array
void aes_print(char *stuff)
{
    int i, j, size = 4;
	for(i = 0; i < 4; ++i) {
		for(j = 0; j < 4; ++j) {
			printf("0x%02x ", stuff[i*size+j]);
		}
		printf("\n");
	}
	return;
}

//Convert ascii to byte
void aes_ascii_to_byte(const char *src, char *dst, int len)
{
	int i;
	for(i=0; i < len; ++i) {
		int val;
		temp[0] = *src; src++;
		temp[1] = *src; src++;
		sscanf(temp, "%0x", &val);
		dst[i] = val;
	}
	return;
}

//Verify by comparing calculated to expected
void AES_check(char *calculated, char *expected, int len)
{
	int i, fail = 0;
	for(i = 0; i < len; ++i) {
		if(calculated[i] != expected[i]) ++fail;
	}
	if(fail > 0) printf("Fail.\n");
	else printf("Pass.\n");
	return;
}

void AES128_ECB_enc(void)
{
	printf("128-bit AES ECB Encryption ... ");
	const char *_key = "10a58869d74be5a374cf867cfb473859";
	char key[MXC_AES_KEY_128_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);
	
	const char *iv_src = "";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_msg = "00000000000000000000000000000000";
	char msg[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_msg, msg, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_ECB, CRYPTO_CIPHER_AES128);
	CRYPTO_AES_Encrypt(msg, iv_dst, key, CRYPTO_CIPHER_AES128, CRYPTO_MODE_ECB, MXC_AES_DATA_LEN, result);

	const char *_expected = "6d251e6944b051e04eaa6fb4dbf78465";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES128_ECB_dec(void)
{
	printf("128-bit AES ECB Decryption ... ");
	const char *_key = "caea65cdbb75e9169ecd22ebe6e54675";
	char key[MXC_AES_KEY_128_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);
	
	const char *iv_src = "";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_ct= "6e29201190152df4ee058139def610bb";
	char ct[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_ct, ct, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_ECB, CRYPTO_CIPHER_AES128);
	CRYPTO_AES_Decrypt(ct, iv_dst, key, CRYPTO_CIPHER_AES128, CRYPTO_MODE_ECB, MXC_AES_DATA_LEN, result);

	const char *_expected = "00000000000000000000000000000000";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES192_ECB_enc(void)
{
	printf("192-bit AES ECB Encryption ... ");
	const char *_key = "ffffffffffffffffffffffffffffffffffffffffffffc000";
	char key[MXC_AES_KEY_192_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_192_LEN);
	
	const char *iv_src = "";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_pt= "00000000000000000000000000000000";
	char pt[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_pt, pt, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_ECB, CRYPTO_CIPHER_AES192);
	CRYPTO_AES_Encrypt(pt, iv_dst, key, CRYPTO_CIPHER_AES192, CRYPTO_MODE_ECB, MXC_AES_DATA_LEN, result);

	const char *_expected = "ace4b91c9c669e77e7acacd19859ed49";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);	
	return;
}

void AES192_ECB_dec(void)
{
	printf("192-bit AES ECB Decryption ... ");
	const char *_key = "ffffffffffffffffffffffffffffffffffffffffe0000000";
	char key[MXC_AES_KEY_192_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_192_LEN);
	
	const char *iv_src = "";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_ct= "fb4bc78b225070773f04c40466d4e90c";
	char ct[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_ct, ct, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_ECB, CRYPTO_CIPHER_AES192);
	CRYPTO_AES_Decrypt(ct, iv_dst, key, CRYPTO_CIPHER_AES192, CRYPTO_MODE_ECB, MXC_AES_DATA_LEN, result);

	const char *_expected = "00000000000000000000000000000000";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);	
	return;
}

void AES256_ECB_enc(void)
{
	printf("256-bit AES ECB Encryption ... ");
	const char *_key = "797f8b3d176dac5b7e34a2d539c4ef367a16f8635f6264737591c5c07bf57a3e";
	char key[MXC_AES_KEY_256_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_256_LEN);
	
	const char *iv_src = "";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_pt= "00000000000000000000000000000000";
	char pt[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_pt, pt, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_ECB, CRYPTO_CIPHER_AES256);
	CRYPTO_AES_Encrypt(pt, iv_dst, key, CRYPTO_CIPHER_AES256, CRYPTO_MODE_ECB, MXC_AES_DATA_LEN, result);

	const char *_expected = "a74289fe73a4c123ca189ea1e1b49ad5";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);		
	return;
}

void AES256_ECB_dec(void)
{
	printf("256-bit AES ECB Decryption ... ");
	const char *_key = "13428b5e4c005e0636dd338405d173ab135dec2a25c22c5df0722d69dcc43887";
	char key[MXC_AES_KEY_256_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_256_LEN);
	
	const char *iv_src = "";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_ct= "649a71545378c783e368c9ade7114f6c";
	char ct[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_ct, ct, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_ECB, CRYPTO_CIPHER_AES256);
	CRYPTO_AES_Decrypt(ct, iv_dst, key, CRYPTO_CIPHER_AES256, CRYPTO_MODE_ECB, MXC_AES_DATA_LEN, result);

	const char *_expected = "00000000000000000000000000000000";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES128_OFB_enc(void)
{
	printf("128-bit AES OFB Encryption ... ");
	const char *_key = "00000000000000000000000000000000";
	char key[MXC_AES_KEY_128_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);
	
	const char *iv_src = "b26aeb1874e47ca8358ff22378f09144";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_pt= "00000000000000000000000000000000";
	char pt[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_pt, pt, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES128);
	CRYPTO_AES_Encrypt(pt, iv_dst, key, CRYPTO_CIPHER_AES128, CRYPTO_MODE_OFB, MXC_AES_DATA_LEN, result);

	const char *_expected = "459264f4798f6a78bacb89c15ed3d601";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES128_OFB_dec(void)
{
	printf("128-bit AES OFB Decryption ... ");
	const char *_key = "00000000000000000000000000000000";
	char key[MXC_AES_KEY_128_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);
	
	const char *iv_src = "cb9fceec81286ca3e989bd979b0cb284";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_ct= "92beedab1895a94faa69b632e5cc47ce";
	char ct[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_ct, ct, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES128);
	CRYPTO_AES_Decrypt(ct, iv_dst, key, CRYPTO_CIPHER_AES128, CRYPTO_MODE_OFB, MXC_AES_DATA_LEN, result);

	const char *_expected = "00000000000000000000000000000000";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES192_OFB_enc(void)
{
	printf("192-bit AES OFB Encryption ... ");
	const char *_key = "15d20f6ebc7e649fd95b76b107e6daba967c8a9484797f29";
	char key[MXC_AES_KEY_192_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_192_LEN);
	
	const char *iv_src = "00000000000000000000000000000000";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_pt= "00000000000000000000000000000000";
	char pt[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_pt, pt, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES192);
	CRYPTO_AES_Encrypt(pt, iv_dst, key, CRYPTO_CIPHER_AES192, CRYPTO_MODE_OFB, MXC_AES_DATA_LEN, result);

	const char *_expected = "8e4e18424e591a3d5b6f0876f16f8594";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES192_OFB_dec(void)
{
	printf("192-bit AES OFB Decryption ... ");
	const char *_key = "000000000000000000000000000000000000000000000000";
	char key[MXC_AES_KEY_192_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_192_LEN);
	
	const char *iv_src = "51719783d3185a535bd75adc65071ce1";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_ct= "4f354592ff7c8847d2d0870ca9481b7c";
	char ct[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_ct, ct, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES192);
	CRYPTO_AES_Decrypt(ct, iv_dst, key, CRYPTO_CIPHER_AES192, CRYPTO_MODE_OFB, MXC_AES_DATA_LEN, result);

	const char *_expected = "00000000000000000000000000000000";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES256_OFB_enc(void)
{
	printf("256-bit AES OFB Encryption ... ");
	const char *_key = "8000000000000000000000000000000000000000000000000000000000000000";
	char key[MXC_AES_KEY_256_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_256_LEN);
	
	const char *iv_src = "00000000000000000000000000000000";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_pt= "00000000000000000000000000000000";
	char pt[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_pt, pt, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES256);
	CRYPTO_AES_Encrypt(pt, iv_dst, key, CRYPTO_CIPHER_AES256, CRYPTO_MODE_OFB, MXC_AES_DATA_LEN, result);

	const char *_expected = "e35a6dcb19b201a01ebcfa8aa22b5759";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}

void AES256_OFB_dec(void)
{
	printf("256-bit AES OFB Encryption ... ");
	const char *_key = "0000000000000000000000000000000000000000000000000000000000000000";
	char key[MXC_AES_KEY_256_LEN];
	aes_ascii_to_byte(_key, key, MXC_AES_KEY_256_LEN);
	
	const char *iv_src = "761c1fe41a18acf20d241650611d90f1";
	char iv_dst[16];
	aes_ascii_to_byte(iv_src, iv_dst, 16);
	
	const char *_ct= "623a52fcea5d443e48d9181ab32c7421";
	char ct[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_ct, ct, MXC_AES_DATA_LEN);

	CRYPTO_Cipher_Config(CRYPTO_MODE_OFB, CRYPTO_CIPHER_AES256);
	CRYPTO_AES_Decrypt(ct, iv_dst, key, CRYPTO_CIPHER_AES256, CRYPTO_MODE_OFB, MXC_AES_DATA_LEN, result);

	const char *_expected = "00000000000000000000000000000000";
	char expected[MXC_AES_DATA_LEN];
	aes_ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

	AES_check(result, expected, MXC_AES_DATA_LEN);
	return;
}


// *****************************************************************************
int aes_main(void)
{
	printf("\n***** AES Example *****\n");

	//ECB
	AES128_ECB_enc();
	AES128_ECB_dec();
	AES192_ECB_enc();
	AES192_ECB_dec();
	AES256_ECB_enc();
	AES256_ECB_dec();

	printf("\n");

	//OFB
	AES128_OFB_enc();
	AES128_OFB_dec();
	AES192_OFB_enc();
	AES192_OFB_dec();
	AES256_OFB_enc();
	AES256_OFB_dec();

	printf("\nExample complete.\n");
    return 0;
}
