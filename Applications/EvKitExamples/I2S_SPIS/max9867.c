/*
    max9867.c  --  MAX9867 CODEC driver
*/

#include <stdint.h>
#include "mxc_errors.h"
#include "i2c.h"
#include "max9867.h"

#define MHZ(f)  (f * 1000000)

static mxc_i2c_regs_t *i2cm;
static uint8_t addr = 0;
static uint32_t mclk;

// *****************************************************************************
int codecWrite(uint8_t reg, uint8_t dat)
{
    uint8_t buf[2];

    if (!addr) {
        return E_UNINITIALIZED;
    }

    buf[0] = reg;
    buf[1] = dat;

    return I2C_MasterWrite(i2cm, addr, buf, sizeof(buf), 0);
}

// *****************************************************************************
int codecRead(uint8_t reg, uint8_t *buf)
{
    int err;

    if (!addr) {
        return E_UNINITIALIZED;
    }

    err = I2C_MasterWrite(i2cm, addr, &reg, sizeof(reg), 1);
    if (err != 1) {
        return err;
    }

    return I2C_MasterRead(i2cm, addr, buf, 1, 0);
}

// *****************************************************************************
int codecUpdate(uint8_t reg, uint8_t msk, uint8_t val)
{
    int err;
    uint8_t tmp;

    if (!addr) {
        return E_UNINITIALIZED;
    }

    if ((err = codecRead(reg, &tmp)) != 1) {
        return err;
    }

    tmp &= ~msk;
    tmp |= val;

    return codecWrite(reg, tmp);
}

// *****************************************************************************
int codecInit(mxc_i2c_regs_t *intfc, uint32_t clk)
{
    uint8_t reg;

    i2cm = intfc;
    mclk = clk;
    addr = MAX9867_I2C_ADDR;

    if (codecRead(MAX9867_REG_FF_REV_ID, &reg) != 1) {
        return E_NO_DEVICE;
    }

    // Clear CODEC state, it remembers its state between software resets
    for (reg = MAX9867_REG_04_INTEN; reg <= MAX9867_REG_17_PWR_MAN; reg++) {
        codecWrite(reg, 0x00);
    }

    codecSuspend();

    codecUpdate(MAX9867_REG_17_PWR_MAN,
                MAX9867_DALEN|MAX9867_DAREN ,
                MAX9867_DALEN|MAX9867_DAREN );

    // configure master clock prescaler
    if ((mclk < MHZ(10)) || (mclk > MHZ(60))) {
        return E_BAD_PARAM;
    }

    if (mclk <= MHZ(20)) {
        codecUpdate(MAX9867_REG_05_SYS_CLK, MAX9867_PSCLK_MASK, MAX9867_PSCLK_10_20_MHZ);
    } else if (mclk <= MHZ(40)) {
        codecUpdate(MAX9867_REG_05_SYS_CLK, MAX9867_PSCLK_MASK, MAX9867_PSCLK_20_40_MHZ);
    } else {
        codecUpdate(MAX9867_REG_05_SYS_CLK, MAX9867_PSCLK_MASK, MAX9867_PSCLK_40_60_MHZ);
    }

    // configure audio clock
    codecWrite(MAX9867_REG_07_CLKCFG_LO, 0);
    codecWrite(MAX9867_REG_06_CLKCFG_HI, 0x60);

    // TDM mode
    codecWrite(MAX9867_REG_08_IFC1A, MAX9867_TDM);
    codecWrite(MAX9867_REG_09_IFC1B, 0x00);

    // max32665 evkit hardware wired for stereo capacitorless (clickless)
    codecUpdate(MAX9867_REG_16_MODECFG, MAX9867_HPMODE_MASK, MAX9867_HPMODE_STEREO_CAPLESS_CLICKLESS);

    codecResume();

    return E_NO_ERROR;
}

