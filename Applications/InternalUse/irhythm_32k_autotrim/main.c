/**
 * @file        main.c
 * @brief       32kHz XTAL internal capacitor self-trim routine
 * @details     The RTC is enabled and the sub-second alarm set to trigger every 200 ms.
 *              The 32MHz crystal is enabled and used as the system clock. This is the
 *              clock for Timer 3 in Continous Mode. On every sub-second alarm, the
 *              Timer 3 count value is used in a binary search for the optimal 32kHz
 *              internal capacitor trim value.
 *              
 *              For 200ms (the wider, the more accurate decision), using the sub-second
 *              counter, the window of time is 819 sub-seconds (1 sub-second = 
 *              1 4096 cycle = 8 32kHz cycles) or 199.951172... divided by 32MHz/2 (PCLK)
 *              cycles or 62.5ns is 0x30d0f2 expected cycle count. This is the number
 *              used in the binary search to determine if it's too slow.
 *
 */

/*******************************************************************************
 * Copyright (C) 2019 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-06-04 18:17:28 -0500 (Tue, 04 Jun 2019) $
 * $Revision: 43670 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "nvic_table.h"
#include "board.h"
#include "rtc_regs.h"
#include "rtc.h"
#include "mxc_sys.h"
#include "mcr_regs.h"
#include "pwrseq_regs.h"
#include "led.h"
#include "pb.h"
#include "tmr.h"
#include "tmr_utils.h"

/***** Definitions *****/
#define LED_ALARM           1
#define DEBUG_ME

#define SUBSECOND_MSEC_0    200
#define SEARCH_STEPS        6
#define CORRECTION_COUNTS   0xc30  /* Accounts for algo overhead, xtal offset, start uncertainty, and affects final accuracy */
#define SEARCH_TARGET       0x30d400  /* 1/2 of 32 MHz periods in 32.768 kHz */

/* Converts a time in milleseconds to the equivalent RSSA register value. */
#define MSEC_TO_RSSA(x) (unsigned int)(0x100000000ULL    - ((x * 4096) / 1000))

#define SECS_PER_MIN        60
#define SECS_PER_HR         (60 * SECS_PER_MIN)
#define SECS_PER_DAY        (24 * SECS_PER_HR)

#define MXC_BASE_TEST       ((uint32_t)0x40000C00UL)
#define MXC_BASE_BBSIR      ((uint32_t)0x40005400UL)
#define RTCX1x_MASK         0x1F /* 5 bits */       
#define RTCX1x_SHIFT        4
#define RTCX2x_MASK         0x1F /* 5 bits */
#define RTCX2x_SHIFT        9

/***** Globals *****/
#ifdef DEBUG_ME
volatile struct {
    unsigned int want;
    unsigned int elapsed;
    unsigned int lower;
    unsigned int upper;
    unsigned int trim;
    unsigned int path;
} record[SEARCH_STEPS];
#endif

int trim_32k(void);

