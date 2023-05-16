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

#include "scp_definitions.h"
#include "session_build.h"
#include "scp_utils.h"
#include "read_file.h"
#include "ecdsa.h"

void maxq1852_add_trid ()
{
	payload[ipayload++] = trid >> 24;
	payload[ipayload++] = (trid >> 16) & 255;
	payload[ipayload++] = (trid >> 8) & 255;
	payload[ipayload++] = trid & 255;
}

void maxq1852_add_seq ()
{
	payload[ipayload++] = seq & 255;
	payload[ipayload++] = seq >> 8;
	seq = (seq + 1) & 0xffff;
}

void maxq1852_add_seq_noincr ()
{
	payload[ipayload++] = seq & 255;
	payload[ipayload++] = seq >> 8;
	/*  seq=(seq+1)&0xffff; */
}

void maxq1852_response_synchro (void)
{
	iframe = 0;
}

void maxq1852_generic_response (void)
{
	maxq1852_response_synchro ();
	ipayload = 0;
	/*   payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = 'A';
	payload[ipayload++] = 'G';
	payload[ipayload++] = 'P';
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = MAXQ1852_SCPROMPT;
}

void maxq1852_synchro (void)
{
	iframe = 0;
	payload[ipayload++] = MAXQ1852_SCDESIGNATOR;
	/* reserved 2 bytes for length, computed later */
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
}

void erase_all_flash_areas_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "erase_all_flash_areas_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_ERASE_ALL_FLASH_AREAS]);
	packet_send ();
}

int erase_all_flash_areas (void)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*   payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_ERASE_ALL_FLASH_AREAS;

	maxq1852_add_trid ();
	maxq1852_add_seq ();

	/* now, we can update the 2 length bytes
	   payload[MAXQ1852_SC_LEN_BYTE1]=ipayload&255;
	   payload[MAXQ1852_SC_LEN_BYTE2]=ipayload>>8;
	 */
	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	printf ("len=%02x\n", length);
	ecdsa_sign_payload ();
	sprintf (name_file, "erase_all_flash_areas");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_ERASE_ALL_FLASH_AREAS]);
	packet_send ();
	return (EXIT_SUCCESS);
}

