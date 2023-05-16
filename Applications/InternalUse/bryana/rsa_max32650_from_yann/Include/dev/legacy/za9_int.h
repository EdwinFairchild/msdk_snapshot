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
#ifndef _ZA9_INT_H_
#define _ZA9_INT_H_

 /*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       INCLUDE FILES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/
//#include <bsp_types.h>

/*
 * Exception Table addresses
 */
#define RESET_ADDR              0x020
#define UNDEFINED_ADDR          0x024
#define SWI_ADDR                0x028
#define PREFETCH_ADDR           0x02C
#define ABORT_ADDR              0x030
#define RESERVED_ADDR           0x034
#define IRQ_ADDR                0x038
#define FIQ_ADDR                0x03C
                                
/*
 * INTC Registers
 */
#define INTC_EN0                (INTC_BASE | 0x0000)
#define INTC_EN0_SET            (INTC_BASE | 0x0004)
#define INTC_EN0_CLR            (INTC_BASE | 0x0008)
#define INTC_EN1                (INTC_BASE | 0x000C)
#define INTC_EN1_SET            (INTC_BASE | 0x0010)
#define INTC_EN1_CLR            (INTC_BASE | 0x0014)
#define INTC_DFLT_VECTR         (INTC_BASE | 0x0018)
#define INTC_CNTL               (INTC_BASE | 0x001C)
#define INTC_SW_INT0            (INTC_BASE | 0x0020)
#define INTC_SW_INT0_SET        (INTC_BASE | 0x0024)
#define INTC_SW_INT0_CLR        (INTC_BASE | 0x0028)
#define INTC_SW_INT1            (INTC_BASE | 0x002C)
#define INTC_SW_INT1_SET        (INTC_BASE | 0x0030)
#define INTC_SW_INT1_CLR        (INTC_BASE | 0x0034)
#define INTC_INT0_STAT          (INTC_BASE | 0x0038)
#define INTC_INT1_STAT          (INTC_BASE | 0x003C)
#define INTC_RAW0_INT_STAT      (INTC_BASE | 0x0040)
#define INTC_RAW1_INT_STAT      (INTC_BASE | 0x0044)
#define INTC_IRQ_DBG            (INTC_BASE | 0x0048)
#define INTC_FIQ_DBG            (INTC_BASE | 0x004C)
#define INTC_PEP_EN0		(INTC_BASE | 0x0050)
#define INTC_PEP_EN1		(INTC_BASE | 0x0054)
#define INTC_PEP_VEC		(INTC_BASE | 0x0058)

#define INTC_VEC_N              (INTC_BASE | 0x0100)
#define INTC_CFG_N              (INTC_BASE | 0x0200)

#define INTC_IRQ_VEC            (INTC_BASE | 0x0F00)
#define INTC_FIQ_VEC            (INTC_BASE | 0x0F04)
#define INTC_IRQ_END            (INTC_BASE | 0x0F08)
#define INTC_FIQ_END            (INTC_BASE | 0x0F0C)

