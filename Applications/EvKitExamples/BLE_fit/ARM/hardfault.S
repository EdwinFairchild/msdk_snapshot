;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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

;*******************************************************************************
; Handle hard fault in a way that makes
; appropriate registers accessible to C routine.
;*******************************************************************************
HardFault_Handler	PROC
		EXPORT HardFault_Handler
		IMPORT HardFault_Decoder

		TST LR, #4
		ITE EQ
		MRSEQ R0, MSP
		MRSNE R0, PSP
		B HardFault_Decoder
		ENDP

		END

;*******************************************************************************
; End of file.
;*******************************************************************************
