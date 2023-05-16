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
 * $Id: ipc_def.h 41757 2019-03-20 14:32:52Z kevin.gillespie $
 *
 *******************************************************************************
 */



#define SDMA_GPIO_PORT      MXC_GPIO1
#define SDMA_GPIO_PIN0      (0x1 << 15)
#define SDMA_GPIO_PIN       (SDMA_GPIO_PIN0)

// #define IPC_LHCI        1
#define ARM_SEMA        0
#define SDMA_SEMA       1

// 2 bytes for dirty and length
#define SHARED_MEM_LEN      (0x200-3)

#ifdef __CROSSWORKS
typedef __packed struct {
#else
typedef struct __attribute__((packed)) {
#endif
    volatile uint8_t dirty;
    volatile uint16_t len;
    volatile uint8_t data[SHARED_MEM_LEN];
} shared_mem_t;

#define ARM_SHARED_MEM_BASE             ((uint32_t)0x2003FC00UL)
#define ARM_SHARED_MEM                  ((shared_mem_t*)ARM_SHARED_MEM_BASE)

#define SDMA_SHARED_MEM_BASE            ((uint32_t)0x2003FE00UL)
#define SDMA_SHARED_MEM                 ((shared_mem_t*)SDMA_SHARED_MEM_BASE)
