/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Data transmitter sample application.
 *
 *  Copyright (c) 2012-2018 Arm Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include "mxc_device.h"
#include "gcr_regs.h"
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_buf.h"
#include "wsf_assert.h"
#include "hci_api.h"
#include "hci_vs.h"
#include "sec_api.h"
#include "dm_api.h"
#include "smp_api.h"
#include "att_api.h"
#include "app_api.h"
#include "app_main.h"
#include "app_db.h"
#include "app_ui.h"
#include "svc_ch.h"
#include "svc_core.h"
#include "svc_wp.h"
#include "util/calc128.h"
#include "dats/dats_api.h"
#include "board.h"
#include "led.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define CONN_TIMER_IND   0x97

/*! Enumeration of client characteristic configuration descriptors */
enum
{
  DATS_GATT_SC_CCC_IDX,           /*! GATT service, service changed characteristic */
  DATS_WP_DAT_CCC_IDX,            /*! ARM Ltd. proprietary service, data transfer characteristic */
  DATS_NUM_CCC_IDX
};

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for advertising */
/* These intervals directly impact energy usage during the non-connected/advertising mode */

static const appAdvCfg_t cubeAdvCfg =
{
  { 1000,     0,     0},                  /*! Advertising durations in ms */
  {   96,   200,     0}                   /*! Advertising intervals in 0.625 ms units */
};

/*! configurable parameters for slave */
static const appSlaveCfg_t cubeSlaveCfg =
{
  1,                                      /*! Maximum connections */
};

