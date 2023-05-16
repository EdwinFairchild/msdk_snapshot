/**
 * @file    test_regs.h
 * @brief   Registers, Bit Masks and Bit Positions for the TEST Peripheral Module.
 */

/* ****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-03-22 14:14:45 -0500 (Thu, 22 Mar 2018) $
 * $Revision: 34154 $
 *
 *************************************************************************** */

#ifndef _TEST_REGS_H_
#define _TEST_REGS_H_

/* **** Includes **** */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @cond
/*
    If types are not defined elsewhere (CMSIS) define them here
*/
#ifndef __IO
#define __IO volatile
#endif
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __R
#define __R  volatile const
#endif
/// @endcond

/* **** Definitions **** */

/**
 * @ingroup     test
 * @defgroup    test_registers Registers
 * @brief       Registers, Bit Masks and Bit Positions for the TEST Peripheral Module.
 * @description register to manipulate in testmode
 */

/**
 * @ingroup test_registers
 * Structure type to access the TEST Registers.
 */
typedef struct {
    __IO uint32_t tm;                   /**< <tt>\b 0x00:<\tt> TEST TM Register */
    __IO uint32_t trcn;                 /**< <tt>\b 0x04:<\tt> TEST TRCN Register */
    __IO uint32_t tclk;                 /**< <tt>\b 0x08:<\tt> TEST TCLK Register */
    __IO uint32_t tmr0;                 /**< <tt>\b 0x0C:<\tt> TEST TMR0 Register */
    __IO uint32_t tmr1;                 /**< <tt>\b 0x10:<\tt> TEST TMR1 Register */
    __IO uint32_t tmr2;                 /**< <tt>\b 0x14:<\tt> TEST TMR2 Register */
    __IO uint32_t tmr3;                 /**< <tt>\b 0x18:<\tt> TEST TMR3 Register */
    __IO uint32_t stcr;                 /**< <tt>\b 0x1C:<\tt> TEST STCR Register */
    __IO uint32_t mbcn;                 /**< <tt>\b 0x20:<\tt> TEST MBCN Register */
    __IO uint32_t mbstat;               /**< <tt>\b 0x24:<\tt> TEST MBSTAT Register */
    __IO uint32_t mbdsel;               /**< <tt>\b 0x28:<\tt> TEST MBDSEL Register */
    __IO uint32_t mbdiag;               /**< <tt>\b 0x2C:<\tt> TEST MBDIAG Register */
} mxc_test_regs_t;

/* Register offsets for module TEST */
/**
 * @ingroup    test_registers
 * @defgroup   TEST_Register_Offsets Register Offsets
 * @brief      TEST Peripheral Register Offsets from the TEST Base Peripheral Address. 
 * @{
 */
 #define MXC_R_TEST_TM                      ((uint32_t)0x00000000UL) /**< Offset from TEST Base Address: <tt> 0x0x000 */ 
 #define MXC_R_TEST_TRCN                    ((uint32_t)0x00000004UL) /**< Offset from TEST Base Address: <tt> 0x0x004 */ 
 #define MXC_R_TEST_TCLK                    ((uint32_t)0x00000008UL) /**< Offset from TEST Base Address: <tt> 0x0x008 */ 
 #define MXC_R_TEST_TMR0                    ((uint32_t)0x0000000CUL) /**< Offset from TEST Base Address: <tt> 0x0x00C */ 
 #define MXC_R_TEST_TMR1                    ((uint32_t)0x00000010UL) /**< Offset from TEST Base Address: <tt> 0x0x010 */ 
 #define MXC_R_TEST_TMR2                    ((uint32_t)0x00000014UL) /**< Offset from TEST Base Address: <tt> 0x0x014 */ 
 #define MXC_R_TEST_TMR3                    ((uint32_t)0x00000018UL) /**< Offset from TEST Base Address: <tt> 0x0x018 */ 
 #define MXC_R_TEST_STCR                    ((uint32_t)0x0000001CUL) /**< Offset from TEST Base Address: <tt> 0x0x01C */ 
 #define MXC_R_TEST_MBCN                    ((uint32_t)0x00000020UL) /**< Offset from TEST Base Address: <tt> 0x0x020 */ 
 #define MXC_R_TEST_MBSTAT                  ((uint32_t)0x00000024UL) /**< Offset from TEST Base Address: <tt> 0x0x024 */ 
 #define MXC_R_TEST_MBDSEL                  ((uint32_t)0x00000028UL) /**< Offset from TEST Base Address: <tt> 0x0x028 */ 
 #define MXC_R_TEST_MBDIAG                  ((uint32_t)0x0000002CUL) /**< Offset from TEST Base Address: <tt> 0x0x02C */ 
