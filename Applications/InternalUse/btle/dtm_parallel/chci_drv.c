/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-10-02 15:27:29 -0500 (Wed, 02 Oct 2019) $
 * $Revision: 47325 $
 *
 ******************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief Controller HCI interface to the Host
 */
/*************************************************************************************************/

#include <stdio.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "hci_defs.h"
#include "chci_api.h"
#include "hci_drv.h"
#include "chci_tr_serial.h"
#include "parallel_defs.h"
#include "dbb_registers.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

int command_reset;


/*************************************************************************************************/
void Parallel_IRQHandler(void)
{
    int command_len = 0;
    uint8_t tx_buff[32];

    // Clear interrupt
    INPUT_PORT->int_clr = INPUT_RESET_PIN;

    // Process the command if the INPUT_RESET_PIN is low and the previous command has been reset
    if (command_reset && !INPUT_RESET)
    {
        // Setup INPUT_RESET for positive edge interrupt
        command_reset = 0;

        switch (INPUT_COMMAND)
        {
            case HCI_RESET:
                // Give HCI Reset command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0x03;
                tx_buff[2] = 0x0C;
                tx_buff[3] = 0x00;
                command_len = 4;
                break;

            case HCI_ENH_RX_TEST:
                // Give the HCI Enhanced receiver test command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0x33;
                tx_buff[2] = 0x20;
                tx_buff[3] = 0x03;

                // Get the channel
                tx_buff[4] = INPUT_P0;

                // Get the PHY
                tx_buff[5] = INPUT_P1;

                // Fix the modulation index to standard
                tx_buff[6] = 0;

                command_len = 7;
                break;

            case HCI_ENH_TX_TEST:

                // Give the HCI Enhanced transmitter test
                tx_buff[0] = 0x01;
                tx_buff[1] = 0x34;
                tx_buff[2] = 0x20;
                tx_buff[3] = 0x04;

                // Get the channel
                tx_buff[4] = INPUT_P0;

                // Payload length fixed
                tx_buff[5] = TX_PAYLOAD_LENGTH;

                // Get the payload
                tx_buff[6] = INPUT_P2;

                // Get the PHY
                tx_buff[7] = INPUT_P1;

                command_len = 8;
                break;

            case HCI_TEST_END:

                // Give the test end command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0x1F;
                tx_buff[2] = 0x20;
                tx_buff[3] = 0x00;

                command_len = 4;
                break;

            case HCI_ENABLE_CFO:

                // Give the HCI_ENABLE_CFO command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD0;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x01;

                // Set the enable/disable parameter
                tx_buff[4] = 0x01;

                MXC_DBB_RX->rx_phy_cfo_cfg |= (0x2);

                // command_len = 5;
                command_len = 0;

                // Reset the command input when the pin goes high
                if (INPUT_RESET)
                {
                    command_reset = 1;
                }

                // Set the reset pin
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;

                break;

            case HCI_DISABLE_CFO:

                // Give the HCI_DISABLE_CFO command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD0;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x01;

                // Set the enable/disable parameter
                tx_buff[4] = 0x00;

                MXC_DBB_RX->rx_phy_cfo_cfg &= ~(0x2);

                // command_len = 5;
                command_len = 0;

                // Reset the command input when the pin goes high
                if (INPUT_RESET)
                {
                    command_reset = 1;
                }

                // Set the reset pin
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;

                break;

            case HCI_GET_RSSI:
            {

                int8_t rssi;
                // Give the HCI_GET_RSSI command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD1;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x00;

                rssi = MXC_DBB_RX->rx_phy_rssi_out_pwr;

                // Output the RSSI
                OUTPUT_PORT->out &= ~(OUTPUT_P0_PIN);
                OUTPUT_PORT->out |= (rssi << OUTPUT_P0_POS);

                // command_len = 4;
                command_len = 0;

                // Reset the command input when the pin goes high
                if (INPUT_RESET)
                {
                    command_reset = 1;
                }

                // Set the reset pin
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;

                break;
            }

            case HCI_SET_TX_POWER:

                // Give the HCI_SET_TX_POWER command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD4;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x01;

                // Get the power level
                tx_buff[4] = INPUT_P0;

                dbb_set_rfpower(tx_buff[4]);

                command_len = 0;

                // Reset the command input when the pin goes high
                if (INPUT_RESET)
                {
                    command_reset = 1;
                }

                // Set the reset pin
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;

                break;

            case HCI_ENABLE_TX:

                // Give the HCI_ENABLE_TX command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD5;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x00;

                dbb_seq_tx_enable();

                // command_len = 4;
                command_len = 0;

                // Reset the command input when the pin goes high
                if (INPUT_RESET)
                {
                    command_reset = 1;
                }

                // Set the reset pin
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;
                break;

            case HCI_DISABLE_TX:

                // Give the HCI_DISABLE_TX command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD6;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x00;

                dbb_seq_tx_disable();

                // command_len = 4;
                command_len = 0;

                // Reset the command input when the pin goes high
                if (INPUT_RESET)
                {
                    command_reset = 1;
                }

                // Set the reset pin
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;

                break;

            case HCI_SET_RFFE_9:

                // Give the HCI_SET_RFFE_9 command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD7;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x01;

                // Get the register value
                tx_buff[4] = INPUT_P0;

                command_len = 5;
                break;

            case HCI_SET_RFFE_12:

                // Give the HCI_SET_RFFE_12 command
                tx_buff[0] = 0x01;
                tx_buff[1] = 0xD8;
                tx_buff[2] = 0xFF;
                tx_buff[3] = 0x01;

                // Get the register value
                tx_buff[4] = INPUT_P0;

                command_len = 5;
                break;

            default:
                // printf("ERROR: unknown command 0x%x\n", INPUT_COMMAND);

                // Reset the output trigger
                OUTPUT_PORT->out |= OUTPUT_RESET_PIN;

                OUTPUT_PORT->out &= (OUTPUT_STATUS_PIN);
                OUTPUT_PORT->out |= OUTPUT_STATUS_ERROR;

                // Set the output trigger
                OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

                return;
        }

        // Reset the output trigger
        OUTPUT_PORT->out |= OUTPUT_RESET_PIN;

        // Write command to stack
        chciTrSerialRxIncoming(tx_buff, command_len);
    }

    // Reset the command input when the pin goes high
    if (INPUT_RESET)
    {
        command_reset = 1;
    }
}

