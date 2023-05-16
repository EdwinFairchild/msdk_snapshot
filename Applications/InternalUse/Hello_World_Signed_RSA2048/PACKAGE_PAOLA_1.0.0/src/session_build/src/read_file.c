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
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include <ucl/ucl_config.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_aes.h>

#include "session_build.h"
#include "scp_utils.h"

int hex_extended_address;

int last_packet_len;

/* -- ASCII files - format parameters -- */

#define S19_ADDRESS_LEN		4
#define S19_ADDRESS_START	4
#define S19_DATA_START		(S19_ADDRESS_START+S19_ADDRESS_LEN*2)
#define S19_CRC_LEN			1
#define S19_WDATA_CHAR1		'S'
#define S19_WDATA_CHAR2		'3'
#define S19_LINE_LEN_POS1	2
#define S19_LINE_LEN_POS2	3

#define S20_ADDRESS_LEN		4
#define S20_ADDRESS_START	6
#define S20_DATA_START		(S20_ADDRESS_START+S20_ADDRESS_LEN*2)
#define S20_CRC_LEN 1
#define S20_WDATA_CHAR1		'S'
#define S20_WDATA_CHAR2		'3'
#define S20_LINE_LEN_POS1	2
#define S20_LINE_LEN_POS2	3
#define S20_LINE_LEN_POS3	4
#define S20_LINE_LEN_POS4	5

#define HEX_START_CHAR			':'
#define HEX_RECORD_TYPE_POS1	 7
#define HEX_RECORD_TYPE_POS2	8
#define HEX_LINE_LEN_POS1		1
#define HEX_LINE_LEN_POS2		2
#define HEX_ADDRESS_START		3
#define HEX_ADDRESS_END			6
#define HEX_EXTENDED_LINEAR_ADDRESS_START		9
#define HEX_EXTENDED_LINEAR_ADDRESS_END			10
#define HEX_DATA_START			9

