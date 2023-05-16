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

#ifndef __RSA_H__
#define __RSA_H__

#include <ucl/ucl_config.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_sys.h>
#include <ucl/ucl_aes.h>
#include <ucl/ucl_data_conv.h>
#include <ucl/ucl_rsa.h>



#ifdef __cplusplus
extern "C"
{
#endif


/**
 *
 */
	int rsa_sign (uint8_t * input, unsigned int input_size, uint8_t * signature, 
				ucl_rsa_public_key_t keyPu, ucl_rsa_private_key_t keyPr);

/**
 *
 */
	int read_file_signed_rsa_publickey (u8 * puk, int *puksize, u8 * pukexp, int expsize, u8 * signature, int *sigsize,
					    char *filename);

/**
 *
 */
	int read_file_rsa (u8 * puk, unsigned int *puksize, u8 * pukexp, u8 * privexp, unsigned int expsize, char *filename);

#ifdef _MAXIM_HSM
/**
 *
 */
int load_HSM_rsa_key (type_config_struct * config, ucl_rsa_public_key_t * keyPu, char *keyname);
#endif				/* _MAXIM_HSM */

#ifdef __cplusplus
}
#endif

#endif				/* __RSA_H__ */