/*************************************************************************************************/
void chciDrvInit(void)
{
    // Setup input port for input
    INPUT_PORT->en |= INPUT_MASK;
    INPUT_PORT->out_en &= ~(INPUT_MASK);
    INPUT_PORT->pad_cfg1 &= ~(INPUT_MASK);
    INPUT_PORT->pad_cfg2 &= ~(INPUT_MASK);

    // Setup output port for output
    OUTPUT_PORT->out &= ~(OUTPUT_MASK);
    OUTPUT_PORT->out |= OUTPUT_STATUS_IDLE;
    OUTPUT_PORT->en |= OUTPUT_MASK;
    OUTPUT_PORT->pad_cfg1 &= ~(OUTPUT_MASK);
    OUTPUT_PORT->pad_cfg2 &= ~(OUTPUT_MASK);
    OUTPUT_PORT->out_en |= OUTPUT_MASK;

    // Enable negative edge interrupt in INPUT_REST pin
    NVIC_EnableIRQ(INPUT_IRQ);
    NVIC_SetVector(INPUT_IRQ, Parallel_IRQHandler);

    INPUT_PORT->int_mod |= INPUT_RESET_PIN;
    INPUT_PORT->int_dual_edge |= INPUT_RESET_PIN;
    INPUT_PORT->int_en_set = INPUT_RESET_PIN;

    command_reset = 1;
}

/*************************************************************************************************/
uint16_t chciDrvWrite(uint8_t prot, uint8_t type, uint16_t len, uint8_t *pData)
{
    uint16_t i = 0;

    // Output Status
    OUTPUT_PORT->out &= ~(OUTPUT_STATUS_PIN);
    if(pData[5] != 0) 
    {
        OUTPUT_PORT->out |= OUTPUT_STATUS_ERROR;
    } else {
        OUTPUT_PORT->out |= OUTPUT_STATUS_IDLE;
    }

    // Output Received packet count to OUTPUT_PORT if Test end command
    if((len >= 8) && (pData[2] == 0x01) && (pData[3] == 0x1F) && (pData[4] == 0x20)) {
        // Save the number of received packets
        i = pData[7] << 8;
        i |= pData[6];

        // Set received packet count to the output pins
        OUTPUT_PORT->out &= ~(OUTPUT_P0_PIN);
        OUTPUT_PORT->out |= (i << OUTPUT_P0_POS);
    } 

    // Output the RSSI value
    else if((len >= 7) && (pData[2] == 0x01) && (pData[3] == 0xD1) && (pData[4] == 0xFF)) {
        // Save the number of received packets
        i = (uint16_t)pData[6];

        // Set RSSI to the output pins
        OUTPUT_PORT->out &= ~(OUTPUT_P0_PIN);
        OUTPUT_PORT->out |= (i << OUTPUT_P0_POS);
    }

    // Set the reset pin
    OUTPUT_PORT->out &= ~(OUTPUT_RESET_PIN);

  // Return the number of packets written
  return (len+1);
}
