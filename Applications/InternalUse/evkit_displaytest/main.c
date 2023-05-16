/**
 * @file    main.c
 * @brief   SPI Master Demo
 * @details Shows Master loopback demo for QSPI0
 *          Read the printf() for instructions
 */

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
 * $Date: 2018-08-31 15:14:52 -0500 (Fri, 31 Aug 2018) $
 * $Revision: 37592 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pb.h"
#include "mxc_config.h"
#include "led.h"
#include "mxc_pins.h"
#include "uart.h"
#include "spi.h"

/***** Definitions *****/
#define TEST_LEN        60      // Words
#define TEST_VALUE      0xA5A5  // This is for master mode only...
#define VALUE           0xFFFF
//#define SPI_SPEED       100000  // Bit Rate
#define SPI_SPEED       1000000  // Bit Rate (works, should be maximum for display)
//#define SPI_SPEED       3000000  // Bit Rate


#define SPI SPI0
#define SPI_IRQ SPI0_IRQn

#define UPDATE_FASTER

// SELECT FROM BELOW
//#define ASYNC         // Comment out line for Async operation

/***** Globals *****/
uint8_t rx_data[TEST_LEN];
uint8_t tx_data[TEST_LEN];
volatile int spi_flag;

/***** Functions *****/

void spi_cb(void *req, int error)
{
    spi_flag = error;
}

void SPI0_IRQHandler(void)
{
    SPI_Handler(SPI);
}

void roll_led(void)
{
    unsigned int max_state = (1<<num_leds)-1; /* 2^n - 1 */
    static unsigned int state = 0;
    unsigned int i;
    
    if (num_leds) {
	for (i = 0; i < num_leds; i++) {
	    if (state & (1<<i)) {
		LED_On(i);
	    } else {
		LED_Off(i);
	    }
	}
	if (++state > max_state) {
	    state = 0;
	}
    }
}

uint8_t reverse(uint8_t val)
{
	uint8_t newval = 0;
	int i;

	for (i = 0;i < 8;i++)
	{
		newval <<= 1;
		if (val & 0x01)
		{
			newval |= 1;
		}
		val >>= 1;
	}

	return newval;
}

    gpio_cfg_t gpio_ssel0;
    gpio_cfg_t gpio_displayon;

void update(uint8_t *arr)
{
    spi_req_t req;
	uint8_t rx_data[1+1+16+2];
	uint8_t tx_data[1+1+16+2];
	int i;
    int offset;

#ifdef UPDATE_FASTER
// SSEL0 high
GPIO_OutSet(&gpio_ssel0);
#endif // UPDATE_FASTER

	for (i = 1;i <= 128;i++)
	{
//	printf("Updating row: %d\n",i);
        offset = 0;
#ifdef UPDATE_FASTER
        // Fast update only requires command byte on first row transfer.
        if (i == 1)
#endif // UPDATE_FASTER
        {
		    tx_data[offset++] = 0x80; // Command, update data
        }
		tx_data[offset++] = reverse(i); // Address (high bit is in LSBit location)
		memcpy(tx_data+offset,arr,16);
		arr += 16;
        offset += 16;
		// 16 bits of dummy data.
		tx_data[offset++] = 0x00;
#ifdef UPDATE_FASTER
        // Fast update only requires command byte on first row transfer.
        if (i == 128)
#endif // UPDATE_FASTER
        {
		    tx_data[offset++] = 0x00; // Extra clocks
        }

        req.tx_data = tx_data;
        req.rx_data = rx_data;
        req.len = offset;
        req.bits = 8;
        req.width = SPI17Y_WIDTH_1;
        req.ssel = 0;
        req.deass = 1;
        req.tx_num = 0;
        req.rx_num = 0;
        req.callback = spi_cb;
        spi_flag =1;

#ifndef UPDATE_FASTER
		// SSEL0 high
        GPIO_OutSet(&gpio_ssel0);
#endif // UPDATE_FASTER
//	mxc_delay(1000);
	// Transfer data
        SPI_MasterTrans(SPI, &req);
//	mxc_delay(1000);
#ifndef UPDATE_FASTER
        // SSEL0 low
        GPIO_OutClr(&gpio_ssel0);
#endif // UPDATE_FASTER
	}

#ifdef UPDATE_FASTER
	// SSEL0 low
        GPIO_OutClr(&gpio_ssel0);
#endif // UPDATE_FASTER

}

