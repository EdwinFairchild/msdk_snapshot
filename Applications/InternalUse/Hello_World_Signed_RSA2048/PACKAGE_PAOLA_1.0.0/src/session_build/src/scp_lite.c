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

void scp_lite_add_synchro (void)
{
	iframe = 0;
	ipayload = 0;
	payload[ipayload++] = SCP_LITE_SYNC1;
	payload[ipayload++] = SCP_LITE_SYNC2;
	/* reserved 2 bytes for length, computed later */
	/* payload[ipayload++]=0x00; */
	/* payload[ipayload++]=0x00; */
}

void scp_lite_add_generic_response (void)
{
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
}

void scp_lite_load_mem_response (void)
{
	scp_lite_add_synchro ();
	scp_lite_add_generic_response ();
	sprintf (name_file, "scp_lite_load_ram_response");
	add_payload ();
	sprintf (message, "%s-response", idf_scp_cmd[COMMAND_SCP_LITE_LOAD_RAM]);
	packet_send ();
}

int scp_lite_add_address (char *address)
{
	int i;
	unsigned int one_byte;
	for (i = 0; i < 4; i++)
	{
		if (0 == sscanf (&(address[i * 2]), "%02x", &one_byte))
		{
			printf ("ERROR: reg parameter <%s> incorrectly formatted as a number\n", address);
			return (EXIT_FAILURE);
		}
		payload[ipayload++] = one_byte;
	}
	return (EXIT_SUCCESS);
}

void scp_lite_add_datalen (int data_len)
{
	int i;
	for (i = 3; i >= 0; i--)
	{
		payload[ipayload] = (data_len >> (8 * i)) & 255;
		ipayload++;
	}
}

void scp_lite_add_payload (u8 * data, int data_len)
{
	int i;
	for (i = 0; i < data_len; i++)
		payload[ipayload++] = data[i];
}

void scp_lite_load_mem_payload (char *ptr_addr_offset, u8 * data, int data_len)
{
	scp_lite_add_synchro ();
	scp_lite_add_address (ptr_addr_offset);
	scp_lite_add_datalen (data_len);
	scp_lite_add_payload (data, data_len);
}

/* SCP and SCP flora shared command (named WRITE-DATA for FLORA) */
int scp_lite_load_mem (char *ptr_addr_offset, u8 * data, int data_len)
{
	int err;
	scp_lite_load_mem_payload (ptr_addr_offset, data, data_len);
	err = ecdsa_sign_payload ();
	if (EXIT_FAILURE == err)
		return (err);
	add_payload ();
	sprintf (message, "%s-scp-lite-", idf_ctl[DATA_TRANSFER]);
	sprintf (name_file, "scp_lite_load_mem");
	packet_send ();
	return (EXIT_SUCCESS);
}

int scp_lite_load_ram (char *ptr_address_offset, char *sfilename, uint8_t * data)
{
	int resu;
	if (EXIT_SUCCESS == extension (".s19", sfilename))
	{
		if (EXIT_SUCCESS != read_s19 (sfilename, ptr_address_offset, data))
			return (EXIT_FAILURE);
	}
	else if (EXIT_SUCCESS == extension (".s20", sfilename))
	{
		if (EXIT_SUCCESS != read_s20 (sfilename, ptr_address_offset, data))
			return (EXIT_FAILURE);
	}
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .s19 and .s20)\n", sfilename);
		return (EXIT_FAILURE);
	}
	resu = scp_lite_load_mem (ptr_address_offset, data, data_len_g);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR in scp_lite_load_mem\n");
		return (EXIT_FAILURE);
	}
	usip ();
	scp_lite_load_mem_response ();
	host ();

	return (EXIT_SUCCESS);
}

int process_script_scp_lite_ecdsa (void)
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
		case COMMAND_SCP_LITE_LOAD_RAM:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != scp_lite_load_ram (params[0], params[1], data_g))
				{
					printf ("ERROR: scp_lite_load_ram\n");
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR: incorrect format for load-ram command: load-ram\n");
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
