/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  System hooks.
 *
 *  Copyright (c) 2016-2019 Arm Ltd.
 *
 *  Copyright (c) 2019 Packetcraft, Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/
#ifdef __ICCARM__
#pragma diag_warning=Pe223
#endif


#include "pal_sys.h"
#include "pal_led.h"
#include "pal_rtc.h"
#include "wsf_cs.h"
#include "max32665.h"

#ifdef __IAR_SYSTEMS_ICC__
#include <intrinsics.h>
#endif

/*! \brief Value of PRIMASK during first nested critical section entry. */
static uint32_t palInitPrimask = 0u;

/*! \brief      Number of assertions. */
static uint32_t palSysAssertCount;

/*! \brief      Trap enabled flag. */
static volatile bool_t PalSysAssertTrapEnable;

static uint32_t palSysBusyCount;

static PalSysSleep_t sleepState;

/*************************************************************************************************/
/*!
 *  \brief  Enter a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalEnterCs(void)
{

  /* PRIMASK contains the current interrupt enable state of the Cortex-M4.
     Save the current state of interrupt enable state so we can restore when 
     exiting the critical section.
   */

  palInitPrimask = __get_PRIMASK();

  /* Possible to be interrupted here where PRIMASK can change. 
     If interrupting thread calls PalEnterCs, PRIMASK will change to 1, causing 
     palInitPrimask to be inconsistent. 

     Assumption is that PalEnterCs and PalExitCs calls are matched. 

     With this assumption, interrupting process will also call PalExitCs, resetting 
     PRIMASK to 0, making palInitPrimask consistent.
   */

#ifdef __IAR_SYSTEMS_ICC__
    __disable_interrupt();
#endif
#ifdef __GNUC__
    __asm volatile ("cpsid i"); // PRIMASK -> 1, interrupts disabled
#endif
#if (defined(__CC_ARM) || defined(__CROSSWORKS))
    __disable_irq();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief  Exit a critical section.
 *
 *  \return None.
 */
/*************************************************************************************************/
void PalExitCs(void)
{
  /* Re-enable interrupts if interrupts were disabled as a result of calling PalEnterCs */
  if (palInitPrimask == 0)
  {
#ifdef __IAR_SYSTEMS_ICC__
    __enable_interrupt();
#endif
#ifdef __GNUC__
    __asm volatile ("cpsie i");  // PRIMASK -> 0, interrupts enabled
#endif
#if (defined(__CC_ARM) || defined(__CROSSWORKS))
    __enable_irq();
#endif
  }
}

/*************************************************************************************************/
/*!
 *  \brief      System fault trap.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysAssertTrap(void)
{
  PalLedOn(PAL_LED_ID_ERROR);

  palSysAssertCount++;

  while (PalSysAssertTrapEnable);
}

/*************************************************************************************************/
/*!
 *  \brief      Set system trap.
 *
 *  \param      enable    Enable assert trap or not.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysSetTrap(bool_t enable)
{
  PalSysAssertTrapEnable = enable;
}

/*************************************************************************************************/
/*!
 *  \brief      Get assert count.
 *
 *  \return     None.
 */
/*************************************************************************************************/
uint32_t PalSysGetAssertCount(void)
{
  return palSysAssertCount;
}

/*************************************************************************************************/
/*!
 *  \brief      Count stack usage.
 *
 *  \return     Number of bytes used by the stack.
 */
/*************************************************************************************************/
uint32_t PalSysGetStackUsage(void)
{
  /* Not available; stub routine. */
  return 0;
}

/*************************************************************************************************/
/*!
 *  \brief      System sleep.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSleep(void)
{
#ifdef __IAR_SYSTEMS_ICC__
  __WFI();
#endif
#ifdef __GNUC__
  __asm volatile ("wfi");
#endif
#ifdef __CC_ARM
  __wfi();
#endif
}

/*************************************************************************************************/
/*!
 *  \brief      Check if system is busy.
 *
 *  \return     True if system is busy.
 */
/*************************************************************************************************/
bool_t PalSysIsBusy(void)
{
  bool_t sysIsBusy = FALSE;
  WsfCsEnter();
  sysIsBusy = ((palSysBusyCount == 0) ? FALSE : TRUE);
  WsfCsExit();
  return sysIsBusy;
}

/*************************************************************************************************/
/*!
 *  \brief      Set system busy.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSetBusy(void)
{
  WsfCsEnter();
  palSysBusyCount++;
  WsfCsExit();
}

/*************************************************************************************************/
/*!
 *  \brief      Set system idle.
 *
 *  \return     none.
 */
/*************************************************************************************************/
void PalSysSetIdle(void)
{
  WsfCsEnter();
  if (palSysBusyCount) {
    palSysBusyCount--;
  }
  WsfCsExit();
}

/*************************************************************************************************/
/*!
 *  \brief      Common platform initialization.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalSysInit(void)
{
  PalRtcInit();
  PalLedOff(PAL_LED_ID_ERROR);
  PalLedOn(PAL_LED_ID_CPU_ACTIVE);
  sleepState = PAL_SYS_SLEEP;
}

/*************************************************************************************************/
/*!
 *  \brief      Get the sleep state.
 *
 *  \return     Sleep state for the next sleep event.
 */
/*************************************************************************************************/
PalSysSleep_t PalSysGetSleepState(void)
{
  return sleepState;
}

/*************************************************************************************************/
/*!
 *  \brief      Set the sleep state for the next sleep event.
 *
 *  \return     None.
 */
/*************************************************************************************************/

void PalSysSetSleepState(PalSysSleep_t state)
{
  sleepState = state;
}
