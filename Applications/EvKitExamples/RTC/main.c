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
 * $Date: 2020-11-11 12:00:51 -0600 (Wed, 11 Nov 2020) $
 * $Revision: 56668 $
 *
 ******************************************************************************/

/**
 * @file        main.c
 * @brief       Configures and starts the RTC and demonstrates the use of the alarms.
 * @details     The RTC is enabled and the sub-second alarm set to trigger every 250 ms.
 *              P2.25 (LED0) is toggled each time the sub-second alarm triggers.  The
 *              time-of-day alarm is set to 2 seconds.  When the time-of-day alarm
 *              triggers, the rate of the sub-second alarm is switched to 500 ms.  The
 *              time-of-day alarm is then rearmed for another 2 sec.  Pressing SW2 will
 *              output the current value of the RTC to the console UART.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "board.h"
#include "rtc.h"
#include "led.h"
#include "pb.h"
#include "tmr_utils.h"
#include "simo_regs.h"

/***** Definitions *****/
#define LED_ALARM           1

#define TIME_OF_DAY_SEC     2
#define SUBSECOND_MSEC_0    250
#define SUBSECOND_MSEC_1    500

#define MSEC_TO_RSSA(x) (unsigned int)(0x100000000ULL    - ((x * 4096) / 1000)) /* Converts a time in milleseconds to the equivalent RSSA register value. */

#define SECS_PER_MIN        60
#define SECS_PER_HR         (60 * SECS_PER_MIN)
#define SECS_PER_DAY        (24 * SECS_PER_HR)

/***** Globals *****/
int ss_interval = SUBSECOND_MSEC_0;

/***** Functions *****/
void RTC_IRQHandler(void)
{
    int time;
    int flags = RTC_GetFlags();
    /* Check sub-second alarm flag. */
    if (flags & MXC_F_RTC_CTRL_ALSF) {
        LED_Toggle(LED_ALARM);
        RTC_ClearFlags(MXC_F_RTC_CTRL_ALSF);
    }
    
    /* Check time-of-day alarm flag. */
    if (flags & MXC_F_RTC_CTRL_ALDF) {
        RTC_ClearFlags(MXC_F_RTC_CTRL_ALDF);
        
        /* Set a new alarm 10 seconds from current time. */
        time = RTC_GetSecond();
        if (RTC_SetTimeofdayAlarm(MXC_RTC, time + TIME_OF_DAY_SEC) != E_NO_ERROR) {
            /* Handle Error */
        }
        
        // Toggle the sub-second alarm interval.
        if (ss_interval == SUBSECOND_MSEC_0) {
            ss_interval = SUBSECOND_MSEC_1;
        } else {
            ss_interval = SUBSECOND_MSEC_0;
        }
        
        if (RTC_SetSubsecondAlarm(MXC_RTC, MSEC_TO_RSSA(ss_interval)) != E_NO_ERROR) {
            /* Handle Error */
        }
    }
    
    return;
}

volatile int buttonPressed = 0;
void buttonHandler()
{
    buttonPressed = 1;
}

void printTime()
{
    int day, hr, min, sec;
    double subsec;
    
    subsec = RTC_GetSubSecond() / 4096.0;
    sec = RTC_GetSecond();
    
    day = sec / SECS_PER_DAY;
    sec -= day * SECS_PER_DAY;
    
    hr = sec / SECS_PER_HR;
    sec -= hr * SECS_PER_HR;
    
    min = sec / SECS_PER_MIN;
    sec -= min * SECS_PER_MIN;
    
    subsec += sec;
    
    printf("Current Time (dd:hh:mm:ss): %02d:%02d:%02d:%05.2f\n", day, hr, min, subsec);
}

/*************************************************************************************************/
/*!
 *  \fn     trimRTC
 *
 *  \brief  Trim the internal RTC load capacitors to increase the 32 kHz accuracy.
 *
 *  \param  None.
 *
 *  \return None.
 */
