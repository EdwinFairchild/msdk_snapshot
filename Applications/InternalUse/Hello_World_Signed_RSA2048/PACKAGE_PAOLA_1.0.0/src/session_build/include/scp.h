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
* @author: Benjamin VINOT <benjamin.vinot@maximintegrated.com>
*
*/

#ifndef __SCP_H__
#define __SCP_H__

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *
 */
	void define_const (void);

/**
 *
 */
	void define_cmd (void);

/**
 *
 */
	void synchro (void);

/**
 *
 */
	void con_req (void);

/**
 *
 */
	void con_rep (void);
/**
 *
 */
	void discon_req (void);
/**
 *
 */
	void discon_rep (void);
/**
 *
 */
	void add_byte (u8 value);
/**
 *
 */
	void add_channel_id_seq (void);
	void header_crc (void);
	int connection_request (void);

	int connection_reply (void);

	int disconnection_request (void);
	int disconnection_reply (void);

	int echo_request_payload (void);

	int echo_req (void);
	int echo_reply (void);

	int dump (void);

	void ack_code (void);
	int ack (void);

	void data_code (void);
	int hello_request_payload (void);
	int hello_reply_payload (void);

	int hello_request (void);

	int hello_reply (void);

	int challenge_payload (void);
	int challenge (void);
	int challenge_response_payload (void);

	int challenge_response (void);

	int aes_comp_response_payload (void);

	int aes_comp_response (void);

	int write_crk_payload_rsa (u8 * rsa_modulus, int rsa_modulus_len, u8 * rsa_pubexp,
				   int rsa_explen, u8 * mrk_signature, int mrk_signature_len);

	int write_crk_payload_ecdsa (u8 * x, u8 * y, u8 * r, u8 * s, int ecdsa_len);

	int rewrite_crk_payload_ecdsa (u8 * oldx, u8 * oldy, u8 * newx, u8 * newy, u8 * newr, u8 * news, int ecdsa_len);

	int rewrite_crk_payload_rsa (u8 * oldmodulus, int oldmodulus_len, u8 * oldpubexp,
				     int oldpub_len, u8 * newmodulus, int newmodulus_len,
				     u8 * newpubexp, int newpub_len, u8 * signature, int signature_len);

	int write_crk (char *signaturefile);

	int rewrite_crk (char *oldsignaturefile, char *newsignaturefile);

	int aes_comp_payload (void);

	int aes_comp (void);

	int write_mem_payload (u8 * data, int data_len, int data_addr);

	int write_mem (u8 * data, int data_len, int data_addr);
	int write_file (char *sfilename, char *ptr_address_offset);
	int write_only (char *sfilename, char *ptr_address_offset);

	int verify_data_payload (u8 * data, int data_len, int data_addr);

	int verify_data (u8 * data, int data_len, int data_addr);

	int verify_file (char *sfilename, char *ptr_address_offset, char b_dump);

	int del_mem (char *sfilename, char *ptr_address_offset);

	int del_data (char *sstart_addr, char *slength);

	int write_bpk_blpk_payload (char *bpk_char);

	int write_bpk_blpk (char *bpk_char);
	int write_bpk_fak_payload (char *fak_char);

	int write_bpk_fak (char *fak_char);

	int write_timeout_payload (char timeout_target_char, char *timeout_char);

	int write_timeout (char timeout_target_char, char *timeout_char);

	int kill_chip_payload (void);

	int kill_chip2_payload (void);

	int kill_chip (void);

	int kill_chip2 (void);

	int write_bpk_payload (char *data_char, char *offset_char);

	int write_bpk (char *offset_char, char *data_char);

	int execute_code_payload (char *address);

	int execute_code (char *address);

	int write_otp_payload (char *data_char, char *offset_char);

	int write_otp (char *offset_char, char *data_char);
	int write_bpk_blpk_response (void);

	int write_bpk_fak_response (void);

	int write_bpk_response (void);

	int write_otp_response (void);

	int write_crk_response (void);

	int rewrite_crk_response (void);

	int write_timeout_response (void);

	int kill_chip_response (void);

	int execute_code_response (void);

	int del_mem_response (void);

	int verify_data_response (void);

	int write_mem_response (void);

	int del_mem_payload (int start_addr, int end_addr);

	int mem_mapping_payload (void);

	int mem_mapping (void);

	int read_configuration_payload (void);

	int read_configuration (void);

	int read_configuration_response_payload (void);

	int read_configuration_response (void);

	int mem_mapping_response_payload (void);

	int mem_mapping_response (void);
	int generic_response_payload (void);

	int generic_response (char *msg);

	int process_script (void);

#ifdef __cplusplus
}
#endif

#endif				/* __SCP_H__ */
