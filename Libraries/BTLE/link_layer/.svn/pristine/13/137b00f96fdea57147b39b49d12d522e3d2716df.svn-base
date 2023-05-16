/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief LL initialization for SoC configuration.
 */
/*************************************************************************************************/

#include "ll_init_api.h"
#include "bb_ble_api.h"
#include "sch_api.h"
#include "hci_drv_sdma.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

#ifdef __CROSSWORKS
extern SDMADiag_t _sdma_diag;
extern uint16_t BbBleInitResolvingListSh(uint8_t numEntries, uint8_t *pFreeMem, uint32_t freeMemSize);
#endif /* __CROSSWORKS */

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize BB.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LlInitBbInitSh(void)
#else /* __CROSSWORKS */
void LlInitBbInit(void)
#endif /* __CROSSWORKS */
{
#ifdef __CROSSWORKS
  BbInitSh();
#else /* __CROSSWORKS */
  BbInit();
#endif /* __CROSSWORKS */

  BbBleInit();

  #ifdef INIT_CENTRAL
    BbBleScanMasterInit();
    BbBleConnMasterInit();
  #else
    #ifdef INIT_OBSERVER
      BbBleScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    #ifdef __CROSSWORKS
      BbBleAdvSlaveInitSh();
    #else /* __CROSSWORKS */
      BbBleAdvSlaveInit();
    #endif /* __CROSSWORKS */
    #ifdef __CROSSWORKS
      BbBleConnSlaveInitSh();
    #else /* __CROSSWORKS */
      BbBleConnSlaveInit();
    #endif /* __CROSSWORKS */
  #else
    #ifdef INIT_BROADCASTER
      #ifdef __CROSSWORKS
        BbBleAdvSlaveInitSh();
      #else /* __CROSSWORKS */
        BbBleAdvSlaveInit();
      #endif /* __CROSSWORKS */
    #endif
  #endif

  #ifdef __CROSSWORKS
    BbBleTestInitSh();
  #else /* __CROSSWORKS */
    BbBleTestInit();
  #endif /* __CROSSWORKS */
}
#ifdef __CROSSWORKS
#pragma codeseg(default)

void LlInitBbInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitBbInitSh << 1);
  LlInitBbInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize scheduler.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