int test_hex (char c1, char c2)
{
	int value;
	int ok1, ok2;
	value = 0;
	ok1 = ok2 = 0;
	if (c1 >= 'A' && c1 <= 'F')
	{
		value = (c1 - 'A' + 10);
		ok1 = 1;
	}
	if (c1 >= 'a' && c1 <= 'f')
	{
		value = (c1 - 'a' + 10);
		ok1 = 1;
	}
	if (c1 >= '0' && c1 <= '9')
	{
		value = (c1 - '0');
		ok1 = 1;
	}
	value *= 16;
	if (c2 >= 'A' && c2 <= 'F')
	{
		value += (c2 - 'A' + 10);
		ok2 = 1;
	}
	if (c2 >= 'a' && c2 <= 'f')
	{
		value += (c2 - 'a' + 10);
		ok2 = 1;
	}
	if (c2 >= '0' && c2 <= '9')
	{
		value += (c2 - '0');
		ok2 = 1;
	}
	if (ok1 == 0 || ok2 == 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/* HEX file processing */
/* HEX format reference is http://en.wikipedia.org/wiki/Intel_HEX */
int read_hex (char *hexfilename, uint8_t * data)
{
	FILE *fpread;
	char line[MAXLINE];
	int nb_bytes;
	int i;
	int line_length;
	int line_addr;
	fpread = fopen (hexfilename, "r");
	if (fpread == NULL)
	{
		printf ("ERROR: <%s> not found\n", hexfilename);
		return (EXIT_FAILURE);
	}
	nb_bytes = 0;
	data_len_g = 0;
	hex_extended_address = 0;
	while (fscanf (fpread, "%s", line) != EOF)
	{
		if (MAXLINE < strlen (line))
		{
			printf ("ERROR: line too long: %d chars, while limited to %d\n", (int) strlen (line), MAXLINE);
			return (EXIT_FAILURE);
		}
		if (line[0] == HEX_START_CHAR)
		{
			/* for each new line, check the record type is 00, so these are data */
			if ('0' == line[HEX_RECORD_TYPE_POS1] && '0' == line[HEX_RECORD_TYPE_POS2])
			{
				/* retrieve the address, stored in HEX_ADDRESS_START..HEX_ADDRESS_END */
				for (line_addr = 0, i = HEX_ADDRESS_START; i < HEX_ADDRESS_END; i += 2)
					if (EXIT_SUCCESS == test_hex (line[i], line[i + 1]))
						line_addr = (line_addr << 8) + hex (line[i], line[i + 1]);
					else
					{
						printf ("ERROR: non hexa char detected in #%d <%c%c>\n", i, line[i], line[i + 1]);
						return (EXIT_FAILURE);
					}
				/*               printf("line address=%04x ",line_addr); */
				/*  *2 as a byte length and not a char length */
				if (EXIT_SUCCESS == test_hex (line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]))
					line_length = hex (line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]) * 2;
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", HEX_LINE_LEN_POS1,
						line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]);
					return (EXIT_FAILURE);
				}
				/*              printf("line length=%d\n ",line_length);
				   read and store data bytes
				   i is the position in the line, so expressed in char */
				if (TRUE == verbose)
					printf ("\n@=%04x-%04x (%08x) ",
						hex_extended_address, line_addr, (hex_extended_address << 16) ^ line_addr);
				for (addr_g[data_len_g] =
				     (hex_extended_address << 16) ^ line_addr, i = 0; i < line_length; i += 2, data_len_g++)
				{
					if (EXIT_SUCCESS == test_hex (line[HEX_DATA_START + i], line[HEX_DATA_START + i + 1]))
					{
						data[data_len_g] = hex (line[HEX_DATA_START + i], line[HEX_DATA_START + i + 1]);
						if (TRUE == verbose)
							printf ("%02x", data[data_len_g]);
					}
					else
					{
						printf ("ERROR: non hexa char detected in #%d <%c%c>\n", HEX_DATA_START + i,
							line[HEX_DATA_START + i], line[HEX_DATA_START + i + 1]);
						return (EXIT_FAILURE);
					}
					/*              printf("%02x (%04x)",data[data_len],addr[data_len]); */
					addr_g[data_len_g + 1] = addr_g[data_len_g] + 1;
					if (data_len_g >= max_data_size)
					{
						printf ("ERROR: hex file <%s> is too large (limited to %dMB)\n", hexfilename,
							config_struct.flash_mb);
						return (EXIT_FAILURE);
					}
				}
				/*              printf("\n"); */
				/* compute total nb of bytes of data in the hex file */
				/* not counting address and crc */
				if (EXIT_SUCCESS == test_hex (line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]))
					nb_bytes += hex (line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]);
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", HEX_LINE_LEN_POS1,
						line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]);
					return (EXIT_FAILURE);
				}
			}
			else
				/* if this is an extended address line: '04' */
			if ('0' == line[HEX_RECORD_TYPE_POS1] && '4' == line[HEX_RECORD_TYPE_POS2])
			{
				/* *2 as a byte length and not a char length */
				if (EXIT_SUCCESS == test_hex (line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]))
				{
					line_length = hex (line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]) * 2;
					/* line length shall be 4 (2*2 chars) */
					if (4 != line_length)
					{
						printf ("ERROR: extended linear address line with a length different from 2 (%d bytes)\n", line_length);
						return (EXIT_FAILURE);
					}
				}
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", HEX_LINE_LEN_POS1,
						line[HEX_LINE_LEN_POS1], line[HEX_LINE_LEN_POS2]);
					return (EXIT_FAILURE);
				}
				/* retrieve the extended address from the data */
				/* i is the position in the line, so expressed in char */
				for (hex_extended_address = 0, i = 0; i < line_length; i += 2)
				{
					if (EXIT_SUCCESS ==
					    test_hex (line
						      [HEX_EXTENDED_LINEAR_ADDRESS_START
						       + i], line[HEX_EXTENDED_LINEAR_ADDRESS_START + i + 1]))
					{
						hex_extended_address
							=
							(hex_extended_address
							 <<
							 8)
							^
							(hex
							 (line
							  [HEX_EXTENDED_LINEAR_ADDRESS_START
							   + i], line[HEX_EXTENDED_LINEAR_ADDRESS_START + i + 1]));
					}
					else
					{
						printf ("ERROR: non hexa char detected in #%d <%c%c>\n",
							HEX_EXTENDED_LINEAR_ADDRESS_START + i,
							line[HEX_EXTENDED_LINEAR_ADDRESS_START + i],
							line[HEX_EXTENDED_LINEAR_ADDRESS_START + i + 1]);
						return (EXIT_FAILURE);
					}
				}
				if (TRUE == verbose)
					printf ("\nextended-address=%08x\n", hex_extended_address);
			}
		}
		else
		{
			printf ("ERROR: unexpected char <%c>\n", line[HEX_START_CHAR]);
			return (EXIT_FAILURE);
		}
	}
	if (TRUE == verbose)
		printf ("\n");
	if (-1 == hex_extended_address)
	{
		printf ("WARNING: extended address not initialized, so set to NULL\n");
		hex_extended_address = 0;
	}
	(void) fclose (fpread);
	fprintf (fp_g, "%d==%d bytes\n", data_len_g, nb_bytes);
	fprintf (fp_g, "nb of write-mem packets: %.0f\n", 1.0 * data_len_g / (chunk_size));
	return (EXIT_SUCCESS);
}

