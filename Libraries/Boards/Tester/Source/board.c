/*******************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-01-23 08:41:40 -0600 (Wed, 23 Jan 2019) $
 * $Revision: 40507 $
 *
 ******************************************************************************/

#include <stdio.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_assert.h"
#include "board.h"
#include "uart.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "led.h"
#include "pb.h"
#include "spixfc.h"

/***** Global Variables *****/
mxc_uart_regs_t * ConsoleUart = MXC_UART_GET_UART(CONSOLE_UART);
extern uint32_t SystemCoreClock;

/***** File Scope Variables *****/
const uart_cfg_t uart_cfg = {
    .parity = UART_PARITY_DISABLE,
    .size   = UART_DATA_SIZE_8_BITS,
    .stop   = UART_STOP_1,
    .flow   = UART_FLOW_CTRL_DIS,
    .pol    = UART_FLOW_POL_DIS,
    .baud   = CONSOLE_BAUD,
    .clksel = UART_CLKSEL_SYSTEM
};

const sys_cfg_uart_t uart_sys_cfg = {
    .map = MAP_A,
    .flow = Disable
};

/******************************************************************************/
void mxc_assert(const char *expr, const char *file, int line)
{
    // printf("MXC_ASSERT %s #%d: (%s)\n", file, line, expr);
    while (1);
}

/******************************************************************************/
int Board_Init(void)
{
    int err;

    if ((err = Console_Init()) != E_NO_ERROR) {
        MXC_ASSERT_FAIL();
        return err;
    }

    return E_NO_ERROR;
}

/******************************************************************************/
int Console_Init(void)
{
    int err;

    if ((err = UART_Init(ConsoleUart, &uart_cfg, &uart_sys_cfg)) != E_NO_ERROR) {
        return err;
    }

    return E_NO_ERROR;
}

/******************************************************************************/
int Console_Shutdown(void)
{
    int err;
    
    if ((err = UART_Shutdown(ConsoleUart)) != E_NO_ERROR) {
        return err;
    }
    
    return E_NO_ERROR;
}

/******************************************************************************/
void NMI_Handler(void)
{
    __NOP();
}