/**@} end of group test_registers */

/**
 * @ingroup  test_registers
 * @defgroup TM_Register
 * @brief    TestMode Select
 * @{
 */
 #define MXC_F_TEST_TM_TME_POS                          (0) /**< TM_TME Position */
 #define MXC_F_TEST_TM_TME                              ((uint32_t)(0x1 << MXC_F_TEST_TM_TME_POS)) /**< TM_TME Mask */

 #define MXC_F_TEST_TM_SCAN_POS                         (3) /**< TM_SCAN Position */
 #define MXC_F_TEST_TM_SCAN                             ((uint32_t)(0x1 << MXC_F_TEST_TM_SCAN_POS)) /**< TM_SCAN Mask */

 #define MXC_F_TEST_TM_SCAN_MD_POS                      (16) /**< TM_SCAN_MD Position */
 #define MXC_F_TEST_TM_SCAN_MD                          ((uint32_t)(0x7 << MXC_F_TEST_TM_SCAN_MD_POS)) /**< TM_SCAN_MD Mask */

 #define MXC_F_TEST_TM_SRT_POS                          (5) /**< TM_SRT Position */
 #define MXC_F_TEST_TM_SRT                              ((uint32_t)(0x1 << MXC_F_TEST_TM_SRT_POS)) /**< TM_SRT Mask */

 #define MXC_F_TEST_TM_IDDQ_POS                         (6) /**< TM_IDDQ Position */
 #define MXC_F_TEST_TM_IDDQ                             ((uint32_t)(0x1 << MXC_F_TEST_TM_IDDQ_POS)) /**< TM_IDDQ Mask */

 #define MXC_F_TEST_TM_SFBIST_POS                       (13) /**< TM_SFBIST Position */
 #define MXC_F_TEST_TM_SFBIST                           ((uint32_t)(0x1 << MXC_F_TEST_TM_SFBIST_POS)) /**< TM_SFBIST Mask */

 #define MXC_F_TEST_TM_PKG_CFG16_POS                    (19) /**< TM_PKG_CFG16 Position */
 #define MXC_F_TEST_TM_PKG_CFG16                        ((uint32_t)(0x1 << MXC_F_TEST_TM_PKG_CFG16_POS)) /**< TM_PKG_CFG16 Mask */

 #define MXC_F_TEST_TM_RETLDO_EN_POS                    (28) /**< TM_RETLDO_EN Position */
 #define MXC_F_TEST_TM_RETLDO_EN                        ((uint32_t)(0x1 << MXC_F_TEST_TM_RETLDO_EN_POS)) /**< TM_RETLDO_EN Mask */

 #define MXC_F_TEST_TM_NMI_DIS_POS                      (31) /**< TM_NMI_DIS Position */
 #define MXC_F_TEST_TM_NMI_DIS                          ((uint32_t)(0x1 << MXC_F_TEST_TM_NMI_DIS_POS)) /**< TM_NMI_DIS Mask */

 #define MXC_F_TEST_TM_XTALSEL_POS                      (7) /**< TM_XTALSEL Position */
 #define MXC_F_TEST_TM_XTALSEL                          ((uint32_t)(0x1 << MXC_F_TEST_TM_XTALSEL_POS)) /**< TM_XTALSEL Mask */

 #define MXC_F_TEST_TM_CKT_POS                          (20) /**< TM_CKT Position */
 #define MXC_F_TEST_TM_CKT                              ((uint32_t)(0x1 << MXC_F_TEST_TM_CKT_POS)) /**< TM_CKT Mask */

 #define MXC_F_TEST_TM_DOFF_POS                         (21) /**< TM_DOFF Position */
 #define MXC_F_TEST_TM_DOFF                             ((uint32_t)(0x1 << MXC_F_TEST_TM_DOFF_POS)) /**< TM_DOFF Mask */

 #define MXC_F_TEST_TM_POR_POS                          (14) /**< TM_POR Position */
 #define MXC_F_TEST_TM_POR                              ((uint32_t)(0x1 << MXC_F_TEST_TM_POR_POS)) /**< TM_POR Mask */

 #define MXC_F_TEST_TM_BOR_POS                          (15) /**< TM_BOR Position */
 #define MXC_F_TEST_TM_BOR                              ((uint32_t)(0x1 << MXC_F_TEST_TM_BOR_POS)) /**< TM_BOR Mask */

 #define MXC_F_TEST_TM_LDOOFF_POS                       (9) /**< TM_LDOOFF Position */
 #define MXC_F_TEST_TM_LDOOFF                           ((uint32_t)(0x1 << MXC_F_TEST_TM_LDOOFF_POS)) /**< TM_LDOOFF Mask */

 #define MXC_F_TEST_TM_TTAP_REQ_POS                     (1) /**< TM_TTAP_REQ Position */
 #define MXC_F_TEST_TM_TTAP_REQ                         ((uint32_t)(0x1 << MXC_F_TEST_TM_TTAP_REQ_POS)) /**< TM_TTAP_REQ Mask */

 #define MXC_F_TEST_TM_TTAP_PIF_POS                     (2) /**< TM_TTAP_PIF Position */
 #define MXC_F_TEST_TM_TTAP_PIF                         ((uint32_t)(0x1 << MXC_F_TEST_TM_TTAP_PIF_POS)) /**< TM_TTAP_PIF Mask */

 #define MXC_F_TEST_TM_DCW_POS                          (22) /**< TM_DCW Position */
 #define MXC_F_TEST_TM_DCW                              ((uint32_t)(0x1 << MXC_F_TEST_TM_DCW_POS)) /**< TM_DCW Mask */

 #define MXC_F_TEST_TM_SVM_BG_POS                       (24) /**< TM_SVM_BG Position */
 #define MXC_F_TEST_TM_SVM_BG                           ((uint32_t)(0x1 << MXC_F_TEST_TM_SVM_BG_POS)) /**< TM_SVM_BG Mask */

 #define MXC_F_TEST_TM_SVM_HV_POS                       (25) /**< TM_SVM_HV Position */
 #define MXC_F_TEST_TM_SVM_HV                           ((uint32_t)(0x1 << MXC_F_TEST_TM_SVM_HV_POS)) /**< TM_SVM_HV Mask */

 #define MXC_F_TEST_TM_PWCN_BOD_POS                     (26) /**< TM_PWCN_BOD Position */
 #define MXC_F_TEST_TM_PWCN_BOD                         ((uint32_t)(0x1 << MXC_F_TEST_TM_PWCN_BOD_POS)) /**< TM_PWCN_BOD Mask */

 #define MXC_F_TEST_TM_VMAIN_MON_PD_POS                 (27) /**< TM_VMAIN_MON_PD Position */
 #define MXC_F_TEST_TM_VMAIN_MON_PD                     ((uint32_t)(0x1 << MXC_F_TEST_TM_VMAIN_MON_PD_POS)) /**< TM_VMAIN_MON_PD Mask */

 #define MXC_F_TEST_TM_SCANFB_POS                       (4) /**< TM_SCANFB Position */
 #define MXC_F_TEST_TM_SCANFB                           ((uint32_t)(0x1 << MXC_F_TEST_TM_SCANFB_POS)) /**< TM_SCANFB Mask */

