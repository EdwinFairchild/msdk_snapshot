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
#include <ctype.h>
#include <string.h>

#include <ucl/ucl_config.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_sys.h>
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_aes.h>
#include <ucl/ucl_sha256.h>
#include <ucl/ecdsa_generic_api.h>

#include <ca_sign_build.h>
#include <maxim_c_utils.h>



int ecdsa_sign (unsigned char *input, unsigned int input_size, unsigned char *signature, ecdsa_key_t key)
{
	uint8_t a[] =
		{ 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC
	};
	/*  uint8_t b[]={0x5A,0xC6,0x35,0xD8,0xAA,0x3A,0x93,0xE7,0xB3,0xEB,0xBD,0x55,0x76,0x98,0x86,0xBC,0x65,0x1D,0x06,0xB0,0xCC,0x53,0xB0,0xF6,0x3B,0xCE,0x3C,0x3E,0x27,0xD2,0x60,0x4B};
	 */
	uint8_t xg[] =
		{ 0x6B, 0x17, 0xD1, 0xF2, 0xE1, 0x2C, 0x42, 0x47, 0xF8, 0xBC, 0xE6, 0xE5, 0x63, 0xA4, 0x40, 0xF2, 0x77, 0x03,
		0x7D, 0x81, 0x2D, 0xEB, 0x33, 0xA0, 0xF4, 0xA1, 0x39, 0x45, 0xD8, 0x98, 0xC2, 0x96
	};
	uint8_t yg[] =
		{ 0x4F, 0xE3, 0x42, 0xE2, 0xFE, 0x1A, 0x7F, 0x9B, 0x8E, 0xE7, 0xEB, 0x4A, 0x7C, 0x0F, 0x9E, 0x16, 0x2B, 0xCE,
		0x33, 0x57, 0x6B, 0x31, 0x5E, 0xCE, 0xCB, 0xB6, 0x40, 0x68, 0x37, 0xBF, 0x51, 0xF5
	};
	uint8_t n[] =
		{ 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBC, 0xE6,
		0xFA, 0xAD, 0xA7, 0x17, 0x9E, 0x84, 0xF3, 0xB9, 0xCA, 0xC2, 0xFC, 0x63, 0x25, 0x51
	};
	uint8_t p[] =
		{ 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
	};
	/* RFC4754 test vector */
	/* secret key */
#ifndef _MAXIM_HSM
	uint8_t d3[] =
		{ 0xDC, 0x51, 0xD3, 0x86, 0x6A, 0x15, 0xBA, 0xCD, 0xE3, 0x3D, 0x96, 0xF9, 0x92, 0xFC, 0xA9, 0x9D, 0xA7, 0xE6,
		0xEF, 0x09, 0x34, 0xE7, 0x09, 0x75, 0x59, 0xC2, 0x7F, 0x16, 0x14, 0xC8, 0x8A, 0x7F
	};
#endif /* _MAXIM_HSM */
	/* message */
	uint8_t msg3[] = { 'a', 'b', 'c' };
	/* public key */
	uint8_t xq3[] =
		{ 0x24, 0x42, 0xA5, 0xCC, 0x0E, 0xCD, 0x01, 0x5F, 0xA3, 0xCA, 0x31, 0xDC, 0x8E, 0x2B, 0xBC, 0x70, 0xBF, 0x42,
		0xD6, 0x0C, 0xBC, 0xA2, 0x00, 0x85, 0xE0, 0x82, 0x2C, 0xB0, 0x42, 0x35, 0xE9, 0x70
	};
	uint8_t yq3[] =
		{ 0x6F, 0xC9, 0x8B, 0xD7, 0xE5, 0x02, 0x11, 0xA4, 0xA2, 0x71, 0x02, 0xFA, 0x35, 0x49, 0xDF, 0x79, 0xEB, 0xCB,
		0x4B, 0xF2, 0x46, 0xB8, 0x09, 0x45, 0xCD, 0xDF, 0xE7, 0xD5, 0x09, 0xBB, 0xFD, 0x7D
	};
	/* signature for the message above */
	uint8_t r3[] =
		{ 0xCB, 0x28, 0xE0, 0x99, 0x9B, 0x9C, 0x77, 0x15, 0xFD, 0x0A, 0x80, 0xD8, 0xE4, 0x7A, 0x77, 0x07, 0x97, 0x16,
		0xCB, 0xBF, 0x91, 0x7D, 0xD7, 0x2E, 0x97, 0x56, 0x6E, 0xA1, 0xC0, 0x66, 0x95, 0x7C
	};
	uint8_t s3[] =
		{ 0x86, 0xFA, 0x3B, 0xB4, 0xE2, 0x6C, 0xAD, 0x5B, 0xF9, 0x0B, 0x7F, 0x81, 0x89, 0x92, 0x56, 0xCE, 0x75, 0x94,
		0xBB, 0x1E, 0xA0, 0xC8, 0x92, 0x12, 0x74, 0x8B, 0xFF, 0x3B, 0x3D, 0x5B, 0x03, 0x15
	};

	unsigned int i;
	int resu;

#ifdef _MAXIM_HSM

	long unsigned int l_iSignatureLength = 128;
	u8 hash[UCL_SHA256_HASHSIZE];
#endif

	/* verify the KAT */

	resu = ucl_ecdsa_verify_p256r1_sha256 (32, xg, yg, xq3, yq3, r3, s3, a, n, p, msg3, sizeof (msg3));

	if (4 <= verbose)
	{
		if (resu == UCL_OK)
			printf ("KAT ECDSA-P256r1-SHA256 SIGNATURE VERIFICATION TEST-1 OK\n");
		else
			printf ("KAT ECDSA-P256r1-SHA256 SIGNATURE VERIFICATION TEST-1 NOK %d \n", resu);
	}

	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		xq3[i] = key.ecdsa_pubkey_x[i];

	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		yq3[i] = key.ecdsa_pubkey_y[i];

#ifdef _MAXIM_HSM

	resu = ucl_sha256 (hash, input, input_size);
	resu = HSM_SignECDSA (session, hash, UCL_SHA256_HASHSIZE, signature, &l_iSignatureLength, key.HSM_Objkey);

	if (resu != UCL_OK)
	{
		printf ("ERROR on ECDSA sha256 sign (%d)\n", resu);
		HSM_pError (resu);
		return (EXIT_FAILURE);
	}

	for (i = 0; i < 64; i++)
	{
		if (i < 32)
		{
			r3[i] = signature[i];
		}
		else
		{
			s3[i - 32] = signature[i];
		}
	}

#else
	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		d3[i] = key.ecdsa_privkey[i];

	resu = ucl_ecdsa_sign_p256r1_sha256 (32, xg, yg, xq3, yq3, r3, s3, a, n, p, d3, input, input_size);
#endif /* _MAXIM_HSM */

	if (resu != UCL_OK)
	{
		printf ("ECDSA-P256r1-SHA256 SIGNATURE COMPUTATION TEST-1 NOK %d \n", resu);
		exit (0);
	}

#ifdef _MAXIM_HSM
	resu = HSM_VerifyECDSA (session, hash, UCL_SHA256_HASHSIZE, signature, l_iSignatureLength, key.HSM_Objpubkey);
#else

	resu = ucl_ecdsa_verify_p256r1_sha256 (32, xg, yg, xq3, yq3, r3, s3, a, n, p, input, input_size);

#endif /* _MAXIM_HSM */

	if (resu != UCL_OK)
	{
		printf ("ECDSA-P256r1-SHA256 SIGNATURE VERIFICATION TEST-1 NOK %d \n", resu);
		exit (0);
	}
	else if (4 <= verbose)
		printf ("ECDSA-P256r1-SHA256 SIGNATURE VERIFICATION TEST-1 OK %d \n", resu);

	/* adding computed signature to the payload */
	if (4 <= verbose)
	{
		printf ("payload(%d):", input_size);

		for (i = 0; i < input_size; i++)
			printf ("%02x", input[i]);
		printf ("\n");
		printf ("r3:");

		for (i = 0; i < 32; i++)
			printf ("%02x", r3[i]);
		printf ("\n");
		printf ("s3:");

		for (i = 0; i < 32; i++)
			printf ("%02x", s3[i]);
		printf ("\n");
	}

	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		signature[i] = r3[i];

	for (i = 0; i < ECDSA_MODULUS_LEN; i++)
		signature[ECDSA_MODULUS_LEN + i] = s3[i];

	return (EXIT_SUCCESS);
}

