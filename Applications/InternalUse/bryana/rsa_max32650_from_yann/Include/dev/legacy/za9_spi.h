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
#ifndef _ZA9_SPI_H_
#define _ZA9_SPI_H_

//#include <bsp_types.h>

/*
 * SPI Register Offsets
 */
#define SPI_DATA            	0x000
#define SPI_CTRL            	0x004
#define SPI_STAT        	0x008
#define SPI_MOD        		0x00C
#define SPI_DIAG            	0x010
#define SPI_BRG        		0x014
#define SPI_DMA		        0x018
#define SPI_I2S		        0x01C

/*
 * SPI_CTL_REG bit definitions
 */
#define CTL_IRQE            	0x80
#define CTL_STR                 0x40
#define CTL_BIRQ                0x20
#define CTL_PHASE               0x10
#define CTL_CLKPOL          	0x08
#define CTL_WOR             	0x04
#define CTL_MMEN                0x02
#define CTL_SPIEN               0x01

/*
 * SPI_STA_REG bit definitions
 */
#define STA_IRQ             	0x80
#define STA_TOVR            	0x40
#define STA_COL             	0x20
#define STA_ABT             	0x10
#define STA_ROVR            	0x08
#define STA_TUND            	0x04
#define STA_TXST                0x02
#define STA_SLAS                0x01

/*
 * SPI_MOD_REG bit definitions
 */
#define MOD_DIAG              0x40
#define MOD_NUMBITS         	0x3C
#define MOD_NUMBITS_1      	0x04
#define MOD_NUMBITS_2      	0x08
#define MOD_NUMBITS_3      	0x0C
#define MOD_NUMBITS_4      	0x10
#define MOD_NUMBITS_5      	0x14
#define MOD_NUMBITS_6      	0x18
#define MOD_NUMBITS_7      	0x1C
#define MOD_NUMBITS_8      	0x20
#define MOD_NUMBITS_9      	0x24
#define MOD_NUMBITS_10      	0x28
#define MOD_NUMBITS_11      	0x2C
#define MOD_NUMBITS_12      	0x30
#define MOD_NUMBITS_13      	0x34
#define MOD_NUMBITS_14      	0x38
#define MOD_NUMBITS_15      	0x3C
#define MOD_NUMBITS_16      	0x00
#define MOD_SSIO              0x02
#define MOD_SSIO_OUTPUT       0x02
#define MOD_SSIO_INPUT        0x00
#define MOD_SSV             	0x01
#define MOD_SSV0             	0x00
#define MOD_SSV1            	0x01

/*
 * SPI_DIAG_REG bit definitions
 */
#define DIAG_SCKEN          	0x80
#define DIAG_TCKEN          	0x40
#define DIAG_SPISTATE       	0x3F

/*
 * SPI_DMA_REG bit definitions
 */
#define RX_DMA_EN               BIT31
#define RX_FIFO_CNT         	(BIT26 | BIT25 | BIT24)
#define RX_FIFO_CLR         	BIT20
#define RX_FIFO_LEVEL       	(BIT17 | BIT16)
#define TX_DMA_EN               BIT15
#define TX_FIFO_CNT         	(BIT10 | BIT9 | BIT8)
#define TX_FIFO_CLR         	BIT4
#define TX_FIFO_LEVEL       	(BIT1 | BIT0)
#define TX_FIFO_CNT_SHIFT   	8
#define RX_FIFO_CNT_SHIFT   	24
#define MAX_TX_CNT          	0x04
 
/*
 * SPI_I2S_REG bit definitions
 */
#define I2S_LJ              	0x10
#define I2S_MONO            	0x08
#define I2S_PAUSE           	0x04
#define I2S_MUTE            	0x02
#define I2S_EN              	0x01

