/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Stack initialization for dats.
 *
 *  Copyright (c) 2016-2017 ARM Ltd. All Rights Reserved.
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

#include <string.h>
#include "wsf_types.h"
#include "wsf_os.h"
#include "util/bstream.h"
#include "hci_handler.h"
#include "ll_init_api.h"
#include "hci_api.h"
#include "chci_drv.h"

#define LL_IMPL_REV             0x2303

#define LL_MEMORY_FOOTPRINT     0xF022

#ifdef _RTE_
#include "RTE_Components.h"             // IAR/Keil Component selection
#endif

uint8_t LlMem[LL_MEMORY_FOOTPRINT];

const LlRtCfg_t _ll_cfg = {
    /* Device */
    /*compId*/                  LL_COMP_ID_ARM,
    /*implRev*/                 LL_IMPL_REV,
    /*btVer*/                   LL_VER_BT_CORE_SPEC_5_1,
    /*_align32 */               0, // padding for alignment

    /* Advertiser */
    /*maxAdvSets*/              6,
    /*maxAdvReports*/           6,
    /*maxExtAdvDataLen*/        LL_MAX_ADV_DATA_LEN,
    /*defExtAdvDataFrag*/       64,
    /*auxDelayUsec*/            0,

    /* Scanner */
    /*maxScanReqRcvdEvt*/       4,
    /*maxExtScanDataLen*/       LL_MAX_ADV_DATA_LEN,

    /* Connection */
    /*maxConn*/                 4,
    /*numTxBufs*/               16,
    /*numRxBufs*/               16,
    /*maxAclLen*/               256,
    /*defTxPwrLvl*/             0,
    /*ceJitterUsec*/            0,

    /* DTM */
    /*dtmRxSyncMs*/             1000,

    /* PHY */
    /*phy2mSup*/                TRUE,
    /*phyCodedSup*/             TRUE,
    /*stableModIdxTxSup*/       FALSE,
    /*stableModIdxRxSup*/       FALSE
};

const BbRtCfg_t _bb_cfg = {
    /*clkPpm*/                  20,
    /*rfSetupDelayUsec*/        BB_RF_SETUP_DELAY_US,
    /*defaultTxPower*/          4,
    /*maxScanPeriodMsec*/       BB_MAX_SCAN_PERIOD_MS,
    /*schSetupDelayUsec*/       BB_SCH_SETUP_DELAY_US
};

/*************************************************************************************************/
/*!
 *  \brief      Initialize stack.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void StackInit(void)
{
  uint8_t mask[HCI_LE_EVT_MASK_LEN];

  /* Initialize link layer. */
  LlInitRtCfg_t ll_init_cfg =
  {
      .pBbRtCfg     = &_bb_cfg,
      .wlSizeCfg    = 4,
      .rlSizeCfg    = 4,
      .plSizeCfg    = 4,
      .pLlRtCfg     = &_ll_cfg,
      .pFreeMem     = LlMem,
      .freeMemAvail = LL_MEMORY_FOOTPRINT
  };

  LlInitControllerExtInit(&ll_init_cfg);

  // Enable LE Events
  memset(mask, 0xFF, HCI_LE_EVT_MASK_LEN);
  HciLeSetEventMaskCmd(mask);
  HciSetEventMaskCmd(mask);
  
}
