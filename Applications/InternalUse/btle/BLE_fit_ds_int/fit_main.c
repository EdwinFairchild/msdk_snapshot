/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Fitness sample application for the following profiles:
 *            Heart Rate profile
 *
 *  Copyright (c) 2011-2018 Arm Ltd. All Rights Reserved.
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
// *INDENT-SPACE=2*

#include <string.h>
#include "wsf_types.h"
#include "util/bstream.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "wsf_buf.h"
#include "hci_api.h"
#include "dm_api.h"
#include "att_api.h"
#include "smp_api.h"
#include "app_api.h"
#include "app_db.h"
#include "app_ui.h"
#include "app_hw.h"
#include "svc_ch.h"
#include "svc_core.h"
#include "svc_hrs.h"
#include "svc_dis.h"
#include "svc_batt.h"
#include "svc_rscs.h"
#include "bas/bas_api.h"
#include "hrps/hrps_api.h"
#include "rscp/rscp_api.h"
#include "fit/fit_api.h"
#include "util/calc128.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! WSF message event starting value */
#define FIT_MSG_START               0xA0

/* Default Running Speed and Cadence Measurement period (seconds) */
#define FIT_DEFAULT_RSCM_PERIOD        1

/*! WSF message event enumeration */
enum {
    FIT_HR_TIMER_IND = FIT_MSG_START,       /*! Heart rate measurement timer expired */
    FIT_BATT_TIMER_IND,                     /*! Battery measurement timer expired */
    FIT_RUNNING_TIMER_IND                   /*! Running speed and cadence measurement timer expired */
};

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! Application message type */
typedef union {
    wsfMsgHdr_t     hdr;
    dmEvt_t         dm;
    attsCccEvt_t    ccc;
    attEvt_t        att;
} fitMsg_t;

/**************************************************************************************************
  Configurable Parameters
**************************************************************************************************/

/*! configurable parameters for advertising */
/* These intervals directly impact energy usage during the non-connected/advertising mode */
static const appAdvCfg_t fitAdvCfg = {
    { 0,     0,     0},                  /*! Advertising durations in ms */
    { 200,   10000,     0}                   /*! Advertising intervals in 0.625 ms units */
};

/*! configurable parameters for slave */
static const appSlaveCfg_t fitSlaveCfg = {
    1,                           /*! Maximum connections */
};

/*! configurable parameters for security */
static const appSecCfg_t fitSecCfg = {
    DM_AUTH_BOND_FLAG | DM_AUTH_SC_FLAG,    /*! Authentication and bonding flags */
    0,                                      /*! Initiator key distribution flags */
    DM_KEY_DIST_LTK,                        /*! Responder key distribution flags */
    FALSE,                                  /*! TRUE if Out-of-band pairing data is present */
    FALSE                                    /*! TRUE to initiate security upon connection */
};

/*! TRUE if Out-of-band pairing data is to be sent */
static const bool_t fitSendOobData = FALSE;

/*! configurable parameters for connection parameter update */
static const appUpdateCfg_t fitUpdateCfg = {
    6000,                                   /*! Connection idle period in ms before attempting
                                              connection parameter update; set to zero to disable */
    640,                                    /*! Minimum connection interval in 1.25ms units */
    800,                                    /*! Maximum connection interval in 1.25ms units */
    0,                                      /*! Connection latency */
    900,                                    /*! Supervision timeout in 10ms units */
    5                                       /*! Number of update attempts before giving up */
};

/*! heart rate measurement configuration */
static const hrpsCfg_t fitHrpsCfg = {
    2000      /*! Measurement timer expiration period in ms */
};

/*! battery measurement configuration */
static const basCfg_t fitBasCfg = {
    30,       /*! Battery measurement timer expiration period in seconds */
    1,        /*! Perform battery measurement after this many timer periods */
    100       /*! Send battery level notification to peer when below this level. */
};