/**@} end of group TM_Register */

/**
 * @ingroup  test_registers
 * @defgroup TRCN_Register
 * @brief    trim mode register, only i2cl trim bits are used
 * @{
 */
 #define MXC_F_TEST_TRCN_I2C0_POS                       (17) /**< TRCN_I2C0 Position */
 #define MXC_F_TEST_TRCN_I2C0                           ((uint32_t)(0x1 << MXC_F_TEST_TRCN_I2C0_POS)) /**< TRCN_I2C0 Mask */

 #define MXC_F_TEST_TRCN_I2C1_POS                       (22) /**< TRCN_I2C1 Position */
 #define MXC_F_TEST_TRCN_I2C1                           ((uint32_t)(0x1 << MXC_F_TEST_TRCN_I2C1_POS)) /**< TRCN_I2C1 Mask */

/**@} end of group TRCN_Register */

/**
 * @ingroup  test_registers
 * @defgroup TCLK_Register
 * @brief    tclk and event
 * @{
 */
 #define MXC_F_TEST_TCLK_VCLKSRC_POS                    (0) /**< TCLK_VCLKSRC Position */
 #define MXC_F_TEST_TCLK_VCLKSRC                        ((uint32_t)(0xFF << MXC_F_TEST_TCLK_VCLKSRC_POS)) /**< TCLK_VCLKSRC Mask */

 #define MXC_F_TEST_TCLK_VCLKDIV_POS                    (8) /**< TCLK_VCLKDIV Position */
 #define MXC_F_TEST_TCLK_VCLKDIV                        ((uint32_t)(0xF << MXC_F_TEST_TCLK_VCLKDIV_POS)) /**< TCLK_VCLKDIV Mask */

 #define MXC_F_TEST_TCLK_EVENTSRC_POS                   (16) /**< TCLK_EVENTSRC Position */
 #define MXC_F_TEST_TCLK_EVENTSRC                       ((uint32_t)(0xFF << MXC_F_TEST_TCLK_EVENTSRC_POS)) /**< TCLK_EVENTSRC Mask */

 #define MXC_F_TEST_TCLK_EVENTDIV_POS                   (24) /**< TCLK_EVENTDIV Position */
 #define MXC_F_TEST_TCLK_EVENTDIV                       ((uint32_t)(0xF << MXC_F_TEST_TCLK_EVENTDIV_POS)) /**< TCLK_EVENTDIV Mask */

 #define MXC_F_TEST_TCLK_VCLKEN_POS                     (15) /**< TCLK_VCLKEN Position */
 #define MXC_F_TEST_TCLK_VCLKEN                         ((uint32_t)(0x1 << MXC_F_TEST_TCLK_VCLKEN_POS)) /**< TCLK_VCLKEN Mask */

 #define MXC_F_TEST_TCLK_EVENTEN_POS                    (31) /**< TCLK_EVENTEN Position */
 #define MXC_F_TEST_TCLK_EVENTEN                        ((uint32_t)(0x1 << MXC_F_TEST_TCLK_EVENTEN_POS)) /**< TCLK_EVENTEN Mask */