int main(void)
{
    int i,j;
    int row;
    uint16_t temp;
    sys_cfg_spi_t spi17y_master_cfg;
    spi_req_t req;
	uint8_t framebuffer[128*16];

    // Turn on use of VDDOIH on P1.8,9,10,11
    MXC_GPIO1->vssel |= PIN_8 | PIN_9 | PIN_10 | PIN_11;
  
    // Initialize slave select 
    gpio_ssel0.port = PORT_1;
    gpio_ssel0.mask = PIN_8;
    gpio_ssel0.pad = GPIO_PAD_NONE;
    gpio_ssel0.func = GPIO_FUNC_OUT;
    GPIO_Config(&gpio_ssel0);
    GPIO_OutClr(&gpio_ssel0);


    spi17y_master_cfg.map = MAP_A;
//    spi17y_master_cfg.ss0 = Enable;
    spi17y_master_cfg.ss0 = Disable;
    spi17y_master_cfg.ss1 = Disable;
    spi17y_master_cfg.ss2 = Disable;
   
    printf("\n***** SPI Display Testing \n"); 
    
        // Configure the peripheral
        if (SPI_Init(SPI, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
        
            Console_Init();
            printf("Error configuring SPI\n");
            while (1) {}
        }

    // Initialize display on signal, take it back from SPI.
    gpio_displayon.port = PORT_1;
    gpio_displayon.mask = PIN_10;
    gpio_displayon.pad = GPIO_PAD_NONE;
    gpio_displayon.func = GPIO_FUNC_OUT;
    GPIO_Config(&gpio_displayon);
    GPIO_OutSet(&gpio_displayon);
    GPIO_OutClr(&gpio_displayon);
    GPIO_OutSet(&gpio_displayon);

	int count = 0;
	int direction = 1;
	uint8_t value;

#if 0
	// Column shutter pattern animation
	while (1)
	{
		value = 0;
		for (i = 0;i < count;i++)
		{
			value <<= 1;
			value |= 1;
		}

		memset(framebuffer,value,sizeof(framebuffer));
		update(framebuffer);

		count+=direction;

		if (count == 8) direction = -1;
		if (count == 0) direction = 1;
	}
#endif

#if 1
	// Horizontal line moving top to bottom animation
	count = 0;
	while (1)
	{
		memset(framebuffer,0xFF,sizeof(framebuffer));
		memset(framebuffer+count*16,0,32);
		update(framebuffer);

		if (count == 126)
		{	
			direction = -1;
			roll_led();
			printf("screen bottom\n");
		}
		if (count == 1)
		{
		       	direction = 1;
			roll_led();
			printf("screen top\n");
		}
		count+=direction;
	}
#endif

#if 0
    // Slow draw of each individual pixel 0 of each 8 bit block.
	while (1)
	{
	    memset(framebuffer,0xFF,sizeof(framebuffer));
		update(framebuffer);
        for (i = 0;i < 128*16;i++)
        {
            framebuffer[i & 0x7FF] &= ~(1);
		    update(framebuffer);
        }
	}
#endif
       
        row = 1;
    while (row <= 128)
    {	
	printf("Updating row: %d\n",row);
	i = 0;
	tx_data[i++] = 0x80; // Command, update data
	tx_data[i++] = reverse(row); // Address
	for (j = 0;j < 128/8;j++)
	{
	  tx_data[i++] = 0x55;
	}
	// 16 bits of dummy data.
	tx_data[i++] = 0x00;
	tx_data[i++] = 0x00;
        
        req.tx_data = tx_data;
        req.rx_data = rx_data;
        req.len = 1+1+128/8+2;
        req.bits = 8;
        req.width = SPI17Y_WIDTH_1;
        req.ssel = 0;
        req.deass = 1;
        req.tx_num = 0;
        req.rx_num = 0;
        req.callback = spi_cb;
        spi_flag =1;

	// SSEL0 high
        GPIO_OutSet(&gpio_ssel0);
	mxc_delay(1000);
	// Transfer data
        SPI_MasterTrans(SPI, &req);
	mxc_delay(1000);
	// SSEL0 low
        GPIO_OutClr(&gpio_ssel0);
	row++;
    }
        
//   SPI_Shutdown(SPI);
        
    printf("\n Done testing \n");
    while(1);
    
    
    return 0;
}