int load_customer_key_payload (char *pub_x, char *pub_y)
{
	unsigned int i;
	unsigned int one_byte;
	u8 xq[ECDSA_MODULUS_LEN];
	u8 yq[ECDSA_MODULUS_LEN];

	for (i = 0; i < config_struct.ecdsa_len; i++)
	{
		if (0 == sscanf (&(pub_x[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: pub_x parameter <%s> incorrectly formatted as a number\n", pub_x);
			return (EXIT_FAILURE);
		}
		/*      payload[ipayload++]=one_byte; */
		/* 3.5.0 */
		xq[i] = one_byte;
	}
	for (i = 0; i < config_struct.ecdsa_len; i++)
	{
		if (0 == sscanf (&(pub_y[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: pub_y parameter <%s> incorrectly formatted as a number\n", pub_y);
			return (EXIT_FAILURE);
		}
		/*      payload[ipayload++]=one_byte; */
		yq[i] = one_byte;
	}
	/* 3.5.0 */
	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		payload[ipayload++] = xq[ECDSA_MODULUS_LEN - 1 - i];
	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		payload[ipayload++] = yq[ECDSA_MODULUS_LEN - 1 - i];
	return (EXIT_SUCCESS);
}

int load_customer_key (char *pub_x, char *pub_y)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_LOAD_CUSTOMER_KEY;

	load_customer_key_payload (pub_x, pub_y);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	/* now, we can update the 2 length bytes */
	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	/* payload[MAXQ1852_SC_LEN_BYTE1]=ipayload&255; */
	/* payload[MAXQ1852_SC_LEN_BYTE2]=ipayload>>8; */
	ecdsa_sign_payload ();
	sprintf (name_file, "load_customer_key");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_LOAD_CUSTOMER_KEY]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void verify_customer_key_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "verify_customer_key_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_CUSTOMER_KEY]);
	packet_send ();
}

/* 3.5.0 */
int verify_customer_key_payload (char *pub_x, char *pub_y)
{
	unsigned int i;
	unsigned int one_byte;
	u8 xq[ECDSA_MODULUS_LEN];
	u8 yq[ECDSA_MODULUS_LEN];

	for (i = 0; i < config_struct.ecdsa_len; i++)
	{
		if (0 == sscanf (&(pub_x[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: pub_x parameter <%s> incorrectly formatted as a number\n", pub_x);
			return (EXIT_FAILURE);
		}
		/*      payload[ipayload++]=one_byte; */
		/* 3.5.0 */
		xq[i] = one_byte;
	}
	for (i = 0; i < config_struct.ecdsa_len; i++)
	{
		if (0 == sscanf (&(pub_y[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: pub_y parameter <%s> incorrectly formatted as a number\n", pub_y);
			return (EXIT_FAILURE);
		}
		/*      payload[ipayload++]=one_byte; */
		yq[i] = one_byte;
	}
	/* 3.5.0 */
	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		payload[ipayload++] = xq[ECDSA_MODULUS_LEN - 1 - i];
	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		payload[ipayload++] = yq[ECDSA_MODULUS_LEN - 1 - i];
	return (EXIT_SUCCESS);
}

int verify_customer_key (char *pub_x, char *pub_y)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_VERIFY_CUSTOMER_KEY;

	verify_customer_key_payload (pub_x, pub_y);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	/* now, we can update the 2 length bytes */
	/*  payload[MAXQ1852_SC_LEN_BYTE1]=ipayload&255; */
	/*  payload[MAXQ1852_SC_LEN_BYTE2]=ipayload>>8;  */
	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "verify_customer_key");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_CUSTOMER_KEY]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void activate_customer_key_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "activate_customer_key_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_ACTIVATE_CUSTOMER_KEY]);
	packet_send ();
}

int activate_customer_key (void)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_ACTIVATE_CUSTOMER_KEY;

	maxq1852_add_trid ();
	maxq1852_add_seq ();

	/* now, we can update the 2 length bytes */
	/*  payload[MAXQ1852_SC_LEN_BYTE1]=ipayload&255; */
	/*  payload[MAXQ1852_SC_LEN_BYTE2]=ipayload>>8; */
	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "activate_customer_key");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_ACTIVATE_CUSTOMER_KEY]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void generate_application_startup_signature_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "generate_application_startup_signature_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_GENERATE_APPLICATION_STARTUP_SIGNATURE]);
	packet_send ();
}

int generate_application_startup_signature (void)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_GENERATE_APPLICATION_STARTUP_SIGNATURE;

	maxq1852_add_trid ();
	maxq1852_add_seq ();

	/* now, we can update the 2 length bytes */
	/*  payload[MAXQ1852_SC_LEN_BYTE1]=ipayload&255; */
	/* payload[MAXQ1852_SC_LEN_BYTE2]=ipayload>>8; */
	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "generate_application_startup_signature");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_GENERATE_APPLICATION_STARTUP_SIGNATURE]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void verify_application_startup_signature_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "verify_application_startup_signature_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_APPLICATION_STARTUP_SIGNATURE]);
	packet_send ();
}

int verify_application_startup_signature (void)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_VERIFY_APPLICATION_STARTUP_SIGNATURE;

	maxq1852_add_trid ();
	maxq1852_add_seq ();

	/* now, we can update the 2 length bytes */
	/*  payload[MAXQ1852_SC_LEN_BYTE1]=ipayload&255; */
	/*  payload[MAXQ1852_SC_LEN_BYTE2]=ipayload>>8; */
	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "verify_application_startup_signature");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_APPLICATION_STARTUP_SIGNATURE]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void write_register_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "write_register_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_WRITE_REGISTER]);
	packet_send ();
}

int write_register_payload (char *reg, char *value)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(reg[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: reg parameter <%s> incorrectly formatted as a number\n", reg);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(value[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: value parameter <%s> incorrectly formatted as a number\n", value);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

int write_register (char *reg, char *value)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*   payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_WRITE_REGISTER;

	write_register_payload (reg, value);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "write_register");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_WRITE_REGISTER]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void read_register_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "read_register_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_READ_REGISTER]);
	packet_send ();
}

int read_register_payload (char *reg)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(reg[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: reg parameter <%s> incorrectly formatted as a number\n", reg);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

int read_register (char *reg)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();

	payload[ipayload++] = MAXQ1852_READ_REGISTER;

	read_register_payload (reg);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "read_register");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_READ_REGISTER]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void load_code_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "load_code_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_LOAD_CODE]);
	packet_send ();
}