// *****************************************************************************
void codecSuspend(void)
{
    codecUpdate(MAX9867_REG_17_PWR_MAN, MAX9867_SHDN, 0);
}

// *****************************************************************************
void codecResume(void)
{
    codecUpdate(MAX9867_REG_17_PWR_MAN, MAX9867_SHDN, MAX9867_SHDN);
}

// *****************************************************************************
void codecSetFilterMode(int mode)
{
    codecUpdate(MAX9867_REG_0A_CODECFLTR,
                MAX9867_FLTR_MODE,
                mode ? MAX9867_FLTR_MODE : 0);
}

// *****************************************************************************
void codecSetADCFilter(int mode)
{
    codecUpdate(MAX9867_REG_0A_CODECFLTR,
                MAX9867_AVFLT_MASK,
                (mode << MAX9867_AVFLT_SHFT) & MAX9867_AVFLT_MASK);
}

// *****************************************************************************
void codecSetDACFilter(int mode)
{
    codecUpdate(MAX9867_REG_0A_CODECFLTR,
                MAX9867_DVFLT_MASK,
                (mode << MAX9867_DVFLT_SHFT) & MAX9867_DVFLT_MASK);
}

// *****************************************************************************
void codecSetSidetoneSource(int source)
{
    codecUpdate(MAX9867_REG_0B_SIDETONE, MAX9867_DSTS_MASK, source & MAX9867_DSTS_MASK);
}

// *****************************************************************************
void codecSetSidetoneLevel(int level)
{
    codecUpdate(MAX9867_REG_0B_SIDETONE, MAX9867_DVST_MASK, (level << MAX9867_DVST_SHFT) & MAX9867_DVST_MASK);
}

// *****************************************************************************
void codecSetDACMute(int state)
{
    codecUpdate(MAX9867_REG_0C_DACLEVEL, MAX9867_DACM, state ? MAX9867_DACM : 0);
}

// *****************************************************************************
void codecSetDACGain(int gain)
{
    codecUpdate(MAX9867_REG_0C_DACLEVEL,
                MAX9867_DACG_MASK,
                (gain << MAX9867_DACG_SHFT) & MAX9867_DACG_MASK);
}

// *****************************************************************************
void codecSetDACLevel(int level)
{
    codecUpdate(MAX9867_REG_0C_DACLEVEL,
                MAX9867_DACA_MASK,
                (level << MAX9867_DACA_SHFT) & MAX9867_DACA_MASK);
}

// *****************************************************************************
void codecSetADCLevel(int left, int right)
{
    codecWrite(MAX9867_REG_0D_ADCLEVEL,
               ((left << MAX9867_AVL_SHFT) & MAX9867_AVL_MASK) |
               ((right << MAX9867_AVL_SHFT) & MAX9867_AVR_MASK));
}

// *****************************************************************************
void codecSetLineInGain(int left, int right)
{
    codecUpdate(MAX9867_REG_0E_LEFTLINELVL, MAX9867_LINEIN_GAIN_MASK, (left << MAX9867_LINEIN_GAIN_SHFT) & MAX9867_LINEIN_GAIN_MASK);
    codecUpdate(MAX9867_REG_0F_RIGHTLINELVL, MAX9867_LINEIN_GAIN_MASK, (right << MAX9867_LINEIN_GAIN_SHFT) & MAX9867_LINEIN_GAIN_MASK);
}

// *****************************************************************************
void codecSetLineInMute(int state)
{
    codecUpdate(MAX9867_REG_0E_LEFTLINELVL, MAX9867_LINEIN_MUTE, state ? MAX9867_LINEIN_MUTE : 0);
    codecUpdate(MAX9867_REG_0F_RIGHTLINELVL, MAX9867_LINEIN_MUTE, state ? MAX9867_LINEIN_MUTE : 0);
}

