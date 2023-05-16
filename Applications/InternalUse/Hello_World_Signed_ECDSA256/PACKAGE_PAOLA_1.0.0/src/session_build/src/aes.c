/*******************************************************************************
* Copyright (C) 2009-2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
*******************************************************************************
*
* @author: Yann Loisel <yann.loisel@maximintegrated.com>
* @author: Benjamin VINOT <benjamin.vinot@maximintegrated.com>
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <ucl/ucl_config.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_aes.h>
#include <ucl/ucl_aes_ecb.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_defs.h>

#include "session_build.h"
#include "scp_definitions.h"
#include "scp_utils.h"

int crypto_aes_cmac (u8 * mac, u32 mac_byteLen, u8 * key, u8 * dataIn, u32 byteLen)
{
	u8 null_data[UCL_AES_BLOCKSIZE];
	u8 resu[UCL_AES_BLOCKSIZE];
	u8 k1[16];
	u8 k2[16];
	u8 l[UCL_AES_BLOCKSIZE];
	u8 block[UCL_AES_BLOCKSIZE];
	u8 memory[UCL_AES_BLOCKSIZE];
	int i, j;
	if (mac == NULL || dataIn == NULL || key == NULL)
		return (UCL_INVALID_INPUT);
	if ((byteLen % UCL_AES_BLOCKSIZE) != 0)
		return (UCL_INVALID_ARG);
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		null_data[i] = 0;
	/* subkeys computation */
	ucl_aes_ecb (l, null_data, UCL_AES_BLOCKSIZE, key, UCL_AES_BLOCKSIZE, UCL_CIPHER_ENCRYPT);

	for (i = 0; i < UCL_AES_BLOCKSIZE - 1; i++)
		k1[i] = (l[i] << 1) ^ (l[i + 1] >> 7);
	k1[UCL_AES_BLOCKSIZE - 1] = (l[UCL_AES_BLOCKSIZE - 1] << 1) & 0xFF;
	/* xor with Rb */
	if (l[0] >> 7)
		k1[UCL_AES_BLOCKSIZE - 1] ^= 0x87;
	for (i = 0; i < UCL_AES_BLOCKSIZE - 1; i++)
		k2[i] = (k1[i] << 1) ^ (k1[i + 1] >> 7);
	k2[UCL_AES_BLOCKSIZE - 1] = (k1[UCL_AES_BLOCKSIZE - 1] << 1) & 0xFF;
	if (k1[0] >> 7)
		k2[UCL_AES_BLOCKSIZE - 1] ^= 0x87;
	/* ---------------------------------------- */
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		memory[i] = 0;
	for (i = 0; i < (int) (byteLen - UCL_AES_BLOCKSIZE); i += UCL_AES_BLOCKSIZE)
	{
		for (j = 0; j < UCL_AES_BLOCKSIZE; j++)
			block[j] = dataIn[j + i] ^ memory[j];

		ucl_aes_ecb (resu, block, UCL_AES_BLOCKSIZE, key, UCL_AES_BLOCKSIZE, UCL_CIPHER_ENCRYPT);

		for (j = 0; j < UCL_AES_BLOCKSIZE; j++)
			memory[j] = resu[j];
	}
	for (j = 0; j < UCL_AES_BLOCKSIZE; j++)
		block[j] = dataIn[byteLen - UCL_AES_BLOCKSIZE + j] ^ k1[j] ^ memory[j];

	ucl_aes_ecb (resu, block, UCL_AES_BLOCKSIZE, key, UCL_AES_BLOCKSIZE, UCL_CIPHER_ENCRYPT);

	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		mac[i] = 0;
	for (i = 0; i < (int) mac_byteLen; i++)
		mac[i] = resu[i];
	return (UCL_OK);
}

