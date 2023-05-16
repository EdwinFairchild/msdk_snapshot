/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2021-08-03 10:16:23 -0500 (Tue, 03 Aug 2021) $
 * $Revision: 58927 $
 *
 ******************************************************************************/

#include "lp.h"
#include "icc.h"
#include "simo.h"
#include "board.h"
#include "wut_regs.h"
#include "wsf_types.h"
#include "wsf_timer.h"
#include "usbhs_regs.h"
#include "gcr_regs.h"
#include "mcr_regs.h"
#include "tmr_regs.h"
#include "wut.h"
#include "mxc_sys.h"
#include "bb_drv.h"
#include "wakeup.h"
#include "led.h"

uint32_t wut_count;
uint32_t wut_snapshot;

#define ENABLE_BACKGROUND_HARDFAULT_WORKAROUND

#define MXC_R_SIR_SHR13         *((uint32_t*)(0x40005434))
#define MXC_R_SIR_SHR17         *((uint32_t*)(0x40005444))

int bHaveWUTEvent = 0;

/* Assembly functions in backup.S */
extern void Backup_Init(void);
extern void Backup_Setup(void);

/*************************************************/
/* Arm WUT for wakeup from Sleep */
void WUT_SetInt(uint32_t sleep_time)
{
    uint32_t targetTick;

    /* arm WUT for wakeup */
    targetTick = WUT_GetCount();
    targetTick +=
            ((uint64_t) (sleep_time) * SYS_WUT_GetFreq() / BB_CLK_RATE_HZ);
    WUT_SetCompare(targetTick);
}

/*************************************************/
/* Arm WUT for wakeup from Sleep, save BB clock */
void WUT_SetWakeup(uint32_t sleep_time)
{
    /* save clocks */
    WUT_Edge();
    WUT_StoreSnap(BbDrvGetCurrentTime());
    /* arm WUT for wakeup */
    WUT_SetInt(sleep_time);
}

/*************************************************/
/* Sleep for the given number of us */
void WUT_sleepUs(uint32_t us)
{
    uint32_t targetTick, wakeDiv;

    targetTick = WUT_GetCount();
    targetTick += ((uint64_t) (us) * SYS_WUT_GetFreq() / 1000000);
    WUT_SetCompare(targetTick);

    /* Save the current divider setting */
    wakeDiv = MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC;

    /* Divide the system clock while sleeping */
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV4);

    LED_On(0);
    LP_EnterSleepMode();
    LED_Off(0);

    /* Restore system clock */
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, wakeDiv);
}

/*************************************************/
/*  This will calculate extra delay needed to power up
    after WAIT_TICKS long Sleep
    Input and output are in DBB_TICK units (0.625us per tick)
    This timing depends on how fast SIMO voltages decay/restore,
    i.e. must be characterized per board design

    For now use data points for EvKit_V1 board:
      1) Sleep time <250ms -> extra delay 0us
      2) Sleep time <750ms -> extra delay 45us
      3) Sleep time <2 sec -> extra delay 75us
      4) Sleep time <5 sec -> extra delay 105us
      5) Sleep time >5 sec -> extra delay 150us

*/
uint32_t GetWakeDelay(uint32_t wait_ticks)
{
    uint32_t ret;

    if (wait_ticks < US_TO_BBTICKS(250000))
        ret = 0;
    else if (wait_ticks < US_TO_BBTICKS(750000))
        ret = US_TO_BBTICKS(45);
    else if (wait_ticks < US_TO_BBTICKS(2000000))
        ret = US_TO_BBTICKS(75);
    else if (wait_ticks < US_TO_BBTICKS(5000000))
        ret = US_TO_BBTICKS(105);
    else
        ret = US_TO_BBTICKS(150);

    ret += US_TO_BBTICKS(DS_WAKEUP_TIME_US);
    return ret;
}

/*************************************************/
/* This will switch system clock to HIRC 60MHz source */
void switchToHIRC(void)
{
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV1);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL,
            MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    /* Disable unused clocks */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));  /* Wait for the switch to occur */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);
    SystemCoreClockUpdate();
}

/*************************************************/
/* This will switch system clock to HIRC 60MHz source */
void switchToHIRCD4(void)
{
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV4);

    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL,
            MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    /* Disable unused clocks */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));  /* Wait for the switch to occur */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);
    SystemCoreClockUpdate();
}

/*************************************************/
/* This will switch system clock to 96MHz source */
void switchTo96M(void)
{
    MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
#ifdef ENABLE_SDMA
    MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV1;
#else
    MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV2;
#endif
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL,
            MXC_S_GCR_CLKCN_CLKSEL_HIRC96);

    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));  /* Wait for the switch to occur */

    /* Disable unused clocks */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC_EN);
    SystemCoreClockUpdate();
}

