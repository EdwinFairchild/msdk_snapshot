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
#ifndef _ZA9_DMA_H_
#define _ZA9_DMA_H_

 /*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       INCLUDE FILES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/
//#include <bsp_types.h>


/*
 * DMA registers 
 */
#define DMA_CTRL                 (DMAC_BASE|0x000)
#define DMA_STAT                 (DMAC_BASE|0x004)
#define DMA_IER                  (DMAC_BASE|0x008)
#define DMA_INT_STAT             (DMAC_BASE|0x00C)

#define DMA_ISR                  (DMA_BASE|0x00C)

// Channel 0
#define DMA_W0R0                 (DMAC_BASE|0x0100)
#define DMA_W0R1                 (DMAC_BASE|0x0104)
#define DMA_W0R2                 (DMAC_BASE|0x0108)
#define DMA_W0R3                 (DMAC_BASE|0x010C)
#define DMA_W0R4                 (DMAC_BASE|0x0110)
#define DMA_W0R5                 (DMAC_BASE|0x0114)
#define DMA_W0R6                 (DMAC_BASE|0x0118)
#define DMA_W0R7                 (DMAC_BASE|0x011C)

#define DMA_CH0REG0                  (DMA_BASE|0x100)
#define DMA_CH0REG1                  (DMA_BASE|0x104)
#define DMA_CH0REG2                  (DMA_BASE|0x108)
#define DMA_CH0REG3                  (DMA_BASE|0x110)
#define DMA_CH0REG4                  (DMA_BASE|0x114)
#define DMA_CH0REG5                  (DMA_BASE|0x118)
#define DMA_CH0REG6                  (DMA_BASE|0x11C)
// Channel 1

#define DMA_W1R0                 (DMAC_BASE|0x0120)
#define DMA_W1R1                 (DMAC_BASE|0x0124)
#define DMA_W1R2                 (DMAC_BASE|0x0128)
#define DMA_W1R3                 (DMAC_BASE|0x012C)
#define DMA_W1R4                 (DMAC_BASE|0x0130)
#define DMA_W1R5                 (DMAC_BASE|0x0134)
#define DMA_W1R6                 (DMAC_BASE|0x0138)
#define DMA_W1R7                 (DMAC_BASE|0x013C)

// Channel 2

#define DMA_W2R0                 (DMAC_BASE|0x0140)
#define DMA_W2R1                 (DMAC_BASE|0x0144)
#define DMA_W2R2                 (DMAC_BASE|0x0148)
#define DMA_W2R3                 (DMAC_BASE|0x014C)
#define DMA_W2R4                 (DMAC_BASE|0x0150)
#define DMA_W2R5                 (DMAC_BASE|0x0154)
#define DMA_W2R6                 (DMAC_BASE|0x0158)
#define DMA_W2R7                 (DMAC_BASE|0x015C)

// Channel 3

#define DMA_W3R0                 (DMAC_BASE|0x0160)
#define DMA_W3R1                 (DMAC_BASE|0x0164)
#define DMA_W3R2                 (DMAC_BASE|0x0168)
#define DMA_W3R3                 (DMAC_BASE|0x016C)
#define DMA_W3R4                 (DMAC_BASE|0x0170)
#define DMA_W3R5                 (DMAC_BASE|0x0174)
#define DMA_W3R6                 (DMAC_BASE|0x0178)
#define DMA_W3R7                 (DMAC_BASE|0x017C)

// Channel 4

#define DMA_W4R0                 (DMAC_BASE|0x0180)
#define DMA_W4R1                 (DMAC_BASE|0x0184)
#define DMA_W4R2                 (DMAC_BASE|0x0188)
#define DMA_W4R3                 (DMAC_BASE|0x018C)
#define DMA_W4R4                 (DMAC_BASE|0x0190)
#define DMA_W4R5                 (DMAC_BASE|0x0194)
#define DMA_W4R6                 (DMAC_BASE|0x0198)
#define DMA_W4R7                 (DMAC_BASE|0x019C)

