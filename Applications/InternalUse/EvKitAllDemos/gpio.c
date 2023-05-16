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
 * @brief   	GPIO Example
 * @details     
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "board.h"
#include "gpio.h"
#include "tmr_utils.h"

/***** Definitions *****/
#define NUM_PINS

/***** Globals *****/
const unsigned int masks[NUM_PINS] = {PIN_24, PIN_25}; //P1.24 and P1.25 respectively
volatile int isr_check;

/***** Functions *****/
void gpio_isr(void *cbdata)
{
	isr_check = 1;
    return;
}

// *****************************************************************************
// Removed
/*int test1(void)
{
    int a;
    for(a = 0; a < 100; ++a) printf("\n");

	printf("\n***** GPIO Example *****\n");
	gpio_cfg_t gpio0, gpio1; //rename for clarity gpio1_24
	gpio0.gpio = gpio1.gpio = MXC_GPIO_GET_GPIO(1); //Port 1
    //Variables used for interrupt part of example
	gpio_int_mode_t int_mode;  //declare all variables at the top
	gpio_int_pol_t int_pol;

	//Using P1.24 as input, P1.25 as output
	printf("P1.25 output LOW to input P1.24\n");
	gpio0.mask = masks[0]; //PIN_24
	gpio0.pad = GPIO_PAD_HI_Z;
	gpio0.func = GPIO_FUNC_IN;
	GPIO_Config(&gpio0);

	gpio1.mask = masks[1]; //PIN_25
	gpio1.pad = GPIO_PAD_HI_Z;
	gpio1.func = GPIO_FUNC_OUT;
	GPIO_Config(&gpio1);

	printf("Output HIGH\n");
	GPIO_OutPut(&gpio1, masks[1]);

	printf("Expected 0x%08x, got 0x%08x\n", masks[0], (GPIO_InGet(&gpio0) & masks[0])); //don't hide inside printf (use a variable)

	printf("Output LOW\n");
	GPIO_OutPut(&gpio1, 0);

	printf("Expected 0x%08x, got 0x%08x\n", 0, (GPIO_InGet(&gpio0) & masks[0]));

	//Using interrupts: catch HIGH to LOW transistion
	printf("\nTesting falling edge sensitive interrupt.\n");
	int_mode = GPIO_INT_EDGE; //Interrupt is edge sensitive
	int_pol = GPIO_INT_FALLING; //Interrupt triggers on falling edge

	GPIO_RegisterCallback(&gpio0, gpio_isr, &gpio0);
	GPIO_IntConfig(&gpio0, int_mode, int_pol);
	NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(gpio0.gpio))); //does this line get called by line 105? check

	isr_check = 0;
	GPIO_IntEnable(&gpio0);
	//4 SPACES NOT TABS 
	printf("Output LOW: ");
	GPIO_OutPut(&gpio1, 0);

	if(isr_check == 0) {
		printf("No interrupt detected.\n");
	} else {
		printf("Interrupt detected.\n"); //Should not happen
	}
	
	printf("Output HIGH: ");
	GPIO_OutPut(&gpio1, masks[1]);
	
	if(isr_check == 0) {
		printf("No interrupt detected.\n");
	} else {
		printf("Interrupt detected.\n"); //Should not happen
	}

	printf("Output LOW: ");
	GPIO_OutPut(&gpio1, 0);

	//Interrupt should now be active
	if(isr_check == 0) {
		printf("No interrupt detected.\n"); //Should not happen
	} else {
		printf("Interrupt detected.\n");
	}

	GPIO_IntDisable(&gpio0);

	printf("\nExample complete.\n");
	return 0;
}*/

int gpio_main(void)
{
    int a;
    for(a = 0; a < 100; ++a) printf("\n");

	printf("\n***** GPIO Example *****\n");
	gpio_cfg_t gpio0, gpio1; //rename for clarity gpio1_24
	gpio0.gpio = gpio1.gpio = MXC_GPIO_GET_GPIO(1); //Port 1
    //Variables used for interrupt part of example
	// gpio_int_mode_t int_mode; //declare all variables at the top
	// gpio_int_pol_t int_pol;

	//Using P1.24 as input, P1.25 as output
	printf("P1.25 output LOW to input P1.24\n");
	gpio0.mask = masks[0]; //PIN_24
	gpio0.pad = GPIO_PAD_HI_Z;
	gpio0.func = GPIO_FUNC_IN;
	GPIO_Config(&gpio0);

	gpio1.mask = masks[1]; //PIN_25
	gpio1.pad = GPIO_PAD_PULL_DOWN;
	gpio1.func = GPIO_FUNC_OUT;
	GPIO_Config(&gpio1);

    while(1){
        int in = GPIO_InGet(&gpio0) & masks[0];
        if(in == masks[0]){
            GPIO_OutPut(&gpio1, 0);
            printf("hooyah\n");
        }else{
            GPIO_OutPut(&gpio1, masks[1]);
            printf("rip\n");
        }
    }

	return 0;
}
