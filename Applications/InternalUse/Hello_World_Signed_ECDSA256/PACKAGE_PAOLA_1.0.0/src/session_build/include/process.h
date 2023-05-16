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

#ifndef __PROCESS_H__
#define __PROCESS_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *
 */
	int process_string (char *output, char *reference, char *line, int fgets_correction, int *p_piFound);

/**
 *
 */
	int process_hexvalue (int *value_len, u8 * value, char *reference, char *line, int fgets_correction);

/**
 *
 */
	int process_value (u8 * value, char *reference, char *line, int limit, int fgets_correction);

/**
 *
 */
	int process_intvalue (int *value, char *reference, char *line, int limit, int fgets_correction);

/**
 *
 */
	int process_hexint (int *value, char *reference, char *line, int fgets_correction);

#ifdef __cplusplus
}
#endif

#endif				/* __PROCESS_H__ */
