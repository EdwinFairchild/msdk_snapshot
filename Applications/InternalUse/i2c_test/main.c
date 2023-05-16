/*******************************************************************************
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
 * $Id$
 *
 *******************************************************************************
 */

 /* Tested using an I2C Memory EEPROM */

#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "led.h"
#include "mxc_pins.h"
#include "uart_regs.h"
#include "i2c.h"
#include "gpio_regs.h"

#define I2C_ADDR            (0x51<<1)

//******************************************************************************
int main(void)
{
    uint8_t data[16];
    uint8_t addr[2];
    int i;

    printf("\n***** I2C Test *****\n");

    for(i = 0; i < 16; i++) {
        data[i] = 16 -i;
    }
    addr[0] = 0;
    addr[1] = 0;

    I2C_Init(MXC_I2C0, I2C_STD_MODE);

    I2C_MasterWrite(MXC_I2C0, I2C_ADDR, data, 16, 0);

    memset(data, 0x0, 16);
    I2C_MasterRead(MXC_I2C0, I2C_ADDR, data, 16, 1);

    for(i = 0; i < 16; i++) {
        if(data[i] != i) {
            printf("Error verifying data 0x%x != 0x%x\n", data[i], i);
            break;
        }
    }

    printf("Done Testing\n");
    while(1) {}

    while(1) {}
    return 0;
}