#ifdef _MAXIM_HSM

int load_HSM_ecdsa_key (ecdsa_key_t * key, char *keyname)
{
	CK_RV rv = EXIT_SUCCESS;

	if (4 <= verbose)
	{
		printf ("<load_HSM_ecdsa_key <%s>>\n", keyname);
	}

	if (4 <= verbose)
	{
		printf ("HSM_FindKey \n");
	}

	rv = HSM_FindKey (session, keyname, TRUE, &(key->HSM_Objkey));
	HSM_pError (rv);
	if (rv != 0)
	{
		printf ("ERROR : HSM_FindKey \n");
		return rv;
	}
	rv = HSM_FindKey (session, keyname, FALSE, &(key->HSM_Objpubkey));
	HSM_pError (rv);
	HSM_pError (rv);
	if (rv != 0)
	{
		printf ("ERROR : HSM_FindKey \n");
		return rv;
	}

	return rv;
}
#endif /* _MAXIM_HSM */

int read_file_ecdsa (u8 * puk_x, u8 * puk_y, u8 * privk, unsigned int size, char *filename)
{
	FILE *l_pFile = NULL;
	int l_iErr = 0;
	unsigned int l_iSize = 0;
	unsigned int l_iIndex = 0;

	if (filename == NULL)
	{
		printf ("ERROR read_file_ecdsa - invalid file name. \n");
		return EXIT_FAILURE;
	}

	if (4 <= verbose)
		printf ("<read_file_ecdsa <%s>>\n", filename);

	l_pFile = fopen (filename, "r");

	if (l_pFile == NULL)
	{
		printf ("ERROR on opening <%s>\n", filename);
		return EXIT_FAILURE;
	}

	/* ECDSA - PRIVK -- */
	memset (privk, 0, size);
	l_iSize = size;
	l_iErr = read_file_ascii_data (l_pFile, size, privk, &l_iSize);

	if (l_iErr != EXIT_SUCCESS)
	{
		printf ("ERROR: read privk in ecdsa key file error\n");
		return (EXIT_FAILURE);
	}

	/* ECDSA - PUK_X -- */
	memset (puk_x, 0, size);
	l_iSize = size;
	l_iErr = read_file_ascii_data (l_pFile, size, puk_x, &l_iSize);

	if (l_iErr != EXIT_SUCCESS)
	{
		printf ("ERROR: read puk_x in ecdsa key file error\n");
		return (EXIT_FAILURE);
	}

	/* ECDSA - PUK_Y -- */
	memset (puk_y, 0, size);
	l_iSize = size;
	l_iErr = read_file_ascii_data (l_pFile, size, puk_y, &l_iSize);
	if (l_iErr != EXIT_SUCCESS)
	{
		printf ("ERROR: read puk_y in ecdsa key file error\n");
		return (EXIT_FAILURE);
	}

	if (4 <= verbose)
	{
		for (l_iIndex = 0; l_iIndex < size; l_iIndex++)
			printf ("%02x", puk_x[l_iIndex]);
		printf ("\n");

		for (l_iIndex = 0; l_iIndex < size; l_iIndex++)
			printf ("%02x", puk_y[l_iIndex]);
		printf ("\n");

		for (l_iIndex = 0; l_iIndex < size; l_iIndex++)
			printf ("%02x", privk[l_iIndex]);
		printf ("\n");
	}

	fclose (l_pFile);

	return EXIT_SUCCESS;
}

