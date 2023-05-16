/*******************************************************************************
* Copyright (C) 2009-2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
* @contributor: Emmanuel Puerto <emmanuel.puerto@maximintegrated.com>
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <ucl/ucl_config.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_sys.h>
#include <ucl/ucl_info.h>
#include <ucl/ucl_data_conv.h>
#include <ucl/ucl_stack.h>
#include <ucl/ucl_sha1.h>
#include <ucl/ucl_rng.h>

#include <ucl/ucl_trng.h>
#include <ucl/ucl_aes.h>
#include <ucl/ucl_uaes.h>
#include <ucl/ucl_uaes_ecb.h>
#include <ucl/ucl_uaes_cbc.h>
#include <ucl/ucl_rsa.h>
#include <ucl/ucl_sha1.h>
#include <ucl/ucl_sha256.h>
#include <ucl/ucl_pkcs1_ssa_pss_sha256.h>
#include <ucl/ecdsa_generic_api.h>

#include <ca_sign_build.h>
#include "maxim_c_utils.h"

char load_address_string[MAX_STRING];
char jump_address_string[MAX_STRING];
char arguments[MAX_STRING];
u8 sr_papd;
u32 sr_prfsh;
u32 sr_pcfg;
u32 dmc_gcfg;
u8 dmc_clk;
u32 uci2_ctrl_reg;
u8 uci0_ksrc_configencint;
u32 uci0_ac1r_so;
u32 uci0_ac1r_eo;
u32 uci0_ddr_r0;
u8 sr_pext;
u32 version;
u32 application_version;
int header_len;
char algo[MAX_STRING];
algo_t algro;
char boot_method[64];
u32 init_buffer[2048];
u8 data[MAX_FLASH];
int found;
char cafile[MAX_STRING];
char scafile[MAX_STRING];
char sigfile[MAX_STRING];
char rsafile[MAX_STRING];
char ecdsafile[MAX_STRING];
char tmp_str[MAX_STRING];

u8 signonly;
u8 headergenerated;

char * algo_name[]={"c", "rsa_paola", "sha256", "crc32", "none", "ecdsa"};

int hash_sign_payload (u8 * hash, u8 * payload, u32 payload_len)
{
	unsigned int i;
	int err;
  /** Set up data to be signed */
	err = ucl_sha256 (hash, payload, payload_len);
	if (UCL_OK != err)
	{
		print_error ("Hash sha256 failed\n");
		return EXIT_FAILURE;
	}
	if (4 <= verbose)
	{
		print_debug("hash: ");
		for (i = 0; i < UCL_SHA256_HASHSIZE; i++)
		{
			print_d ("%02x", hash[i]);
		}
		print_d ("\n");
	}
	return EXIT_SUCCESS;
}

int crc32_sign_payload (u32 * p_crc, u8 * payload, u32 payload_len)
{
	const unsigned int Polynomial = 0xEDB88320;
	volatile unsigned char *p_current;
	unsigned int i;
	/** Check inputs */
	if (!p_crc || !payload)
	{
		print_error("[crc32_sign_payload] Input pointer null\n");
		return EXIT_FAILURE;
	}
	else
	{
		/** Assign input pointer to work pointer */
		p_current = (volatile unsigned char *) payload;
		*p_crc = 0;
		/** Same as crc ^ 0xffffffff */
		/**p_crc = ~(*p_crc);*/
	}
	/** Compute CRC */
	while (payload_len--)
	{
		*p_crc ^= *p_current++;
		for (i = 0; i < 8; i++)
		{
			if (*p_crc & 1)
			{
				*p_crc = (*p_crc >> 1) ^ Polynomial;
			}
			else
			{
				*p_crc = *p_crc >> 1;
			}
		}
	}
	/** Same as crc ^ 0xffffffff */
	/**p_crc = ~(*p_crc);*/
	print_debug ("\ncrc32: 0x%08x\n", *p_crc);
	return EXIT_SUCCESS;
}

int write_file (char *filename, u8 * payload, u32 payload_len, u8 * signature, int signature_len)
{
	FILE *fp;
	u32 i;
	int resu;
	fp = fopen (filename, "wb");
	if (fp == NULL)
	{
		print_error ("Failed opening <%s>\n", filename);
		return EXIT_FAILURE;
	}
	for (i = 0; i < payload_len; i++)
	{
		resu = fwrite (&(payload[i]), sizeof (u8), 1, fp);
		if (1 != resu)
		{
			print_error ("on writing payload byte #%d\n", i);
			return EXIT_FAILURE;
		}
	}
	for (i = 0; i < (u32) signature_len; i++)
	{
		resu = fwrite (&(signature[i]), sizeof (u8), 1, fp);
		if (1 != resu)
		{
			print_error ("on writing signature byte #%d\n", i);
			return EXIT_FAILURE;
		}
	}
	
	print_debug("signature:");
	for (i = 0; i < (u32) signature_len; i++)
		print_d ("%02x", signature[i]);
	print_d ("\n");
	
	(void) fclose (fp);
	return EXIT_SUCCESS;
}

