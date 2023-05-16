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

#include "session_build.h"
#include "scp_utils.h"
#include "read_file.h"
#include "ecdsa.h"
#include "rsa.h"
#include "aes.h"
#include "scp.h"

#define CHAR_TIMEOUT_UART_TARGET	'0'
#define CHAR_TIMEOUT_USB_TARGET		'U'
#define CHAR_TIMEOUT_VBUS_TARGET	'V'

#define TIMEOUT_UART_TARGET			0x00
#define TIMEOUT_USB_TARGET			0x55
#define TIMEOUT_VBUS_TARGET			0x56

char idf_cmd[MAX_IDF][MAX_STRING];

u8 response[UCL_AES_BLOCKSIZE];

int list_cmd[MAX_IDF];

u8 hello_req_const[MAX_TAB];
u8 hello_rep_const[MAX_TAB];
u8 hello_off_req_const[MAX_TAB];
u8 hello_off_rep_const[MAX_TAB];
u8 hello_scp_req_const[MAX_TAB];
u8 hello_scp_rep_const[MAX_TAB];

u8 crk_rsa_modulus[PAOLA_RSA_MODULUS_LEN];
u8 crk_rsa_pubexp[RSA_PUBLIC_EXPONENT_LEN];
u8 mrk_signature_g[PAOLA_RSA_MODULUS_LEN];
u8 crk_rsa_modulus2[PAOLA_RSA_MODULUS_LEN];
u8 crk_rsa_pubexp2[RSA_PUBLIC_EXPONENT_LEN];
u8 mrk_signature2[PAOLA_RSA_MODULUS_LEN];

u8 crk_ecdsa_x[ECDSA_MODULUS_LEN];
u8 crk_ecdsa_y[ECDSA_MODULUS_LEN];
u8 crk_ecdsa_x2[ECDSA_MODULUS_LEN];
u8 crk_ecdsa_y2[ECDSA_MODULUS_LEN];
u8 mrk_ecdsa_r[ECDSA_MODULUS_LEN];
u8 mrk_ecdsa_s[ECDSA_MODULUS_LEN];
u8 mrk_ecdsa_r2[ECDSA_MODULUS_LEN];
u8 mrk_ecdsa_s2[ECDSA_MODULUS_LEN];

int ch_id, tr_id;
type_config_struct config_struct;
u8 session_mode;

int last_packet_len;

void define_const (void)
{
	hello_req_const[0] = 0x48;
	hello_req_const[1] = 0x49;
	hello_req_const[2] = 0x2D;
	hello_req_const[3] = 0x55;
	hello_req_const[4] = 0x53;
	hello_req_const[5] = 0x49;
	/* to be corrected */
	hello_req_const[6] = 0x50;
	hello_req_const[7] = 0x00;

	hello_rep_const[0] = 0x48;
	hello_rep_const[1] = 0x49;
	hello_rep_const[2] = 0x2D;
	hello_rep_const[3] = 0x55;
	hello_rep_const[4] = 0x53;
	hello_rep_const[5] = 0x49;
	hello_rep_const[6] = 0x50;

	hello_scp_req_const[0] = 'H';
	hello_scp_req_const[1] = 'E';
	hello_scp_req_const[2] = 'L';
	hello_scp_req_const[3] = 'L';
	hello_scp_req_const[4] = 'O';
	hello_scp_req_const[5] = ' ';
	hello_scp_req_const[6] = 'B';
	hello_scp_req_const[7] = 'L';
	hello_scp_req_const[8] = 0x00;

	hello_scp_rep_const[0] = 'H';
	hello_scp_rep_const[1] = 'E';
	hello_scp_rep_const[2] = 'L';
	hello_scp_rep_const[3] = 'L';
	hello_scp_rep_const[4] = 'O';
	hello_scp_rep_const[5] = ' ';
	hello_scp_rep_const[6] = 'H';
	hello_scp_rep_const[7] = 'O';
	hello_scp_rep_const[8] = 'S';
	hello_scp_rep_const[9] = 'T';

	hello_off_req_const[0] = 0x48;
	hello_off_req_const[1] = 0x4F;
	hello_off_req_const[2] = 0x2D;
	hello_off_req_const[3] = 0x55;
	hello_off_req_const[4] = 0x53;
	hello_off_req_const[5] = 0x49;
	/*  to be corrected */
	hello_off_req_const[6] = 0x50;
	hello_off_req_const[7] = 0x00;

	hello_off_rep_const[0] = 0x48;
	hello_off_rep_const[1] = 0x49;
	hello_off_rep_const[2] = 0x2D;
	hello_off_rep_const[3] = 0x55;
	hello_off_rep_const[4] = 0x53;
	hello_off_rep_const[5] = 0x49;
	hello_off_rep_const[6] = 0x50;

}

void define_cmd (void)
{
	int i;
	for (i = 0; i < MAX_IDF; i++)
		list_cmd[i] = 0;
	list_cmd[1] = list_cmd[2] = list_cmd[7] = list_cmd[3] = list_cmd[4] = list_cmd[5] = list_cmd[8] = list_cmd[9] = 1;
	strcpy (idf_cmd[1], "HELLO");
	strcpy (idf_cmd[2], "HELLO_REP");
	strcpy (idf_cmd[7], "CHLG");
	strcpy (idf_cmd[3], "SUCCESS");
	strcpy (idf_cmd[4], "FAILURE");
	strcpy (idf_cmd[5], "DATA");
	strcpy (idf_cmd[8], "HELLO_OFF");
	strcpy (idf_cmd[9], "HELLO_OFF_REP");
}

void synchro (void)
{
	iframe = 0;
	frame[iframe++] = SYNCH1;
	frame[iframe++] = SYNCH2;
	frame[iframe++] = SYNCH3;
}

void con_req (void)
{
	frame[iframe++] = CON_REQ;
}

void con_rep (void)
{
	frame[iframe++] = CON_REP;
}

void discon_req (void)
{
	frame[iframe++] = DISC_REQ;
}

void discon_rep (void)
{
	frame[iframe++] = DISC_REP;
}

void add_byte (u8 value)
{
	frame[iframe++] = value;
}

void add_channel_id_seq (void)
{
	frame[iframe++] = (ch_id << 4) + (seq & 15);
}

void header_crc (void)
{
	u8 crc[16];
	aes_checksum (crc, frame, iframe, 1);
	frame[iframe++] = crc[0];
}

int connection_request (void)
{
	sprintf (message, "%s", idf_ctl[1]);
	synchro ();
	con_req ();
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	add_channel_id_seq ();
	header_crc ();
	sprintf (name_file, "connection_request");
	packet_send ();
	return (EXIT_SUCCESS);
}

int connection_reply (void)
{
	sprintf (message, "%s", idf_ctl[2]);
	synchro ();
	con_rep ();
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	add_channel_id_seq ();
	header_crc ();
	sprintf (name_file, "connection_reply");
	packet_send ();
	return (EXIT_SUCCESS);
}

int disconnection_request (void)
{
	sprintf (message, "%s", idf_ctl[3]);
	seq++;
	synchro ();
	discon_req ();
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	add_channel_id_seq ();
	header_crc ();
	sprintf (name_file, "disconnection_request");
	packet_send ();
	return (EXIT_SUCCESS);
}

int disconnection_reply (void)
{
	sprintf (message, "%s", idf_ctl[4]);
	synchro ();
	discon_rep ();
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	add_channel_id_seq ();
	header_crc ();
	sprintf (name_file, "disconnection_reply");
	packet_send ();
	return (EXIT_SUCCESS);
}

/*            ECHO */

int echo_request_payload (void)
{
	int i;

	for (i = 0; i < 35; i++)
	{
		payload[ipayload++] = HELLO_REQ << 4;
		payload[ipayload++] = 0xBE & i;
		payload[ipayload++] = 0x54 & i;
		payload[ipayload++] = 0xFF & i;
		payload[ipayload++] = 0x4A & i;
		payload[ipayload++] = 0x02 & i;
		payload[ipayload++] = 0x0A & i;
		payload[ipayload++] = 0x76 & i;
	}

	return (EXIT_SUCCESS);
}

int echo_req (void)
{

	ipayload = 0;
	echo_request_payload ();
	sprintf (message, "%s", idf_ctl[0xb]);
	seq++;
	synchro ();
	frame[iframe++] = ECHO_REQ;
	/* len */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);

	add_channel_id_seq ();
	seq--;
	header_crc ();
	add_payload ();
	payload_crc ();

	sprintf (name_file, "echo_request");
	packet_send ();
	return (EXIT_SUCCESS);
}

int echo_reply (void)
{
	sprintf (message, "%s", idf_ctl[0xc]);
	synchro ();
	frame[iframe++] = ECHO_REP;
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	add_channel_id_seq ();
	header_crc ();
	sprintf (name_file, "echo_reply");
	packet_send ();
	return (EXIT_SUCCESS);
}

int dump (void)
{
	sprintf (message, "%s", idf_ctl[6]);
	synchro ();
	/* ack_code(); */
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	/* add_channel_id_seq(); */
	/* header_crc(); */
	sprintf (name_file, "dump");
	packet_send ();
	return (EXIT_SUCCESS);
}

void ack_code (void)
{
	frame[iframe++] = ACK;
}