/*read signed ecdsa public key file
  contains: ecdsa public key (x&y)and signature
  note: public key and signature have same length
*/

int read_file_signed_ecdsa_publickey (u8 * x, u8 * y, u8 * r, u8 * s, int size, char *filename)
{
	FILE *fp;
	int i;
	int resu;
	char line[MAXLINE];
	u8 d8 = 0x00;

	if (4 <= verbose)
		printf ("<read_file_signed_ecdsa_puk <%s>>\n", filename);
	fp = fopen (filename, "r");
	if (NULL == fp)
	{
		printf ("ERROR on opening <%s>\n", filename);
		return (EXIT_FAILURE);
	}
	memset (x, 0, size);
	for (i = 0; i < size; i++)
	{

		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", size, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}

		x[i] = d8;

	}
	for (i = 0; i < size; i++)
		y[i] = 0;
	for (i = 0; i < size; i++)
	{
		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", size, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}

		y[i] = d8;

	}
	for (i = 0; i < size; i++)
		r[i] = 0;
	for (i = 0; i < size; i++)
	{

		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", size, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}

		r[i] = d8;

	}
	for (i = 0; i < size; i++)
		s[i] = 0;
	for (i = 0; i < size; i++)
	{

		resu = fscanf (fp, "%02x", (unsigned int *) &d8);

		if (resu != 1)
		{
			printf ("ERROR: unexpected size (%d-%d)\n", size, (int) strlen (line) - 1);
			return (EXIT_FAILURE);
		}

		s[i] = d8;

	}
	if (4 <= verbose)
	{
		printf ("x:");
		for (i = 0; i < size; i++)
			printf ("%02x", x[i]);
		printf ("\n");
		printf ("y:");
		for (i = 0; i < size; i++)
			printf ("%02x", y[i]);
		printf ("\n");
		printf ("r:");
		for (i = 0; i < size; i++)
			printf ("%02x", r[i]);
		printf ("\n");
		printf ("s:");
		for (i = 0; i < size; i++)
			printf ("%02x", s[i]);
		printf ("\n");
	}
	(void) fclose (fp);
	return (EXIT_SUCCESS);
}
