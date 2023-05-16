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

#include <stdint.h>

#include <ucl/ucl_config.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_aes.h>

#ifdef _MAXIM_HSM
#include <libhsm/HSM.h>
#endif /* _MAXIM_HSM */

#include "scp_definitions.h"

#define MAJV		3
#define MINV		10
#define ZVER		2
#define BUILD		0

#define INIFILE		 ".\\session_build.ini"

#ifndef TRUE
#define TRUE							1
#endif

#ifndef FALSE
#define FALSE							0
#endif

#define MAX_FRAME 102400
#define MAX_STRING 						1000
#define MAX_PARAMS 						4
#define MAXLINE							35000
#define MAX_TAB							100
#define MAX_FLASH_MB 					1023
#define MAX_CHUNK_SIZE 					102400
#define SCP_LITE_MAX_CHUNK_SIZE 		40960
#define ANGELA_MAX_CHUNK_SIZE 			15354
#define PAOLA_MAX_CHUNK_SIZE 			MAX_CHUNK_SIZE
#define MAXQ1852_MAX_CHUNK_SIZE 		1856
#define FLORA_MAX_CHUNK_SIZE 			4094
/* #define OK								0
#define YES								1
#define NOT								0
*/
#define ERR_INVAL 						UCL_ERROR
#define ERR_NO							UCL_OK

/**
 *		Cryptography information
 */

#define MAX_RSA							512

/* byte length of the CRK public exponent */
#define RSA_MODULUS_LEN					256
#define PAOLA_RSA_MODULUS_LEN           512
#define RSA_PUBLIC_EXPONENT_LEN			4
#define SIGNATURE_LEN					RSA_MODULUS_LEN
#define PAOLA_SIGNATURE_LEN				PAOLA_RSA_MODULUS_LEN

/* ECDSA256 so signature is 2*32=64 bytes */
#define MAX_ECDSA						64

/* byte length of the CRK public exponent */
#define ECDSA_MODULUS_LEN				32
#define ECDSA_SIGNATURE_LEN				(2*ECDSA_MODULUS_LEN)

/* -- MAXIM Chip - information -- */

#define MAX32653 						8
#define MAX32550 						7
#define MAXQ1852 						6
#define JIBE							5
#define IC400D							4
#define IC400ABC 						1
#define IC300							2
#define IC200							3

#define SECTOR_MAX 						35
#define SECTOR_SIZE 					4096

#define USN_LEN							16
#define USN_FLORA_LEN					13
#define USN_ANGELA_LEN					13

/* -- MAXIM Chip - device life cycles -- */

#define P3		3
#define P4		4
#define P5		5
#define P6		6
#define P6A		0x6a
#define P7		7
#define P8		8

#define UNUSED_PARAMETER(x) (void)(x)	/* optimized away by compiler */

typedef struct _type_config
{
	u8 cpu;
	u8 life_cycle;
	u8 usn[USN_LEN];
	int usn_len;
	u8 mka[UCL_AES_BLOCKSIZE];
	u8 mkc[UCL_AES_BLOCKSIZE];
	u8 mks[UCL_AES_BLOCKSIZE];
	u8 tka[UCL_AES_BLOCKSIZE];
	u8 tkc[UCL_AES_BLOCKSIZE];
	u8 tks[UCL_AES_BLOCKSIZE];
	u8 pka[UCL_AES_BLOCKSIZE];
	u8 pkc[UCL_AES_BLOCKSIZE];
	u8 pks[UCL_AES_BLOCKSIZE];
	u8 fka[UCL_AES_BLOCKSIZE];
	u8 fkc[UCL_AES_BLOCKSIZE];
	u8 fks[UCL_AES_BLOCKSIZE];
	u8 sblpk[UCL_AES_BLOCKSIZE];
	u8 rsa[PAOLA_RSA_MODULUS_LEN];
	u8 rsa_privexp[PAOLA_RSA_MODULUS_LEN];
	u8 rsa_pubexp[PAOLA_RSA_MODULUS_LEN];
	unsigned int rsa_len;
	unsigned int rsa_explen;
	unsigned int rsa_privexplen;
	unsigned int ecdsa_len;
	u8 ecdsa_privkey[ECDSA_MODULUS_LEN];
	u8 ecdsa_pubkey_x[ECDSA_MODULUS_LEN];
	u8 ecdsa_pubkey_y[ECDSA_MODULUS_LEN];
	u8 already_diversified;
	u8 pp;
	/* this parameter represents the size, in MB, of the flash */
	/* targeted for the file programming (write-file) */
	int flash_mb;
	int address_offset;
	u8 random_number[UCL_AES_BLOCKSIZE];
#ifdef _MAXIM_HSM
	CK_OBJECT_HANDLE HSM_Objkey;
	CK_OBJECT_HANDLE HSM_Objpubkey;
	char HSM_KeyLabel[MAX_STRING];
	char hsm_thales_dll[MAX_STRING];
	int32_t hsm_slot_nb;
#endif				/* _MAXIM_HSM */
} type_config_struct;

type_config_struct config_struct;
u8 verbose;
char params[MAX_PARAMS][MAX_STRING];
int nb_params;

#ifdef _MAXIM_HSM
CK_SESSION_HANDLE session;
#endif /* _MAXIM_HSM */

/* SCP - SCP lite - SCP-1852 - RSA - ECDSA*/
unsigned int ipayload;
u8 payload[MAX_FRAME];
u8 frame[MAX_FRAME];

/* RSA - ECDSA */
u8 session_mode;

/* SCP - SCP lite - SCP-1852  */
uint8_t *data_g;
int data_len_g;

/* SCP - SCP-1852 - readfile */
int chunk_size;
int *addr_g;

/* SCP - readfile */
FILE *fp_g;
/* max_data_size is used to control read data are not too large vs *data allocated size */
int max_data_size;

/* SCP - SCP lite - SCP-1852*/
unsigned int iframe;
char name_file[MAX_STRING];
char message[MAX_STRING];
char idf_scp_cmd[MAX_SCP_COMMAND][MAX_STRING];
int seq;
char script_file[MAX_STRING];

/* SCP - SCP lite */
char idf_ctl[MAX_IDF][MAX_STRING];

/* SCP */
u8 aes_key[UCL_AES_BLOCKSIZE];
u8 aes_data[UCL_AES_BLOCKSIZE];
u8 keya[UCL_AES_BLOCKSIZE];

/* SPC 1852 */
int trid;

/* Process */
int found;

/* utils */
int compteur;
char output_file[MAX_STRING];

int help (void);
/* aes */
u8 keyc[UCL_AES_BLOCKSIZE];
u8 keys[UCL_AES_BLOCKSIZE];