int ack (void)
{
	sprintf (message, "%s", idf_ctl[6]);
	synchro ();
	ack_code ();
	/* len */
	add_byte (0x00);
	add_byte (0x00);

	add_channel_id_seq ();
	header_crc ();
	sprintf (name_file, "ack");
	packet_send ();
	return (EXIT_SUCCESS);
}

void data_code (void)
{
	frame[iframe++] = DATA_TRANSFER;
}

int generic_response_payload (void)
{
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	tr_id = (tr_id + 1) % 256;
	payload[ipayload++] = 0;
	/* response size: 4 bytes for code */
	payload[ipayload++] = 4;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	return (EXIT_SUCCESS);
}

int generic_response (char *msg)
{
	if (SBL == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	generic_response_payload ();
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode
	    || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		/* nothing: no signature on response */
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], msg);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "%s", msg);
	packet_send ();
	return (EXIT_SUCCESS);
}

int hello_request_payload (void)
{
	int i;
	if (SBL == session_mode)
	{
		payload[ipayload++] = HELLO_REQ << 4;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x08;
		for (i = 0; i < HELLO_REQ_CONST_LEN; i++)
			payload[ipayload++] = hello_req_const[i];
	}
	else
	{
		payload[ipayload++] = HELLO_REQ << 4;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x0A;
		for (i = 0; i < HELLO_SCP_REQ_CONST_LEN; i++)
			payload[ipayload++] = hello_scp_req_const[i];
		/* from the specs, the hello are the same for these two modes */
		if (SCP_ON_AES == session_mode || SCP_FLORA_AES == session_mode)
			payload[ipayload - 1] = 0x00;
		if (SCP_OFF_AES == session_mode)
			payload[ipayload - 1] = 0x01;
		/* from the specs, the hello are the same for these two modes */
		if ((SCP_RSA == session_mode) || (SCP_FLORA_RSA == session_mode) || (SCP_PAOLA == session_mode))
			payload[ipayload - 1] = 0x02;
		if ((SCP_ANGELA_ECDSA == session_mode))
			payload[ipayload - 1] = 0x03;
		/* Admin mode */
		/* alway used for SCP_FLORA */
		payload[ipayload++] = 0x02;
	}
	return (EXIT_SUCCESS);
}

int hello_reply_payload (void)
{
	int i;
	if (SBL == session_mode)
	{
		payload[ipayload++] = HELLO_REP << 4;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x2E;
		for (i = 0; i < HELLO_REP_CONST_LEN; i++)
			payload[ipayload++] = hello_rep_const[i];
		payload[ipayload++] = 0x04;
		payload[ipayload++] = 0x03;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x03;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x03;
		payload[ipayload++] = 0x00;
		for (i = 0; i < config_struct.usn_len; i++)
			payload[ipayload++] = config_struct.usn[i];
		for (i = 0; i < 16; i++)
			payload[ipayload++] = rand () & 255;
	}
	else if (SCP_ON_AES == session_mode || SCP_FLORA_AES == session_mode)
	{
		payload[ipayload++] = HELLO_REP << 4;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x32;
		for (i = 0; i < HELLO_SCP_REP_CONST_LEN; i++)
			payload[ipayload++] = hello_scp_rep_const[i];
		if (SCP_FLORA_AES == session_mode)
		{
			/* flora major version one byte */
			payload[ipayload++] = 0x00;
			/* flora minor version one byte */
			payload[ipayload++] = 0x00;
			/*  4 rfu bytes */
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			/* 0x00 byte */
			payload[ipayload++] = 0x00;
			/* configuration */
			payload[ipayload++] = 0x01;
		}
		else
		{
			/*  bug #1621 correction */
			/* major version number, on 4 bytes */
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x01;
			/* rfu byte, 0x00 */
			payload[ipayload++] = 0x00;
			/* annex identifier */
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x01;
			payload[ipayload++] = 0x01;
		}
		/* usn */
		for (i = 0; i < config_struct.usn_len; i++)
			payload[ipayload++] = config_struct.usn[i];
		/* random number */
		for (i = 0; i < 16; i++)
			payload[ipayload++] = config_struct.random_number[i];
	}
	else if (SCP_OFF_AES == session_mode || SCP_RSA == session_mode
		 || SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		payload[ipayload++] = HELLO_REP << 4;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x00;
		payload[ipayload++] = 0x32;
		for (i = 0; i < HELLO_SCP_REP_CONST_LEN; i++)
			payload[ipayload++] = hello_scp_rep_const[i];
		if (SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
		{
			/* flora major version, one byte */
			payload[ipayload++] = 0x01;
			/* flora minor version, one byte */
			payload[ipayload++] = 0x00;
			/* four RFU bytes */
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			/* 0x00 byte */
			payload[ipayload++] = 0x00;
			/* configuration byte */
			/* jtag and rwk enabled */
			payload[ipayload++] = (1 << 7) ^ (1 << 6);
		}
		else
		{
			/*  bug #1621 correction */
			/* major version number, on 4 bytes */
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x01;
			/* rfu byte, 0x00 */
			payload[ipayload++] = 0x00;
			/* annex identifier */
			payload[ipayload++] = 0x00;
			payload[ipayload++] = 0x01;
			payload[ipayload++] = 0x01;
		}
		/* usn */
		for (i = 0; i < config_struct.usn_len; i++)
			payload[ipayload++] = config_struct.usn[i];
		/* for flora modes, pad with 0 up to 16 bytes */
		for (i = config_struct.usn_len; i < 16; i++)
			payload[ipayload++] = 0;
		/* random number */
		for (i = 0; i < 16; i++)
			payload[ipayload++] = 0x00;
	}
	else
		printf ("ERROR: session mode not supported yet\n");
	return (EXIT_SUCCESS);
}

int hello_request (void)
{
	ipayload = 0;
	hello_request_payload ();
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_cmd[1]);
	synchro ();
	data_code ();
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	header_crc ();
	add_payload ();
	payload_crc ();
	sprintf (name_file, "hello_request");
	packet_send ();
	return (EXIT_SUCCESS);
}

int hello_reply (void)
{
	ipayload = 0;
	seq++;
	hello_reply_payload ();
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_cmd[2]);
	synchro ();
	data_code ();
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	header_crc ();
	add_payload ();
	payload_crc ();
	sprintf (name_file, "hello_reply");
	packet_send ();
	return (EXIT_SUCCESS);
}

int challenge_payload (void)
{
	u8 fixed_random_number[] = { 0xdc, 0xa3, 0xae, 0x5f, 0x69, 0x8f, 0xb2, 0xeb, 0xcd, 0x99,
		0x91,
		0x79,
		0x98, 0x75, 0xdd, 0x78
	};
	int i;
	int resu;
	payload[ipayload++] = (CHALLENGE << 4) ^ config_struct.pp;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x10;
	if (SBL == session_mode)
	{
		resu = ucl_aes_ecb (response, config_struct.random_number, 16, keya, 16, UCL_CIPHER_ENCRYPT);

		if (UCL_OK != resu)
			return (EXIT_FAILURE);
		for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
			payload[ipayload++] = response[i];
	}
	else if (SCP_ON_AES == session_mode || SCP_FLORA_AES == session_mode)
	{
		/*  encrypted+cmac */
		if (TRUE == verbose)
		{
			printf ("rn:");
			for (i = 0; i < 16; i++)
				printf ("%02x", config_struct.random_number[i]);
			printf ("\n");
		}
		config_struct.random_number[0] = (fixed_random_number[0]) ^ config_struct.pp;
		if (TRUE == verbose)
		{
			printf ("rn^pp:");
			for (i = 0; i < 16; i++)
				printf ("%02x", config_struct.random_number[i]);
			printf ("\n");
		}

		resu = ucl_aes_ecb (response, config_struct.random_number, 16, keya, 16, UCL_CIPHER_ENCRYPT);

		if (UCL_OK != resu)
			return (EXIT_FAILURE);
		if (TRUE == verbose)
		{
			printf ("response:");
			for (i = 0; i < 16; i++)
				printf ("%02x", response[i]);
			printf ("\n");
		}
		for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
			payload[ipayload++] = response[i];
	}
	else
		printf ("ERROR: no challenge for this mode\n");
	return (EXIT_SUCCESS);
}

int challenge (void)
{
	ipayload = 0;
	seq++;
	challenge_payload ();
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_cmd[7]);
	synchro ();
	data_code ();
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	header_crc ();
	add_payload ();
	payload_crc ();
	sprintf (name_file, "challenge");
	packet_send ();
	return (EXIT_SUCCESS);
}

int challenge_response_payload (void)
{
	payload[ipayload++] = SUCCESS << 4;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	payload[ipayload++] = 0x00;
	return (EXIT_SUCCESS);
}

int challenge_response (void)
{
	ipayload = 0;
	seq++;
	challenge_response_payload ();
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_cmd[3]);
	synchro ();
	data_code ();
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	header_crc ();
	add_payload ();
	payload_crc ();
	sprintf (name_file, "challenge_response");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* le mapping renvoy\E9 est bidon */
