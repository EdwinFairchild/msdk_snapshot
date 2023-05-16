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

#include <ca_sign_build.h>
#include <maxim_c_utils.h>

#include <ucl/ucl_pkcs1_ssa_pss_sha256.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_defs.h>



/* byte length of the CRK public exponent */
#define RSA_MODULUS_LEN					256
#define PAOLA_RSA_MODULUS_LEN           512

int ucl_pkcs1_emsa_pss_sha256_encode (u8 * EM, u8 * M, u32 M_length, u32 salt_length, u32 k, int offset);


int rsa_sign (uint8_t * input, unsigned int input_size, uint8_t * signature, 
				ucl_rsa_public_key_t keyPu, ucl_rsa_private_key_t keyPr)
{
	unsigned int i = 0;
	int err = 0;
	int sl = 0;		/* salt length is 0 */

#ifndef _MAXIM_HSM

	
#else

	long unsigned int l_iSignatureLength = config_struct.rsa_len;
	uint8_t EncodedMessage[MAX_RSA];
	uint8_t lastbyte;
	int offset;
	uint32_t tmp;
#endif /* _MAXIM_HSM */

	print_debug ("rsa len=%d bytes\n", keyPr.modulus_length);

#ifndef _MAXIM_HSM

	err = ucl_pkcs1_ssa_pss_sha256_sign (signature, input, input_size, &keyPr, sl);

	if (err != UCL_OK)
	{
		print_error("rsa pkcs1 sha256 sign (%d) %d %d\n", err, input_size, sl);
		return (EXIT_FAILURE);
	}

#else

	ucl_data_os2int ((uint32_t *) EncodedMessage, keyPr.modulus_length / 4, keyPu.modulus, keyPr.modulus_length);

	tmp = ((u32 *) (EncodedMessage))[(keyPr.modulus_length / 4) - 1];
	lastbyte = (u8) (tmp >> 24);
	offset = 9;
	while (0 != lastbyte)
	{
		lastbyte >>= 1;
		offset--;
	}

	err = ucl_pkcs1_emsa_pss_sha256_encode (EncodedMessage, input, input_size, sl, keyPr.modulus_length, offset);
	if (err != UCL_OK)
	{
		print_error("on ucl_pkcs1_emsa_pss_sha256_encode (%d)\n", err);
		return (EXIT_FAILURE);
	}

	err = HSM_DecryptRSA (session, EncodedMessage, keyPr.modulus_length, signature, &l_iSignatureLength,
			      config_struct.HSM_Objkey);

	if (err != UCL_OK)
	{
		print_error("on HSM_DecryptRSA (0x%x)\n", err);
		return (EXIT_FAILURE);
	}

#endif /* _MAXIM_HSM */

	err = ucl_pkcs1_ssa_pss_sha256_verify (signature, input, input_size, &keyPu, sl);

	if (err != UCL_OK)
	{
		print_error("in verify signature (%d)\n", err);
		return (EXIT_FAILURE);
	}
	else
	{
		print_debug("pkcs1 sha256 ok\n");
	}

	print_debug("signature:");
	for (i = 0; i < keyPr.modulus_length; i++)
		print_d("%02x", signature[i]);
	print_d ("\n");
	
	return (EXIT_SUCCESS);
}

/*read signed rsa public key file
 *contains: rsa modulus, public exponent and signature
 *note: rsa modulus and signature have same length
 * #4596: support of mixed rsa2048 signed by rsa4096
 */
int read_file_signed_rsa_publickey (u8 * puk, int *puksize, u8 * pukexp, int expsize, u8 * signature, int *sigsize,
				    char *filename)
{
	FILE *fp;
	int i;
	int resu;
	unsigned int filesize;
	char line[MAXLINE];

	u8 d8 = 0x00;

	if (4 <= verbose)
		printf ("<read_file_signed_rsa_puk <%s>>\n", filename);

	if (EXIT_SUCCESS != determine_file_size (&filesize, filename))
	{
		printf ("ERROR with <%s>\n", filename);
		return (EXIT_FAILURE);
	}
	printf("\tfilesize = %d\n", filesize);
	/* convert filesize from nb of chars to nb of bytes */
	filesize /= 2;
	/* 4096 signed by 4096 */
	if ( ( PAOLA_RSA_MODULUS_LEN * 2 ) < filesize )
	{
		*sigsize = *puksize = PAOLA_RSA_MODULUS_LEN;
	}
	/* 2048 signed by 4096 */
	else if ( ( PAOLA_RSA_MODULUS_LEN + RSA_MODULUS_LEN ) < filesize)
	{
		*puksize = RSA_MODULUS_LEN;
		*sigsize = PAOLA_RSA_MODULUS_LEN;
	}
	/* 2048 signed by 2048 */
	else if ( ( 2 * RSA_MODULUS_LEN ) < filesize )
	{
		*puksize = RSA_MODULUS_LEN;
		*sigsize = RSA_MODULUS_LEN;
	}
	else
	{
		printf ("ERROR: file size is incorrect %d bytes\n", filesize);
		return (EXIT_FAILURE);
	}
	printf ("puksize=%d sigsize=%d\n", *puksize, *sigsize);
	fp = fopen (filename, "r");
	if (NULL == fp)
	{
		printf ("ERROR on opening <%s>\n", filename);
		return (EXIT_FAILURE);
	}
	memset (puk, 0, *puksize);
	for (i = 0; i < *puksize; i++)
	{
		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", *puksize, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}
		puk[i] = d8;

	}
	for (i = 0; i < expsize; i++)
		pukexp[i] = 0;
	for (i = 0; i < expsize; i++)
	{
		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", *puksize, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}
		pukexp[i] = d8;

	}
	for (i = 0; i < *sigsize; i++)
		signature[i] = 0;
	for (i = 0; i < *sigsize; i++)
	{

		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", *sigsize, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}
		signature[i] = d8;

	}
	if (4 <= verbose)
	{
		printf ("puk:");
		for (i = 0; i < *puksize; i++)
			printf ("%02x", puk[i]);
		printf ("\n");
		printf ("exp:");
		for (i = 0; i < expsize; i++)
			printf ("%02x", pukexp[i]);
		printf ("\n");
		printf ("signature:");
		for (i = 0; i < *sigsize; i++)
			printf ("%02x", signature[i]);
		printf ("\n");
	}

	(void) fclose (fp);

	return (EXIT_SUCCESS);
}