/*

// Channel 5

#define DMA_W5R0                 (DMAC_BASE|0x0160)
#define DMA_W5R1                 (DMAC_BASE|0x0164)
#define DMA_W5R2                 (DMAC_BASE|0x0168)
#define DMA_W5R3                 (DMAC_BASE|0x016C)
#define DMA_W5R4                 (DMAC_BASE|0x0160)
#define DMA_W5R5                 (DMAC_BASE|0x0164)
#define DMA_W5R6                 (DMAC_BASE|0x0168)
#define DMA_W5R7                 (DMAC_BASE|0x016C)



// Channel 6

#define DMA_W6R0                 (DMAC_BASE|0x0170)
#define DMA_W6R1                 (DMAC_BASE|0x0174)
#define DMA_W6R2                 (DMAC_BASE|0x0178)
#define DMA_W6R3                 (DMAC_BASE|0x017C)
#define DMA_W6R4                 (DMAC_BASE|0x0170)
#define DMA_W6R5                 (DMAC_BASE|0x0174)
#define DMA_W6R6                 (DMAC_BASE|0x0178)
#define DMA_W6R7                 (DMAC_BASE|0x017C)

// Channel 7

#define DMA_W7R0                 (DMAC_BASE|0x0180)
#define DMA_W7R1                 (DMAC_BASE|0x0184)
#define DMA_W7R2                 (DMAC_BASE|0x0188)
#define DMA_W7R3                 (DMAC_BASE|0x018C)
#define DMA_W7R4                 (DMAC_BASE|0x0180)
#define DMA_W7R5                 (DMAC_BASE|0x0184)
#define DMA_W7R6                 (DMAC_BASE|0x0188)
#define DMA_W7R7                 (DMAC_BASE|0x018C)

// Channel 8

#define DMA_W8R0                 (DMAC_BASE|0x0190)
#define DMA_W8R1                 (DMAC_BASE|0x0194)
#define DMA_W8R2                 (DMAC_BASE|0x0198)
#define DMA_W8R3                 (DMAC_BASE|0x019C)
#define DMA_W8R4                 (DMAC_BASE|0x0190)
#define DMA_W8R5                 (DMAC_BASE|0x0194)
#define DMA_W8R6                 (DMAC_BASE|0x0198)
#define DMA_W8R7                 (DMAC_BASE|0x019C)

// Channel 9

#define DMA_W9R0                 (DMAC_BASE|0x01A0)
#define DMA_W9R1                 (DMAC_BASE|0x01A4)
#define DMA_W9R2                 (DMAC_BASE|0x01A8)
#define DMA_W9R3                 (DMAC_BASE|0x01AC)
#define DMA_W9R4                 (DMAC_BASE|0x01A0)
#define DMA_W9R5                 (DMAC_BASE|0x01A4)
#define DMA_W9R6                 (DMAC_BASE|0x01A8)
#define DMA_W9R7                 (DMAC_BASE|0x01AC)

// Channel 10

#define DMA_W10R0                 (DMAC_BASE|0x01B0)
#define DMA_W10R1                 (DMAC_BASE|0x01B4)
#define DMA_W10R2                 (DMAC_BASE|0x01B8)
#define DMA_W10R3                 (DMAC_BASE|0x01BC)
#define DMA_W10R4                 (DMAC_BASE|0x01B0)
#define DMA_W10R5                 (DMAC_BASE|0x01B4)
#define DMA_W10R6                 (DMAC_BASE|0x01B8)
#define DMA_W10R7                 (DMAC_BASE|0x01BC)

// Channel 11

#define DMA_W11R0                 (DMAC_BASE|0x01C0)
#define DMA_W11R1                 (DMAC_BASE|0x01C4)
#define DMA_W11R2                 (DMAC_BASE|0x01C8)
#define DMA_W11R3                 (DMAC_BASE|0x01CC)
#define DMA_W11R4                 (DMAC_BASE|0x01C0)
#define DMA_W11R5                 (DMAC_BASE|0x01C4)
#define DMA_W11R6                 (DMAC_BASE|0x01C8)
#define DMA_W11R7                 (DMAC_BASE|0x01CC)

// Channel 12

#define DMA_W12R0                 (DMAC_BASE|0x01D0)
#define DMA_W12R1                 (DMAC_BASE|0x01D4)
#define DMA_W12R2                 (DMAC_BASE|0x01D8)
#define DMA_W12R3                 (DMAC_BASE|0x01DC)
#define DMA_W12R4                 (DMAC_BASE|0x01D0)
#define DMA_W12R5                 (DMAC_BASE|0x01D4)
#define DMA_W12R6                 (DMAC_BASE|0x01D8)
#define DMA_W12R7                 (DMAC_BASE|0x01DC)

// Channel 13

#define DMA_W13R0                 (DMAC_BASE|0x01E0)
#define DMA_W13R1                 (DMAC_BASE|0x01E4)
#define DMA_W13R2                 (DMAC_BASE|0x01E8)
#define DMA_W13R3                 (DMAC_BASE|0x01EC)
#define DMA_W13R4                 (DMAC_BASE|0x01E0)
#define DMA_W13R5                 (DMAC_BASE|0x01E4)
#define DMA_W13R6                 (DMAC_BASE|0x01E8)
#define DMA_W13R7                 (DMAC_BASE|0x01EC)

// Channel 14

#define DMA_W14R0                 (DMAC_BASE|0x01F0)
#define DMA_W14R1                 (DMAC_BASE|0x01F4)
#define DMA_W14R2                 (DMAC_BASE|0x01F8)
#define DMA_W14R3                 (DMAC_BASE|0x01FC)
#define DMA_W14R4                 (DMAC_BASE|0x01F0)
#define DMA_W14R5                 (DMAC_BASE|0x01F4)
#define DMA_W14R6                 (DMAC_BASE|0x01F8)
#define DMA_W14R7                 (DMAC_BASE|0x01FC)

// Channel 15

#define DMA_W15R0                 (DMAC_BASE|0x02F0)
#define DMA_W15R1                 (DMAC_BASE|0x02F4)
#define DMA_W15R2                 (DMAC_BASE|0x02F8)
#define DMA_W15R3                 (DMAC_BASE|0x02FC)
#define DMA_W15R4                 (DMAC_BASE|0x02F0)
#define DMA_W15R5                 (DMAC_BASE|0x02F4)
#define DMA_W15R6                 (DMAC_BASE|0x02F8)
#define DMA_W15R7                 (DMAC_BASE|0x02FC)

*/


