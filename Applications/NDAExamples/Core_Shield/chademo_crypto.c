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

/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include <ucl/types.h>
#include <ucl/ucl_config.h>
#include <ucl/ucl_defs.h>
//#include <ucl/ucl_info.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_sys.h>
#include <ucl/ucl_rng.h>
#include <ucl/ucl_aes.h>
#include <ucl/ucl_aes_cbc.h>
#include <ucl/ucl_aes_ecb.h>

// Hash related
#include <ucl/ucl_sha256.h>

// ECDSA related
#include <ucl/ecdsa_generic_api.h>
//#include <ucl/ucl_ecc_keygen.h>

#include "chademo_crypto.h"

/***** Definitions *****/

// Define SEPARATE_HASH to calculate SHA hash outside of ECDSA sign/verify function.
#define SEPARATE_HASH

#define UCL_WORK_SIZE_WORDS    1024

/***** Globals *****/

volatile unsigned int kill_selftest = 0;

/***** Functions *****/

__attribute__((section(".bss"),aligned(4))) uint32_t ucl_work_buffer[UCL_WORK_SIZE_WORDS];
__attribute__((section(".bss"),aligned(4))) ucl_sha256_ctx_t sha256_context;
extern ucl_type_curve secp256r1;

// SHA256 context for all SHA state.
ucl_sha256_ctx_t sha256_context;

// Storage for demo ECDSA private key
uint8_t demo_d[] = {
0xD5, 0x7D, 0x1B, 0xD2, 0xDF, 0x0F, 0x18, 0xA7, 0x74, 0x24, 0xE6, 0x6F, 0x58, 0xC4, 0x2B, 0x7B, 
0x58, 0xC7, 0x66, 0xA6, 0xC6, 0x65, 0x7B, 0xCD, 0x2B, 0xC6, 0x32, 0xE4, 0x09, 0x7A, 0x90, 0x85, };

uint8_t demo_x[] = {
0xBC, 0x57, 0x10, 0xAA, 0xB4, 0xE6, 0x15, 0x5B, 0x8B, 0x31, 0xF1, 0xE5, 0x20, 0x60, 0x4F, 0x4A, 
0x93, 0x1A, 0x16, 0xD0, 0xE8, 0x51, 0x21, 0xAC, 0xDB, 0x08, 0xA1, 0x37, 0xDF, 0xA5, 0x56, 0xF9, };

uint8_t demo_y[] = {
0x78, 0x22, 0x97, 0x83, 0x37, 0x2A, 0x95, 0xFE, 0x62, 0x7E, 0x55, 0x6E, 0x99, 0x86, 0x2E, 0x1E, 
0x2C, 0x0A, 0x90, 0x25, 0xB5, 0x08, 0xE9, 0x57, 0x08, 0x9F, 0x6E, 0x3A, 0x56, 0x68, 0xDF, 0x73, };

// Storage for demo ECDSA public key
ucl_type_ecc_u8_affine_point demo_q = {.x = demo_x,.y = demo_y};

