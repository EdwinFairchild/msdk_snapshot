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

#include <stdio.h>
#include <string.h>
#include "gpio_regs.h"
#include "mxc_config.h"
#include "i2c.h"
#include "led.h"

#define I2C_ADDR            (0x51<<1)

uint8_t rx_data[16];
uint8_t tx_data[16];
volatile int i2c_flag;
/******************************************************************************/
void I2C0_IRQHandler(void)
{
    LED_On(0);
    I2C_Handler(MXC_I2C0);
    LED_Off(0);
}

/******************************************************************************/
void i2c_callback(i2c_req_t *req, int error) 
{
    i2c_flag = error;
}
//******************************************************************************
int test0(void)
{
    int error, i;
    int num = 0;
    memset(tx_data, 0, 16);
    if((error = I2C_Slave(MXC_I2C0, I2C_ADDR, rx_data, 16, tx_data, 16, &num)) != E_NO_ERROR) {
        printf("Error reading %d\n", error);
        return -1;
    }

    // Check that the tx_data is still empty
    for(i = 0; i < 16; i++) {
        if(tx_data[i] != 0) {
            printf("Error with tx_data\n");
            return -2;
        }
    }

    return 0;
}

//******************************************************************************
int test1(void)
{
    int error, i;
    int num = 0;
    memset(tx_data, 0, 16);
    if((error = I2C_Slave(MXC_I2C0, I2C_ADDR, rx_data, 16, tx_data, 16, &num)) != E_NO_ERROR) {
        printf("Error writing %d\n", error);
        return -1;
    }

    // Verify with tx_data
    error = 0;
    for(i = 0; i < 16; i++) {
        if(tx_data[i] != i+0x10) {
            printf("Error with tx_data\n");
            return -2;
        }
    }

    return 0;
}

//******************************************************************************
int test2(void)
{
    // Test master writing less than expected number of bytes
    int error, i;
    int num = 0;
    memset(tx_data, 0, 16);
    if((error = I2C_Slave(MXC_I2C0, I2C_ADDR, rx_data, 16, tx_data, 16, &num)) != E_NO_ERROR) {
        printf("Error writing %d\n", error);
        return -1;
    }

    // Verify with tx_data
    error = 0;
    for(i = 0; i < 14; i++) {
        if(tx_data[i] != i+0x10) {
            printf("Error with tx_data\n");
            return -2;
        }
    }
    if(tx_data[14] != 0) {
        printf("Error with tx_data\n");
        return -3;
    }
    if(tx_data[15] != 0) {
        printf("Error with tx_data\n");
        return -4;
    }

    return 0;
}

//******************************************************************************
int test3(void)
{
    // Test master reading less than expected number of bytes
    int error, i;
    int num = 0;
    memset(tx_data, 0, 16);
    if((error = I2C_Slave(MXC_I2C0, I2C_ADDR, rx_data, 16, tx_data, 16, &num)) != E_NO_ERROR) {
        printf("Error writing %d\n", error);
        return -1;
    }

    // Verify with tx_data
    error = 0;
    for(i = 0; i < 16; i++) {
        if(tx_data[i] != 0x0) {
            printf("Error with tx_data\n");
            return -2;
        }
    }

    return 0;
}

//******************************************************************************
int test4(void)
{
    // Test master read async
    int error, i;
    int num = 0;
    memset(tx_data, 0, 16);
    if((error = I2C_Slave(MXC_I2C0, I2C_ADDR, rx_data, 16, tx_data, 16, &num)) != E_NO_ERROR) {
        printf("Error writing %d\n", error);
        return -1;
    }

    // Verify with tx_data
    error = 0;
    for(i = 0; i < 16; i++) {
        if(tx_data[i] != 0x0) {
            printf("Error with tx_data\n");
            return -2;
        }
    }

    return 0;
}