/**@} end of group TCLK_Register */

/**
 * @ingroup  test_registers
 * @defgroup TMR1_Register
 * @brief    clock test mode
 * @{
 */
 #define MXC_F_TEST_TMR1_HIRC_PD_POS                    (11) /**< TMR1_HIRC_PD Position */
 #define MXC_F_TEST_TMR1_HIRC_PD                        ((uint32_t)(0x1 << MXC_F_TEST_TMR1_HIRC_PD_POS)) /**< TMR1_HIRC_PD Mask */

 #define MXC_F_TEST_TMR1_8K_PD_POS                      (14) /**< TMR1_8K_PD Position */
 #define MXC_F_TEST_TMR1_8K_PD                          ((uint32_t)(0x1 << MXC_F_TEST_TMR1_8K_PD_POS)) /**< TMR1_8K_PD Mask */

 #define MXC_F_TEST_TMR1_6K_PD_POS                      (15) /**< TMR1_6K_PD Position */
 #define MXC_F_TEST_TMR1_6K_PD                          ((uint32_t)(0x1 << MXC_F_TEST_TMR1_6K_PD_POS)) /**< TMR1_6K_PD Mask */

 #define MXC_F_TEST_TMR1_X32D_POS                       (16) /**< TMR1_X32D Position */
 #define MXC_F_TEST_TMR1_X32D                           ((uint32_t)(0x1 << MXC_F_TEST_TMR1_X32D_POS)) /**< TMR1_X32D Mask */

 #define MXC_F_TEST_TMR1_32KBYP_POS                     (17) /**< TMR1_32KBYP Position */
 #define MXC_F_TEST_TMR1_32KBYP                         ((uint32_t)(0x1 << MXC_F_TEST_TMR1_32KBYP_POS)) /**< TMR1_32KBYP Mask */

