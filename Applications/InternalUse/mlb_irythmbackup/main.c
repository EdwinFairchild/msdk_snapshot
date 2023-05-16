/* *****************************************************************************
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
 * $Date: 2018-10-15 16:49:29 -0500 (Mon, 15 Oct 2018) $
 * $Revision: 38520 $
 *
 **************************************************************************** */

#include <stdio.h>
#include <max32665.h>
#include <pwrseq_regs.h>
#include <tmr_utils.h>
#include "core_cm4.h"
#include "max32665.h"
#include "icc_regs.h"
#include "mxc_sys.h"
#include <gcr_regs.h>
#include "simo.h"
#include "rtc.h"
#include "nvic_table.h"
#include "pwrseq_regs.h"
#include "usbhs_regs.h"
#include "mcr_regs.h"
#include "gcr_regs.h"
#include "uart.h"
#include "lp.h"
#include "icc.h"
#include "board.h"
#include "pb.h"
#include "led.h"

#define BACKUP_FLAG 0x424B5550

volatile int backup_state = 0;
volatile uint32_t preinit_cnt = 0;
volatile int alarmed = 0;
volatile int wait_time = 5;
volatile int buttonPressed = 0;
extern uint32_t SystemCoreClock;
extern void Reset_Handler(void);

void turnOnConsole(void);
void switchToHIRC96M(void);
void setupRTC(void);

/*********************************************************************/
// These functions to be added to lp.c in the next release
typedef enum {
    RETAIN_NONE= MXC_S_PWRSEQ_LPCN_RAMRET_DIS,
    RETAIN_32k = MXC_S_PWRSEQ_LPCN_RAMRET_EN1,
    RETAIN_64k = MXC_S_PWRSEQ_LPCN_RAMRET_EN2,
    RETAIN_ALL = MXC_S_PWRSEQ_LPCN_RAMRET_EN3,
} ram_retained_t;

void LP_BandgapBackupModeEnable(void)
{
    MXC_PWRSEQ->lpcn &= ~MXC_F_PWRSEQ_LPCN_BGOFF; // Bandgap will remain on in backup mode
}

void LP_BandgapBackupModeDisable(void)
{
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF; // Bandgap will turn off in backup mode
}

void LP_SetRAMRetention(ram_retained_t ramRetained)
{
    MXC_SETFIELD(MXC_PWRSEQ->lpcn, MXC_F_PWRSEQ_LPCN_RAMRET, ramRetained);
}

void LP_DisableAllMonitors(void) 
{
    MXC_PWRSEQ->lpcn |= 0x3FE00000;
}

void LP_EnableAllMonitors(void) 
{
    MXC_PWRSEQ->lpcn &= ~0x3FE00000;
}
/*********************************************************************/
void alarmHandler(void)
{
    int flags = RTC_GetFlags();
    alarmed = 1;

    if (flags & MXC_F_RTC_CTRL_ALSF) {
        RTC_ClearFlags(MXC_F_RTC_CTRL_ALSF);
    }

    if (flags & MXC_F_RTC_CTRL_ALDF) {
        RTC_ClearFlags(MXC_F_RTC_CTRL_ALDF);
    }
}

// *****************************************************************************
void setTrigger(int waitForTrigger)
{
    alarmed = 0;
    while (RTC_Init(MXC_RTC, 0, 0, NULL) == E_BUSY);
    while(RTC_SetTimeofdayAlarm(MXC_RTC, wait_time) == E_BUSY);
    while(RTC_EnableRTCE(MXC_RTC) == E_BUSY);
    if (waitForTrigger) {
        while(!alarmed);
    }
}

