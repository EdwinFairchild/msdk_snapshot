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
 *  \brief Link manager master role implementation file.
 */
/*************************************************************************************************/

#include "lmgr_api_adv_master.h"
#include "bb_ble_api.h"
#include <string.h>

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/*! \brief      Master role device parameters. */
lmgrMstScanCtrlBlk_t lmgrMstScanCb;

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer manager master mode resources.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LmgrMstInit(void)
{
  static lmgrScanParam_t defScanParam;
  defScanParam.scanType       = LL_SCAN_PASSIVE;
  defScanParam.scanInterval   = 0x0010;
  defScanParam.scanWindow     = 0x0010;
  defScanParam.ownAddrType    = LL_ADDR_PUBLIC;
  defScanParam.scanFiltPolicy = LL_SCAN_FILTER_NONE;

  memset(&lmgrMstScanCb, 0, sizeof(lmgrMstScanCb));
  lmgrMstScanCb.scanParam = defScanParam;
  lmgrMstScanCb.chanClass = LL_CHAN_DATA_ALL;
  lmgrMstScanCb.scanChanMap = LL_ADV_CHAN_ALL;
}
