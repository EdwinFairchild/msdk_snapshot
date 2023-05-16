/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Proprietary data transfer client sample application for Nordic-ble.
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
#include <ctype.h>
#include "mxc_device.h"
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "hci_api.h"
#include "dm_api.h"
#include "gap/gap_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "app_cfg.h"
#include "app_api.h"
#include "app_db.h"
#include "app_ui.h"
#include "svc_core.h"
#include "svc_ch.h"
#include "hci_vs.h"
#include "gatt/gatt_api.h"
#include "wpc/wpc_api.h"
#include "datc/datc_api.h"
#include "util/calc128.h"
#include "gcr_regs.h"
#include "ll_defs.h"
#include "tmr_utils.h"
#include "board.h"
#include "led.h"
#include "gpio.h"
#include "mxc_delay.h"

/**************************************************************************************************
Macros
**************************************************************************************************/

#define TEST_TIMER_IND   0x99 
#define LOCK_TIMER_IND   0x98 
#define CONN_TIMER_IND   0x97

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*! application control block */
struct
{
  uint16_t          hdlList[DM_CONN_MAX][APP_DB_HDL_LIST_LEN];   /*! Cached handle list */
  wsfHandlerId_t    handlerId;                      /*! WSF hander ID */
  bool_t            scanning;                       /*! TRUE if scanning */
  bool_t            autoConnectSync;                /*! TRUE if auto-connecting */
  uint8_t           discState[DM_CONN_MAX];         /*! Service discovery state */
  uint8_t           hdlListLen;                     /*! Cached handle list length */
  uint8_t           btnConnId;                      /*! The index of the connection ID for button presses */
} doorCb;

/*! connection control block */
typedef struct {
  appDbHdl_t          dbHdl;                        /*! Device database record handle type */
  uint8_t             addrType;                     /*! Type of address of device to connect to */
  bdAddr_t            addr;                         /*! Address of device to connect to */
  bool_t              doConnect;                    /*! TRUE to issue connect on scan complete */
} doorConnInfo_t;

doorConnInfo_t doorConnInfo;

/*! test control block */
struct
{
  unsigned int counter;
  unsigned int connectCount;
  dmConnId_t connId;
  wsfTimer_t timer;
  wsfTimer_t lockTimer;
  wsfTimer_t connTimer;
} testCb;

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/


/*! configurable parameters for master */
static const appMasterCfg_t doorMasterCfg =
{
  420,                                     /*! The scan interval, in 0.625 ms units */
  420,                                     /*! The scan window, in 0.625 ms units  */
  0,                                       /*! The scan duration in ms */
  DM_DISC_MODE_NONE,                       /*! The GAP discovery mode */
  DM_SCAN_TYPE_PASSIVE
                                           /*!< The scan type (active or passive) */
};

