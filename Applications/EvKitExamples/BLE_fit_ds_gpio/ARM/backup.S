;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ; Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
 ;
 ; Permission is hereby granted, free of charge, to any person obtaining a
 ; copy of this software and associated documentation files (the "Software"),
 ; to deal in the Software without restriction, including without limitation
 ; the rights to use, copy, modify, merge, publish, distribute, sublicense,
 ; and/or sell copies of the Software, and to permit persons to whom the
 ; Software is furnished to do so, subject to the following conditions:
 ;
 ; The above copyright notice and this permission notice shall be included
 ; in all copies or substantial portions of the Software.
 ;
 ; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 ; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 ; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 ; IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 ; OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ; ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 ; OTHER DEALINGS IN THE SOFTWARE.
 ;
 ; Except as contained in this notice, the name of Maxim Integrated
 ; Products, Inc. shall not be used except as stated in the Maxim Integrated
 ; Products, Inc. Branding Policy.
 ;
 ; The mere transfer of this software does not imply any licenses
 ; of trade secrets, proprietary technology, copyrights, patents,
 ; trademarks, maskwork rights, or any other form of intellectual
 ; property whatsoever. Maxim Integrated Products, Inc. retains all
 ; ownership rights.
 ;
 ; $Date: 2020-01-15 14:54:54 -0600 (Wed, 15 Jan 2020) $
 ; $Revision: 50656 $
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

		AREA |.text|, CODE, READONLY

		PRESERVE8
		THUMB
		ALIGN

; Code executed out of backup mode
Backup_Init	PROC
		EXPORT Backup_Init

		; Clear backup state
		LDR	R0, =0x40006800
		LDR	R1, [R0]
		ORR     R2, R1, #0x1
		STR     R2, [R0]

		; Restore the stack pointer
		LDR	R0, =0x4000684C ; MXC_PWRSEQ->gp1
		LDR	SP, [R0]

		; Pop all of the registers
		POP	{R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,LR}

		; Return to where Backup_Setup was called
		BX	LR

    ;.globl   Backup_Setup
    ;.type    Backup_Setup %function
	
		ENDP

; Code executed going into backup mode
Backup_Setup	PROC
		EXPORT Backup_Setup

		; Push all of the registers to the stack
		PUSH	{R0,R1,R2,R3,R4,R5,R6,R7,R8,R9,R10,R11,R12,LR}

		; Save the stack pointer
		LDR	R0, =0x4000684C ; MXC_PWRSEQ->gp1
		STR	SP, [R0]

		; Go to backup
		LDR	R0, =0x4000000C ; MXC_GCR->pm
		LDR	R1, [R0]
		ORR	R2, R1, #0x4 ; Set Backup bit
		STR	R2, [R0]

		; Wait for backup mode, will enter into Backup_Init out of backup mode
		B	. 

		ENDP

		END
