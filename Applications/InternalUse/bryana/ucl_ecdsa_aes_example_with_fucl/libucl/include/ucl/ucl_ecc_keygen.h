#ifndef UCL_ECCKEYGEN_H_
#define UCL_ECCKEYGEN_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */
#include <ucl/ucl_retdefs.h>
#include <ucl/ucl_types.h>
#include <ucl/ucl_config.h>
#include <ucl/ucl_defs.h>
#include <ucl/ucl_sys.h>
#include <ucl/ucl_sha1.h>
#include <ucl/ucl_sha224.h>
#include <ucl/ucl_sha256.h>
#include <ucl/ucl_sha384.h>
#include <ucl/ucl_sha512.h>
#include <ucl/ucl_rsa.h>
#include <ucl/ecdsa_generic_api.h>
/** @defgroup UCL_ECC_KEYGEN ECC keys generation.
 * ECC keys generation.
 *
 * @ingroup UCL_ECC
 */
/** <b>ECC key Generation</b>.
 * this function generates a public/private keypair on the provided curve
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 20-byte array
 * @param[out]  d: output, the secret key, generated by the function
 * @param[in] curve_params: input, the pointer to the curve domain parameters structure
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 * @retval #UCL_STACK_ERROR if the UCL stack has a problem
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_generic(ucl_type_ecc_u8_affine_point Q,u8 *d,ucl_type_curve *curve_params);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-160
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 20-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p160r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-192
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 24-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p192r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-224
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 28-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p224r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-256
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 32-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p256r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-384
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 48-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p384r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-521
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 66-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p521r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve Brainpool P-256
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 32-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_bp256r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve Brainpool P-384
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 48-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_bp384r1(ucl_type_ecc_u8_affine_point Q,u8 *d);

/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve Brainpool P-512
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 64-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_bp512r1(ucl_type_ecc_u8_affine_point Q,u8 *d);

/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-160
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_p521r1(ucl_type_ecc_u8_affine_point Q,u8 *d);
/** <b>ECC SM2 DSA key Generation</b>.
 * this function generates a public/private keypair on the chinese standard curve SM2 FP256
 * using the API introduced in the UCL 2.4.9
 *
 * @param[out]  Q: output, the public key structure: R=d.G, pointing to a 32-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygeneration_sm2fp256(ucl_type_ecc_u8_affine_point Q,u8 *d);

int ucl_ecc_point_on_curve_p160r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-192
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_p192r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-224
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_p224r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-256
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_p256r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-384
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_p384r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-521
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_p521r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve Brainpool P-256
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_bp256r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve Brainpool P-384
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_bp384r1(ucl_type_ecc_u8_affine_point Q);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve Brainpool P-512
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_bp512r1(ucl_type_ecc_u8_affine_point Q);

/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the chinese SM2 FP 256 curve
 * using the new ECC API introduced in the UCL 2.4.9
 *
 * @param[in]  Q: input, the point structure to be checked
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_point_on_curve_sm2fp256(ucl_type_ecc_u8_affine_point Q);


int ucl_ecc_keygen_generic(u8 *xr,u8 *yr,u8 *d,int curve);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-160
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 20-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_p160r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-192
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 24-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_p192r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-224
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 28-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_p224r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-256
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 32-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_p256r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-384
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 48-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_p384r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve BP-256
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 32-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_bp256r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve BP-384
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 48-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_bp384r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve BP-512
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to a 64-byte array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_bp512r1(u8 *xr,u8 *yr,u8 *d);
/** <b>ECDSA key Generation</b>.
 * this function generates a public/private keypair on the curve P-521
 *
 *
 * @param[out]  xr, yr: output, the public key: R=d.G, xr and yr pointers to an array
 * @param[out]  d: output, the secret key, generated by the function
 *
 * @return Error code
 *
 * @retval #UCL_OK              No error occurred
 * @retval #UCL_INVALID_INPUT  if one of the inputs is the pointer NULL
 * @retval #UCL_INVALID_OUTPUT  if one of the outputs is the pointer NULL
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_keygen_p521r1(u8 *xr,u8 *yr,u8 *d);

/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-160
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_p160r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-192
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_p192r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-224
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_p224r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-256
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_p256r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve BP-256
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_bp256r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-384
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_p384r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve BP-384
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_bp384r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve BP-512
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_bp512r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);
/** <b>ECDSA point verification</b>.
 * this function checks whether or not a point is on the curve P-521
 *
 *
 * @param[in]  u8 *x: input, the x affine coordinate of the point to be checked
 * @param[in]  u8 *y: input, the y affine coordinate of the point to be checked
 * @param[in] u8 *a, u8 *n, u8 *p: curve parameters
 *
 * @return Error code
 *
 * @retval #UCL_OK if the point is on the curve
 * @retval #UCL_ERROR if the point is not on the curve
 * @retval #UCL_INVALID_INPUT in case of erroneous input parameters,
 *
 * @ingroup UCL_ECC_KEYGEN */
int ucl_ecc_on_curve_p521r1(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p);

int ucl_ecc_on_curve_generic(u8 *x,u8 *y, u8 *a,u8 *b, u8 *p,int curve);
int ucl_ecc_point_on_curve_generic(ucl_type_ecc_u8_affine_point Q,ucl_type_curve *curve_params);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif
