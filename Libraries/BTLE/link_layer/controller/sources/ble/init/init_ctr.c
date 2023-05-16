/*************************************************************************************************/
/*!
 *  \brief  LL initialization for controller configuration.
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
extern void LhciScanMasterInitSh(void);
extern void LhciEncMasterInitSh(void);
extern void LhciConnMasterInitSh(void);
extern void LhciScanMasterInitSh(void);
extern void LhciAdvSlaveInitSh(void);
extern void LhciEncSlaveInitSh(void);
extern void LhciConnInitSh(void);
extern void LhciAdvSlaveInitSh(void);
extern void LhciPrivAdvInitSh(void);
extern void LhciPrivConnInitSh(void);
extern void LhciScInitSh(void);
extern void LhciPhyInitSh(void);
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize controller HCI transport.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LlInitChciTrInitSh(void)
#else /* __CROSSWORKS */
void LlInitChciTrInit(void)
#endif /* __CROSSWORKS */
{
  wsfHandlerId_t handlerId = WsfOsSetNextHandler(ChciTrHandler);
#ifdef __CROSSWORKS
  ChciTrHandlerInitSh(handlerId);
#else /* __CROSSWORKS */
  ChciTrHandlerInit(handlerId);
#endif /* __CROSSWORKS */
}
#ifdef __CROSSWORKS
#pragma codeseg(default)

void LlInitChciTrInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitChciTrInitSh << 1);
  LlInitChciTrInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL HCI.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LlInitLhciInitSh(void)
#else /* __CROSSWORKS */
void LlInitLhciInit(void)
#endif /* __CROSSWORKS */
{
  wsfHandlerId_t handlerId;

  #ifdef INIT_CENTRAL
    #ifdef __CROSSWORKS
      LhciScanMasterInitSh();
    #else /* __CROSSWORKS */
      LhciScanMasterInit();
    #endif /* __CROSSWORKS */
    #ifdef INIT_ENCRYPTED
      #ifdef __CROSSWORKS
        LhciEncMasterInitSh();
      #else /* __CROSSWORKS */
        LhciEncMasterInit();
      #endif /* __CROSSWORKS */
    #endif
    #ifdef __CROSSWORKS
      LhciConnMasterInitSh();
    #else /* __CROSSWORKS */
      LhciConnMasterInit();
    #endif /* __CROSSWORKS */
  #else
    #ifdef INIT_OBSERVER
      #ifdef __CROSSWORKS
        LhciScanMasterInitSh();
      #else /* __CROSSWORKS */
        LhciScanMasterInit();
      #endif /* __CROSSWORKS */
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    #ifdef __CROSSWORKS
      LhciAdvSlaveInitSh();
    #else /* __CROSSWORKS */
      LhciAdvSlaveInit();
    #endif /* __CROSSWORKS */
    #ifdef INIT_ENCRYPTED
      #ifdef __CROSSWORKS
        LhciEncSlaveInitSh();
      #else /* __CROSSWORKS */
        LhciEncSlaveInit();
      #endif /* __CROSSWORKS */
    #endif
    #ifdef __CROSSWORKS
    LhciConnInitSh();
    #else /* __CROSSWORKS */
    LhciConnInit();
    #endif /* __CROSSWORKS */
  #else
    #ifdef INIT_BROADCASTER
      #ifdef __CROSSWORKS
        LhciAdvSlaveInitSh();
      #else /* __CROSSWORKS */
        LhciAdvSlaveInit();
      #endif /* __CROSSWORKS */
    #endif
  #endif

  #ifdef INIT_ENCRYPTED
    #ifdef __CROSSWORKS
      LhciPrivAdvInitSh();
    #else /* __CROSSWORKS */
      LhciPrivAdvInit();
    #endif /* __CROSSWORKS */
    #ifdef __CROSSWORKS
      LhciPrivConnInitSh();
    #else /* __CROSSWORKS */
      LhciPrivConnInit();
    #endif /* __CROSSWORKS */
    #ifdef __CROSSWORKS
      LhciScInitSh();
    #else /* __CROSSWORKS */
      LhciScInit();
    #endif /* __CROSSWORKS */
  #endif

  #if defined(INIT_PERIPHERAL) || defined(INIT_CENTRAL)
  #ifdef INIT_PHY
  #ifdef __CROSSWORKS
    LhciPhyInitSh();
  #else /* __CROSSWORKS */
    LhciPhyInit();
  #endif /* __CROSSWORKS */
  #endif
  #endif

  handlerId = WsfOsSetNextHandler(LhciHandler);
  LhciHandlerInit(handlerId);
}
#ifdef __CROSSWORKS
#pragma codeseg(default)

void LlInitLhciInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitLhciInitSh << 1);
  LlInitLhciInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize standard controller configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
uint32_t LlInitControllerInitSh(LlInitRtCfg_t *pCfg)
#else /* __CROSSWORKS */
uint32_t LlInitControllerInit(LlInitRtCfg_t *pCfg)
#endif /* __CROSSWORKS */
{
  uint32_t totalMemUsed;

#ifdef __CROSSWORKS
  totalMemUsed = LlInitStdInitSh(pCfg);
#else /* __CROSSWORKS */
  totalMemUsed = LlInitStdInit(pCfg);
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

  LhciVsExtInit(lhciVsExtDecodeCmdPkt);

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
uint32_t LlInitControllerInit(LlInitRtCfg_t *pCfg)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitControllerInitSh << 1);
  return LlInitControllerInitSh(pCfg);
}
#endif /* __CROSSWORKS */
