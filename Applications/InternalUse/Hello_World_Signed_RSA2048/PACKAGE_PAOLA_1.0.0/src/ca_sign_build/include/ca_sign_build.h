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


#include <ucl/ecdsa_generic_api.h>

#ifdef _MAXIM_HSM
#include <libhsm/HSM.h>
#endif /* _MAXIM_HSM */

#define MAJV 1 
#define MINV 8 
#define ZVER 4
#define BUILD 1 


#define INIFILE "ca_sign_build.ini"

#include <config.h>




#define FLORA_SIGNATURE_LEN RSA_BYTE_LEN
#define PAOLA_SIGNATURE_LEN PAOLA_RSA_BYTE_LEN
#define ANGELA_SIGNATURE_LEN ECDSA_MODULUS_LEN*2

#define RSA_BYTE_LEN 256
#define PAOLA_RSA_BYTE_LEN 512
#define AES_BYTE_LEN 16
#define EXP_BYTE_LEN 4
#define CV_BYTE_LEN 4
#define HEADER_SYNC_LEN 8

#define HEADER_VERSION_LEN 4
#define HEADER_APPLICATION_VERSION_LEN 4
#define HEADER_LOAD_ADDRESS_LEN 4
#define HEADER_JUMP_ADDRESS_LEN 4
#define HEADER_BINARY_LEN 4
#define HEADER_ARGV_LEN 4
#define HEADER_SR_PAPD_LEN 1
#define HEADER_SR_PRFSH_LEN 4
#define HEADER_SR_PCFG_LEN 4
#define HEADER_SR_PEXT_LEN 1
#define HEADER_DMC_GCFG_LEN 4
#define HEADER_DMC_CLK_LEN 1
#define HEADER_UCI_KSRC_CONFIGENCINT_LEN 1
#define HEADER_UCI0_AC1R_START_OFFSET_LEN 4
#define HEADER_UCI0_AC1R_END_OFFSET_LEN 4
#define HEADER_UCI0_DDR_R0_LEN 4
#define FLORA_HEADER_LEN HEADER_SYNC_LEN+HEADER_VERSION_LEN+HEADER_LOAD_ADDRESS_LEN+HEADER_JUMP_ADDRESS_LEN+HEADER_BINARY_LEN+HEADER_ARGV_LEN+/*HEADER_GENERAL_INFO_LEN*/+HEADER_SR_PAPD_LEN+HEADER_SR_PRFSH_LEN+HEADER_SR_PCFG_LEN+HEADER_SR_PEXT_LEN+HEADER_DMC_GCFG_LEN+HEADER_DMC_CLK_LEN+HEADER_UCI_KSRC_CONFIGENCINT_LEN+HEADER_UCI0_AC1R_START_OFFSET_LEN+HEADER_UCI0_AC1R_END_OFFSET_LEN+HEADER_UCI0_DDR_R0_LEN
#define ANGELA_HEADER_LEN HEADER_SYNC_LEN+HEADER_VERSION_LEN+HEADER_LOAD_ADDRESS_LEN+HEADER_JUMP_ADDRESS_LEN+HEADER_BINARY_LEN+HEADER_ARGV_LEN+HEADER_APPLICATION_VERSION_LEN
#define MAX_HEADER_LEN FLORA_HEADER_LEN


typedef enum algo_e {a_rsa, a_rsa_paola, a_sha256, a_crc32, a_none, a_ecdsa} algo_t;



typedef struct
{
  u8 rsa[MAX_STRING];
  u8 rsa_privexp[MAX_STRING];
  u8 rsa_pubexp[MAX_STRING];
  int rsa_len;
  int rsa_explen;
  int ecdsa_len;
  u8 ecdsa_privkey[ECDSA_MODULUS_LEN];
  u8 ecdsa_pubkey_x[ECDSA_MODULUS_LEN];
  u8 ecdsa_pubkey_y[ECDSA_MODULUS_LEN];
  u8 already_diversified;
  u8 pp;
#ifdef _MAXIM_HSM
	CK_OBJECT_HANDLE HSM_Objkey;
	CK_OBJECT_HANDLE HSM_Objpubkey;
	char HSM_KeyLabel[MAX_STRING];
	char hsm_thales_dll[MAX_STRING];
	int32_t hsm_slot_nb;
#endif	
} type_config_struct;

type_config_struct config_struct;

#ifdef _MAXIM_HSM
CK_SESSION_HANDLE session;
#endif /* _MAXIM_HSM */


uint8_t verbose;
int 	data_len_g;
int 	addr_g[MAX_FLASH];