int load_code_payload (char *addr, char *code)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(addr[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: addr parameter <%s> incorrectly formatted as a number\n", addr);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	for (i = 0; i < (int) strlen (code); i += 2)
	{
		if (0 == sscanf (&(code[i]), "%02x", &one_byte))
		{
			printf ("ERROR: code parameter <%s> incorrectly formatted as a number\n", code);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

int load_code (char *addr, char *code)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();

	payload[ipayload++] = MAXQ1852_LOAD_CODE;

	load_code_payload (addr, code);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "load_code");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_LOAD_CODE]);
	packet_send ();
	return (EXIT_SUCCESS);
}

int load_file (char *hexfilename, uint8_t * data)
{
	int i, k;
	int last_index;
	u8 *dataloc;
	char schunk_addr[10];
	char schunk[20000];
	char schunk_tmp[20000];
	int allff;
	u8 ad1, ad2, ad3, ad4;
	u32 ad;
	int resu;
	dataloc = (u8 *) malloc (sizeof (u8) * 1024 * 1024);
	if (NULL == data)
	{
		printf ("ERROR: <data> allocation is not possible (%dMB requested)\n", config_struct.flash_mb);
		return (EXIT_FAILURE);
	}
	if (EXIT_SUCCESS == extension (".hex", hexfilename))
	{
		if (EXIT_SUCCESS != read_hex (hexfilename, data))
			return (EXIT_FAILURE);
	}
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .hex)\n", hexfilename);
		return (EXIT_FAILURE);
	}
	for (i = 0; i < 1024 * 1024; i++)
		dataloc[i] = 0xff;
	for (i = 0; i < data_len_g; i++)
	{
		if (addr_g[i] > 1024 * 1024)
		{
			printf ("ERROR: addr[%d]=%x is too large\n", i, addr_g[i]);
			exit (1);
		}
		if (dataloc[addr_g[i]] != 0xff)
			printf ("ERROR: data already allocated in %x: %02x-%02x\n", addr_g[i], dataloc[addr_g[i]], data[i]);
		else
			dataloc[addr_g[i]] = data[i];
	}
	/* #4393 */
	for (last_index = 0, i = 0; i < data_len_g; i++)
		if (last_index < addr_g[i])
			last_index = addr_g[i];
	/*  last_index=addr[data_len-1]; */
	for (i = 0; i < last_index; i += chunk_size)
	{
		/* 3.7.14 */
		/* extended-address is now already included in the address */
		/*      ad=(hex_extended_address<<16)^(i); */
		ad = i;
		ad1 = ad >> 24;
		ad2 = (ad >> 16) & 255;
		ad3 = (ad >> 8) & 255;
		ad4 = (ad & 255);
		sprintf (schunk_addr, "%08x", (ad4 << 24) ^ (ad3 << 16) ^ (ad2 << 8) ^ (ad1));

		/* sprintf(schunk_tmp,""); */
		schunk_tmp[0] = '\0';
		for (allff = 1, k = 0; k < chunk_size; k++)
		{
			if (0xff != dataloc[i + k])
			{
				allff = 0;
			}
			sprintf (schunk, "%s%02x", schunk_tmp, dataloc[i + k]);
			sprintf (schunk_tmp, "%s", schunk);
		}
		/* load-code only if not ff    */
		if (0 == allff)
		{
			resu = load_code (schunk_addr, schunk);
			if (EXIT_SUCCESS != resu)
			{
				printf ("ERROR in write_mem\n");
				free (dataloc);
				return (EXIT_FAILURE);
			}
			usip ();
			load_code_response ();
			host ();
		}
	}
	free (dataloc);
	return (EXIT_SUCCESS);
}

void maxq1852_agf_response (void)
{
	maxq1852_response_synchro ();
	ipayload = 0;
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = 'A';
	payload[ipayload++] = 'G';
	payload[ipayload++] = 'F';
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = MAXQ1852_SCPROMPT;
}

void load_customer_key_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "load_customer_key_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_LOAD_CUSTOMER_KEY]);
	packet_send ();
}

