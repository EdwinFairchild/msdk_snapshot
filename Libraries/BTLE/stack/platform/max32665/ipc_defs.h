/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Id: ipc_defs.h 58925 2021-08-03 15:00:38Z nathan.goldstick $
 *
 *******************************************************************************
 */

#ifndef IPC_DEFS_H
#define IPC_DEFS_H

#define ARM_SDMA_SEMA           0

#define SDMA_FLAG_INIT          0       /* SDMA core is starting */
#define SDMA_FLAG_RUNNING       1       /* SDMA is running */
#define SDMA_FLAG_SLEEPING      2       /* SDMA is ready to sleep */
#define SDMA_FLAG_SHUTDOWN      3       /* SDMA shutdown */
#define SDMA_FLAG_ENOMEM        4       /* SDMA error (ran out of memory) */
#define SDMA_FLAG_EINVALIDIP    5       /* SDMA error (invalid instruction pointer) */
#define SDMA_FLAG_STATE         0x7FUL  /* SDMA state mask */
#define SDMA_FLAG_CHANGED       0x80UL  /* Toggled to be able to signal null state transitions. */

#define ARM_FLAG_INIT           0       /* SDMA will not sleep */
#define ARM_FLAG_SLEEP          1       /* SDMA will sleep, must be reset by host */
#define ARM_FLAG_STATE          0x7FUL  /* ARM state mask. */
#define ARM_FLAG_CHANGED        0x80UL  /* Toggled to be able to signal null state transitions. */

/* ARM writes to ARM_FLAG_POS, SDMA writes to SDMA_FLAG_POS */
#define ARM_FLAG_POS            0
#define SDMA_FLAG_POS           1

// Memory sizes and locations allocated in the linker file
// 3 bytes for dirty and length
#define SHARED_MEM_LEN          (0x400 >> 1)
#define SHARED_MEM_DATA_LEN     (SHARED_MEM_LEN - \
				 (sizeof(shared_state_t) + sizeof(uint8_t) + sizeof(uint16_t)))

#ifdef __CROSSWORKS
typedef __packed struct
#else
typedef struct __attribute__((packed))
#endif
{
    volatile uint8_t fsm;
    volatile uint8_t reserved0[3];
    volatile uint32_t reserved1;
} arm_state_t;

#ifdef __CROSSWORKS
typedef __packed struct
#else
typedef struct __attribute__((packed))
#endif
{
    volatile uint8_t fsm;
    uint8_t reserved0[3];
    volatile uint32_t wakeupTime;
} sdma_state_t;

typedef union {
    arm_state_t arm;
    sdma_state_t sdma;
} shared_state_t;

#ifdef __CROSSWORKS
typedef __packed struct
#else
typedef struct __attribute__((packed))
#endif
{
    volatile uint8_t dirty;
    volatile uint16_t len;
    volatile uint8_t data[SHARED_MEM_DATA_LEN];
} shared_hci_t;

typedef struct
{
    shared_state_t state;
    shared_hci_t hci;
} shared_mem_t;

// Shared addr defined in Makefile
#define OTP_LEN  				0x40
#ifdef __CROSSWORKS
#define ARM_SHARED_MEM_BASE     ((uint32_t)SHARED_ADDR)
#define ARM_SHARED_MEM          ((shared_mem_t*)ARM_SHARED_MEM_BASE)

#define SDMA_SHARED_MEM_BASE    ((uint32_t)(SHARED_ADDR+SHARED_MEM_LEN))
#define SDMA_SHARED_MEM         ((shared_mem_t*)SDMA_SHARED_MEM_BASE)

#define SDMA_OTP_MEM            ((uint8_t*)OTP_ADDR)

#endif

#endif /* IPC_DEFS_H */
