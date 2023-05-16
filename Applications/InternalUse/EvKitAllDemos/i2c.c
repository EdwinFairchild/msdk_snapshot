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
 * $Date: 2018-08-10 09:36:39 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/**
 * @file    	main.c
 * @brief   	I2C (Loopback?) Example 
 * @details 	This example uses the I2C Master to read/write from/to the I2C Slave. For
 * 		        this example you must connect P2.7 to P2.17 (SDA) and P2.8 to P2.18 (SCL). The Master
 * 		        will use P2.7 and P2.8. The Slave will use P2.17 and P2.18. You must also
 * 		        connect the pull-up jumpers (JP23 and JP24) to the proper I/O voltage.
 * 		        Refer to JP27 to determine the I/O voltage.
 * @note	    Other devices on the EvKit will be using the same bus. This example cannot be combined with
 * 		        a PMIC or bluetooth example because the I2C Slave uses GPIO pins for those devices.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "i2c.h"

/***** Definitions *****/
#define I2C_MASTER	    MXC_I2C0
#define I2C_MASTER_IDX	0

#define I2C_SLAVE	    MXC_I2C1
#define I2C_SLAVE_IDX	1
#define I2C_SLAVE_ADDR	(0x51<<1)

/***** Globals *****/
static uint8_t txdata[16];
static uint8_t rxdata[16];

volatile int i2c_flag;
volatile int i2c_flag1;

/***** Functions *****/
//Slave interrupt handler
void I2C1_IRQHandler(void) 
{
    I2C_Handler(I2C_SLAVE);
    return;
}

//Master interrupt handler
void I2C0_IRQHandler(void)
{
    I2C_Handler(I2C_MASTER);
    return;
}

//I2C callback 
void i2c_callback(i2c_req_t *req, int error)
{
    i2c_flag = error;
    return;
}

//Prints out human-friendly format to read txdata and rxdata
void i2c_print_data(void)
{
    int i;
    printf("txdata: ");
    for(i = 0; i < 16; ++i) {
        printf("%d\t", txdata[i]);
    }

    printf("\nrxdata: ");
    for(i = 0; i < 16; ++i) {
        printf("%d\t", rxdata[i]);
    }

    printf("\n");

    return;
}

//Compare txdata and rxdata to see if they are the same
void i2c_verify(void)
{
    int i; 
    int fails = 0;
    for(i = 0; i < 16; ++i) {
        if(txdata[i] != rxdata[i]) {
            ++fails;
	}
    }
    if(fails > 0) printf("Fail.\n");
    else printf("Pass.\n");

    return;
}

// *****************************************************************************
int i2c_main(void)
{
    printf("\n***** I2C Loopback Example *****\n");

    int error, i = 0;

    //Setup the I2CM
    I2C_Shutdown(I2C_MASTER);
    I2C_Init(I2C_MASTER, I2C_STD_MODE);
    NVIC_EnableIRQ(I2C0_IRQn);

    //Setup the I2CS
    I2C_Shutdown(I2C_SLAVE);
    I2C_Init(I2C_SLAVE, I2C_STD_MODE);
    NVIC_EnableIRQ(I2C1_IRQn);	

    //MASTER WRITE SLAVE READ **********************************************
    printf("Master write, Slave read ... \n");
    //Initialize test data
    for(i = 0; i < 16; i++) {
        txdata[i] = i;
        rxdata[i] = 0;
    }

    printf("Initial test data:\n");
    i2c_print_data();

    //Prepare SlaveAsync
    i2c_req_t req;
    req.addr = I2C_SLAVE_ADDR;
    req.tx_data = txdata;
    req.tx_len = 16;
    req.rx_data = rxdata;
    req.rx_len = 16;
    req.restart = NULL;
    req.callback = i2c_callback;

    i2c_flag = 1;
    if((error = I2C_SlaveAsync(MXC_I2C1, &req)) != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        return -1;
    }

    printf("Master writes data to Slave.\n");
    if((error = I2C_MasterWrite(MXC_I2C0, I2C_SLAVE_ADDR, txdata, 16, 0)) != 16) {
        printf("Error writing %d\n", error);
        return -1;
    }

    //Wait for callback
    while(i2c_flag == 1);

    //Check callback flag
    if(i2c_flag != E_NO_ERROR) {
    printf("Error with i2c_callback %d\n", i2c_flag);
    }

    printf("Checking test data:\n");
    i2c_print_data();
    i2c_verify();

    //Initialize new test data
    for(i = 0; i < 16; i++) {
        txdata[i] = 15-i;
        rxdata[i] = 0;
    }

    //SLAVE WRITE MASTER READ **********************************************
    printf("Slave write, Master read ... \n");
    printf("New test data:\n");
    i2c_print_data();

    i2c_flag = 1;
    if((error = I2C_SlaveAsync(MXC_I2C1, &req)) != E_NO_ERROR) {
        printf("Error starting async read %d\n", error);
        return -1;
    }

    printf("Master reads data from Slave.\n");
    if((error = I2C_MasterRead(MXC_I2C0, I2C_SLAVE_ADDR, rxdata, 16, 0)) != 16) {
        printf("Error reading%d\n", error);
        return -1;
    }

    //Wait for callback
    while(i2c_flag == 1);

    //Check callback flag
    if(i2c_flag != E_NO_ERROR) {
        printf("Error with i2c_callback %d\n", i2c_flag);
        return -1;
    }

    printf("Checking test data:\n");
    i2c_print_data();
    i2c_verify();


    printf("\nExample complete.\n");

    return 0;
}