/*! configurable parameters for security */
static const appSecCfg_t doorSecCfg =
{
  DM_AUTH_BOND_FLAG,                                         /*! Authentication and bonding flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK | DM_KEY_DIST_CSRK,      /*! Initiator key distribution flags */
  DM_KEY_DIST_LTK | DM_KEY_DIST_IRK | DM_KEY_DIST_CSRK,      /*! Responder key distribution flags */
  FALSE,                                                     /*! TRUE if Out-of-band pairing data is present */
  FALSE                                                      /*! TRUE to initiate security upon connection */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t doorSendOobData = FALSE;

/*! SMP security parameter configuration */
static const smpCfg_t doorSmpCfg =
{
  3000,                                   /*! 'Repeated attempts' timeout in msec */
  SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
  7,                                      /*! Minimum encryption key length */
  16,                                     /*! Maximum encryption key length */
  3,                                      /*! Attempts to trigger 'repeated attempts' timeout */
  0,                                      /*! Device authentication requirements */
};

/*! Connection parameters */
static const hciConnSpec_t doorConnCfg =
{
  6,                                      /*! Minimum connection interval in 1.25ms units */
  6,                                      /*! Maximum connection interval in 1.25ms units */
  0,                                      /*! Connection latency */
  100,                                    /*! Supervision timeout in 10ms units */
  0,                                      /*! Minimum CE length, in 0.625 ms units */
  0                                       /*! Maximum CE length, in 0.625 ms units */
};

/*! Configurable parameters for service and characteristic discovery */
static const appDiscCfg_t doorDiscCfg =
{
  FALSE                                   /*! TRUE to wait for a secure connection before initiating discovery */
};

static const appCfg_t doorAppCfg =
{
  FALSE,                                  /*! TRUE to abort service discovery if service not found */
  TRUE                                    /*! TRUE to disconnect if ATT transaction times out */
};

/*! ATT configurable parameters (increase MTU) */
static const attCfg_t doorAttCfg =
{
  15,                               /* ATT server service discovery connection idle timeout in seconds */
  23,                               /* desired ATT MTU */
  ATT_MAX_TRANS_TIMEOUT,            /* transcation timeout in seconds */
  4                                 /* number of queued prepare writes supported by server */
};

/*! local IRK */
static uint8_t localIrk[] =
{
  0xA6, 0xD9, 0xFF, 0x70, 0xD6, 0x1E, 0xF0, 0xA4, 0x46, 0x5F, 0x8D, 0x68, 0x19, 0xF3, 0xB4, 0x96
};

// static uint8_t peerIrk[] =
// {
//   0x95, 0xC8, 0xEE, 0x6F, 0xC5, 0x0D, 0xEF, 0x93, 0x35, 0x4E, 0x7C, 0x57, 0x08, 0xE2, 0xA3, 0x85
// };

/**************************************************************************************************
  ATT Client Discovery Data
**************************************************************************************************/

/*! Discovery states:  enumeration of services to be discovered */
enum
{
  DATC_DISC_GATT_SVC,      /*! GATT service */
  DATC_DISC_GAP_SVC,       /*! GAP service */
  DATC_DISC_WP_SVC,        /*! ARM Ltd. proprietary service */
  DATC_DISC_SVC_MAX        /*! Discovery complete */
};

/*! the Client handle list, doorCb.hdlList[], is set as follows:
 *
 *  ------------------------------- <- DATC_DISC_GATT_START
 *  | GATT svc changed handle     |
 *  -------------------------------
 *  | GATT svc changed ccc handle |
 *  ------------------------------- <- DATC_DISC_GAP_START
 *  | GAP central addr res handle |
 *  -------------------------------
 *  | GAP RPA Only handle         |
 *  ------------------------------- <- DATC_DISC_WP_START
 *  | WP handles                  |
 *  | ...                         |
 *  -------------------------------
 */

/*! Start of each service's handles in the the handle list */
#define DATC_DISC_GATT_START       0
#define DATC_DISC_GAP_START        (DATC_DISC_GATT_START + GATT_HDL_LIST_LEN)
#define DATC_DISC_WP_START         (DATC_DISC_GAP_START + GAP_HDL_LIST_LEN)
#define DATC_DISC_HDL_LIST_LEN     (DATC_DISC_WP_START + WPC_P1_HDL_LIST_LEN)

/*! Pointers into handle list for each service's handles */
static uint16_t *pDoorGattHdlList[DM_CONN_MAX];
static uint16_t *pDoorGapHdlList[DM_CONN_MAX];
static uint16_t *pDoorWpHdlList[DM_CONN_MAX];

/* LESC OOB configuration */
static dmSecLescOobCfg_t *doorOobCfg;

/**************************************************************************************************
  ATT Client Configuration Data
**************************************************************************************************/

/*
 * Data for configuration after service discovery
 */

/* Default value for CCC indications */
const uint8_t doorCccIndVal[2] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_INDICATE)};

/* Default value for CCC notifications */
const uint8_t doorCccNtfVal[2] = {UINT16_TO_BYTES(ATT_CLIENT_CFG_NOTIFY)};

