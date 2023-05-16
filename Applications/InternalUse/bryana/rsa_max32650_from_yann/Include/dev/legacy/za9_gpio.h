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

 /*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       INCLUDE FILES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       DEFINES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/
/*
 * GPIO Registers Offsets
 */

//#include "bsp_types.h"
#define GPIO_EN                             0x00
#define GPIO_OUT_EN                         0x0C
#define GPIO_OUT                            0x18
#define GPIO_OUT_SET                        0x1C
#define GPIO_OUT_CLR                        0x20
#define GPIO_IN                             0x24
#define GPIO_INT_MOD                        0x28
#define GPIO_INT_POL                        0x2C
#define GPIO_INT_EN                         0x34
#define GPIO_INT_EN_SET                     0x38
#define GPIO_INT_EN_CLR                     0x3C
#define GPIO_INT_STAT                       0x40
#define GPIO_INT_CLR                        0x48
#define GPIO_WAKE_EN                        0x4C
#define GPIO_OPEN_DRAIN_EN                  0x58
#define GPIO_INT_DUAL_EDGE                  0x5C
#define GPIO_PAD_CFG_1                      0x60
#define GPIO_PAD_CFG_2                      0x64
#define GPIO_EN1                            0x68


/*
 * GPIO Port 0 Registers
 */
#define GPIO0_EN                            (GPIO0_BASE | GPIO_EN            )
#define GPIO0_OUT_EN                        (GPIO0_BASE | GPIO_OUT_EN        )
#define GPIO0_OUT                           (GPIO0_BASE | GPIO_OUT           )
#define GPIO0_OUT_SET                       (GPIO0_BASE | GPIO_OUT_SET       )
#define GPIO0_OUT_CLR                       (GPIO0_BASE | GPIO_OUT_CLR       )
#define GPIO0_IN                            (GPIO0_BASE | GPIO_IN            )
#define GPIO0_INT_MOD                       (GPIO0_BASE | GPIO_INT_MOD       )
#define GPIO0_INT_POL                       (GPIO0_BASE | GPIO_INT_POL       )
#define GPIO0_INT_EN                        (GPIO0_BASE | GPIO_INT_EN        )
#define GPIO0_INT_EN_SET                    (GPIO0_BASE | GPIO_INT_EN_SET    )
#define GPIO0_INT_EN_CLR                    (GPIO0_BASE | GPIO_INT_EN_CLR    )
#define GPIO0_INT_STAT                      (GPIO0_BASE | GPIO_INT_STAT      )
#define GPIO0_INT_CLR                       (GPIO0_BASE | GPIO_INT_CLR       )
#define GPIO0_WAKE_EN                       (GPIO0_BASE | GPIO_WAKE_EN       )
#define GPIO0_OPEN_DRAIN_EN                 (GPIO0_BASE | GPIO_OPEN_DRAIN_EN )
#define GPIO0_INT_DUAL_EDGE                 (GPIO0_BASE | GPIO_INT_DUAL_EDGE )
#define GPIO0_PAD_CFG_1                     (GPIO0_BASE | GPIO_PAD_CFG_1     )
#define GPIO0_PAD_CFG_2                     (GPIO0_BASE | GPIO_PAD_CFG_2     )    
#define GPIO0_EN1                           (GPIO0_BASE | GPIO_EN1           )
/*
 * GPIO Port 1 Registers
 */
