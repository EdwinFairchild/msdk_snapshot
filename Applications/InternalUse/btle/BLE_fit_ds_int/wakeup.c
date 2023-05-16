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
 * $Date: 2021-09-10 16:36:02 -0500 (Fri, 10 Sep 2021) $
 * $Revision: 59692 $
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

#define WAKEUP_32M_US           1600
#define MXC_R_SIR_SHR13         *((uint32_t*)(0x40005434))
#define MXC_R_SIR_SHR17         *((uint32_t*)(0x40005444))

int bLastUserLevel = 0;
int bHaveUserEvent = 0;

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
    targetTick += ((uint64_t)(sleep_time) * SYS_WUT_GetFreq() / BB_CLK_RATE_HZ);
    WUT_SetCompare(targetTick);
}

/*************************************************/
/* Arm WUT for wakeup from Sleep, save BB clock */
void WUT_SetWakeup(uint32_t sleep_time)
{
    /* save clocks */
    WUT_Edge();
    WUT_Store();
    /* arm WUT for wakeup */
    WUT_SetInt(sleep_time);
}

/*************************************************/
/* Sleep for the given number of us */
void WUT_sleepUs(uint32_t us)
{
    uint32_t targetTick;

    targetTick = WUT_GetCount();
    targetTick += ((uint64_t)(us) * SYS_WUT_GetFreq() / 1000000);
    WUT_SetCompare(targetTick);

	/* Divide system clock to save power while sleeping */
    LP_EnterSleepMode();
}

/*************************************************/
/*  This will calculate extra delay needed to power up
    after WAIT_TICKS long Sleep
    Input and output are in DBB_TICK units (0.625us per tick)
    This timing depends on how fast SIMO voltages decay/restore,
    i.e. must be characterized per board design

    For now use data points for EvKit_V1 board:
      1) Sleep time ~125ms -> power up delay ~0.5ms
      2) Sleep time ~500ms -> power up delay ~1.5ms
      3) Sleep time ~2 sec -> power up delay ~3.5ms
      4) Sleep time ~8 sec -> power up delay ~5.5ms

    Adding time to power 32 MHz crystal
*/
uint32_t GetWakeDelay(uint32_t wait_ticks)
{
    uint32_t ret;

    if (wait_ticks < US_TO_BBTICKS(125000))
        ret = US_TO_BBTICKS(500);
    else if (wait_ticks < US_TO_BBTICKS(500000))
        ret = US_TO_BBTICKS(1500);
    else if (wait_ticks < US_TO_BBTICKS(2000000))
        ret = US_TO_BBTICKS(3500);
    else
        ret = US_TO_BBTICKS(5500);

    ret += US_TO_BBTICKS(WAKEUP_32M_US);

    return ret;
}

/*************************************************/
/* This will switch system clock to HIRC 60MHz source */
void switchToHIRC(void)
{
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV1);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    
    /* Wait for the switch to occur */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));

    /* Disable unused clocks */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);
    SystemCoreClockUpdate();
}

/*************************************************/
/* This will switch system clock to HIRC 60MHz source */
void switchToHIRCD4(void)
{
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV4);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);

    /* Wait for the switch to occur */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));

    /* Disable unused clocks */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC96M_EN);
    SystemCoreClockUpdate();
}

/*************************************************/
/* This will switch system clock to 96MHz source */
void switchTo96M(void)
{
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV2);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC96);

    /* Wait for the switch to occur */
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));

    /* Disable unused clocks */
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC_EN);
    SystemCoreClockUpdate();
}

