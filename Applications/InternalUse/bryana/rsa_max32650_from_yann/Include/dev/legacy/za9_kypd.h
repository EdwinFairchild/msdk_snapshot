/*
 * Copyright 2006, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */
#ifndef _ZA9_KEYP_H_
#define _ZA9_KEYP_H_

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       INCLUDE FILES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 */
#include <bsp_types.h>
/*
 * KEYP Register Offsets
 */
#define KEYP_CR0	(KEYP_BASE | 0x000)
#define KEYP_CR1	(KEYP_BASE | 0x004)
#define KEYP_SR	    (KEYP_BASE | 0x008)
#define KEYP_IER	(KEYP_BASE | 0x00C)
#define KEYP_ISR	(KEYP_BASE | 0x010)
#define KEYP_KEY0	(KEYP_BASE | 0x014)
#define KEYP_KEY1	(KEYP_BASE | 0x018)
#define KEYP_KEY2	(KEYP_BASE | 0x01C)
#define KEYP_KEY3   (KEYP_BASE | 0x020)
#define KEYP_GPIO0	(KEYP_BASE | 0x024)
#define KEYP_GPIO1  (KEYP_BASE | 0x028)
#define KYPD_STAT_KEYROW_SHIFT	5
#define KYPD_STAT_KEYCOL_SHIFT	0

#define	KYPD_STAT_OVERIS		BIT1
#define	KYPD_STAT_INT		BIT1
typedef struct KEYP_HW_BLOCK_S {
	UINT32		KEYP_ControlReg0;
	UINT32		KEYP_ControlReg1;
	UINT32		KEYP_StatusReg;
	UINT32		KEYP_InterruptEnReg;
	UINT32		KEYP_InterruptStReg;
	UINT32		KEYP_BaudRtReg;
	UINT32		KEYP_RxCntReg;
	UINT32		KEYP_TxFifoOutReg;
	UINT32		KEYP_DataReg;
	UINT32		KEYP_ReservedReg0;
	UINT32		KEYP_ReservedReg1;
	UINT32		KEYP_ReservedReg2;

} KEYP_HW_BLOCK;


#endif 

