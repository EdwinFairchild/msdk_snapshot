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
#include <ucl/ucl_info.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_sys.h>

#ifdef _MAXIM_HSM
#include <libhsm/HSM.h>
#endif /* _MAXIM_HSM */

#include "scp_definitions.h"
#include "session_build.h"

#include "process.h"
#include "scp.h"
#include "scp_lite.h"
#include "scp_maxq1852.h"
#include "ecdsa.h"
#include "rsa.h"

u32 init_buffer[2048];

int mode[MAX_SCP_COMMAND];
int list_ctl[MAX_IDF];

char idf_pp[MAX_IDF][MAX_STRING];
int list_pp[MAX_IDF];

u8 random_chlg[UCL_AES_BLOCKSIZE];

char session_string[MAX_STRING];
char output_name[MAX_STRING];
char s19file[MAX_STRING];
char rsafile[MAX_STRING];
char ecdsafile[MAX_STRING];
u8 blpk[UCL_AES_BLOCKSIZE];
u8 fak[UCL_AES_BLOCKSIZE];

void define_ctl (void)
{
	int i;
	for (i = 0; i < MAX_IDF; i++)
		list_ctl[i] = 0;
	list_ctl[1] = list_ctl[2] = list_ctl[3] = list_ctl[4] = list_ctl[5] =
		list_ctl[6] = list_ctl[0xB] = list_ctl[0xC] = list_ctl[7] =
		list_ctl[0x17] = list_ctl[0x27] = list_ctl[0x37] =
		list_ctl[0x47] = list_ctl[0x57] = list_ctl[0x67] =
		list_ctl[0x77] = list_ctl[0x87] = list_ctl[0x08] = list_ctl[0x09] = 1;
	strcpy (idf_ctl[1], "CON_REQ");
	strcpy (idf_ctl[2], "CON_REP");
	strcpy (idf_ctl[0x09], "CON_REF");
	strcpy (idf_ctl[3], "DISC_REQ");
	strcpy (idf_ctl[4], "DISC_REP");
	strcpy (idf_ctl[DATA_TRANSFER], "DATA_TRANSFER");
	strcpy (idf_ctl[6], "ACK");
	strcpy (idf_ctl[0xB], "ECHO_REQ");
	strcpy (idf_ctl[0xC], "ECHO_REP");
	strcpy (idf_ctl[7], "CHG_SP_0");
	strcpy (idf_ctl[0x17], "CHG_SP_1");
	strcpy (idf_ctl[0x27], "CHG_SP_2");
	strcpy (idf_ctl[0x37], "CHG_SP_3");
	strcpy (idf_ctl[0x47], "CHG_SP_4");
	strcpy (idf_ctl[0x57], "CHG_SP_5");
	strcpy (idf_ctl[0x67], "CHG_SP_6");
	strcpy (idf_ctl[0x77], "CHG_SP_7");
	strcpy (idf_ctl[0x87], "CHG_SP_8");
	strcpy (idf_ctl[8], "CHG_SP_REP");
}