#define GPIO1_EN                            (GPIO1_BASE | GPIO_EN            )
#define GPIO1_OUT_EN                        (GPIO1_BASE | GPIO_OUT_EN        )
#define GPIO1_OUT                           (GPIO1_BASE | GPIO_OUT           )
#define GPIO1_OUT_SET                       (GPIO1_BASE | GPIO_OUT_SET       )
#define GPIO1_OUT_CLR                       (GPIO1_BASE | GPIO_OUT_CLR       )
#define GPIO1_IN                            (GPIO1_BASE | GPIO_IN            )
#define GPIO1_INT_MOD                       (GPIO1_BASE | GPIO_INT_MOD       )
#define GPIO1_INT_POL                       (GPIO1_BASE | GPIO_INT_POL       )
#define GPIO1_INT_EN                        (GPIO1_BASE | GPIO_INT_EN        )
#define GPIO1_INT_EN_SET                    (GPIO1_BASE | GPIO_INT_EN_SET    )
#define GPIO1_INT_EN_CLR                    (GPIO1_BASE | GPIO_INT_EN_CLR    )
#define GPIO1_INT_STAT                      (GPIO1_BASE | GPIO_INT_STAT      )
#define GPIO1_INT_CLR                       (GPIO1_BASE | GPIO_INT_CLR       )
#define GPIO1_WAKE_EN                       (GPIO1_BASE | GPIO_WAKE_EN       )
#define GPIO1_OPEN_DRAIN_EN                 (GPIO1_BASE | GPIO_OPEN_DRAIN_EN )
#define GPIO1_INT_DUAL_EDGE                 (GPIO1_BASE | GPIO_INT_DUAL_EDGE )
#define GPIO1_PAD_CFG_1                     (GPIO1_BASE | GPIO_PAD_CFG_1     )
#define GPIO1_PAD_CFG_2                     (GPIO1_BASE | GPIO_PAD_CFG_2     )    
#define GPIO1_EN1                           (GPIO1_BASE | GPIO_EN1           )
/*
 * GPIO Port 2 Registers
 */
#define GPIO2_EN                            (GPIO2_BASE | GPIO_EN            )
#define GPIO2_OUT_EN                        (GPIO2_BASE | GPIO_OUT_EN        )
#define GPIO2_OUT                           (GPIO2_BASE | GPIO_OUT           )
#define GPIO2_OUT_SET                       (GPIO2_BASE | GPIO_OUT_SET       )
#define GPIO2_OUT_CLR                       (GPIO2_BASE | GPIO_OUT_CLR       )
#define GPIO2_IN                            (GPIO2_BASE | GPIO_IN            )
#define GPIO2_INT_MOD                       (GPIO2_BASE | GPIO_INT_MOD       )
#define GPIO2_INT_POL                       (GPIO2_BASE | GPIO_INT_POL       )
#define GPIO2_INT_EN                        (GPIO2_BASE | GPIO_INT_EN        )
#define GPIO2_INT_EN_SET                    (GPIO2_BASE | GPIO_INT_EN_SET    )
#define GPIO2_INT_EN_CLR                    (GPIO2_BASE | GPIO_INT_EN_CLR    )
#define GPIO2_INT_STAT                      (GPIO2_BASE | GPIO_INT_STAT      )
#define GPIO2_INT_CLR                       (GPIO2_BASE | GPIO_INT_CLR       )
#define GPIO2_WAKE_EN                       (GPIO2_BASE | GPIO_WAKE_EN       )
#define GPIO2_OPEN_DRAIN_EN                 (GPIO2_BASE | GPIO_OPEN_DRAIN_EN )
#define GPIO2_INT_DUAL_EDGE                 (GPIO2_BASE | GPIO_INT_DUAL_EDGE )
#define GPIO2_PAD_CFG_1                     (GPIO2_BASE | GPIO_PAD_CFG_1     )
#define GPIO2_PAD_CFG_2                     (GPIO2_BASE | GPIO_PAD_CFG_2     )    
#define GPIO2_EN1                           (GPIO2_BASE | GPIO_EN1           )
/*
 * GPIO Port 3 Registers
 */
#define GPIO3_EN                            (GPIO3_BASE | GPIO_EN            )
#define GPIO3_OUT_EN                        (GPIO3_BASE | GPIO_OUT_EN        )
#define GPIO3_OUT                           (GPIO3_BASE | GPIO_OUT           )
#define GPIO3_OUT_SET                       (GPIO3_BASE | GPIO_OUT_SET       )
#define GPIO3_OUT_CLR                       (GPIO3_BASE | GPIO_OUT_CLR       )
#define GPIO3_IN                            (GPIO3_BASE | GPIO_IN            )
#define GPIO3_INT_MOD                       (GPIO3_BASE | GPIO_INT_MOD       )
#define GPIO3_INT_POL                       (GPIO3_BASE | GPIO_INT_POL       )
#define GPIO3_INT_EN                        (GPIO3_BASE | GPIO_INT_EN        )
#define GPIO3_INT_EN_SET                    (GPIO3_BASE | GPIO_INT_EN_SET    )
#define GPIO3_INT_EN_CLR                    (GPIO3_BASE | GPIO_INT_EN_CLR    )
#define GPIO3_INT_STAT                      (GPIO3_BASE | GPIO_INT_STAT      )
#define GPIO3_INT_CLR                       (GPIO3_BASE | GPIO_INT_CLR       )
#define GPIO3_WAKE_EN                       (GPIO3_BASE | GPIO_WAKE_EN       )
#define GPIO3_OPEN_DRAIN_EN                 (GPIO3_BASE | GPIO_OPEN_DRAIN_EN )
#define GPIO3_INT_DUAL_EDGE                 (GPIO3_BASE | GPIO_INT_DUAL_EDGE )
#define GPIO3_PAD_CFG_1                     (GPIO3_BASE | GPIO_PAD_CFG_1     )
#define GPIO3_PAD_CFG_2                     (GPIO3_BASE | GPIO_PAD_CFG_2     )    
#define GPIO3_EN1                           (GPIO3_BASE | GPIO_EN1           )
/*
 * GPIO Port 3 Registers
 */