/**@} end of group TMR1_Register */

/**
 * @ingroup  test_registers
 * @defgroup TMR2_Register
 * @brief    i2c test bits
 * @{
 */
 #define MXC_F_TEST_TMR2_SCL0_BE_POS                    (0) /**< TMR2_SCL0_BE Position */
 #define MXC_F_TEST_TMR2_SCL0_BE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL0_BE_POS)) /**< TMR2_SCL0_BE Mask */

 #define MXC_F_TEST_TMR2_SCL1_BE_POS                    (1) /**< TMR2_SCL1_BE Position */
 #define MXC_F_TEST_TMR2_SCL1_BE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL1_BE_POS)) /**< TMR2_SCL1_BE Mask */

 #define MXC_F_TEST_TMR2_SCL0_CE_POS                    (2) /**< TMR2_SCL0_CE Position */
 #define MXC_F_TEST_TMR2_SCL0_CE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL0_CE_POS)) /**< TMR2_SCL0_CE Mask */

 #define MXC_F_TEST_TMR2_SCL1_CE_POS                    (3) /**< TMR2_SCL1_CE Position */
 #define MXC_F_TEST_TMR2_SCL1_CE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL1_CE_POS)) /**< TMR2_SCL1_CE Mask */

 #define MXC_F_TEST_TMR2_IRRX_SDA0_POS                  (4) /**< TMR2_IRRX_SDA0 Position */
 #define MXC_F_TEST_TMR2_IRRX_SDA0                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_IRRX_SDA0_POS)) /**< TMR2_IRRX_SDA0 Mask */

 #define MXC_F_TEST_TMR2_IRRX_SCL0_POS                  (5) /**< TMR2_IRRX_SCL0 Position */
 #define MXC_F_TEST_TMR2_IRRX_SCL0                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_IRRX_SCL0_POS)) /**< TMR2_IRRX_SCL0 Mask */

 #define MXC_F_TEST_TMR2_IRRX_SDA1_POS                  (6) /**< TMR2_IRRX_SDA1 Position */
 #define MXC_F_TEST_TMR2_IRRX_SDA1                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_IRRX_SDA1_POS)) /**< TMR2_IRRX_SDA1 Mask */

 #define MXC_F_TEST_TMR2_IRRX_SCL1_POS                  (7) /**< TMR2_IRRX_SCL1 Position */
 #define MXC_F_TEST_TMR2_IRRX_SCL1                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_IRRX_SCL1_POS)) /**< TMR2_IRRX_SCL1 Mask */

 #define MXC_F_TEST_TMR2_SDA0_DGEN_POS                  (8) /**< TMR2_SDA0_DGEN Position */
 #define MXC_F_TEST_TMR2_SDA0_DGEN                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA0_DGEN_POS)) /**< TMR2_SDA0_DGEN Mask */

 #define MXC_F_TEST_TMR2_SCL0_DGEN_POS                  (9) /**< TMR2_SCL0_DGEN Position */
 #define MXC_F_TEST_TMR2_SCL0_DGEN                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL0_DGEN_POS)) /**< TMR2_SCL0_DGEN Mask */

 #define MXC_F_TEST_TMR2_SDA1_DGEN_POS                  (10) /**< TMR2_SDA1_DGEN Position */
 #define MXC_F_TEST_TMR2_SDA1_DGEN                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA1_DGEN_POS)) /**< TMR2_SDA1_DGEN Mask */

 #define MXC_F_TEST_TMR2_SCL1_DGEN_POS                  (11) /**< TMR2_SCL1_DGEN Position */
 #define MXC_F_TEST_TMR2_SCL1_DGEN                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL1_DGEN_POS)) /**< TMR2_SCL1_DGEN Mask */

 #define MXC_F_TEST_TMR2_SDA0_DGBYP_POS                 (12) /**< TMR2_SDA0_DGBYP Position */
 #define MXC_F_TEST_TMR2_SDA0_DGBYP                     ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA0_DGBYP_POS)) /**< TMR2_SDA0_DGBYP Mask */

 #define MXC_F_TEST_TMR2_SCL0_DGBYP_POS                 (13) /**< TMR2_SCL0_DGBYP Position */
 #define MXC_F_TEST_TMR2_SCL0_DGBYP                     ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL0_DGBYP_POS)) /**< TMR2_SCL0_DGBYP Mask */

 #define MXC_F_TEST_TMR2_SDA1_DGBYP_POS                 (14) /**< TMR2_SDA1_DGBYP Position */
 #define MXC_F_TEST_TMR2_SDA1_DGBYP                     ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA1_DGBYP_POS)) /**< TMR2_SDA1_DGBYP Mask */

 #define MXC_F_TEST_TMR2_SCL1_DGBYP_POS                 (15) /**< TMR2_SCL1_DGBYP Position */
 #define MXC_F_TEST_TMR2_SCL1_DGBYP                     ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL1_DGBYP_POS)) /**< TMR2_SCL1_DGBYP Mask */

 #define MXC_F_TEST_TMR2_SDA0_FE_POS                    (16) /**< TMR2_SDA0_FE Position */
 #define MXC_F_TEST_TMR2_SDA0_FE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA0_FE_POS)) /**< TMR2_SDA0_FE Mask */

 #define MXC_F_TEST_TMR2_SCL0_FE_POS                    (17) /**< TMR2_SCL0_FE Position */
 #define MXC_F_TEST_TMR2_SCL0_FE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL0_FE_POS)) /**< TMR2_SCL0_FE Mask */

 #define MXC_F_TEST_TMR2_SDA1_FE_POS                    (18) /**< TMR2_SDA1_FE Position */
 #define MXC_F_TEST_TMR2_SDA1_FE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA1_FE_POS)) /**< TMR2_SDA1_FE Mask */

 #define MXC_F_TEST_TMR2_SCL1_FE_POS                    (19) /**< TMR2_SCL1_FE Position */
 #define MXC_F_TEST_TMR2_SCL1_FE                        ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL1_FE_POS)) /**< TMR2_SCL1_FE Mask */

 #define MXC_F_TEST_TMR2_SDA0_I2CM_POS                  (20) /**< TMR2_SDA0_I2CM Position */
 #define MXC_F_TEST_TMR2_SDA0_I2CM                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA0_I2CM_POS)) /**< TMR2_SDA0_I2CM Mask */

 #define MXC_F_TEST_TMR2_SCL0_I2CM_POS                  (21) /**< TMR2_SCL0_I2CM Position */
 #define MXC_F_TEST_TMR2_SCL0_I2CM                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL0_I2CM_POS)) /**< TMR2_SCL0_I2CM Mask */

 #define MXC_F_TEST_TMR2_SDA1_I2CM_POS                  (22) /**< TMR2_SDA1_I2CM Position */
 #define MXC_F_TEST_TMR2_SDA1_I2CM                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SDA1_I2CM_POS)) /**< TMR2_SDA1_I2CM Mask */

 #define MXC_F_TEST_TMR2_SCL1_I2CM_POS                  (23) /**< TMR2_SCL1_I2CM Position */
 #define MXC_F_TEST_TMR2_SCL1_I2CM                      ((uint32_t)(0x1 << MXC_F_TEST_TMR2_SCL1_I2CM_POS)) /**< TMR2_SCL1_I2CM Mask */