/*
  SPI0 Register Definitions



#define SPI0_DAT_REG        (SPI0_BASE | SPI_DATA )
#define SPI0_CTL_REG        (SPI0_BASE | SPI_CTRL )
#define SPI0_STA_REG        (SPI0_BASE | SPI_STAT )
#define SPI0_MOD_REG        (SPI0_BASE | SPI_MOD )
#define SPI0_DIAG_REG       (SPI0_BASE | SPI_DIAG )
#define SPI0_BRG_REG        (SPI0_BASE | SPI_BRG )
#define SPI0_DMA_REG        (SPI0_BASE | SPI_DMA )
#define SPI0_I2S_REG        (SPI0_BASE | SPI_I2S )




  SPI1 Register Definitions

#define SPI1_DAT_REG         (SPI1_BASE | SPI_DATA )
#define SPI1_CTL_REG         (SPI1_BASE | SPI_CTRL )
#define SPI1_STA_REG         (SPI1_BASE | SPI_STAT )
#define SPI1_MOD_REG         (SPI1_BASE | SPI_MOD  )
#define SPI1_DIAG_REG        (SPI1_BASE | SPI_DIAG )
#define SPI1_BRG_REG         (SPI1_BASE | SPI_BRG  )
#define SPI1_DMA_REG         (SPI1_BASE | SPI_DMA  )
#define SPI1_I2S_REG         (SPI1_BASE | SPI_I2S  )
*/

#define SPI0_DAT_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_DATA ))
#define SPI0_CTL_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_CTRL ))
#define SPI0_STA_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_STAT ))
#define SPI0_MOD_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_MOD ))
#define SPI0_DIAG_REG       (*(volatile UINT32 *) (SPI0_BASE | SPI_DIAG ))
#define SPI0_BRG_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_BRG ))
#define SPI0_DMA_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_DMA ))
#define SPI0_I2S_REG        (*(volatile UINT32 *) (SPI0_BASE | SPI_I2S ))

#define SPI1_DAT_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_DATA ))
#define SPI1_CTL_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_CTRL ))
#define SPI1_STA_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_STAT ))
#define SPI1_MOD_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_MOD ))
#define SPI1_DIAG_REG       (*(volatile UINT32 *) (SPI1_BASE | SPI_DIAG ))
#define SPI1_BRG_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_BRG ))
#define SPI1_DMA_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_DMA ))
#define SPI1_I2S_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_I2S ))

/*
Never use as pointer 
#define SPI1_DAT_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_DATA ))
#define SPI1_CTL_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_CTRL ))
#define SPI1_STA_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_STAT ))
#define SPI1_MOD_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_MOD ))
#define SPI1_DIAG_REG       (*(volatile UINT32 *) (SPI1_BASE | SPI_DIAG ))
#define SPI1_BRG_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_BRG ))
#define SPI1_DMA_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_DMA ))
#define SPI1_I2S_REG        (*(volatile UINT32 *) (SPI1_BASE | SPI_I2S ))

*/


typedef volatile struct SPI_HW_BLOCK_S {
	UINT32		Data;
	UINT32		Control;
	UINT32		Status;
	UINT32		Mode;
	UINT32		Diagnostic;
	UINT32		BaudRate;
	UINT32		Dma;
	UINT32		I2sModeControl;

} SPI_HW_BLOCK;



#define  REG32(ADDR)        (*(volatile UINT32 *)ADDR)
UINT32 i;
UINT32 rxtmp;
volatile UINT32 DefaultEvents = 0;
volatile UINT32 SPI0Events = 0;
volatile UINT32 SPI1Events = 0;


void Spi0FifoClear();
void Spi1FifoClear();
void Spi0FifoOverflow();
void Spi1FifoOverflow();
void cfg_spi0_master();
void cfg_spi1_master();
void sendspi0_m_data(UINT32 * data);
void sendspi1_m_data(UINT32 * data);
void check_spi0_rx_data(UINT32 * data);
void check_spi1_rx_data(UINT32 * data);
#endif 

