/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI driver implementation file.
 *
 *  $Date: 2019-07-29 13:34:27 -0500 (Mon, 29 Jul 2019) $
 *  $Revision: 45080 $
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

/**************************************************************************************************
  Macros
**************************************************************************************************/

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
void INT_IN_IRQHandler(void) __interrupt
{
    uint32_t sema_val = 1;

    while(sema_val) {
        // Attempt to lock the SDMA_SEMA
        // Reading the register does an atomic test and set, returns previous value
        sema_val = MXC_SEMA->semaphores[SDMA_SEMA];
    }

    // Clear ARM interrupt
    MXC_SDMA->int_in_flag = 1;

    if(!SDMA_SHARED_MEM->dirty) {
        chciTrSerialRxIncoming((uint8_t*)SDMA_SHARED_MEM->data, SDMA_SHARED_MEM->len);
        SDMA_SHARED_MEM->dirty = 1;
    }

    // Unlock the SDMA_SEMA
    MXC_SEMA->semaphores[SDMA_SEMA] = 0x0;
}

/*************************************************************************************************/
/*!
 *  \fn     lhciDrvInit
 *
 *  \brief  Initialize the driver.
 */
/*************************************************************************************************/
void chciDrvInit(void)
{
    // Initialize the SDMA shared memory segment
    memset((void*)SDMA_SHARED_MEM->data, 0, SHARED_MEM_DATA_LEN);
    SDMA_SHARED_MEM->dirty = 1;
    SDMA_SHARED_MEM->len = 0;

    // Enable interrupt from ARM
    MXC_SDMA->int_in_ie = 1;
}

/*************************************************************************************************/
/*!
 *  \brief  Write data the driver.
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

    // Convert CHCI to HCI
    uint8_t hciType = (type == CHCI_TR_TYPE_EVT) ? HCI_EVT_TYPE : HCI_ACL_TYPE;

    // Add 1 byte for type
    len++;

    __disable_irq();

    // Attempt to lock the ARM_SEMA
    // Reading the register does an atomic test and set, returns previous value
    sema_val = MXC_SEMA->semaphores[ARM_SEMA];

    // Write the data to the shared memory segment
    if(!sema_val && ARM_SHARED_MEM->dirty) {

        if(len > SHARED_MEM_LEN) {
            len_written = SHARED_MEM_LEN;
        } else {
            len_written = len;
        }

        memcpy((void*)ARM_SHARED_MEM->data,(void*)&hciType, 1);
        memcpy((void*)&ARM_SHARED_MEM->data[1], (void*)pData, len_written-1);
        ARM_SHARED_MEM->len = len_written;
        ARM_SHARED_MEM->dirty = 0;
    }

    // Interrupt the ARM to signal there is data to process
    if(len_written) {
        interrupt_ARM();
    }

    // Unlock the ARM_SEMA
    if(!sema_val) {
        MXC_SEMA->semaphores[ARM_SEMA] = 0x0;
    }

    __enable_irq();

    // Wait for the ARM to read the event
    while(!ARM_SHARED_MEM->dirty) {}

    return len_written;
}