int aes_encrypt_payload (void)
{
	unsigned int i;
	u8 output[MAX_FRAME];
	u8 input[MAX_FRAME];
	unsigned int inputsize;
	int resu;
	/* copy payload */
	memcpy (input, payload, ipayload);
	padding (&inputsize, input);
	if (TRUE == verbose)
	{
		printf ("INFO: plain payload:");
		for (i = 0; i < inputsize; i++)
			printf ("%02x", input[i]);
		printf ("\n");
	}
	/* encrypt it with key-c */

	resu = ucl_aes_ecb (output, input, 16, keyc, inputsize, UCL_CIPHER_ENCRYPT);

	if (resu != UCL_OK)
	{
		printf ("ERROR during AES encryption\n");
		return (EXIT_FAILURE);
	}
	if (TRUE == verbose)
	{
		printf ("INFO: encrypted payload:");
		for (i = 0; i < inputsize; i++)
			printf ("%02x", output[i]);
		printf ("\n");
	}
	/* replace padded plain payload by encrypted data */
	ipayload = inputsize;
	for (i = 0; i < ipayload; i++)
		payload[i] = output[i];
	return (EXIT_SUCCESS);
}

int aes_cmac_payload (void)
{
	u8 cmac[UCL_AES_BLOCKSIZE];
	int resu;
	unsigned int i;
	int j;
	u8 input[MAX_FRAME];
	unsigned int inputsize;
	if (TRUE == verbose)
		printf ("INFO:payload signed\n");
	/* copy payload */
	if (TRUE == verbose)
	{
		printf ("INFO: payload:");
		for (i = 0; i < ipayload; i++)
			printf ("%02x", payload[i]);
		printf ("\n");
	}
	for (j = 0, i = 4; i < ipayload; i++, j++)
		input[j] = payload[i];
	if (TRUE == verbose)
	{
		printf ("INFO:input(%d):", ipayload - 4);
		for (i = 0; i < ipayload - 4; i++)
			printf ("%02x", input[i]);
		printf ("\n");
	}
	padding (&inputsize, input);
	if (TRUE == verbose)
	{
		printf ("INFO:padded input(%d):", inputsize);
		for (i = 0; i < inputsize; i++)
			printf ("%02x", input[i]);
		printf ("\n");
	}
	if (TRUE == verbose)
	{
		printf ("INFO:keys:");
		for (i = 0; i < 16; i++)
			printf ("%02x", keys[i]);
		printf ("\n");
	}
	resu = crypto_aes_cmac (cmac, UCL_AES_BLOCKSIZE, keys, input, inputsize);
	if (resu != UCL_OK)
	{
		printf ("ERROR during CMAC computation\n");
		return (EXIT_FAILURE);
	}
	if (TRUE == verbose)
	{
		printf ("INFO:payload cmac:");
		for (i = 0; i < inputsize; i++)
			printf ("%02x", cmac[i]);
		printf ("\n");
	}
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		payload[ipayload++] = cmac[i];
	return (EXIT_SUCCESS);
}

int aes_checksum (u8 * crc, u8 * data, int size, int trunk)
{
	int i, j;
	u8 keynull[16];
	u8 h[16];
	int resu;
	u8 input[16];
	for (i = 0; i < 16; i++)
		h[i] = keynull[i] = 0;
	for (i = 0; i < size; i += 16)
	{
		for (j = 0; j < 16; j++)
			if (i + j < size)
				input[j] = h[j] ^ data[i + j];
			else
			{
				/* for SBL, for bytes out of size, stuffing is made with h */
				/*      if(SBL==session_mode) */
				input[j] = h[j];
				/*            else */
				/* for SCP, stuffing is made with 0 ? or not ? to be clarified... */
				/* input[j]=0; */
			}

		resu = ucl_aes_ecb (h, input, 16, keynull, 16, UCL_CIPHER_ENCRYPT);

		if (resu != UCL_OK)
		{
			printf ("ERROR AES\n");
			return (resu);
		}
	}
	for (i = 0; i < trunk; i++)
		crc[i] = h[i];
	return (UCL_OK);
}