int read_s19 (char *s19filename, char *ptr_address_offset, uint8_t * data)
{
	FILE *fpread;
	char line[MAXLINE];
	int nb_bytes;
	int i;
	int line_length;
	int line_addr;
	UNUSED_PARAMETER (ptr_address_offset);
	fpread = fopen (s19filename, "r");

	if (fpread == NULL)
	{
		printf ("ERROR: <%s> not found\n", s19filename);
		return (EXIT_FAILURE);
	}
	nb_bytes = 0;
	data_len_g = 0;
	while (fscanf (fpread, "%s", line) != EOF)
	{
		if (MAXLINE < strlen (line))
		{
			printf ("ERROR: line too long: %d chars, while limited to %d\n", (int) strlen (line), MAXLINE);
			return (EXIT_FAILURE);
		}
		if (line[0] == S19_WDATA_CHAR1 && line[1] == S19_WDATA_CHAR2)
		{
			/* for each new line, get the starting address of the line */
			/* which may be different, non contiguous with the previous line */
			for (line_addr = 0, i = S19_ADDRESS_START; i < S19_ADDRESS_START + 2 * S19_ADDRESS_LEN; i += 2)
				if (EXIT_SUCCESS == test_hex (line[i], line[i + 1]))
					line_addr = (line_addr << 8) + hex (line[i], line[i + 1]);
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", i, line[i], line[i + 1]);
					return (EXIT_FAILURE);
				}
			line_addr ^= config_struct.address_offset;
			/*  *2 as a byte length and not a char length */
			if (EXIT_SUCCESS == test_hex (line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]))
				line_length = hex (line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]) * 2;
			else
			{
				printf ("ERROR: non hexa char detected in #%d <%c%c>\n", S19_LINE_LEN_POS1,
					line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]);
				return (EXIT_FAILURE);
			}
			/* read and store data bytes */
			/* i is the position in the line, so expressed in char */
			for (addr_g[data_len_g] = line_addr, i =
			     0; i < line_length - S19_ADDRESS_LEN * 2 - S19_CRC_LEN * 2; i += 2, data_len_g++)
			{
				if (EXIT_SUCCESS == test_hex (line[S19_DATA_START + i], line[S19_DATA_START + i + 1]))
					data[data_len_g] = hex (line[S19_DATA_START + i], line[S19_DATA_START + i + 1]);
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", S19_DATA_START + i,
						line[S19_DATA_START + i], line[S19_DATA_START + i + 1]);
					return (EXIT_FAILURE);
				}
				addr_g[data_len_g + 1] = addr_g[data_len_g] + 1;
				if (data_len_g >= max_data_size)
				{
					printf ("ERROR: s19 file <%s> is too large (limited to %dMB)\n", s19filename,
						config_struct.flash_mb);
					return (EXIT_FAILURE);
				}
			}
			/* compute total nb of bytes of data in the s19 file */
			/* not counting address and crc */
			if (EXIT_SUCCESS == test_hex (line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]))
				nb_bytes +=
					hex (line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]) - S19_ADDRESS_LEN - S19_CRC_LEN;
			else
			{
				printf ("ERROR: non hexa char detected in #%d <%c%c>\n", S19_LINE_LEN_POS1,
					line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]);
				return (EXIT_FAILURE);
			}
		}
	}
	if (TRUE == verbose)
	{
		last_packet_len = data_len_g % (chunk_size);
		last_packet_len = ((last_packet_len / 16) + 1) * 16;
		fflush (0);
		if (TRUE == verbose)
		{
			printf ("last packet:%d\n", last_packet_len);
			/*  for(i=0;i<last_packet_len;i++) */
			/*    printf("%02x",data[i+(data_len_g/chunk_size)*chunk_size]); */
			printf ("\n");
			printf ("%d==%d bytes\n", data_len_g, nb_bytes);
			printf ("nb of write-mem packets: %.0f\n", 1.0 * data_len_g / (chunk_size));
		}
	}
	(void) fclose (fpread);
	fprintf (fp_g, "%d==%d bytes\n", data_len_g, nb_bytes);
	fprintf (fp_g, "nb of write-mem packets: %.0f\n", 1.0 * data_len_g / (chunk_size));

	return (EXIT_SUCCESS);
}