/* 1.5.4: #4539 */
int write_signature (char *filename, u8 * signature, int signature_len)
{
	FILE *fp;
	u32 i;
	int resu;
	fp = fopen (filename, "wb");
	if (fp == NULL)
	{
		print_error ("on opening <%s>\n", filename);
		return EXIT_FAILURE;
	}
	else if (!signature_len)
	{
		print_error("[write_signature] No length, no signature.\n");
		(void) fclose (fp);
		return EXIT_SUCCESS;
	}
	/**  */
	for (i = 0; i < (u32) signature_len; i++)
	{
		resu = fwrite (&(signature[i]), sizeof (u8), 1, fp);
		if (1 != resu)
		{
			print_error ("on writing signature byte #%d\n", i);
			(void) fclose (fp);
			return EXIT_FAILURE;
		}
	}
	
	print_debug ("signature:");
	for (i = 0; i < (u32) signature_len; i++)
	{
		print_d ("%02x", signature[i]);
	}
	
	print_d ("\n");
	
	/** Close file */
	(void) fclose (fp);
	/** We're done */
	return EXIT_SUCCESS;
}

int add_header_to_payload (u8 * payload, u32 * payload_len)
{
	int i;
	/* offset represents the current index when filling the *header */
	int offset;
	int arguments_len;
	u8 header[MAX_HEADER_LEN];
	/* synchro pattern (64 bits) */
	
	switch(algro){
		case a_rsa: /* flora */
			header_len = FLORA_HEADER_LEN;
			header[0] = 0x59;
			header[1] = 0x45;
			header[2] = 0x53;
			header[3] = 0x57;
			header[4] = 0x45;
			header[5] = 0x43;
			header[6] = 0x41;
			header[7] = 0x4E;
			break;
	
		case a_rsa_paola:
			/* paola rsa 4096 */
			if ( config_struct.rsa_len == PAOLA_RSA_BYTE_LEN )
			{
				header_len = ANGELA_HEADER_LEN;
				header[0] = 0x47;
				header[1] = 0x49;
				header[2] = 0x53;
				header[3] = 0x57;
				header[4] = 0x45;
				header[5] = 0x44;
				header[6] = 0x47;
				header[7] = 0x44;
			}
			/* paola rsa 2048 */
			else if (config_struct.rsa_len == RSA_BYTE_LEN )
			{
				header_len = ANGELA_HEADER_LEN;
				header[0] = 0x46;
				header[1] = 0x49;
				header[2] = 0x53;
				header[3] = 0x57;
				header[4] = 0x45;
				header[5] = 0x44;
				header[6] = 0x47;
				header[7] = 0x44;
			}
			
			if (strstr (boot_method, "cmsis") != NULL)
			{
				/*  0xf6 to indicate that ROM Code shall boot CMSIS way */
				header[0] |= 0xb0;
			}
			break;
		case a_sha256:	/* paola sha256 */
			header_len = ANGELA_HEADER_LEN;
			header[0] = 0x49;
			header[1] = 0x49;
			header[2] = 0x53;
			header[3] = 0x57;
			header[4] = 0x45;
			header[5] = 0x44;
			header[6] = 0x47;
			header[7] = 0x44;
	
			if (strstr (boot_method, "cmsis") != NULL)
			{
				/*  0xf9 to indicate that ROM Code shall boot CMSIS way */
				header[0] |= 0xb0;
			}
			break;
	
		case a_crc32 :	/* paola crc */
			header_len = ANGELA_HEADER_LEN;
			header[0] = 0x44;
			header[1] = 0x49;
			header[2] = 0x53;
			header[3] = 0x57;
			header[4] = 0x45;
			header[5] = 0x44;
			header[6] = 0x47;
			header[7] = 0x44;
			if (strstr (boot_method, "cmsis") != NULL)
			{
				/*  0xf8 to indicate that ROM Code shall boot CMSIS way */
				header[0] |= 0xb0;
			}
			break;
		case a_none: /* paola none */
			header_len = ANGELA_HEADER_LEN;
			header[0] = 0xff;
			header[1] = 0x49;
			header[2] = 0x53;
			header[3] = 0x57;
			header[4] = 0x45;
			header[5] = 0x44;
			header[6] = 0x47;
			header[7] = 0x44;
			break;
			
		case a_ecdsa:	/* angela/lhassa/clara/maria */
			header_len = ANGELA_HEADER_LEN;
			header[0] = 0x48;
			header[1] = 0x49;
			header[2] = 0x53;
			header[3] = 0x57;
			header[4] = 0x45;
			header[5] = 0x44;
			header[6] = 0x47;
			header[7] = 0x44;
			break;
		default:
			print_error ("Bad Algorithm %d\n",algro);
			return EXIT_FAILURE;
	}
	
	offset = 8;
	/* 1.2.5 */
	/* copy version (32bits) */
	/* copy byte per byte version to header, starting from msB to lsB */
	for (i = 0; i < HEADER_VERSION_LEN; i++, offset++)
		header[offset] = version >> ((HEADER_VERSION_LEN - i - 1) * 8) & 0xff;
	/* binary load address (32 bits) */
	/* check load_address_string is ok, i.e. 8 chars <=> 4 hex bytes <=> 32bits */
	if (strlen (load_address_string) != HEADER_LOAD_ADDRESS_LEN * 2)
	{
		print_error ("on load_address len <%s>: is %d and shall be %d\n",
			     load_address_string, (int) strlen (load_address_string), HEADER_SYNC_LEN);
		return EXIT_FAILURE;
	}
	for (i = 0; i < (int) strlen (load_address_string); i += 2, offset++)
		if (EXIT_SUCCESS == test_hex (load_address_string[i], load_address_string[i + 1]))
			header[offset] = hex (load_address_string[i], load_address_string[i + 1]);
		else
		{
			print_error (" non hexa char detected in string <%s> <%c%c>\n",
				     load_address_string, load_address_string[i], load_address_string[i + 1]);
			return EXIT_FAILURE;
		}
	if (sizeof (*payload_len) != HEADER_BINARY_LEN)
	{
		print_error (" payload len size (%d bytes) is not %d bytes\n", (int) sizeof (*payload_len), HEADER_BINARY_LEN);
		return EXIT_FAILURE;
	}
	/* copy byte per byte payload_len to header, starting from msB to lsB */
	for (i = 0; i < HEADER_BINARY_LEN; i++, offset++)
		header[offset] = (*payload_len >> ((HEADER_BINARY_LEN - i - 1) * 8)) & 0xff;
	/* binary jump address (32 bits) */
	/* check jump_address_string is ok, i.e. 8 chars <=> 4 hex bytes <=> 32bits */
	if (strlen (jump_address_string) != HEADER_JUMP_ADDRESS_LEN * 2)
	{
		print_error ("on jump_address len <%s>: is %d and shall be %d\n",
			     jump_address_string, (int) strlen (jump_address_string), HEADER_SYNC_LEN);
		return EXIT_FAILURE;
	}
	for (i = 0; i < (int) strlen (jump_address_string); i += 2, offset++)
		if (EXIT_SUCCESS == test_hex (jump_address_string[i], jump_address_string[i + 1]))
			header[offset] = hex (jump_address_string[i], jump_address_string[i + 1]);
		else
		{
			print_error (" non hexa char detected in string <%s> <%c%c>\n",
				     jump_address_string, jump_address_string[i], jump_address_string[i + 1]);
			return EXIT_FAILURE;
		}
	arguments_len = (u32) strlen (arguments);
	/* copy byte per byte arguments_len to header, starting from msB to lsB */
	for (i = 0; i < HEADER_ARGV_LEN; i++, offset++)
		header[offset] = arguments_len >> ((HEADER_ARGV_LEN - i - 1) * 8) & 0xff;
	
	switch(algro){
		case a_rsa: /* flora */
			/* copy general_information byte -removed- */
			/*   header[offset]=general_info; */
			/* offset++; */
			/* copy sr_papd (1byte) */
			header[offset] = sr_papd;
			offset++;
			/* copy sr_prfsh (4bytes) */
			/* copy byte per byte sr_prfsh to header, starting from msB to lsB */
			for (i = 0; i < HEADER_SR_PRFSH_LEN; i++, offset++)
				header[offset] = sr_prfsh >> ((HEADER_SR_PRFSH_LEN - i - 1) * 8) & 0xff;
			/* copy sr_pcfg (4bytes) */
			/* copy byte per byte sr_pcfg to header, starting from msB to lsB */
			for (i = 0; i < HEADER_SR_PCFG_LEN; i++, offset++)
				header[offset] = sr_pcfg >> ((HEADER_SR_PCFG_LEN - i - 1) * 8) & 0xff;
			/* 1.2.5 */
			/* copy sr_pext (1byte) */
			header[offset] = sr_pext;
			offset++;
			/* copy dmc_gcfg(4bytes) */
			/* copy byte per byte dmc_pcfg to header, starting from msB to lsB */
			for (i = 0; i < HEADER_DMC_GCFG_LEN; i++, offset++)
				header[offset] = dmc_gcfg >> ((HEADER_DMC_GCFG_LEN - i - 1) * 8) & 0xff;
			/* copy dmc_clk(1byte) */
			/* 1.2.6: 1byte and not 2 */
			/* copy byte per byte dmc_clk to header, starting from msB to lsB */
			/*   for(i=0;i<HEADER_DMC_CLK_LEN;i++,offset++) */
			/*  header[offset]=dmc_clk>>((HEADER_DMC_CLK_LEN-i-1)*8)&0xff; */
			header[offset] = dmc_clk;
			offset++;
			/* 1.2.7: ksrc renamed */
			/* copy uci0_ksrc_configencint */
			header[offset] = uci0_ksrc_configencint;
			offset++;
			/* copy uci0_ac1r_start_offset */
			/* copy byte per byte uci0_ac1r_so to header, starting from msB to lsB */
			for (i = 0; i < HEADER_UCI0_AC1R_START_OFFSET_LEN; i++, offset++)
				header[offset] = uci0_ac1r_so >> ((HEADER_UCI0_AC1R_START_OFFSET_LEN - i - 1) * 8) & 0xff;
			/* copy uci0_ac1r_end_offset */
			/* copy byte per byte uci0_ac1r_eo to header, starting from msB to lsB */
			for (i = 0; i < HEADER_UCI0_AC1R_END_OFFSET_LEN; i++, offset++)
				header[offset] = uci0_ac1r_eo >> ((HEADER_UCI0_AC1R_END_OFFSET_LEN - i - 1) * 8) & 0xff;
			/* copy uci0_ddr_r0 */
			/* copy byte per byte uci0_ddr_r0 to header, starting from msB to lsB */
			for (i = 0; i < HEADER_UCI0_DDR_R0_LEN; i++, offset++)
				header[offset] = uci0_ddr_r0 >> ((HEADER_UCI0_DDR_R0_LEN - i - 1) * 8) & 0xff;
			/* 1.2.8: ky1r removed */
			/* copy ky1r */
			/* copy byte per byte ky1r to header, */
			/*   for(i=0;i<HEADER_KY1R_LEN;i++,offset++) */
			/* header[offset]=ky1r[i]; */
			break;
			
		case a_ecdsa:
		case a_sha256:
		case a_crc32:
		case a_none:
		
			for (i = 0; i < HEADER_APPLICATION_VERSION_LEN; i++, offset++)
			{
				header[offset] = application_version >> ((HEADER_APPLICATION_VERSION_LEN - i - 1) * 8) & 0xff;
			}
			break;
		case a_rsa_paola:
	
			for (i = 0; i < HEADER_APPLICATION_VERSION_LEN; i++, offset++)
			{
				header[offset] = application_version >> ((HEADER_APPLICATION_VERSION_LEN - i - 1) * 8) & 0xff;
			}
			break;
		default:
	
			print_error (" header algo has not been identified.\n");
			return EXIT_FAILURE;
			break;
	}
	
	print_debug ("header:");
	for (i = 0; i < header_len; i++)
		print_d ("%02x", header[i]);
	print_d ("\n");
	print_d ("header len: %d\n", header_len);
	
	/* shift the payload to accept the arguments */
	for (i = *payload_len - 1; i >= 0; i--)
		payload[i + arguments_len] = payload[i];
	/* put the arguments in the empty space */
	for (i = 0; i < arguments_len; i++)
		payload[i] = arguments[i];
	
		print_debug("binary after arguments\n");
		for (i = 0; i < (int) (*payload_len + arguments_len); i++)
			print_d("%02x", payload[i]);
		print_d("\n");
	
	
		print_debug ("payload len was %d bytes\n", *payload_len);
		*payload_len = *payload_len + arguments_len;

	print_debug ("payload len is now %d bytes\n", *payload_len);

	/* shift the payload to accept the header */
	for (i = *payload_len - 1; i >= 0; i--)
		payload[i + header_len] = payload[i];
	/* put the header in the empty space */
	for (i = 0; i < header_len; i++)
		payload[i] = header[i];
	print_debug ("payload len was %d bytes\n", *payload_len);

	*payload_len = *payload_len + header_len;
	
		print_debug ("binary after HEADER\n");
		for (i = 0; i < (int) (*payload_len); i++)
			print_d ("%02x", payload[i]);
		print_debug ("\n");
	
	print_debug ("payload len is now %d bytes\n", *payload_len);

	return EXIT_SUCCESS;
}