/*
 * Base address for configuration of each channel
 */
#define DMA_CH0                  (DMAC_BASE|0x100)
#define DMA_CH1                  (DMAC_BASE|0x120)
#define DMA_CH2                  (DMAC_BASE|0x140)
#define DMA_CH3                  (DMAC_BASE|0x160)
#define DMA_CH4                  (DMAC_BASE|0x180)
#define DMA_CH5                  (DMAC_BASE|0x1A0)
#define DMA_CH6                  (DMAC_BASE|0x1C0)
#define DMA_CH7                  (DMAC_BASE|0x1E0)


/*
 * DMA Register offsets (from DMA_CHx base)
 */
#define DMA_CFG                  0x00
//#define DMA_STAT                 0x04
#define DMA_SRC                  0x08
#define DMA_DST                  0x0C
#define DMA_CNT                  0x10
#define DMA_SRC_RLD              0x14
#define DMA_DST_RLD              0x18
#define DMA_CNT_RLD              0x1C  


/*
 * DMA_CFG_N_REG register bit fields
 */
#define CTZ_IEN                   BIT31
#define EN_STA_IEN                BIT30
#define DST_AUTO_INC              BIT22
#define SRC_AUTO_INC              BIT18
#define DSTWIDTH_FIELD_BYTE       (0<<20)
#define DSTWIDTH_FIELD_HWORD      (1<<20)
#define DSTWIDTH_FIELD_WORD       (2<<20)
#define SRCWIDTH_FIELD_BYTE       (0<<16)
#define SRCWIDTH_FIELD_HWORD      (1<<16)
#define SRCWIDTH_FIELD_WORD       (2<<16)

#define DMA_PS_SEL_MASK           (BIT15 | BIT14)
#define DMA_PS_SEL_DISABLE        (0<<14)
#define DMA_PS_SEL_256            (1<<14)
#define DMA_PS_SEL_64K            (2<<14)
#define DMA_PS_SEL_16M            (3<<14)

#define DMA_TO_SEL_MASK           (BIT13 | BIT12 | BIT11)
#define DMA_TO_SEL_3_4            (0<<11)
#define DMA_TO_SEL_7_8            (1<<11)
#define DMA_TO_SEL_15_16          (2<<11)
#define DMA_TO_SEL_31_32          (3<<11)
#define DMA_TO_SEL_63_64          (4<<11)
#define DMA_TO_SEL_127_128        (5<<11)
#define DMA_TO_SEL_255_256        (6<<11)
#define DMA_TO_SEL_511_512        (7<<11)
                               
#define DMA_REQ_WAIT              BIT10

#define DMA_CH_EN                 BIT0

/*
 * DMA_STA_N_REG register bit fields
 */
#define DMA_STA_TO                BIT6
#define DMA_STA_OVFL              BIT5
#define DMA_STA_BUS_ERR           BIT4
#define DMA_STA_RLOAD             BIT3
#define DMA_STA_CTZ               BIT2
#define DMA_STA_PEND              BIT1
#define DMA_STA_EN                BIT0
                            