void verify_code_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "verify_code_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_CODE]);
	packet_send ();
}

int verify_code_payload (char *addr, char *code)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(addr[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: addr parameter <%s> incorrectly formatted as a number\n", addr);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	for (i = 0; i < (int) strlen (code); i += 2)
	{
		if (0 == sscanf (&(code[i]), "%02x", &one_byte))
		{
			printf ("ERROR: code parameter <%s> incorrectly formatted as a number\n", code);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

int verify_code (char *addr, char *code)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_VERIFY_CODE;

	verify_code_payload (addr, code);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "verify_code");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_CODE]);
	packet_send ();
	return (EXIT_SUCCESS);
}

int verify_code_file (char *hexfilename, uint8_t * data)
{
	int i, k;
	int last_index;
	u8 *dataloc;
	char schunk_addr[10];
	char schunk[20000];
	char schunk_tmp[20000];
	int allff;
	u8 ad1, ad2, ad3, ad4;
	u32 ad;
	int resu;

	dataloc = (u8 *) malloc (sizeof (u8) * 1024 * 1024);
	if (NULL == data)
	{
		printf ("ERROR: <data> allocation is not possible (%dMB requested)\n", config_struct.flash_mb);
		return (EXIT_FAILURE);
	}
	if (EXIT_SUCCESS == extension (".hex", hexfilename))
	{
		if (EXIT_SUCCESS != read_hex (hexfilename, data))
			return (EXIT_FAILURE);
	}
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .hex)\n", hexfilename);
		return (EXIT_FAILURE);
	}
	for (i = 0; i < 1024 * 1024; i++)
		dataloc[i] = 0xff;
	for (i = 0; i < data_len_g; i++)
		dataloc[addr_g[i]] = data[i];
	last_index = addr_g[data_len_g - 1];
	for (i = 0; i < last_index; i += chunk_size)
	{
		/*3.7.14
		   extended-address is now already included in the address
		   ad=(hex_extended_address<<16)^(i);
		 */
		ad = i;
		ad1 = ad >> 24;
		ad2 = (ad >> 16) & 255;
		ad3 = (ad >> 8) & 255;
		ad4 = (ad & 255);
		sprintf (schunk_addr, "%08x", (ad4 << 24) ^ (ad3 << 16) ^ (ad2 << 8) ^ (ad1));

		/* sprintf(schunk_tmp,""); */
		schunk_tmp[0] = '\0';
		for (allff = 1, k = 0; k < chunk_size; k++)
		{
			if (0xff != dataloc[i + k])
			{
				allff = 0;
			}
			sprintf (schunk, "%s%02x", schunk_tmp, dataloc[i + k]);
			sprintf (schunk_tmp, "%s", schunk);
		}
		/* load-code only if not ff   */
		if (0 == allff)
		{
			resu = verify_code (schunk_addr, schunk);
			if (EXIT_SUCCESS != resu)
			{
				printf ("ERROR in write_mem\n");
				free (dataloc);
				return (EXIT_FAILURE);
			}
			usip ();
			verify_code_response ();
			host ();
		}
	}
	free (dataloc);
	return (EXIT_SUCCESS);
}

void load_data_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "load_data_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_LOAD_DATA]);
	packet_send ();
}

int load_data_payload (char *addr, char *data)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(addr[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: addr parameter <%s> incorrectly formatted as a number\n", addr);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	for (i = 0; i < (int) strlen (data); i += 2)
	{
		if (0 == sscanf (&(data[i]), "%02x", &one_byte))
		{
			printf ("ERROR: data parameter <%s> incorrectly formatted as a number\n", data);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

int load_data (char *addr, char *data)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_LOAD_DATA;

	load_data_payload (addr, data);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "load_data");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_LOAD_DATA]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void verify_maxq1852_data_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "verify_data_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_DATA]);
	packet_send ();
}

int verify_maxq1852_data_payload (char *addr, char *data)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(addr[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: addr parameter <%s> incorrectly formatted as a number\n", addr);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	for (i = 0; i < (int) strlen (data); i += 2)
	{
		if (0 == sscanf (&(data[i]), "%02x", &one_byte))
		{
			printf ("ERROR: data parameter <%s> incorrectly formatted as a number\n", data);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

int verify_maxq1852_data (char *addr, char *data)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();
	/*  payload[ipayload++]=MAXQ1852_SCOFFSET; */
	payload[ipayload++] = MAXQ1852_VERIFY_DATA;

	verify_maxq1852_data_payload (addr, data);
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "verify_data");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_DATA]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void erase_code_flash_area_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "erase_code_flash_area_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_ERASE_CODE_FLASH_AREA]);
	packet_send ();
}

int erase_code_flash_area (void)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();

	payload[ipayload++] = MAXQ1852_ERASE_CODE_FLASH_AREA;

	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "erase_code_flash_area");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_ERASE_CODE_FLASH_AREA]);
	packet_send ();
	return (EXIT_SUCCESS);
}