int read_binary_file (u8 * p_pucData, int *size, char *filename)
{
	int i = 0;
	int resu = EXIT_SUCCESS;
	FILE *pFile = NULL;
	print_debug ("<read_binary_file <%s>>\n", filename);
	pFile = fopen (filename, "rb");
	if (pFile == NULL)
	{
		print_error ("on opening <%s>\n", filename);
		return EXIT_FAILURE;
	}
	/*  obtain file size: */
	fseek (pFile, 0, SEEK_END);
	*size = ftell (pFile);
	rewind (pFile);
	/*  copy the file into the buffer: */
	resu = fread (p_pucData, 1, *size, pFile);
	if (resu != *size)
	{
		print_error ("Reading error <%s>\n", filename);
		return EXIT_FAILURE;
	}
	
	print_debug ("%d bytes read\n", *size);
	for (i = 0; i < (int) (*size); i++)
		print_d ("%02x", p_pucData[i]);
	print_d ("\n");
	
	fclose (pFile);
	return EXIT_SUCCESS;
}

int read_file_size (u32 * size, char *filename)
{
	FILE *pFile = NULL;
	print_debug ("<read_file_size <%s>>\n", filename);
	pFile = fopen (filename, "rb");
	if (pFile == NULL)
	{
		print_error ("File error\n");
		return EXIT_FAILURE;
	}
	/** obtain file size */
	fseek (pFile, 0, SEEK_END);
	*size = ftell (pFile);
	rewind (pFile);
	/**  */
	print_debug("%s %d\n", filename, *size);
	fclose (pFile);
	/** We're done */
	return EXIT_SUCCESS;
}

