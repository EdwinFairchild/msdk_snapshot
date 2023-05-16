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
 * @brief   	Instruction cache example
 * @details     Show the time difference when executing operations when instruction
 *              cache is enabled and disabled
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "icc.h"
#include "rtc.h"
#include "tmr_utils.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/

//Junk function to do simple calculations
void icc_test_function(void)
{
	unsigned int a, i;
    unsigned int j;
	
	for(a = 0; a < 99999; ++a) {
		for(i = 0; i < 100000; ++i) {
			j = (i*128/2)/4;
			j = 0;
		}
		if((a % 10000) == 0)
			printf("%d\n", a);
	}
	printf("\n");

	return;
}

//Start timer before junk function
void icc_start_timer(void)
{
	if(RTC_Init(MXC_RTC, 0, 0) != E_NO_ERROR) {
		printf("Failed setup_timer.\n");
		return;
	}
	RTC_EnableRTCE(MXC_RTC);

	return;
}

//Stop current timer and print elapsed time
void icc_stop_timer(void)
{
	printf("time elapsed: %d.%03d \n", MXC_RTC->sec, MXC_RTC->ssec);
	RTC_DisableRTCE(MXC_RTC);
	return;
}

// *****************************************************************************
int icc_main(void)
{
	printf("\n***** ICC Example *****\n");

	//Instruction cache enabled
	printf("\nWith instruction cache enabled:\n");
	ICC_Enable();
	icc_start_timer();
	icc_test_function(); //waste time
	icc_stop_timer();

	//Instruction cache disabled
	printf("\nWith instruction cache disabled:\n");
	ICC_Disable();
	icc_start_timer();
	icc_test_function(); //waste time
	icc_stop_timer();

	printf("\nExample complete.\n");
	return 0;
}