int get_start_addr_and_length_s19 (char *s19filename, int *end_addr_ret)
{
	FILE *fp;
	char line[MAXLINE];
	int i;
	int nb_bytes;
	int start_addr, end_addr;

	fp = fopen (s19filename, "r");
	if (fp == NULL)
	{
		printf ("ERROR: <%s> not found\n", s19filename);
		return (EXIT_FAILURE);
	}
	nb_bytes = 0;
	start_addr = -1;
	while (fscanf (fp, "%s", line) != EOF)
	{
		if (MAXLINE < strlen (line))
		{
			printf ("ERROR: line too long: %d chars, while limited to %d\n", (int) strlen (line), MAXLINE);
			return (EXIT_FAILURE);
		}
		if (line[0] == S19_WDATA_CHAR1 && line[1] == S19_WDATA_CHAR2)
		{
			if (-1 == start_addr)
			{
				for (i = S19_ADDRESS_START; i < S19_ADDRESS_START + 2 * S19_ADDRESS_LEN; i += 2)
					start_addr = (start_addr << 8) + hex (line[i], line[i + 1]);
				start_addr ^= config_struct.address_offset;
				if (TRUE == verbose)
					printf ("starting address=%08x\n", start_addr);
			}
			for (end_addr = 0, i = S19_ADDRESS_START; i < S19_ADDRESS_START + 2 * S19_ADDRESS_LEN; i += 2)
			{
				end_addr = (end_addr << 8) + hex (line[i], line[i + 1]);
			}
			/* do not forget to add the length of the line in order to have the real last address */
			end_addr += hex (line[2], line[3]) - 1 - 4;
			end_addr ^= config_struct.address_offset;
			/* compute total nb of bytes of data in the s19 file */
			/* not counting address and crc */
			nb_bytes += hex (line[S19_LINE_LEN_POS1], line[S19_LINE_LEN_POS2]) - S19_ADDRESS_LEN - S19_CRC_LEN;
		}
	}
	if (TRUE == verbose)
	{
		printf ("end addr=%08x\n", end_addr);
		printf ("file <%s> nb bytes: %d\n", s19filename, nb_bytes);
	}
	(void) fclose (fp);
	*end_addr_ret = end_addr;
	return (start_addr);
}

