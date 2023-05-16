/*============================================================================
 *
 * ucl_aes_cbc.h [30-mar-06]
 *
 *==========================================================================*/
/*============================================================================
 *
 * Copyright © 2009 Innova Card. All Rights Reserved. Do not disclose.
 *
 * This software is the confidential and proprietary information of
 * Innova Card ("Confidential Information"). You shall not
 * disclose such Confidential Information and shall use it only in
 * accordance with the terms of the license agreement you entered
 * into with Innova Card.
 *
 * Innova Card makes no representations or warranties about the suitability of
 * the software, either express or implied, including but not limited to
 * the implied warranties of merchantability, fitness for a particular purpose,
 * or non-infrigement. Innova Card shall not be liable for any damages suffered
 * by licensee as the result of using, modifying or distributing this software
 * or its derivatives.
 *
 *==========================================================================*/
/*============================================================================
 *
 * Purpose :
 *
 *==========================================================================*/
#ifndef _UCL_AES_CBC_H_
#define _UCL_AES_CBC_H_

#include "ucl/ucl_aes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

/** @file ucl_aes_cbc.h
 * @defgroup UCL_CBC_AES AES CBC
 * Encrypt / Decrypt with AES in CBC (Cipher Block Chaining) mode.
 *
 * @par Header:
 * @link ucl_aes_cbc.h ucl_aes_cbc.h @endlink
 *
 * @ingroup UCL_CBC
 */

/*============================================================================*/
/** <b>AES-CBC</b>.
 * Encrypt / Decrypt with AES in CBC (Cipher Block Chaining) mode.
 *
 *
 * @param[out] dst    Pointer to the output data
 * @param[in]  src    Pointer to the input data
 * @param[in]  len    Data byte length
 * @param[in]  key    Pointer to the AES Key
 * @param[in]  keylen Key byte length:
 *                        @li #UCL_AES_KEYLEN_128
 *                        @li #UCL_AES_KEYLEN_192
 *                        @li #UCL_AES_KEYLEN_256
 * @param[in] IV      Pointer to the initialization vector
 * @param[in] mode    The mode (Encryption/Decryption):
 *                        @li #UCL_CIPHER_ENCRYPT
 *                        @li #UCL_CIPHER_DECRYPT
 *
 * @return Error code
 *
 * @retval #UCL_OK             no error occurred
 * @retval #UCL_INVALID_INPUT  one of the input is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT one of the output is the pointer NULL
 * @retval  #UCL_INVALID_ARG   @p len is not a multiple of
 *                             #UCL_AES_BLOCKSIZE or @p keylen is invalid
 *
 * @ingroup UCL_CBC_AES
 */
int ucl_aes_cbc(u8 *dst, u8 *src, u32 len, u8 *key, u32 keylen, u8 *IV,
                int mode);


/*============================================================================*/
/** <b>AES-CBC Init</b>.
 * Initialize AES CBC Context.
 *
 * @warning on platforms using a AES hardware block, this function is required to change the key
 * the use of another context in the ucl_aes_core function will not mean using the other context key
 *
 * @param[out] ctx    Pointer to the context
 * @param[in]  key    Pointer to the AES Key
 * @param[in]  keylen Key byte length:
 *                        @li #UCL_AES_KEYLEN_128
 *                        @li #UCL_AES_KEYLEN_192
 *                        @li #UCL_AES_KEYLEN_256
 * @param[in]  IV     Pointer to the initialization vector
 * @param[in]  mode   The mode (Encryption/Decryption) :
 *                        @li #UCL_CIPHER_ENCRYPT
 *                        @li #UCL_CIPHER_DECRYPT
 *
 * @return Error code
 *
 * @retval #UCL_OK             No error occurred
 * @retval #UCL_INVALID_INPUT  The input is the pointer #NULL
 * @retval #UCL_INVALID_OUTPUT The output is the pointer #NULL
 * @retval #UCL_INVALID_ARG    @p keylen is invalid
 * @retval #UCL_INVALID_MODE   The mode is not one of those described
 *
 * @ingroup UCL_CBC_AES 
 */
int ucl_aes_cbc_init(ucl_aes_ctx_t *ctx, u8 *key, u32 keylen, u8 *IV, int mode);


/*============================================================================*/
/** <b>AES-CBC Core</b>.
 * Process the Data.
 *
 * @param[out]    dst  Pointer to the processed data
 * @param[out,in] ctx  Pointer to the context
 * @param[in]     src  Pointer to the data
 * @param[in]     len  Data byte length
 *
 * @pre The byte length must be a multiple of #UCL_AES_BLOCKSIZE.
 *
 * @return Error code
 *
 * @retval #UCL_OK             No error occurred
 * @retval #UCL_INVALID_INPUT  One of the input is the pointer #NULL
 * @retval #UCL_INVALID_OUTPUT The output is the pointer #NULL
 * @retval #UCL_INVALID_ARG    The data byte length is not a multiple of
 *                             #UCL_AES_BLOCKSIZE
 *
 * @ingroup UCL_CBC_AES 
 */
int ucl_aes_cbc_core(u8 *dst, ucl_aes_ctx_t *ctx, u8 *src,
                     u32 len);


/*============================================================================*/
/**<b>AES-CBC Finish</b>.
 * Zeroize the context.
 *
 * @param[out,in] ctx Pointer to the context
 *
 * @return Error code
 *
 * @retval #UCL_OK             No error occurred
 * @retval #UCL_INVALID_OUTPUT The output is the pointer #NULL
 *
 * @ingroup UCL_CBC_AES 
 */
int ucl_aes_cbc_finish(ucl_aes_ctx_t *ctx);


/** <b>AES-CBC Core</b>.
 * Process the Data.
 *
 * @param[out]  dst Pointer to the output data
 * @param[out,in] ctx AES context
 * @param[in]  src Pointer to the input data
 * @param[in]  len Data byte length
 *
 * @pre The byte length must be a multiple of #UCL_AES_BLOCKSIZE.
 *
 * @return Error code
 *
 * @retval #UCL_OK    No error occurred
 * @retval #UCL_INVALID_INPUT One of the input is the pointer #NULL
 * @retval #UCL_INVALID_OUTPUT The output is the pointer #NULL
 * @retval #UCL_INVALID_ARG  The byte length is not a multiple of
 *          #UCL_AES_BLOCKSIZE
 *
 * @ingroup UCL_CBC_AES */
  int ucl_aes_cbc_core_context(u8 *dst, ucl_aes_ctx_t *ctx, u8 *src, u32 len);


#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /*_UCL_AES_CBC_H_*/