void engage_pllo_response (void)
{
	maxq1852_generic_response ();
	sprintf (name_file, "engage_pllo_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_MAXQ1852_ENGAGE_PLLO]);
	packet_send ();
}

void engage_pllo_payload (void)
{

	payload[ipayload++] = 'P';
	payload[ipayload++] = 'L';
	payload[ipayload++] = 'L';
	payload[ipayload++] = 'O';
}

int engage_pllo (void)
{
	int length;
	ipayload = 0;
	maxq1852_synchro ();

	payload[ipayload++] = MAXQ1852_ENGAGE_PLLO;

	engage_pllo_payload ();
	maxq1852_add_trid ();
	maxq1852_add_seq ();

	length = ipayload + ECDSA_SIGNATURE_LEN - 3;
	payload[MAXQ1852_SC_LEN_BYTE1] = length & 255;
	payload[MAXQ1852_SC_LEN_BYTE2] = length >> 8;
	ecdsa_sign_payload ();
	sprintf (name_file, "engage_pllo");
	add_payload ();
	sprintf (message, "%s", idf_scp_cmd[COMMAND_MAXQ1852_ENGAGE_PLLO]);
	packet_send ();
	return (EXIT_SUCCESS);
}

/* this function processes the commands listed in the script file "script_file"
  the ECDSA mode is managed out of the commands list
  these commands are closed to the SCP commands, but some include some abstraction
  e.g. the write-file, which writes the whole file, instead of write-data, which writes only one line
  the session opening is out of the script commands process 
*/
int process_script_maxq1852_ecdsa (void)
{

	char line[MAX_STRING];
	int command;
	FILE *fpscript;
	fpscript = fopen (script_file, "r");
	if (NULL == fpscript)
	{
		printf ("ERROR: impossible to open <%s>\n", script_file);
		return (EXIT_FAILURE);
	}
	/* initialize seq & ch id */
	seq = 1;
	host ();
	while (fgets (line, MAX_STRING, fpscript) != NULL)
	{
		if (TRUE == verbose)
			printf ("<%s>", line);
		/* if 1st char is a #, then considered as a comment and skip to next line */
		if ('#' == line[0])
			continue;
		/* look for the command */
		command = process_command (line);
		if (TRUE == verbose)
			printf ("command=%s\n", idf_scp_cmd[command]);
		if (COMMAND_UNKNOWN == command)
		{
			printf ("ERROR: the command <%s> is unknown or not supported; check the script file <%s>\n", line,
				script_file);
			return (EXIT_FAILURE);
		}
		switch (command)
		{
		case COMMAND_HELP:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != help ())
				{
					printf ("ERROR: help\n");
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR for help command\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_ENGAGE_PLLO:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != engage_pllo ())
				{
					printf ("ERROR: engage-pllo\n");
					return (EXIT_FAILURE);
				}
				usip ();
				engage_pllo_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for engage-pllo command: engage-pllo\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_ERASE_CODE_FLASH_AREA:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != erase_code_flash_area ())
				{
					printf ("ERROR: erase-code-flash-area\n");
					return (EXIT_FAILURE);
				}
				usip ();
				erase_code_flash_area_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for erase-code-flash-area command: erase-code-flash-area\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_ERASE_ALL_FLASH_AREAS:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != erase_all_flash_areas ())
				{
					printf ("ERROR: erase-all-flash-areas\n");
					return (EXIT_FAILURE);
				}
				usip ();
				erase_all_flash_areas_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for erase-all-flash-areas command: erase-all-flash-areas\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_ACTIVATE_CUSTOMER_KEY:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != activate_customer_key ())
				{
					printf ("ERROR: activate-customer-key\n");
					return (EXIT_FAILURE);
				}
				usip ();
				activate_customer_key_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for activate-customer-key command: activate-customer-key\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_GENERATE_APPLICATION_STARTUP_SIGNATURE:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != generate_application_startup_signature ())
				{
					printf ("ERROR: generate-application-startup-signature\n");
					return (EXIT_FAILURE);
				}
				usip ();
				generate_application_startup_signature_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for generate-application-startup-signature command: generate-application-startup-signature\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_VERIFY_APPLICATION_STARTUP_SIGNATURE:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != verify_application_startup_signature ())
				{
					printf ("ERROR: verify-application-startup-signature\n");
					return (EXIT_FAILURE);
				}
				usip ();
				verify_application_startup_signature_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for verify-application-startup-signature command: verify-application-startup-signature\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_LOAD_CUSTOMER_KEY:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != load_customer_key (params[0], params[1]))
				{
					printf ("ERROR: load-customer-key\n");
					return (EXIT_FAILURE);
				}
				usip ();
				load_customer_key_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for load-customer-key command: load-customer-key <pub-x> <pub-y>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_VERIFY_CUSTOMER_KEY:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != verify_customer_key (params[0], params[1]))
				{
					printf ("ERROR: verify-customer-key\n");
					return (EXIT_FAILURE);
				}
				usip ();
				verify_customer_key_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for verify-customer-key command: verify-customer-key <pub-x> <pub-y>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_WRITE_REGISTER:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != write_register (params[0], params[1]))
				{
					printf ("ERROR: write-register\n");
					return (EXIT_FAILURE);
				}
				usip ();
				write_register_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for write-register: write-register <register> <value>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_READ_REGISTER:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != read_register (params[0]))
				{
					printf ("ERROR: read-register\n");
					return (EXIT_FAILURE);
				}
				usip ();
				read_register_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for write-register: read-register <register>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* write-only-file is an abstraction for write-data, used in scp and scp-flora */
		case COMMAND_MAXQ1852_LOAD_FILE:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != load_file (params[0], data_g))
				{
					printf ("ERROR: load-file\n");
					return (EXIT_FAILURE);
				}
			}
			else if (2 == nb_params)
			{
				if (EXIT_SUCCESS != load_file (params[0], data_g))
				{
					printf ("ERROR: load-file\n");
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-ONLY command: write-file-only <s19file> <address-offset:optional>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_LOAD_CODE:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != load_code (params[0], params[1]))
				{
					printf ("ERROR: load-code\n");
					return (EXIT_FAILURE);
				}
				usip ();
				load_code_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for load-code: load-code <address> <code>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_VERIFY_FILE:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != verify_code_file (params[0], data_g))
				{
					printf ("ERROR: verify-file\n");
					return (EXIT_FAILURE);
				}
			}
			else if (2 == nb_params)
			{
				if (EXIT_SUCCESS != verify_code_file (params[0], data_g))
				{
					printf ("ERROR: verify-file\n");
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-ONLY command: verify-code-file-only <s19file> <address-offset:optional>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_VERIFY_CODE:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != verify_code (params[0], params[1]))
				{
					printf ("ERROR: verify-code\n");
					return (EXIT_FAILURE);
				}
				usip ();
				verify_code_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for verify-code: verify-code <address> <code>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_LOAD_DATA:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != load_data (params[0], params[1]))
				{
					printf ("ERROR: load-data\n");
					return (EXIT_FAILURE);
				}
				usip ();
				load_data_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for load-data: load-data <address> <data>\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_MAXQ1852_VERIFY_DATA:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != verify_maxq1852_data (params[0], params[1]))
				{
					printf ("ERROR: verify-data\n");
					return (EXIT_FAILURE);
				}
				usip ();
				verify_maxq1852_data_response ();
				host ();
			}
			else
			{
				printf ("ERROR: incorrect format for verify-data: verify-data <address> <code>\n");
				return (EXIT_FAILURE);
			}
			break;
		default:
			printf ("ERROR: the command <%s> is not supported\n", line);
			return (EXIT_FAILURE);
		}
	}
	(void) fclose (fpscript);
	return (EXIT_SUCCESS);
}