int read_s20 (char *s20filename, char *ptr_start_address, uint8_t * data)
{
	FILE *fpread;
	char line[MAXLINE];
	int nb_bytes;
	int i;
	int line_length;
	int line_addr;
	UNUSED_PARAMETER (ptr_start_address);
	fpread = fopen (s20filename, "r");
	if (fpread == NULL)
	{
		printf ("ERROR: <%s> not found\n", s20filename);
		return (EXIT_FAILURE);
	}
	nb_bytes = 0;
	data_len_g = 0;
	while (fscanf (fpread, "%s", line) != EOF)
	{
		if (MAXLINE < strlen (line))
		{
			printf ("ERROR: line too long: %d chars, while limited to %d\n", (int) strlen (line), MAXLINE);
			return (EXIT_FAILURE);
		}
		if (line[0] == S20_WDATA_CHAR1 && line[1] == S20_WDATA_CHAR2)
		{
			/* for each new line, get the starting address of the line */
			/* which may be different, non contiguous with the previous line */
			for (line_addr = 0, i = S20_ADDRESS_START; i < S20_ADDRESS_START + 2 * S20_ADDRESS_LEN; i += 2)
				if (EXIT_SUCCESS == test_hex (line[i], line[i + 1]))
					line_addr = (line_addr << 8) + hex (line[i], line[i + 1]);
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", i, line[i], line[i + 1]);
					return (EXIT_FAILURE);
				}
			line_addr ^= config_struct.address_offset;
			/* *2 as a two-byte length (we are in s20) and not a char length */
			if (EXIT_SUCCESS ==
			    test_hex (line[S20_LINE_LEN_POS1],
				      line[S20_LINE_LEN_POS2])
			    && EXIT_SUCCESS == test_hex (line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4]))
				line_length =
					(hex
					 (line
					  [S20_LINE_LEN_POS1],
					  line
					  [S20_LINE_LEN_POS2])
					 * 256 + hex (line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4])) * 2;
			else
			{
				printf ("ERROR: non hexa char detected in <%c%c><%c%c>\n", line[S20_LINE_LEN_POS1],
					line[S20_LINE_LEN_POS2], line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4]);
				return (EXIT_FAILURE);
			}
			/* read and store data bytes */
			/* i is the position in the line, so expressed in char */
			for (addr_g[data_len_g] = line_addr, i =
			     0; i < line_length - S20_ADDRESS_LEN * 2 - S20_CRC_LEN * 2; i += 2, data_len_g++)
			{
				if (EXIT_SUCCESS == test_hex (line[S20_DATA_START + i], line[S20_DATA_START + i + 1]))
					data[data_len_g] = hex (line[S20_DATA_START + i], line[S20_DATA_START + i + 1]);
				else
				{
					printf ("ERROR: non hexa char detected in #%d <%c%c>\n", S20_DATA_START + i,
						line[S20_DATA_START + i], line[S20_DATA_START + i + 1]);
					return (EXIT_FAILURE);
				}
				addr_g[data_len_g + 1] = addr_g[data_len_g] + 1;
				if (data_len_g >= max_data_size)
				{
					printf ("ERROR: s20 file <%s> is too large (limited to %dMB)\n", s20filename,
						config_struct.flash_mb);
					return (EXIT_FAILURE);
				}
			}
			/* compute total nb of bytes of data in the s20 file */
			/* not counting address and crc */
			if (EXIT_SUCCESS ==
			    test_hex (line[S20_LINE_LEN_POS1],
				      line[S20_LINE_LEN_POS2])
			    && EXIT_SUCCESS == test_hex (line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4]))
				nb_bytes +=
					hex (line
					     [S20_LINE_LEN_POS1],
					     line
					     [S20_LINE_LEN_POS2])
					* 256 +
					hex (line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4]) - S20_ADDRESS_LEN - S20_CRC_LEN;
			else
			{
				printf ("ERROR: non hexa char detected in <%c%c><%c%c>\n", line[S20_LINE_LEN_POS1],
					line[S20_LINE_LEN_POS2], line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4]);
				return (EXIT_FAILURE);
			}
		}
	}
	if (TRUE == verbose)
	{
		last_packet_len = data_len_g % (chunk_size);
		if (TRUE == verbose)
			printf ("last packet:%d\n", last_packet_len);
		last_packet_len = ((last_packet_len / 16) + 1) * 16;
		if (TRUE == verbose)
		{
			printf ("last packet:%d\n", last_packet_len);
			for (i = 0; i < last_packet_len; i++)
				printf ("%02x", data[i + (data_len_g / chunk_size) * chunk_size]);
			printf ("\n");
			printf ("%d==%d bytes\n", data_len_g, nb_bytes);
			printf ("nb of write-mem packets: %.0f\n", 1.0 * data_len_g / (chunk_size));
		}
	}
	(void) fclose (fpread);
	fprintf (fp_g, "%d==%d bytes\n", data_len_g, nb_bytes);
	fprintf (fp_g, "nb of write-mem packets: %.0f\n", 1.0 * data_len_g / (chunk_size));
	return (EXIT_SUCCESS);
}

