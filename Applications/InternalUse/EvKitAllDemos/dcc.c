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
 * @brief   	Instruction Cache Test 
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "mxc_config.h"
#include "dcc.h"
#include "rtc.h"
#include "tmr_utils.h"
#include "spid.h"


/***** Definitions *****/
// RAM Vendor Specific Commands
#define A1024_READ      0x03
#define A1024_WRITE     0x02
#define A1024_EQIO      0x38

// RAM Vendor Specific Values
#define BUFFER_SIZE     110000
#define A1024_ADDRESS   0x80000000

/***** Globals *****/
int s, ss;

spid_cfg_t init_cfg = {
    MXC_V_SPID_CTRL1_SS_SS0,        /* Slave select 0                   */
    0x08,                           /* Number of bits per character     */
    SPID_QUAD,                      /* SPI Data Width                   */
    0x04,                           /* num of system clocks between SS active & first serial clock edge     */
    0x08,                           /* num of system clocks between last serial clock edge and ss inactive  */
    0x10,                           /* num of system clocks between transactions (read / write)             */
    0x1,                            /* Baud freq                        */
    0x1                             /* Baud scale                       */
};

/***** Functions *****/


void dcc_setup(void) 
{
    uint8_t quad_cmd =  A1024_EQIO; /* pre-defined command to use quad mode         */

    // Enable the SPID to talk to RAM 
    SPID_Enable();                  /* Enable the SPID communication mode           */

    // Initialize the desired configuration
    if(SPID_Config(&init_cfg) == E_NO_ERROR) {   
        printf("SPID was initialized properly.\n");
    }   else {
        printf("SPID was not initialized properly.\n");
    }

    /* Hide this with function in SPID.C later */
    MXC_SPID->dma &= ~MXC_S_SPID_DMA_RX_DMA_EN_EN;
    MXC_SPID->dma |= MXC_F_SPID_DMA_TX_FIFO_EN;
    MXC_SPID->ctrl3 &= ~MXC_F_SPID_CTRL3_DATA_WIDTH;

    // dcc_setup to communicate in quad mode
    SPID_Send_Command (&quad_cmd, 1, 1);
    // Wait until quad cmd is sent
    while(SPID_Busy());

    MXC_SPID->ctrl3 &= ~MXC_F_SPID_CTRL3_DATA_WIDTH;
    MXC_SPID->ctrl3 |= MXC_S_SPID_CTRL3_DATA_WIDTH_QUAD;
    MXC_SPID->ctrl3 &= ~MXC_V_SPID_CTRL3_THREE_WIRE_DIS;

    MXC_SPID->dma = 0x00;           /* Disable the FIFOs for transparent operation  */
    MXC_SPID->xmem_ctrl = (0x01 << MXC_F_SPID_XMEM_CTRL_DUMMY_CLK_POS)      |
                          (A1024_READ << MXC_F_SPID_XMEM_CTRL_RD_CMD_POS)   |
                          (A1024_WRITE << MXC_F_SPID_XMEM_CTRL_WR_CMD_POS)  |
                          MXC_F_SPID_XMEM_CTRL_XMEM_EN;
    return;
}


void dcc_test_function(void)
{
    // Defining Variable(s) to write & store data to RAM
    uint8_t write_buffer[BUFFER_SIZE], read_buffer[BUFFER_SIZE];
    uint8_t *address = (uint8_t*) A1024_ADDRESS;;                            /* Variable to store address of RAM */
    int temp,i;


    printf("\n***** SPID Example communicating with RAM in SPI Quad Mode *****\n");

    // Configure the SPID
    printf("Setting up the SPID to communicate with RAM in Quad Mode \n");
    dcc_setup();
    
    // Initialize & write pseudo-random data to be written to the RAM
    printf("Initializing & Writing pseudo-random data to be written to RAM \n");
    srand(0);
    for(i = 0; i < BUFFER_SIZE; i++) { 
        temp = rand(); 
        write_buffer[i] = temp;
        // Write the data to the RAM
        *(address + i) = temp; 
    }

    // Read data from RAM 
    printf("Reading data from RAM and store it inside the read_buffer \n"); 
    for(i = 0; i < BUFFER_SIZE; i++) {
        read_buffer[i] = *(address + i);   
    }

    // Verify data being read from RAM
    if(memcmp(write_buffer, read_buffer, BUFFER_SIZE)) {
        printf("Data is not verified.\n\n");
    } else {
        printf("Data is verified.\n\n");
    }

    // Disable the SPID 
    SPID_Disable();
    return;
}

void dcc_start_timer(void)
{
	if(RTC_Init(MXC_RTC, 0x0000, 0x0000) != E_NO_ERROR) {
		printf("Failed dcc_setup_timer.\n");
		return;
	}
	RTC_EnableRTCE(MXC_RTC);
	printf("Start: %d.%03d \n", MXC_RTC->sec, MXC_RTC->ssec);
	return;
}

void dcc_stop_timer0(void) 
{
	printf("time elapsed: %d.%03d \n", MXC_RTC->sec, MXC_RTC->ssec);
	s = MXC_RTC->sec;
	ss = MXC_RTC->ssec;
	RTC_DisableRTCE(MXC_RTC);
	return;
}

void dcc_stop_timer(void) 
{
	printf("time elapsed: %d.%03d \n", MXC_RTC->sec, MXC_RTC->ssec);
	RTC_DisableRTCE(MXC_RTC);
	return;
}

// *****************************************************************************
int dcc_main(void)
{
	int a;
	for(a = 0; a < 100; ++a) printf("\n");

	printf("***** DCC Example *****\n");

	//Instruction cache enabled
	printf("\nWith data cache enabled:\n");
	DCC_Enable();
	dcc_start_timer();
	dcc_test_function();
	dcc_stop_timer0();

	//Instruction cache disabled
	printf("\nWith data cache disabled:\n");
	DCC_Disable();
	dcc_start_timer();
	dcc_test_function();
	dcc_stop_timer();
	printf("\nExample complete.\n");
	return 0;
}