#define GPIO4_EN                            (GPIO4_BASE | GPIO_EN            )
#define GPIO4_OUT_EN                        (GPIO4_BASE | GPIO_OUT_EN        )
#define GPIO4_OUT                           (GPIO4_BASE | GPIO_OUT           )
#define GPIO4_OUT_SET                       (GPIO4_BASE | GPIO_OUT_SET       )
#define GPIO4_OUT_CLR                       (GPIO4_BASE | GPIO_OUT_CLR       )
#define GPIO4_IN                            (GPIO4_BASE | GPIO_IN            )
#define GPIO4_INT_MOD                       (GPIO4_BASE | GPIO_INT_MOD       )
#define GPIO4_INT_POL                       (GPIO4_BASE | GPIO_INT_POL       )
#define GPIO4_INT_EN                        (GPIO4_BASE | GPIO_INT_EN        )
#define GPIO4_INT_EN_SET                    (GPIO4_BASE | GPIO_INT_EN_SET    )
#define GPIO4_INT_EN_CLR                    (GPIO4_BASE | GPIO_INT_EN_CLR    )
#define GPIO4_INT_STAT                      (GPIO4_BASE | GPIO_INT_STAT      )
#define GPIO4_INT_CLR                       (GPIO4_BASE | GPIO_INT_CLR       )
#define GPIO4_WAKE_EN                       (GPIO4_BASE | GPIO_WAKE_EN       )
#define GPIO4_OPEN_DRAIN_EN                 (GPIO4_BASE | GPIO_OPEN_DRAIN_EN )
#define GPIO4_INT_DUAL_EDGE                 (GPIO4_BASE | GPIO_INT_DUAL_EDGE )
#define GPIO4_PAD_CFG_1                     (GPIO4_BASE | GPIO_PAD_CFG_1     )
#define GPIO4_PAD_CFG_2                     (GPIO4_BASE | GPIO_PAD_CFG_2     )    
#define GPIO4_EN1                           (GPIO4_BASE | GPIO_EN1           )

typedef struct GPIO_HW_BLOCK_S {
    UINT32      Enable;                     // 0x00
    UINT32      Reserve1;                   // 0x04
    UINT32      Reserve2;                   // 0x08
    UINT32      OutputEnable;                       // 0x0C
    UINT32      Reserve3;                   // 0x10
    UINT32      Reserve4;                   // 0x14
    UINT32      Out;                        // 0x18
    UINT32      OutSet;                     // 0x1C
    UINT32      OutClear;                   // 0x20
    UINT32      Input;                      // 0x24
    UINT32      InterruptMode;                      // 0x28
    UINT32      InterruptPolarity;                  // 0x2C
    UINT32      Reserve5;                   // 0x30
    UINT32      InterruptEnable;                    // 0x34
    UINT32      InterruptEnableSet;                 // 0x38
    UINT32      InterruptEnableClear;                       // 0x3C
    UINT32      InterruptStatus;                    // 0x40
    UINT32      Reserve6;                   // 0x44
    UINT32      InterruptClear;                     // 0x48
    UINT32      WakeEnable;                 // 0x4C
    UINT32      Reserve7;                   // 0x50
    UINT32      Reserve8;                   // 0x54
    UINT32      OpenDrainEnable;                // 0x58
    UINT32      InterruptDualEdge;                  // 0x5C
    UINT32      Config1;                    // 0x60
    UINT32      Config2;                    // 0x64
    UINT32      Enable1;                    // 0x68

} GPIO_HW_BLOCK;