int get_start_addr_and_length_s20 (char *s20filename, int *end_addr_ret)
{
	FILE *fp;
	char line[MAXLINE];
	int i;
	int nb_bytes;
	int start_addr, end_addr;

	fp = fopen (s20filename, "r");
	if (fp == NULL)
	{
		printf ("ERROR: <%s> not found\n", s20filename);
		return (EXIT_FAILURE);
	}
	nb_bytes = 0;
	start_addr = -1;
	while (fscanf (fp, "%s", line) != EOF)
	{
		if (MAXLINE < strlen (line))
		{
			printf ("ERROR: line too long: %d chars, while limited to %d\n", (int) strlen (line), MAXLINE);
			return (EXIT_FAILURE);
		}
		if (line[0] == S20_WDATA_CHAR1 && line[1] == S20_WDATA_CHAR2)
		{
			if (-1 == start_addr)
			{
				for (i = S20_ADDRESS_START; i < S20_ADDRESS_START + 2 * S20_ADDRESS_LEN; i += 2)
					start_addr = (start_addr << 8) + hex (line[i], line[i + 1]);
				start_addr ^= config_struct.address_offset;
				if (TRUE == verbose)
					printf ("starting address=%08x\n", start_addr);
			}
			for (end_addr = 0, i = S20_ADDRESS_START; i < S20_ADDRESS_START + 2 * S20_ADDRESS_LEN; i += 2)
				end_addr = (end_addr << 8) + hex (line[i], line[i + 1]);
			/* do not forget to add the length of the line in order to have the real last address */
			end_addr += hex (line[2], line[3]) - 1;
			end_addr ^= config_struct.address_offset;
			/* compute total nb of bytes of data in the s20 file */
			/* not counting address and crc */
			/*      nb_bytes+=hex(line[S20_LINE_LEN_POS1],line[S20_LINE_LEN_POS2])-S20_ADDRESS_LEN-S20_CRC_LEN; */
			nb_bytes +=
				hex (line[S20_LINE_LEN_POS1],
				     line[S20_LINE_LEN_POS2])
				* 256 + hex (line[S20_LINE_LEN_POS3], line[S20_LINE_LEN_POS4]) - S20_ADDRESS_LEN - S20_CRC_LEN;
		}
	}
	if (TRUE == verbose)
	{
		printf ("end addr=%08x\n", end_addr);
		printf ("file <%s> nb bytes: %d\n", s20filename, nb_bytes);
	}
	(void) fclose (fp);
	*end_addr_ret = end_addr;
	return (start_addr);
}

int read_file_ascii_data (FILE * p_pFile, const unsigned int p_iHexDataLength, u8 * p_pucHexDataBuf,
			  unsigned int *p_piHexDataBufLen)
{
	int l_iErr = EXIT_SUCCESS;
	unsigned int l_iIndex = 0;
	unsigned int l_iData = 0;
	if (p_pFile == NULL)
	{
		return EXIT_FAILURE;
	}
	if (p_pucHexDataBuf == NULL)
	{
		return EXIT_FAILURE;
	}
	if (p_piHexDataBufLen == NULL)
	{
		return EXIT_FAILURE;
	}
	if (*p_piHexDataBufLen == 0)
	{
		return EXIT_FAILURE;
	}
	if (*p_piHexDataBufLen < p_iHexDataLength)
	{
		return EXIT_FAILURE;
	}
	for (l_iIndex = 0; l_iIndex < p_iHexDataLength; l_iIndex++)
	{
		l_iErr = fscanf (p_pFile, "%02x", &l_iData);
		if (l_iErr != 1)
		{
			printf ("ERROR: read text file error\n");
			return (EXIT_FAILURE);
		}
		p_pucHexDataBuf[l_iIndex] = l_iData;
	}
	return EXIT_SUCCESS;
}

int extension (char *ext, char *name)
{
	int i;
	char name1[MAXLINE];
	char ext1[MAXLINE];
	for (i = 0; i < (int) strlen (ext); i++)
		ext1[i] = (char) toupper ((int) ext[i]);
	ext1[strlen (ext)] = '\0';
	for (i = 0; i < (int) strlen (name); i++)
		name1[i] = (char) toupper ((int) name[i]);
	name1[strlen (name)] = '\0';
	if (strstr (name1, ext1) != NULL)
		return (EXIT_SUCCESS);
	else
		return (EXIT_FAILURE);
}

int determine_file_size (unsigned int *filesize, char *filename)
{
	FILE *fp;
	u8 d8;
	int resu;
	fp = fopen (filename, "r");
	if (NULL == fp)
	{
		printf ("error determine\n");
		return (EXIT_FAILURE);
	}
	*filesize = 0;
	while ((resu = fscanf (fp, "%c", (unsigned char *) &d8)) == 1)
		*filesize = *filesize + 1;
	fclose (fp);
	return (EXIT_SUCCESS);
}