/*************************************************/
/* This will power off unused hardware */
void DisableUnused(void)
{
    /* Prevent SIMO leakage in DS */
    if (MXC_GCR->revision == 0xA2) {
        MXC_R_SIR_SHR13 = 0x0;
        MXC_R_SIR_SHR17 &= ~(0xC0);
    } else if (MXC_GCR->revision == 0xA4) {
        //MXC_R_SIR_SHR13 = 0x0;
        MXC_R_SIR_SHR17 &= ~(0xC0);
    }

    /* Allow the USB Switch to be turned off in deepsleep and backup modes */
    LP_USBSWLPDisable();

    /* Shut down all unneeded power gates */
    LP_ICacheXIPShutdown();
    LP_CryptoShutdown();
    LP_SysCacheShutdown();
    LP_USBFIFOShutdown();
    LP_ROMShutdown();
    LP_ROM1Shutdown();
    LP_ICache1Shutdown();
}

/*************************************************/
void BackupMode(void)
{
    LP_ClearWakeStatus();

    /* Assembly function in backup.S called when coming out of backup mode */
    MXC_PWRSEQ->buretvec = (uint32_t) (&Backup_Init) | 1;

    // Enable the VDDCSW to ensure we have enough power to start
    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;

    // Enable backup mode
    MXC_GCR->pm &= ~MXC_F_GCR_PM_MODE;

    /* Calls into assembly function in backup.S */
    Backup_Setup();

    /* Executed out of Backup Mode */
    SystemInit();

    /* Enable WUT interrupt */
    MXC_GCR->pm |= MXC_F_GCR_PM_WUTWKEN;
    NVIC_EnableIRQ(WUT_IRQn);

    /* power off unused hardware */
    DisableUnused();
}

/*************************************************/
/* @brief This will enable wakeup from WUT and GPIO (if required).
 * @param bNeedExclusive Set if non WUT wakeup sources need to be disabled. */
static void SetWakeupSources(bool_t bNeedExclusive)
{
    uint32_t u32PMVal;

    // disable other wakeups (if requested)
    u32PMVal = MXC_GCR->pm;
    if (bNeedExclusive) {
        u32PMVal &= ~(MXC_F_GCR_PM_GPIOWKEN |
                MXC_F_GCR_PM_RTCWKEN |
                MXC_F_GCR_PM_WUTWKEN |
                MXC_F_GCR_PM_USBWKEN |
                MXC_F_GCR_PM_SDMAWKEN);
    }

    // enable WUT wakeup.
    u32PMVal |= MXC_F_GCR_PM_WUTWKEN;

    MXC_GCR->pm = u32PMVal;
}

/*************************************************/
/* This will enter DeepSleep Mode */
void EnterDeepsleep(void)
{
    // Enable WUT wakeup
    SetWakeupSources(TRUE);

    /*Shutdown unused power domains */
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF;

    if (MXC_GCR->revision == 0xA2) {
        MXC_R_SIR_SHR13 = 0x0;
    }
    if (MXC_GCR->revision != 0xA1) {
        MXC_R_SIR_SHR17 &= ~(0xC0);
    }

    ICC_Disable();
    LP_ICacheShutdown();

    LP_VDD3PowerDown();
    LP_VDD4PowerDown();

    LP_FastWakeupEnable();

    BbDrvDisable();

    /* Retain all SRAM */
    MXC_PWRSEQ->lpcn |= (MXC_S_PWRSEQ_LPCN_RAMRET_EN3);

    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;
    switchToHIRCD4();
    SIMO_setVregO_B(DS_VOLTAGE);

#if (BACKUP_MODE==1)
    BackupMode();
#elsif defined(NORMAL_SLEEP)
    LP_EnterSleepMode();
#else
    LP_EnterDeepSleepMode();
#endif
}

/*************************************************/
/* This will enter DeepSleep Mode */
void EnterDeepsleepSDMA(void)
{
#ifndef NORMAL_SLEEP
    // Enable WUT wakeup
    SetWakeupSources(TRUE);

    //Shutdown unused power domains
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF;

    if (MXC_GCR->revision == 0xA2) {
        MXC_R_SIR_SHR13 = 0x0;
    }
    if (MXC_GCR->revision != 0xA1) {
        MXC_R_SIR_SHR17 &= ~(0xC0);
    }

    ICC_Disable();
    LP_ICacheShutdown();

    LP_VDD3PowerDown();
    LP_VDD4PowerDown();

    LP_FastWakeupEnable();

    // Retain all SRAM
    MXC_PWRSEQ->lpcn |= (MXC_S_PWRSEQ_LPCN_RAMRET_EN3);

    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;
    switchToHIRCD4();
    SIMO_setVregO_B(DS_VOLTAGE);
#endif /* NORMAL_SLEEP */

#ifdef NORMAL_SLEEP
    LP_EnterSleepMode();
#else /* NORMAL_SLEEP */
    LP_EnterDeepSleepMode();
#endif /* NORMAL_SLEEP */
}