/*! configurable parameters for security */
static const appSecCfg_t cubeSecCfg =
{
  DM_AUTH_BOND_FLAG,                                         /*! Authentication and bonding flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK | DM_KEY_DIST_CSRK,      /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK | DM_KEY_DIST_CSRK,      /*! Responder key distribution flags */
  FALSE,                                                     /*! TRUE if Out-of-band pairing data is present */
  FALSE                                                       /*! TRUE to initiate security upon connection */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t cubeSendOobData = FALSE;

/*! SMP security parameter configuration */
static const smpCfg_t cubeSmpCfg =
{
  3000,                                   /*! 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  3,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  0,                                      /*! Device authentication requirements */
};

/*! configurable parameters for connection parameter update */
static const appUpdateCfg_t cubeUpdateCfg =
{
  0,                                      /*! Connection idle period in ms before attempting
                                              connection parameter update; set to zero to disable */
  640,                                    /*! Minimum connection interval in 1.25ms units */
  800,                                    /*! Maximum connection interval in 1.25ms units */
  3,                                      /*! Connection latency */
  100,                                    /*! Supervision timeout in 10ms units */
  5                                       /*! Number of update attempts before giving up */
};

/*! ATT configurable parameters (increase MTU) */
static const attCfg_t cubeAttCfg =
{
  15,                               /* ATT server service discovery connection idle timeout in seconds */
  241,                              /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/*! local IRK */
static uint8_t localIrk[] =
{
  0x95, 0xC8, 0xEE, 0x6F, 0xC5, 0x0D, 0xEF, 0x93, 0x35, 0x4E, 0x7C, 0x57, 0x08, 0xE2, 0xA3, 0x85
};

// static uint8_t peerIrk[] =
// {
//   0xA6, 0xD9, 0xFF, 0x70, 0xD6, 0x1E, 0xF0, 0xA4, 0x46, 0x5F, 0x8D, 0x68, 0x19, 0xF3, 0xB4, 0x96
// };

/**************************************************************************************************
  Advertising Data
**************************************************************************************************/

/*! advertising data, discoverable mode */
static const uint8_t cubeAdvDataDisc[] =
{
  /*! flags */
  2,                                      /*! length */
  DM_ADV_TYPE_FLAGS,                      /*! AD type */
  DM_FLAG_LE_GENERAL_DISC |               /*! flags */
  DM_FLAG_LE_BREDR_NOT_SUP,
  5,
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'C',
  'u',
  'b',
  'e'
};

/**************************************************************************************************
  Client Characteristic Configuration Descriptors
**************************************************************************************************/

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t cubeCccSet[DATS_NUM_CCC_IDX] =
{
  /* cccd handle          value range               security level */
  {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* DATS_GATT_SC_CCC_IDX */
  {WP_DAT_CH_CCC_HDL,     ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE}    /* DATS_WP_DAT_CCC_IDX */
};

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! application control block */
static struct
{
  wsfHandlerId_t    handlerId;        /* WSF handler ID */
  dmConnId_t        peerConnId;
  wsfTimer_t        connTimer;
} cubeCb;

/* LESC OOB configuration */
static dmSecLescOobCfg_t *cubeOobCfg;

bool_t resetFlag;

/*************************************************************************************************/
/*!
 *  \brief  Send notification containing data.
 *
 *  \param  connId      DM connection ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeSendData(dmConnId_t connId)
{
  uint8_t str[] = "Unlock";

  if (AttsCccEnabled(connId, DATS_WP_DAT_CCC_IDX))
  {
    /* send notification */
    AttsHandleValueNtf(connId, WP_DAT_HDL, sizeof(str), str);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t   *pMsg;
  uint16_t  len;

  if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND)
  {
    DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

    // If the local device sends OOB data.
    if (cubeSendOobData)
    {
      uint8_t oobLocalRandom[SMP_RAND_LEN];
      SecRand(oobLocalRandom, SMP_RAND_LEN);
      DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
    }
  }
  else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND)
  {
    if (cubeOobCfg == NULL)
    {
      cubeOobCfg = WsfBufAlloc(sizeof(dmSecLescOobCfg_t));
    }

    if (cubeOobCfg)
    {
      Calc128Cpy(cubeOobCfg->localConfirm, pDmEvt->oobCalcInd.confirm);
      Calc128Cpy(cubeOobCfg->localRandom, pDmEvt->oobCalcInd.random);
    }
  }
  else
  {
    len = DmSizeOfEvt(pDmEvt);

    if ((pMsg = WsfMsgAlloc(len)) != NULL)
    {
      memcpy(pMsg, pDmEvt, len);
      WsfMsgSend(cubeCb.handlerId, pMsg);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application ATT callback.
 *
 *  \param  pEvt    ATT callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeAttCback(attEvt_t *pEvt)
{

}

/*************************************************************************************************/
/*!
 *  \brief  Application ATTS client characteristic configuration callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeCccCback(attsCccEvt_t *pEvt)
{
  appDbHdl_t    dbHdl;

  /* if CCC not set from initialization and there's a device record */
  if ((pEvt->handle != ATT_HANDLE_NONE) &&
      ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE))
  {
    /* store value in device database */
    AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  ATTS write callback for proprietary data service.
 *
 *  \return ATT status.
 */
/*************************************************************************************************/
uint8_t cubeWpWriteCback(dmConnId_t connId, uint16_t handle, uint8_t operation,
                          uint16_t offset, uint16_t len, uint8_t *pValue, attsAttr_t *pAttr)
{
  /* print received data */
  printf("%s\n", pValue);

  return ATT_SUCCESS;
}

/*************************************************************************************************/
/*!
*
*  \brief  Get peer key from a device database record.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return Pointer to peer key if key is valid or NULL if not valid.
*/
/*************************************************************************************************/
static dmSecKey_t *cubeGetPeerKey(dmEvt_t *pMsg)
{
  appConnCb_t *pCb;

  /* look up app connection control block from DM connection ID */
  pCb = &appConnCb[pMsg->hdr.param - 1];

  /* if database record handle valid */
  if (pCb->dbHdl != APP_DB_HDL_NONE)
  {
    return AppDbGetKey(pCb->dbHdl, DM_KEY_IRK, NULL);
  }

  return NULL;
}

/*************************************************************************************************/
/*!
*
*  \brief  Handle add device to resolving list indication.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return None.
*/
/*************************************************************************************************/
static void cubePrivAddDevToResListInd(dmEvt_t *pMsg)
{
  dmSecKey_t *pPeerKey;

  /* if peer IRK present */
  if ((pPeerKey = cubeGetPeerKey(pMsg)) != NULL)
  {
    /* set advertising peer address */
    AppSetAdvPeerAddr(pPeerKey->irk.addrType, pPeerKey->irk.bdAddr);
  }
}

/*************************************************************************************************/
/*!
*
*  \brief  Handle remove device from resolving list indication.
*
*  \param  pMsg    Pointer to DM callback event message.
*
*  \return None.
*/
/*************************************************************************************************/
static void cubePrivRemDevFromResListInd(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    appConnCb_t *pCb;

    /* look up app connection control block from DM connection ID */
    pCb = &appConnCb[pMsg->hdr.param - 1];

    if (pCb->dbHdl != APP_DB_HDL_NONE)
    {
      uint8_t addrZeros[BDA_ADDR_LEN] = { 0 };

      /* clear advertising peer address and its type */
      AppSetAdvPeerAddr(HCI_ADDR_TYPE_PUBLIC, addrZeros);
    }
  }
}

/*************************************************************************************************/
/*!
 *
 *  \brief  Display stack version.
 *
 *  \param  version    version number.
 *
 *  \return None.
 */
/*************************************************************************************************/
void cubeDisplayStackVersion(const char *pVersion)
{
  APP_TRACE_INFO1("Stack Version: %s", pVersion);
}

/*************************************************************************************************/
/*!
 *  \brief  Set up advertising and other procedures that need to be performed after
 *          device reset.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeSetup(dmEvt_t *pMsg)
{

  cubeCb.connTimer.handlerId = cubeCb.handlerId;
  cubeCb.connTimer.msg.event = CONN_TIMER_IND;
  LED_On(LED_BLUE);
  WsfTimerStartMs(&cubeCb.connTimer, 500);

  /* Scan responses and connections limited to the EvKit dats peer */
  // DmDevWhiteListAdd(DM_ADDR_PUBLIC, (bdAddr_t){0x02, 0x01, 0x44, 0x8B, 0x05, 0x00});
  // DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_ADV, HCI_FILT_WHITE_LIST);

  /* set advertising and scan response data for discoverable mode */
  AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(cubeAdvDataDisc), (uint8_t *) cubeAdvDataDisc);

  /* set advertising for connectable mode */
  AppAdvSetData(APP_ADV_DATA_CONNECTABLE, sizeof(cubeAdvDataDisc), (uint8_t *) cubeAdvDataDisc);

  /* Set default PHY to 1M */
  DmSetDefaultPhy(0, HCI_PHY_LE_1M_BIT, HCI_PHY_LE_1M_BIT);

  AppAdvStart(APP_MODE_AUTO_INIT);
}

