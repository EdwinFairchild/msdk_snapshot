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
* @author: Benjamin VINOT <benjamin.vinot@maximintegrated.com>
*
*/

#ifndef __ECDSA_H__
#define __ECDSA_H__

#ifdef __cplusplus
extern "C"
{
#endif


#define ECDSA_MODULUS_LEN				32

typedef struct{
	uint8_t ecdsa_privkey[ECDSA_MODULUS_LEN];
	uint8_t ecdsa_pubkey_x[ECDSA_MODULUS_LEN];
	uint8_t ecdsa_pubkey_y[ECDSA_MODULUS_LEN];
	size_t ecdsa_len;
#ifdef _MAXIM_HSM
	CK_OBJECT_HANDLE HSM_Objkey;
	CK_OBJECT_HANDLE HSM_Objpubkey;
	char HSM_KeyLabel[MAX_LINE];
#endif		
} ecdsa_key_t;


/**
 *
 */
int ecdsa_sign (unsigned char *input, unsigned int input_size, unsigned char *signature, ecdsa_key_t key);

/**
 *
 */
	int read_file_ecdsa (u8 * puk_x, u8 * puk_y, u8 * privk, unsigned int size, char *filename);

/**
 *
 */
	int read_file_signed_ecdsa_publickey (u8 * x, u8 * y, u8 * r, u8 * s, int size, char *filename);

#ifdef _MAXIM_HSM
/**
 *
 */
int load_HSM_ecdsa_key (ecdsa_key_t * key, char *keyname);
#endif				/* _MAXIM_HSM */

#ifdef __cplusplus
}
#endif

#endif				/* __ECDSA_H__ */