// Storage for demo AES128 key.
u8 demo_key[16] ={0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
u8 demo_iv[16] ={0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};

int crypto_aes_test(void)
{
    int i;
    int retval;
    uint8_t aes_plaintext[256];
    uint8_t aes_ciphertext[256];
    uint8_t testmessage[256];

    for (i = 0;i < sizeof(testmessage);i++)
    {
        testmessage[i] = (uint8_t)i;
    }

    // Do AES CBC encryption
    memcpy(aes_plaintext,testmessage,sizeof(testmessage));
    retval = ucl_aes_cbc(aes_ciphertext,aes_plaintext,sizeof(aes_plaintext),demo_key,UCL_AES_KEYLEN_128,demo_iv,UCL_CIPHER_ENCRYPT);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Do AES CBC decryption
    memset(aes_plaintext,0,sizeof(aes_plaintext));
    retval = ucl_aes_cbc(aes_plaintext,aes_ciphertext,sizeof(aes_plaintext),demo_key,UCL_AES_KEYLEN_128,demo_iv,UCL_CIPHER_DECRYPT);
    if (UCL_OK != retval)
    {
        return -1;
    }
    if (memcmp(aes_plaintext,testmessage,sizeof(testmessage)) != 0)
    {
        return -1;
    }

    return 0;
}

// "abc" hash result.
uint8_t sha256_hash_result[] = {
0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23, 
0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD, };

int crypto_sha_test(void)
{
    int retval;
    uint8_t abc[] = { 'a', 'b', 'c' };
    uint8_t hash256[32];

    // Initialize SHA256 working area.
    retval = ucl_sha256_init(&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Hash test string.
    retval = ucl_sha256_core(&sha256_context,abc,3);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Finish hash.
    memset(hash256,0,sizeof(hash256));
    retval = ucl_sha256_finish(hash256,&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }
    if (memcmp(hash256,sha256_hash_result,sizeof(sha256_hash_result)) != 0)
    {
        return -1;
    }

    return 0;
}

int crypto_ecdsa_test(void)
{
    int i;
    int retval;

    // Set ECC Curve
    ucl_type_curve *curve_params = &secp256r1;

    // Setup signature storage
    u8 r[SECP256R1_BYTESIZE];
    u8 s[SECP256R1_BYTESIZE];
    ucl_type_ecdsa_signature signature = { .r = r, .s = s};

    // Storage for SHA context and output.
    ucl_sha256_ctx_t sha256_context;
    uint8_t hash256[32];

    // Set hash function
    int (*hash_function_ptr)(u8*,u8*,u32) = &ucl_sha256;

    // ECDSA function configuration flags
    int configuration;

    // Set up message to sign/verify.
    uint8_t message[] = {0,1,2,3,4,5,6};
    uint8_t message_length = sizeof(message); 

    // Sign message
    // Initialize SHA256 working area.
    retval = ucl_sha256_init(&sha256_context);
    if (UCL_OK != retval)
    {
        while(1);
    }
    // Slow hash one byte at a time.
    for (i = 0;i < message_length;i++)
    {
        retval = ucl_sha256_core(&sha256_context,message+i,1);
        if (UCL_OK != retval)
        {
            while(1);
        }
    }

    // Finish hash.
    memset(hash256,0,sizeof(hash256));
    retval = ucl_sha256_finish(hash256,&sha256_context);
    if (UCL_OK != retval)
    {
        while(1);
    }

    configuration=(SECP256R1<<UCL_CURVE_SHIFT)^(UCL_HASH_INPUT<<UCL_INPUT_SHIFT)^(UCL_SHA256<<UCL_HASH_SHIFT)^(UCL_NO_PRECOMP<<UCL_PRECOMP_TRICK_SHIFT);
    retval = ucl_ecdsa_signature(signature,demo_d,hash_function_ptr,hash256,sizeof(hash256),curve_params,configuration);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Verify signature
    // Initialize SHA256 working area.
    retval = ucl_sha256_init(&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }
    retval = ucl_sha256_core(&sha256_context,message,sizeof(message));
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Finish hash.
    memset(hash256,0,sizeof(hash256));
    retval = ucl_sha256_finish(hash256,&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }

    configuration=(SECP256R1<<UCL_CURVE_SHIFT)^(UCL_HASH_INPUT<<UCL_INPUT_SHIFT)^(UCL_SHA256<<UCL_HASH_SHIFT)^(UCL_NO_PRECOMP<<UCL_PRECOMP_TRICK_SHIFT);
    retval = ucl_ecdsa_verification(demo_q,signature,hash_function_ptr,hash256,sizeof(hash256),curve_params,configuration);
    if (UCL_OK != retval)
    {
        return -1;
    }

    return 0;
}

int crypto_test()
{
    int retval;

    if (kill_selftest) {
      return 0;
    }
      
    retval = crypto_aes_test();
    if (retval < 0)
    {
        return retval;
    }
    retval = crypto_sha_test();
    if (retval < 0)
    {
        return retval;
    }
    retval = crypto_ecdsa_test();
    if (retval < 0)
    {
        return retval;
    }
    
    return 0;
}


int crypto_sign(uint8_t *signatureout)
{
    int retval;

    // Setup signature storage
    u8 r[SECP256R1_BYTESIZE];
    u8 s[SECP256R1_BYTESIZE];
    ucl_type_ecdsa_signature signature = { .r = r, .s = s};

    // Set ECC Curve
    ucl_type_curve *curve_params = &secp256r1;
    // Set configuration for ECDSA call, curve P256R1, use hash input, no precomputed r
    int configuration=(SECP256R1<<UCL_CURVE_SHIFT)^(UCL_HASH_INPUT<<UCL_INPUT_SHIFT)^(UCL_SHA256<<UCL_HASH_SHIFT)^(UCL_NO_PRECOMP<<UCL_PRECOMP_TRICK_SHIFT);
    // Set hash function
    int (*hash_function_ptr)(u8*,u8*,u32) = &ucl_sha256;

    uint8_t hash256[32];

    // Hash last block
    retval = ucl_sha256_finish(hash256,&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Sign hash
    retval = ucl_ecdsa_signature(signature,demo_d,hash_function_ptr,hash256,sizeof(hash256),curve_params,configuration);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Copy signature to regular byte array.
    memcpy(signatureout,signature.r,SECP256R1_BYTESIZE);
    memcpy(signatureout+32,signature.s,SECP256R1_BYTESIZE);

    return 0;
}

int crypto_verify(uint8_t *signaturein)
{
    int retval;

    // Setup signature storage
    u8 r[SECP256R1_BYTESIZE];
    u8 s[SECP256R1_BYTESIZE];
    ucl_type_ecdsa_signature signature = { .r = r, .s = s};

    // Set ECC Curve
    ucl_type_curve *curve_params = &secp256r1;
    // Set configuration for ECDSA call, curve P256R1, use hash input, no precomputed r
    int configuration=(SECP256R1<<UCL_CURVE_SHIFT)^(UCL_HASH_INPUT<<UCL_INPUT_SHIFT)^(UCL_SHA256<<UCL_HASH_SHIFT)^(UCL_NO_PRECOMP<<UCL_PRECOMP_TRICK_SHIFT);
    // Set hash function
    int (*hash_function_ptr)(u8*,u8*,u32) = &ucl_sha256;

    uint8_t hash256[32];

    // Hash last block
    retval = ucl_sha256_finish(hash256,&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }

    // Copy signature from regular byte array to struct.
    memcpy(signature.r,signaturein,SECP256R1_BYTESIZE);
    memcpy(signature.s,signaturein+32,SECP256R1_BYTESIZE);

    // Verify hash
    configuration=(SECP256R1<<UCL_CURVE_SHIFT)^(UCL_HASH_INPUT<<UCL_INPUT_SHIFT)^(UCL_SHA256<<UCL_HASH_SHIFT)^(UCL_NO_PRECOMP<<UCL_PRECOMP_TRICK_SHIFT);
    retval = ucl_ecdsa_verification(demo_q,signature,hash_function_ptr,hash256,sizeof(hash256),curve_params,configuration);
    if (UCL_OK != retval)
    {
        return -1;
    }

    return 0;
}

// Running values for continuous AES CBC processing
uint8_t aes_iv[16];
uint8_t aes_buffer[16];
int aes_buffer_length = 0;

// Finish the last crypto operation and handle unaligned data.
int crypto_decrypt_finish(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout)
{
    int retval;
    uint8_t oneblock[16];
    unsigned int lastblocklength;

    retval = crypto_decrypt_continue(datain,lengthin,dataout,lengthout);
    if (UCL_OK != retval)
    {
        return -1;
    }
    if (aes_buffer_length > 0)
    {
        memset(oneblock,0,sizeof(oneblock));
        memcpy(oneblock,aes_buffer,aes_buffer_length);
        aes_buffer_length = 0;
        retval = crypto_decrypt_continue(oneblock,16,dataout+*lengthout,&lastblocklength);
        if (UCL_OK != retval)
        {
            return -1;
        }
        *lengthout += lastblocklength;
    }

    return 0;
}

int crypto_decrypt_continue(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout)
{
    int retval;
    uint8_t oneblock[16];
    *lengthout = 0;
    int length;


    length = lengthin + aes_buffer_length;
    if (length >= 16)
    {
        while (length >= 16)
        {
            // Handle AES buffer cruft.
            memcpy(oneblock,aes_buffer,aes_buffer_length);
            memcpy(oneblock+aes_buffer_length,datain,16-aes_buffer_length);
            datain += 16-aes_buffer_length;
            lengthin -= 16-aes_buffer_length;
            // Hash intermediate data
            retval = ucl_sha256_core(&sha256_context,oneblock,16);
            if (UCL_OK != retval)
            {
                return -1;
            }
            // Decrypt even 16 byte blocks of data
            retval = ucl_aes_cbc(dataout,oneblock,16,demo_key,UCL_AES_KEYLEN_128,aes_iv,UCL_CIPHER_DECRYPT);
            // Save IV for next time
            memcpy(aes_iv,oneblock,16);
            // Increment pointer and length of data output
            dataout += 16;
            *lengthout += 16;
            // Decrement available data length
            length -= 16;
            // AES buffer is empty.
            aes_buffer_length = 0;
        }
        // Save any unaligned data for next time
        memcpy(aes_buffer,datain,length);
        aes_buffer_length = length;
    }
    else
    {
        // Save any unaligned data for next time
        memcpy(aes_buffer+aes_buffer_length,datain,lengthin);
        aes_buffer_length += lengthin;
    }

    return 0;
}

int crypto_decrypt_start(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout)
{
    int retval;

    // Initialize AES partial block buffer.
    aes_buffer_length = 0;
    memset(aes_buffer,0,sizeof(aes_buffer));
    // Initialize IV
    memcpy(aes_iv,demo_iv,sizeof(aes_iv));

    // Initialize SHA256 working area.
    retval = ucl_sha256_init(&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }

    return crypto_decrypt_continue(datain, lengthin, dataout, lengthout);
}

// Finish the last crypto operation and handle unaligned data.
int crypto_encrypt_finish(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout)
{
    int retval;
    uint8_t oneblock[16];
    unsigned int lastblocklength;

    retval = crypto_encrypt_continue(datain,lengthin,dataout,lengthout);
    if (UCL_OK != retval)
    {
        return -1;
    }
    if (aes_buffer_length > 0)
    {
        memset(oneblock,0,sizeof(oneblock));
        memcpy(oneblock,aes_buffer,aes_buffer_length);
        aes_buffer_length = 0;
        retval = crypto_encrypt_continue(oneblock,16,dataout+*lengthout,&lastblocklength);
        if (UCL_OK != retval)
        {
            return -1;
        }
        *lengthout += lastblocklength;
    }

    return 0;
}

int crypto_encrypt_continue(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout)
{
    int retval;
    uint8_t oneblock[16];
    *lengthout = 0;
    int length;


    length = lengthin + aes_buffer_length;
    if (length >= 16)
    {
        while (length >= 16)
        {
            // Handle AES buffer cruft.
            memcpy(oneblock,aes_buffer,aes_buffer_length);
            memcpy(oneblock+aes_buffer_length,datain,16-aes_buffer_length);
            datain += 16-aes_buffer_length;
            lengthin -= 16-aes_buffer_length;
            // Encrypt even 16 byte blocks of data
            retval = ucl_aes_cbc(dataout,oneblock,16,demo_key,UCL_AES_KEYLEN_128,aes_iv,UCL_CIPHER_ENCRYPT);
            if (UCL_OK != retval)
            {
                return -1;
            }
            // Hash intermediate data
            retval = ucl_sha256_core(&sha256_context,dataout,16);
            if (UCL_OK != retval)
            {
                return -1;
            }
            // Save IV for next time
            memcpy(aes_iv,dataout,16);
            // Increment pointer and length of data output
            dataout += 16;
            *lengthout += 16;
            // Decrement available data length
            length -= 16;
            // AES buffer is empty.
            aes_buffer_length = 0;
        }
        // Save any unaligned data for next time
        memcpy(aes_buffer,datain,length);
        aes_buffer_length = length;
    }
    else
    {
        // Save any unaligned data for next time
        memcpy(aes_buffer+aes_buffer_length,datain,lengthin);
        aes_buffer_length += lengthin;
    }

    return 0;
}

int crypto_encrypt_start(uint8_t *datain, int lengthin, uint8_t *dataout, unsigned int *lengthout)
{
    int retval;

    // Initialize AES partial block buffer.
    aes_buffer_length = 0;
    memset(aes_buffer,0,sizeof(aes_buffer));
    // Initialize IV
    memcpy(aes_iv,demo_iv,sizeof(aes_iv));

    // Initialize SHA256 working area.
    retval = ucl_sha256_init(&sha256_context);
    if (UCL_OK != retval)
    {
        return -1;
    }

    return crypto_encrypt_continue(datain, lengthin, dataout, lengthout);
}

/* NOTE: Only call crypto_init() once. */
int crypto_init(void)
{
    int retval;

    // NOTE: ucl_init() cannot be called more than once, or it will return -1. */
    // Initialize libucl (buffer size is in 32-bit words)
    //retval = ucl_init((u32*)ucl_work_buffer, UCL_WORK_SIZE_WORDS);
    retval = ucl_init();
    if (UCL_OK != retval)
    {
        return -1;
    }

    return 0;
}

void hexdump(uint8_t *data, int length)
{
    int i;

    for (i = 0;i < length;i++)
    {
        if (((i & 0x0F) == 0) && (i != 0))
        {
            printf("\n");
        }
        printf("%02X ",data[i]);
    }

    printf("\n");
}

#if 0
int crypto_example()
{
    int retval;
    uint8_t out[1024];
    uint8_t signature[64];
    int outlen;
    uint8_t test_plaintext[128];
    uint8_t test_ciphertext[128];
    uint8_t iv[16];
    int i;

    // Variables for testing unaligned data input/output
    int startbytes;
    int continuebytes1;
    int continuebytes2;
    int finishbytes;
    uint8_t *in_ptr;
    uint8_t *out_ptr;
    int out_totallength;

    // Create some plaintext.
    for (i = 0;i < sizeof(test_plaintext);i++)
    {
        test_plaintext[i] = i;
    }
    // Encrypt the plaintext.
    memcpy(iv,demo_iv,16);
    retval = ucl_aes_cbc(test_ciphertext,test_plaintext,sizeof(test_plaintext),demo_key,UCL_AES_KEYLEN_128,iv,UCL_CIPHER_ENCRYPT);
    if (UCL_OK != retval)
    {
        while(1);
    }

    startbytes = 17;
    continuebytes1 = 17;
    continuebytes2 = 17;
    finishbytes = 77;
    out_totallength = 0;
    out_ptr = out;
    in_ptr = test_plaintext;
    retval = crypto_encrypt_start(in_ptr,startbytes,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += startbytes;
    out_totallength += outlen;
    out_ptr += outlen;

    retval = crypto_encrypt_continue(in_ptr,continuebytes1,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += continuebytes1;
    out_totallength += outlen;
    out_ptr += outlen;

    retval = crypto_encrypt_continue(in_ptr,continuebytes2,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += continuebytes2;
    out_totallength += outlen;
    out_ptr += outlen;

    retval = crypto_encrypt_finish(in_ptr,finishbytes,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += finishbytes;
    out_totallength += outlen;
    out_ptr += outlen;

    // Check output.
    if ((out_totallength != sizeof(test_plaintext)) ||
       (memcmp(out,test_ciphertext,sizeof(test_ciphertext))))
    { 
        hexdump(out,sizeof(test_ciphertext));
        hexdump(out,sizeof(out));
        while(1);
    }
    printf("Encrypt test pass.\n");

    retval = crypto_sign(signature);
    if (UCL_OK != retval)
    {
        printf("Sig sign failed\n");
        while(1);
    }

    // Decryption test.
    startbytes = 17;
    continuebytes1 = 17;
    continuebytes2 = 17;
    finishbytes = 77;
    out_totallength = 0;
    out_ptr = out;
    in_ptr = test_ciphertext;

    retval = crypto_decrypt_start(in_ptr,startbytes,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += startbytes;
    out_totallength += outlen;
    out_ptr += outlen;

    retval = crypto_decrypt_continue(in_ptr,continuebytes1,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += continuebytes1;
    out_totallength += outlen;
    out_ptr += outlen;

    retval = crypto_decrypt_continue(in_ptr,continuebytes2,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += continuebytes2;
    out_totallength += outlen;
    out_ptr += outlen;

    retval = crypto_decrypt_finish(in_ptr,finishbytes,out_ptr,&outlen);
    if (UCL_OK != retval)
    {
        while(1);
    }
    in_ptr += finishbytes;
    out_totallength += outlen;
    out_ptr += outlen;

    // Check output.
    if ((out_totallength != sizeof(test_plaintext)) ||
       (memcmp(out,test_plaintext,sizeof(test_ciphertext))))
    { 
        hexdump(out,sizeof(test_plaintext));
        hexdump(out,sizeof(out));
        while(1);
    }
    printf("Decrypt test pass.\n");

    retval = crypto_verify(signature);
    if (UCL_OK != retval)
    {
        printf("Sig verify failed\n");
        while(1);
    }

    return 0;
}
#endif