/*
 * Define the 64 individual INTC_VEC_N and
 * INTC_CFG_N registers
*/
#define INTC_VEC_0              (INTC_BASE | 0x0100)
#define INTC_VEC_1              (INTC_BASE | 0x0104)
#define INTC_VEC_2              (INTC_BASE | 0x0108)
#define INTC_VEC_3              (INTC_BASE | 0x010C)
#define INTC_VEC_4              (INTC_BASE | 0x0110)
#define INTC_VEC_5              (INTC_BASE | 0x0114)
#define INTC_VEC_6              (INTC_BASE | 0x0118)
#define INTC_VEC_7              (INTC_BASE | 0x011C)
#define INTC_VEC_8              (INTC_BASE | 0x0120)
#define INTC_VEC_9              (INTC_BASE | 0x0124)
#define INTC_VEC_10             (INTC_BASE | 0x0128)
#define INTC_VEC_11             (INTC_BASE | 0x012C)
#define INTC_VEC_12             (INTC_BASE | 0x0130)
#define INTC_VEC_13             (INTC_BASE | 0x0134)
#define INTC_VEC_14             (INTC_BASE | 0x0138)
#define INTC_VEC_15             (INTC_BASE | 0x013C)
#define INTC_VEC_16             (INTC_BASE | 0x0140)
#define INTC_VEC_17             (INTC_BASE | 0x0144)
#define INTC_VEC_18             (INTC_BASE | 0x0148)
#define INTC_VEC_19             (INTC_BASE | 0x014C)
#define INTC_VEC_20             (INTC_BASE | 0x0150)
#define INTC_VEC_21             (INTC_BASE | 0x0154)
#define INTC_VEC_22             (INTC_BASE | 0x0158)
#define INTC_VEC_23             (INTC_BASE | 0x015C)
#define INTC_VEC_24             (INTC_BASE | 0x0160)
#define INTC_VEC_25             (INTC_BASE | 0x0164)
#define INTC_VEC_26             (INTC_BASE | 0x0168)
#define INTC_VEC_27             (INTC_BASE | 0x016C)
#define INTC_VEC_28             (INTC_BASE | 0x0170)
#define INTC_VEC_29             (INTC_BASE | 0x0174)
#define INTC_VEC_30             (INTC_BASE | 0x0178)
#define INTC_VEC_31             (INTC_BASE | 0x017C)
#define INTC_VEC_32             (INTC_BASE | 0x0180)
#define INTC_VEC_33             (INTC_BASE | 0x0184)
#define INTC_VEC_34             (INTC_BASE | 0x0188)
#define INTC_VEC_35             (INTC_BASE | 0x018C)
#define INTC_VEC_36             (INTC_BASE | 0x0190)
#define INTC_VEC_37             (INTC_BASE | 0x0194)
#define INTC_VEC_38             (INTC_BASE | 0x0198)
#define INTC_VEC_39             (INTC_BASE | 0x019C)
#define INTC_VEC_40             (INTC_BASE | 0x01A0)
#define INTC_VEC_41             (INTC_BASE | 0x01A4)
#define INTC_VEC_42             (INTC_BASE | 0x01A8)
#define INTC_VEC_43             (INTC_BASE | 0x01AC)
#define INTC_VEC_44             (INTC_BASE | 0x01B0)
#define INTC_VEC_45             (INTC_BASE | 0x01B4)
#define INTC_VEC_46             (INTC_BASE | 0x01B8)
#define INTC_VEC_47             (INTC_BASE | 0x01BC)
#define INTC_VEC_48             (INTC_BASE | 0x01C0)
#define INTC_VEC_49             (INTC_BASE | 0x01C4)
#define INTC_VEC_50             (INTC_BASE | 0x01C8)
#define INTC_VEC_51             (INTC_BASE | 0x01CC)
#define INTC_VEC_52             (INTC_BASE | 0x01D0)
#define INTC_VEC_53             (INTC_BASE | 0x01D4)
#define INTC_VEC_54             (INTC_BASE | 0x01D8)
#define INTC_VEC_55             (INTC_BASE | 0x01DC)
#define INTC_VEC_56             (INTC_BASE | 0x01E0)
#define INTC_VEC_57             (INTC_BASE | 0x01E4)
#define INTC_VEC_58             (INTC_BASE | 0x01E8)
#define INTC_VEC_59             (INTC_BASE | 0x01EC)
#define INTC_VEC_60             (INTC_BASE | 0x01F0)
#define INTC_VEC_61             (INTC_BASE | 0x01F4)
#define INTC_VEC_62             (INTC_BASE | 0x01F8)
#define INTC_VEC_63             (INTC_BASE | 0x01FC)
                                                                           
