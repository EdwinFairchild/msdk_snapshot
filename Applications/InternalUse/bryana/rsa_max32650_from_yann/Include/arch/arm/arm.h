/*
 * arm.h --
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Maxim Integrated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MAXIM INTEGRATED ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* [INTERNAL] ------------------------------------------------------------------
 * Created on: May 25, 2010
 * Author:
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision:: 1112     $:  Revision of last commit
 * $Author:: gregory.ro#$:  Author of last commit
 * $Date:: 2011-02-10 1#$:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */
#ifndef ARM_H_
#define ARM_H_

#define Mode_USR        0x10 /**< User execution mode */
#define Mode_FIQ        0x11 /**< FIQ execution mode */
#define Mode_IRQ        0x12 /**< IRQ execution mode */
#define Mode_SVC        0x13 /**< Supervisor execution mode */
#define Mode_ABT        0x17 /**< Abort execution mode */

#define I_Bit           0x80 /**< Disables IRQ interrupts*/
#define F_Bit           0x40 /**< Disables FIQ interrupts*/

/*
 * Exception Table addresses
 */
#define RESET_ADDR		0x00
#define UNDEFINED_ADDR	0x04
#define SWI_ADDR		0x08
#define PREFETCH_ADDR	0x0C
#define ABORT_ADDR		0x10
#define RESERVED_ADDR	0x14
#define IRQ_ADDR		0x18
#define FIQ_ADDR		0x1C

#if !__ASSEMBLER__

/** Read CPSR.
 *
 * Get the Current Program Status Register.
 *
 * @return CPSR
 */
static inline unsigned int arm_get_cpsr() {
	unsigned int cpsr;
	asm volatile ("mrs %0, CPSR" :"=r"(cpsr));
	return cpsr;
}

#endif

#endif /* ARM_H_ */
