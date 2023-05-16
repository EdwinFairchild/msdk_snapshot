#include <stdio.h>
#include <string.h>
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_buf.h"
#include "wsf_assert.h"
#include "dm_api.h"
#include "att_api.h"
#include "svc_wp.h"

#include "process_msg.h"

/** UCL garbage */
#include "ucl/types.h"
#include "ucl/ecdsa_generic_api.h"
#include "chademo_crypto.h"

/*! Enumeration of client characteristic configuration descriptors -- must match dats_main.c */
enum
{
  DATS_GATT_SC_CCC_IDX,           /*! GATT service, service changed characteristic */
  DATS_WP_DAT_CCC_IDX,            /*! ARM Ltd. proprietary service, data transfer characteristic */
  DATS_NUM_CCC_IDX
};

static uint8_t rbuf[256];
static unsigned int started = 0;

void send_resp(dmConnId_t connId, uint16_t len, uint8_t *data)
{
  if (AttsCccEnabled(connId, DATS_WP_DAT_CCC_IDX)) {
      /* send notification */
      AttsHandleValueNtf(connId, WP_DAT_HDL, len, data);
    } 
}

uint8_t process_msg(dmConnId_t connId, uint16_t handle, uint8_t operation,
		    uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
  uint16_t mlen;
  uint8_t *mptr;
  uint16_t rlen = 3;
  unsigned int outlen;
  
  APP_TRACE_INFO3("Message from client (0x%02x/0x%04x/0x%02x):", connId, handle, operation);
  APP_TRACE_INFO2("[Offset 0x%04x, Length 0x%04x]", offset, len);
  //APP_TRACE_INFO0("Got: ");
  //hexdump(pValue, len);

  /* Error message by default */
  rbuf[0] = RESP_ERROR;
  rbuf[1] = 0;
  rbuf[2] = 0;
  
  if (len >= 3) {
    mlen = (pValue[1] << 8) + pValue[2];
    mptr = &pValue[3];
    APP_TRACE_INFO2("Command 0x%02x, Length %u", pValue[0], mlen);
    switch (pValue[0]) {
      case CMD_DONGLE_ID:
	/* Identification */
	rbuf[0] = RESP_DONGLE_ID;
	rlen += sprintf((char *)rbuf+3, "Chupacabra v1.0");
	rbuf[1] = (rlen - 3) >> 8;
	rbuf[2] = (rlen - 3) & 0xff;
	break;
      case CMD_RESET_STATE:
	started = 0;
	rbuf[0] = RESP_OK;
	rbuf[1] = 0;
	rbuf[2] = 0;
	break;
      case CMD_ENCRYPT_BLK:
	if (mlen < 129 && ((len - 3) == mlen)) {
	  /* Appropriate length of incoming data */
	  if (!started) {
	    if (!crypto_encrypt_start(mptr, mlen, rbuf+3, &outlen)) {
	      rbuf[0] = RESP_ENCRYPTED;
	      rlen += outlen;
	      rbuf[1] = (rlen - 3) >> 8;
	      rbuf[2] = (rlen - 3) & 0xff;
	      started++;
	    }
	  } else {
	    if (mlen > 0) {
	      if (!crypto_encrypt_continue(mptr, mlen, rbuf+3, &outlen)) {
		rbuf[0] = RESP_ENCRYPTED;
		rlen += outlen;
		rbuf[1] = (rlen - 3) >> 8;
		rbuf[2] = (rlen - 3) & 0xff;
	      }
	    } else {
	      if (!crypto_encrypt_finish(mptr, mlen, rbuf+3, &outlen)) {
		rbuf[0] = RESP_ENCRYPTED;
		rlen += outlen;
		rbuf[1] = (rlen - 3) >> 8;
		rbuf[2] = (rlen - 3) & 0xff;
	      }
	    }
	  }
	}
	break;
      case CMD_RETR_SIG:
	if (started) {
	  /* Retrieve the signature */
	  if (!crypto_sign(rbuf+3)) {
	    rbuf[0] = RESP_SIGNATURE;
	    rlen += 64; /* magic #!!! */
	    rbuf[1] = (rlen - 3) >> 8;
	    rbuf[2] = (rlen - 3) & 0xff;
	    started = 0;
	  }
	}
	break;
      case CMD_DECRYPT_BLK:
	if (mlen < 129 && ((len - 3) == mlen)) {
	  if (!started) {
	    if (!crypto_decrypt_start(mptr, mlen, rbuf+3, &outlen)) {
	      rbuf[0] = RESP_DECRYPTED;
	      rlen += outlen;
	      rbuf[1] = (rlen - 3) >> 8;
	      rbuf[2] = (rlen - 3) & 0xff;
	      started++;
	    }
	  } else {
	    if (mlen > 0) {
	      if (!crypto_decrypt_continue(mptr, mlen, rbuf+3, &outlen)) {
		rbuf[0] = RESP_DECRYPTED;
		rlen += outlen;
		rbuf[1] = (rlen - 3) >> 8;
		rbuf[2] = (rlen - 3) & 0xff;
	      }
	    } else {
	      if (!crypto_decrypt_finish(mptr, mlen, rbuf+3, &outlen)) {
		rbuf[0] = RESP_DECRYPTED;
		rlen += outlen;
		rbuf[1] = (rlen - 3) >> 8;
		rbuf[2] = (rlen - 3) & 0xff;
	      }
	    }
	  }
	}
	break;
      case CMD_VERIFY_SIG:
	if (started) {
	  if ((mlen == 64) && (len - 3) == mlen) {
	    /* Verify the signature */
	    if (!crypto_verify(mptr)) {
	      rbuf[0] = RESP_SIG_GOOD;
	    } else {
	      rbuf[0] = RESP_SIG_BAD;
	    }
	    rbuf[1] = 0;
	    rbuf[2] = 0;
	    started = 0;
	  }
	}
	break;
      default:
	/* Invalid command */
	rbuf[0] = RESP_ERROR;
	rbuf[1] = 0;
	rbuf[2] = 0;
	break;
    }
    //APP_TRACE_INFO0("Sending: ");
    //hexdump(rbuf, rlen);
    send_resp(connId, rlen, rbuf);
  }

  return ATT_SUCCESS;
}