#define INTC_CFG_0              (INTC_BASE | 0x0200)
#define INTC_CFG_1              (INTC_BASE | 0x0204)
#define INTC_CFG_2              (INTC_BASE | 0x0208)
#define INTC_CFG_3              (INTC_BASE | 0x020C)
#define INTC_CFG_4              (INTC_BASE | 0x0210)
#define INTC_CFG_5              (INTC_BASE | 0x0214)
#define INTC_CFG_6              (INTC_BASE | 0x0218)
#define INTC_CFG_7              (INTC_BASE | 0x021C)
#define INTC_CFG_8              (INTC_BASE | 0x0220)
#define INTC_CFG_9              (INTC_BASE | 0x0224)
#define INTC_CFG_10             (INTC_BASE | 0x0228)
#define INTC_CFG_11             (INTC_BASE | 0x022C)
#define INTC_CFG_12             (INTC_BASE | 0x0230)
#define INTC_CFG_13             (INTC_BASE | 0x0234)
#define INTC_CFG_14             (INTC_BASE | 0x0238)
#define INTC_CFG_15             (INTC_BASE | 0x023C)
#define INTC_CFG_16             (INTC_BASE | 0x0240)
#define INTC_CFG_17             (INTC_BASE | 0x0244)
#define INTC_CFG_18             (INTC_BASE | 0x0248)
#define INTC_CFG_19             (INTC_BASE | 0x024C)
#define INTC_CFG_20             (INTC_BASE | 0x0250)
#define INTC_CFG_21             (INTC_BASE | 0x0254)
#define INTC_CFG_22             (INTC_BASE | 0x0258)
#define INTC_CFG_23             (INTC_BASE | 0x025C)
#define INTC_CFG_24             (INTC_BASE | 0x0260)
#define INTC_CFG_25             (INTC_BASE | 0x0264)
#define INTC_CFG_26             (INTC_BASE | 0x0268)
#define INTC_CFG_27             (INTC_BASE | 0x026C)
#define INTC_CFG_28             (INTC_BASE | 0x0270)
#define INTC_CFG_29             (INTC_BASE | 0x0274)
#define INTC_CFG_30             (INTC_BASE | 0x0278)
#define INTC_CFG_31             (INTC_BASE | 0x027C)
#define INTC_CFG_32             (INTC_BASE | 0x0280)
#define INTC_CFG_33             (INTC_BASE | 0x0284)
#define INTC_CFG_34             (INTC_BASE | 0x0288)
#define INTC_CFG_35             (INTC_BASE | 0x028C)
#define INTC_CFG_36             (INTC_BASE | 0x0290)
#define INTC_CFG_37             (INTC_BASE | 0x0294)
#define INTC_CFG_38             (INTC_BASE | 0x0298)
#define INTC_CFG_39             (INTC_BASE | 0x029C)
#define INTC_CFG_40             (INTC_BASE | 0x02A0)
#define INTC_CFG_41             (INTC_BASE | 0x02A4)
#define INTC_CFG_42             (INTC_BASE | 0x02A8)
#define INTC_CFG_43             (INTC_BASE | 0x02AC)
#define INTC_CFG_44             (INTC_BASE | 0x02B0)
#define INTC_CFG_45             (INTC_BASE | 0x02B4)
#define INTC_CFG_46             (INTC_BASE | 0x02B8)
#define INTC_CFG_47             (INTC_BASE | 0x02BC)
#define INTC_CFG_48             (INTC_BASE | 0x02C0)
#define INTC_CFG_49             (INTC_BASE | 0x02C4)
#define INTC_CFG_50             (INTC_BASE | 0x02C8)
#define INTC_CFG_51             (INTC_BASE | 0x02CC)
#define INTC_CFG_52             (INTC_BASE | 0x02D0)
#define INTC_CFG_53             (INTC_BASE | 0x02D4)
#define INTC_CFG_54             (INTC_BASE | 0x02D8)
#define INTC_CFG_55             (INTC_BASE | 0x02DC)
#define INTC_CFG_56             (INTC_BASE | 0x02E0)
#define INTC_CFG_57             (INTC_BASE | 0x02E4)
#define INTC_CFG_58             (INTC_BASE | 0x02E8)
#define INTC_CFG_59             (INTC_BASE | 0x02EC)
#define INTC_CFG_60             (INTC_BASE | 0x02F0)
#define INTC_CFG_61             (INTC_BASE | 0x02F4)
#define INTC_CFG_62             (INTC_BASE | 0x02F8)
#define INTC_CFG_63             (INTC_BASE | 0x02FC)