#endif /* __CROSSWORKS */
void LlInitSchInit(void)
{
  wsfHandlerId_t handlerId = WsfOsSetNextHandler(SchHandler);
#ifdef __CROSSWORKS
  SchHandlerInitSh(handlerId);
#else /* __CROSSWORKS */
  SchHandlerInit(handlerId);
#endif /* __CROSSWORKS */
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL.
 *
 *  \param  initHandler     Initialize WSF handler.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
#endif /* __CROSSWORKS */
void LlInitLlInit(bool_t initHandler)
{
  if (initHandler)
  {
    wsfHandlerId_t handlerId = WsfOsSetNextHandler(LlHandler);
#ifdef __CROSSWORKS
    LlHandlerInitSh(handlerId);
#else /* __CROSSWORKS */
    LlHandlerInit(handlerId);
#endif /* __CROSSWORKS */
  }
  /* else LlHandlerInit() is called by client */

  #ifdef INIT_CENTRAL
    LlScanMasterInit();
    LlInitMasterInit();
    #ifdef INIT_ENCRYPTED
      LlEncConnMasterInit();
    #else
      LlConnMasterInit();
    #endif
    #ifdef INIT_PHY
    LlPhyMasterInit();
    #endif
  #else
    #ifdef INIT_OBSERVER
      LlScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    LlAdvSlaveInit();
    #ifdef INIT_ENCRYPTED
      LlEncConnSlaveInit();
    #else
      LlConnSlaveInit();
    #endif
    #ifdef INIT_PHY
    LlPhySlaveInit();
    #endif
  #else
    #ifdef INIT_BROADCASTER
      LlAdvSlaveInit();
    #endif
  #endif

  #ifdef INIT_ENCRYPTED
    LlPrivInit();
    LlScInit();
  #endif
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Set BB runtime configuration.
 *
 *  \param  pBbRtCfg        BB runtime configuration (must be static).
 *  \param  wlSizeCfg       Whitelist size.
 *  \param  rlSizeCfg       Resolving list size.
 *  \param  plSizeCfg       Periodic list size.
 *  \param  pFreeMem        Free memory.
 *  \param  freeMemAvail    Amount of free memory in bytes.
 *
 *  \return Amount of free memory consumed.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
uint32_t LlInitSetBbRtCfgSh(const BbRtCfg_t *pBbRtCfg, const uint8_t wlSizeCfg, const uint8_t rlSizeCfg,
                          const uint8_t plSizeCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
#else /* __CROSSWORKS */
uint32_t LlInitSetBbRtCfg(const BbRtCfg_t *pBbRtCfg, const uint8_t wlSizeCfg, const uint8_t rlSizeCfg,
                          const uint8_t plSizeCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
#endif /* __CROSSWORKS */
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

#ifdef __CROSSWORKS
  BbInitRunTimeCfgSh(pBbRtCfg);
#else /* __CROSSWORKS */
  BbInitRunTimeCfg(pBbRtCfg);
#endif /* __CROSSWORKS */

#ifdef __CROSSWORKS
  memUsed = BbBleInitWhiteListSh(wlSizeCfg, pFreeMem, freeMemAvail);
#else /* __CROSSWORKS */
  memUsed = BbBleInitWhiteList(wlSizeCfg, pFreeMem, freeMemAvail);
#endif /* __CROSSWORKS */
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

#ifdef __CROSSWORKS
  memUsed = BbBleInitPeriodicListSh(plSizeCfg, pFreeMem, freeMemAvail);
#else /* __CROSSWORKS */
  memUsed = BbBleInitPeriodicList(plSizeCfg, pFreeMem, freeMemAvail);
#endif /* __CROSSWORKS */
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  #ifdef INIT_ENCRYPTED
    #ifdef __CROSSWORKS
      memUsed = BbBleInitResolvingListSh(rlSizeCfg, pFreeMem, freeMemAvail);
    #else /* __CROSSWORKS */
      memUsed = BbBleInitResolvingList(rlSizeCfg, pFreeMem, freeMemAvail);
    #endif /* __CROSSWORKS */
    /* pFreeMem += memUsed; */
    /* freeMemAvail -= memUsed; */
    totalMemUsed += memUsed;
  #else
    BbBleInitResolvingList(0, NULL, 0);
  #endif

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)

uint32_t LlInitSetBbRtCfg(const BbRtCfg_t *pBbRtCfg, const uint8_t wlSizeCfg, const uint8_t rlSizeCfg,
                          const uint8_t plSizeCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitSetBbRtCfgSh << 1);
  return LlInitSetBbRtCfgSh(pBbRtCfg, wlSizeCfg, rlSizeCfg, plSizeCfg, pFreeMem, freeMemAvail);
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Set LL runtime configuration.
 *
 *  \param  pLlRtCfg        LL runtime configuration (must be static).
 *  \param  pFreeMem        Free memory.
 *  \param  freeMemAvail    Amount of free memory in bytes.
 *
 *  \return Amount of free memory consumed.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
uint32_t LlInitSetLlRtCfgSh(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
#else /* __CROSSWORKS */
uint32_t LlInitSetLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
#endif /* __CROSSWORKS */
{
  uint32_t totalMemUsed = 0;
  #if defined (INIT_PERIPHERAL) || defined (INIT_CENTRAL)
  uint32_t memUsed;
  #endif

  LlInitRunTimeCfg(pLlRtCfg);

  #if defined (INIT_PERIPHERAL) || defined (INIT_CENTRAL)
    memUsed = LlInitConnMem(pFreeMem, freeMemAvail);
    /* pFreeMem += memUsed; */
    /* freeMemAvail -= memUsed; */
    totalMemUsed += memUsed;
  #endif

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")

uint32_t LlInitSetLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitSetLlRtCfgSh << 1);
  return LlInitSetLlRtCfgSh(pLlRtCfg, pFreeMem, freeMemAvail);
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize standard configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
uint32_t LlInitStdInitSh(LlInitRtCfg_t *pCfg)
#else /* __CROSSWORKS */
uint32_t LlInitStdInit(LlInitRtCfg_t *pCfg)
#endif /* __CROSSWORKS */
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

#ifdef __CROSSWORKS
  memUsed = LlInitSetBbRtCfgSh(pCfg->pBbRtCfg, pCfg->wlSizeCfg, pCfg->rlSizeCfg, pCfg->plSizeCfg,
                            pCfg->pFreeMem, pCfg->freeMemAvail);
#else /* __CROSSWORKS */
  memUsed = LlInitSetBbRtCfg(pCfg->pBbRtCfg, pCfg->wlSizeCfg, pCfg->rlSizeCfg, pCfg->plSizeCfg,
                            pCfg->pFreeMem, pCfg->freeMemAvail);
#endif /* __CROSSWORKS */
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

#ifdef __CROSSWORKS
  memUsed = LlInitSetLlRtCfgSh(pCfg->pLlRtCfg, pCfg->pFreeMem, pCfg->freeMemAvail);
#else /* __CROSSWORKS */
  memUsed = LlInitSetLlRtCfg(pCfg->pLlRtCfg, pCfg->pFreeMem, pCfg->freeMemAvail);
#endif /* __CROSSWORKS */
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

#ifdef __CROSSWORKS
  LlInitBbInitSh();
#else /* __CROSSWORKS */
  LlInitBbInit();
#endif /* __CROSSWORKS */
  LlInitSchInit();
  LlInitLlInit(TRUE);

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)

uint32_t LlInitStdInit(LlInitRtCfg_t *pCfg)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitStdInitSh << 1);
  return LlInitStdInitSh(pCfg);
}
#endif /* __CROSSWORKS */
