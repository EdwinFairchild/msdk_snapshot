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
 * @brief   	Configures and starts the RTC with LED alarms
 * @details	    RTC set to 1 sec ticks with alarms at 3 sec and 5 sec
 *		
 * 		        LED0 - Turns on when alarm 0 value is reached (3 sec)
 * 		        LED0 - Turns on when alarm 0 value is reached (3 sec)
 * 		        LED0 - Turns on when alarm 0 value is reached (3 sec)
 * 		
 * 		        SW1 - Turns on when alarm 0 value is reached (3 sec)
 * 		        SW2 - Turns on when alarm 0 value is reached (3 sec)
 *
 *		        LEDs were connected to GPIO pins (P0.6, P0.7, and P1.4)
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "board.h"
#include "rtc.h"
#include "led.h"
#include "tmr_utils.h"
#include "pb.h"

/***** Definitions *****/
#define SW1		    0
#define SW2		    1

#define NUM_LEDS	1	
#define LED_Alarm0	0

#define ALARM0_SEC	100	
#define SNOOZE_SEC	5	

/***** Globals *****/
volatile int alarm0_flag = 1;
volatile int isr_check;

const unsigned int pin[NUM_LEDS] = {PIN_2, PIN_3, PIN_24}; //P0.6, P0.7, and P1.4 respectively

/***** Functions *****/
//Interrupt handler for start up procedure. If pressed after the first 5 seconds, should do nothing
void RTC_IRQHandler(void)
{
	int flags = MXC_RTC->cn;

	if((flags & MXC_F_RTC_CN_ALSF)>>MXC_F_RTC_CN_ALSF_POS){
		alarm0_flag = -1;
		MXC_RTC->cn &= ~(MXC_F_RTC_CN_ALSF);
	}

	if((flags & MXC_F_RTC_CN_ALDF)>>MXC_F_RTC_CN_ALDF_POS){
		alarm0_flag = 0;
		MXC_RTC->cn &= ~(MXC_F_RTC_CN_ALDF);
	}

	return;
}

void PB_Handler_ResetRTC(void *pb_num)
{
	printf("RTC Reset\n");
	//Turn off Alarm LEDs
	LED_Off(LED_Alarm0);

	//Reset 
	printf("%d\n", MXC_RTC->sec);
	RTC_Init(MXC_RTC, 0x0000, 0x0000);
	RTC_SetTimeofdayAlarm(MXC_RTC, ALARM0_SEC);
	RTC_EnableRTCE(MXC_RTC);
	printf("%d\n", MXC_RTC->sec);

	return;
}

void PB_Handler_SetSnooze(void *pb_num)
{
	printf("Snooze.\n");
	RTC_DisableRTCE(MXC_RTC);
	TMR_Delay(MXC_TMR0, SEC(SNOOZE_SEC));
	RTC_EnableRTCE(MXC_RTC);

	printf("End snooze.\n");
	return;
}

void start_RTC_Alarm0(void)
{
	RTC_SetTimeofdayAlarm(MXC_RTC, ALARM0_SEC); 
	TMR_Delay(MXC_TMR0, USEC(200));

	RTC_EnableRTCE(MXC_RTC);

	return;
}

// *****************************************************************************
int rtc_main(void)
{
    int i = 0;
    for(i = 0; i < 100; ++i) printf("\n");
	printf("\n***** RTC Example *****\n");
	printf("LED0 = Turns on when alarm 0 value is reached (%d secs)\n", ALARM0_SEC);
	printf("SW1  = Push to reset the RTC count to 0 and reset alarm interrupts\n");
	printf("SW2  = Push to set snooze for alarm 0,  alarm 0 = current time + %d sec\n", SNOOZE_SEC);

	NVIC_EnableIRQ(RTC_IRQn);

	LED_Off(LED_Alarm0);

	PB_RegisterCallback(SW1, PB_Handler_ResetRTC);
	PB_RegisterCallback(SW2, PB_Handler_SetSnooze);	

	if(RTC_Init(MXC_RTC, 0x00000,0x0000) != E_NO_ERROR) {
		printf("Failed RTC_Setup().\n");
		return -1;
	}

	start_RTC_Alarm0();

    //Wait for interrupt to trigger
	while(alarm0_flag == 1) {}
	LED_On(LED_Alarm0);

	printf("\nExample complete.\n");

	return 0;
}