/**@} end of group TMR2_Register */

/**
 * @ingroup  test_registers
 * @defgroup MBCN_Register
 * @brief    bist control
 * @{
 */
 #define MXC_F_TEST_MBCN_SYSRAM1_POS                    (0) /**< MBCN_SYSRAM1 Position */
 #define MXC_F_TEST_MBCN_SYSRAM1                        ((uint32_t)(0x1 << MXC_F_TEST_MBCN_SYSRAM1_POS)) /**< MBCN_SYSRAM1 Mask */

 #define MXC_F_TEST_MBCN_ICACHE_POS                     (3) /**< MBCN_ICACHE Position */
 #define MXC_F_TEST_MBCN_ICACHE                         ((uint32_t)(0x1 << MXC_F_TEST_MBCN_ICACHE_POS)) /**< MBCN_ICACHE Mask */

/**@} end of group MBCN_Register */

/**
 * @ingroup  test_registers
 * @defgroup MBSTAT_Register
 * @brief    bist stat
 * @{
 */
 #define MXC_F_TEST_MBSTAT_SYSRAM1_POS                  (0) /**< MBSTAT_SYSRAM1 Position */
 #define MXC_F_TEST_MBSTAT_SYSRAM1                      ((uint32_t)(0x1 << MXC_F_TEST_MBSTAT_SYSRAM1_POS)) /**< MBSTAT_SYSRAM1 Mask */

 #define MXC_F_TEST_MBSTAT_ICACHE_POS                   (3) /**< MBSTAT_ICACHE Position */
 #define MXC_F_TEST_MBSTAT_ICACHE                       ((uint32_t)(0x1 << MXC_F_TEST_MBSTAT_ICACHE_POS)) /**< MBSTAT_ICACHE Mask */

