/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI driver implementation file.
 *
 *  $Date: 2019-03-20 09:32:52 -0500 (Wed, 20 Mar 2019) $
 *  $Revision: 41757 $
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
#include "gcr_regs.h"
#include "sema_regs.h"
#include "ipc_def.h"

#ifdef IPC_LHCI

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
uint16_t lhciDrvRead(uint16_t len, uint8_t *pData)
{
    uint32_t sema_val;
    uint16_t read_len = 0;

    __disable_irq();

    // Attempt to lock the ARM_SEMA
    // Reading the register does an atomic test and set, returns previous value
    sema_val = MXC_SEMA->semaphores[ARM_SEMA];

    if(!sema_val && !ARM_SHARED_MEM->dirty) {

        // lhciTrSerialRxIncoming((uint8_t*)SDMA_SHARED_MEM->data, SDMA_SHARED_MEM->len);
        memcpy((void*)pData, (void*)ARM_SHARED_MEM->data, ARM_SHARED_MEM->len);
        read_len = ARM_SHARED_MEM->len;
        ARM_SHARED_MEM->dirty = 1;
    }

    // Unlock the SDMA_SEMA
    if(!sema_val) {
        MXC_SEMA->semaphores[ARM_SEMA] = 0x0;
    }

    __enable_irq();

    return read_len;
}

/*************************************************************************************************/
/*!
 *  \fn     lhciDrvInit
 *
 *  \brief  Initialize the driver.
 */
/*************************************************************************************************/
void lhciDrvInit(void)
{
    // Initialize the SDMA shared memory segment
    memset((void*)ARM_SHARED_MEM->data, 0, SHARED_MEM_LEN);
    ARM_SHARED_MEM->dirty = 1;
    ARM_SHARED_MEM->len = 0;
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
uint16_t lhciDrvWrite(uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t len_written = 0;
    uint32_t sema_val;

    // Add 1 byte for type
    len++;

    __disable_irq();

    // Attempt to lock the SDMA_SEMA
    // Reading the register does an atomic test and set, returns previous value
    sema_val = MXC_SEMA->semaphores[SDMA_SEMA];

    // Write the data to the shared memory segment
    if(!sema_val && SDMA_SHARED_MEM->dirty) {


        if(len > SHARED_MEM_LEN) {
            len_written = SHARED_MEM_LEN;
        } else {
            len_written = len;
        }

        memcpy((void*)SDMA_SHARED_MEM->data,(void*)&type, 1);
        memcpy((void*)&SDMA_SHARED_MEM->data[1], (void*)pData, len_written-1);
        SDMA_SHARED_MEM->len = len_written;
        SDMA_SHARED_MEM->dirty = 0;
    }

    // Unlock the SDMA_SEMA
    if(!sema_val) {
        MXC_SEMA->semaphores[SDMA_SEMA] = 0x0;
    }

    __enable_irq();

    return len_written;
}

#endif
