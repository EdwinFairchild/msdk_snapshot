/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI driver implementation file.
 *
 *  $Date: 2021-08-03 10:16:23 -0500 (Tue, 03 Aug 2021) $
 *  $Revision: 58927 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "wsf_cs.h"
#include "chci_tr_serial.h"
#include "chci_api.h"
#include "hci_defs.h"
#include "uart_regs.h"
#include "gpio_regs.h"
#include "gcr_regs.h"
#include "sema_regs.h"
#include "wsf_trace.h"
#include "wsf_assert.h"
#include "common.h"
#include "ipc_defs.h"
#include "sdma_regs.h"
#include "ll_debug.h"
#include "wut_regs.h"
#include "wakeup_sdma.h"
#include "hci_drv_sdma.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

#define ENABLE_ARM_HCI_INT

//#define BOD_DELAY_WAKEUP_THRESHOLD (3U * 1000000U / 625U) // 3ms

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
extern SDMADiag_t _sdma_diag;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

extern bool_t BbGetNextRelativeDueTime(int32_t *pi32RelDueTime);

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \fn     lhciDrvRead
 *  \param  data    Buffer to store data.
 *
 *  \return Number of data bytes written.
 *  \brief  Read data from the shared memory segment.
 */
/*************************************************************************************************/
void INT_IN_IRQHandler(void) __interrupt {
    uint32_t sema_val;

    ll_dbg_pin_assert(LL_DBG_PIN_SDMA_IN);

    do {
        // Attempt to lock the ARM_SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];
    } while (sema_val);

    // Clear dirty bit here.
    if (!SDMA_SHARED_MEM->hci.dirty)
    {
        chciTrSerialRxIncoming((uint8_t*)SDMA_SHARED_MEM->hci.data, SDMA_SHARED_MEM->hci.len);
        SDMA_SHARED_MEM->hci.dirty = 1;
    }

    // Unlock the ARM_SDMA_SEMA
    MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0x0;

    // Clear ARM interrupt flag.
    // ARM is polling on this so this will release
    // ARM transmit.
    MXC_SDMA->int_in_flag = 1;
 
    ll_dbg_pin_deassert(LL_DBG_PIN_SDMA_IN);
}

/*************************************************************************************************/
/*!
 *  \fn     lhciDrvInit
 *
 *  \brief  Initialize the driver.
 */
/*************************************************************************************************/
#ifdef __CROSSWORKS
#pragma codeseg("SHCODE")
void chciDrvInitSh(void)
#else /* __CROSSWORKS */
void chciDrvInit(void)
#endif /* __CROSSWORKS */
{
#ifdef DEBUG
    uint32_t sema_val;

    do {
        // Attempt to lock the ARM_SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];

        // Only called during startup. ARM should be waiting for us to return.
        MXC_ASSERT(sema_val != 0);
    } while (sema_val);
#endif /* DEBUG */

    // Initialize the SDMA shared memory segment
    memset((void*)SDMA_SHARED_MEM->hci.data, 0, SHARED_MEM_DATA_LEN);
    SDMA_SHARED_MEM->hci.dirty = 1;
    SDMA_SHARED_MEM->hci.len = 0;

#ifdef DEBUG
    // Unlock the ARM_SDMA_SEMA
    MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0x0;
#endif /* DEBUG */

    // Enable interrupt from ARM
    MXC_SDMA->int_in_ie = 1;
}
#ifdef __CROSSWORKS
#pragma codeseg(default)
void chciDrvInit(void) {
    _sdma_diag.pfnLastShCall = ((size_t)&chciDrvInitSh << 1);
    chciDrvInitSh();
}
#endif /* __CROSSWORKS */

/*************************************************************************************************/
/*!
 *  \brief  Send data to the driver.
 *
 *  \param  type     HCI packet type
 *  \param  len      Number of bytes to write.
 *  \param  pData    Byte array to write.
 *
 *  \return Return actual number of data bytes written.
 *
 *  \note   The type parameter allows the driver layer to prepend the data with a header on the
 *          same write transaction.
 */
/*************************************************************************************************/
uint16_t chciDrvWrite(uint8_t prot, uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t len_written = 0;
    uint32_t sema_val;
    uint8_t dirty;

    bool_t bHaveRelDueTime;
    int32_t i32RelDueTime;

    // Convert CHCI to HCI
    uint8_t hciType = (type == CHCI_TR_TYPE_EVT) ? HCI_EVT_TYPE : HCI_ACL_TYPE;

    ll_dbg_pin_assert(LL_DBG_PIN_SDMA_WRITE);

    // wake_ARM();

    // Add 1 byte for type
    len++;

    WsfCsEnter();

    // Attempt to lock the ARM_SDMA_SEMA
    // Reading the register does an atomic test and set, returns previous value
    sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];

    // If we hold the semaphore, read the dirty bit.
    if (!sema_val) dirty = ARM_SHARED_MEM->hci.dirty;

    // Write the data to the shared memory segment
    if (dirty) {

        if (len > SHARED_MEM_LEN) {
            len_written = SHARED_MEM_LEN;
        } else {
            len_written = len;
        }

        memcpy((void*)ARM_SHARED_MEM->hci.data,(void*)&hciType, 1);
        memcpy((void*)&ARM_SHARED_MEM->hci.data[1], (void*)pData, len_written-1);
        ARM_SHARED_MEM->hci.len = len_written;
        ARM_SHARED_MEM->hci.dirty = 0;
    }

    // Unlock the ARM_SEMA
    if (!sema_val) {
        MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0x0;
    }
 

    // Interrupt the ARM to signal there is data to process
#ifdef BOD_DELAY_WAKEUP_THRESHOLD
    if (len_written) {
        // But hold of on the notification if a BOD i in progress.
        // We can wake the ARM when this is completed.
        bHaveRelDueTime = BbGetNextRelativeDueTime(&i32RelDueTime);
        if (!bHaveRelDueTime ||
                (i32RelDueTime > BOD_DELAY_WAKEUP_THRESHOLD)) {
            wake_ARM();
            interrupt_ARM();
        }
    }

#else /* BOD_DELAY_WAKEUP_THRESHOLD */
#ifdef ENABLE_ARM_HCI_INT
    if (len_written) {
        wake_ARM();
        interrupt_ARM();
    }
#endif /* ENABLE_ARM_HCI_INT */
#endif /* BOD_DELAY_WAKEUP_THRESHOLD */

#ifdef ENABLE_ARM_HCI_INT
    // Wait for ARM to process the data
    dirty = 0;
    do {
        // Attempt to lock the ARM_SDMA_SEMA
        sema_val = MXC_SEMA->semaphores[ARM_SDMA_SEMA];

        // Poll dirty bit.
        if (!sema_val) dirty = ARM_SHARED_MEM->hci.dirty;

        // Release semaphore for dirty bit access.
        if (!sema_val) MXC_SEMA->semaphores[ARM_SDMA_SEMA] = 0;
    } while(dirty == 0);
#endif /* ENABLE_ARM_HCI_INT */

    WsfCsExit();

    ll_dbg_pin_deassert(LL_DBG_PIN_SDMA_WRITE);

    return len_written;
}