#ifdef _MAXIM_HSM

int load_HSM_rsa_key (type_config_struct * config, ucl_rsa_public_key_t * keyPu, char *keyname)
{
	CK_RV rv = EXIT_SUCCESS;
	unsigned int i;

	print_debug ("<load_HSM_rsa_key <%s>>\n", keyname);
	print_debug ("HSM_FindKey \n");


	rv = HSM_FindKey (session, keyname, TRUE, &(config->HSM_Objkey));
	
	if (rv != 0)
	{
		print_error(" HSM_FindKey \n");
		HSM_pError (rv);
		return rv;
	}

	rv = HSM_GetRSAPublicKey (session, config->HSM_Objkey, keyPu->modulus, (long unsigned int *) &(keyPu->modulus_length),
				  keyPu->public_exponent, (long unsigned int *) &(keyPu->public_exponent_length));
	if (rv != 0)
	{
		HSM_pError (rv);
	}
	
	
	print_debug ("key name: %s\n",keyname);
	print_debug ("Exponent Length: %d\n",keyPu->public_exponent_length);
	
	print_debug ("Public Exponent :\n");
	for(i=0; i< keyPu->public_exponent_length; i++){
		print_d("%02x",keyPu->public_exponent[i]);
	}
	print_d("\n");
	
	print_debug ("Modulus Length: %d\n",keyPu->modulus_length);
	print_debug ("Modulus :\n");
	for(i=0; i< keyPu->modulus_length; i++){
		print_d("%02x",keyPu->modulus[i]);
	}
	print_d("\n");
	return rv;
}
#endif /* _MAXIM_HSM */

int read_file_rsa (u8 * puk, unsigned int *puksize, u8 * pukexp, u8 * privexp, unsigned int expsize, char *filename)
{
	FILE *l_pFile = NULL;
	int l_iErr = 0;
	unsigned int l_iSize = 0;
	unsigned int l_iIndex = 0;
	unsigned int filesize;

	if (4 <= verbose)
	{
		printf ("<read_file_rsa <%s>>\n", filename);
	}
	if ((filename == NULL) || (strlen (filename) == 0))
	{
		printf ("ERROR invalid rsa file: pointer is null or string paramter is empty\n");
		return (EXIT_FAILURE);
	}

	if (EXIT_SUCCESS != determine_file_size (&filesize, filename))
	{
		printf ("ERROR when opening <%s>\n", filename);
		return (EXIT_FAILURE);
	}

	printf ("filesize=%d\n", filesize);

	if ((*puksize) * 4 > filesize)
	{
		printf ("ERROR: rsa file too small (%d bytes for RSA-%d key)\n", filesize, (*puksize) * 8);
		return (EXIT_FAILURE);
	}
	l_pFile = fopen (filename, "r");

	if (l_pFile == NULL)
	{
		printf ("ERROR on opening rsa file <%s>\n", filename);
		return (EXIT_FAILURE);
	}

	memset (puk, 0, *puksize);
	l_iSize = *puksize;
	l_iErr = read_file_ascii_data (l_pFile, *puksize, puk, &l_iSize);

	if (l_iErr != EXIT_SUCCESS)
	{
		printf ("ERROR: read puk in rsa file error\n");
		return (EXIT_FAILURE);
	}

	/* -- Read first private exponent and then public exponent */
	memset (privexp, 0, *puksize);
	l_iSize = *puksize;
	l_iErr = read_file_ascii_data (l_pFile, *puksize, privexp, &l_iSize);
	if (l_iErr != EXIT_SUCCESS)
	{
		printf ("ERROR: read privexp in rsa file error\n");
		return (EXIT_FAILURE);
	}
	memset (pukexp, 0, expsize);
	l_iSize = expsize;
	l_iErr = read_file_ascii_data (l_pFile, expsize, pukexp, &l_iSize);

	if (l_iErr != EXIT_SUCCESS)
	{
		printf ("ERROR: read pukexp in rsa file error\n");
		return (EXIT_FAILURE);
	}

	if (4 <= verbose)
	{
		printf ("modulus\n");

		for (l_iIndex = 0; l_iIndex < *puksize; l_iIndex++)
			printf ("%02x", puk[l_iIndex]);

		printf ("\n");
		printf ("pub.exp\n");

		for (l_iIndex = 0; l_iIndex < expsize; l_iIndex++)
			printf ("%02x", pukexp[l_iIndex]);

		printf ("\n");
		printf ("priv.exp\n");

		for (l_iIndex = 0; l_iIndex < *puksize; l_iIndex++)
			printf ("%02x", privexp[l_iIndex]);

		printf ("\n");
	}

	fclose (l_pFile);

	return (EXIT_SUCCESS);
}
