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

#ifndef _CHADEMO_CRYPTO_H_
#define _CHADEMO_CRYPTO_H_


// Define SEPARATE_HASH to calculate SHA hash outside of ECDSA sign/verify function.
#define SEPARATE_HASH

// Storage for demo ECDSA private key
extern uint8_t demo_d[];
extern uint8_t demo_x[];
extern uint8_t demo_y[];

// Storage for demo ECDSA public key
extern ucl_type_ecc_u8_affine_point demo_q;

// Storage for demo AES128 key.
extern u8 demo_key[16];
extern u8 demo_iv[16];

int crypto_sign(uint8_t *signatureout);
int crypto_verify(uint8_t *signaturein);
int crypto_decrypt_finish(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout);
int crypto_decrypt_continue(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout);
int crypto_decrypt_start(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout);
int crypto_encrypt_finish(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout);
int crypto_encrypt_continue(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout);
int crypto_encrypt_start(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout);
int crypto_init(void);

#endif // _CHADEMO_CRYPTO_H_

