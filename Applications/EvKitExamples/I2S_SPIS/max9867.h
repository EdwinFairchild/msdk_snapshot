/*

    max9867.h  --  MAX9867 CODEC driver

 */

#include <stdint.h>
#include "i2c.h"

/* I2C slave address */
#define MAX9867_I2C_ADDR 0x30

/*
    MAX9867 registers and register bitfields
*/
#define MAX9867_REG_00_STATUS       0x00
#define MAX9867_REG_01_JACKSENSE    0x01
#define MAX9867_REG_02_AUXHIGH      0x02
#define MAX9867_REG_03_AUXLOW       0x03
#define MAX9867_REG_04_INTEN        0x04
#define MAX9867_REG_05_SYS_CLK      0x05
#define MAX9867_REG_06_CLKCFG_HI    0x06
#define MAX9867_REG_07_CLKCFG_LO    0x07
#define MAX9867_REG_08_IFC1A        0x08
#define MAX9867_REG_09_IFC1B        0x09
#define MAX9867_REG_0A_CODECFLTR    0x0A
#define MAX9867_REG_0B_SIDETONE     0x0B
#define MAX9867_REG_0C_DACLEVEL     0x0C
#define MAX9867_REG_0D_ADCLEVEL     0x0D
#define MAX9867_REG_0E_LEFTLINELVL  0x0E
#define MAX9867_REG_0F_RIGHTLINELVL 0x0F
#define MAX9867_REG_10_LEFTVOL      0x10
#define MAX9867_REG_11_RIGHTVOL     0x11
#define MAX9867_REG_12_LEFTMICGAIN  0x12
#define MAX9867_REG_13_RIGHTMICGAIN 0x13
#define MAX9867_REG_14_ADCINPUT     0x14
#define MAX9867_REG_15_MICCFG       0x15
#define MAX9867_REG_16_MODECFG      0x16
#define MAX9867_REG_17_PWR_MAN      0x17
#define MAX9867_REG_FF_REV_ID       0xFF

// MAX9867_REG_00_STATUS
#define MAX9867_CLD  0x80
#define MAX9867_SLD  0x40
#define MAX9867_ULK  0x20
#define MAX9867_JDET 0x02

// MAX9867_REG_02_JACKSENSE
#define MAX9867_LSNS  0x80
#define MAX9867_JKSNS 0x40
#define MAX9867_JKMIC 0x20

// MAX9867_REG_04_INTEN
#define MAX9867_ICLD   0x80
#define MAX9867_ISLD   0x40
#define MAX9867_IULK   0x20
#define MAX9867_SDODLY 0x04
#define MAX9867_IJDET  0x02

// MAX9867_REG_05_SYS_CLK
#define MAX9867_PSCLK_MASK      0x30
#define MAX9867_PSCLK_10_20_MHZ 0x10
#define MAX9867_PSCLK_20_40_MHZ 0x20
#define MAX9867_PSCLK_40_60_MHZ 0x30
#define MAX9867_FREQ_MASK       0x0F

// MAX9867_REG_06_CLKCFG_HI
#define MAX9867_PLL      0x80
#define MAX9867_NIH_MASK 0x7F

// MAX9867_REG_07_CLKCFG_LO
#define MAX9867_NIL_MASK   0xFF
#define MAX9867_RAPID_LOCK 0x01

// MAX9867_REG_08_IFC1A
#define MAX9867_MAS       0x80
#define MAX9867_WCI       0x40
#define MAX9867_BCI       0x20
#define MAX9867_DLY       0x10
#define MAX9867_HIZOFF    0x08
#define MAX9867_TDM       0x04

// MAX9867_REG_09_IFC1B
#define MAX9867_LVOLFIX      0x10
#define MAX9867_DMONO        0x08
#define MAX9867_BSEL_MASK    0x07
#define MAX9867_BSEL_64X     0x01
#define MAX9867_BSEL_48X     0x02
#define MAX9867_BSEL_PCLK_2  0x04
#define MAX9867_BSEL_PCLK_4  0x05
#define MAX9867_BSEL_PCLK_8  0x06
#define MAX9867_BSEL_PCLK_16 0x07

// MAX9867_REF_0A_CODECFLTR
#define MAX9867_FLTR_MODE  0x80
#define MAX9867_AVFLT_MASK 0x70
#define MAX9867_AVFLT_SHFT 4
#define MAX9867_DVFLT_MASK 0x07
#define MAX9867_DVFLT_SHFT 0

// MAX9867_REG_0B_SIDETONE
#define MAX9867_DSTS_MASK  0xC0
#define MAX9867_DSTS_SHFT  6
#define MAX9867_DSTS_NONE  0x00
#define MAX9867_DSTS_LADC  0x40
#define MAX9867_DSTS_RADC  0x80
#define MAX9867_DSTS_LRADC 0xC0
#define MAX9867_DVST_MASK  0x1F
#define MAX9867_DVST_SHFT  0

// MAX9867_REG_0C_DACLEVEL
#define MAX9867_DACM      0x40
#define MAX9867_DACG_MASK 0x30
#define MAX9867_DACG_SHFT 4
#define MAX9867_DACA_MASK 0x0F
#define MAX9867_DACA_SHFT 0

