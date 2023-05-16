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
#include <ucl/ucl_sys.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_aes.h>
#include <ucl/ucl_aes_ecb.h>

#include "scp_definitions.h"
#include "session_build.h"
#include "aes.h"

u8 who;
char *source[2] = { "host", "bl" };

int hex (char c1, char c2)
{
	int value;
	value = 0;
	if (c1 >= 'A' && c1 <= 'F')
		value = (c1 - 'A' + 10);
	if (c1 >= 'a' && c1 <= 'f')
		value = (c1 - 'a' + 10);
	if (c1 >= '0' && c1 <= '9')
		value = (c1 - '0');
	value *= 16;
	if (c2 >= 'A' && c2 <= 'F')
		value += (c2 - 'A' + 10);
	if (c2 >= 'a' && c2 <= 'f')
		value += (c2 - 'a' + 10);
	if (c2 >= '0' && c2 <= '9')
		value += (c2 - '0');
	return (value);
}

void display_frame (void)
{
	unsigned int i;
	if (iframe != 0)
	{
		if (HOST == who)
		{
			if (TRUE == verbose)
				printf ("<host>.%07d.%s\n", compteur, message);
			fprintf (fp_g, "<host>.%07d.%s\n", compteur, message);
		}
		else
		{
			if (TRUE == verbose)
				printf ("<chip>.%07d.%s\n", compteur, message);
			fprintf (fp_g, "<chip>.%07d.%s\n", compteur, message);
		}
		if (TRUE == verbose)
		{
			for (i = 0; i < iframe; i++)
				printf ("%02x", frame[i]);
			printf ("\n");
		}
		for (i = 0; i < iframe; i++)
			fprintf (fp_g, "%02x", frame[i]);
		fprintf (fp_g, "\n");
	}
}

void host (void)
{
	who = HOST;
}

/* for historical reason, the chip is named USIP */
/* so, this function should be changed, because we now support JIBE, LONDA, MAXQ1852 */
/* anyway, its purpose is mainly to say "not HOST" ! */
void usip (void)
{
	who = USIP;
}

void add_payload (void)
{
	unsigned int i;
	for (i = 0; i < ipayload; i++)
		frame[iframe++] = payload[i];
}

void payload_crc (void)
{
	int i;
	u8 crc[16];
	int resu;
	resu = aes_checksum (crc, payload, ipayload, 4);
	if (resu != UCL_OK)
		printf ("error: %d\n", resu);
	for (i = 3; i >= 0; i--)
		frame[iframe++] = crc[i];
}

int display_bin (void)
{
	FILE *fp;
	char name[MAX_STRING];
	unsigned int i;
	sprintf (name, "%s.%07d.%s.%s.packet", output_file, compteur, source[who], name_file);
	fp = fopen (name, "wb");
	if (NULL == fp)
	{
		printf ("ERROR while opening <%s>\n", name);
		return (EXIT_FAILURE);
	}
	for (i = 0; i < iframe; i++)
		fprintf (fp, "%c", frame[i]);
	(void) fclose (fp);
	if (TRUE == verbose)
		printf ("<%s> created\n", name);
	return (EXIT_SUCCESS);
}

void packet_send (void)
{
	compteur++;
	display_frame ();
	display_bin ();
}

int padding (unsigned int *size, u8 * input)
{
	int i;
	i = ipayload - 4;
	while ((i % 16) != 0)
		input[i++] = 0;
	(*size) = i;
	return (EXIT_SUCCESS);
}

int process_command (char *line)
{
	int i, j, k, l;
	int found_l;
	char loline[MAX_STRING];
	for (i = 0; i < (int) strlen (line); i++)
		loline[i] = (char) tolower ((int) line[i]);
	/* parse every command */
	for (found_l = 0, i = 0; i < MAX_SCP_COMMAND; i++)
		/* if command found */
		if (strstr (loline, idf_scp_cmd[i]) != NULL)
		{
			found_l = 1;
			/* process params, if any */
			/* 1st skip the identifier string */
			j = strlen (idf_scp_cmd[i]);
			/* 2nd, skip whitespaces */
			nb_params = 0;
			/* while not eol */
			while (j < (int) strlen (line) - 1)
			{
				/* while space, go on */
				while (' ' == line[j])
					j++;
				/* if not eol */
				if (j == (int) strlen (line) - 1)
					break;
				else
				{
					k = 0;
					/* while no space, retrieve param */
					while (' ' != line[j] && j < (int) strlen (line) - 1)
					{
						params[nb_params][k] = line[j];
						k++;
						j++;
					}
					params[nb_params][k] = '\0';
					k++;
					/* display param */
					if (TRUE == verbose)
					{
						printf ("params[%d]:", nb_params);
						for (l = 0; l < k - 1; l++)
							printf ("%c", params[nb_params][l]);
						printf ("\n");
					}
					nb_params++;
					/* go on */
				}
			}
			break;
		}
	if (0 == found_l)
		return (COMMAND_UNKNOWN);
	/* return command identifier, even unknown command */
	return (i);
}
