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
 * $Date: 2018-11-09 13:54:22 -0600 (Fri, 09 Nov 2018) $
 * $Revision: 39089 $
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
#define SPI_SPEED       1000000  // Bit Rate (works, should be maximum for display)


#define SPI SPI0
#define SPI_IRQ SPI0_IRQn

#define UPDATE_FASTER

// SELECT FROM BELOW
//#define ASYNC         // Comment out line for Async operation

/***** Globals *****/
uint8_t rx_data[TEST_LEN];
uint8_t tx_data[TEST_LEN];
volatile int spi_flag;

gpio_cfg_t gpio_ssel0 = { PORT_1, PIN_8, GPIO_FUNC_OUT,GPIO_PAD_NONE };
gpio_cfg_t gpio_displayon = { PORT_1, PIN_10, GPIO_FUNC_OUT, GPIO_PAD_NONE };

extern mxc_uart_regs_t *ConsoleUart;

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

// Reverse bits in a byte.
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


// Update from passed in frame buffer array.
// 1 is white, 0 is black
// 128x128 pixels
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
	// Transfer data
        SPI_MasterTrans(SPI, &req);
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

int display_init(void)
{
    sys_cfg_spi_t spi17y_master_cfg;

    // Turn on use of VDDOIH on P1.8,9,10,11
    MXC_GPIO1->vssel |= PIN_8 | PIN_9 | PIN_10 | PIN_11;
  
    // Initialize slave select 
    GPIO_Config(&gpio_ssel0);
    GPIO_OutClr(&gpio_ssel0);


    spi17y_master_cfg.map = MAP_A;
//    spi17y_master_cfg.ss0 = Enable;
    spi17y_master_cfg.ss0 = Disable;
    spi17y_master_cfg.ss1 = Disable;
    spi17y_master_cfg.ss2 = Disable;
   
        // Configure the peripheral
        if (SPI_Init(SPI, 0, SPI_SPEED, spi17y_master_cfg) != 0) {
        
            Console_Init();
            printf("Error configuring SPI\n");
            while (1) {}
        }

    // Initialize display on signal, take it back from SPI.
    GPIO_Config(&gpio_displayon);
    GPIO_OutSet(&gpio_displayon);
    GPIO_OutClr(&gpio_displayon);
    GPIO_OutSet(&gpio_displayon);

    return 0;
}

void display_test(void)
{
    int i;
    uint8_t value;
    uint8_t framebuffer[128*16];

    memset(framebuffer,0,sizeof(framebuffer));
	update(framebuffer);
    for (i = 0;i < sizeof(framebuffer);i++)
    {
        value = (i & 1)?0xFF:0x00;
        if (i & 0x80) value ^= 0xFF;
        framebuffer[i] = value;
    }
	update(framebuffer);
//while(1);
#if 0
    while (1) {
    for (i = 0;i < sizeof(framebuffer);i++)
    {
        framebuffer[i] ^= 0xff;
    }
	update(framebuffer);
	mxc_delay(1000);
    }
#endif
}

int main(void)
{
    int i,j;
    int row;
    uint16_t temp;
    spi_req_t req;
    uint8_t framebuffer[128*16];

    printf("\n***** ME14 EV Kit Quick Test **** \n"); 

	if (display_init() != 0 ) {
	    printf("Error configuring display\n");
	}

    printf("Display Test\n");
    display_test();
    printf("Display Test Checkerboard OK?:\n");
    while (UART_ReadByte(ConsoleUart) != 'y');
    printf("y\n");
    LED_On(0);
    printf("LED D2 On?:\n");
    while (UART_ReadByte(ConsoleUart) != 'y');
    printf("y\n");
    LED_On(1);
    printf("LED D3 On?:\n");
    while (UART_ReadByte(ConsoleUart) != 'y');
    printf("y\n");
    printf("Press SW2\n");
    while (PB_Get(0) == 0);
    printf("Press SW3\n");
    while (PB_Get(1) == 0);

    SPI_Shutdown(SPI);

    printf("Plug in USB data cable\n");
    usb_test();
    printf("USB PASS\n");

    printf("Starting BLE Test\n");
    printf("Attempt to connect to \"FIT\"\n");
    ble_test();
        
    printf("\nTest PASS\n");
    while(1);
    
    
    return 0;
}