/* this function retrieves (and checks if possible) parameters provided in the ini file */
int process_arg (char *line, int fgets_correction)
{
	int resu;
	found = 0;
	/*  resu=process_hexvalue(&general_info,"general_info",line,255,fgets_correction);
	   if ( EXIT_SUCCESS != resu )
	   {
	   print_error("while extracting <general_info> field\n");
	   return EXIT_FAILURE;
	   } */
	resu = process_longhexvalue (&application_version, "application_version", line, 0xffffffff, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <version> field\n");
		return EXIT_FAILURE;
	}
	resu = process_hexvalue (&sr_papd, "sr_papd", line, 255, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <sr_papd> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.5 */
	resu = process_hexvalue (&sr_pext, "sr_pext", line, 255, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <sr_pext> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.6: 4bytes */
	resu = process_longhexvalue (&sr_prfsh, "sr_prfsh", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <sr_prfsh> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.5 */
	resu = process_longhexvalue (&version, "version", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <version> field\n");
		return EXIT_FAILURE;
	}
	resu = process_longhexvalue (&sr_pcfg, "sr_pcfg", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <sr_pcfg> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.6: 4bytes */
	resu = process_longhexvalue (&dmc_gcfg, "dmc_gcfg", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <dmc_gcfg> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.6: 1byte */
	/*   resu=process_longhexvalue(&dmc_clk,"dmc_clk",line,255,fgets_correction); */
	resu = process_hexvalue (&dmc_clk, "dmc_clk", line, 255, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <dmc_clk> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.7: ksrc renamed */
	resu = process_hexvalue (&uci0_ksrc_configencint, "uci0_ksrc_configencint", line, 255, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		/* 1.2.7: ksrc renamed */
		print_error ("while extracting <uci0_ksrc_configencint> field\n");
		return EXIT_FAILURE;
	}
	resu = process_longhexvalue (&uci0_ac1r_so, "uci0_ac1r_so", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <uci0_ac1r_so> field\n");
		return EXIT_FAILURE;
	}
	resu = process_longhexvalue (&uci0_ac1r_eo, "uci0_ac1r_eo", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <uci0_ac1r_eo> field\n");
		return EXIT_FAILURE;
	}
	resu = process_longhexvalue (&uci0_ddr_r0, "uci0_ddr_r0", line, 0xFFFFFFFF, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <uci0_ddr_r0> field\n");
		return EXIT_FAILURE;
	}
	/* 1.2.8: ky1r removed */
	/*   resu=process_hextab(ky1r,"ky1r",line,fgets_correction); */
	/* if ( EXIT_SUCCESS != resu ) */
	/* { */
	/*   print_error("while extracting <ky1r> field\n"); */
	/*   return EXIT_FAILURE; */
	/* } */
	resu = process_string (rsafile, "rsa_file", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <rsa_file> field\n");
		return EXIT_FAILURE;
	}
	resu = process_string (ecdsafile, "ecdsa_file", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <ecdsa_file> field\n");
		return EXIT_FAILURE;
	}
	resu = process_string (algo, "algo", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <algo> field\n");
		return EXIT_FAILURE;
	}
	if (strstr (line, "verbose") != NULL)
	{
		verbose = (strstr (line, "yes") != NULL) ? 4 : 2;
		found = 1;
	}
	if (strstr (line, "signonly") != NULL)
	{
		signonly = (strstr (line, "yes") != NULL) ? TRUE : FALSE;
		found = 1;
	}
	if (strstr (line, "header") != NULL)
	{
		headergenerated = (strstr (line, "yes") != NULL) ? TRUE : FALSE;
		found = 1;
	}
	resu = process_string (cafile, "ca", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <ca> field\n");
		return EXIT_FAILURE;
	}
	resu = process_string (load_address_string, "load_address", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <load_address> field\n");
		return EXIT_FAILURE;
	}
	resu = process_string (jump_address_string, "jump_address", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <jump_address> field\n");
		return EXIT_FAILURE;
	}
	resu = process_string (arguments, "arguments", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <arguments> field\n");
		return EXIT_FAILURE;
	}
	resu = process_string (scafile, "sca", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("while extracting <sca> field\n");
		return EXIT_FAILURE;
	}
#ifdef _MAXIM_HSM
	resu = process_string (config_struct.HSM_KeyLabel, "hsm_key_name=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error (" while extracting <hsm_name_key> field\n");
		return (EXIT_FAILURE);
	}
	resu = process_string (config_struct.hsm_thales_dll, "hsm_thales_dll=", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("While extracting <hsm_thales_dll> field\n");
		return (EXIT_FAILURE);
	}
	process_intvalue (&config_struct.hsm_slot_nb, "hsm_slot_nb=", line, 10, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("While extracting <hsm_slot_nb> field\n");
		return (EXIT_FAILURE);
	}
#endif /* _MAXIM_HSM */
	/* 1.6.2 : Retrieve 'boot_method' parameter */
	resu = process_string (boot_method, "boot_method", line, fgets_correction);
	if (EXIT_SUCCESS != resu)
	{
		print_error ("While extracting <boot_method> field\n");
		return EXIT_FAILURE;
	}
	if (!found)
	{
		print_error ("Line with unknown field: <%s>\n", line);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

int load_args (int argc, char **argv)
{
	int k;
	int resu;
	for (k = 1; k < argc; k++)
	{
		resu = process_arg (argv[k], 0);
		if (EXIT_SUCCESS != resu)
			return EXIT_FAILURE;
	}
	

	for (k = 0; k < (int) strlen (algo); k++)
		algo[k] = (char) tolower ((int) algo[k]);
	
	for (k = 0; k < (int) (sizeof(algo_name)/8); k++){
	
		if(strstr(algo,algo_name[k]) != NULL){
			algro = k;
		}
	
	}

	return EXIT_SUCCESS;
}

int process (void)
{
	int resu = 0;
	int err = 0;
	int signature_len;
	unsigned int payload_len;
	unsigned char *payload;
	unsigned char signature[2048];
	ecdsa_key_t ecdsaKey;
	ucl_rsa_public_key_t keyPu;
	ucl_rsa_private_key_t keyPr;
	char *ptr;

	/**  */
	keyPr.modulus_length = PAOLA_SIGNATURE_LEN;
	keyPu.public_exponent_length = EXP_BYTE_LEN;
	keyPr.private_exponent = malloc (PAOLA_SIGNATURE_LEN);
	config_struct.rsa_len = PAOLA_SIGNATURE_LEN;
	config_struct.ecdsa_len = ECDSA_MODULUS_LEN;
	ecdsaKey.ecdsa_len = ECDSA_MODULUS_LEN;
	
	switch(algro){
		case a_rsa: 
		case a_rsa_paola:	/** RSA-2048/4096 *********************************/
	
	#ifndef _MAXIM_HSM
			/** Reading the signing key with highest length value */
			resu = read_file_rsa (keyPu.modulus,
								  &(keyPr.modulus_length),
								  keyPu.public_exponent,
								  keyPr.private_exponent,
								  keyPu.public_exponent_length,
								  rsafile);
			if ( EXIT_SUCCESS != resu )
			{
				/** Try with smallest key */
				keyPr.modulus_length = FLORA_SIGNATURE_LEN;
				keyPr.private_exponent = malloc (FLORA_SIGNATURE_LEN);
				config_struct.rsa_len = FLORA_SIGNATURE_LEN;
				resu = read_file_rsa (keyPu.modulus,
									  &(keyPr.modulus_length),
									  keyPu.public_exponent,
									  keyPr.private_exponent,
									  keyPu.public_exponent_length,
									  rsafile);
			}
			/**  */
			keyPu.modulus_length = keyPr.modulus_length;
	#else /* 
		   */
			resu = load_HSM_rsa_key (&config_struct, &keyPu, config_struct.HSM_KeyLabel);
	#endif /* _MAXIM_HSM */
			keyPr.modulus = keyPu.modulus;
			config_struct.rsa_len = keyPu.modulus_length;
			if ( EXIT_SUCCESS != resu )
			{
				print_error ("in read_file_rsa\n");
				return EXIT_FAILURE;
			}
			break;
			
		case a_ecdsa: 		/** ECDSA-256 *************************************/
	
	#ifndef _MAXIM_HSM
			/** Reading the signing key */
			resu = read_file_ecdsa (ecdsaKey.ecdsa_pubkey_x,
						ecdsaKey.ecdsa_pubkey_y, ecdsaKey.ecdsa_privkey, ecdsaKey.ecdsa_len, ecdsafile);
	#else /* 
		   */
			resu = load_HSM_ecdsa_key (&ecdsaKey, config_struct.HSM_KeyLabel);
	#endif /* _MAXIM_HSM */
			if (EXIT_SUCCESS != resu)
			{
				print_error ("in read_file_ecdsa\n");
				return EXIT_FAILURE;
			}
			break;
			
		case a_sha256:		/** HASH - SHA256 *********************************/
	
			/** Setting "signature" length */
			config_struct.rsa_len = UCL_SHA256_HASHSIZE;
			resu = EXIT_SUCCESS;
			break;
	
		case a_crc32: 		/** CRC32 *****************************************/
	
			/** Setting "signature" length */
			config_struct.rsa_len = sizeof (unsigned int);
			resu = EXIT_SUCCESS;
			break;
	
		case a_none:		/** Nothing ***************************************/
		
			/** Setting "signature" length */
			config_struct.rsa_len = 0;
			resu = EXIT_SUCCESS;
			break;
		default:
			print_error("Unknown signature algorithm\n");
			return EXIT_FAILURE;
			break;
	}
	
	/** Reading the binary file to be signed
	 * file is read and data are put in payload, payload_len */
	err = read_file_size (&payload_len, cafile);
	if (EXIT_SUCCESS != err)
	{
		return EXIT_FAILURE;
	}
	
	/** Memory space allocation */
	payload = (u8 *) malloc ((MAX_HEADER_LEN + payload_len + strlen (arguments)) * sizeof (u8));
	if (NULL == payload)
	{
		print_error ("Unable to allocate memory for payload (%d bytes required)\n", payload_len);
		return EXIT_FAILURE;
	}
	
	print_debug("Memory allocation for all binary (except signautre if any) done : 0x"
			SSIZET_XFMT "\n", (MAX_HEADER_LEN + payload_len + strlen (arguments)));
	
	/** Read binary */
	err = read_binary_file (payload, (int *) &payload_len, cafile);
	if (EXIT_SUCCESS != err)
	{
		print_error ("read_binary_file(%s)=%d\n", cafile, err);
		return EXIT_FAILURE;
	}
	
	print_debug("Algorithm : %s \n",algo_name[algro]);

	/**  */
	if (TRUE == headergenerated)
	{
		err = add_header_to_payload (payload, &payload_len);
		if (EXIT_SUCCESS != err)
		{
			print_error ("add_header(%d)\n", err);
			return EXIT_FAILURE;
		}
	}
	else
	{
		print_info("header not generated as instructed\n");
	}
	
	
	switch(algro){
		case a_rsa: 		/** RSA-2048/4096 *********************************/
		case a_rsa_paola:
		
			print_info("rsa len=%d\n", config_struct.rsa_len);
			err = rsa_sign (payload, payload_len, signature, keyPu, keyPr);
			if (EXIT_SUCCESS != err)
			{
				print_error ("rsa_sign(%d)\n", err);
				return EXIT_FAILURE;
			}
			
			signature_len = config_struct.rsa_len;
			break;
			
		case a_ecdsa:		/** ECDSA-256 *************************************/
	
			err = ecdsa_sign (payload, payload_len, signature, ecdsaKey);
			if (EXIT_SUCCESS != err)
			{
				print_error ("ecdsa_sign(%d)\n", err);
				return EXIT_FAILURE;
			}
			
			signature_len = ANGELA_SIGNATURE_LEN;
			break;
	
		case a_sha256:		/** Hash - SHA256 *********************************/

			print_info("hash length = %d\n", config_struct.rsa_len);
			err = hash_sign_payload ((unsigned char *) signature, payload, payload_len);
			
			if (EXIT_SUCCESS != err)
			{
				print_error ("rsa_sign(%d)\n", err);
				return EXIT_FAILURE;
			}
			
			signature_len = config_struct.rsa_len;
			break;
			
		case a_crc32:	/** CRC32 *****************************************/
	
			print_info("crc32 length = %d\n", config_struct.rsa_len);
			err = crc32_sign_payload ((unsigned int *) signature, payload, payload_len);
			
			if (EXIT_SUCCESS != err)
			{
				print_error ("crc32_sign(%d)\n", err);
				return EXIT_FAILURE;
			}
			
			signature_len = config_struct.rsa_len;
			break;
			
		case a_none:	/** No signature **********************************/
			
			print_info("No signature therefore no length.\n");
			signature_len = 0;
			break;
			
		default:
			print_warn("Algo problem : %s\n", algo_name[algro]);
			break;
	}
	
	
	print_debug ("write_signature\n");
	
	ptr = strrchr(scafile,'.');
	if (NULL != ptr) {
		memset(tmp_str,0,sizeof(tmp_str));
		memcpy(tmp_str,scafile,ptr-scafile);
		sprintf (sigfile, "%s.sig", tmp_str);
	} else {
		sprintf (sigfile, "%s.sig", scafile);
	} 
	err = write_signature (sigfile, signature, signature_len);
	if (TRUE != signonly)
	{
		print_debug ("write_file\n");
		err = write_file (scafile, payload, payload_len, signature, signature_len);
	}
	
	/** Free memory */
	free (payload);
	if (EXIT_SUCCESS != err)
	{
		print_error ("Write(%d)\n", err);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int load_default_config (void)
{
	verbose = 5;
	sprintf (cafile, "file.bin");
	sprintf (scafile, "file.sbin");
	sprintf (rsafile, "crk_rsa_2048bits_test1.key");
	sprintf (load_address_string, "01020304");
	sprintf (jump_address_string, "02030405");
	sprintf (arguments, "now, this is time for all");
	algro = a_rsa;
	version = 0x010203ff;
	sr_papd = 0xac;
	sr_prfsh = 0xabcdef01;
	sr_pcfg = 0x01efcdab;
	sr_pext = 0xa3;
	dmc_gcfg = 0x02030405;
	dmc_clk = 0x04;
	uci2_ctrl_reg = 0x040506cd;
	return EXIT_SUCCESS;
}

int load_ini_config (FILE * fp)
{
	char line[MAXLINE];
	int resu;
	unsigned int k, fcorection;
	print_debug("< load .ini config >\n"); 
	while (fgets (line, MAXLINE, fp) != NULL)
	{
		if (line[strlen (line)] != '\0')
		{
			print_error ("Overflow on line <%s>\n", line);
			return EXIT_FAILURE;
		}
		
		if (line[strlen (line)-1] != '\n')
		{
			fcorection = 0;
		}else{
			fcorection = 1;
		}
		
		if ('#' == line[0])
			continue;
		if ('\r' == line[0] || '\n' == line[0] )
			continue;
		resu = process_arg (line, fcorection);
		if (EXIT_SUCCESS != resu)
			return EXIT_FAILURE;
	}
	
	for (k = 0; k < (unsigned int) strlen (algo); k++)
		algo[k] = (char) tolower ((int) algo[k]);
	
	for (k = 0; k < (unsigned int) (sizeof(algo_name)/8); k++){
		if(strstr(algo,algo_name[k]) != NULL){
			algro = k;
			}
	
	}
	
	return EXIT_SUCCESS;
}

/* this function reads the .ini and configures the parameters */
int load_config (void)
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
		print_warn ("configuration file <%s> not found\n", INIFILE);
	}
	else
	{
		resu = load_ini_config (fp);
		if (EXIT_SUCCESS != resu)
			return EXIT_FAILURE;
		(void) fclose (fp);
	}
	return EXIT_SUCCESS;
}

int display_config (void)
{
	printf ("Config :\n");
	if (TRUE <= verbose)
		printf ("\tverbose\n");
	else
		printf ("\tmute\n");
	
	if (TRUE == signonly)
		printf ("\tsignature file generation only\n");
	else
		printf ("\tsigned application file generation\n");
	
	if (TRUE == headergenerated)
		printf ("\tthe secure header is generated\n");
	else
		printf ("\tthe secure header is not generated\n");
	
	printf ("\tversion:\t %08x\n", version);
	
#ifndef _MAXIM_HSM
	if (algro == a_rsa || algro == a_rsa_paola)
		printf ("\trsa file:\t %s\n", rsafile);
	if (algro == a_ecdsa)
		printf ("\tecdsa file:\t %s\n", ecdsafile);
#else
	printf ("\tHSM key name :\t %s\n", config_struct.HSM_KeyLabel);
	printf ("\tHSM Thales DLL path :\t %s\n", config_struct.hsm_thales_dll);
	printf ("\tHSM Slot number :\t %d\n", config_struct.hsm_slot_nb);
		
#endif /* _MAXIM_HSM */

	printf ("\tcustomer application (input) file:\t %s\n", cafile);
	printf ("\tsigned customer application (output) file:\t %s\n", scafile);
	printf ("\tbinary load address:\t %s\n", load_address_string);
	printf ("\tbinary jump address:\t %s\n", jump_address_string);
	printf ("\tapplication arguments:\t <%s> \n", arguments);
	printf ("\n\tAlgorithm : %s \n",algo_name[algro]);
	
			
	if (algro == a_ecdsa)
	{
		printf ("application version: %08x\n", application_version);
	}
	
	if (algro == a_rsa)	/* i.e. only flora */
	{
		printf ("\tDynamic Memory Slot:\n");
		printf ("\t\tSR_PAPD:\t %02x\n", sr_papd);
		printf ("\t\tSR_PRFSH:\t %08x\n", sr_prfsh);
		printf ("\t\tSR_PCFG:\t %08x\n", sr_pcfg);
		printf ("\t\tSR_PEXT:\t %02x\n", sr_pext);
		printf ("\t\tMEM_GCFG:\t %08x\n", dmc_gcfg);
		printf ("\t\tDMC_CLK:\t %02x\n", dmc_clk);
		printf ("\tUCI2 parameters\n");
		printf ("\t\tKSRC-Config ENC-INT:\t %02x\n", uci0_ksrc_configencint);
		printf ("\t\tuci0-AC1R-start-offset:\t %08x\n", uci0_ac1r_so);
		printf ("\t\tuci0-AC1R-end-offset:\t %08x\n", uci0_ac1r_eo);
		printf ("\t\tuci0-DDR-r0:\t %08x\n", uci0_ddr_r0);
	}
	return EXIT_SUCCESS;
}

int init (void)
{
	int err = EXIT_SUCCESS;
	err = ucl_init (init_buffer, 2048);
	if (err != UCL_OK)
	{
		print_error ("for ucl_init %d\n", err);
		return EXIT_FAILURE;
	}
#ifdef _MAXIM_HSM
	/* Initialise DLL, giving the path of Thales cknfast DLL  */
	err = HSM_InitDLL (config_struct.hsm_thales_dll);
	if (err != CKR_OK)
	{
		print_error ("Unable to Initialise DLL \n");
		HSM_pError (err);
		return err;
	}
#endif /* _MAXIM_HSM */
	return err;
}



int main (int argc, char **argv)
{
	int resu;
#ifdef _MAXIM_HSM
	int i;
#endif 	

#ifdef __WIN	
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif 
	
	printf ("CA signature build v%d.%d.%d (build %d) (c)Maxim Integrated 2006-2017\n", MAJV, MINV, ZVER, BUILD);
#ifdef _MAXIM_HSM
	print_info ("This tool does handle keys with Thales nCipher Edge HSM \n");
	print_info ("LibHSM Version: %s (%s)\n", HSM_GetVersion (), HSM_GetBuildDate ());
#else /* 
       */
	print_warn ("This tool does not handle keys in a PCI-PTS compliant way, only for test \n");
	print_info ("UCL Version: %s (%s)\n", (char *) ucl_get_version (), (char *) ucl_get_build_date ());
#endif	/*_MAXIM_HSM */
	resu = load_config ();
	if (EXIT_SUCCESS != resu)
	{
		return EXIT_FAILURE;
	}
	/**  */
	resu = load_args (argc, argv);
	if (EXIT_SUCCESS != resu)
	{
		return EXIT_FAILURE;
	}
	/**  */
	if (4 <= verbose)
	{
		resu = display_config ();
		/**  */
		if (EXIT_SUCCESS != resu)
		{
			return EXIT_FAILURE;
		}
	}
	/**  */
	if (EXIT_SUCCESS != init ())
	{
		return EXIT_FAILURE;
	}
#ifdef _MAXIM_HSM
	print_info ("HSM try open connection\n");
	/* Open HSM Session and Login using OCS Cards */
	resu = CKR_GENERAL_ERROR;
	for(i = 0; (i<3 && resu != CKR_OK); i++)
	{
		resu = HSM_Login (&session, config_struct.hsm_slot_nb);
	}
	if (EXIT_SUCCESS != resu)
	{
		print_error ("Unable to open connection with HSM.\n");
		HSM_pError (resu);
		return (EXIT_FAILURE);
	}
#endif /* _MAXIM_HSM */
	resu = process ();
	if (EXIT_SUCCESS != resu)
	{
#ifdef _MAXIM_HSM
		/* Close HSM Sessions */
		resu = HSM_Close (session);
		if (EXIT_SUCCESS != resu)
		{
			print_warn ("Warning: fails to close HSM connection\n");
		}
#endif /* _MAXIM_HSM */
		return EXIT_FAILURE;
	}
#ifdef _MAXIM_HSM
	resu = HSM_Close (session);
	if (EXIT_SUCCESS != resu)
	{
		print_warn ("Warning: fails to close HSM connection\n");
	}
#endif /* _MAXIM_HSM */

	printf ("\nCustomer application successfully signed\n");

	/**  */
	return EXIT_SUCCESS;
}