/*************************************************/
/* This will power off unused hardware */
void DisableUnused(void)
{
    /* Prevent SIMO leakage in DS */
    if(MXC_GCR->revision == 0xA2) {
        MXC_R_SIR_SHR13 = 0x0;
        MXC_R_SIR_SHR17 &= ~(0xC0);
    }
    else if(MXC_GCR->revision == 0xA4) {
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
    MXC_PWRSEQ->buretvec = (uint32_t)(&Backup_Init) | 1;

    // Enable the VDDCSW to ensure we have enough power to start
    MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;

    // Enable backup mode
    MXC_GCR->pm &= ~MXC_F_GCR_PM_MODE;

    /* Calls into assembly function in backup.S */
    Backup_Setup();

    /* Executed out of Backup Mode */
    SystemInit();

    /* Enable WUT interrupt */
    MXC_GCR->pm |= (MXC_F_GCR_PM_GPIOWKEN | MXC_F_GCR_PM_WUTWKEN);
    NVIC_EnableIRQ(WUT_IRQn);

    /* power off unused hardware */
    DisableUnused();
}

/*************************************************/
/* @brief This will enable wakeup from WUT and GPIO (if required).
 * @param bNeedExclusive Set if non WUT/GPIO wakeup sources need to be disabled.
 * @param bNeedGPIOWakeup Set GPIO must be selected as a possible wakeup source. */
static void SetWakeupSources(bool_t bNeedExclusive, bool_t bNeedGPIOWakeup)
{
    uint32_t u32PMVal;

    // disable other wakeups (if requested)
    u32PMVal = MXC_GCR->pm;
    if (bNeedExclusive) {
        u32PMVal &= ~(
                MXC_F_GCR_PM_GPIOWKEN |
                MXC_F_GCR_PM_RTCWKEN |
                MXC_F_GCR_PM_WUTWKEN |
                MXC_F_GCR_PM_SDMAWKEN |
                MXC_F_GCR_PM_USBWKEN);
    }

    // Enable GPIO wakeup.
    if (bNeedGPIOWakeup) {
        u32PMVal |= MXC_F_GCR_PM_GPIOWKEN;
    }

    // Enable WUT wakeup.
    u32PMVal |= MXC_F_GCR_PM_WUTWKEN;

    MXC_GCR->pm = u32PMVal;
}

/*************************************************/
/* This will enable user GPIO wakeup from low power modes.
 * @param bEnabled Set if wakeups are to be enabled for the selected pin. */
static void SetUserGPIOWakeups(bool_t bEnabled)
{
    const gpio_cfg_t *pUserPinInCfg;
    __IO uint32_t *pUserPinInPwrSeqStReg;
    __IO uint32_t *pUserPinInPwrSeqEnReg;
    uint32_t u32RegVal;

    // Get the gpio configuration for the utput pin.
    pUserPinInCfg = &user_pin[USER_PIN_IN];
    if (user_pin[USER_PIN_IN].port == PORT_1) {
        pUserPinInPwrSeqStReg = &MXC_PWRSEQ->lpwkst1;
        pUserPinInPwrSeqEnReg = &MXC_PWRSEQ->lpwken1;
    } else {
        pUserPinInPwrSeqStReg = &MXC_PWRSEQ->lpwkst0;
        pUserPinInPwrSeqEnReg = &MXC_PWRSEQ->lpwken0;
    }

    MXC_ASSERT(pUserPinInPwrSeqStReg != NULL);
    MXC_ASSERT(pUserPinInPwrSeqEnReg != NULL);

    // clear wake state.
    *pUserPinInPwrSeqStReg &= ~(pUserPinInCfg->mask);

    // Enable/Disable as a wake source.
    u32RegVal = *pUserPinInPwrSeqEnReg;
    if (bEnabled) {
        u32RegVal |= pUserPinInCfg->mask;
    } else {
        u32RegVal &= ~(pUserPinInCfg->mask);
    }
    *pUserPinInPwrSeqEnReg = u32RegVal;
}

/*************************************************/
/* This will clear user GPIO wakeup from low power modes flags. */
static void ClearUserGPIOWakeupSource(void)
{
    const gpio_cfg_t *pUserPinInCfg;
    __IO uint32_t *pUserPinInPwrSeqStReg;

    // Get the gpio configuration for the utput pin.
    pUserPinInCfg = &user_pin[USER_PIN_IN];
    if (user_pin[USER_PIN_IN].port == PORT_1) {
        pUserPinInPwrSeqStReg = &MXC_PWRSEQ->lpwkst1;
    } else {
        pUserPinInPwrSeqStReg = &MXC_PWRSEQ->lpwkst0;
    }

    *pUserPinInPwrSeqStReg &= ~(pUserPinInCfg->mask);
}

/*************************************************/
/* This will enter DeepSleep Mode */
void EnterDeepsleep(void)
{
    // Enable WUT wakeup
    SetWakeupSources(TRUE, TRUE);

    // Clear GPIO wakeup source status and reenable.
    SetUserGPIOWakeups(TRUE);

    /*Shutdown unused power domains */
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BGOFF;

    if(MXC_GCR->revision == 0xA2) {
        MXC_R_SIR_SHR13 = 0x0;
    }
    if(MXC_GCR->revision != 0xA1) {
        MXC_R_SIR_SHR17 &= ~(0xC0);
    }

    ICC_Disable();
    LP_ICacheShutdown();

    LP_VDD3PowerDown();
    LP_VDD4PowerDown();

    LP_FastWakeupEnable();

    BbDrvDisable();
    //MXC_PWRSEQ->lpvddpd |= MXC_F_PWRSEQ_LPVDDPD_VREGODPD;

    /* Retain all SRAM */
    MXC_PWRSEQ->lpcn |= (MXC_S_PWRSEQ_LPCN_RAMRET_EN3);

    //MXC_MCR->ctrl |= MXC_F_MCR_CTRL_VDDCSWEN;
    //switchToHIRC();
    //SIMO_setVregO_B(DS_VOLTAGE);

#if (BACKUP_MODE==1)
    BackupMode();
#else
    LP_EnterDeepSleepMode();
#endif
}

/*************************************************/
/* this will restore from DeepSleep Mode */
void ExitBackup(void)
{
    uint32_t buckoutFlag;

    /* Restore the core voltage before restoring the system clock */
    SIMO_setVregO_B(RUN_VOLTAGE);
    buckoutFlag = MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB;
    do {
        buckoutFlag &= ~(MXC_SIMO->buck_out_ready & buckoutFlag);
    } while (buckoutFlag);

    /* Move VCORE switch back to VCOREB */
    MXC_MCR->ctrl =
            (MXC_MCR->ctrl & ~(MXC_F_MCR_CTRL_VDDCSW)) | (0x1 <<
            MXC_F_MCR_CTRL_VDDCSW_POS);

    WUT_IntClear();
    NVIC_ClearPendingIRQ(WUT_IRQn);

    MXC_PWRSEQ->lpcn &= ~0x3FE00000;      /* Disable voltage Monitors for unused rails */

    // Enable WUT wakeup
    SetWakeupSources(FALSE, TRUE);

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BCKGRND;
    LP_ICacheWakeup();
    ICC_Enable();

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &= ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_TRNGD);

    MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VREGODPD;
    while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYD)) {
    }

    /* Wait for SIMO warmup */
    WUT_sleepUs(500);
    BbDrvEnable();

    switchTo96M();
}

