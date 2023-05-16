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
 *  \brief Link layer (LL) Host Controller Interface (HCI) initialization implementation file.
 */
/*************************************************************************************************/

#include "lhci_int.h"
#include "hci_drv_sdma.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

#ifdef __CROSSWORKS
extern SDMADiag_t _sdma_diag;
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL HCI subsystem for master mode.
 *
 *  \return     None.
 *
 *  This function initializes the LL HCI subsystem for master commands.  It is typically called
 *  once upon system initialization.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void LhciExtScanMasterInitSh(void)
#else /* __CROSSWORKS */
void LhciExtScanMasterInit(void)
#endif /* __CROSSWORKS */
{
  lhciCmdTbl[LHCI_MSG_EXT_SCAN] = lhciMstExtScanDecodeCmdPkt;
  lhciEvtTbl[LHCI_MSG_EXT_SCAN] = lhciMstExtScanEncodeEvtPkt;

  lhciCmdTbl[LHCI_MSG_PER_SCAN] = lhciMstExtScanDecodeCmdPkt;
  lhciEvtTbl[LHCI_MSG_PER_SCAN] = lhciMstExtScanEncodeEvtPkt;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void LhciExtScanMasterInit(void)
{
  _sdma_diag.pfnLastShCall = ((size_t)&LhciExtScanMasterInitSh << 1);
  LhciExtScanMasterInitSh();
}
#endif /* __CROSSWORKS */