//******************************************************************************
int test5(void)
{
    // Test master write async
    int error, i;
    int num = 0;
    memset(tx_data, 0, 16);
    if((error = I2C_Slave(MXC_I2C0, I2C_ADDR, rx_data, 16, tx_data, 16, &num)) != E_NO_ERROR) {
        printf("Error writing %d\n", error);
        return -1;
    }

    // Verify with tx_data
    error = 0;
    for(i = 0; i < 16; i++) {
        if(tx_data[i] != (i + 0x10)) {
            printf("Error with tx_data\n");
            return -2;
        }
    }

    return 0;
}
/******************************************************************************/
int test6(void)
{
    // Test master write async
    int error, i;
    i2c_req_t req;
    req.addr = I2C_ADDR;
    req.tx_data = tx_data;
    req.tx_len = 16;
    req.rx_data = rx_data;
    req.rx_len = 16;
    req.restart = NULL;
    req.callback = i2c_callback;

    i2c_flag = 1;
    LED_Off(0);
    if((error = I2C_SlaveAsync(MXC_I2C0, &req)) != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        return -1;
    }

    // Wait for callback
   while(i2c_flag == 1);

    // Check callback flag
    if(i2c_flag != E_NO_ERROR) {
        printf("Error with i2c_callback %d\n", i2c_flag);
    }

    for(i = 0; i < 16; i++) {
        if(rx_data[i] != (i +0x10)) {
            printf("error verifying %d\n", i);
            return -1;
        }
    }

    return 0;
}
/******************************************************************************/
int test7(void)
{
    // Test master read async
    int error, i;
    i2c_req_t req;
    req.addr = I2C_ADDR;
    req.rx_data = rx_data;
    req.rx_len =16;
    req.tx_data = tx_data;
    req.tx_len = 16;
    req.restart = NULL;
    req.callback = i2c_callback;

    for(i = 0; i < 16; i++) {
        tx_data[i] = i;
        rx_data[i] = 0;
    }
    i2c_flag = 1;

    LED_Off(0);
    if((error = I2C_SlaveAsync(MXC_I2C0, &req)) != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        return -1;
    }

    // Wait for callback
   while(i2c_flag == 1);

    // Check callback flag
    if(i2c_flag != E_NO_ERROR) {
        printf("Error with i2c_callback %d\n", i2c_flag);
    }

    // Verify with tx_data
    error = 0;
    for(i = 0; i < 16; i++) {
        if(rx_data[i] != 0x0) {
            printf("Error with rx_data\n");
            return -2;
        }
    }
    return 0;
}
//******************************************************************************
int main(void)
{
    int i;

    printf("\n***** I2C Slave Test *****\n");

    for(i = 0; i < 16; i++) {
        rx_data[i] = i;
        tx_data[i] = 0;
    }
    I2C_Shutdown(MXC_I2C0);
    I2C_Init(MXC_I2C0, I2C_STD_MODE);

    NVIC_EnableIRQ(I2C0_IRQn);
#if 1
    if(test0() != 0) {
        printf("Error with test0\n");
    } else {
        printf("Test0 passed\n");
    }
#endif

#if 1
    if(test1() != 0) {
        printf("Error with test1\n");
    } else {
        printf("Test1 passed\n");
    }
#endif

#if 1
    if(test2() != 0) {
        printf("Error with test2\n");
    } else {
        printf("Test2 passed\n");
    }
#endif

#if 1
    if(test3() != 0) {
        printf("Error with test3\n");
    } else {
        printf("Test3 passed\n");
    }
#endif


#if 1
    if(test4() != 0) {
        printf("Error with test4\n");
    } else {
        printf("Test4 passed\n");
    }
#endif

#if 1
    if(test5() != 0) {
        printf("Error with test5\n");
    } else {
        printf("Test5 passed\n");
    }
#endif

#if 1
    if(test6() != 0) {
        printf("Error with test6\n");
    } else {
        printf("Test6 passed\n");
    }
#endif
#if 1
    if(test7() != 0) {
        printf("Error with test7\n");
    } else {
        printf("Test7 passed\n");
    }
#endif
    printf("Done Testing\n");
    while(1) {}

    return 0;
}