/*! SMP security parameter configuration */
static const smpCfg_t fitSmpCfg = {
    3000,                                   /*! 'Repeated attempts' timeout in msec */
    SMP_IO_NO_IN_NO_OUT,                    /*! I/O Capability */
    7,                                      /*! Minimum encryption key length */
    16,                                     /*! Maximum encryption key length */
    3,                                      /*! Attempts to trigger 'repeated attempts' timeout */
    0,                                      /*! Device authentication requirements */
};

/**************************************************************************************************
  Advertising Data
**************************************************************************************************/

/*! advertising data, discoverable mode */
static const uint8_t fitAdvDataDisc[] = {
  /*! flags */
  2,                                      /*! length */
  DM_ADV_TYPE_FLAGS,                      /*! AD type */
  DM_FLAG_LE_GENERAL_DISC |               /*! flags */
      DM_FLAG_LE_BREDR_NOT_SUP,

  /*! tx power */
  2,                                      /*! length */
  DM_ADV_TYPE_TX_POWER,                   /*! AD type */
  0,                                      /*! tx power */

  /*! service UUID list */
  9,                                      /*! length */
  DM_ADV_TYPE_16_UUID,                    /*! AD type */
  UINT16_TO_BYTES(ATT_UUID_HEART_RATE_SERVICE),
  UINT16_TO_BYTES(ATT_UUID_RUNNING_SPEED_SERVICE),
  UINT16_TO_BYTES(ATT_UUID_DEVICE_INFO_SERVICE),
  UINT16_TO_BYTES(ATT_UUID_BATTERY_SERVICE)
};

/*! scan data, discoverable mode */
static const uint8_t fitScanDataDisc[] = {
  /*! device name */
  4,                                      /*! length */
  DM_ADV_TYPE_LOCAL_NAME,                 /*! AD type */
  'F',
  'i',
  't'
};

/**************************************************************************************************
  Client Characteristic Configuration Descriptors
**************************************************************************************************/

/*! enumeration of client characteristic configuration descriptors */
enum {
    FIT_GATT_SC_CCC_IDX,                    /*! GATT service, service changed characteristic */
    FIT_HRS_HRM_CCC_IDX,                    /*! Heart rate service, heart rate monitor characteristic */
    FIT_BATT_LVL_CCC_IDX,                   /*! Battery service, battery level characteristic */
    FIT_RSCS_SM_CCC_IDX,                   /*! Runninc speed and cadence measurement characteristic */
    FIT_NUM_CCC_IDX
};

