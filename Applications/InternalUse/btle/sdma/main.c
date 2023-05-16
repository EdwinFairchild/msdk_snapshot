/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Id: main.c 41997 2019-03-29 14:52:27Z kevin.gillespie $
 *
 *******************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "sdma_regs.h"
#include "mxc_pins.h"
#include "board.h"
#include "gpio.h"
#include "uart.h"
#include "ipc_def.h"
#include "tmr.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define MXC_R_SIR_SHR4          *((volatile uint32_t*)(0x40000410))
#define MXC_R_TM                *((volatile uint32_t*)(0x40000C00))
#define MXC_R_TMR3              *((volatile uint32_t*)(0x40000C18))

#define MXC_R_OTP_ACNTL         *((volatile uint32_t*)(0x40029040))

#define OTP_ADDR                0x10800400 
#define OTP_LEN                 0x40

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

volatile uint8_t __attribute__ ((section (".sdma_code"))) sdma_code[] = {  
#include "sdma.inc"
};

// SDMA can not read from OTP, have to copy it into an SRAM section
uint8_t __attribute__ ((section (".otp"))) otp[OTP_LEN];

/*************************************************************************************************/
int main(void)
{
    tmr_cfg_t tmr;
    uint32_t ticks;

    printf("\n***** MAX32665 SDMA, ARM Core *****\n");

    printf("Starting init\n");

     /* Change the pullup on the RST pin to 25K */
    MXC_MCR->ctrl = 0x202;
    
    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Power up the 32MHz XO */
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_X32M_EN;

    // Enable test mode
    MXC_R_TM = 0x1;
    if((MXC_R_TM & 0x1) != 0x1) {
        printf("Error enabling test mode!\n");
    } else {

        // Enable BLE  if disabled by test
        MXC_R_SIR_SHR4 = 0x00100010;

        MXC_R_TMR3 = 0;
    }

    // Enable SDMA clock
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_SDMAD);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_BTLED | MXC_F_GCR_PERCKCN1_TRNGD);
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO1D | MXC_F_GCR_PERCKCN0_GPIO0D);

    // Disable SDMA, assert reset
    MXC_SDMA->ctrl &= ~(MXC_F_SDMA_CTRL_EN);

    // Setup SDMA debugger pins
    GPIO_Init();
    GPIO_Config(&gpio_cfg_sdma);

    // Setup code address
    MXC_SDMA->ip_addr = (uint32_t)&sdma_code;

    // Setup interrupts
    MXC_SDMA->int_mux_ctrl0 = ((BTLE_TX_DONE_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL16_POS) |
        (BTLE_RX_RCVD_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL17_POS) |
        (BTLE_RX_ENG_DET_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL18_POS) |
        (BTLE_SFD_DET_IRQn << MXC_F_SDMA_INT_MUX_CTRL0_INTSEL19_POS));

    MXC_SDMA->int_mux_ctrl1 = ((BTLE_SFD_TO_IRQn << MXC_F_SDMA_INT_MUX_CTRL1_INTSEL20_POS) |
        (BTLE_RFFE_SPIM_IRQn << MXC_F_SDMA_INT_MUX_CTRL1_INTSEL21_POS) |
        (BTLE_GP_EVENT_IRQn << MXC_F_SDMA_INT_MUX_CTRL1_INTSEL22_POS));
        // (TMR0_IRQn << MXC_F_SDMA_INT_MUX_CTRL1_INTSEL23_POS));

    // Disable interrupt mux and poll with A1
    MXC_SDMA->int_mux_ctrl0 = 0;
    MXC_SDMA->int_mux_ctrl1 = 0;
    MXC_SDMA->int_mux_ctrl2 = 0;
    MXC_SDMA->int_mux_ctrl3 = 0;

    // Initialize timer interrupt for testing
    TMR_Init(MXC_TMR0, TMR_PRES_16, NULL);
    TMR_GetTicks(MXC_TMR0, 1, TMR_UNIT_MILLISEC, &ticks);
    tmr.mode = TMR_MODE_CONTINUOUS;
    tmr.cmp_cnt = ticks;
    tmr.pol = 0;
    TMR_Config(MXC_TMR0, &tmr);

    // Setup debug GPIO pins
    SDMA_GPIO_PORT->en |= SDMA_GPIO_PIN;
    SDMA_GPIO_PORT->out |= SDMA_GPIO_PIN;
    SDMA_GPIO_PORT->out_en |= SDMA_GPIO_PIN;

    // Setup GPIO for LHCI UART1, AF3
    MXC_GPIO0->en_clr =     ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->en1_clr =    ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->en2_set =    ((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->pad_cfg1 &= ~((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));
    MXC_GPIO0->pad_cfg2 &= ~((1 << 20) | (1 << 21) | (1 << 22) | (1 << 23));

    // Enable GPIO0 and UART1 clocks
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_UART1D);

    /* Unlock OTP */
    MXC_R_OTP_ACNTL = 0x3a7f5ca3;
    MXC_R_OTP_ACNTL = 0xa1e34f20;
    MXC_R_OTP_ACNTL = 0x9608b2c1;

    // Copy the OTP into the OTP memory section
    memcpy((void*)otp, (void*)OTP_ADDR, OTP_LEN);

    printf("Setup complete\n");

    printf("Starting SDMA\n");

    MXC_SDMA->ctrl |= MXC_F_SDMA_CTRL_EN;

    while (1) {}
}
