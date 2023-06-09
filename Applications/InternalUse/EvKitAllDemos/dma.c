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
 * @brief   	DMA Example
 */

/***** Includes *****/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mxc_config.h"
#include "dma.h"
#include "dma_regs.h"
#include "tmr_utils.h"
#include "tmr_regs.h"

/***** Definitions *****/

#define MAX_CHANNEL	16
#define MAX_SIZE	64

/***** Globals *****/
int mychannel = -1;

/***** Functions *****/
void dma_my_int_func(int a, int b)
{
	unsigned int dma_flag;
	
	DMA_GetFlags(mychannel, &dma_flag);
	DMA_ClearFlags(mychannel);
}

void DMA0_IRQHandler()
{
    DMA_Handler(mychannel);
}

void dma_example1(void)
{
	printf("Transfer from memory to memory.\n");

	int retval;
	int i, channels[MAX_CHANNEL] = {0};

    //Init data
	uint8_t *srcdata, *dstdata;
	srcdata = (uint8_t*)malloc(MAX_SIZE);
	dstdata = (uint8_t*)malloc(MAX_SIZE);
	for(i = 0; i < MAX_SIZE; ++i){
		srcdata[i] = i;
		dstdata[i] = 0;
	}

	retval = DMA_Init();
	if(retval != E_NO_ERROR){
		printf("Failed DMA_Init().\n");
	}

    //Demo of acquiring channels
	for(i = 0; i < MAX_CHANNEL; ++i){
		retval = DMA_AcquireChannel();
		if(retval == E_BAD_STATE){
			printf("Failed to acquire channel: %d\n", i);
		}
		channels[i] = retval;
	}

	//Only keeping the first channel [0] for use
	for(i = 1; i < MAX_CHANNEL; ++i){
		retval = DMA_ReleaseChannel(channels[i]);
		if(retval != E_NO_ERROR){
			printf("Failed to release channel %d\n");
		}
	}

	retval = DMA_ConfigChannel(	channels[0],	//ch
					DMA_PRIO_HIGH,		        //prio
					DMA_REQSEL_MEMTOMEM,	    //reqsel
					0,			                //reqwait_en
					DMA_TIMEOUT_4_CLK,	        //tosel
					DMA_PRESCALE_DISABLE,	    //pssel
					DMA_WIDTH_WORD,		        //srcwd
					1,			                //srcinc_en
					DMA_WIDTH_WORD,		        //dstwd
					1,			                //dstinc_en
					31, 			            //burst_size (bytes-1)
					1, 			                //chdis_inten
					0			                //ctz_inten
					);

	if(retval != E_NO_ERROR){
		printf("Failed to config channel\n");
	}

	retval = DMA_SetSrcDstCnt(channels[0], srcdata, dstdata, MAX_SIZE);
	if(retval != E_NO_ERROR){
		printf("Failed to set count\n");
	}

	printf("Starting transfer\n");

	if(DMA_Start(channels[0]) != E_NO_ERROR){
		printf("Failed to start.\n");
	}

	//Validate
    if(memcmp(srcdata, dstdata, MAX_SIZE) != 0){
        printf("Data mismatch.\n");
    }else{
        printf("Data verified.\n");
    }
	
	if(DMA_ReleaseChannel(channels[0]) != E_NO_ERROR){
		printf("Failed to release channel 0\n");
	}

	free(srcdata);
	free(dstdata);

    return;
}

void dma_example2(void)
{
	printf("Transfer with Reload and Callback.\n");

	int i, retval;

    //Init data
	uint8_t *srcdata, *dstdata, *srcdata2, *dstdata2;
	srcdata = (uint8_t*)malloc(MAX_SIZE);
	dstdata = (uint8_t*)malloc(MAX_SIZE);
	srcdata2 = (uint8_t*)malloc(MAX_SIZE);
	dstdata2 = (uint8_t*)malloc(MAX_SIZE);
	for(i = 0; i < MAX_SIZE; ++i){
		srcdata[i] = i;
		dstdata[i] = 0;
        //Different set of data
        srcdata2[i] = MAX_SIZE-1-i;
		dstdata2[i] = 0;
	}

    NVIC_EnableIRQ(DMA0_IRQn);
    __enable_irq();

    DMA_Init();

    mychannel = DMA_AcquireChannel();

	DMA_ConfigChannel(  mychannel,  	            //ch
					    DMA_PRIO_HIGH,		        //prio
                        DMA_REQSEL_MEMTOMEM,	    //reqsel
                        0,			                //reqwait_en
                        DMA_TIMEOUT_4_CLK,	        //tosel
                        DMA_PRESCALE_DISABLE,	    //pssel
                        DMA_WIDTH_WORD,		        //srcwd
                        1,			                //srcinc_en
                        DMA_WIDTH_WORD,		        //dstwd
                        1,			                //dstinc_en
                        31, 			            //burst_size (bytes-1)
                        1, 			                //chdis_inten
                        0			                //ctz_inten
                        );

	DMA_SetSrcDstCnt(mychannel, srcdata, dstdata, MAX_SIZE);

    retval = DMA_SetReload(mychannel, srcdata2, dstdata2, MAX_SIZE);
    if(retval != E_NO_ERROR){
        printf("Failed DMA_SetReload.\n");
    }

    DMA_SetCallback(mychannel, dma_my_int_func);
    DMA_EnableInterrupt(mychannel);
	DMA_Start(mychannel);

	//Validate
    if(memcmp(srcdata, dstdata, MAX_SIZE) != 0 || memcmp(srcdata2, dstdata2, MAX_SIZE) != 0){
        printf("Data mismatch.\n");
    }else{
        printf("Data verified.\n");
    }
	
	if(DMA_ReleaseChannel(mychannel) != E_NO_ERROR){
		printf("Failed to release channel 0\n");
	}

	free(srcdata);
	free(dstdata);
	free(srcdata2);
	free(dstdata2);

    return;
}

// *****************************************************************************
int dma_main(void)
{
	int f;
	for(f = 0; f < 100; ++f) printf("\n");

	printf("***** DMA Example *****\n");
	
    dma_example1();
    dma_example2();
	
	printf("\nExample complete.\n");
    return 0;
}