/* List of characteristics to configure after service discovery */
static const attcDiscCfg_t doorDiscCfgList[] =
{
  /* Write:  GATT service changed ccc descriptor */
  {doorCccIndVal, sizeof(doorCccIndVal), (GATT_SC_CCC_HDL_IDX + DATC_DISC_GATT_START)},

  /* Write:  Proprietary data service changed ccc descriptor */
  {doorCccNtfVal, sizeof(doorCccNtfVal), (WPC_P1_NA_CCC_HDL_IDX + DATC_DISC_WP_START)},
};

/* Characteristic configuration list length */
#define DATC_DISC_CFG_LIST_LEN   (sizeof(doorDiscCfgList) / sizeof(attcDiscCfg_t))

/* sanity check:  make sure configuration list length is <= handle list length */
WSF_CT_ASSERT(DATC_DISC_CFG_LIST_LEN <= DATC_DISC_HDL_LIST_LEN);

gpio_cfg_t lock_out;
gpio_cfg_t unlock_out;

void LockDoor(void)
{
  LED_On(LED_RED);
  LED_Off(LED_GREEN);

  GPIO_OutSet(&lock_out);
  mxc_delay(4000);
  GPIO_OutClr(&lock_out);
}

void UnlockDoor(void)
{

  LED_Off(LED_RED);
  LED_On(LED_GREEN);

  GPIO_OutSet(&unlock_out);
  mxc_delay(4000);
  GPIO_OutClr(&unlock_out);
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
static void doorDmCback(dmEvt_t *pDmEvt)
{
  dmEvt_t   *pMsg;
  uint16_t  len;
  uint16_t  reportLen;

  if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND)
  {
    DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

    // If the local device sends OOB data.
    if (doorSendOobData)
    {
      uint8_t oobLocalRandom[SMP_RAND_LEN];
      SecRand(oobLocalRandom, SMP_RAND_LEN);
      DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
    }
  }
  else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND)
  {
    if (doorOobCfg == NULL)
    {
      doorOobCfg = WsfBufAlloc(sizeof(dmSecLescOobCfg_t));
    }

    if (doorOobCfg)
    {
      Calc128Cpy(doorOobCfg->localConfirm, pDmEvt->oobCalcInd.confirm);
      Calc128Cpy(doorOobCfg->localRandom, pDmEvt->oobCalcInd.random);
    }
  }
  else
  {
    len = DmSizeOfEvt(pDmEvt);

    if (pDmEvt->hdr.event == DM_SCAN_REPORT_IND)
    {
      reportLen = pDmEvt->scanReport.len;
    }
    else
    {
      reportLen = 0;
    }

    if ((pMsg = WsfMsgAlloc(len + reportLen)) != NULL)
    {
      memcpy(pMsg, pDmEvt, len);
      if (pDmEvt->hdr.event == DM_SCAN_REPORT_IND)
      {
        pMsg->scanReport.pData = (uint8_t *) ((uint8_t *) pMsg + len);
        memcpy(pMsg->scanReport.pData, pDmEvt->scanReport.pData, reportLen);
      }
      WsfMsgSend(doorCb.handlerId, pMsg);
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Application  ATT callback.
 *
 *  \param  pEvt    ATT callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorAttCback(attEvt_t *pEvt)
{
  attEvt_t *pMsg;

  if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL)
  {
    memcpy(pMsg, pEvt, sizeof(attEvt_t));
    pMsg->pValue = (uint8_t *) (pMsg + 1);
    memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
    WsfMsgSend(doorCb.handlerId, pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform actions on scan start.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorScanStart(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    doorCb.scanning = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform actions on scan stop.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorScanStop(dmEvt_t *pMsg)
{
  if (pMsg->hdr.status == HCI_SUCCESS)
  {
    doorCb.scanning = FALSE;
    doorCb.autoConnectSync = FALSE;

    /* Open connection */
    if (doorConnInfo.doConnect)
    {
      testCb.connId = AppConnOpen(doorConnInfo.addrType, doorConnInfo.addr, doorConnInfo.dbHdl);
      doorConnInfo.doConnect = FALSE;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Send example data.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorSendData(dmConnId_t connId)
{
  uint8_t str[] = "hello world";

  if (pDoorWpHdlList[WPC_P1_DAT_HDL_IDX] != ATT_HANDLE_NONE)
  {
    AttcWriteCmd(connId, pDoorWpHdlList[connId-1][WPC_P1_DAT_HDL_IDX], sizeof(str), str);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Handle a scan report.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorScanReport(dmEvt_t *pMsg)
{
  uint8_t *pData;

  appDbHdl_t dbHdl;
  bool_t  connect = FALSE;

  uint8_t *pReportName;
  unsigned int lenPrintName;
  unsigned int lenReportName;
  unsigned int iPrintName;
  unsigned int iReportName;
  char strPrintName[32];

  /* disregard if not scanning or autoconnecting or syncing */
  if (!doorCb.scanning || !doorCb.autoConnectSync)
  {
    return;
  }

  printf("doorScanReport() %x : %02x:%02x:%02x:%02x:%02x:%02x\n", pMsg->scanReport.addrType,
                                                                  pMsg->scanReport.addr[5],
                                                                  pMsg->scanReport.addr[4],
                                                                  pMsg->scanReport.addr[3],
                                                                  pMsg->scanReport.addr[2],
                                                                  pMsg->scanReport.addr[1],
                                                                  pMsg->scanReport.addr[0]);

  printf("  len %u, rssi %d, evtType %x, addrType %x", pMsg->scanReport.len, pMsg->scanReport.rssi, pMsg->scanReport.eventType, pMsg->scanReport.addrType);

  /* search for the friendly name. */
  pData = NULL;
  if ((pMsg->scanReport.pData != NULL) && (pMsg->scanReport.len > 0))
  {
    pData = DmFindAdType(DM_ADV_TYPE_LOCAL_NAME,
        pMsg->scanReport.len, pMsg->scanReport.pData);

    if (pData == NULL)
    {
      pData = DmFindAdType(DM_ADV_TYPE_SHORT_NAME,
          pMsg->scanReport.len, pMsg->scanReport.pData);
    }
  }

  /* if the device has a friendly name, print friendly name in report. */
  if (pData != NULL && pData[1] == DM_ADV_TYPE_LOCAL_NAME)
  {
    /* Where is and how long is the name as stored in the report. */
    pReportName = &pData[2];
    lenReportName = pData[0];

    // Don't count type byte.
    if (lenReportName > 0) lenReportName --;

    // How long is the printed copy of the friendly name?
    lenPrintName = sizeof(strPrintName) - 1;

    // Initialize name string.
    strPrintName[lenPrintName] = (char)0;

    // Copy friendly name.
    iPrintName = 0;
    for (iReportName = 0; iReportName < lenReportName; iReportName ++)
    {
      if (isprint(pReportName[iReportName]))
      {
        strPrintName[iPrintName ++] = pReportName[iReportName];
      }
      else
      {
        snprintf(&strPrintName[iPrintName], (lenPrintName - iPrintName), "\\x%02X", pReportName[iReportName]);
        iPrintName += 4; // strlen("\\x0");
      }
    }
    if (iPrintName < lenPrintName)
      strPrintName[iPrintName ++] = (uint8_t)0;
    printf(" | \"%s\"\n", strPrintName);
  }
  /* otherwise, just print a hex dump of the advertisement payload. */
  else {
    printf(", data");
    int i;
    for (i = 0; i < pMsg->scanReport.len; i++) {
      printf(" %02x", pMsg->scanReport.pData[i]);
    }
    printf("\n");
  }

  /* if we already have a bond with this device then connect to it */
  if ((dbHdl = AppDbFindByAddr(pMsg->scanReport.addrType, pMsg->scanReport.addr)) != APP_DB_HDL_NONE)
  {
    /* if this is a directed advertisement where the initiator address is an RPA */
    if (DM_RAND_ADDR_RPA(pMsg->scanReport.directAddr, pMsg->scanReport.directAddrType))
    {
      /* resolve direct address to see if it's addressed to us */
      AppMasterResolveAddr(pMsg, dbHdl, APP_RESOLVE_DIRECT_RPA);
    }
    else
    {
      connect = TRUE;
    }
  }
  /* if the peer device uses an RPA */
  else if (DM_RAND_ADDR_RPA(pMsg->scanReport.addr, pMsg->scanReport.addrType))
  {
    /* resolve advertiser's RPA to see if we already have a bond with this device */
    AppMasterResolveAddr(pMsg, APP_DB_HDL_NONE, APP_RESOLVE_ADV_RPA);
  }
  /* find vendor-specific advertising data */
  else
  {
    pData = NULL;
    if ((pMsg->scanReport.pData != NULL) && (pMsg->scanReport.len > 0))
    {
      pData = DmFindAdType(DM_ADV_TYPE_LOCAL_NAME,
          pMsg->scanReport.len, pMsg->scanReport.pData);
    }
    if (pData != NULL)
    {
      /* check length and name */
      if (pData[DM_AD_LEN_IDX] >= 4)
      {
        if(((char)pData[DM_AD_DATA_IDX] == (char)'C') &&
        ((char)pData[DM_AD_DATA_IDX+1] == (char)'u') &&
        ((char)pData[DM_AD_DATA_IDX+2] == (char)'b') &&
        ((char)pData[DM_AD_DATA_IDX+3] == (char)'e')) {
          connect = TRUE;
        }
      }
    }
  }

  if (connect)
  {
    /* stop scanning and connect */
    doorCb.autoConnectSync = FALSE;
    AppScanStop();

    /* Store peer information for connect on scan stop */
    doorConnInfo.addrType = DmHostAddrType(pMsg->scanReport.addrType);
    memcpy(doorConnInfo.addr, pMsg->scanReport.addr, sizeof(bdAddr_t));
    doorConnInfo.dbHdl = dbHdl;
    doorConnInfo.doConnect = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform UI actions on connection open.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorOpen(dmEvt_t *pMsg)
{
  testCb.connectCount = testCb.counter;
}

/*************************************************************************************************/
/*!
 *  \brief  Perform UI actions on connection close.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorClose(dmEvt_t *pMsg)
{
  testCb.connId = DM_CONN_ID_NONE;

  if (!doorCb.scanning)
  {
    doorCb.autoConnectSync = TRUE;
    AppScanStart(doorMasterCfg.discMode, doorMasterCfg.scanType, doorMasterCfg.scanDuration);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Process a received ATT notification.
 *
 *  \param  pMsg    Pointer to ATT callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorValueNtf(attEvt_t *pMsg)
{
  /* print the received data */
  printf("%s\n", pMsg->pValue);

  UnlockDoor();

  WsfTimerStop(&testCb.lockTimer);
  WsfTimerStartMs(&testCb.lockTimer, 5000);

}

void LockPinsInit(void)
{
  lock_out.port = PORT_0;
  lock_out.mask = PIN_12;
  lock_out.pad = GPIO_PAD_NONE;
  lock_out.func = GPIO_FUNC_OUT;

  MXC_GPIO0->vssel |= (0x1 << 12);
  GPIO_Config(&lock_out);
  GPIO_OutClr(&lock_out);

  unlock_out.port = PORT_0;
  unlock_out.mask = PIN_1;
  unlock_out.pad = GPIO_PAD_NONE;
  unlock_out.func = GPIO_FUNC_OUT;

  MXC_GPIO0->vssel |= (0x1 << 1);
  GPIO_Config(&unlock_out);
  GPIO_OutClr(&unlock_out);
}

/*************************************************************************************************/
/*!
 *  \brief  Set up procedures that need to be performed after device reset.
 *
 *  \param  pMsg    Pointer to DM callback event message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorSetup(dmEvt_t *pMsg)
{
  doorCb.scanning = FALSE;
  doorCb.autoConnectSync = FALSE;
  doorConnInfo.doConnect = FALSE;

  testCb.timer.handlerId = doorCb.handlerId;
  testCb.timer.msg.event = TEST_TIMER_IND;
  WsfTimerStartMs(&testCb.timer, 1000);
  testCb.connectCount = 0x80000000;

  testCb.lockTimer.handlerId = doorCb.handlerId;
  testCb.lockTimer.msg.event = LOCK_TIMER_IND;

  testCb.connTimer.handlerId = doorCb.handlerId;
  testCb.connTimer.msg.event = CONN_TIMER_IND;

  LED_On(LED_BLUE);
  WsfTimerStartMs(&testCb.connTimer, 500);

  LockPinsInit();

  /* Scanning will be limited to the selected peer bdaddr. */
  /* TODO: Use resolving list */
  // DmDevWhiteListAdd(DM_ADDR_PUBLIC, (bdAddr_t){0x02, 0x00, 0x44, 0x8B, 0x05, 0x00});
  // DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_SCAN, HCI_FILT_WHITE_LIST);
  // DmDevSetFilterPolicy(DM_FILT_POLICY_MODE_INIT, HCI_FILT_WHITE_LIST);

  /* Set default PHY to 1M */
  DmSetDefaultPhy(0, HCI_PHY_LE_1M_BIT, HCI_PHY_LE_1M_BIT);

  DmConnSetConnSpec((hciConnSpec_t *) &doorConnCfg);
}

/*************************************************************************************************/
/*!
 *  \brief  GAP service discovery has completed.
 *
 *  \param  connId    Connection identifier.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorDiscGapCmpl(dmConnId_t connId)
{
  appDbHdl_t dbHdl;

  /* if RPA Only attribute found on peer device */
  if ((pDoorGapHdlList[connId-1][GAP_RPAO_HDL_IDX] != ATT_HANDLE_NONE) &&
      ((dbHdl = AppDbGetHdl(connId)) != APP_DB_HDL_NONE))
  {
    /* update DB */
    AppDbSetPeerRpao(dbHdl, TRUE);
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
static void doorBtnCback(uint8_t btn)
{
  dmConnId_t  connIdList[DM_CONN_MAX];
  uint8_t     numConnections = AppConnOpenList(connIdList);

  if(testCb.connId == DM_CONN_ID_NONE)
  {
    return;
  }

  /* button actions when connected */
  if (numConnections > 0)
  {
    switch (btn)
    {
      case APP_UI_BTN_1_MED:
        doorSendData(testCb.connId);
        break;

      case APP_UI_BTN_1_SHORT:
        doorSendData(testCb.connId);
        break;

      case APP_UI_BTN_1_LONG:
        doorSendData(testCb.connId);
        break;

      case APP_UI_BTN_1_EX_LONG:
        doorSendData(testCb.connId);
        break;

      default:
        break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Discovery callback.
 *
 *  \param  connId    Connection identifier.
 *  \param  status    Service or configuration status.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorDiscCback(dmConnId_t connId, uint8_t status)
{
  switch(status)
  {
    case APP_DISC_INIT:
      /* set handle list when initialization requested */
      AppDiscSetHdlList(connId, doorCb.hdlListLen, doorCb.hdlList[connId-1]);
      break;

    case APP_DISC_SEC_REQUIRED:
      /* initiate security */
      AppMasterSecurityReq(connId);
      break;

    case APP_DISC_START:
      /* initialize discovery state */
      doorCb.discState[connId-1] = DATC_DISC_GATT_SVC;

      /* discover GATT service */
      GattDiscover(connId, pDoorGattHdlList[connId-1]);
      break;

    case APP_DISC_FAILED:
      if (pAppCfg->abortDisc)
      {
        /* if discovery failed for proprietary data service then disconnect */
        if (doorCb.discState[connId-1] == DATC_DISC_WP_SVC)
        {
          AppConnClose(connId);
          break;
        }
      }
      /* else fall through and continue discovery */

    case APP_DISC_CMPL:
      /* next discovery state */
      doorCb.discState[connId-1]++;

      if (doorCb.discState[connId-1] == DATC_DISC_GAP_SVC)
      {
        /* discover GAP service */
        GapDiscover(connId, pDoorGapHdlList[connId-1]);
      }
      else if (doorCb.discState[connId-1] == DATC_DISC_WP_SVC)
      {
        /* discover proprietary data service */
        WpcP1Discover(connId, pDoorWpHdlList[connId-1]);
      }
      else
      {
        /* discovery complete */
        AppDiscComplete(connId, APP_DISC_CMPL);

        /* GAP service discovery completed */
        doorDiscGapCmpl(connId);

        /* start configuration */
        AppDiscConfigure(connId, APP_DISC_CFG_START, DATC_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *) doorDiscCfgList, DATC_DISC_HDL_LIST_LEN, doorCb.hdlList[connId-1]);

        /* Update the data length */
        DmConnSetDataLen(connId, LL_MAX_DATA_LEN_ABS_MAX, LL_MAX_DATA_TIME_ABS_MAX_1M);
      }
      break;

    case APP_DISC_CFG_START:
        /* start configuration */
        AppDiscConfigure(connId, APP_DISC_CFG_START, DATC_DISC_CFG_LIST_LEN,
                         (attcDiscCfg_t *) doorDiscCfgList, DATC_DISC_HDL_LIST_LEN, doorCb.hdlList[connId-1]);
      break;

    case APP_DISC_CFG_CMPL:
      AppDiscComplete(connId, status);
      break;

    case APP_DISC_CFG_CONN_START:
      /* no connection setup configuration */
      break;

    default:
      break;
  }
}

/*************************************************************************************************/
static void testTimerHandler(void)
{
  testCb.counter++;

  if (testCb.counter == 2)
  {
    doorCb.autoConnectSync = TRUE;
    AppScanStart(doorMasterCfg.discMode, doorMasterCfg.scanType, doorMasterCfg.scanDuration);
  }

  WsfTimerStartMs(&testCb.timer, 1000);
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
static void doorProcMsg(dmEvt_t *pMsg)
{
  uint8_t uiEvent = APP_UI_NONE;

  switch(pMsg->hdr.event)
  {
    case ATTC_HANDLE_VALUE_NTF:
      doorValueNtf((attEvt_t *) pMsg);
      break;

    case ATTC_WRITE_CMD_RSP:
    {
      break;
    }

    case ATTC_WRITE_RSP:
        break;

    case DM_RESET_CMPL_IND:
      DmSecGenerateEccKeyReq();
      doorSetup(pMsg);
      uiEvent = APP_UI_RESET_CMPL;
      break;

    case DM_SCAN_START_IND:
      doorScanStart(pMsg);
      uiEvent = APP_UI_SCAN_START;
      break;

    case DM_SCAN_STOP_IND:
      doorScanStop(pMsg);
      uiEvent = APP_UI_SCAN_STOP;
      break;

    case DM_SCAN_REPORT_IND:
      doorScanReport(pMsg);
      break;

    case DM_CONN_OPEN_IND:
      doorOpen(pMsg);
      uiEvent = APP_UI_CONN_OPEN;
      LED_Off(LED_BLUE);
      WsfTimerStop(&testCb.connTimer);
      break;

    case DM_CONN_READ_RSSI_IND:
      printf("Connection 0x%x RSSI: %d\n", pMsg->readRssi.handle, pMsg->readRssi.rssi);
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
      doorClose(pMsg);
      uiEvent = APP_UI_CONN_CLOSE;

      LED_On(LED_BLUE);
      WsfTimerStartMs(&testCb.connTimer, 500);
      LockDoor();
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


        /* TODO: Fill datsOobCfg peerConfirm and peerRandom with value passed out of band */

        if (doorOobCfg != NULL)
        {
          DmSecSetOob(connId, doorOobCfg);
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

    case DM_ADV_NEW_ADDR_IND:
      break;

    case TEST_TIMER_IND:
      testTimerHandler();
      break;

    case LOCK_TIMER_IND:
      LockDoor();
      break;

    case CONN_TIMER_IND:
      LED_Toggle(LED_BLUE);
      WsfTimerStartMs(&testCb.connTimer, 500);
      break;

    case DM_VENDOR_SPEC_IND:
      break;

    case DM_PHY_UPDATE_IND:
      APP_TRACE_INFO2("DM_PHY_UPDATE_IND - RX: %d, TX: %d", pMsg->phyUpdate.rxPhy, pMsg->phyUpdate.txPhy);
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
void DoorHandlerInit(wsfHandlerId_t handlerId)
{
  APP_TRACE_INFO0("DoorHandlerInit");

  /* store handler ID */
  doorCb.handlerId = handlerId;

  /* set handle list length */
  doorCb.hdlListLen = DATC_DISC_HDL_LIST_LEN;

  doorCb.btnConnId = 1;
  /* Set configuration pointers */
  pAppMasterCfg = (appMasterCfg_t *) &doorMasterCfg;
  pAppSecCfg = (appSecCfg_t *) &doorSecCfg;
  pAppDiscCfg = (appDiscCfg_t *) &doorDiscCfg;
  pAppCfg = (appCfg_t *)&doorAppCfg;
  pSmpCfg = (smpCfg_t *) &doorSmpCfg;
  pAttCfg = (attCfg_t *) &doorAttCfg;

  /* Initialize application framework */
  AppMasterInit();
  AppDiscInit();

  /* Set IRK for the local device */
  DmSecSetLocalIrk(localIrk);
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
void DoorHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
  if (pMsg != NULL)
  {
    if ((pMsg->event != TEST_TIMER_IND) && (pMsg->event != ATTC_WRITE_RSP))
    {
      APP_TRACE_INFO2("Door got evt 0x%x, param %u", pMsg->event, pMsg->param);
    }

    /* process ATT messages */
    if (pMsg->event <= ATT_CBACK_END)
    {
      /* process discovery-related ATT messages */
      AppDiscProcAttMsg((attEvt_t *) pMsg);
    }
    /* process DM messages */
    else if (pMsg->event <= DM_CBACK_END)
    {
      /* process advertising and connection-related messages */
      AppMasterProcDmMsg((dmEvt_t *) pMsg);

      /* process security-related messages */
      AppMasterSecProcDmMsg((dmEvt_t *) pMsg);

      /* process discovery-related messages */
      AppDiscProcDmMsg((dmEvt_t *) pMsg);
    }

    /* perform profile and user interface-related operations */
    doorProcMsg((dmEvt_t *) pMsg);
  }
}

/*************************************************************************************************/
/*!
 *  \brief  Initialize the pointers into the handle list.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void doorInitSvcHdlList(void)
{
  uint8_t i;

  for (i=0; i<DM_CONN_MAX; i++)
  {
    /*! Pointers into handle list for each service's handles */
    pDoorGattHdlList[i] = &doorCb.hdlList[i][DATC_DISC_GATT_START];
    pDoorGapHdlList[i] = &doorCb.hdlList[i][DATC_DISC_GAP_START];
    pDoorWpHdlList[i] = &doorCb.hdlList[i][DATC_DISC_WP_START];
  }
}
/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void DoorStart(void)
{
  LockDoor();

  /* Initialize handle pointers */
  doorInitSvcHdlList();

  /* Register for stack callbacks */
  DmRegister(doorDmCback);
  DmConnRegister(DM_CLIENT_ID_APP, doorDmCback);
  AttRegister(doorAttCback);

  /* Register for app framework button callbacks */
  AppUiBtnRegister(doorBtnCback);

  /* Register for app framework discovery callbacks */
  AppDiscRegister(doorDiscCback);

  /* Initialize attribute server database */
  SvcCoreAddGroup();

  /* Reset the device */
  DmDevReset();
}