/*
 * DMA_CRLD_N_REG reload enable bit
 */
    #define DMA_RLD_EN            BIT31

//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
//<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;       REGISTER VALUES
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

//#define DMA_CONTROL_REG                      (DMAC_BASE|0x00)
    #define DMA_CNTL_DMA_ACK_EARLY               0<<17
    #define DMA_CNTL_DMA_ACK_LATE                1<<17
    #define DMA_CNTL_SIGNALS_HIGH                1<<16
    #define DMA_CNTL_SIGNALS_LOW                 0<<16
    #define DMA_CNTL_INTR_ENABLE_7               1<<7
    #define DMA_CNTL_INTR_ENABLE_6               1<<6
    #define DMA_CNTL_INTR_ENABLE_5               1<<5
    #define DMA_CNTL_INTR_ENABLE_4               1<<4
    #define DMA_CNTL_INTR_ENABLE_3               1<<3
    #define DMA_CNTL_INTR_ENABLE_2               1<<2
    #define DMA_CNTL_INTR_ENABLE_1               1<<1
    #define DMA_CNTL_INTR_ENABLE_0               1<<0
    #define DMA_CNTL_INTR_ENABLE_ALL             0xFF

//#define DMA_INTERRUPT_STATUS_REG             (DMAC_BASE|0x04)
    #define DMA_INTR_STATUS_PENDING_7            1<<7
    #define DMA_INTR_STATUS_PENDING_6            1<<6
    #define DMA_INTR_STATUS_PENDING_5            1<<5
    #define DMA_INTR_STATUS_PENDING_4            1<<4
    #define DMA_INTR_STATUS_PENDING_3            1<<3
    #define DMA_INTR_STATUS_PENDING_2            1<<2
    #define DMA_INTR_STATUS_PENDING_1            1<<1
    #define DMA_INTR_STATUS_PENDING_0            1<<0

#define DMA_CHANNEL_SIZE                     0x20

// --- CHANNEL REGISTERS (offsets from channel start address)

//#define DMA_CHN_CONFIG_REG                   (DMAC_BASE|0x100)
    #define DMA_CCFG_TERM_CNT_INTR_ENB           1<<31 
    #define DMA_CCFG_CHN_DISBL_INTR_ENB          1<<30 
    #define DMA_CCFG_DST_AUTO_INC                1<<22 
    #define DMA_CCFG_DST_WIDTH_8                 0<<20 
    #define DMA_CCFG_DST_WIDTH_16                1<<20 
    #define DMA_CCFG_DST_WIDTH_32                2<<20 
    #define DMA_CCFG_SRC_AUTO_INC                1<<18 
    #define DMA_CCFG_SRC_WIDTH_8                 0<<16 
    #define DMA_CCFG_SRC_WIDTH_16                1<<16 
    #define DMA_CCFG_SRC_WIDTH_32                2<<16 
    #define DMA_CCFG_PRESCALE_DIV_DISABLE        0<<14 
    #define DMA_CCFG_PRESCALE_DIV_256            1<<14 
    #define DMA_CCFG_PRESCALE_DIV_64             2<<14 
    #define DMA_CCFG_PRESCALE_DIV_16             3<<14 
    #define DMA_CCFG_TIME_OUT_4                  0<<11 
    #define DMA_CCFG_TIME_OUT_8                  1<<11 
    #define DMA_CCFG_TIME_OUT_16                 2<<11 
    #define DMA_CCFG_TIME_OUT_32                 3<<11 
    #define DMA_CCFG_TIME_OUT_64                 4<<11 
    #define DMA_CCFG_TIME_OUT_128                5<<11 
    #define DMA_CCFG_TIME_OUT_256                6<<11 
    #define DMA_CCFG_TIME_OUT_512                7<<11 
    #define DMA_CCFG_WAIT_INTR_ENB               1<<10
    #define DMA_CCFG_PERIPH_MEMORY               0<<4
    #define DMA_CCFG_PERIPH_RX_UART0             1<<4
    #define DMA_CCFG_PERIPH_RX_UART1             2<<4
    #define DMA_CCFG_PERIPH_RX_UART2             3<<4
    #define DMA_CCFG_PERIPH_RX_SPI0              4<<4
    #define DMA_CCFG_PERIPH_RX_SMC0              5<<4
    #define DMA_CCFG_PERIPH_RX_DAA               6<<4
    #define DMA_CCFG_PERIPH_RX_EXTERNAL          7<<4
    #define DMA_CCFG_PERIPH_RX_ADC               8<<4
    #define DMA_CCFG_PERIPH_RX_MCR               9<<4
    #define DMA_CCFG_PERIPH_RX_SPI1             10<<4
    #define DMA_CCFG_PERIPH_RX_SPI3             11<<4
    #define DMA_CCFG_PERIPH_RX_SMC1             12<<4
    #define DMA_CCFG_PERIPH_RX_I2C              13<<4
    #define DMA_CCFG_PERIPH_RX_SPI4             14<<4
    #define DMA_CCFG_PERIPH_RX_SPI2             15<<4

    #define DMA_CCFG_PERIPH_TX_UART0            17<<4
    #define DMA_CCFG_PERIPH_TX_UART1            18<<4
    #define DMA_CCFG_PERIPH_TX_UART2            19<<4
    #define DMA_CCFG_PERIPH_TX_SPI0             20<<4
    #define DMA_CCFG_PERIPH_TX_SMC              21<<4
    #define DMA_CCFG_PERIPH_TX_DAA              22<<4
    #define DMA_CCFG_PERIPH_TX_EXTERNAL         23<<4
    #define DMA_CCFG_PERIPH_TX_HASH             24<<4
    #define DMA_CCFG_PERIPH_TX_LCD              25<<4
    #define DMA_CCFG_PERIPH_TX_SPI1             26<<4
    #define DMA_CCFG_PERIPH_TX_SPI3             27<<4
    #define DMA_CCFG_PERIPH_TX_SMC1             28<<4
    #define DMA_CCFG_PERIPH_TX_I2C              29<<4
    #define DMA_CCFG_PERIPH_TX_SPI4             30<<4
    #define DMA_CCFG_PERIPH_TX_SPI2             31<<4

    #define DMA_CCFG_PERIPH_RX_UART3            32<<4
    #define DMA_CCFG_PERIPH_TX_UART3            48<<4

    #define DMA_CCFG_PRIORITY_SHIFT              2
    #define DMA_CCFG_PRIORITY_0                  0<<2
    #define DMA_CCFG_PRIORITY_1                  1<<2
    #define DMA_CCFG_PRIORITY_2                  2<<2
    #define DMA_CCFG_PRIORITY_3                  3<<2
    #define DMA_CCFG_INITIAL_RELOAD_ENABLE       1<<1
    #define DMA_CCFG_ENABLE                      1<<0
    #define DMA_CCFG_DISABLE                     0<<0
    #define DMA_CCFG_BURST_SIZE_SHIFT            24
    #define DMA_CCFG_DST_WIDTH_SHIFT             20
    #define DMA_CCFG_SRC_WIDTH_SHIFT             16


