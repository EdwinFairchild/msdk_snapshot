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

#ifndef _SIM_TF_KYPD_H_
#define _SIM_TF_KYPD_H_

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	INCLUDE FILES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/


/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	DEFINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define	SIM_TF_KYPD_CNTRL	(SIM_TF_KYPD_BASE|0x00)
#define	SIM_TF_KYPD_STAT	(SIM_TF_KYPD_BASE|0x04)
#define	SIM_TF_KYPD_DELAY	(SIM_TF_KYPD_BASE|0x08)
#define	SIM_TF_KYPD_DUR		(SIM_TF_KYPD_BASE|0x0C)
#define	SIM_TF_KYPD_PRESCALE	(SIM_TF_KYPD_BASE|0x10)
#define	SIM_TF_KYPD_SIZE	(SIM_TF_KYPD_BASE|0x14)
#define	SIM_TF_KYPD_COUNT	(SIM_TF_KYPD_BASE|0x18)
#define	SIM_TF_KYPD_DATA	(SIM_TF_KYPD_BASE|0x1C)


/*
 * Control Register Fields / Bits
 */

#define	SIM_TF_KYPD_CNTRL_BOUNCE_MASK	0xFFE0
#define	SIM_TF_KYPD_CNTRL_BOUNCEN_MASK	0xF000
#define	SIM_TF_KYPD_CNTRL_BOUNCEN_SHIFT	12
#define	SIM_TF_KYPD_CNTRL_BOUNCED_MASK	0x0FE0
#define	SIM_TF_KYPD_CNTRL_BOUNCED_SHIFT	5
#define	SIM_TF_KYPD_CNTRL_POL		(1<<1)
#define	SIM_TF_KYPD_CNTRL_ST		1

/*
 * Status Register Fields / Bits
 */

#define	SIM_TF_KYPD_STAT_KEY_MASK	0x007C
#define	SIM_TF_KYPD_STAT_KEY_SHIFT	2
#define	SIM_TF_KYPD_STAT_EXC		(1<<1)
#define SIM_TF_KYPD_STAT_ACT		1

/*
 * Size Register Fields
 */

#define	SIM_TF_KYPD_SIZE_ROW_MASK	0x0700
#define	SIM_TF_KYPD_SIZE_ROW_SHIFT	8
#define	SIM_TF_KYPD_SIZE_COL_MASK	0x0007
#define	SIM_TF_KYPD_SIZE_COL_SHIFT	0

/*
 * Count Register Field
 */

#define	SIM_TF_KYPD_COUNT_COUNT_MASK	0x001F
#define	SIM_TF_KYPD_COUNT_COUNT_SHIFT	0

/*
 * Data Register Fields
 */

#define	SIM_TF_KYPD_DATA_ROW_MASK	0x0700
#define	SIM_TF_KYPD_DATA_ROW_SHIFT	8
#define	SIM_TF_KYPD_DATA_COL_MASK	0x0007
#define	SIM_TF_KYPD_DATA_COL_SHIFT	0

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	TYPEDEFS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	MACROS
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#define	SIM_TF_KYPD_CNTRL_BOUNCE(cnt, dur) (((cnt << SIM_TF_KYPD_CNTRL_BOUNCEN_SHIFT) & SIM_TF_KYPD_CNTRL_BOUNCEN_MASK) | \
					    ((dur << SIM_TF_KYPD_CNTRL_BOUNCED_SHIFT) & SIM_TF_KYPD_CNTRL_BOUNCED_MASK))
#define	SIM_TF_KYPD_STAT_KEY(stat)	((stat & SIM_TF_KYPD_STAT_KEY_MASK) >> SIM_TF_KYPD_STAT_KEY_SHIFT)
#define	SIM_TF_KYPD_SIZE_SET(row, col)	(((row << SIM_TF_KYPD_SIZE_ROW_SHIFT) & SIM_TF_KYPD_SIZE_ROW_MASK) | \
					 ((col << SIM_TF_KYPD_SIZE_COL_SHIFT) & SIM_TF_KYPD_SIZE_COL_MASK))
#define	SIM_TF_KYPD_DATA_SET(row, col)	(((row << SIM_TF_KYPD_DATA_ROW_SHIFT) & SIM_TF_KYPD_DATA_ROW_MASK)  | \
					 ((col << SIM_TF_KYPD_DATA_COL_SHIFT) & SIM_TF_KYPD_DATA_COL_MASK))
#define	SIM_TF_KYPD_DATA_GETROW(val)	((val & SIM_TF_KYPD_DATA_ROW_MASK) >> SIM_TF_KYPD_DATA_ROW_SHIFT)
#define	SIM_TF_KYPD_DATA_GETCOL(val)	((val & SIM_TF_KYPD_DATA_COL_MASK) >> SIM_TF_KYPD_DATA_COL_SHIFT)

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	STRUCTURES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

typedef struct SIM_TF_KYPD_BLOCK_S {
    UINT16	Control;
    UINT16	Status;
    UINT16	Delay;
    UINT16	Duration;
    UINT16	Prescale;
    UINT16	Size;
    UINT16	Count;
    UINT16	rsvd[10];
    UINT16	Data[63];
} SIM_TF_KYPD_BLOCK;

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	PROTOTYPES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

#endif