/*************************************************************************************************/
/*!
 *  \brief  Process messages from the event handler.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case DM_RESET_CMPL_IND:
      DmSecGenerateEccKeyReq();
      cubeSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_ADV_START_IND:
      uiEvent = APP_UI_ADV_START;
      break;

    case DM_ADV_STOP_IND:
      uiEvent = APP_UI_ADV_STOP;
      break;

    case DM_CONN_OPEN_IND:
      uiEvent = APP_UI_CONN_OPEN;
      cubeCb.peerConnId = (dmConnId_t)pMsg->hdr.param;
      LED_Off(LED_BLUE);
      WsfTimerStop(&cubeCb.connTimer);
      break;

    case DM_CONN_CLOSE_IND:
      printf("Connection closed status 0x%x, reason 0x%x", pMsg->connClose.status, pMsg->connClose.reason);
      switch (pMsg->connClose.reason)
      {
        case HCI_ERR_CONN_TIMEOUT:      printf(" TIMEOUT\n");         break;
        case HCI_ERR_LOCAL_TERMINATED:  printf(" LOCAL TERM\n");      break;
        case HCI_ERR_REMOTE_TERMINATED: printf(" REMOTE TERM\n");     break;
        case HCI_ERR_CONN_FAIL:         printf(" FAIL ESTABLISH\n");  break;
        case HCI_ERR_MIC_FAILURE:       printf(" MIC FAILURE\n");     break;
      }
      uiEvent = APP_UI_CONN_CLOSE;
      cubeCb.peerConnId = DM_CONN_ID_NONE;
      LED_On(LED_BLUE);
      WsfTimerStartMs(&cubeCb.connTimer, 500);
      break;

    case DM_SEC_PAIR_CMPL_IND:
      printf("Pairing completed successfully.\n");
      uiEvent = APP_UI_SEC_PAIR_CMPL;
      break;

    case DM_SEC_PAIR_FAIL_IND:
      uiEvent = APP_UI_SEC_PAIR_FAIL;
      break;

    case DM_SEC_ENCRYPT_IND:
      uiEvent = APP_UI_SEC_ENCRYPT;
      break;

    case DM_SEC_ENCRYPT_FAIL_IND:
      uiEvent = APP_UI_SEC_ENCRYPT_FAIL;
      break;

    case DM_SEC_AUTH_REQ_IND:

      if (pMsg->authReq.oob)
      {
        dmConnId_t connId = (dmConnId_t) pMsg->hdr.param;

        /* TODO: Perform OOB Exchange with the peer. */


        /* TODO: Fill cubeOobCfg peerConfirm and peerRandom with value passed out of band */

        if (cubeOobCfg != NULL)
        {
          DmSecSetOob(connId, cubeOobCfg);
        }

        DmSecAuthRsp(connId, 0, NULL);
      }
      else
      {
        AppHandlePasskey(&pMsg->authReq);
      }
      break;

    case DM_SEC_COMPARE_IND:
      AppHandleNumericComparison(&pMsg->cnfInd);
      break;

    case DM_PRIV_ADD_DEV_TO_RES_LIST_IND:
      cubePrivAddDevToResListInd(pMsg);
      break;

    case DM_PRIV_REM_DEV_FROM_RES_LIST_IND:
      cubePrivRemDevFromResListInd(pMsg);
      break;

    case DM_ADV_NEW_ADDR_IND:
      break;

    case DM_VENDOR_SPEC_IND:
      break;

    case CONN_TIMER_IND:
      LED_Toggle(LED_BLUE);
      WsfTimerStartMs(&cubeCb.connTimer, 500);
      break;

    default:
      break;
  }

  if (uiEvent != APP_UI_NONE)
  {
    AppUiAction(uiEvent);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application handler init function called during system initialization.
 *
 *  \param  handlerID  WSF handler ID.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CubeHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("CubeHandlerInit");

  /* store handler ID */
  cubeCb.handlerId = handlerId;
  cubeCb.peerConnId = DM_CONN_ID_NONE;

  /* Set configuration pointers */
  pAppSlaveCfg = (appSlaveCfg_t *) &cubeSlaveCfg;
  pAppAdvCfg = (appAdvCfg_t *) &cubeAdvCfg;
  pAppSecCfg = (appSecCfg_t *) &cubeSecCfg;
  pAppUpdateCfg = (appUpdateCfg_t *) &cubeUpdateCfg;
  pSmpCfg = (smpCfg_t *) &cubeSmpCfg;
  pAttCfg = (attCfg_t *) &cubeAttCfg;

  /* Initialize application framework */
  AppSlaveInit();

  /* Set IRK for the local device */
  DmSecSetLocalIrk(localIrk);

  resetFlag = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief  Callback for WSF buffer diagnostic messages.
 *
 *  \param  pInfo     Diagnostics message
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeWsfBufDiagnostics(WsfBufDiag_t *pInfo)
{
  if (pInfo->type == WSF_BUF_ALLOC_FAILED)
  {
    APP_TRACE_INFO2("Cube got WSF Buffer Allocation Failure - Task: %d Len: %d",
                     pInfo->param.alloc.taskId, pInfo->param.alloc.len);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Button press callback.
 *
 *  \param  btn    Button press.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void cubeBtnCback(uint8_t btn)
{
  dmConnId_t  connIdList[DM_CONN_MAX];
  uint8_t     numConnections = AppConnOpenList(connIdList);

  if(cubeCb.peerConnId == DM_CONN_ID_NONE)
  {
    return;
  }

  /* button actions when connected */
  if (numConnections > 0)
  {
    switch (btn)
    {
      case APP_UI_BTN_1_MED:
        cubeSendData(cubeCb.peerConnId);
        break;

      case APP_UI_BTN_1_SHORT:
        cubeSendData(cubeCb.peerConnId);
        break;

      case APP_UI_BTN_1_LONG:
        cubeSendData(cubeCb.peerConnId);
        break;

      case APP_UI_BTN_1_EX_LONG:
        cubeSendData(cubeCb.peerConnId);
        break;

      default:
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  WSF event handler for application.
 *
 *  \param  event   WSF event mask.
 *  \param  pMsg    WSF message.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CubeHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    APP_TRACE_INFO1("Cube got evt 0x%x", pMsg->event);

    if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END)
    {
      /* process advertising and connection-related messages */
      AppSlaveProcDmMsg((dmEvt_t *) pMsg);

      /* process security-related messages */
      AppSlaveSecProcDmMsg((dmEvt_t *) pMsg);
    }

    /* perform profile and user interface-related operations */
    cubeProcMsg((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void CubeStart(void)
{
  /* Register for stack callbacks */
  DmRegister(cubeDmCback);
  DmConnRegister(DM_CLIENT_ID_APP, cubeDmCback);
  AttRegister(cubeAttCback);
  AttConnRegister(AppServerConnCback);
  AttsCccRegister(DATS_NUM_CCC_IDX, (attsCccSet_t *) cubeCccSet, cubeCccCback);

  /* Register for app framework button callbacks */
  AppUiBtnRegister(cubeBtnCback);

  /* Initialize attribute server database */
  SvcCoreAddGroup();
  SvcWpCbackRegister(NULL, cubeWpWriteCback);
  SvcWpAddGroup();

  WsfBufDiagRegister(cubeWsfBufDiagnostics);

  /* Reset the device */
  DmDevReset();
}