//#define DMA_CHN_STATUS_REG                   (DMAC_BASE|0x104)
    #define DMA_CSTAT_TIMEOUT_OCCURED            1<<6
    #define DMA_CSTAT_OVERFLOW_OCCURED           1<<5
    #define DMA_CSTAT_AHB_ABORT_OCCURED          1<<4
    #define DMA_CSTAT_RELOAD_OCCURED             1<<3
    #define DMA_CSTAT_TERMINAL_CNT_OCCURED       1<<2
    #define DMA_CSTAT_DMA_PENDING                1<<1
    #define DMA_CSTAT_ENABLED                    1<<0
    #define DMA_CSTAT_DISABLED                   0<<0

//#define DMA_CHN_SOURCE_ADDR_REG              (DMAC_BASE|0x108)
//#define DMA_CHN_DESTINATION_ADDR_REG         (DMAC_BASE|0x10C)
//#define DMA_CHN_TERMINAL_CNT_REG             (DMAC_BASE|0x110)
//#define DMA_CHN_RELOAD_SRC_ADDR_REG          (DMAC_BASE|0x114)
//#define DMA_CHN_RELOAD_DEST_ADDR_REG         (DMAC_BASE|0x118)
//#define DMA_CHN_RELOAD_COUNT_REG             (DMAC_BASE|0x11C)
    #define DMA_SRLC_RELOAD_ENABLED              1<<31


/* DMA Controller */
typedef volatile struct DMA_HW_BLOCK_S {
	UINT32		Control;
	UINT32		Status;
	UINT32		Interrupt_en;
	UINT32		Interrupt_st;
}DMA_HW_BLOCK;

typedef volatile struct DMA_HW_BLOCK_CH_S {

	UINT32		CH0REG0;
	UINT32		CH0REG1;
	UINT32		CH0REG2;
	UINT32		CH0REG3;
	UINT32		CH0REG4;
	UINT32		CH0REG5;
	UINT32		CH0REG6;
} DMA_HW_BLOCK_CH;

#endif 