/*
 * CFG_N_REG bit definitions
 */
#define INTC_FIRQ_MASK          0x08
#define INTC_FIQ                0x08
#define INTC_IRQ                0x00
#define INTC_PRIO_MASK          0x07
#define INTC_PRIO_0             0x00
#define INTC_PRIO_1             0x01
#define INTC_PRIO_2             0x02
#define INTC_PRIO_3             0x03
#define INTC_PRIO_4             0x04
#define INTC_PRIO_5             0x05
#define INTC_PRIO_6             0x06
#define INTC_PRIO_7             0x07


/*
 * Interrupt Channel Numbers
 */
#define INTNUM_TIMER6           0
#define INTNUM_WDT              1
#define INTNUM_UART0            2
#define INTNUM_TIMER0           3
#define INTNUM_TIMER1           4
#define INTNUM_TIMER2           5
#define INTNUM_TIMER3           6
#define INTNUM_TIMER4           7
#define INTNUM_TIMER5           8
#define INTNUM_SPI3             9
#define INTNUM_SCU0             10
#define INTNUM_SCU1             11
#define INTNUM_GPIO0            12
#define INTNUM_GPIO1            13
#define INTNUM_GPIO2            14
#define INTNUM_GPIO3            15
#define INTNUM_RES1             16
#define INTNUM_RES2             17
#define INTNUM_KEYPAD           18
#define INTNUM_RES3             18
#define INTNUM_MCR              19
#define INTNUM_DAA              20
#define INTNUM_SSD              21
#define INTNUM_SDD              21
#define INTNUM_ADC              22
#define INTNUM_SPI0             23
#define INTNUM_SPI1             24
#define INTNUM_UART1            25
#define INTNUM_UART2            26
#define INTNUM_UART3            27
#define INTNUM_RNG              28
#define INTNUM_RTC              29
#define INTNUM_TIMER7           30
#define INTNUM_TIMER8           31
#define INTNUM_USB0             32
#define INTNUM_USB1             33
#define INTNUM_USB2             34
#define INTNUM_RES4             35
#define INTNUM_RES5             36
#define INTNUM_I2C              37
#define INTNUM_ETH              38
#define INTNUM_RES6             39
#define INTNUM_CRYPT            40
#define INTNUM_SDIO             41
#define INTNUM_MEMC             43
#define INTNUM_PEP              44
#define INTNUM_CLRLCD           45
#define INTNUM_SPI2             46
#define INTNUM_SPI4             47
#define INTNUM_RES8             48
#define INTNUM_RES9             49
#define INTNUM_RES10            50
#define INTNUM_RES11            51
#define INTNUM_RES12            52
#define INTNUM_RES13            53
#define INTNUM_RES14            54
#define INTNUM_RES15            55
#define INTNUM_DMA0             56
#define INTNUM_DMA1             57
#define INTNUM_DMA2             58
#define INTNUM_DMA3             59
#define INTNUM_DMA4             60
#define INTNUM_DMA5             61
#define INTNUM_DMA6             62
#define INTNUM_DMA7             63
                                
/*                              
 * Interrupt controller interrupt sources bit masks
 */