/*************************************************/
/* this will restore from DeepSleep Mode */
void ExitDeepsleep(void)
{
#if (BACKUP_MODE==1)
    ExitBackup();
    return;
#endif

    //MXC_PWRSEQ->lpvddpd &= ~MXC_F_PWRSEQ_LPVDDPD_VREGODPD;

    MXC_PWRSEQ->lpcn &= ~0x3FE00000;      /* Disable voltage Monitors for unused rails */

    // Enable WUT wakeup
    SetWakeupSources(FALSE, TRUE);

    // Clear GPIO wakeup source status and reenable.
    ClearUserGPIOWakeupSource();

    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    MXC_PWRSEQ->lpcn |= MXC_F_PWRSEQ_LPCN_BCKGRND;
    LP_ICacheWakeup();
    ICC_Enable();

    /* Enable peripheral clocks */
    MXC_GCR->perckcn0 &=
            ~(MXC_F_GCR_PERCKCN0_GPIO0D | MXC_F_GCR_PERCKCN0_GPIO1D);
    MXC_GCR->perckcn1 &= ~(MXC_F_GCR_PERCKCN1_TRNGD);

    BbDrvEnable();

    /* Restore the core voltage before restoring the system clock */
    //SIMO_setVregO_B(RUN_VOLTAGE);
    //while (!(MXC_SIMO->buck_out_ready & MXC_F_SIMO_BUCK_OUT_READY_BUCKOUTRDYB)) {
    //}

    //switchTo96M();
}