void define_scp_cmd (void)
{
	strcpy (idf_scp_cmd[COMMAND_UNKNOWN], "unknown-command");
	strcpy (idf_scp_cmd[COMMAND_HELP], "help");
	strcpy (idf_scp_cmd[COMMAND_WRITE_FILE], "write-file");
	strcpy (idf_scp_cmd[COMMAND_WRITE_ONLY], "write-only");
	strcpy (idf_scp_cmd[COMMAND_ERASE_DATA], "erase-data");
	strcpy (idf_scp_cmd[COMMAND_VERIFY_FILE], "verify-file");
	strcpy (idf_scp_cmd[COMMAND_WRITE_BLPK], "write-blpk");
	strcpy (idf_scp_cmd[COMMAND_WRITE_FAK], "write-fak");
	strcpy (idf_scp_cmd[COMMAND_READ_CONFIGURATION], "read-configuration");
	strcpy (idf_scp_cmd[COMMAND_READ_MEMORY_MAPPING], "read-memory-mapping");
	strcpy (idf_scp_cmd[COMMAND_WRITE_CRK], "write-crk");
	strcpy (idf_scp_cmd[COMMAND_REWRITE_CRK], "renew-crk");
	strcpy (idf_scp_cmd[COMMAND_WRITE_BPK], "write-bpk");
	strcpy (idf_scp_cmd[COMMAND_WRITE_OTP], "write-otp");
	strcpy (idf_scp_cmd[COMMAND_ECHO], "echo");
	strcpy (idf_scp_cmd[COMMAND_WRITE_TIMEOUT], "write-timeout");
	strcpy (idf_scp_cmd[COMMAND_KILL_CHIP], "kill-chip");
	strcpy (idf_scp_cmd[COMMAND_KILL_CHIP2], "kill-chip2");
	strcpy (idf_scp_cmd[COMMAND_EXECUTE_CODE], "execute-code");
	strcpy (idf_scp_cmd[COMMAND_WRITE_CONFIGURATION], "write-configuration");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_LOAD_CUSTOMER_KEY], "load-customer-key");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_ERASE_CODE_FLASH_AREA], "erase-code-flash-area");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_ERASE_ALL_FLASH_AREAS], "erase-all-flash-areas");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_LOAD_CODE], "load-code");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_LOAD_FILE], "load-file");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_FILE], "verify-1852-file");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_LOAD_DATA], "load-data");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_CODE], "verify-code");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_DATA], "verify-data");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_WRITE_REGISTER], "write-register");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_READ_REGISTER], "read-register");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_ENGAGE_PLLO], "engage-pllo");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_CUSTOMER_KEY], "verify-customer-key");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_ACTIVATE_CUSTOMER_KEY], "activate-customer-key");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_GENERATE_APPLICATION_STARTUP_SIGNATURE], "generate-application-startup-signature");
	strcpy (idf_scp_cmd[COMMAND_MAXQ1852_VERIFY_APPLICATION_STARTUP_SIGNATURE], "verify-application-startup-signature");
	strcpy (idf_scp_cmd[COMMAND_SCP_LITE_LOAD_RAM], "load-ram");
	mode[COMMAND_HELP] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_RSA * 10 + SCP_FLORA_RSA;
	mode[COMMAND_WRITE_FILE] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_RSA * 10 + SCP_FLORA_RSA;
	mode[COMMAND_WRITE_ONLY] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_RSA * 10 + SCP_FLORA_RSA;
	mode[COMMAND_ERASE_DATA] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_RSA * 10 + SCP_FLORA_RSA;
	mode[COMMAND_VERIFY_FILE] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_RSA * 10 + SCP_FLORA_RSA;
	mode[COMMAND_WRITE_BLPK] = SCP_RSA * 10;
	mode[COMMAND_WRITE_FAK] = SCP_RSA * 10;
	mode[COMMAND_READ_CONFIGURATION] = SCP_RSA * 10;
	mode[COMMAND_WRITE_CONFIGURATION] = SCP_RSA * 10;
	mode[COMMAND_READ_MEMORY_MAPPING] = SCP_RSA * 10;
	mode[COMMAND_WRITE_CRK] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_REWRITE_CRK] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_WRITE_BPK] = SCP_FLORA_RSA;
	mode[COMMAND_WRITE_OTP] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_WRITE_TIMEOUT] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_KILL_CHIP] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_KILL_CHIP2] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_EXECUTE_CODE] = SCP_PAOLA * 1000 + SCP_ANGELA_ECDSA * 100 + SCP_FLORA_RSA;
	mode[COMMAND_MAXQ1852_LOAD_CUSTOMER_KEY] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_VERIFY_CUSTOMER_KEY] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_ACTIVATE_CUSTOMER_KEY] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_ERASE_CODE_FLASH_AREA] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_ERASE_ALL_FLASH_AREAS] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_LOAD_CODE] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_LOAD_FILE] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_LOAD_DATA] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_VERIFY_FILE] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_VERIFY_CODE] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_VERIFY_DATA] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_WRITE_REGISTER] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_READ_REGISTER] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_ENGAGE_PLLO] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_GENERATE_APPLICATION_STARTUP_SIGNATURE] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_MAXQ1852_VERIFY_APPLICATION_STARTUP_SIGNATURE] = MSP_MAXQ1852_ECDSA;
	mode[COMMAND_SCP_LITE_LOAD_RAM] = SCP_LITE_ECDSA;
}

void define_pp (void)
{
	int i;
	for (i = 0; i < MAX_IDF; i++)
		list_pp[i] = 0;
	list_pp[SCP_PP_CLEAR] = 1;
	list_pp[SCP_PP_RMAC] = list_pp[SCP_PP_CLEAR];
	list_pp[SCP_PP_E_RMAC] = list_pp[SCP_PP_CMAC] = list_pp[SCP_PP_CLEAR];
	list_pp[SCP_PP_E_CMAC] = list_pp[SCP_PP_RSA] = list_pp[SCP_PP_CLEAR];
	list_pp[SCP_PP_ECDSA] = list_pp[SCP_PP_PAOLA_2048] = list_pp[SCP_PP_CLEAR];
	list_pp[SCP_PP_PAOLA_4096] = list_pp[SCP_PP_CLEAR];
	strcpy (idf_pp[SCP_PP_CLEAR], "CLEAR");
	strcpy (idf_pp[SCP_PP_RMAC], "RMAC");
	strcpy (idf_pp[SCP_PP_E_RMAC], "ENC-RMAC");
	strcpy (idf_pp[SCP_PP_CMAC], "CMAC");
	strcpy (idf_pp[SCP_PP_E_CMAC], "ENC-CMAC");
	strcpy (idf_pp[SCP_PP_RSA], "RSA");
	strcpy (idf_pp[SCP_PP_ECDSA], "ECDSA");
	strcpy (idf_pp[SCP_PP_PAOLA_4096], "PAOLA-RSA-4096");
	strcpy (idf_pp[SCP_PP_PAOLA_2048], "PAOLA-RSA-2048");
}

int included (int value, int *list)
{
	if (value < MAX_IDF)
	{
		if (list[value] == 0)
			return (EXIT_FAILURE);
		else
			return (EXIT_SUCCESS);
	}
	else
		return (EXIT_FAILURE);
}

