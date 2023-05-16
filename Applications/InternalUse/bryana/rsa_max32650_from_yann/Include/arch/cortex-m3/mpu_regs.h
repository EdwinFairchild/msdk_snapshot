/*
 * mpu_regs.h --
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Maxim Integrated Products
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
 * THIS SOFTWARE IS PROVIDED BY MAXIM INTEGRATED PRODUCTS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MAXIM INTEGRATED PRODUCTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* [INTERNAL] ------------------------------------------------------------------
 * Created on: June 6, 2013
 * Author: Jeremy B. <jeremy.brodt@maximintegrated.com>
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision: 4406 $:  Revision of last commit
 * $Author: jeremy.brodt $:  Author of last commit
 * $Date: 2012-07-06 13:09:03 -0500 (Fri, 06 Jul 2012) $:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */

#ifndef _MPU_REGS_H_
#define _MPU_REGS_H_


/** Definitions ***************************************************************/
#define MPU_TYPE_OFST							0x00
#define MPU_CTRL_OFST							0x04
#define MPU_RNR_OFST							0x08
#define MPU_RBAR_OFST							0x0c
#define MPU_RASR_OFST							0x10

#define MPU_RBAR_A1_OFST						0x14
#define MPU_RASR_A1_OFST						0x18
#define MPU_RBAR_A2_OFST						0x1c
#define MPU_RASR_A2_OFST						0x20
#define MPU_RBAR_A3_OFST						0x24
#define MPU_RASR_A3_OFST						0x28

/** MPU_CTRL */
#define MPU_ENABLE								0x01
#define MPU_HF_ENABLE							0x02
#define MPU_PRIV_ENABLE							0x04

/** MPU_RNR */
#define MPU_RNR 								0xff

/** MPU_RBAR */
#define MPU_RBAR_R								0x0f
#define MPU_RBAR_VALID							0x10

/** MPU_RASR */
#define MPU_RASR_ENABLE							0x01
#define MPU_RASR_SIZE							0x3e
#define MPU_RASR_SIZE_OFST						1
#define MPU_RASR_SRD 							0xff00
#define MPU_RASR_B 								0x10000
#define MPU_RASR_C 								0x20000
#define MPU_RASR_S 								0x40000
#define MPU_RASR_TEX 							0x380000
#define MPU_RASR_AP								0x7000000
#define MPU_RASR_AP_OFST						24
#define MPU_RASR_XN								0x10000000

#ifndef __ASSEMBLER__
/* -------------------------------------------------------------------------- */

/** MPU Registers.
 */
typedef struct
{
	/** 0xe00ed90 */
	unsigned int								type;
	/** 0xe00ed94 */
	unsigned int								ctrl;
	/** 0xe00ed98 */
	unsigned int								rnr;
	/** 0xe00ed9C */
	unsigned int								rbar;
	/** 0xe00eda0 */
	unsigned int								rasr;
	/** 0xe00eda4 */
	unsigned int								rbar_a1;
	/** 0xe00eda8 */
	unsigned int								rasr_a1;
	/** 0xe00edac */
	unsigned int								rbar_a2;
	/** 0xe00edb0 */
	unsigned int								rasr_a2;
	/** 0xe00edb4 */
	unsigned int								rbar_a3;
	/** 0xe00edb8 */
	unsigned int								rasr_a3;

} mpu_regs_t;

#endif /* __ASSEMBLER__ */

#endif /* _MPU_REGS_H_ */

/******************************************************************************/
/* EOF */