/*************************************************************************************************/
void trimRTC(void)
{
    int freq;

    /* Initialize VREGO_D and BTLELDOCN for 32 MHz crystal */
    MXC_SIMO->vrego_d = (0x50 << MXC_F_SIMO_VREGO_D_VSETD_POS);
    while(!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYD)) {}

    MXC_GCR->btleldocn = 0xD5;

    TMR_Delay(MXC_TMR0, MSEC(1000), NULL);

    /* Select the 32.00 MHz crystal oscillator (XO) as the system clock source */
    if (SYS_Clock_Select(SYS_CLOCK_XTAL32M, NULL) != E_NO_ERROR) {
        printf("Clock switch unsuccessful\n");
        return;
    } else {
        Console_Init();
    }

    sys_cfg_rtc_t sys_cfg = NULL; /*  No Specific system configuration object */
    if (RTC_Init(MXC_RTC, 0, 0, &sys_cfg) != E_NO_ERROR) {
        printf("Failed RTC_Setup().\n");
        return;
    }

    /* Clear out any previous configuration */
    RTC_DisableTimeofdayInterrupt(MXC_RTC);
    RTC_DisableSubsecondInterrupt(MXC_RTC);
    RTC_ClearFlags(RTC_GetFlags());

    if (RTC_EnableRTCE(MXC_RTC) != E_NO_ERROR) {
        printf("Failed RTC_EnableRTCE()\n");
        return;
    }

    /* Run the trim, save the freq */
    freq = RTC_LoadTrim();
    if(freq < 0) {
        printf("Failed to trim RTC %d\n", freq);
    } else {
        printf("RTC Trimmed to %d Hz\n", freq);
    }

    RTC_DisableRTCE(MXC_RTC);

    /* Wait for the UART to finish printing to the console */
    while(Console_PrepForSleep() != E_NO_ERROR) {}

    /* Restore the system clock source to the 96 MHz oscillator */
    if (SYS_Clock_Select(SYS_CLOCK_HIRC96, NULL) != E_NO_ERROR) {
        printf("Clock switch unsuccessful\n");
    } else {
        Console_Init();
    }

    /* Apply the freq */
    SYS_WUT_SetFreq(freq);
}

// *****************************************************************************
int main(void)
{
    sys_cfg_tmr_t sys_tmr_cfg = NULL;  /* No system specific settings needed for the timers. */
    
    printf("\n***** RTC Example *****\n\n");
    printf("The RTC is enabled and the sub-second alarm set to trigger every %d ms.\n", SUBSECOND_MSEC_0);
    printf("P1.14 (LED0) is toggled each time the sub-second alarm triggers.  The\n");
    printf("time-of-day alarm is set to %d seconds.  When the time-of-day alarm\n", TIME_OF_DAY_SEC);
    printf("triggers, the rate of the sub-second alarm is switched to %d ms.  The\n", SUBSECOND_MSEC_1);
    printf("time-of-day alarm is then rearmed for another %d sec.  Pressing SW2 will\n",TIME_OF_DAY_SEC);
    printf("output the current value of the RTC to the console UART.\n\n");

    trimRTC();
    
    NVIC_EnableIRQ(RTC_IRQn);
    
    /* Setup callback to receive notification of when button is pressed. */
    PB_RegisterCallback(0, (pb_callback)buttonHandler);
    
    /* Turn LED off initially */
    LED_Off(LED_ALARM);
    int error;
    sys_cfg_rtc_t sys_cfg = NULL; //No Specific system configuration object
    if ((error = RTC_Init(MXC_RTC, 0, 0, &sys_cfg)) != E_NO_ERROR) {
        printf("Failed RTC_Setup().   %d \n ", error);
        printf("Example Failed\n");
        while(1);
    }
    
    printf("RTC started.\n");
    printTime();
    
    if (RTC_SetTimeofdayAlarm(MXC_RTC, TIME_OF_DAY_SEC) != E_NO_ERROR) {
        printf("Failed RTC_SetTimeofdayAlarm().\n");
        printf("Example Failed\n");
        while(1);
    }
    
    if (RTC_SetSubsecondAlarm(MXC_RTC,  MSEC_TO_RSSA(SUBSECOND_MSEC_0)) != E_NO_ERROR) {
        printf("Failed RTC_SetSubsecondAlarm().\n");
        printf("Example Failed\n");
        while(1);
    }
    
    if (RTC_EnableRTCE(MXC_RTC) != E_NO_ERROR) {
        printf("Failed RTC_EnableRTCE().\n");
        printf("Example Failed\n");
        while(1);
    }
    
    while (1) {
        if (buttonPressed) {
            /* Show the time elapsed. */
            printTime();
            /* Delay for switch debouncing. */
            TMR_Delay(MXC_TMR0, MSEC(100), &sys_tmr_cfg);
            /* Re-arm switch detection. */
            buttonPressed = 0;
        }
    }
}