/*! client characteristic configuration descriptors settings, indexed by above enumeration */
static const attsCccSet_t fitCccSet[FIT_NUM_CCC_IDX] = {
    /* cccd handle          value range               security level */
    {GATT_SC_CH_CCC_HDL,    ATT_CLIENT_CFG_INDICATE,  DM_SEC_LEVEL_NONE},   /* FIT_GATT_SC_CCC_IDX */
    {HRS_HRM_CH_CCC_HDL,    ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* FIT_HRS_HRM_CCC_IDX */
    {BATT_LVL_CH_CCC_HDL,   ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE},   /* FIT_BATT_LVL_CCC_IDX */
    {RSCS_RSM_CH_CCC_HDL,   ATT_CLIENT_CFG_NOTIFY,    DM_SEC_LEVEL_NONE}    /* FIT_RSCS_SM_CCC_IDX */
};

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! WSF handler ID */
wsfHandlerId_t fitHandlerId;

/* WSF Timer to send running speed and cadence measurement data */
wsfTimer_t     fitRscmTimer;

/* Running Speed and Cadence Measurement period - Can be changed at runtime to vary period */
static uint16_t fitRscmPeriod = FIT_DEFAULT_RSCM_PERIOD;

/* Heart Rate Monitor feature flags */
static uint8_t fitHrmFlags = CH_HRM_FLAGS_VALUE_8BIT | CH_HRM_FLAGS_ENERGY_EXP;

/* LESC OOB configuration */
static dmSecLescOobCfg_t *fitOobCfg;

/*************************************************************************************************/
/*!
 *  \brief  Application DM callback.
 *
 *  \param  pDmEvt  DM callback event
 *
 *  \return None.
 */
/*************************************************************************************************/
static void fitDmCback(dmEvt_t *pDmEvt)
{
    dmEvt_t *pMsg;
    uint16_t len;

    if (pDmEvt->hdr.event == DM_SEC_ECC_KEY_IND) {
        DmSecSetEccKey(&pDmEvt->eccMsg.data.key);

        // If the local device sends OOB data.
        if (fitSendOobData) {
            uint8_t oobLocalRandom[SMP_RAND_LEN];
            SecRand(oobLocalRandom, SMP_RAND_LEN);
            DmSecCalcOobReq(oobLocalRandom, pDmEvt->eccMsg.data.key.pubKey_x);
        }
    } else if (pDmEvt->hdr.event == DM_SEC_CALC_OOB_IND) {
        if (fitOobCfg == NULL) {
            fitOobCfg = WsfBufAlloc(sizeof(dmSecLescOobCfg_t));
        }

        if (fitOobCfg) {
            Calc128Cpy(fitOobCfg->localConfirm, pDmEvt->oobCalcInd.confirm);
            Calc128Cpy(fitOobCfg->localRandom, pDmEvt->oobCalcInd.random);
        }
    } else {
        len = DmSizeOfEvt(pDmEvt);

        if ((pMsg = WsfMsgAlloc(len)) != NULL) {
            memcpy(pMsg, pDmEvt, len);
            WsfMsgSend(fitHandlerId, pMsg);
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
static void fitAttCback(attEvt_t *pEvt)
{
    attEvt_t *pMsg;

    if ((pMsg = WsfMsgAlloc(sizeof(attEvt_t) + pEvt->valueLen)) != NULL) {
        memcpy(pMsg, pEvt, sizeof(attEvt_t));
        pMsg->pValue = (uint8_t *) (pMsg + 1);
        memcpy(pMsg->pValue, pEvt->pValue, pEvt->valueLen);
        WsfMsgSend(fitHandlerId, pMsg);
    }
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
static void fitCccCback(attsCccEvt_t *pEvt)
{
    attsCccEvt_t  *pMsg;
    appDbHdl_t    dbHdl;

    /* if CCC not set from initialization and there's a device record */
    if ((pEvt->handle != ATT_HANDLE_NONE) &&
            ((dbHdl = AppDbGetHdl((dmConnId_t) pEvt->hdr.param)) != APP_DB_HDL_NONE)) {
        /* store value in device database */
        AppDbSetCccTblValue(dbHdl, pEvt->idx, pEvt->value);
    }

    if ((pMsg = WsfMsgAlloc(sizeof(attsCccEvt_t))) != NULL) {
        memcpy(pMsg, pEvt, sizeof(attsCccEvt_t));
        WsfMsgSend(fitHandlerId, pMsg);
    }
}


/*************************************************************************************************/
/*!
*  \brief  Send a Running Speed and Cadence Measurement Notification.
*
*  \param  connId    connection ID
*
*  \return None.
*/
/*************************************************************************************************/
static void fitSendRunningSpeedMeasurement(dmConnId_t connId)
{
    if (AttsCccEnabled(connId, FIT_RSCS_SM_CCC_IDX)) {
        static uint8_t walk_run = 1;

        /* TODO: Set Running Speed and Cadence Measurement Parameters */

        RscpsSetParameter(RSCP_SM_PARAM_SPEED, 1);
        RscpsSetParameter(RSCP_SM_PARAM_CADENCE, 2);
        RscpsSetParameter(RSCP_SM_PARAM_STRIDE_LENGTH, 3);
        RscpsSetParameter(RSCP_SM_PARAM_TOTAL_DISTANCE, 4);

        /* Toggle running/walking */
        walk_run = walk_run? 0 : 1;
        RscpsSetParameter(RSCP_SM_PARAM_STATUS, walk_run);

        RscpsSendSpeedMeasurement(connId);
    }

    /* Configure and start timer to send the next measurement */
    fitRscmTimer.msg.event = FIT_RUNNING_TIMER_IND;
    fitRscmTimer.msg.status = FIT_RSCS_SM_CCC_IDX;
    fitRscmTimer.handlerId = fitHandlerId;
    fitRscmTimer.msg.param = connId;

    WsfTimerStartSec(&fitRscmTimer, fitRscmPeriod);
}

/*************************************************************************************************/
/*!
 *  \brief  Process CCC state change.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void fitProcCccState(fitMsg_t *pMsg)
{
    APP_TRACE_INFO3("ccc state ind value:%d handle:%d idx:%d", pMsg->ccc.value, pMsg->ccc.handle, pMsg->ccc.idx);

    /* handle heart rate measurement CCC */
    if (pMsg->ccc.idx == FIT_HRS_HRM_CCC_IDX) {
        if (pMsg->ccc.value == ATT_CLIENT_CFG_NOTIFY) {
            HrpsMeasStart((dmConnId_t) pMsg->ccc.hdr.param, FIT_HR_TIMER_IND, FIT_HRS_HRM_CCC_IDX);
        } else {
            HrpsMeasStop((dmConnId_t) pMsg->ccc.hdr.param);
        }
        return;
    }

    /* handle running speed and cadence measurement CCC */
    if (pMsg->ccc.idx == FIT_RSCS_SM_CCC_IDX) {
        if (pMsg->ccc.value == ATT_CLIENT_CFG_NOTIFY) {
            fitSendRunningSpeedMeasurement((dmConnId_t)pMsg->ccc.hdr.param);
        } else {
            WsfTimerStop(&fitRscmTimer);
        }
        return;
    }

    /* handle battery level CCC */
    if (pMsg->ccc.idx == FIT_BATT_LVL_CCC_IDX) {
        if (pMsg->ccc.value == ATT_CLIENT_CFG_NOTIFY) {
            BasMeasBattStart((dmConnId_t) pMsg->ccc.hdr.param, FIT_BATT_TIMER_IND, FIT_BATT_LVL_CCC_IDX);
        } else {
            BasMeasBattStop((dmConnId_t) pMsg->ccc.hdr.param);
        }
        return;
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Perform UI actions on connection close.
 *
 *  \param  pMsg    Pointer to message.
 *
 *  \return None.
 */
/*************************************************************************************************/
static void fitClose(fitMsg_t *pMsg)
{
    /* stop heart rate measurement */
    HrpsMeasStop((dmConnId_t) pMsg->hdr.param);

    /* stop battery measurement */
    BasMeasBattStop((dmConnId_t) pMsg->hdr.param);

    /* Stop running speed and cadence timer */
    WsfTimerStop(&fitRscmTimer);
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
static void fitSetup(fitMsg_t *pMsg)
{
    /* set the default PHY */
    DmSetDefaultPhy(0, HCI_PHY_LE_1M_BIT, HCI_PHY_LE_1M_BIT);

    /* set advertising and scan response data for discoverable mode */
    AppAdvSetData(APP_ADV_DATA_DISCOVERABLE, sizeof(fitAdvDataDisc), (uint8_t *) fitAdvDataDisc);
    AppAdvSetData(APP_SCAN_DATA_DISCOVERABLE, sizeof(fitScanDataDisc), (uint8_t *) fitScanDataDisc);

    /* set advertising and scan response data for connectable mode */
    AppAdvSetData(APP_ADV_DATA_CONNECTABLE, 0, NULL);
    AppAdvSetData(APP_SCAN_DATA_CONNECTABLE, 0, NULL);

    /* start advertising; automatically set connectable/discoverable mode and bondable mode */
    AppAdvStart(APP_MODE_AUTO_INIT);
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
static void fitBtnCback(uint8_t btn)
{
    dmConnId_t      connId;
    static uint8_t  heartRate = 78;    /* for testing/demonstration */

    /* button actions when connected */
    if ((connId = AppConnIsOpen()) != DM_CONN_ID_NONE) {
        switch (btn) {
            case APP_UI_BTN_1_SHORT:
                /* increment the heart rate */
                AppHwHrmTest(++heartRate);
                break;

            case APP_UI_BTN_1_MED:
                break;

            case APP_UI_BTN_1_LONG:
                AppConnClose(connId);
                break;

            case APP_UI_BTN_2_SHORT:
                /* decrement the heart rate */
                AppHwHrmTest(--heartRate);
                break;

            case APP_UI_BTN_2_MED:
                /* Toggle HRM Sensor DET flags */
                if (!(fitHrmFlags & (CH_HRM_FLAGS_SENSOR_DET | CH_HRM_FLAGS_SENSOR_NOT_DET))) {
                    fitHrmFlags |= CH_HRM_FLAGS_SENSOR_DET;
                } else if (fitHrmFlags & CH_HRM_FLAGS_SENSOR_DET) {
                    fitHrmFlags &= ~CH_HRM_FLAGS_SENSOR_DET;
                    fitHrmFlags |= CH_HRM_FLAGS_SENSOR_NOT_DET;
                } else {
                    fitHrmFlags &= ~CH_HRM_FLAGS_SENSOR_NOT_DET;
                }

                HrpsSetFlags(fitHrmFlags);
                break;

            case APP_UI_BTN_2_LONG:
                /* Toggle HRM RR Interval feature flag */
                if (fitHrmFlags & CH_HRM_FLAGS_RR_INTERVAL) {
                    fitHrmFlags &= ~CH_HRM_FLAGS_RR_INTERVAL;
                } else {
                    fitHrmFlags |= CH_HRM_FLAGS_RR_INTERVAL;
                }

                HrpsSetFlags(fitHrmFlags);
                break;

            default:
                break;
        }
    }
    /* button actions when not connected */
    else {
        switch (btn) {
            case APP_UI_BTN_1_SHORT:
                /* start or restart advertising */
                AppAdvStart(APP_MODE_AUTO_INIT);
                break;

            case APP_UI_BTN_1_MED:
                /* enter discoverable and bondable mode mode */
                AppSetBondable(TRUE);
                AppAdvStart(APP_MODE_DISCOVERABLE);
                break;

            case APP_UI_BTN_1_LONG:
                /* clear bonded device info and restart advertising */
                AppDbDeleteAllRecords();
                AppAdvStart(APP_MODE_AUTO_INIT);
                break;

            case APP_UI_BTN_2_SHORT:
                /* Toggle HRM Flag for 8 and 16 bit values */
                if (fitHrmFlags & CH_HRM_FLAGS_VALUE_16BIT) {
                    fitHrmFlags &= ~CH_HRM_FLAGS_VALUE_16BIT;
                } else {
                    fitHrmFlags |= CH_HRM_FLAGS_VALUE_16BIT;
                }

                HrpsSetFlags(fitHrmFlags);
                break;

            default:
                break;
        }
    }
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
static void fitProcMsg(fitMsg_t *pMsg)
{
    uint8_t uiEvent = APP_UI_NONE;

    switch(pMsg->hdr.event) {
        case FIT_RUNNING_TIMER_IND:
            fitSendRunningSpeedMeasurement((dmConnId_t)pMsg->ccc.hdr.param);
            break;

        case FIT_HR_TIMER_IND:
            HrpsProcMsg(&pMsg->hdr);
            break;

        case FIT_BATT_TIMER_IND:
            BasProcMsg(&pMsg->hdr);
            break;


        case ATTS_HANDLE_VALUE_CNF:
            HrpsProcMsg(&pMsg->hdr);
            BasProcMsg(&pMsg->hdr);
            break;

        case ATTS_CCC_STATE_IND:
            fitProcCccState(pMsg);
            break;

        case DM_RESET_CMPL_IND:
            DmSecGenerateEccKeyReq();
            fitSetup(pMsg);
            uiEvent = APP_UI_RESET_CMPL;
            break;

        case DM_ADV_START_IND:
            uiEvent = APP_UI_ADV_START;
            break;

        case DM_ADV_STOP_IND:
            uiEvent = APP_UI_ADV_STOP;
            break;

        case DM_CONN_OPEN_IND:
            HrpsProcMsg(&pMsg->hdr);
            BasProcMsg(&pMsg->hdr);
            uiEvent = APP_UI_CONN_OPEN;
            break;

        case DM_CONN_CLOSE_IND:
            fitClose(pMsg);
            uiEvent = APP_UI_CONN_CLOSE;
            break;

        case DM_SEC_PAIR_CMPL_IND:
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
            AppHandlePasskey(&pMsg->dm.authReq);
            break;

        case DM_SEC_ECC_KEY_IND:
            DmSecSetEccKey(&pMsg->dm.eccMsg.data.key);
            break;

        case DM_SEC_COMPARE_IND:
            AppHandleNumericComparison(&pMsg->dm.cnfInd);
            break;

        case DM_HW_ERROR_IND:
            uiEvent = APP_UI_HW_ERROR;
            break;

        default:
            break;
    }

    if (uiEvent != APP_UI_NONE) {
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
void FitHandlerInit(wsfHandlerId_t handlerId)
{
    APP_TRACE_INFO0("FitHandlerInit");

    /* store handler ID */
    fitHandlerId = handlerId;

    /* Set configuration pointers */
    pAppAdvCfg = (appAdvCfg_t *) &fitAdvCfg;
    pAppSlaveCfg = (appSlaveCfg_t *) &fitSlaveCfg;
    pAppSecCfg = (appSecCfg_t *) &fitSecCfg;
    pAppUpdateCfg = (appUpdateCfg_t *) &fitUpdateCfg;

    /* Initialize application framework */
    AppSlaveInit();

    /* Set stack configuration pointers */
    pSmpCfg = (smpCfg_t *) &fitSmpCfg;

    /* initialize heart rate profile sensor */
    HrpsInit(handlerId, (hrpsCfg_t *) &fitHrpsCfg);
    HrpsSetFlags(fitHrmFlags);

    /* initialize battery service server */
    BasInit(handlerId, (basCfg_t *) &fitBasCfg);
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
void FitHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg)
{
    if (pMsg != NULL) {
	APP_TRACE_INFO1("Fit got evt %d", pMsg->event);

        if (pMsg->event >= DM_CBACK_START && pMsg->event <= DM_CBACK_END) {
            /* process advertising and connection-related messages */
            AppSlaveProcDmMsg((dmEvt_t *) pMsg);

            /* process security-related messages */
            AppSlaveSecProcDmMsg((dmEvt_t *) pMsg);
        }

        /* perform profile and user interface-related operations */
        fitProcMsg((fitMsg_t *) pMsg);
    }
}

/*************************************************************************************************/
/*!
 *  \brief  Start the application.
 *
 *  \return None.
 */
/*************************************************************************************************/
void FitStart(void)
{
    /* Register for stack callbacks */
    DmRegister(fitDmCback);
    DmConnRegister(DM_CLIENT_ID_APP, fitDmCback);
    AttRegister(fitAttCback);
    AttConnRegister(AppServerConnCback);
    AttsCccRegister(FIT_NUM_CCC_IDX, (attsCccSet_t *) fitCccSet, fitCccCback);

    /* Register for app framework callbacks */
    AppUiBtnRegister(fitBtnCback);

    /* Initialize attribute server database */
    SvcCoreAddGroup();
    SvcHrsCbackRegister(NULL, HrpsWriteCback);
    SvcHrsAddGroup();
    SvcDisAddGroup();
    SvcBattCbackRegister(BasReadCback, NULL);
    SvcBattAddGroup();
    SvcRscsAddGroup();

    /* Set running speed and cadence features */
    RscpsSetFeatures(RSCS_ALL_FEATURES);

    /* Reset the device */
    DmDevReset();
}