/**@} end of group MBSTAT_Register */

/**
 * @ingroup  test_registers
 * @defgroup MBDSEL_Register
 * @brief    bist selection
 * @{
 */
 #define MXC_F_TEST_MBDSEL_SYSRAM1_POS                  (0) /**< MBDSEL_SYSRAM1 Position */
 #define MXC_F_TEST_MBDSEL_SYSRAM1                      ((uint32_t)(0x1 << MXC_F_TEST_MBDSEL_SYSRAM1_POS)) /**< MBDSEL_SYSRAM1 Mask */

 #define MXC_F_TEST_MBDSEL_ICACHE_POS                   (3) /**< MBDSEL_ICACHE Position */
 #define MXC_F_TEST_MBDSEL_ICACHE                       ((uint32_t)(0x1 << MXC_F_TEST_MBDSEL_ICACHE_POS)) /**< MBDSEL_ICACHE Mask */

/**@} end of group MBDSEL_Register */

/**
 * @ingroup  test_registers
 * @defgroup MBDIAG_Register
 * @brief    bist diag
 * @{
 */
 #define MXC_F_TEST_MBDIAG_SYSRAM1_POS                  (0) /**< MBDIAG_SYSRAM1 Position */
 #define MXC_F_TEST_MBDIAG_SYSRAM1                      ((uint32_t)(0x1 << MXC_F_TEST_MBDIAG_SYSRAM1_POS)) /**< MBDIAG_SYSRAM1 Mask */

 #define MXC_F_TEST_MBDIAG_ICACHE_POS                   (3) /**< MBDIAG_ICACHE Position */
 #define MXC_F_TEST_MBDIAG_ICACHE                       ((uint32_t)(0x1 << MXC_F_TEST_MBDIAG_ICACHE_POS)) /**< MBDIAG_ICACHE Mask */

/**@} end of group MBDIAG_Register */

#define MXC_BASE_TEST                    ((uint32_t)0x40000C00UL)
#define MXC_TEST                         ((mxc_test_regs_t*)MXC_BASE_TEST)
#define MXC_TEST_INSTANCES               (1)

#ifdef __cplusplus
}
#endif

#endif /* _TEST_REGS_H_ */