/***** Functions *****/
void write_bb_sir6(unsigned int rtcx1_, unsigned int rtcx2_)
{
  uint32_t *bb_sir6 = (uint32_t *)(MXC_BASE_BBSIR+0x00000018UL);

  /* Clear old values */
  *bb_sir6 &= ~((RTCX1x_MASK << RTCX1x_SHIFT) | (RTCX2x_MASK << RTCX2x_SHIFT));

  /* Insert new values, DO NOT ALTER ANY OTHER BITS WITHIN BB-SIR6 */
  *bb_sir6 |= ((rtcx1_ & RTCX1x_MASK) << RTCX1x_SHIFT) | ((rtcx2_ & RTCX2x_MASK) << RTCX2x_SHIFT);
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

// *****************************************************************************
int main(void)
{
    unsigned int i;
    volatile uint32_t *test_tmr = (uint32_t *)MXC_BASE_TEST;

    printf("\n***** RTC 32kHz Internal Capacitor Trim Program *****\n\n");
    printf("This program will use the 32.00 MHz crystal oscillator to trim\n");
    printf("the 32.768 kHz oscillator\n\n");
        
    /* Turn LED off initially */
    LED_Off(LED_ALARM);

    /* Safety delay */
    printf("Switching to 32 MHz XO in 1 second\n");
    TMR_Delay(MXC_TMR0, MSEC(1000), 0);
    LED_Toggle(LED_ALARM);

    /* Change the pullup on the RST pin to 25K */
    MXC_MCR->ctrl = 0x202;

    /* Set VREGO_D to 1.3V */
    *((volatile uint32_t*)0x40004410) = 0x50;

    /* Set TX LDO to 1.1V and enable LDO. Set RX LDO to 0.9V and enable LDO */
    MXC_GCR->btleldocn = 0xD9; // TX 1.1V RX 0.9V

    /* Select the 32.00 MHz crystal oscillator (XO) as the system clock source */
    if (SYS_Clock_Select(SYS_CLOCK_XTAL32M, NULL) != E_NO_ERROR) {
      printf("FAIL: Clock switch unsuccessful\n");
      while (1);
    } else {
      Console_Init();
      printf("Clock switch complete\n");
    }

    /* Set Test Mode Enable */
    *test_tmr |= 1;

    sys_cfg_rtc_t sys_cfg = NULL; //No Specific system configuration object
    if (RTC_Init(MXC_RTC, 0, 0, &sys_cfg) != E_NO_ERROR) {
        printf("Failed RTC_Setup().\n");
        printf("Example Failed\n");
        while(1);
    }

    /* Clear out any previous configuration */
    RTC_DisableTimeofdayInterrupt(MXC_RTC);
    RTC_DisableSubsecondInterrupt(MXC_RTC);
    RTC_ClearFlags(RTC_GetFlags());
    
    /* Enable 4.096 kHz output */
    MXC_RTC->oscctrl |= MXC_F_RTC_OSCCTRL_32KOUT;
    MXC_MCR->outen |= MXC_F_MCR_OUTEN_SQWOUT1EN; /* Use P0.26 -- SQWOUT0EN is P0.19, if desired */
    MXC_RTC->ctrl |= MXC_S_RTC_CTRL_FT_FREQ4KHZ | MXC_F_RTC_CTRL_SQE;

    if (RTC_EnableRTCE(MXC_RTC) != E_NO_ERROR) {
        printf("Failed RTC_EnableRTCE().\n");
        while(1);
    }

    printf("RTC started.\n");
    printTime();

#ifdef DEBUG_ME
    bzero((void *)record, sizeof(record));
#endif
      
    i = trim_32k();

    printf("Calibration complete. rtcx1 = 0x%02x, rtcx2 = 0x%02x\n\n", i, i);

#ifdef DEBUG_ME
    printf("N: want       elapsed    low  high trim\n");
    printf("---------------------------------------\n");
    for (i = 0; i < SEARCH_STEPS; i++) {
	printf("%d: 0x%08x 0x%08x 0x%02x 0x%02x 0x%02x %u\n", i,
	       record[i].want, record[i].elapsed,
	       record[i].lower, record[i].upper, record[i].trim, record[i].path); 
    }
#endif
}

int trim_32k(void)
{
  unsigned int flags, search_step;
  unsigned int elapsed;
  tmr_cfg_t tmr_config = { .mode = TMR_MODE_CONTINUOUS, .cmp_cnt = 0xffffffff, .pol = 0 };
  unsigned int upper, lower, trim, oldtrim;

  /* Determine starting point for internal load capacitors */
  upper = RTCX1x_MASK;
  lower = 0;
  trim = (upper + lower) / 2;

  printf("Initial trim 0x%02x\n", trim);
  
  /* Init timer to count 32 MHz periods */
  TMR_Init(MXC_TMR3, TMR_PRES_1, NULL);
  TMR_Config(MXC_TMR3, &tmr_config);

  /* Trim loop */
  search_step = 0;
  while (search_step < SEARCH_STEPS) {
    /* Set new trim point */
    oldtrim = trim;
    trim = (lower + upper) / 2;
    if ((search_step > 0) && (trim == oldtrim)) {
      /* Found trim value */
      break;
    }
    write_bb_sir6(trim, trim);

    /* Sleep to settle new caps */
    TMR_Delay(MXC_TMR0, MSEC(10), 0);

    /* Start 200 msec sampling window */
    TMR_Disable(MXC_TMR3);
    TMR_SetCount(MXC_TMR3, 0);
    if (RTC_SetSubsecondAlarm(MXC_RTC, MSEC_TO_RSSA(SUBSECOND_MSEC_0)) != E_NO_ERROR) {
      printf("Failed RTC_SetSubsecondAlarm().\n");
      while(1);
    }
    if (RTC_CheckBusy() != E_NO_ERROR) {
      printf("Synchronization timed out.\n");
      while(1);
    }
    TMR_Enable(MXC_TMR3);
    RTC_ClearFlags(RTC_GetFlags());

    flags = RTC_GetFlags();
    while (!(flags & MXC_F_RTC_CTRL_ALSF)) {
      flags = RTC_GetFlags();
    }

    elapsed = TMR_GetCount(MXC_TMR3);
    TMR_Disable(MXC_TMR3);

    elapsed -= CORRECTION_COUNTS;
    /* Binary search for optimal trim value */
    if (elapsed > SEARCH_TARGET) {
      /* Too slow */
      upper = trim;
#ifdef DEBUG_ME
      record[search_step].path = 0;
#endif
    } else {
      /* Too fast */
      lower = trim;
#ifdef DEBUG_ME
      record[search_step].path = 1;
#endif
    }

#ifdef DEBUG_ME
    record[search_step].want = SEARCH_TARGET;
    record[search_step].elapsed = elapsed;
    record[search_step].upper = upper;
    record[search_step].lower = lower;
    record[search_step].trim = trim;
#endif

    search_step++;
  }

  return trim;
}
