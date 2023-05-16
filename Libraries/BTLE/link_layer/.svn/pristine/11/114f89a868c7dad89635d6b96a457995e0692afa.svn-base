/*************************************************************************************************/
/*!
 *  \brief  LL initialization for extended controller configuration.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
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

#include "ll_init_api.h"
#include "lhci_api.h"
#include "chci_api.h"
#include "hci_drv_sdma.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

#ifdef __CROSSWORKS
extern SDMADiag_t _sdma_diag;
#endif /* __CROSSWORKS */

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*! \brief      Extended VS command decoder. */
extern bool_t lhciVsExtDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf);

#ifdef __CROSSWORKS
extern void LhciExtScanMasterInitSh(void);
extern void LhciExtConnMasterInitSh(void);
extern void LhciExtScanMasterInitSh(void);
extern void LhciExtAdvSlaveInitSh(void);
extern void LhciChannelSelection2InitSh(void);
extern void LhciPhyInitSh(void);
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL HCI extended features.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LlInitLhciExtInitSh(void)
#else /* __CROSSWORKS */
void LlInitLhciExtInit(void)
#endif /* __CROSSWORKS */
{
  LhciVsExtInit(lhciVsExtDecodeCmdPkt);

  #ifdef INIT_CENTRAL
    #ifdef __CROSSWORKS
      LhciExtScanMasterInitSh();
    #else /* __CROSSWORKS */
      LhciExtScanMasterInit();
    #endif /* __CROSSWORKS */
    #ifdef __CROSSWORKS
      LhciExtConnMasterInitSh();
    #else /* __CROSSWORKS */
      LhciExtConnMasterInit();
    #endif /* __CROSSWORKS */
  #else
    #ifdef INIT_OBSERVER
      #ifdef __CROSSWORKS
        LhciExtScanMasterInitSh();
      #else /* __CROSSWORKS */
        LhciExtScanMasterInit();
      #endif /* __CROSSWORKS */
    #endif
  #endif

  #ifdef INIT_BROADCASTER
    #ifdef __CROSSWORKS
      LhciExtAdvSlaveInitSh();
    #else /* __CROSSWORKS */
      LhciExtAdvSlaveInit();
    #endif /* __CROSSWORKS */
  #endif

  #if defined(INIT_PERIPHERAL) || defined(INIT_CENTRAL)
    #ifdef __CROSSWORKS
      LhciChannelSelection2InitSh();
    #else /* __CROSSWORKS */
      LhciChannelSelection2Init();
    #endif /* __CROSSWORKS */
  #ifdef INIT_PHY
  #ifdef __CROSSWORKS
    LhciPhyInitSh();
  #else /* __CROSSWORKS */
    LhciPhyInit();
  #endif /* __CROSSWORKS */
  #endif
  #endif
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void LlInitLhciExtInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitLhciExtInitSh << 1);
  LlInitLhciExtInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize extended controller configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
uint32_t LlInitControllerExtInitSh(LlInitRtCfg_t *pCfg)
#else /* __CROSSWORKS */
uint32_t LlInitControllerExtInit(LlInitRtCfg_t *pCfg)
#endif /* __CROSSWORKS */
{
  uint32_t totalMemUsed;

#ifdef __CROSSWORKS
  totalMemUsed = LlInitExtInitSh(pCfg);
#else /* __CROSSWORKS */
  totalMemUsed = LlInitExtInit(pCfg);
#endif /* __CROSSWORKS */

#ifdef __CROSSWORKS
  LlInitChciTrInitSh();
#else /* __CROSSWORKS */
  LlInitChciTrInit();
#endif /* __CROSSWORKS */
#ifdef __CROSSWORKS
  LlInitLhciInitSh();
#else /* __CROSSWORKS */
  LlInitLhciInit();
#endif /* __CROSSWORKS */
#ifdef __CROSSWORKS
  LlInitLhciExtInitSh();
#else /* __CROSSWORKS */
  LlInitLhciExtInit();
#endif /* __CROSSWORKS */

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
uint32_t LlInitControllerExtInit(LlInitRtCfg_t *pCfg)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitControllerExtInitSh << 1);
  return LlInitControllerExtInitSh(pCfg);
}
#endif /* __CROSSWORKS */