// MAX9867_REG_0D_ADCLEVEL
#define MAX9867_AVL_MASK 0xF0
#define MAX9867_AVL_SHFT 4
#define MAX9867_AVR_MASK 0x0F
#define MAX9867_AVR_SHFT 0

// MAX9867_REG_0E_LEFTLINELVL
// MAX9867_REG_0F_RIGHTLINELVL
#define MAX9867_LINEIN_MUTE      0x40
#define MAX9867_LINEIN_GAIN_MASK 0x0F
#define MAX9867_LINEIN_GAIN_SHFT 0

// MAX9867_REG_10_LEFTVOL
// MAX9867_REG_11_RIGHTVOL
#define MAX9867_VOL_MUTE 0x40
#define MAX9867_VOL_MASK 0x3F
#define MAX9867_VOL_SHFT 0

// MAX9867_REG_12_LEFTMICGAIN
// MAX9867_REG_13_RIGHTMICGAIN
#define MAX9867_PREAMP_MASK 0x60
#define MAX9867_PREAMP_SHFT 5
#define MAX9867_PGA_MASK    0x1F
#define MAX9867_PGA_SHFT    0

// MAX9867_REG_14_ADCINPUT
#define MAX9867_MXINL_MASK     0xC0
#define MAX9867_MXINL_NONE     0x00
#define MAX9867_MXINL_MIC      0x40
#define MAX9867_MXINL_LINE     0x80
#define MAX9867_MXINL_MIC_LINE 0xC0
#define MAX9867_MXINR_MASK     0x30
#define MAX9867_MXINR_NONE     0x00
#define MAX9867_MXINR_MIC      0x10
#define MAX9867_MXINR_LINE     0x20
#define MAX9867_MXINR_MIC_LINE 0x30
#define MAX9867_AUXCAP         0x08
#define MAX9867_AUXGAIN        0x04
#define MAX9867_AUXCAL         0x02
#define MAX9867_AUXEN          0x01

// MAX9867_REG_15_MICCFG
#define MAX9867_MICCLK_MASK     0xC0
#define MAX9867_MICCLK_PCLKDIV8 0x00
#define MAX9867_MICCLK_PCLKDIV6 0x40
#define MAX9867_DIGMICL         0x20
#define MAX9867_DIGMICR         0x10

// MAX9867_REG_16_MODECFG
#define MAX9867_DSLEW                                   0x80
#define MAX9867_VSEN                                    0x40
#define MAX9867_ZDEN                                    0x20
#define MAX9867_JDETEN                                  0x08
#define MAX9867_HPMODE_MASK                             0x07
#define MAX9867_HPMODE_STEREO_DIFF_CLICKLESS            0x00
#define MAX9867_HPMODE_MONO_DIFF_CLICKLESS              0x01
#define MAX9867_HPMODE_STEREO_CAPLESS_CLICKLESS         0x02
#define MAX9867_HPMODE_MONO_CAPLESS_CLICKLESS           0x03
#define MAX9867_HPMODE_STEREO_SINGLE_ENDED_CLICKLESS    0x04
#define MAX9867_HPMODE_MONO_SINGLE_ENEDED_CLICKLESS     0x05
#define MAX9867_HPMODE_STEREO_SINGLE_ENDED_FAST_TURN_ON 0x06
#define MAX9867_HPMODE_MONO_SINGLE_ENDED_FAST_TURN_ON   0x07

// MAX9867_REG_17_PWR_MAN
#define MAX9867_SHDN  0x80
#define MAX9867_LNLEN 0x40
#define MAX9867_LNREN 0x20
#define MAX9867_DALEN 0x08
#define MAX9867_DAREN 0x04
#define MAX9867_ADLEN 0x02
#define MAX9867_ADREN 0x01


int codecWrite(uint8_t reg, uint8_t dat);
int codecRead(uint8_t reg, uint8_t *buf);
int codecUpdate(uint8_t reg, uint8_t msk, uint8_t val);
int codecInit(mxc_i2c_regs_t *intfc, uint32_t clk);

void codecSuspend(void);
void codecResume(void);
void codecSetFilterMode(int mode);
void codecSetADCFilter(int mode);
void codecSetDACFilter(int mode);
void codecSetSidetoneSource(int source);
void codecSetSidetoneLevel(int level);
void codecSetDACMute(int state);
void codecSetDACGain(int gail);
void codecSetDACLevel(int level);
void codecSetADCLevel(int left, int right);
void codecSetLineInGain(int left, int right);
void codecSetLineInMute(int state);
void codecSetVolume(int left, int right);
void codecSetMute(int state);
void codecSetMicPreamp(int left, int right);
void codecSetMicPGA(int left, int right);
void codecSetADCInput(int state);
void codecSetMicClkDiv(int div);
void codecSetVolSlewSpeed(int speed);
void codecSetVolChangeSmoothing(int state);
void codecSetLineInZeroCrossingDetect(int state);
void codecSetJackDetect(int state);
void codecSetHeadphoneMode(int state);
void codecSetBias(int msk, int val);
