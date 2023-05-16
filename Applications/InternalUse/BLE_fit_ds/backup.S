/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-11-27 15:29:47 -0600 (Wed, 27 Nov 2019) $ 
 * $Revision: 49250 $
 *
 ******************************************************************************/

    .syntax unified
    .arch armv7-m
    .text   
    .thumb
    .thumb_func
    .align 2


    .globl   Backup_Init
    .type    Backup_Init %function
/* Code executed out of backup mode */
Backup_Init:

    /* Clear backup state */
    ldr     r0, =0x40006800
    ldr     r1, [r0]
    orr     r2, r1, #0x1
    str     r2, [r0]

    /* Restore the stack pointer */
    ldr     r0, =0x4000684C /* MXC_PWRSEQ->gp1 */
    ldr     sp, [r0]

    /* Pop all of the registers */
    pop     {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}

    /* Return to where Backup_Setup was called */
    bx      lr



    .globl   Backup_Setup
    .type    Backup_Setup %function
/* Code executed going into backup mode */
Backup_Setup:

    nop

    /* Push all of the registers to the stack */
    push    {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}

    /* Save the stack pointer */
    ldr     r0, =0x4000684C /* MXC_PWRSEQ->gp1 */
    str     sp, [r0]

    /* Go to backup */
    ldr     r0, =0x4000000C /* MXC_GCR->pm */
    ldr     r1, [r0]
    orr     r2, r1, #0x4    /* Set Backup bit */
    str     r2, [r0]

    /* Wait for backup mode, will enter into Backup_Init out of backup mode */
    b       . 
