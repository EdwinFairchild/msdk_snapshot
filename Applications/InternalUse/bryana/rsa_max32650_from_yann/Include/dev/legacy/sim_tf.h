/*
 * Copyright 2007, ZiLOG Inc.
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

#ifndef _SIM_TF_H_
#define _SIM_TF_H_

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;       INCLUDE FILES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/


// locations of the test fixture base addresses

#define SIM_V_BASE_                    0x70002100
#define SIM_V_INDEX                   0x00000000
#define SIM_V_BASE_PLUS_INDEX         SIM_V_BASE_|SIM_V_INDEX

#define USB_V_ADD1                  SIM_V_BASE_PLUS_INDEX|0x00000000 
#define USB_V_ADD2                  SIM_V_BASE_PLUS_INDEX|0x00000004 
#define SIM_TF_KYPD_BASE            SIM_V_BASE_PLUS_INDEX|0x00000200
#define SIM_TF_MSG_BASE             SIM_V_BASE_PLUS_INDEX|0x0000001C

//#define SYSREG_SIM_MESSAGE_ADDR          SIM_V_BASE_PLUS_INDEX|0x00000020
//#define SYSREG_SIM_CHARACTER_ADDR        SIM_V_BASE_PLUS_INDEX|0x00000024
  
#define SYSREG_SIM_MESSAGE_ADDR           0x70002120
#define SYSREG_SIM_CHARACTER_ADDR         0x70002124


#define SYSREG_SIM_MESSAGE_ADDR_REG         (*(volatile UINT16*)SYSREG_SIM_MESSAGE_ADDR)
#define SYSREG_SIM_CHARACTER_ADDR_REG       (*(volatile UINT16*)SYSREG_SIM_CHARACTER_ADDR)
#endif

