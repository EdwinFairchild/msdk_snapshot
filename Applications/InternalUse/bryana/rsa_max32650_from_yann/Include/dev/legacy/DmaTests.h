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
#ifndef _DMA_TESTS_H_
#define _DMA_TESTS_H_

#define DMA_CTRL_REG		(*(volatile UINT32 *) DMA_CTRL)
#define DMA_ISTA_REG		(*(volatile UINT32 *) DMA_INT_STAT)

#define DMA_XXX_0_REG		((volatile void *) DMA_CH0)
#define DMA_CFG_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_CFG))
#define DMA_STA_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_STAT))
#define DMA_SRC_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_SRC))
#define DMA_DEST_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_DST))
#define DMA_CNT_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_CNT))
#define DMA_SRLD_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_SRC_RLD))
#define DMA_DRLD_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_DST_RLD))
#define DMA_CRLD_0_REG		(*(volatile UINT32 *) (DMA_CH0|DMA_CNT_RLD))

#define DMA_XXX_1_REG		((volatile void *) DMA_CH1)
#define DMA_CFG_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_CFG))
#define DMA_STA_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_STAT))
#define DMA_SRC_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_SRC))
#define DMA_DEST_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_DST))
#define DMA_CNT_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_CNT))
#define DMA_SRLD_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_SRC_RLD))
#define DMA_DRLD_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_DST_RLD))
#define DMA_CRLD_1_REG		(*(volatile UINT32 *) (DMA_CH1|DMA_CNT_RLD))

#define DMA_XXX_2_REG		((volatile void *) DMA_CH2)
#define DMA_CFG_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_CFG))
#define DMA_STA_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_STAT))
#define DMA_SRC_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_SRC))
#define DMA_DEST_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_DST))
#define DMA_CNT_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_CNT))
#define DMA_SRLD_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_SRC_RLD))
#define DMA_DRLD_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_DST_RLD))
#define DMA_CRLD_2_REG		(*(volatile UINT32 *) (DMA_CH2|DMA_CNT_RLD))

#define DMA_XXX_3_REG		((volatile void *) DMA_CH0)
#define DMA_CFG_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_CFG))
#define DMA_STA_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_STAT))
#define DMA_SRC_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_SRC))
#define DMA_DEST_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_DST))
#define DMA_CNT_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_CNT))
#define DMA_SRLD_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_SRC_RLD))
#define DMA_DRLD_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_DST_RLD))
#define DMA_CRLD_3_REG		(*(volatile UINT32 *) (DMA_CH3|DMA_CNT_RLD))

#define DMA_XXX_4_REG		((volatile void *) DMA_CH4)
#define DMA_CFG_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_CFG))
#define DMA_STA_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_STAT))
#define DMA_SRC_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_SRC))
#define DMA_DEST_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_DST))
#define DMA_CNT_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_CNT))
#define DMA_SRLD_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_SRC_RLD))
#define DMA_DRLD_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_DST_RLD))
#define DMA_CRLD_4_REG		(*(volatile UINT32 *) (DMA_CH4|DMA_CNT_RLD))

#define DMA_XXX_5_REG		((volatile void *) DMA_CH5)
#define DMA_CFG_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_CFG))
#define DMA_STA_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_STAT))
#define DMA_SRC_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_SRC))
#define DMA_DEST_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_DST))
#define DMA_CNT_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_CNT))
#define DMA_SRLD_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_SRC_RLD))
#define DMA_DRLD_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_DST_RLD))
#define DMA_CRLD_5_REG		(*(volatile UINT32 *) (DMA_CH5|DMA_CNT_RLD))

#define DMA_XXX_6_REG		((volatile void *) DMA_CH6)
#define DMA_CFG_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_CFG))
#define DMA_STA_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_STAT))
#define DMA_SRC_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_SRC))
#define DMA_DEST_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_DST))
#define DMA_CNT_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_CNT))
#define DMA_SRLD_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_SRC_RLD))
#define DMA_DRLD_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_DST_RLD))
#define DMA_CRLD_6_REG		(*(volatile UINT32 *) (DMA_CH6|DMA_CNT_RLD))

#define DMA_XXX_7_REG		((volatile void *) DMA_CH7)
#define DMA_CFG_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_CFG))
#define DMA_STA_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_STAT))
#define DMA_SRC_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_SRC))
#define DMA_DEST_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_DST))
#define DMA_CNT_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_CNT))
#define DMA_SRLD_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_SRC_RLD))
#define DMA_DRLD_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_DST_RLD))
#define DMA_CRLD_7_REG		(*(volatile UINT32 *) (DMA_CH7|DMA_CNT_RLD))

#endif
