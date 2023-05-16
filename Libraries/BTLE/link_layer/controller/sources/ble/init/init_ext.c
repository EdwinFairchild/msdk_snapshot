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
 *  \brief LL initialization for extended SoC configuration.
 */
/*************************************************************************************************/

#include "ll_init_api.h"
#include "bb_ble_api.h"
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

/*************************************************************************************************/
/*!
 *  \brief  Initialize auxiliary BB operations.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LlInitBbAuxInitSh(void)
#else /* __CROSSWORKS */
void LlInitBbAuxInit(void)
#endif /* __CROSSWORKS */
{
  #ifdef INIT_OBSERVER
    BbBleAuxScanMasterInit();
    BbBlePerScanMasterInit();
  #endif

  #ifdef INIT_BROADCASTER
    BbBleAuxAdvSlaveInit();
    BbBleInitRfPathComp();
  #endif
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void LlInitBbAuxInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitBbAuxInitSh << 1);
  LlInitBbAuxInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize LL extended features.
 *
 *  \return None.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LlInitLlExtInitSh(void)
#else /* __CROSSWORKS */
void LlInitLlExtInit(void)
#endif /* __CROSSWORKS */
{
  wsfHandlerId_t handlerId;

  #ifdef INIT_CENTRAL
    LlExtScanMasterInit();
    LlExtInitMasterInit();
  #ifdef INIT_PHY
    LlPhyMasterInit();
  #endif
  #else
    #ifdef INIT_OBSERVER
      LlExtScanMasterInit();
    #endif
  #endif

  #ifdef INIT_PERIPHERAL
    LlExtAdvSlaveInit();
  #ifdef INIT_PHY
    LlPhySlaveInit();
  #endif
  #else
    #ifdef INIT_BROADCASTER
      LlExtAdvSlaveInit();
    #endif
  #endif

  #if defined(INIT_PERIPHERAL) || defined(INIT_CENTRAL)
    LlChannelSelection2Init();
  #endif
  /* Initialize handler after feature bits are set. */
  handlerId = WsfOsSetNextHandler(LlHandler);
#ifdef __CROSSWORKS
  LlHandlerInitSh(handlerId);
#else /* __CROSSWORKS */
  LlHandlerInit(handlerId);
#endif /* __CROSSWORKS */
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void LlInitLlExtInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitLlExtInitSh << 1);
  LlInitLlExtInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Set extended LL runtime configuration.
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
uint32_t LlInitSetExtLlRtCfgSh(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
#else /* __CROSSWORKS */
uint32_t LlInitSetExtLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
#endif /* __CROSSWORKS */
{
  uint32_t memUsed;
  uint32_t totalMemUsed = 0;

#ifdef __CROSSWORKS
  memUsed = LlInitSetLlRtCfgSh(pLlRtCfg, pFreeMem, freeMemAvail);
#else /* __CROSSWORKS */
  memUsed = LlInitSetLlRtCfg(pLlRtCfg, pFreeMem, freeMemAvail);
#endif /* __CROSSWORKS */
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = LlInitExtScanMem(pFreeMem, freeMemAvail);
  pFreeMem += memUsed;
  freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

  memUsed = LlInitExtAdvMem(pFreeMem, freeMemAvail);
  /* pFreeMem += memUsed; */
  /* freeMemAvail -= memUsed; */
  totalMemUsed += memUsed;

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
uint32_t LlInitSetExtLlRtCfg(const LlRtCfg_t *pLlRtCfg, uint8_t *pFreeMem, uint32_t freeMemAvail)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitSetExtLlRtCfgSh << 1);
  return LlInitSetExtLlRtCfgSh(pLlRtCfg, pFreeMem, freeMemAvail);
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Initialize extended configuration.
 *
 *  \param  pCfg    Runtime configuration.
 *
 *  \return Memory used.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
uint32_t LlInitExtInitSh(LlInitRtCfg_t *pCfg)
#else /* __CROSSWORKS */
uint32_t LlInitExtInit(LlInitRtCfg_t *pCfg)
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
  memUsed = LlInitSetExtLlRtCfgSh(pCfg->pLlRtCfg, pCfg->pFreeMem, pCfg->freeMemAvail);
#else /* __CROSSWORKS */
  memUsed = LlInitSetExtLlRtCfg(pCfg->pLlRtCfg, pCfg->pFreeMem, pCfg->freeMemAvail);
#endif /* __CROSSWORKS */
  pCfg->pFreeMem += memUsed;
  pCfg->freeMemAvail -= memUsed;
  totalMemUsed += memUsed;

#ifdef __CROSSWORKS
  LlInitBbInitSh();
#else /* __CROSSWORKS */
  LlInitBbInit();
#endif /* __CROSSWORKS */
#ifdef __CROSSWORKS
  LlInitBbAuxInitSh();
#else /* __CROSSWORKS */
  LlInitBbAuxInit();
#endif /* __CROSSWORKS */
  LlInitSchInit();
  LlInitLlInit(FALSE);
#ifdef __CROSSWORKS
  LlInitLlExtInitSh();
#else /* __CROSSWORKS */
  LlInitLlExtInit();
#endif /* __CROSSWORKS */

  return totalMemUsed;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
uint32_t LlInitExtInit(LlInitRtCfg_t *pCfg)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LlInitExtInitSh << 1);
  return LlInitExtInitSh(pCfg);
}
#endif /* __CROSSWORKS */
