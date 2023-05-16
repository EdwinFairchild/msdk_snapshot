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
 *  \brief Generic baseband driver implementation file.
 */
/*************************************************************************************************/

#include <string.h>
#include "bb_api.h"
#include "bb_int.h"
#include "bb_drv.h"
#include "hci_drv_sdma.h"

/**************************************************************************************************
  Globals
**************************************************************************************************/

#ifdef __CROSSWORKS
extern SDMADiag_t _sdma_diag;
#endif /* __CROSSWORKS */

BbCtrlBlk_t bbCb;                       /*!< BB control block. */
const BbRtCfg_t *pBbRtCfg = NULL;       /*!< Runtime configuration. */

// Allow bb_ble_drv to convert BOD duration (ms) to DBB ticks.
// This requires access to PHY code, which is not available in bb_main.
extern uint32_t BbUsecToTMR(uint32_t u32Usec);

/**************************************************************************************************
  Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize runtime configuration.
 *
 *  \param      pCfg        Pointer to runtime configuration parameters (data must be static).
 *
 *  \return     None.
 *
 *  This function initializes the BB subsystem's runtime configuration.
 *
 *  \note       This routine must be called only once before any other initialization routine.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void BbInitRunTimeCfgSh(const BbRtCfg_t *pCfg)
#else /* __CROSSWORKS */
void BbInitRunTimeCfg(const BbRtCfg_t *pCfg)
#endif /* __CROSSWORKS */
{
  WSF_ASSERT(pBbRtCfg == NULL);
  WSF_ASSERT(pCfg);

  WSF_ASSERT(pCfg->clkPpm >= 20);
  WSF_ASSERT(pCfg->rfSetupDelayUs > 0);
  WSF_ASSERT(pCfg->maxScanPeriodMs > 0);
  WSF_ASSERT(pCfg->schSetupDelayUs > 0);

  pBbRtCfg = pCfg;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void BbInitRunTimeCfg(const BbRtCfg_t *pCfg)
{
    _sdma_diag.pfnLastShCall = ((size_t)&BbInitRunTimeCfgSh << 1);
    BbInitRunTimeCfgSh(pCfg);
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief      Initialize the BB.
 *
 *  \return     None.
 *
 *  Initialize baseband resources.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void BbInitSh(void)
#else /* __CROSSWORKS */
void BbInit(void)
#endif /* __CROSSWORKS */
{
  WSF_ASSERT(pBbRtCfg);

  BbDrvInit();

  memset(&bbCb, 0, sizeof(bbCb));
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void BbInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&BbInitSh << 1);
  BbInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief      Register operation completion handler.
 *
 *  \param      eventCback  Event callback.
 *
 *  \return     None.
 *
 *  Register operation completion handler.
 */
/*************************************************************************************************/
void BbRegister(BbBodCompCback_t eventCback)
{
  bbCb.bodCompCback = eventCback;
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbProtStart(uint8_t protId)
{
  WSF_ASSERT(bbCb.prot[protId].startProtCback != NULL);

  /* Enable protocol-specific BB. */
  bbCb.prot[protId].startProtCback();

  /* Protocol now started. */
  bbCb.protStarted   = TRUE;
  bbCb.protIdStarted = protId;
}

/*************************************************************************************************/
/*!
 *  \brief      Start BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 *
 *  Enable BB and start processing the list of BODs.  This routine may be called several times, if
 *  a protocol layers has several simultaneously-enabled operations.  However, \ref BbStop() must
 *  be called an equal number of time to disable the baseband.
 */
/*************************************************************************************************/
void BbStart(uint8_t protId)
{
  WSF_ASSERT(protId < BB_PROT_NUM);

  if (!bbCb.protStarted)
  {
    /* Enable generic BB. */
    BbDrvEnable();

    /* Enable protocol-specific BB. */
    bbProtStart(protId);
  }

  bbCb.prot[protId].startCnt++;
}

/*************************************************************************************************/
/*!
 *  \brief      Stop BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void bbProtStop(uint8_t protId)
{
  WSF_ASSERT(bbCb.prot[protId].stopProtCback != NULL);

  /* No protocol started. */
  bbCb.protStarted = FALSE;

  /* Disable protocol-specific BB. */
  bbCb.prot[protId].stopProtCback();
}

/*************************************************************************************************/
/*!
 *  \brief      Stop BB processing of given protocol.
 *
 *  \param      protId  Protocol ID.
 *
 *  Disable BB processing of BODs.
 *
 *  \note       For any particular protocol, calls to \ref BbStart() and \ref BbStop() must be
 *              balanced to ensure that the hardware is disabled if and only if appropriate.
 */
/*************************************************************************************************/
void BbStop(uint8_t protId)
{
  WSF_ASSERT(protId < BB_PROT_NUM);
  WSF_ASSERT(bbCb.prot[protId].startCnt > 0);

  bbCb.prot[protId].startCnt--;

  if (bbCb.protStarted && (bbCb.protIdStarted == protId) && (bbCb.prot[protId].startCnt == 0))
  {
    /* Disable protocol-specific BB. */
    bbProtStop(protId);

    /* Disable generic BB. */
    BbDrvDisable();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Execute BOD.
 *
 *  \param      pBod    Pointer to the BOD to execute.
 *
 *  \return     None.
 *
 *  Execute the protocol specific BOD handler.
 */
/*************************************************************************************************/
void BbExecuteBod(BbOpDesc_t *pBod)
{
  WSF_ASSERT(pBod);

  WSF_ASSERT(pBod->protId < BB_PROT_NUM);
  /* TODO: Removed this assert as it spuriously seems to be taken. */
  /* WSF_ASSERT(!bbCb.pOpInProgress); */
  bbCb.pOpInProgress = pBod;
  bbCb.termBod = FALSE;

  /* Enable generic BB. */
  if (!bbCb.protStarted)
  {
    BbDrvEnable();
  }

  /* Switch protocols if necessary. */
  if (bbCb.protStarted && (bbCb.protIdStarted != pBod->protId))
  {
    /* Disable protocol-specific BB. */
    bbProtStop(bbCb.protIdStarted);   /* sets bbCb.protStarted = FALSE */
  }
  if (!bbCb.protStarted)
  {
    /* TODO: Removed this assert as it prevents background BOD. Seems unnecessary? */
    /* WSF_ASSERT(bbCb.prot[pBod->protId].startCnt > 0); */

    /* Enable protocol-specific BB. */
    bbProtStart(pBod->protId);
  }

  if (bbCb.prot[pBod->protId].execOpCback != NULL)
  {
    bbCb.prot[pBod->protId].execOpCback(pBod);
  }

  if (bbCb.termBod)
  {
    bbCb.pOpInProgress = NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel current executing BOD.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbCancelBod(void)
{
  if (bbCb.pOpInProgress)
  {
    BbOpDesc_t * const pBod = bbCb.pOpInProgress;

    WSF_ASSERT(pBod->protId < BB_PROT_NUM);
    if (bbCb.prot[pBod->protId].cancelOpCback != NULL)
    {
      bbCb.prot[pBod->protId].cancelOpCback(pBod);
    }

    bbCb.pOpInProgress = NULL;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Get the currently-executing BOD.
 *
 *  \return     Currently-executing BOD.
 */
/*************************************************************************************************/
BbOpDesc_t *BbGetCurrentBod(void)
{
  return bbCb.pOpInProgress;
}

/*************************************************************************************************/
/*!
 *  \brief      Cancel current executing BOD.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD, typically in the related ISRs.
 */
/*************************************************************************************************/
void BbSetBodTerminateFlag(void)
{
  if (bbCb.pOpInProgress)
  {
    bbCb.termBod = TRUE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Get termination state of current executing BOD.
 *
 *  \return     TRUE if termination flag set, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t BbGetBodTerminateFlag(void)
{
  return bbCb.termBod;
}

/*************************************************************************************************/
/*!
 *  \brief      Terminate a BOD immediately.
 *
 *  \return     None.
 *
 *  \note       This function is expected to be called during the execution context of the
 *              current executing BOD, typically in the related ISRs.
 */
/*************************************************************************************************/
void BbTerminateBod(void)
{
  WSF_ASSERT(bbCb.bodCompCback);

  bbCb.pOpInProgress = NULL;
  bbCb.termBod = TRUE;
  bbCb.bodCompCback();
}

/*************************************************************************************************/
/*!
 *  \brief      Get BB clock accuracy.
 *
 *  \return     Clock accuracy in part per million.
 *
 *  Returns the current BB clock accuracy.
 */
/*************************************************************************************************/
uint16_t BbGetClockAccuracy(void)
{
  return pBbRtCfg->clkPpm;
}

/*************************************************************************************************/
/*!
 *  \brief      Get scheduler setup delay.
 *
 *  \return     Scheduler setup delay in microseconds.
 *
 *  Returns the scheduler setup delay.
 */
/*************************************************************************************************/
uint16_t BbGetSchSetupDelayUs(void)
{
  return pBbRtCfg->schSetupDelayUs;
}

/*************************************************************************************************/
/*!
 *  \brief      Returns the ID of the active protocol.
 *
 *  \return     Protocol operation in progress.
 */
/*************************************************************************************************/
uint8_t BbGetActiveProtocol(void)
{
  return bbCb.protIdStarted;
}

/*************************************************************************************************/
/*!
 *  \brief      Register protocol handlers.
 *
 *  \param      protId          Protocol ID.
 *  \param      execOpCback     Execute operation callback.
 *  \param      cancelOpCback   Cancel operation callback.
 *  \param      startProtCback  Start protocol callback.
 *  \param      stopProtCback   Stop protocol callback.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void BbRegisterProt(uint8_t protId, BbBodCback_t execOpCback, BbBodCback_t cancelOpCback,
    BbProtCback_t startProtCback, BbProtCback_t stopProtCback)
{
  WSF_ASSERT(protId < BB_PROT_NUM);
  WSF_ASSERT(startProtCback != NULL);
  WSF_ASSERT(stopProtCback != NULL);

  bbCb.prot[protId].execOpCback    = execOpCback;
  bbCb.prot[protId].cancelOpCback  = cancelOpCback;
  bbCb.prot[protId].startProtCback = startProtCback;
  bbCb.prot[protId].stopProtCback  = stopProtCback;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the time of the next BOD transition in DBB ticks.
 *
 *  A BOD transition is defined as either a BOD due time, or the BOD due time plus the
 *  maximum possible BOD duration. The current DBB tick count is subtracted from the
 *  expected DBB tick count at the next BOD transition. This value is returned via
 *  the pi32RelDueTime parameter.
 *
 *  \param      pi32RelDueTime The location where the relative due time is to be written.
 *
 *  \return     TRUE if there is a bod scheduled.
 */
/*************************************************************************************************/
bool_t BbGetNextRelativeDueTime(int32_t *pi32RelDueTime)
{
    const BbOpDesc_t *pOp = bbCb.pOpInProgress;
    uint32_t u32CurTime;
    uint32_t u32Due;
    uint32_t u32MaxDur;
    uint32_t u32Delta;
    bool_t bHaveRelDueTime;

    bHaveRelDueTime = FALSE;
    
    if (pOp != NULL) {
        u32CurTime = BbDrvGetCurrentTime();
        u32Due = pOp->due;
        u32MaxDur = BbUsecToTMR(pOp->maxDurUsec);

        u32Delta = u32Due - u32CurTime;
        // Interperet as signed here because the due time for the current BOD
        // may have already passed. This is ambiguous since the due time could
        // also be nearly 0xFFFFFFFF ticks greater than the current time.
        // If this is true, then we might end up delaying the SDMA's issuance
        // of an interrupt/wake event until it becomes clear that this BOD
        // is scheduled in the future. This would cost some power for very
        // long connection intervals.
        if ((signed)(u32Delta) >= 0) {
            bHaveRelDueTime = TRUE;
        }
        else {
            // The BOD is in the past (probably). Is it currently running?
            u32Delta += u32MaxDur;
            if ((signed)(u32Delta) >= 0) {
                // It is currently running.
                bHaveRelDueTime = TRUE;
                *pi32RelDueTime = (int32_t)(u32Delta);
            }
            else {
                // It is (or should be) completed.
                // Same as having no BOD scheduled.
                // The SDMA should go to sleep.
                bHaveRelDueTime = FALSE;
            }
        }
    }

    return (bHaveRelDueTime);
}