int aes_comp_response_payload (void)
{
	int i;
	int resu;
	u8 output[16];

	resu = ucl_aes_ecb (output, aes_data, 16, aes_key, 16, UCL_CIPHER_ENCRYPT);

	if (UCL_OK != resu)
	{
		printf ("ERROR: AES code %d\n", resu);
		return (EXIT_FAILURE);
	}
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	tr_id = (tr_id + 1) % 256;
	for (i = 0; i < 16; i++)
		payload[ipayload++] = output[i];
	/* error code, on 4 bytes */
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	return (EXIT_SUCCESS);
}

int aes_comp_response (void)
{
	if (SBL == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	mem_mapping_response_payload ();
	if (SCP_RSA == session_mode)
	{
		/* nothing: no signature on response */
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], "AES-COMP-RESPONSE");
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "aes_comp_response");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int
write_crk_payload_rsa (u8 * rsa_modulus, int rsa_modulus_len, u8 * rsa_pubexp,
		       int rsa_explen, u8 * mrk_signature, int mrk_signature_len)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	/* command length: 2 bytes for command , 2 bytes for data len, 256 bytes for crk modulus, 4 bytes for pubexp, 256 bytes for mrk signature */
	/* data value is a hexa string so, its data len is string len/2 */
	payload[ipayload++] = (2 + 2 + rsa_modulus_len + 4 + mrk_signature_len) >> 8;
	payload[ipayload++] = (2 + 2 + rsa_modulus_len + 4 + mrk_signature_len) & 255;
	payload[ipayload++] = WRITE_CRK >> 8;
	payload[ipayload++] = WRITE_CRK & 255;
	payload[ipayload++] = (rsa_modulus_len + mrk_signature_len + rsa_explen) >> 8;
	payload[ipayload++] = (rsa_modulus_len + mrk_signature_len + rsa_explen) & 255;
	for (i = 0; i < rsa_modulus_len; i++)
		payload[ipayload++] = rsa_modulus[i];
	for (i = 0; i < rsa_explen; i++)
		payload[ipayload++] = rsa_pubexp[i];
	for (i = 0; i < mrk_signature_len; i++)
		payload[ipayload++] = mrk_signature[i];
	if (TRUE == verbose)
	{
		printf ("CRK modulus:");
		for (i = 0; i < rsa_modulus_len; i++)
			printf ("%02x", rsa_modulus[i]);
		printf ("\n");
		printf ("CRK public exponent:");
		for (i = 0; i < rsa_explen; i++)
			printf ("%02x", rsa_pubexp[i]);
		printf ("\n");
		printf ("MRK signature:");
		for (i = 0; i < mrk_signature_len; i++)
			printf ("%02x", mrk_signature[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int write_crk_payload_ecdsa (u8 * x, u8 * y, u8 * r, u8 * s, int ecdsa_len)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	/* command length: 2 bytes for command , 2 bytes for data len, 256 bytes for crk modulus, 4 bytes for pubexp, 256 bytes for mrk signature */
	/* data value is a hexa string so, its data len is string len/2 */
	payload[ipayload++] = (2 + 2 + ecdsa_len * 4) >> 8;
	payload[ipayload++] = (2 + 2 + ecdsa_len * 4) & 255;
	payload[ipayload++] = WRITE_CRK >> 8;
	payload[ipayload++] = WRITE_CRK & 255;
	payload[ipayload++] = (ecdsa_len * 4) >> 8;
	payload[ipayload++] = (ecdsa_len * 4) & 255;
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = x[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = y[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = r[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = s[i];
	if (TRUE == verbose)
	{
		printf ("CRK public key\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", x[i]);
		printf ("\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", y[i]);
		printf ("\n");
		printf ("MRK signature\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", r[i]);
		printf ("\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", s[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int rewrite_crk_payload_ecdsa (u8 * oldx, u8 * oldy, u8 * newx, u8 * newy, u8 * newr, u8 * news, int ecdsa_len)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	/* command length: 2 bytes for command , 2 bytes for data len, 64 bytes for old crk, 64 bytes for new crk, 64 bytes for new crk signature */
	/* data value is a hexa string so, its data len is string len/2 */
	payload[ipayload++] = (2 + 2 + 2 * ecdsa_len + 2 * ecdsa_len + 2 * ecdsa_len) >> 8;
	payload[ipayload++] = (2 + 2 + 2 * ecdsa_len + 2 * ecdsa_len + 2 * ecdsa_len) & 255;
	payload[ipayload++] = REWRITE_CRK >> 8;
	payload[ipayload++] = REWRITE_CRK & 255;
	payload[ipayload++] = (2 * ecdsa_len + 2 * ecdsa_len + 2 * ecdsa_len) >> 8;
	payload[ipayload++] = (2 * ecdsa_len + 2 * ecdsa_len + 2 * ecdsa_len) & 255;
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = oldx[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = oldy[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = newx[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = newy[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = newr[i];
	for (i = 0; i < ecdsa_len; i++)
		payload[ipayload++] = news[i];
	if (TRUE == verbose)
	{
		printf ("old CRK public key\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", oldx[i]);
		printf ("\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", oldy[i]);
		printf ("\n");
		printf ("new CRK public key\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", newx[i]);
		printf ("\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", newy[i]);
		printf ("\n");
		printf ("MRK signature\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", newr[i]);
		printf ("\n");
		for (i = 0; i < ecdsa_len; i++)
			printf ("%02x", news[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int
rewrite_crk_payload_rsa (u8 * oldmodulus, int oldmodulus_len, u8 * oldpubexp,
			 int oldpub_len, u8 * newmodulus, int newmodulus_len,
			 u8 * newpubexp, int newpub_len, u8 * signature, int signature_len)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	/* command length: 2 bytes for command , 2 bytes for data len, 64 bytes for old crk, 64 bytes for new crk, 64 bytes for new crk signature */
	/* data value is a hexa string so, its data len is string len/2 */
	payload[ipayload++] = (2 + 2 + oldmodulus_len + oldpub_len + newmodulus_len + newpub_len + signature_len) >> 8;
	payload[ipayload++] = (2 + 2 + oldmodulus_len + oldpub_len + newmodulus_len + newpub_len + signature_len) & 255;
	payload[ipayload++] = REWRITE_CRK >> 8;
	payload[ipayload++] = REWRITE_CRK & 255;
	payload[ipayload++] = (oldmodulus_len + oldpub_len + newmodulus_len + newpub_len + signature_len) >> 8;
	payload[ipayload++] = (oldmodulus_len + oldpub_len + newmodulus_len + newpub_len + signature_len) & 255;
	for (i = 0; i < oldmodulus_len; i++)
		payload[ipayload++] = oldmodulus[i];
	for (i = 0; i < oldpub_len; i++)
		payload[ipayload++] = oldpubexp[i];
	for (i = 0; i < newmodulus_len; i++)
		payload[ipayload++] = newmodulus[i];
	for (i = 0; i < newpub_len; i++)
		payload[ipayload++] = newpubexp[i];
	for (i = 0; i < signature_len; i++)
		payload[ipayload++] = signature[i];
	if (TRUE == verbose)
	{
		printf ("old CRK public key\n");
		for (i = 0; i < oldmodulus_len; i++)
			printf ("%02x", oldmodulus[i]);
		printf ("\n");
		for (i = 0; i < oldpub_len; i++)
			printf ("%02x", oldpubexp[i]);
		printf ("\n");
		printf ("new CRK public key\n");
		for (i = 0; i < newmodulus_len; i++)
			printf ("%02x", newmodulus[i]);
		printf ("\n");
		for (i = 0; i < newpub_len; i++)
			printf ("%02x", newpubexp[i]);
		printf ("\n");
		printf ("MRK signature\n");
		for (i = 0; i < signature_len; i++)
			printf ("%02x", signature[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_crk (char *signaturefile)
{
	int rsa_explen = RSA_PUBLIC_EXPONENT_LEN;
	int ecdsa_len = ECDSA_MODULUS_LEN;
	int rsa_len;
	int mrk_signature_len;
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_FLORA_AES != session_mode
	    && SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_WRITE_CRK]);
		return (EXIT_FAILURE);
	}
	if (SCP_ANGELA_ECDSA == session_mode)
		err = read_file_signed_ecdsa_publickey (crk_ecdsa_x,
							crk_ecdsa_y, mrk_ecdsa_r, mrk_ecdsa_s, ecdsa_len, signaturefile);
	else
		err = read_file_signed_rsa_publickey (crk_rsa_modulus,
						      &rsa_len,
						      crk_rsa_pubexp,
						      rsa_explen, mrk_signature_g, &mrk_signature_len, signaturefile);
	if (EXIT_SUCCESS != err)
	{
		printf ("ERROR in read_file_signature\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	if (SCP_ANGELA_ECDSA == session_mode)
	{
		write_crk_payload_ecdsa (crk_ecdsa_x, crk_ecdsa_y, mrk_ecdsa_r, mrk_ecdsa_s, ecdsa_len);
	}
	else
		write_crk_payload_rsa (crk_rsa_modulus, rsa_len, crk_rsa_pubexp, rsa_explen, mrk_signature_g, mrk_signature_len);
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_WRITE_CRK]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_crk");
	packet_send ();
	return (EXIT_SUCCESS);
}

int rewrite_crk (char *oldsignaturefile, char *newsignaturefile)
{
	int ecdsa_len = ECDSA_MODULUS_LEN;
	int rsa_len = PAOLA_RSA_MODULUS_LEN;
	int rsa_len2 = PAOLA_RSA_MODULUS_LEN;
	int rsa_pubexplen = RSA_PUBLIC_EXPONENT_LEN;
	int err = UCL_OK;
	int mrk_signature_len;
	int mrk_signature2_len;
	if (SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_REWRITE_CRK]);
		return (EXIT_FAILURE);
	}
	if (SCP_PAOLA == session_mode)
	{
		err = read_file_signed_rsa_publickey (crk_rsa_modulus,
						      &rsa_len,
						      crk_rsa_pubexp,
						      rsa_pubexplen, mrk_signature_g, &mrk_signature_len, oldsignaturefile);
		err = read_file_signed_rsa_publickey
			(crk_rsa_modulus2, &rsa_len2, crk_rsa_pubexp2,
			 rsa_pubexplen, mrk_signature2, &mrk_signature2_len, newsignaturefile);
	}
	printf ("rsa_len=%d mrk len=%d\n", rsa_len, mrk_signature_len);

	if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = read_file_signed_ecdsa_publickey (crk_ecdsa_x,
							crk_ecdsa_y, mrk_ecdsa_r, mrk_ecdsa_s, ecdsa_len, oldsignaturefile);
		err = read_file_signed_ecdsa_publickey (crk_ecdsa_x2,
							crk_ecdsa_y2, mrk_ecdsa_r2, mrk_ecdsa_s2, ecdsa_len, newsignaturefile);
	}
	if (EXIT_SUCCESS != err)
	{
		printf ("ERROR in read_file_signature\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	if (SCP_PAOLA == session_mode)
	{
		rewrite_crk_payload_rsa (crk_rsa_modulus, rsa_len,
					 crk_rsa_pubexp,
					 rsa_pubexplen,
					 crk_rsa_modulus2, rsa_len2,
					 crk_rsa_pubexp2, rsa_pubexplen, mrk_signature2, mrk_signature2_len);
	}
	if (SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	if (SCP_ANGELA_ECDSA == session_mode)
	{
		rewrite_crk_payload_ecdsa (crk_ecdsa_x, crk_ecdsa_y,
					   crk_ecdsa_x2, crk_ecdsa_y2, mrk_ecdsa_r2, mrk_ecdsa_s2, ecdsa_len);
	}
	if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_REWRITE_CRK]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "rewrite_crk");
	packet_send ();
	return (EXIT_SUCCESS);
}

int aes_comp_payload (void)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the response */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 34=2 bytes for command id, 16 for aes key, 16 for aes data */
	payload[ipayload++] = 34;
	payload[ipayload++] = AES_COMP >> 8;
	payload[ipayload++] = AES_COMP & 255;
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		payload[ipayload++] = aes_key[i];
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		payload[ipayload++] = aes_data[i];
	return (EXIT_SUCCESS);
}

/* SCP and SCP FLORA shared command (named ERASE-DATA in FLORA) */
int del_mem (char *sfilename, char *ptr_address_offset)
{
	int err;
	int start_addr, end_addr;

	UNUSED_PARAMETER (ptr_address_offset);
	if (EXIT_SUCCESS == extension (".s19", sfilename))
		start_addr = get_start_addr_and_length_s19 (sfilename, &end_addr);
	else if (EXIT_SUCCESS == extension (".s20", sfilename))
		start_addr = get_start_addr_and_length_s20 (sfilename, &end_addr);
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .s19 and .s20)\n", sfilename);
		return (EXIT_FAILURE);
	}
	if (SBL == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	del_mem_payload (start_addr, end_addr);
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-DEL_MEM", idf_ctl[DATA_TRANSFER]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "del_mem");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP pcilinux bootloader command only */
int aes_comp (void)
{
	int err;
	if (SBL == session_mode || SCP_FLORA_AES == session_mode
	    || SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	aes_comp_payload ();
	if (SCP_RSA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-AES-COMP", idf_ctl[DATA_TRANSFER]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "aes_comp");
	packet_send ();
	return (EXIT_SUCCESS);
}

int write_mem_payload (u8 * data, int data_len, int data_addr)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the response */
	/*   tr_id=(tr_id+1)%256; */
	/*  size in on 2 bytes !! especially important for long frames */
	/*  made of 2 bytes for command id, 4 bytes for start @, 4 bytes for data len and then, the data themselves */
	payload[ipayload++] = (2 + 4 + 4 + data_len) >> 8;
	payload[ipayload++] = (2 + 4 + 4 + data_len) & 255;
	payload[ipayload++] = WRITE_MEM >> 8;
	payload[ipayload++] = WRITE_MEM & 255;
	/* set up the start addr */
	/*  on 4 bytes */
	for (i = 3; i >= 0; i--)
		payload[ipayload++] = (data_addr >> (8 * i)) & 255;
	/* set up the length */
	for (i = 3; i >= 0; i--)
	{
		payload[ipayload] = (data_len >> (8 * i)) & 255;
		ipayload++;
	}
	for (i = 0; i < data_len; i++)
		payload[ipayload++] = data[i];
	return (EXIT_SUCCESS);
}

/* SCP and SCP flora shared command (named WRITE-DATA for FLORA) */
int write_mem (u8 * data, int data_len, int data_addr)
{
	int err;
	if (SBL == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	write_mem_payload (data, data_len, data_addr);
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
		{
			printf ("ERROR: rsa-sign-payload\n");
			return (err);
		}
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-WRITE_MEM", idf_ctl[DATA_TRANSFER]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_mem");
	packet_send ();
	return (EXIT_SUCCESS);
}

int write_file (char *sfilename, char *ptr_address_offset)
{
	int i, j;
	u8 chunk[MAX_CHUNK_SIZE];
	int chunk_len;
	/* this variable is used to have a whole packet with a length of chunk_size */
	/* meaning the data-only payload shall be reduced in order to have the whole packet length=chunk_size */
	/* i.e. header+command+data+signature+crc=chunk_size */
	int new_chunk_size;
	int chunk_addr;
	int resu;
	/* write-file includes automatic erasure of concerned area; */
	/* for data write only, use write-file-only */
	del_mem (sfilename, ptr_address_offset);
	usip ();
	ack ();
	del_mem_response ();
	host ();
	ack ();
	if (EXIT_SUCCESS == extension (".s19", sfilename))
	{
		if (EXIT_SUCCESS != read_s19 (sfilename, ptr_address_offset, data_g))
			return (EXIT_FAILURE);
	}
	else if (EXIT_SUCCESS == extension (".s20", sfilename))
	{
		if (EXIT_SUCCESS != read_s20 (sfilename, ptr_address_offset, data_g))
			return (EXIT_FAILURE);
	}
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .s19 and .s20)\n", sfilename);
		return (EXIT_FAILURE);
	}
	/* 10 is the write-data command len: 2 bytes for the command, 4 bytes for the data length, 4 bytes for the data address */
	new_chunk_size = chunk_size - HEADER_LEN - CRC_LEN - COMMAND_LEN - 10;
	/* if the mode is RSA mode, the signature len shall also be taken into account */
	if (SCP_FLORA_RSA == session_mode)
		new_chunk_size -= SIGNATURE_LEN;
	if (SCP_PAOLA == session_mode)
		new_chunk_size -= PAOLA_SIGNATURE_LEN;
	if (TRUE == verbose)
		printf ("%d new chunk_size=%d\n", chunk_size, new_chunk_size);
	chunk_len = 0;
	i = 0;
	/* parse data */
	/* grouping them by contiguous addresses, up to CHUNK-SIZE */
	/* meaning each time an address is not the next one than the previous one */
	/* or each time a block of CHUNK-SIZE data has been built, */
	/* a write-mem is issued */
	/* until the end. */
	while (i < data_len_g)
	{
		chunk_len = 0;
		chunk[chunk_len] = data_g[i];
		/* the block starting address (needed for write-mem) */
		chunk_addr = addr_g[i];
		chunk_len++;
		i++;
		/* while consecutive addresses and not too big chunk */
		/* take into account the new_chunk_size */
		while ((addr_g[i] == addr_g[i - 1] + 1) && (chunk_len < new_chunk_size) && (i < data_len_g))
		{
			/* accumulate */
			chunk[chunk_len] = data_g[i];
			chunk_len++;
			i++;
		}
		/* the last packet is filled up with FF if needed */
		/* 2.3.6 (#2252): not filled up with FF anymore */
		if (i == data_len_g)
		{
			if (TRUE == verbose)
				printf ("last chunk (%d bytes):", chunk_len);
			/* 2.3.6 (#2252): not filled up with FF anymore */
			/*       new_chunk_len=((chunk_len/16)+1)*16; */
			/* for(j=chunk_len;j<new_chunk_len;j++) */
			/* chunk[j]=0xFF; */
			/* chunk_len=new_chunk_len; */
			if (TRUE == verbose)
				printf ("last chunk (%d bytes):", chunk_len);
			if (TRUE == verbose)
			{
				printf ("last:");
				for (j = 0; j < chunk_len; j++)
					printf ("%02x", chunk[j]);
				printf ("\n");
			}
		}
		resu = write_mem (chunk, chunk_len, chunk_addr);
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR in write_mem\n");
			return (EXIT_FAILURE);
		}
		usip ();
		ack ();
		write_mem_response ();
		host ();
		ack ();
	}
	if (TRUE == verbose)
	{
		for (i = 0; i < chunk_len; i++)
			printf ("%02x", chunk[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int write_only (char *sfilename, char *ptr_address_offset)
{
	int i;
	u8 chunk[MAX_CHUNK_SIZE];
	int chunk_len;
	int new_chunk_size;
	int chunk_addr;
	int resu;
	/*   del_mem(sfilename); */
	/*   usip(); */
	/*   ack(); */
	/*   del_mem_response(); */
	/*   host(); */
	/*   ack(); */
	if (EXIT_SUCCESS == extension (".s19", sfilename))
	{
		if (EXIT_SUCCESS != read_s19 (sfilename, ptr_address_offset, data_g))
			return (EXIT_FAILURE);
	}
	else if (EXIT_SUCCESS == extension (".s20", sfilename))
	{
		if (EXIT_SUCCESS != read_s20 (sfilename, ptr_address_offset, data_g))
			return (EXIT_FAILURE);
	}
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .s19 and .s20)\n", sfilename);
		return (EXIT_FAILURE);
	}
	/* 10 is the write-data command len: 2 byte for the command, 4 bytes for the data length, 4 bytes for the data address */
	new_chunk_size = chunk_size - HEADER_LEN - CRC_LEN - COMMAND_LEN - 10;
	/* if the mode is RSA mode, the signature len shall also be taken into account */
	if (SCP_FLORA_RSA == session_mode)
		new_chunk_size -= SIGNATURE_LEN;
	if (SCP_PAOLA == session_mode)
		new_chunk_size -= PAOLA_SIGNATURE_LEN;
	chunk_len = 0;
	i = 0;
	/* parse data */
	/* grouping them by contiguous addresses, up to CHUNK-SIZE */
	/* meaning each time an address is not the next one than the previous one */
	/* or each time a block of CHUNK-SIZE data has been built, */
	/* a write-mem is issued */
	/* until the end. */
	while (i < data_len_g)
	{
		chunk_len = 0;
		chunk[chunk_len] = data_g[i];
		/* the block starting address (needed for write-mem) */
		chunk_addr = addr_g[i];
		chunk_len++;
		i++;
		/* while consecutive addresses and not too big chunk */
		while ((addr_g[i] == addr_g[i - 1] + 1) && (chunk_len < new_chunk_size) && (i < data_len_g))
		{
			/* accumulate */
			chunk[chunk_len] = data_g[i];
			chunk_len++;
			i++;
		}
		/* the last packet is filled up with FF if needed */
		/* 2.3.6 (#2252): not filled up with FF anymore */
		if (i == data_len_g)
		{
			if (TRUE == verbose)
				printf ("last chunk (%d bytes):", chunk_len);
			/* 2.3.6 (#2252): not filled up with FF anymore */
			/*       new_chunk_len=((chunk_len/16)+1)*16; */
			/*       for(j=chunk_len;j<new_chunk_len;j++) */
			/* chunk[j]=0xFF; */
			/* chunk_len=new_chunk_len; */
			if (TRUE == verbose)
				printf ("last chunk (%d bytes):", chunk_len);
		}
		resu = write_mem (chunk, chunk_len, chunk_addr);
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR in write_mem\n");
			return (EXIT_FAILURE);
		}
		usip ();
		ack ();
		write_mem_response ();
		host ();
		ack ();
	}
	if (TRUE == verbose)
	{
		for (i = 0; i < chunk_len; i++)
			printf ("%02x", chunk[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int verify_data_payload (u8 * data, int data_len, int data_addr)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the response */
	/*   tr_id=(tr_id+1)%256; */
	/*  size in on 2 bytes !! especially important for long frames */
	payload[ipayload++] = (2 + 4 + 4 + data_len) >> 8;
	payload[ipayload++] = (2 + 4 + 4 + data_len) & 255;
	payload[ipayload++] = VERIFY_MEM >> 8;
	payload[ipayload++] = VERIFY_MEM & 255;
	/* set up the start addr */
	/*  on 4 bytes */
	for (i = 3; i >= 0; i--)
		payload[ipayload++] = (data_addr >> (8 * i)) & 255;
	/* set up the length */
	/*  on 4 bytes */
	for (i = 3; i >= 0; i--)
		payload[ipayload++] = (data_len >> (8 * i)) & 255;
	for (i = 0; i < data_len; i++)
		payload[ipayload++] = data[i];
	return (EXIT_SUCCESS);
}

/* SCP and SCP FLORA shared command (named COMPARE-DATA in FLORA) */
int verify_data (u8 * data, int data_len, int data_addr)
{
	int err;
	if (SBL == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	verify_data_payload (data, data_len, data_addr);
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-VERIFY-DATA", idf_ctl[DATA_TRANSFER]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "verify_data");
	packet_send ();
	return (EXIT_SUCCESS);
}

int verify_file (char *sfilename, char *ptr_address_offset, char b_dump)
{
	int i;
	u8 chunk[MAX_CHUNK_SIZE];
	int chunk_len;
	/* this variable is used to have a whole packet with a length of chunk_size */
	/* meaning the data-only payload shall be reduced in order to have the whole packet length=chunk_size */
	/* i.e. header+command+data+signature+crc=chunk_size */
	int new_chunk_size;
	int chunk_addr;
	if (EXIT_SUCCESS == extension (".s19", sfilename))
	{
		if (EXIT_SUCCESS != read_s19 (sfilename, ptr_address_offset, data_g))
			return (EXIT_FAILURE);
	}
	else if (EXIT_SUCCESS == extension (".s20", sfilename))
	{
		if (EXIT_SUCCESS != read_s20 (sfilename, ptr_address_offset, data_g))
			return (EXIT_FAILURE);
	}
	else
	{
		printf ("ERROR: <%s> file extension not supported (only .s19 and .s20)\n", sfilename);
		return (EXIT_FAILURE);
	}
	/* 10 is the write-data command len: 2 bytes for the command, 4 bytes for the data length, 4 bytes for the data address */
	new_chunk_size = chunk_size - HEADER_LEN - CRC_LEN - COMMAND_LEN - 10;
	/* if the mode is RSA mode, the signature len shall also be taken into account */
	if (SCP_FLORA_RSA == session_mode)
		new_chunk_size -= SIGNATURE_LEN;
	if (SCP_PAOLA == session_mode)
		new_chunk_size -= PAOLA_SIGNATURE_LEN;
	if (TRUE == verbose)
		printf ("%d new chunk_size=%d\n", chunk_size, new_chunk_size);

	chunk_len = 0;
	i = 0;
	/* parse data */
	/* grouping them by contiguous addresses, up to CHUNK-SIZE */
	/* meaning each time an address is not the next one than the previous one */
	/* or each time a block of CHUNK-SIZE data has been built, */
	/* a write-mem is issued */
	/* until the end. */
	while (i < data_len_g)
	{
		chunk[chunk_len] = data_g[i];
		/* the block starting address (needed for write-mem) */
		chunk_addr = addr_g[i];
		chunk_len++;
		i++;
		/* while consecutive addresses and not too big chunk */
		/* take into account the new_chunk_size */
		while ((addr_g[i] == addr_g[i - 1] + 1) && (chunk_len < new_chunk_size) && (i < data_len_g))
		{
			chunk[chunk_len] = data_g[i];
			chunk_len++;
			i++;
		}
		/* the last packet is filled up with FF if needed */
		/* 2.3.6 (#2252): not filled up with FF anymore */
		if (i == data_len_g)
		{
			if (TRUE == verbose)
				printf ("last chunk (%d bytes):", chunk_len);
			/* 2.3.6 (#2252): not filled up with FF anymore */
			/*       new_chunk_len=((chunk_len/16)+1)*16; */
			/*       for(j=chunk_len;j<new_chunk_len;j++) */
			/*         chunk[j]=0xFF; */
			/*       chunk_len=new_chunk_len; */
			if (TRUE == verbose)
				printf ("last chunk (%d bytes):", chunk_len);
		}

		verify_data (chunk, chunk_len, chunk_addr);
		chunk_len = 0;
		usip ();
		ack ();
		if (b_dump == TRUE)
			dump ();
		verify_data_response ();
		host ();
		ack ();
	}
	return (EXIT_SUCCESS);
}

/* SCP and SCP FLORA shared command (named ERASE-DATA in FLORA) */
/* this function issues a command for erasing an area defined by */
/* input strings, sstart_addr represents the area starting address, in hexa */
/* and slength represents the length (in bytes) of the area */
/* this function is directly used for the script command ERASE-DATA */
int del_data (char *sstart_addr, char *slength)
{
	unsigned int length;
	int err;
	unsigned int start_addr, end_addr;

	if (SBL == session_mode)
	{
		printf ("ERROR: command not supported\n");
		return (EXIT_FAILURE);
	}
	if (0 == sscanf (sstart_addr, "%x", &start_addr))
	{
		printf ("ERROR: sstart_addr parameter <%s> incorrectly formatted as a number\n", sstart_addr);
		return (EXIT_FAILURE);
	}
	if (0 == sscanf (slength, "%x", &length))
	{
		printf ("ERROR: slength parameter <%s> incorrectly formatted as a number\n", slength);
		return (EXIT_FAILURE);
	}

	end_addr = start_addr + length;
	if (TRUE == verbose)
		printf ("%s -> %x %s -> %x: end@:%x\n", sstart_addr, start_addr, slength, length, end_addr);
	ipayload = 0;
	seq++;
	del_mem_payload (start_addr, end_addr);
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-DEL_MEM", idf_ctl[DATA_TRANSFER]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "del_data");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP command for blpk key writing */
int write_bpk_blpk_payload (char *bpk_char)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 18=2 bytes for command id and 16 for aes key */
	payload[ipayload++] = 18;
	payload[ipayload++] = WRITE_BLPK >> 8;
	payload[ipayload++] = WRITE_BLPK & 255;
	if (TRUE == verbose)
	{
		printf ("BLPK:");
		for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
			printf ("%02x", hex (bpk_char[i * 2], bpk_char[i * 2 + 1]));
		printf ("\n");
	}
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		payload[ipayload++] = hex (bpk_char[i * 2], bpk_char[i * 2 + 1]);
	return (EXIT_SUCCESS);
}

/* SCP pcilinux bootloader command only */
/* bug #1622 correction: the correc key was not used */
int write_bpk_blpk (char *bpk_char)
{
	int err;
	if (SBL == session_mode || SCP_FLORA_AES == session_mode
	    || SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_WRITE_BLPK]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	write_bpk_blpk_payload (bpk_char);
	if (SCP_RSA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_WRITE_BLPK]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_blpk");
	packet_send ();
	return (EXIT_SUCCESS);
}

int write_bpk_fak_payload (char *fak_char)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 18=2 bytes for command id and 16 for aes key */
	payload[ipayload++] = 18;
	payload[ipayload++] = WRITE_FAK >> 8;
	payload[ipayload++] = WRITE_FAK & 255;
	if (TRUE == verbose)
	{
		printf ("FAK:");
		for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
			printf ("%02x", hex (fak_char[i * 2], fak_char[i * 2 + 1]));
		printf ("\n");
	}
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
		payload[ipayload++] = hex (fak_char[i * 2], fak_char[i * 2 + 1]);
	return (EXIT_SUCCESS);
}

/* SCP pcilinux bootloader command only */
/* bug #1622 correction: the correc key was not used */
int write_bpk_fak (char *fak_char)
{
	int err;
	if (SBL == session_mode || SCP_FLORA_AES == session_mode
	    || SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_WRITE_FAK]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	write_bpk_fak_payload (fak_char);
	if (SCP_RSA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_WRITE_FAK]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_fak");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_timeout_payload (char timeout_target_char, char *timeout_char)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id, 1 byte for target, 2 bytes for timeout value */
	/* +1 corrected in 2.3.4 */
	payload[ipayload++] = 2 + 1 + 2;
	payload[ipayload++] = WRITE_TIMEOUT >> 8;
	payload[ipayload++] = WRITE_TIMEOUT & 255;
	if (CHAR_TIMEOUT_UART_TARGET == timeout_target_char)
		payload[ipayload++] = TIMEOUT_UART_TARGET;
	else if (CHAR_TIMEOUT_USB_TARGET == timeout_target_char)
		payload[ipayload++] = TIMEOUT_USB_TARGET;
	else if (CHAR_TIMEOUT_VBUS_TARGET == timeout_target_char)
		payload[ipayload++] = TIMEOUT_VBUS_TARGET;
	payload[ipayload++] = hex (timeout_char[0], timeout_char[1]);
	payload[ipayload++] = hex (timeout_char[2], timeout_char[3]);
	if (TRUE == verbose)
	{
		printf ("timeout:");
		for (i = 0; i < (int) strlen (timeout_char); i++)
			printf ("%c", timeout_char[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_timeout (char timeout_target_char, char *timeout_char)
{
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_ANGELA_ECDSA != session_mode
	    && SCP_FLORA_AES != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_WRITE_TIMEOUT]);
		return (EXIT_FAILURE);
	}
	if (timeout_target_char != CHAR_TIMEOUT_UART_TARGET
	    && timeout_target_char != CHAR_TIMEOUT_USB_TARGET && timeout_target_char != CHAR_TIMEOUT_VBUS_TARGET)
	{
		printf ("ERROR: not allowed target: %c; only %c, %c and %c are allowed\n", timeout_target_char,
			CHAR_TIMEOUT_UART_TARGET, CHAR_TIMEOUT_USB_TARGET, CHAR_TIMEOUT_VBUS_TARGET);
		return (EXIT_FAILURE);
	}
	if (strlen (timeout_char) != 4)
	{
		printf ("ERROR: timeout is not 2-byte long\n");
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	write_timeout_payload (timeout_target_char, timeout_char);
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_WRITE_TIMEOUT]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_timeout");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int kill_chip_payload (void)
{
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id */
	payload[ipayload++] = 2;
	payload[ipayload++] = KILL_CHIP >> 8;
	payload[ipayload++] = KILL_CHIP & 255;
	return (EXIT_SUCCESS);
}

/*  MAX32650 command */
int kill_chip2_payload (void)
{
	int i = 0;

	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id */
	payload[ipayload++] = 2 + config_struct.usn_len;
	payload[ipayload++] = KILL_CHIP2 >> 8;
	payload[ipayload++] = KILL_CHIP2 & 255;
	for (i = 0; i < config_struct.usn_len; i++)
	{
		payload[ipayload++] = config_struct.usn[i];
	}
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int kill_chip (void)
{
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_FLORA_AES != session_mode
	    && SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_KILL_CHIP]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	kill_chip_payload ();
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_KILL_CHIP]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "kill_chip");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int kill_chip2 (void)
{
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_FLORA_AES != session_mode
	    && SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_KILL_CHIP2]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	kill_chip2_payload ();
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_KILL_CHIP2]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "kill_chip2");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_bpk_payload (char *data_char, char *offset_char)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id, 2 bytes for data len, 2 bytes for offset and data value len */
	/* data value is a hexa string so, its data len is string len/2 */
	payload[ipayload++] = 2 + 2 + 2 + strlen (data_char) / 2;
	payload[ipayload++] = WRITE_BPK >> 8;
	payload[ipayload++] = WRITE_BPK & 255;
	payload[ipayload++] = (2 + strlen (data_char) / 2) >> 8;
	payload[ipayload++] = (2 + strlen (data_char) / 2) & 255;
	payload[ipayload++] = hex (offset_char[0], offset_char[1]);
	payload[ipayload++] = hex (offset_char[2], offset_char[3]);
	if (TRUE == verbose)
	{
		printf ("data:");
		for (i = 0; i < (int) strlen (data_char); i++)
			printf ("%c", data_char[i]);
		printf ("\n");
		printf ("offset:");
		for (i = 0; i < (int) strlen (offset_char); i++)
			printf ("%c", offset_char[i]);
		printf ("\n");
	}
	for (i = 0; i < (int) strlen (data_char) / 2; i++)
		payload[ipayload++] = hex (data_char[i * 2], data_char[i * 2 + 1]);
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_bpk (char *offset_char, char *data_char)
{
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_FLORA_AES != session_mode
	    && SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_WRITE_BPK]);
		return (EXIT_FAILURE);
	}
	if ((strlen (data_char) % 2) != 0)
	{
		printf ("ERROR: data are not byte-aligned\n");
		return (EXIT_FAILURE);
	}
	if (strlen (offset_char) != 4)
	{
		printf ("ERROR: offset is not 2-byte long (%d)\n", (int) strlen (offset_char));
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	write_bpk_payload (data_char, offset_char);
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_WRITE_BPK]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_bpk");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int execute_code_payload (char *address)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: address value len */
	/* address value is a hexa string so, its data len is string len/2 */
	/* #bug 2202: missing 2 bytes in payload length */
	payload[ipayload++] = (strlen (address) / 2) + 2;
	payload[ipayload++] = EXECUTE_CODE >> 8;
	payload[ipayload++] = EXECUTE_CODE & 255;
	/* #2203: no data length parameter in the execute-code command */
	/*   payload[ipayload++]=(strlen(address)/2)>>8; */
	/*   payload[ipayload++]=(strlen(address)/2)&255; */
	if (TRUE == verbose)
	{
		printf ("address:");
		for (i = 0; i < (int) strlen (address); i++)
			printf ("%c", address[i]);
		printf ("\n");
	}
	for (i = 0; i < (int) strlen (address) / 2; i++)
		payload[ipayload++] = hex (address[i * 2], address[i * 2 + 1]);
	return (EXIT_SUCCESS);
}

int execute_code (char *address)
{
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_FLORA_AES != session_mode
	    && SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_EXECUTE_CODE]);
		return (EXIT_FAILURE);
	}
	if ((strlen (address) % 2) != 0)
	{
		printf ("ERROR: address is not byte-aligned\n");
		return (EXIT_FAILURE);
	}
	if (strlen (address) != 8)
	{
		printf ("ERROR: address is not 4-byte long (%d)\n", (int) strlen (address));
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	execute_code_payload (address);
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_EXECUTE_CODE]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "execute_code");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_otp_payload (char *data_char, char *offset_char)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the payload */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id, 2 bytes for data len, 2 bytes for offset and data value len */
	/* data value is a hexa string so, its data len is string len/2 */
	payload[ipayload++] = 2 + 2 + 2 + strlen (data_char) / 2;
	payload[ipayload++] = WRITE_OTP >> 8;
	payload[ipayload++] = WRITE_OTP & 255;
	payload[ipayload++] = (2 + strlen (data_char) / 2) >> 8;
	payload[ipayload++] = (2 + strlen (data_char) / 2) & 255;
	payload[ipayload++] = hex (offset_char[0], offset_char[1]);
	payload[ipayload++] = hex (offset_char[2], offset_char[3]);
	if (TRUE == verbose)
	{
		printf ("data:");
		for (i = 0; i < (int) strlen (data_char); i++)
			printf ("%c", data_char[i]);
		printf ("\n");
		printf ("offset:");
		for (i = 0; i < (int) strlen (offset_char); i++)
			printf ("%c", offset_char[i]);
		printf ("\n");
	}
	for (i = 0; i < (int) strlen (data_char) / 2; i++)
		payload[ipayload++] = hex (data_char[i * 2], data_char[i * 2 + 1]);
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_otp (char *offset_char, char *data_char)
{
	int err;
	if (SCP_FLORA_RSA != session_mode && SCP_FLORA_AES != session_mode
	    && SCP_ANGELA_ECDSA != session_mode && SCP_PAOLA != session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_WRITE_OTP]);
		return (EXIT_FAILURE);
	}
	if ((strlen (data_char) % 2) != 0)
	{
		printf ("ERROR: data are not byte-aligned\n");
		return (EXIT_FAILURE);
	}
	if (strlen (offset_char) != 4)
	{
		printf ("ERROR: offset <%s> is not 2-byte long (%d)\n", offset_char, (int) strlen (offset_char));
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	write_otp_payload (data_char, offset_char);
	if (SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		err = ecdsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_WRITE_OTP]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "write_otp");
	packet_send ();
	return (EXIT_SUCCESS);
}

int write_bpk_blpk_response (void)
{
	generic_response ("write_bpk_blpk_response");
	return (EXIT_SUCCESS);
}

int write_bpk_fak_response (void)
{
	generic_response ("write_bpk_fak_response");
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_bpk_response (void)
{
	generic_response ("write_bpk_response");
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_otp_response (void)
{
	generic_response ("write_otp_response");
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_crk_response (void)
{
	generic_response ("write_crk_response");
	return (EXIT_SUCCESS);
}

int rewrite_crk_response (void)
{
	generic_response ("rewrite_crk_response");
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int write_timeout_response (void)
{
	generic_response ("write_timeout_response");
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int kill_chip_response (void)
{
	generic_response ("kill_chip_response");
	return (EXIT_SUCCESS);
}

/* SCP FLORA command */
int execute_code_response (void)
{
	generic_response ("execute_code_response");
	return (EXIT_SUCCESS);
}

int del_mem_response (void)
{
	generic_response ("del_mem_response");
	return (EXIT_SUCCESS);
}

int verify_data_response (void)
{
	generic_response ("verify_data_response");
	return (EXIT_SUCCESS);
}

int write_mem_response (void)
{
	generic_response ("write_mem_response");
	return (EXIT_SUCCESS);
}

int del_mem_payload (int start_addr, int end_addr)
{
	int i;
	int range;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the response */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id, 4 bytes for start add, 2 bytes for length */
	payload[ipayload++] = 10;
	payload[ipayload++] = ERASE_MEM >> 8;
	payload[ipayload++] = ERASE_MEM & 255;
	/* set up the start addr */
	/*  on 4 bytes */
	for (i = 3; i >= 0; i--)
		payload[ipayload++] = (start_addr >> (8 * i)) & 255;
	/* set up the length */
	/*  on 4 bytes */
	range = (end_addr - start_addr);
	if (TRUE == verbose)
		printf ("payload for erase is: %d bytes\n", range);
	/* need to be multiple of 16 */
	/* #bug 2982 */
	if ((range%16)!=0){
	
		range = ((range / 16) + 1) * 16;
	}
	if (TRUE == verbose)
		printf ("payload for erase is: %d bytes\n", range);
	for (i = 3; i >= 0; i--)
		payload[ipayload++] = ((range) >> (8 * i)) & 255;
	return (EXIT_SUCCESS);
}

/* le mapping renvoy\E9 est bidon */
int mem_mapping_response_payload (void)
{
	int i;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	tr_id = (tr_id + 1) % 256;
	/*  on la met ou non ?? */
	/* on met une size sur 4 bytes, en valeur 0x00-00-00-02 */
	payload[ipayload++] = 00;
	payload[ipayload++] = 00;
	payload[ipayload++] = 00;
	/* seulement 2 de longueur */
	payload[ipayload++] = 0x02;
	for (i = 0; i < 2; i++)
		payload[ipayload++] = i;
	/* error code, on 4 bytes */
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	return (EXIT_SUCCESS);
}

int mem_mapping_response (void)
{
	if (SBL == session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_READ_MEMORY_MAPPING]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	mem_mapping_response_payload ();
	if (SCP_RSA == session_mode)
	{
		/* nothing: no signature on response */
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], "mem_mapping_response");
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "mem_mapping_response");
	packet_send ();
	return (EXIT_SUCCESS);
}

int mem_mapping_payload (void)
{
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the response is incremented after the response */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id */
	payload[ipayload++] = 2;
	payload[ipayload++] = MEM_MAPPING >> 8;
	payload[ipayload++] = MEM_MAPPING & 255;
	return (EXIT_SUCCESS);
}

/* SCP only */
int mem_mapping (void)
{
	int err;
	if (SBL == session_mode || SCP_FLORA_AES == session_mode
	    || SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_READ_MEMORY_MAPPING]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	mem_mapping_payload ();
	if (SCP_RSA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-MEM-MAPPING", idf_ctl[DATA_TRANSFER]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "mem_mapping");
	packet_send ();
	return (EXIT_SUCCESS);
}

int read_configuration_payload (void)
{
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	/* the tr-id is incremented after the response */
	/*   tr_id=(tr_id+1)%256; */
	payload[ipayload++] = 0;
	/* command length: 2 bytes for command id */
	payload[ipayload++] = 2;
	payload[ipayload++] = READ_CONFIGURATION >> 8;
	payload[ipayload++] = READ_CONFIGURATION & 255;
	return (EXIT_SUCCESS);
}

/* same for SCP and SCP FLORA */
int read_configuration (void)
{
	int err;
	if (SBL == session_mode || SCP_FLORA_AES == session_mode
	    || SCP_FLORA_RSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_PAOLA == session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_READ_CONFIGURATION]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	read_configuration_payload ();
	if (SCP_RSA == session_mode)
	{
		err = rsa_sign_payload ();
		if (EXIT_FAILURE == err)
			return (err);
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], idf_scp_cmd[COMMAND_READ_CONFIGURATION]);
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "read_configuration");
	packet_send ();
	return (EXIT_SUCCESS);
}

int read_configuration_response_payload (void)
{
	int i;
	int start;
	int end;
	start = 0;
	end = SECTOR_SIZE;
	payload[ipayload++] = (DATA << 4) ^ config_struct.pp;
	payload[ipayload++] = tr_id;
	tr_id = (tr_id + 1) % 256;
	/*  on la met ou non ?? */
	/*   payload[ipayload++]=size>>8; */
	/*   payload[ipayload++]=size&255; */
	for (i = start; i < end; i++)
		payload[ipayload++] = (u8) ((i & 15));
	/* error code, on 4 bytes */
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	payload[ipayload++] = 0;
	return (EXIT_SUCCESS);
}

int read_configuration_response (void)
{
	if (SBL == session_mode)
	{
		printf ("ERROR: command <%s> not supported\n", idf_scp_cmd[COMMAND_READ_CONFIGURATION]);
		return (EXIT_FAILURE);
	}
	ipayload = 0;
	seq++;
	read_configuration_response_payload ();
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode)
	{
		/* nothing: no signature on response */
	}
	else
	{
		if (SCP_PP_E_CMAC == config_struct.pp)
		{
			/* encrypt plain payload */
			aes_encrypt_payload ();
			if (TRUE == verbose)
				printf ("payload encrypted\n");
		}
		if (TRUE == verbose)
			printf ("payload signed\n");
		/* cmac performed on encrypted payload */
		aes_cmac_payload ();
	}
	sprintf (message, "%s-%s", idf_ctl[DATA_TRANSFER], "READ-CONFIG-RESPONSE");
	synchro ();
	data_code ();
	/* add payload size to header */
	add_byte (ipayload >> 8);
	add_byte (ipayload & 255);
	add_channel_id_seq ();
	/* compute header crc (shall be done after payload size be known) */
	header_crc ();
	add_payload ();
	/* finally add payload crc */
	payload_crc ();
	sprintf (name_file, "read_config_response");
	packet_send ();
	return (EXIT_SUCCESS);
}

/* this function processes the commands listed in the script file "script_file" */
/* the AES or RSA mode is managed out of the commands list */
/* these commands are closed to the SCP commands, but some include some abstraction */
/* e.g. the write-file, which writes the whole file, instead of write-data, which writes only one line */
/* the session opening is out of the script commands process */
int process_script (void)
{
	char line[MAX_STRING];
	int command;
	FILE *fpscript;
	if (SCP_RSA == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-RSA>\n");
		fprintf (fp_g, "<session SCP-RSA>\n");
	}
	if (SCP_FLORA_RSA == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-FLORA-RSA>\n");
		fprintf (fp_g, "<session SCP-FLORA-RSA>\n");
	}
	if (SCP_ANGELA_ECDSA == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-ANGELA-ECDSA>\n");
		fprintf (fp_g, "<session SCP-ANGELA-ECDSA>\n");
	}
	if (SCP_PAOLA == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-PAOLA-RSA>\n");
		fprintf (fp_g, "<session SCP-PAOLA-RSA>\n");
	}
	if (SCP_FLORA_AES == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-FLORA-AES>\n");
		fprintf (fp_g, "<session SCP-FLORA-AES>\n");
	}
	if (SCP_ON_AES == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-AES>\n");
		fprintf (fp_g, "<session SCP-AES>\n");
	}
	fpscript = fopen (script_file, "r");
	if (NULL == fpscript)
	{
		printf ("ERROR: impossible to open <%s>\n", script_file);
		return (EXIT_FAILURE);
	}
	/* initialize seq & ch id */
	iframe = 0;
	seq = 0;
	ch_id = 9;
	/* bug #1623 correction */
	tr_id = 0;
	host ();
	connection_request ();
	usip ();
	connection_reply ();
	host ();
	ack ();
	host ();
	hello_request ();
	usip ();
	ack ();
	hello_reply ();
	host ();
	ack ();
	/* if in AES mode, the challenge/response is performed */
	/* note that we are in fixed-random-number mode */
	if (SCP_ON_AES == session_mode || SCP_FLORA_AES == session_mode)
	{
		challenge ();
		usip ();
		ack ();
		challenge_response ();
		host ();
		ack ();
	}
	while (fgets (line, MAX_STRING, fpscript) != NULL)
	{
		if (TRUE == verbose)
			printf ("%s", line);
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
		case COMMAND_WRITE_CONFIGURATION:
			printf ("this command is not implemented\n");
			break;
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
			/* write-file is an abstraction for write-data+erase-data, used in scp and scp-flora */
		case COMMAND_WRITE_FILE:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != write_file (params[0], NULL))
				{
					printf ("ERROR: write-file\n");
					return (EXIT_FAILURE);
				}
			}
			else if (1 == nb_params)
			{
				if (EXIT_SUCCESS != write_file (params[0], params[1]))
				{
					printf ("ERROR: write-file\n");
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-FILE command: write-file <s19file> <address-offset:optional>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* write-only-file is an abstraction for write-data, used in scp and scp-flora */
		case COMMAND_WRITE_ONLY:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != write_only (params[0], NULL))
				{
					printf ("ERROR: write-only\n");
					return (EXIT_FAILURE);
				}
			}
			else if (2 == nb_params)
			{
				if (EXIT_SUCCESS != write_only (params[0], params[1]))
				{
					printf ("ERROR: write-only\n");
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-ONLY command: write-file-only <s19file> <address-offset:optional>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* ERASE_DATA is common to SCP and SCP-FLORA */
		case COMMAND_ERASE_DATA:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != del_data (params[0], params[1]))
				{
					printf ("ERROR: erase-file\n");
					return (EXIT_FAILURE);
				}
				/*  #bug 2130: missing packets added */
				usip ();
				ack ();
				del_mem_response ();
				host ();
				ack ();
			}
			else if (3 == nb_params)
				/*  Version with dump */
			{
				if (EXIT_SUCCESS != del_data (params[0], params[1]))
				{
					printf ("ERROR: erase-file\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				dump ();
				del_mem_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for ERASE-FILE command: erase-file <hex-start-addr> <hex-length>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* VERIFY-FILE is an abstraction for VERIFY-DATA in SCP, renamed in COMPARE-DATA in SCP-FLORA */
		case COMMAND_VERIFY_FILE:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != verify_file (params[0], NULL, FALSE))
				{
					return (EXIT_FAILURE);
				}
			}
			else if (2 == nb_params)
			{
				if (EXIT_SUCCESS != verify_file (params[0], params[1], FALSE))
				{
					return (EXIT_FAILURE);
				}
			}
			else if (3 == nb_params)
			{
				if (EXIT_SUCCESS != verify_file (params[0], params[1], TRUE))
				{
					return (EXIT_FAILURE);
				}
			}
			else
			{
				printf ("ERROR: incorrect format for VERIFY-FILE command: verify-file <s19file> <address-offset:optional> <dump:optional>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* command for SCP only */
		case COMMAND_WRITE_BLPK:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != write_bpk_blpk (params[0]))
				{
					printf ("ERROR: write-bpk-blpk\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				write_bpk_blpk_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-BLPK command: write-blpk <hex-value> <dump:optional>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* command for SCP only */
		case COMMAND_WRITE_FAK:
			if (1 == nb_params)
			{
				if (EXIT_SUCCESS != write_bpk_fak (params[0]))
				{
					printf ("ERROR: write-bpk-fak\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				write_bpk_fak_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-FAK command: write-fak <hex-value>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* command for SCP only */
		case COMMAND_READ_CONFIGURATION:
			if (EXIT_SUCCESS != read_configuration ())
			{
				printf ("ERROR: read-configuration\n");
				return (EXIT_FAILURE);
			}
			usip ();
			ack ();
			read_configuration_response ();
			host ();
			ack ();
			break;
			/* command for SCP only */
		case COMMAND_READ_MEMORY_MAPPING:
			if (EXIT_SUCCESS != mem_mapping ())
			{
				printf ("ERROR: mem-mapping\n");
				return (EXIT_FAILURE);
			}
			usip ();
			ack ();
			mem_mapping_response ();
			host ();
			ack ();
			break;
			/* SCP FLORA specific command */
		case COMMAND_WRITE_BPK:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != write_bpk (params[0], params[1]))
				{
					printf ("ERROR: write-bpk\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				write_bpk_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-BPK command: write-bpk <hex-value> <offset in secure RAM>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* SCP FLORA/ANGELA specific command */
		case COMMAND_WRITE_CRK:
			if (1 == nb_params)
			{
				/* params[0] is the filename containing the RSA PubKey */
				if (EXIT_SUCCESS != write_crk (params[0]))
				{
					printf ("ERROR: write-crk\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				write_crk_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-CRK command: write-crk <public key file name>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* SCP FLORA/ANGELA specific command */
		case COMMAND_REWRITE_CRK:
			if (2 == nb_params)
			{
				/* params[0] is the filename containing the old PubKey */
				/* params[1] is the filename containing the new PubKey */
				if (EXIT_SUCCESS != rewrite_crk (params[0], params[1]))
				{
					printf ("ERROR: rewrite-crk\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				rewrite_crk_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for REWRITE-CRK command: rewrite-crk <ECDSA public key file name>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* SCP FLORA/ANGELA specific command */
		case COMMAND_ECHO:
			if (EXIT_SUCCESS != echo_req ())
			{
				printf ("ERROR: echo\n");
				return (EXIT_FAILURE);
			}
			usip ();
			/* ack(); */
			echo_reply ();
			host ();
			/* ack(); */
			break;
		case COMMAND_WRITE_OTP:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != write_otp (params[0], params[1]))
				{
					printf ("ERROR: write-otp\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				write_otp_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-OTP command: write-otp <offset in customer OTP> <hex-value>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* with 2 parameters, SCP FLORA/ANGELA specific command */
		case COMMAND_WRITE_TIMEOUT:
			if (2 == nb_params)
			{
				if (EXIT_SUCCESS != write_timeout (params[0][0], params[1]))
				{
					printf ("ERROR: write-timeout\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				write_timeout_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for WRITE-TIMEOUT command: write-timeout <0:UART|1:USB> <hex-timeout-value-ms>\n");
				return (EXIT_FAILURE);
			}
			break;
			/* SCP FLORA/ANGELA specific command */
		case COMMAND_KILL_CHIP2:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != kill_chip2 ())
				{
					printf ("ERROR: kill-chip2\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				kill_chip_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for KILL-CHIP2 command: kill-chip2 w/o params\n");
				return (EXIT_FAILURE);
			}
			break;
		case COMMAND_KILL_CHIP:
			if (0 == nb_params)
			{
				if (EXIT_SUCCESS != kill_chip ())
				{
					printf ("ERROR: kill-chip\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				kill_chip_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for KILL-CHIP command: kill-chip w/o params\n");
				return (EXIT_FAILURE);
			}
			break;
			/* SCP FLORA/ANGELA specific command */
		case COMMAND_EXECUTE_CODE:
			if (1 == nb_params)
			{
				/* params[0] is the address where the plugin starts */
				if (EXIT_SUCCESS != execute_code (params[0]))
				{
					printf ("ERROR: execute-code\n");
					return (EXIT_FAILURE);
				}
				usip ();
				ack ();
				execute_code_response ();
				host ();
				ack ();
			}
			else
			{
				printf ("ERROR: incorrect format for EXECUTE-CODE command: execute-code <32-bit address>\n");
				return (EXIT_FAILURE);
			}
			break;

		default:
			printf ("ERROR: the command <%s> is not supported\n", line);
			return (EXIT_FAILURE);
		}
	}
	(void) fclose (fpscript);
	disconnection_request ();
	usip ();
	disconnection_reply ();

	return (EXIT_SUCCESS);
}