// *****************************************************************************
void codecSetVolume(int left, int right)
{
    codecUpdate(MAX9867_REG_10_LEFTVOL, MAX9867_VOL_MASK, (left << MAX9867_VOL_SHFT) & MAX9867_VOL_MASK);
    codecUpdate(MAX9867_REG_11_RIGHTVOL, MAX9867_VOL_MASK, (right << MAX9867_VOL_SHFT) & MAX9867_VOL_MASK);
}

// *****************************************************************************
void codecSetMute(int state)
{
    codecUpdate(MAX9867_REG_10_LEFTVOL, MAX9867_VOL_MUTE, state ? MAX9867_VOL_MUTE : 0);
    codecUpdate(MAX9867_REG_11_RIGHTVOL, MAX9867_VOL_MUTE, state ? MAX9867_VOL_MUTE : 0);
}

// *****************************************************************************
void codecSetMicPreamp(int left, int right)
{
    codecUpdate(MAX9867_REG_12_LEFTMICGAIN, MAX9867_PREAMP_MASK, (left << MAX9867_PREAMP_SHFT) & MAX9867_PREAMP_MASK);
    codecUpdate(MAX9867_REG_13_RIGHTMICGAIN, MAX9867_PREAMP_MASK, (right << MAX9867_PREAMP_SHFT) & MAX9867_PREAMP_MASK);
}

// *****************************************************************************
void codecSetMicPGA(int left, int right)
{
    codecUpdate(MAX9867_REG_12_LEFTMICGAIN, MAX9867_PGA_MASK, (left << MAX9867_PGA_SHFT) & MAX9867_PGA_MASK);
    codecUpdate(MAX9867_REG_13_RIGHTMICGAIN, MAX9867_PGA_MASK, (right << MAX9867_PGA_SHFT) & MAX9867_PGA_MASK);
}

// *****************************************************************************
void codecSetADCInput(int state)
{
    // Missing here are Auxiliary Input Capture, Auxiliary Input Gain Calibration, Auxiliary Input Offset Calibration and Auxiliary Input Enable
    codecUpdate(MAX9867_REG_14_ADCINPUT, MAX9867_MXINL_MASK, state & MAX9867_MXINL_MASK);
}

// *****************************************************************************
void codecSetMicClkDiv(int div)
{
    codecUpdate(MAX9867_REG_15_MICCFG, MAX9867_MICCLK_MASK, div & MAX9867_MICCLK_MASK);
}

// *****************************************************************************
void codecSetMicEnable(int left, int right)
{
    codecUpdate(MAX9867_REG_15_MICCFG,
                MAX9867_DIGMICL | MAX9867_DIGMICR,
                (left ? MAX9867_DIGMICL : 0) | (right ? MAX9867_DIGMICR : 0));
}

// *****************************************************************************
void codecSetVolSlewSpeed(int speed)
{
    codecUpdate(MAX9867_REG_16_MODECFG, MAX9867_DSLEW, speed ? MAX9867_DSLEW : 0);
}

// *****************************************************************************
void codecSetVolChangeSmoothing(int state)
{
    codecUpdate(MAX9867_REG_16_MODECFG, MAX9867_VSEN, state ? 0 : MAX9867_VSEN);
}

// *****************************************************************************
void codecSetLineInZeroCrossingDetect(int state)
{
    codecUpdate(MAX9867_REG_16_MODECFG, MAX9867_ZDEN, state ? 0 : MAX9867_ZDEN);
}

// *****************************************************************************
void codecSetJackDetect(int state)
{
    codecUpdate(MAX9867_REG_16_MODECFG, MAX9867_JDETEN, state ? MAX9867_JDETEN : 0);
}

// *****************************************************************************
void codecSetHeadphoneMode(int state)
{
    codecUpdate(MAX9867_REG_16_MODECFG, MAX9867_HPMODE_MASK, state & MAX9867_HPMODE_MASK);
}

// *****************************************************************************
void codecSetBias(int msk, int val)
{
    codecUpdate(MAX9867_REG_17_PWR_MAN, msk, val);
}