#define INT_MASK_TIMER6           (1<<INTNUM_TIMER6)
#define INT_MASK_WDT              (1<<INTNUM_WDT   )
#define INT_MASK_UART0            (1<<INTNUM_UART0 )
#define INT_MASK_TIMER0           (1<<INTNUM_TIMER0)
#define INT_MASK_TIMER1           (1<<INTNUM_TIMER1)
#define INT_MASK_TIMER2           (1<<INTNUM_TIMER2)
#define INT_MASK_TIMER3           (1<<INTNUM_TIMER3)
#define INT_MASK_TIMER4           (1<<INTNUM_TIMER4)
#define INT_MASK_TIMER5           (1<<INTNUM_TIMER5)
#define INT_MASK_SPI3             (1<<INTNUM_SPI3  )
#define INT_MASK_SCU0             (1<<INTNUM_SCU0  )
#define INT_MASK_SCU1             (1<<INTNUM_SCU1  )
#define INT_MASK_GPIO0            (1<<INTNUM_GPIO0 )
#define INT_MASK_GPIO1            (1<<INTNUM_GPIO1 )
#define INT_MASK_GPIO2            (1<<INTNUM_GPIO2 )
#define INT_MASK_GPIO3            (1<<INTNUM_GPIO3 )
#define INT_MASK_RES1             (1<<INTNUM_RES1  )
#define INT_MASK_RES2             (1<<INTNUM_RES2  )
#define INT_MASK_RES3             (1<<INTNUM_RES3  )
#define INT_MASK_MCR              (1<<INTNUM_MCR   )
#define INT_MASK_DAA              (1<<INTNUM_DAA   )
#define INT_MASK_SSD              (1<<INTNUM_SSD   )
#define INT_MASK_ADC              (1<<INTNUM_ADC   )
#define INT_MASK_SPI0             (1<<INTNUM_SPI0  )
#define INT_MASK_SPI1             (1<<INTNUM_SPI1  )
#define INT_MASK_UART1            (1<<INTNUM_UART1 )
#define INT_MASK_UART2            (1<<INTNUM_UART2 )
#define INT_MASK_UART3            (1<<INTNUM_UART3 )
#define INT_MASK_RNG              (1<<INTNUM_RNG   )
#define INT_MASK_RTC              (1<<INTNUM_RTC   )
#define INT_MASK_TIMER7           (1<<INTNUM_TIMER7)
#define INT_MASK_TIMER8           (1<<INTNUM_TIMER8)
#define INT_MASK_USB0             (1<<(INTNUM_USB0  -32))
#define INT_MASK_USB1             (1<<(INTNUM_USB1  -32))
#define INT_MASK_USB2             (1<<(INTNUM_USB2  -32))
#define INT_MASK_RES4             (1<<(INTNUM_RES4   -32))
#define INT_MASK_RES5             (1<<(INTNUM_RES5   -32))
#define INT_MASK_I2C              (1<<(INTNUM_I2C   -32))
#define INT_MASK_ETH              (1<<(INTNUM_ETH   -32))
#define INT_MASK_RES6             (1<<(INTNUM_RES6   -32))
#define INT_MASK_RES7             (1<<(INTNUM_RES7   -32))
#define INT_MASK_SDIO             (1<<(INTNUM_SDIO  -32))
#define INT_MASK_KEYPAD           (1<<(INTNUM_KEYPAD-32))
#define INT_MASK_MEMC             (1<<(INTNUM_MEMC  -32))
#define INT_MASK_PEP              (1<<(INTNUM_PEP   -32))
#define INT_MASK_CLRLCD           (1<<(INTNUM_CLRLCD-32))
#define INT_MASK_SPI2             (1<<(INTNUM_SPI2  -32))
#define INT_MASK_SPI4             (1<<(INTNUM_SPI4  -32))
#define INT_MASK_RES8             (1<<(INTNUM_RES8   -32))
#define INT_MASK_RES9             (1<<(INTNUM_RES9   -32))
#define INT_MASK_RES10            (1<<(INTNUM_RES10  -32))
#define INT_MASK_RES11            (1<<(INTNUM_RES11  -32))
#define INT_MASK_RES12            (1<<(INTNUM_RES12  -32))
#define INT_MASK_RES13            (1<<(INTNUM_RES13  -32))
#define INT_MASK_RES14            (1<<(INTNUM_RES14  -32))
#define INT_MASK_RES15            (1<<(INTNUM_RES15  -32))
#define INT_MASK_DMA0             (1<<(INTNUM_DMA0  -32))     
#define INT_MASK_DMA1             (1<<(INTNUM_DMA1  -32))     
#define INT_MASK_DMA2             (1<<(INTNUM_DMA2  -32))     
#define INT_MASK_DMA3             (1<<(INTNUM_DMA3  -32))     
#define INT_MASK_DMA4             (1<<(INTNUM_DMA4  -32))
#define INT_MASK_DMA5             (1<<(INTNUM_DMA5  -32))
#define INT_MASK_DMA6             (1<<(INTNUM_DMA6  -32))
#define INT_MASK_DMA7             (1<<(INTNUM_DMA7  -32))
                                       

