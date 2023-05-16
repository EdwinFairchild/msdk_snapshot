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

#ifndef __READ_FILE_H__
#define __READ_FILE_H__

#include "maxim_c_utils.h"

#define	ERR_RF_BASE				0x11000000
#define ERR_RF_ERROR			((ERR_RF_BASE) + 0x000001 )


#ifdef __cplusplus
extern "C"
{
#endif

/**
 *
 */
	int test_hex (char c1, char c2);

/**
 *
 */
int read_hex (char *hexfilename, uint8_t * data, size_t * data_len, uint8_t * addr, int * hex_extended_address);

/**
 *
 */
int read_s19 (char *s19filename, int *ptr_address_offset, uint8_t * data, size_t * data_len, uint8_t * addr);

/**
 *
 */
int get_start_addr_and_length_s19 (char *s19filename, int *end_addr_ret, int address_offset);

/**
 *
 */
int read_s20 (char *s20filename, int *ptr_address_offset, uint8_t * data, size_t * data_len, uint8_t * addr);

/**
 *
 */
int get_start_addr_and_length_s20 (char *s20filename, int *end_addr_ret, int address_offset);

/**
 *
 */
	int read_file_ascii_data (FILE * p_pFile, const unsigned int p_iHexDataLength, uint8_t * p_pucHexDataBuf,
				  unsigned int *p_piHexDataBufLen);

/**
 *
 */
	int extension (char *ext, char *name);

/**
 *
 */
	int determine_file_size (unsigned int *filesize, char *filename);

#ifdef __cplusplus
}
#endif

#endif				/* __READ_FILE_H__ */