/*************************************************/
/* This will enter DeepSleep Mode */
void EnterBackground(void)
{
#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    // Enable WUT wakeup
    SetWakeupSources(TRUE);
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    //Shutdown unused power domains
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF;

    ICC_Disable();
    LP_ICacheShutdown();

    LP_VDD3PowerDown();
    LP_VDD4PowerDown();

    LP_FastWakeupEnable();

    // Retain all SRAM
    MXC_PWRSEQ->lpcn |= (MXC_S_PWRSEQ_LPCN_RAMRET_EN3);

    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;

    while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYD)) {
    }
    SIMO_setVregO_B(BG_VOLTAGE);
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifdef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    LP_EnterSleepMode();
#else /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */
    LP_EnterBackgroundMode();
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */
}

/*************************************************/
/* this will restore from DeepSleep Mode */
void ExitBackup(void)
{
    WUT_IntClear();
    NVIC_ClearPendingIRQ(WUT_IRQn);

    MXC_PWRSEQ->lpcn &= ~0x3FE00000;      /* Disable voltage Monitors for unused rails */

    MXC_GCR->pm |= MXC_F_GCR_PM_WUTWKEN;

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BCKGRND;
    LP_ICacheWakeup();
    ICC_Enable();

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &=
            ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_TRNGD);

    BbDrvEnable();

    switchToHIRC();
}

/*************************************************/
/* this will restore from DeepSleep Mode */
void ExitDeepsleep(void)
{
    /* Restore the core voltage before restoring the system clock */
    SIMO_setVregO_B(RUN_VOLTAGE);
    while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB)) {
    }

    /* Move VCORE switch back to VCOREB */
    MXC_MCR->ctrl = (MXC_MCR->ctrl & ~(MXC_F_MCR_CTRL_VDDCSW)) |
            (0x1 << MXC_F_MCR_CTRL_VDDCSW_POS);

#if (BACKUP_MODE==1)
    ExitBackup();
    return;
#endif

    //MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VREGODPD;

    MXC_PWRSEQ->lpcn &= ~0x3FE00000;      /* Disable voltage Monitors for unused rails */

    // Enable WUT wakeup
    SetWakeupSources(FALSE);

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BCKGRND;
    LP_ICacheWakeup();
    ICC_Enable();

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &=
            ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_TRNGD);

    BbDrvEnable();

    switchToHIRC();
}

/*************************************************/
/* this will restore from DeepSleep Mode */
void ExitDeepsleepSDMA(void)
{
#ifndef NORMAL_SLEEP
    uint32_t buckoutFlag;

    /* Restore the core voltage before restoring the system clock */
    SIMO_setVregO_B(RUN_VOLTAGE);
    buckoutFlag = MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB;
    do {
        buckoutFlag &= ~(MXC_SIMO->buck_out_ready & buckoutFlag);
    } while (buckoutFlag);

    /* Move VCORE switch back to VCOREB */
    MXC_MCR->ctrl = (MXC_MCR->ctrl & ~(MXC_F_MCR_CTRL_VDDCSW)) |
            (0x1 << MXC_F_MCR_CTRL_VDDCSW_POS);

    WUT_IntClear();
    NVIC_ClearPendingIRQ(WUT_IRQn);

    MXC_PWRSEQ->lpcn &= ~0x3FE00000;      // Disable voltage Monitors for unused rails

    // Enable WUT wakeup
    SetWakeupSources(FALSE);

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    MXC_PWRSEQ->lpcn &= ~MXC_F_PWRSEQ_LPCN_BCKGRND;
    LP_ICacheWakeup();
    ICC_Enable();

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &=
            ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_TRNGD | MXC_F_GCR_PERCKCN1_BTLED);

    switchToHIRC();
#endif /* NORMAL_SLEEP */
}

/*************************************************/
/* this will restore from DeepSleep Mode */
void ExitBackground(void)
{
#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    uint32_t buckoutFlag;
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    /* Restore the core voltage before restoring the system clock */
    SIMO_setVregO_B(RUN_VOLTAGE);
    buckoutFlag = MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB;
    do {
        buckoutFlag &= ~(MXC_SIMO->buck_out_ready & buckoutFlag);
    } while (buckoutFlag);
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

    WUT_IntClear();
    NVIC_ClearPendingIRQ(WUT_IRQn);

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    MXC_PWRSEQ->lpcn &= ~0x3FE00000;      // Disable voltage Monitors for unused rails
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    // Enable WUT wakeup
    SetWakeupSources(FALSE);
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BCKGRND;
    LP_ICacheWakeup();
    ICC_Enable();

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &=
            ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_TRNGD | MXC_F_GCR_PERCKCN1_BTLED);
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */

#ifndef ENABLE_BACKGROUND_HARDFAULT_WORKAROUND
    switchToHIRC();
#endif /* ENABLE_BACKGROUND_HARDFAULT_WORKAROUND */
}