void backup(void) {
    //Shutdown unused power domains
    LP_VDD3PowerDown();
    LP_VDD4PowerDown();
    LP_VDD5PowerDown();
    LP_SIMOVregDPowerDown();
    LP_SIMOVregBPowerDown();
    LP_DisableAllMonitors();

    // Turn off other features to save power in backup mode
    ICC_Disable();
    LP_ICacheShutdown();
    LP_BandgapBackupModeDisable();
    
    // Setup part for faster wakeup
    LP_SetRAMRetention(RETAIN_NONE);
    LP_FastWakeupEnable();
    
    // Lowering VREGC setpoint will reduce power used in backup
    SIMO_setVregO_C(850);

    MXC_PWRSEQ->gp1 = BACKUP_FLAG;
    LP_EnterBackupMode();
}

void turnOffConsole(void) {
    Console_Shutdown();
    MXC_GCR->perckcn0 |= MXC_F_GCR_PERCKCN0_UART0D;
}

void setupRTC(void) {
    //Set oscillator in 'low power' (aka not noise immune) mode

    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }

    MXC_RTC->ctrl |= MXC_F_RTC_CTRL_WE;  
    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }

    uint32_t ctrl_temp = MXC_RTC->ctrl;
    ctrl_temp &= ~MXC_F_RTC_CTRL_X32KMD;
    ctrl_temp |= MXC_S_RTC_CTRL_X32KMD_QUIETMODE;
    MXC_RTC->ctrl = ctrl_temp;
    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }

    MXC_RTC->ctrl &= ~MXC_F_RTC_CTRL_WE;  
    if (RTC_CheckBusy()) {
        printf("Error in setupRTC\n");
        while(1);
    }    
}

// *****************************************************************************
int main(void)
{
    uint32_t sec, ssec;
    if(MXC_PWRSEQ->gp1 == BACKUP_FLAG) {

    	/* We may not be running on the fast clock at this point
    	 * We'll need to raise the core's supply voltage (VregO_B) 
    	 * to a suitable setpoint and ensure that it reaches that
    	 * before we can make the switch to 96MHz.
    	 * Once we've switched, we need to reinitialize the Console UART 
    	 * as the baud rate generation settings are dependent on clock speed
    	 */
        SIMO_setVregO_C(1000);  // Restore the Core voltage to 1.0v
        while(!(MXC_SIMO->buck_out_ready & 0x2)); // Wait for the SIMO to reach the setpoint
        MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128); // Set the clock in divide by 1 mode
        SYS_Clock_Select(SYS_CLOCK_HIRC96, MXC_TMR0); // Switch to the 96MHz clock
        Console_Init(); // Adjust the baud rate settings to keep the console at 115200baud
        LP_EnableAllMonitors(); // Reenable the Voltage monitors we turned off before entering backup

        LP_ICacheWakeup();
        ICC_Enable();

        printf("Code Returned to Backup Restore Handler\n");
        
        while(RTC_GetTime(&sec, &ssec) != E_NO_ERROR);
        printf("The MAX32665 spent %d.%03d seconds in backup mode\n\n", sec-5, (int)(ssec/4.096));
        MXC_PWRSEQ->gp1 = 0;
    }
  
    printf("**** Low Power Example ****\n");
    printf("This example will go into backup mode\n");
    printf("using the pushbuttons as wakeup sources.\n");

    // Don't remove these lines
    printf("Starting test in 5 seconds\n");
    LP_EnableRTCAlarmWakeup();
    NVIC_SetVector(RTC_IRQn, alarmHandler);
    setTrigger(1);

    // Setup code entry point when restoring from backup mode
    // This defaults to the reset handler
    MXC_PWRSEQ->gp0 = (uint32_t)(&Reset_Handler) | 1;

    setupRTC(); // Puts RTC in lowest power state
    LP_EnableGPIOWakeup((gpio_cfg_t*)&pb_pin[0]);   // Enable either Pushbutton to wake from backup
    LP_EnableGPIOWakeup((gpio_cfg_t*)&pb_pin[1]);
    turnOffConsole();

    backup();
    
    printf("Returned to main()\n");
    while(1);
}
