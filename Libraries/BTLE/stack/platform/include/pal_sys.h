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
 *  \brief System hooks.
 */
/*************************************************************************************************/

#ifndef PAL_SYS_H
#define PAL_SYS_H

#include "stack/platform/include/pal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! \addtogroup PAL_SYS
 *  \{ */

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Sleep states. */
typedef enum
{
  PAL_SYS_SLEEP = 0,        /*!< Normal sleep state, any interrupt can wake. */
  PAL_SYS_DEEP_SLEEP,       /*!< Deep Sleep state, use RTC/Wakeup timer to wake. */
  PAL_SYS_BACKUP            /*!< Backup state, same as Deep Sleep with reset. */
} PalSysSleep_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/
void PalEnterCs(void);
void PalExitCs(void);
void PalSysAssertTrap(void);
void PalSysSetTrap(bool_t enable);
uint32_t PalSysGetAssertCount(void);
uint32_t PalSysGetStackUsage(void);
void PalSysSleep(void);
bool_t PalSysIsBusy(void);
void PalSysSetBusy(void);
void PalSysSetIdle(void);
void PalSysInit(void);
void PalSysInit(void);
PalSysSleep_t PalSysGetSleepState(void);
void PalSysSetSleepState(PalSysSleep_t state);

/*! \} */    /* PAL_SYS */

#ifdef __cplusplus
};
#endif

#endif  /* PAL_SYS_H */