#define INTC_EN0_REG            (INTC_BASE | 0x0000)
#define INTC_EN0_SET            (INTC_BASE | 0x0004)
#define INTC_EN0_CLR            (INTC_BASE | 0x0008)
#define INTC_EN1                (INTC_BASE | 0x000C)
#define INTC_EN1_SET            (INTC_BASE | 0x0010)
#define INTC_EN1_CLR            (INTC_BASE | 0x0014)
#define INTC_DFLT_VECTR         (INTC_BASE | 0x0018)
#define INTC_CNTL               (INTC_BASE | 0x001C)
#define INTC_SW_INT0            (INTC_BASE | 0x0020)
#define INTC_SW_INT0_SET        (INTC_BASE | 0x0024)
#define INTC_SW_INT0_CLR        (INTC_BASE | 0x0028)
#define INTC_SW_INT1            (INTC_BASE | 0x002C)
#define INTC_SW_INT1_SET        (INTC_BASE | 0x0030)
#define INTC_SW_INT1_CLR        (INTC_BASE | 0x0034)
#define INTC_INT0_STAT          (INTC_BASE | 0x0038)
#define INTC_INT1_STAT          (INTC_BASE | 0x003C)
#define INTC_RAW0_INT_STAT      (INTC_BASE | 0x0040)
#define INTC_RAW1_INT_STAT      (INTC_BASE | 0x0044)
#define INTC_IRQ_DBG            (INTC_BASE | 0x0048)
#define INTC_FIQ_DBG            (INTC_BASE | 0x004C)

typedef volatile struct INTC_HW_BLOCK_S {
    UINT32  Enable0;        /* Interrupt Enable (Low bits)  */
    UINT32  Enable0Set;
    UINT32  Enable0Clr;
    UINT32  Enable1;        /* Interrupt Enable (High bits  */
    UINT32  Enable1Set;
    UINT32  Enable1Clr;
    UINT32  DefaultVector;  /* Default Interrupt Vector */
    UINT32  Control;        /* IRQ Control          */
    UINT32  SWInt0;         /* Software Interupt (Low bits) */
    UINT32  SWInt0Set;
    UINT32  SWInt0Clr;
    UINT32  SWInt1;         /* Software Interupt (High bits)*/
    UINT32  SWInt1Set;
    UINT32  SWInt1Clr;
    UINT32  Status0;        /* Interrupt Status (Low bits)  */
    UINT32  Status1;        /*  (High bits)         */
    UINT32  RawStatus0;     /* Unmasked IRQ Status (Low bts)*/
    UINT32  RawStatus1;     /*  (High bits)         */
    UINT32  IRQDebug;       /* IRQ Processor Debug      */
    UINT32  FIQDebug;       /* FIQ Processor Debug      */
    UINT32  PepEnable0;	    /* Pep enable 0 */
    UINT32  PepEnable1;	    /* Pep enable 1 */
    UINT32  PepVector;	    /* Pep Vector */
    UINT32  rsvd0[41];      /* Unused - Reserved        */
    UINT32  Vector[64];     /* Interrupt Vectors, 0 thru 63 */
    UINT32  Config[64];     /* Interrupt Config,  0 thru 63 */
    UINT32  rsvd1[768];     /* Unused - Reserved        */
    UINT32  IRQVector;      /* IRQ Vector           */
    UINT32  FIQVector;      /* FIQ Vector           */
    UINT32  IRQEnd;         /* IRQ End-of-Interrupt     */
    UINT32  FIQEnd;         /* FIQ End-of-Interrup      */
} INTC_HW_BLOCK;

#endif                                 

                                       
