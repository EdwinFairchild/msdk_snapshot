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
 * @file    main.c
 * @brief   Pulse Train API test.
 * @note    Use a logic analyzer to observe the waveforms
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_lock.h"
#include "board.h"
#include "nvic_table.h"
#include "pt.h"
#include "gpio.h"
#include "mxc_pins.h"
#include "pb.h"

/***** Definitions *****/
#define ALL_PT	0xFFFF
#define SW1	    0
//#define VERBOSE

/***** Globals *****/
extern const gpio_cfg_t pb_pin[];

/***** Functions *****/
static void PB_Start_Stop_Handler(void)
{
	//Check if any PTs are running
	if(PT_IsActiveMulti(ALL_PT)) {
		PT_StopMulti(ALL_PT);
		printf("All PTs stopped.\n");
	}else{
		//Start PT11, PT12, PT13, and PT14
		unsigned int enablePTMask = MXC_F_PT_ENABLE_PT11 | MXC_F_PT_ENABLE_PT12 | MXC_F_PT_ENABLE_PT13 | MXC_F_PT_ENABLE_PT14;
		PT_StartMulti(enablePTMask);
		printf("All PTs started.\n");
	}
	return;
}

//Generates continuous pulse train on P2.27
void continuous_PT(void)
{
	printf("Starting continuous PT ...\n");
	pt_pt_cfg_t pt13_cfg;
	sys_cfg_pt_t gpio2_27_cfg;

	pt13_cfg.bps = SystemCoreClock >> 2; //Can't be greater than /2
	pt13_cfg.pattern = 0x55aa55aa; //0101 0101 1010 1010 ...
	pt13_cfg.ptLength = 0;
	pt13_cfg.loop = 0; //0=continuous
	pt13_cfg.loopDelay = 0;

	gpio2_27_cfg = gpio_cfg_pt13_0;
	if(PT_PTConfig(MXC_PT, 13, &pt13_cfg, &gpio2_27_cfg) != E_NO_ERROR) {
		printf("Fail at PT13\n");
	}

	//Initialize pulse train interface
	
	if(PT_IsActive(MXC_PT, 13)) {
		printf("PT13 reported active before PT_Start ...\n");
	}

	printf("Starting PT13\n");
	PT_Start(MXC_PT, 13);

	if(!PT_IsActive(MXC_PT, 13)) {
		printf("PT13 did not start ...\n");
	}

	printf("continuous_PT() successful.\n");
	return;
}	

//Generates square wave on 
void squarewave(void)
{
	printf("Starting square wave ...\n");
	int freq = (SystemCoreClock >> 3); 
	// pt_pt_cfg_t pt14_cfg;
	sys_cfg_pt_t gpio2_28_cfg;
	
	gpio2_28_cfg = gpio_cfg_pt14_0;
	
	//Configuring a square wave
	if(PT_SqrWaveConfig(MXC_PT, 14, freq, &gpio2_28_cfg) != E_NO_ERROR) {
		printf("Fail at PT14.\n");	
	}

	PT_Start(MXC_PT, 14);

	if(PT_IsActive(MXC_PT, 14)) {
		printf("PT14 is active.\n");
	}

	printf("squarewave() successful.\n");
	return;
}

void PT_sequence(void)
{
	printf("Starting PT sequence ...\n");
	pt_pt_cfg_t pt11_cfg, pt12_cfg;
	sys_cfg_pt_t gpio2_25_cfg, gpio2_26_cfg;
	
	//Setup GPIO P2.25 (uses PT11)
	pt11_cfg.bps = 4; //bit rate
	pt11_cfg.ptLength = 8; //bits
	pt11_cfg.pattern = 0x66;
	pt11_cfg.loop = 2; //# of loops
	pt11_cfg.loopDelay = 0; //delay between loops
	gpio2_25_cfg = gpio_cfg_pt11_0;
	if(PT_PTConfig(MXC_PT, 11, &pt11_cfg, &gpio2_25_cfg) != E_NO_ERROR) {
		printf("Fail at PT11.\n");
	}

	//Setup GPIO P2.26 (uses PT12)
	pt12_cfg.bps = 6; //bit rate
	pt12_cfg.ptLength = 8; //bits
	pt12_cfg.pattern = 0x6C;
	pt12_cfg.loop = 2; //# of loops
	pt12_cfg.loopDelay = 0; //delay between loops
	gpio2_26_cfg = gpio_cfg_pt12_0;
	if(PT_PTConfig(MXC_PT, 12, &pt12_cfg, &gpio2_26_cfg) != E_NO_ERROR) {
		printf("Fail at PT12.\n");
	}
	
	PT_SetRestart(MXC_PT, 12, 11, 0);
	PT_SetRestart(MXC_PT, 11, 12, 0);

	PT_Start(MXC_PT, 11);

#ifdef VERBOSE
	if(PT_IsActive(MXC_PT, 11)) {
		printf("PT11 is active.\n");
	}else{
        printf("PT11 is not active ... :(\n");
    }
	


	if(PT_IsActive(MXC_PT, 12)) {
		printf("PT12 is active.\n");
	}else{
        printf("PT12 is not active ... :(\n");
    }
	
#endif

	printf("PT_sequence successful.\n");

	return;
}

/******************************************************************************/
int pt_main(void)
{
	printf("***** Pulse Train Demo *****\n");

	printf("GPIO 2.27 = Continuous patterns of 0x55aa5aa at __bps\n");
	printf("GPIO 2.28 = Continuous square wave at 2.5 MHz\n");
	printf("GPIO 2.25 = Outputs 8'b01100110 at 4 bps, loops twice then starts at GPIO 2.26\n");
	printf("GPIO 2.26 = Outputs 8'b01101100 at 6 bps, loops twice then starts at GPIO 2.25\n");
	printf("SW1 = Stop/Start all pulse trains.\n\n");

	PB_RegisterCallback(SW1, PB_Start_Stop_Handler);
	PT_EnableINTMulti(ALL_PT);
	PT_Init();

	continuous_PT();
	squarewave();
	PT_sequence();

	printf("\nExample complete.\n");
    return 0;
}
