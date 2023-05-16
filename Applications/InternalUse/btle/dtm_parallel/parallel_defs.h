/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  LL HCI driver implementation file.
 *
 *  $Date: 2019-02-15 14:07:22 -0600 (Fri, 15 Feb 2019) $
 *  $Revision: 41039 $
 *
 *  Copyright (c) 2013 Wicentric, Inc., all rights reserved.
 *  Wicentric confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact Wicentric, Inc. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/


#include "uart_regs.h"
#include "gpio_regs.h"


/**************************************************************************************************
  Macros
**************************************************************************************************/
#define INPUT_PORT                  MXC_GPIO0
#define INPUT_MASK                  0xFFFFFFFF
#define INPUT_IRQ                   GPIO0_IRQn

#define HCI_RESET                   0x0
#define HCI_ENH_RX_TEST             0x1
#define HCI_ENH_TX_TEST             0x2
#define HCI_TEST_END                0x3
#define HCI_ENABLE_CFO              0x4
#define HCI_DISABLE_CFO             0x5
#define HCI_GET_RSSI                0x6
#define HCI_SET_TX_POWER            0x7
#define HCI_ENABLE_TX               0x8
#define HCI_DISABLE_TX              0x9
#define HCI_SET_RFFE_9              0xA
#define HCI_SET_RFFE_12             0xB

#define INPUT_RESET_POS             0
#define INPUT_RESET_PIN             (0x1 << INPUT_RESET_POS)
#define INPUT_RESET                 ((INPUT_PORT->in & INPUT_RESET_PIN) >> INPUT_RESET_POS)
#define INPUT_COMMAND_POS           1
#define INPUT_COMMAND_PIN           (0x7F << INPUT_COMMAND_POS)
#define INPUT_COMMAND               ((INPUT_PORT->in & INPUT_COMMAND_PIN) >> INPUT_COMMAND_POS)
#define INPUT_P0_POS                8
#define INPUT_P0_PIN                (0xFF << INPUT_P0_POS)
#define INPUT_P0                    ((INPUT_PORT->in & INPUT_P0_PIN) >> INPUT_P0_POS)
#define INPUT_P1_POS                16
#define INPUT_P1_PIN                (0xFF << INPUT_P1_POS)
#define INPUT_P1                    ((INPUT_PORT->in & INPUT_P1_PIN) >> INPUT_P1_POS)
#define INPUT_P2_POS                24
#define INPUT_P2_PIN                (0xFF << INPUT_P2_POS)
#define INPUT_P2                    ((INPUT_PORT->in & INPUT_P2_PIN) >> INPUT_P2_POS)

#define OUTPUT_PORT                 MXC_GPIO1
#define OUTPUT_POS                  0
#define OUTPUT_MASK                 (0xFFFF << OUTPUT_POS)
#define OUTPUT_RESET_POS            0
#define OUTPUT_RESET_PIN            (0x1 << OUTPUT_RESET_POS)
#define OUTPUT_STATUS_POS           1
#define OUTPUT_STATUS_PIN           (0xF << OUTPUT_STATUS_POS)
#define OUTPUT_P0_POS               5
#define OUTPUT_P0_PIN               (0x7FF << OUTPUT_P0_POS)

#define OUTPUT_STATUS_INIT          (0x0 << OUTPUT_STATUS_POS)         
#define OUTPUT_STATUS_CALIB         (0x1 << OUTPUT_STATUS_POS)
#define OUTPUT_STATUS_IDLE          (0xA << OUTPUT_STATUS_POS)
#define OUTPUT_STATUS_ERROR         (0x3 << OUTPUT_STATUS_POS)

#define TX_PAYLOAD_LENGTH           37