int init_keys (void)
{
	int i;
	memcpy (keya, config_struct.fka, 16);
	memcpy (keyc, config_struct.fkc, 16);
	memcpy (keys, config_struct.fks, 16);
	if (TRUE == verbose)
	{
		puts ("");
		printf ("----------\n");
		printf ("KA:");
		for (i = 0; i < 16; i++)
			printf ("%02x", keya[i]);
		printf ("\n");
		printf ("KC:");
		for (i = 0; i < 16; i++)
			printf ("%02x", keyc[i]);
		printf ("\n");
		printf ("KS:");
		for (i = 0; i < 16; i++)
			printf ("%02x", keys[i]);
		printf ("\n");
		printf ("----------\n");
		printf ("USN:");
		for (i = 0; i < 16; i++)
			printf ("%02x", config_struct.usn[i]);
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int init (void)
{

	chunk_size = 0;
	init_keys ();
	define_ctl ();
	define_cmd ();
	define_pp ();
	define_const ();
	define_scp_cmd ();
	return (EXIT_SUCCESS);
}

int init_crypto (void)
{

	int err = 0;
	err = ucl_init (init_buffer, 2048);
	compteur = 0;

	if (err != UCL_OK)
	{
		printf ("ERROR for ucl_init %d\n", err);
		return EXIT_FAILURE;
	}

#ifdef _MAXIM_HSM

	/* Initialise DLL, giving the path of Thales cknfast DLL  */
	err = HSM_InitDLL (config_struct.hsm_thales_dll);
	if (err != CKR_OK)
	{
		printf ("Error , Unable to Initialise DLL \n");
		HSM_pError (err);
		return err;
	}

#endif /* _MAXIM_HSM */
	return (EXIT_SUCCESS);
}

int help (void)
{
	int i;
	printf ("supported commands:\n");
	for (i = 0; i < MAX_SCP_COMMAND; i++)
	{
		printf ("%s", idf_scp_cmd[i]);
		if ((mode[i] % 10) == MSP_MAXQ1852_ECDSA)
			printf ("\tMSP_MAXQ1852_ECDSA\n");
		else if (((mode[i] % 100) / 10) == SCP_RSA)
			printf ("\tSCP-RSA");
		else
			printf ("\t");
		if ((mode[i] % 10) == SCP_FLORA_RSA)
			printf ("\tSCP-FLORA");
		if ((mode[i] / 100) == SCP_ANGELA_ECDSA)
			printf ("\tSCP-ECDSA");
		if ((mode[i] / 1000) == SCP_PAOLA)
			printf ("\tSCP-PAOLA");
		printf ("\n");
	}
	return (EXIT_SUCCESS);
}

int process_arg (char *line, int fgets_correction)
{
	int i;
	int j;
	int resu;
	int tmp_value;
	int l_iFoundString = 0;
	char string_pp[MAXLINE];
	found = 0;
	memset (string_pp, 0, MAXLINE);

	/*  resu=process_intvalue(&(config_struct.flash_mb),"flash_size_mb",line,MAX_FLASH_MB); */
	resu = process_intvalue (&j, "flash_size_mb=", line, MAX_FLASH_MB, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR while extracting <flash_size_mb> field\n");
		return (EXIT_FAILURE);
	}
	else if (found == 1)
	{
		config_struct.flash_mb = j;
	}

	resu = process_hexvalue (&i, config_struct.usn, "usn=", line, fgets_correction);
	/* simple filtering on acceptable values, 13 or 16
	   stricter filtering will be done later, because at this stage, session_mode may be unknown */
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on usn retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != USN_LEN && i != USN_FLORA_LEN && i != USN_ANGELA_LEN && i != -1)
		{
			printf ("ERROR while extracting <usn> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
		else
		{
			if (i == USN_FLORA_LEN || i == USN_ANGELA_LEN || i == USN_LEN)
			{
				config_struct.usn_len = i;
			}
		}
	}
	resu = process_hexvalue (&i, aes_key, "aes_key=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on aes_key retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != 16 && i != -1)
		{
			printf ("ERROR while extracting <aes_key> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
	}
	resu = process_hexvalue (&i, aes_data, "aes_data=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on aes_data retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != 16 && i != -1)
		{
			printf ("ERROR while extracting <aes_data> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
	}
	resu = process_hexvalue (&i, config_struct.fka, "fka=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on fka retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != 16 && i != -1)
		{
			printf ("ERROR while extracting <fka> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
	}
	resu = process_hexvalue (&i, config_struct.fkc, "fkc=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on fkc retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != 16 && i != -1)
		{
			printf ("ERROR while extracting <fkc> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
	}
	resu = process_hexvalue (&i, config_struct.fks, "fks=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on fks retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != 16 && i != -1)
		{
			printf ("ERROR while extracting <fks> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
	}
#ifdef _MAXIM_HSM
	resu = process_string (config_struct.HSM_KeyLabel, "hsm_key_name=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR while extracting <hsm_name_key> field\n");
			return (EXIT_FAILURE);
		}
		found = 1;
	}
	resu = process_string (config_struct.hsm_thales_dll, "hsm_thales_dll=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR while extracting <hsm_thales_dll> field\n");
			return (EXIT_FAILURE);
		}
		found = 1;
	}
	process_intvalue (&config_struct.hsm_slot_nb, "hsm_slot_nb=", line, 10, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR while extracting <hsm_slot_nb> field\n");
		return (EXIT_FAILURE);
	}
#endif /* _MAXIM_HSM */
	resu = process_string (ecdsafile, "ecdsa_file=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR on ecdsa_file retrieval\n");
			return (EXIT_FAILURE);
		}
		found = 1;
	}
	resu = process_string (rsafile, "rsa_file=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{

		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR on rsa_file retrieval\n");
			return (EXIT_FAILURE);
		}
		found = 1;
	}

	resu = process_hexint (&tmp_value, "addr_offset=", line, fgets_correction);
	if (resu != EXIT_SUCCESS)
	{
		printf ("ERROR while extracting <addr_offset> field\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (tmp_value != -1)
			config_struct.address_offset = tmp_value;
	}

	resu = process_hexint (&tmp_value, "transaction_id=", line, fgets_correction);
	if (resu != EXIT_SUCCESS)
	{
		printf ("ERROR while extracting <addr_offset> field\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (tmp_value != -1)
			trid = tmp_value;
	}

	/* Add of chunk_size arguments */
	resu = process_intvalue (&tmp_value, "chunk_size=", line, MAX_CHUNK_SIZE, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR while extracting <chunk_size> field\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (tmp_value != -1)
			chunk_size = tmp_value;
	}

	resu = process_hexvalue (&i, config_struct.random_number, "random_number=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR on rsa pub exponent retrieval\n");
		return (EXIT_FAILURE);
	}
	else
	{
		if (i != 16 && i != -1)
		{
			printf ("ERROR while extracting <random_number> field: bad length (%d bytes)\n", i);
			return (EXIT_FAILURE);
		}
	}
	resu = process_string (output_file, "output_file=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR while extracting <output_file> field\n");
			return (EXIT_FAILURE);
		}
		found = 1;
	}
	resu = process_string (script_file, "script_file=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR while extracting <script_file> field\n");
			return (EXIT_FAILURE);
		}
		found = 1;
	}

	resu = process_string (string_pp, "pp=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR while extracting <pp> field\n");
			return (EXIT_FAILURE);
		}

		for (i = 0; i < (int) strlen (string_pp); i++)
			string_pp[i] = (char) toupper ((int) string_pp[i]);
		if (strstr (string_pp, "E_CMAC") != NULL)
		{
			config_struct.pp = SCP_PP_E_CMAC;
		}
		else
		{
			if (strstr (string_pp, "CMAC") != NULL)
			{
				config_struct.pp = SCP_PP_CMAC;
			}
			else
			{
				if (strstr (string_pp, "E_RMAC") != NULL)
				{
					config_struct.pp = SCP_PP_E_RMAC;
				}
				else
				{
					if (strstr (string_pp, "RMAC") != NULL)
					{
						config_struct.pp = SCP_PP_RMAC;
					}
					else
					{
						if (strstr (string_pp, "RSA") != NULL)
						{
							config_struct.pp = SCP_PP_RSA;
						}
						else
						{
							if (strstr (string_pp, "ECDSA") != NULL)
							{
								config_struct.pp = SCP_PP_ECDSA;
							}
							else if (strstr (string_pp, "PAOLA_4096") != NULL)
							{
								config_struct.pp = SCP_PP_PAOLA_4096;
							}
							else if (strstr (string_pp, "PAOLA_2048") != NULL)
							{
								config_struct.pp = SCP_PP_PAOLA_2048;
							}
							else
							{
								config_struct.pp = SCP_PP_CLEAR;
							}
						}
					}
				}
			}
		}
		found = 1;
	}
	resu = process_string (session_string, "session_mode=", line, fgets_correction, &l_iFoundString);
	if (l_iFoundString)
	{
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR while extracting <session_mode> field\n");
			return (EXIT_FAILURE);
		}
		if (strstr (session_string, "SCP_ON_AES") != NULL)
		{
			session_mode = SCP_ON_AES;
		}
		if (strstr (session_string, "SBL") != NULL)
		{
			session_mode = SBL;
		}
		if (strstr (session_string, "SCP_OFF_AES") != NULL)
		{
			session_mode = SCP_OFF_AES;
		}
		if (strstr (session_string, "SCP_FLORA_AES") != NULL)
		{
			session_mode = SCP_FLORA_AES;
		}
		if (strstr (session_string, "SCP_RSA") != NULL)
		{
			session_mode = SCP_RSA;
		}
		if (strstr (session_string, "SCP_FLORA_RSA") != NULL)
		{
			session_mode = SCP_FLORA_RSA;
		}
		if (strstr (session_string, "SCP_ANGELA_ECDSA") != NULL)
		{
			session_mode = SCP_ANGELA_ECDSA;
			printf ("scp-angela-ecdsa\n");
		}
		if (strstr (session_string, "SCP_PAOLA") != NULL)
		{
			session_mode = SCP_PAOLA;
			printf ("scp-paola-rsa\n");
		}
		if (strstr (session_string, "MSP_MAXQ1852_ECDSA") != NULL)
		{
			session_mode = MSP_MAXQ1852_ECDSA;
		}
		if (strstr (session_string, "SCP_LITE_ECDSA") != NULL)
		{
			session_mode = SCP_LITE_ECDSA;
			printf ("scp-lite-ecdsa\n");
		}
		found = 1;
	}
	if (strstr (line, "verbose=") != NULL)
	{
		verbose = (strstr (line, "yes") != NULL) ? TRUE : FALSE;
		found = 1;
	}
	if (!found)
	{
		printf ("ERROR: line with unknown field: <%s>\n", line);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int load_args (int argc, char **argv)
{
	int k;
	int resu;

	for (k = 1; k < argc; k++)
	{
		resu = process_arg (argv[k], 0);
		if (EXIT_SUCCESS != resu)
			return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}

static int load_keys (void)
{

	int resu;
	int max_chunk_size;

	if (MSP_MAXQ1852_ECDSA == session_mode || SCP_ANGELA_ECDSA == session_mode || SCP_LITE_ECDSA == session_mode)
	{
		config_struct.ecdsa_len = ECDSA_MODULUS_LEN;
#ifndef _MAXIM_HSM
		resu = read_file_ecdsa (config_struct.ecdsa_pubkey_x,
					config_struct.ecdsa_pubkey_y,
					config_struct.ecdsa_privkey, config_struct.ecdsa_len, ecdsafile);
#else
		resu = load_HSM_ecdsa_key (&config_struct, config_struct.HSM_KeyLabel);
#endif /* _MAXIM_HSM */

		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR in read_file_ecdsa\n");
			return (EXIT_FAILURE);
		}

		if (config_struct.pp != SCP_PP_ECDSA)
		{
			printf ("WARNING\n");
			printf ("SCP session mode is defined with this value: \"MSP_MAXQ1852_ECDSA\".\n");
			printf ("But the protection profile (\"pp\" value) is different from \"ECDSA\".\n");
		}
	}

	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{

		/* -- Init config_struct RSA parameters */
		if (SCP_PAOLA == session_mode)
		{
			if (SCP_PP_PAOLA_4096 == config_struct.pp)
			{
				config_struct.rsa_len = PAOLA_SIGNATURE_LEN;
				config_struct.rsa_privexplen = PAOLA_RSA_MODULUS_LEN;
			}
			else if (SCP_PP_PAOLA_2048 == config_struct.pp)
			{
				config_struct.rsa_len = SIGNATURE_LEN;
				config_struct.rsa_privexplen = RSA_MODULUS_LEN;
			}
			else
			{
				printf ("ERROR: PP inconsistent with session mode: %d vs %d\n", config_struct.pp, session_mode);
				return (EXIT_FAILURE);
			}
		}
		else
		{
			config_struct.rsa_privexplen = RSA_MODULUS_LEN;
			config_struct.rsa_len = SIGNATURE_LEN;
		}
		config_struct.rsa_explen = RSA_PUBLIC_EXPONENT_LEN;
#ifndef _MAXIM_HSM
		resu = read_file_rsa (config_struct.rsa,
				      &(config_struct.rsa_len),
				      config_struct.rsa_pubexp, config_struct.rsa_privexp, config_struct.rsa_explen, rsafile);
#else
		resu = load_HSM_rsa_key (&config_struct, config_struct.HSM_KeyLabel);
#endif /* _MAXIM_HSM */
		if (EXIT_SUCCESS != resu)
		{
			printf ("ERROR in read_file_rsa\n");
			return (EXIT_FAILURE);
		}

		if (config_struct.pp != SCP_PP_RSA
		    && config_struct.pp != SCP_PP_PAOLA_4096 && config_struct.pp != SCP_PP_PAOLA_2048)
		{
			printf (" WARNING\n");
			printf ("SCP session mode is defined with one of this value: \"SCP_PAOLA\" or \"SCP_RSA\" or \"SCP_FLORA_RSA\".\n");
			printf ("But the protection profile (\"pp\" value) is different from \"RSA\" or \"PAOLA\".\n");
		}
		if (config_struct.rsa_len != config_struct.rsa_privexplen)
		{
			printf ("ERROR: lengths of RSA modulus and RSA private exponent do not match: %d bytes vs %d bytes\n",
				config_struct.rsa_len, config_struct.rsa_privexplen);
			return (EXIT_FAILURE);
		}

	}

	if (SCP_ON_AES == session_mode || SCP_FLORA_AES == session_mode)
	{
		if (config_struct.pp != SCP_PP_E_CMAC)
		{
			printf (" WARNING\n");
			printf ("SCP session mode is defined with one of this value: \"SCP_ON_AES\" or \"SCP_FLORA_AES\".\n");
			printf ("But the protection profile (\"pp\" value) is different from \"E_CMAC\".\n");
		}
	}
	if (((SCP_FLORA_RSA == session_mode || SCP_FLORA_AES == session_mode)
	     && (config_struct.usn_len != USN_FLORA_LEN))
	    ||
	    ((SCP_RSA == session_mode || SCP_ON_AES == session_mode
	      || SCP_OFF_AES == session_mode) && (config_struct.usn_len != USN_LEN)))
	{
		printf ("ERROR: USN with bad length: %d\n", config_struct.usn_len);
		return (EXIT_FAILURE);
	}
	if (SCP_FLORA_RSA == session_mode)
		max_chunk_size = FLORA_MAX_CHUNK_SIZE;
	if (SCP_ANGELA_ECDSA == session_mode)
		max_chunk_size = ANGELA_MAX_CHUNK_SIZE;
	if (SCP_PAOLA == session_mode)
		max_chunk_size = PAOLA_MAX_CHUNK_SIZE;
	if (SCP_LITE_ECDSA == session_mode)
		max_chunk_size = SCP_LITE_MAX_CHUNK_SIZE;
	if (MSP_MAXQ1852_ECDSA == session_mode)
		max_chunk_size = MAXQ1852_MAX_CHUNK_SIZE;
	if (chunk_size > max_chunk_size)
	{
		printf ("WARNING: provided chunk_size (%d bytes) is larger than the supported max chunk_size (%d bytes)\n",
			chunk_size, max_chunk_size);
		chunk_size = max_chunk_size;
	}

	return (EXIT_SUCCESS);
}

int load_default_config (void)
{
	int i;
	u8 usn_default[] = { 0x04, 0x00, 0x43, 0x47, 0x1f, 0xd2, 0x03, 0x08, 0x0c, 0x07,
		0x00,
		0x00,
		0x7f, 0x24, 0xea, 0x2f
	};
	verbose = TRUE;
	session_mode = SCP_RSA;
	/* default config */
	config_struct.cpu = IC400ABC;
	config_struct.life_cycle = P6;
	config_struct.already_diversified = 0;
	/* config_struct.pp=SCP_PP_CMAC; */
	config_struct.pp = SCP_PP_RSA;
	/* flash size is 32MB */
	config_struct.flash_mb = 32;
	config_struct.address_offset = 0;
	chunk_size = 1024;
	for (i = 0; i < 16; i++)
		config_struct.random_number[i] = rand () & 255;
	for (i = 0; i < 16; i++)
		blpk[i] = i + 1;
	for (i = 0; i < 16; i++)
		fak[i] = 0x21;
	for (i = 0; i < 16; i++)
		aes_key[i] = 0x41;
	for (i = 0; i < 16; i++)
		aes_data[i] = 0x51;
	for (i = 0; i < USN_LEN; i++)
		config_struct.usn[i] = usn_default[i];
	for (i = 0; i < UCL_AES_BLOCKSIZE; i++)
	{
		config_struct.mka[i] = 0x31;
		config_struct.mkc[i] = 0x32;
		config_struct.mks[i] = 0x33;
		config_struct.tka[i] = 0x41;
		config_struct.tkc[i] = 0x42;
		config_struct.tks[i] = 0x43;
		config_struct.pka[i] = 0x51;
		config_struct.pkc[i] = 0x52;
		config_struct.pks[i] = 0x53;
		config_struct.fka[i] = 0x61;
		config_struct.fkc[i] = 0x62;
		config_struct.fks[i] = 0x63;
		config_struct.sblpk[i] = 0x91;
	}
	/*  printf("<load default config>\n"); */
	sprintf (script_file, "script.txt");
	sprintf (output_file, "session.txt");
	return (EXIT_SUCCESS);
}

int line_is_void (char *line)
{
	int i;
	for (i = 0; i < (int) strlen (line); i++)
		if (!(line[i] == ' '))
			return (0);
	return (1);
}

int load_ini_config (FILE * fp)
{
	char line[MAXLINE];
	int resu;
	unsigned int fcorection;
	/*    printf("<load .ini config>\n"); */
	while (fgets (line, MAXLINE, fp) != NULL)
	{
		if (line[strlen (line)] != '\0')
		{
			printf ("ERROR: overflow on line <%s>\n", line);
			return (EXIT_FAILURE);
		}
		
		if (line[strlen (line)-1] != '\n')
		{
			fcorection = 0;
		}else{
			fcorection = 1;
		}
		
		if ('#' == line[0])
			continue;
		if (line_is_void (line))
			continue;
		resu = process_arg (line, fcorection);
		if (resu != EXIT_SUCCESS)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/* this function reads the .ini and configures the parameters */
static int load_config (void)
{
	FILE *fp;
	int resu;

	load_default_config ();
	/* read the configuration file */
	fp = fopen (INIFILE, "r");

	/* if file not present */
	if (fp == NULL)
	{
		/* setup with the default configuration */
		printf ("WARNING: <%s> not found\n", INIFILE);
	}
	else
	{
		resu = load_ini_config (fp);
		if (resu != EXIT_SUCCESS)
			return (EXIT_FAILURE);
		(void) fclose (fp);
	}
	return (EXIT_SUCCESS);
}

void display_config (int std_display)
{
	unsigned int i = 0;
	if (TRUE == std_display)
		printf ("<display config>\n");

	if (SBL == session_mode)
	{
		printf ("mode: SBL\n");

		if (SCP_PP_CMAC == config_struct.pp)
			printf ("pp: CMAC\n");

		if (SCP_PP_E_CMAC == config_struct.pp)
			printf ("pp: E-CMAC\n");

		if (SCP_PP_RMAC == config_struct.pp)
			printf ("pp: RMAC\n");

		if (SCP_PP_E_RMAC == config_struct.pp)
			printf ("pp: E-RMAC\n");

		if (SCP_PP_CLEAR == config_struct.pp)
			printf ("pp: PLAIN\n");
	}
	else if (SCP_ON_AES == session_mode)
	{
		printf ("mode: SCP online AES\n");

		if (SCP_PP_CMAC == config_struct.pp)
			printf ("pp: CMAC\n");

		if (SCP_PP_E_CMAC == config_struct.pp)
			printf ("pp: E-CMAC\n");
	}
	else if (SCP_OFF_AES == session_mode)
		printf ("mode: SCP offline AES\n");
	else if (SCP_RSA == session_mode)
		printf ("mode: SCP RSA\n");
	else if (SCP_FLORA_RSA == session_mode)
		printf ("mode: SCP FLORA RSA\n");
	else if (SCP_FLORA_AES == session_mode)
	{
		printf ("mode: SCP FLORA AES\n");
		if (SCP_PP_CMAC == config_struct.pp)
			printf ("pp: CMAC\n");
		if (SCP_PP_E_CMAC == config_struct.pp)
			printf ("pp: E-CMAC\n");
	}
	else if (MSP_MAXQ1852_ECDSA == session_mode)
		printf ("mode: SCP MAXQ1852 ECDSA\n");
	else if (SCP_ANGELA_ECDSA == session_mode)
	{
		printf ("mode: SCP ANGELA ECDSA\n");
		printf ("PP=%d\n", config_struct.pp);
	}
	else if (SCP_PAOLA == session_mode)
	{
		printf ("mode: SCP PAOLA RSA\n");
		if (SCP_PP_PAOLA_4096 == config_struct.pp)
			printf ("PP=PAOLA-4096\n");
		if (SCP_PP_PAOLA_2048 == config_struct.pp)
			printf ("PP=PAOLA-2048\n");
	}
	else if (SCP_LITE_ECDSA == session_mode)
	{
		printf ("mode: SCP LITE ECDSA\n");
		printf ("PP=%d\n", config_struct.pp);
	}
	if (TRUE == verbose)
		printf ("verbose\n");
	else
		printf ("mute\n");
	printf ("PP=%d -> ", config_struct.pp);
	switch (config_struct.pp)
	{
	case SCP_PP_CLEAR:
		printf ("CLEAR\n");
		break;
	case SCP_PP_RMAC:
		printf ("RMAC\n");
		break;
	case SCP_PP_E_RMAC:
		printf ("E-RMAC\n");
		break;
	case SCP_PP_CMAC:
		printf ("CMAC\n");
		break;
	case SCP_PP_E_CMAC:
		printf ("E-CMAC\n");
		break;
/*    case SCP_PP_RSA:
			printf("RSA\n");
			break;*/
	case SCP_PP_PAOLA_4096:
		printf ("RSA-4096\n");
		break;
	case SCP_PP_PAOLA_2048:
		printf ("RSA-2048\n");
		break;
	case SCP_PP_ECDSA:
		printf ("ECDSA\n");
		break;
	case SCP_PP_UNK:
		printf ("unknown\n");
		break;
	default:
		printf ("ERROR\n");
		break;
	}
	printf ("output file: %s\n", output_file);

	/* data used by PCI LINUX SCP only */
	if (SCP_ON_AES == session_mode || SCP_OFF_AES == session_mode || SCP_RSA == session_mode)
	{
		printf ("blpk:");
		for (i = 0; i < 16; i++)
			printf ("%02x", blpk[i]);
		printf ("\n");
		printf ("fak:");
		for (i = 0; i < 16; i++)
			printf ("%02x", fak[i]);
		printf ("\n");
		printf ("aes_key:");
		for (i = 0; i < 16; i++)
			printf ("%02x", aes_key[i]);
		printf ("\n");
		printf ("aes_data:");
		for (i = 0; i < 16; i++)
			printf ("%02x", aes_data[i]);
		printf ("\n");
	}
	if (SCP_ON_AES == session_mode)
	{
		printf ("random number:");
		for (i = 0; i < 16; i++)
			printf ("%02x", config_struct.random_number[i]);
		printf ("\n");
	}
	if (SCP_RSA == session_mode || SCP_FLORA_RSA == session_mode || SCP_PAOLA == session_mode)
	{
#ifndef _MAXIM_HSM
		printf ("rsa modulus:");
		for (i = 0; i < config_struct.rsa_len; i++)
			printf ("%02x", config_struct.rsa[i]);
		printf ("\n");
		printf ("rsa public exponent:");
		for (i = 0; i < config_struct.rsa_explen; i++)
			printf ("%02x", config_struct.rsa_pubexp[i]);
		printf ("\n");
		printf ("rsa private exponent:");
		for (i = 0; i < config_struct.rsa_len; i++)
			printf ("%02x", config_struct.rsa_privexp[i]);
		printf ("\n");
#else
		if (strlen (config_struct.HSM_KeyLabel) != 0)
		{
			printf ("rsa key: %s\n", config_struct.HSM_KeyLabel);
		}
#endif /* _MAXIM_HSM */

	}
	if (MSP_MAXQ1852_ECDSA == session_mode)
	{
#ifndef _MAXIM_HSM
		printf ("transaction ID: %08x\n", trid);
		printf ("ecdsa privkey:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_privkey[i]);
		printf ("\n");
		printf ("ecdsa public x:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_pubkey_x[i]);
		printf ("\n");
		printf ("ecdsa public y:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_pubkey_y[i]);
		printf ("\n");
#else
		if (strlen (config_struct.HSM_KeyLabel) != 0)
		{
			printf ("ecdsa key: %s\n", config_struct.HSM_KeyLabel);
		}
#endif /* _MAXIM_HSM */
	}
	if (SCP_ANGELA_ECDSA == session_mode)
	{
#ifndef _MAXIM_HSM
		printf ("transaction ID: %08x\n", trid);
		printf ("ecdsa privkey:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_privkey[i]);
		printf ("\n");
		printf ("ecdsa public x:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_pubkey_x[i]);
		printf ("\n");
		printf ("ecdsa public y:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_pubkey_y[i]);
		printf ("\n");
#else
		if (strlen (config_struct.HSM_KeyLabel) != 0)
		{
			printf ("ecdsa key: %s\n", config_struct.HSM_KeyLabel);
		}
#endif /* _MAXIM_HSM */
	}
	if (SCP_LITE_ECDSA == session_mode)
	{
#ifndef _MAXIM_HSM
		printf ("ecdsa privkey:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_privkey[i]);
		printf ("\n");
		printf ("ecdsa public x:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_pubkey_x[i]);
		printf ("\n");
		printf ("ecdsa public y:");
		for (i = 0; i < config_struct.ecdsa_len; i++)
			printf ("%02x", config_struct.ecdsa_pubkey_y[i]);
		printf ("\n");
#else
		if (strlen (config_struct.HSM_KeyLabel) != 0)
		{
			printf ("ecdsa key: %s\n", config_struct.HSM_KeyLabel);
		}
#endif /* _MAXIM_HSM */
	}
	printf ("flash maximal size: %dMBytes\n", config_struct.flash_mb);
	printf ("script file:<%s>\n", script_file);
	printf ("chunk size: %d\n", chunk_size);
	printf ("addr.offset: %08x\n", config_struct.address_offset);
#ifdef _MAXIM_HSM	
	printf ("HSM slot: %d\n", config_struct.hsm_slot_nb);
	printf ("HSM Thales DLL: %s\n", config_struct.hsm_thales_dll);
#endif /* _MAXIM_HSM */	
	
}

int process (void)
{

	/* check if requested size is not too large compared to what supported (i.e. MAX_FLASH_MB), because of int coding) */
	if (config_struct.flash_mb > MAX_FLASH_MB)
	{
		printf ("ERROR: requested flash size is too large (%dMB) while limited to %dMB\n", config_struct.flash_mb,
			MAX_FLASH_MB);
		return (EXIT_FAILURE);
	}

	/* dynamic allocation of *data, which contains the meaningful binary file bytes, synchronized */
	data_g = (u8 *) malloc (sizeof (u8) * 1024 * 1024 * config_struct.flash_mb);
	if (NULL == data_g)
	{
		printf ("ERROR: <data> allocation is not possible (%dMB requested)\n", config_struct.flash_mb);
		return (EXIT_FAILURE);
	}
	/* dynamic allocation of *addr, which contains the addresses of meaningful binary file bytes */
	addr_g = (int *) malloc (sizeof (int) * 1024 * 1024 * config_struct.flash_mb);
	if (NULL == addr_g)
	{
		printf ("ERROR: <addr> allocation is not possible (%dMB requested)\n", config_struct.flash_mb);
		return (EXIT_FAILURE);
	}
	max_data_size = 1024 * 1024 * config_struct.flash_mb;

	if (MSP_MAXQ1852_ECDSA == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session MSP-MAXQ1852-ECDSA>\n");
		fprintf (fp_g, "<session MSP-MAXQ1852-ECDSA>\n");
		return process_script_maxq1852_ecdsa ();
	}
	else if (SCP_LITE_ECDSA == session_mode)
	{
		if (TRUE == verbose)
			printf ("<session SCP-LITE-ECDSA>\n");
		fprintf (fp_g, "<session SCP-LITE-ECDSA>\n");
		return process_script_scp_lite_ecdsa ();
	}
	else
	{

		return process_script ();

	}

}

int main (int argc, char **argv)
{
	int resu;

#ifdef _MAXIM_HSM
	int i;
#endif 	

	printf ("SBL/SCP packets builder v%d.%d.%d (build %d) (c)Maxim Integrated 2006-2017\n", MAJV, MINV, ZVER, BUILD);

#ifdef _MAXIM_HSM

	printf ("\n--warning: this tool handles keys in PCI PTS compliant way --\n");
	printf ("\n\tLibHSM Version: %s (%s)", HSM_GetVersion (), HSM_GetBuildDate ());

#else

	printf ("--warning: this tool does not handle keys in a PCI PTS compliant way --\n");

#endif /* _MAXIM_HSM */

	printf ("\n\tUCL Version: %s (%s)\n\n", ucl_get_version (), ucl_get_build_date ());
	init ();

	resu = load_config ();
	if (resu != EXIT_SUCCESS)
	{
		return (EXIT_FAILURE);
	}

	resu = load_args (argc, argv);
	if (resu != EXIT_SUCCESS)
	{
		return (EXIT_FAILURE);
	}

	init_crypto ();
	if (resu != EXIT_SUCCESS)
	{
		return (EXIT_FAILURE);
	}
	
	
	if (TRUE == verbose)
	{
		display_config (FALSE);
	}

#ifdef _MAXIM_HSM

	printf("HSM try open connection\n");
	/* Open HSM Session and Login using OCS Cards */
	resu = CKR_GENERAL_ERROR;
	for(i = 0; (i<3 && resu != CKR_OK); i++)
	{
		resu = HSM_Login (&session, config_struct.hsm_slot_nb);
	}
	if (EXIT_SUCCESS != resu)
	{
		printf("Unable to open connection with HSM.\n");
		HSM_pError (resu);
		return (EXIT_FAILURE);
	}
#endif /* _MAXIM_HSM */

	resu = load_keys ();
	if (resu != EXIT_SUCCESS)
	{
		return (EXIT_FAILURE);
	}
	sprintf (output_name, "%s.log", output_file);
	fp_g = fopen (output_name, "w");
	if (NULL == fp_g)
	{
		printf ("ERROR: unable to create <%s>\n", output_file);
		return (EXIT_FAILURE);
	}

	fprintf (fp_g, "session generation v%d.%d.%d (build %d) (c)Maxim IC 2006-2017\n", MAJV, MINV, ZVER, BUILD);

	resu = process ();

	(void) fclose (fp_g);
	if (EXIT_SUCCESS != resu)
	{
		printf ("ERROR: aborting\n");
#ifdef _MAXIM_HSM

		/* Close HSM Sessions */
		resu = HSM_Close (session);

		if (EXIT_SUCCESS != resu)
		{
			printf ("Warning: fails to close HSM connection\n");
		}
#endif /* _MAXIM_HSM */
		return (EXIT_FAILURE);
	}
	printf ("<%s> created\n", output_name);

#ifdef _MAXIM_HSM
	resu = HSM_Close (session);

	if (EXIT_SUCCESS != resu)
	{
		printf ("Warning: fails to close HSM connection\n");
	}
#endif /* _MAXIM_HSM */

	return (EXIT_SUCCESS);
}
