
#include <stdio.h>
#include <string.h>
//#include "roadtest.h"
#include "tmr_utils.h"
#include "gpio_regs.h"
#include "gpio.h"
#include "gcr_regs.h"
#include "adc_regs.h"
#include "pwrseq_regs.h"
#include "rtc_regs.h"
//#include "test_regs.h"
//#include "bbsir_regs.h"
#include "mxc_config.h"
#include "max32665.h"

#define REVISION            1
#define SLEEP_MODE          0
#define BACKGROUND_MODE     0
#define DEEPSLEEP_MODE      0
#define BACKUP_MODE         0
#define DISABLE_SIMO        0
#define DEBUG_P11           0
#define DEBUG_P0            0
#define DEBUG_VILH          0
#define SWITCH_TO_32MXO     0
#define TESTER              0

void dumb_delay(int count) {
  volatile int i;
  for (i = 0; i < count; i++) { }
}

void toggle_p1_15(int count) {
  volatile int i;

  MXC_GPIO1->out_en = 0x8000;
  for (i=0; i < count; i++) {
    MXC_GPIO1->out &= 0;
    dumb_delay(1);
    MXC_GPIO1->out |= 0x8000;
    dumb_delay(1);
  }
  MXC_GPIO1->out_en = 0;
}

void toggle_p0_11_8() {

  //MXC_GCR->clkcn = 0x640c2040; // 96MHz div1 mode
  //while ((MXC_GCR->clkcn & 0x08000000) == 0x00000000);
  (*(volatile unsigned int*)0x40000074) = 0x000000d9;
  MXC_GCR->clkcn = 0x6d0d2800; // enable 32MHz XO
  //dumb_delay(10);
  //MXC_GCR->clkcn = 0x6d0d2800; // switch to 96MHz RO
  MXC_GCR->clkcn = 0x6d0d2400; // switch to 32MHz XO
  //MXC_GCR->clkcn = 0x6c0c2000; // switch to 60MHz RO

  MXC_GPIO0->out_en = 0xf0000f00;
  //MXC_GPIO1->out_en = 0x00000001;

  while (1) {
    MXC_GPIO0->out &= 0;
    MXC_GPIO0->out |= 0xf0000f00;
    //MXC_GPIO1->out &= 0;
    //MXC_GPIO1->out |= 0x00000001;
  }
}

void GPIO_Port1_Handler(void * pParam) {
  uint32_t stat, i;

  //__disable_irq();
  // Reenable VCOREB
  //(*(volatile unsigned int*)0x40006844) = 0x00000000;

  // Measure P1.15 for wake-up time
  //toggle_p1_15(2);
  
  // Return VCOREA and VCOREB
  (*(volatile unsigned int*)0x4000440c) = 0x00000032; // VCOREA
  (*(volatile unsigned int*)0x40004408) = 0x00000032; // VCOREB (need 2 writes)
  (*(volatile unsigned int*)0x40004408) = 0x00000032; // VCOREB
  (*(volatile unsigned int*)0x40004404) = 0x000000f8; // VDDA
  (*(volatile unsigned int*)0x40006c10) = 0x0000020b; // use for all other LP modes
  (*(volatile unsigned int*)0x40006844) = 0x0;        // Bring up VREGO_D
  stat = MXC_GPIO1->int_stat;
  MXC_GPIO1->int_clr = stat;
  
  // Check that VREGO_A, VREGO_B and VREGO_C are ready
  while ((*(volatile unsigned int*)0x40004440 & 0x00000007) != 0x00000007);

  for (i=0; i<10; i++) {
  //  dumb_delay(1000000); // ~25s
  //  dumb_delay(100000);  // ~2.5s
  }
  // Measure P1.15 for wake-up time
  toggle_p1_15(2);

  // Restore VCORE supply on VCOREB
  (*(volatile unsigned int*)0x40006c10) = 0x0000020b;
}

void Reset_Handler(void);

int main(void)
{
  uint32_t val, i;

  gpio_cfg_t stGpioCfg;
  memset(&stGpioCfg, '\0', sizeof(stGpioCfg));
  stGpioCfg.port = PORT_1;
  GPIO_RegisterCallback(&stGpioCfg, GPIO_Port1_Handler, NULL);

  //printf("\n\n------- ME14 Silicon Testing -------\n");
  //printf("Read MXC_ADC->ctrl is %04x\n", MXC_ADC->ctrl);
  //val = 0x55555555;
  //printf("  writing MXC_ADC->ctrl to %04x\n");
  //MXC_ADC->ctrl = val;
  //printf("Read MXC_ADC->ctrl is %04x\n", MXC_ADC->ctrl);
  //printf("done.\n");

//toggle_p0_11_8();

//---------------------------------------------------------------------------
// Initial setup
//---------------------------------------------------------------------------
// Bring VTXIN/VRXIN back to 1V forcing the 30kHz BUCK clock
if (BACKUP_MODE == 1) {
  (*(volatile unsigned int*)0x40006c10)  = 0x00000200; // use for BACKUP w/o memories
} else {
  (*(volatile unsigned int*)0x40006c10)  = 0x0000020b; // use for all other LP modes
}

(*(volatile unsigned int*)0x40006844)  = 0x0;

// Wait 1 second for VCOREB to recover from a Backup Mode with no memories
dumb_delay(1000000);

MXC_GCR->rstr1 = 0x2000000;
MXC_GCR->rstr1 = 0x0000000;

// Set the return from backup to a valid address
*(uint32_t *) 0x40006848 = (uint32_t)(Reset_Handler) | 1;

//---------------------------------------------------------------------------
// Testmode features
//---------------------------------------------------------------------------
if (DEEPSLEEP_MODE == 1 || BACKUP_MODE == 1) {
  // Enter Test Mode (TME=1)
  //MXC_TEST->tm = (0x1 << 0);
  //(*(volatile unsigned int*)0x40000c00) = 0x00000001;

  // Set VCOREA and VCOREB reset trip to 0.73V
  //(*(volatile unsigned int*)0x40005410) |= 0x00004000; // VCOREA
  //(*(volatile unsigned int*)0x40005410) &= 0xffffcfff;
  //(*(volatile unsigned int*)0x40005418) |= 0x08000000; // VCOREB
  //(*(volatile unsigned int*)0x40005418) &= 0xf9ffffff;

  //(*(volatile unsigned int*)0x40005440) |= 0x01000000; // 16kHz
  //(*(volatile unsigned int*)0x40005440) &= 0xfcffffff;

  // Duty-cycle SIMO BG
  //(*(volatile unsigned int*)0x40005414) |= 0x00000040; // on every 1ms
  //(*(volatile unsigned int*)0x40005414) &= 0xffffff7f; // on every 1ms
  //(*(volatile unsigned int*)0x40005414) |= 0x00000080; // on every 2ms
  //(*(volatile unsigned int*)0x40005414) &= 0xffffffbf; // on every 2ms
  //(*(volatile unsigned int*)0x40005414) |= 0x000000c0; // on every 4ms
  //(*(volatile unsigned int*)0x40005414) &= 0xffffff3f; // no every 0ms or "never on"

}

// Disable 32k crystal, 6kHz oscillators (8kHz cannot be disabled)
//(*(volatile unsigned int*)0x4000542c) |= 0x00000007;

//MXC_SIR_TM->shr4 = 0x00020002;
//(*(volatile unsigned int*)0x40000410) = 0x00020002;

// Set Low Power BUCK clock back to 8kHz (BBSIR17)
(*(volatile unsigned int*)0x40005444) &= 0xffffff3f;

// Scale BUCK clock to /8 or /16 (BBSIR13)
//(*(volatile unsigned int*)0x40005434) |= 0x00000003; // div16
//(*(volatile unsigned int*)0x40005434) |= 0x00000001; // div8
(*(volatile unsigned int*)0x40005434) &= 0xfffffffc; // div1

//---------------------------------------------------------------------------
// Peripheral clock disables
//---------------------------------------------------------------------------
MXC_GCR->perckcn0  &= ~(0x000000003); // Clear GPIO0 and GPIO1 Disable
//MXC_GCR->perckcn1 &= ~(0x000400800); // Clear ICACHE0 and ICACHE1 Disable

//---------------------------------------------------------------------------
// USB PHY Power On Reset and Suspend
//---------------------------------------------------------------------------
(*(volatile unsigned int*)0x400B1410) = 0x00000000; // PONRST=0
(*(volatile unsigned int*)0x400014A0) = 0x00000003; // SUSPEND=1

//---------------------------------------------------------------------------
// Miscellaneous settings
//---------------------------------------------------------------------------
// Bits   9 - RSTN 25kohm resistor pullup
// Bits   8 - BUCK clock scaling on during Low Power modes
// Bits   3 - USB PHY switch off during Low Power modes
// Bits 2:1 - VCORESW state (SW needs to set to 01 after exiting LP modes)
// Bits   0 - VCORESW "switching" on
if (REVISION == 1) {
  (*(volatile unsigned int*)0x40006c10) = 0x0000020b; // debug bit 8
} else {
  (*(volatile unsigned int*)0x40006c10) = 0x0000000b; // debug bit 8
}

// Increase BUCK clock speed to help bring up VCOREB, if need be
//(*(volatile unsigned int*)0x40006c10)  = 0x0000020b;
//(*(volatile unsigned int*)0x40005444) |= 0x00000040; // 16kHz
//(*(volatile unsigned int*)0x40005444) &= 0xffffff7f;
//(*(volatile unsigned int*)0x40005444) |= 0x00000080; // 30kHz
//(*(volatile unsigned int*)0x40005444) &= 0xffffffbf;

// RTC set to Quiet Mode always (RTCCN, 0x40006010)
MXC_RTC->ctrl = 0x00008000;
MXC_RTC->ctrl = 0x00008801;
MXC_RTC->ctrl = 0x00008801;

//---------------------------------------------------------------------------
// Set GPIO mode as input, weak pull-up
//---------------------------------------------------------------------------
MXC_GPIO1->en       = 0xFFFFFFFF;
MXC_GPIO1->en1      = 0;
MXC_GPIO1->pad_cfg1 = 0xffffffff;
MXC_GPIO1->pad_cfg2 = 0x00000000;
if (REVISION == 1) {
MXC_GPIO1->ps       = 0;
} else {
MXC_GPIO1->ps       = 0xffffffff;
}

// set to all input 
MXC_GPIO1->out_en = 0x0;

MXC_GPIO0->en       = 0xFFFFFFFF;
MXC_GPIO0->en1      = 0;
MXC_GPIO0->pad_cfg1 = 0xffffffff;
MXC_GPIO0->pad_cfg2 = 0x00000000;
if (REVISION == 1) {
MXC_GPIO0->ps       = 0;
} else {
MXC_GPIO0->ps       = 0xffffffff;
}

// set to all input 
MXC_GPIO0->out_en = 0x0;

// Set stronger pullups on P1[9,1:0] (0x400090b8)
//MXC_GPIO1->pssel = 0x00000203;

// Debug P0[31:24] VIL/VIH
if (DEBUG_VILH == 1) {
  MXC_GPIO0->pad_cfg1 = 0x00ffffff;
  MXC_GPIO0->out_en   = 0x000000ff;
  while(1) {
    val = (MXC_GPIO0->in)>>26; //19
    MXC_GPIO0->out = val;
  }
}

if (DEBUG_P11 == 1) {
  MXC_GPIO1->out_en = 0x2;
  while(1) {
    MXC_GPIO1->out &= 0;
    dumb_delay(1);
    MXC_GPIO1->out |= 0x2;
    dumb_delay(1);
  }
}

if (DEBUG_P0 == 1) {
  MXC_GPIO0->out_en = 0x3f;
  while(1) {
    MXC_GPIO0->out &= 0;
    //dumb_delay(1);
    MXC_GPIO0->out |= 0x3f;
    //dumb_delay(1);
  }
}

//---------------------------------------------------------------------------
// Adjust IPK values to optimize efficiency, especially in Active Modes
//---------------------------------------------------------------------------
//(*(volatile unsigned int*)0x40004414) = 0x00000000; // IPKSET1, IPKSET2 12uA
//(*(volatile unsigned int*)0x40004418) = 0x00000000; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004418) = 0x00000000; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004414) = 0x00000088; // IPKSET1, IPKSET2  8.3uA
//(*(volatile unsigned int*)0x40004418) = 0x00000088; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004418) = 0x00000088; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004414) = 0x00000011; // IPKSET1, IPKSET2  8.0uA
//(*(volatile unsigned int*)0x40004418) = 0x00000054; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004418) = 0x00000054; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004414) = 0x000000ff; // IPKSET1, IPKSET2
//(*(volatile unsigned int*)0x40004418) = 0x000000ff; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x40004418) = 0x000000ff; // IPKSET3, IPKSET4
//(*(volatile unsigned int*)0x4000441c) = 0x0000000f; // TON

// TON: bits 29:25, base 3uA current mirror
//      bit 29 adds 1.000uA,
//          28 adds 0.500uA,
//          27 adds 0.250uA,
//          26 adds 0.125uA,
//          25 adds 0.062uA
//(*(volatile unsigned int*)0x40005428) |= 0x3e000000;


/////////////////////////////////////////////////////////////////////////////


back_to_checking_p1_9:
//---------------------------------------------------------------------------
// Use P1.9 to gate remaining code
// All normal setup is done, place remaining configurations here
//---------------------------------------------------------------------------
// Wait for P1[9] to be high, i.e. set a jumper to ground to keep it here
//while(1);
MXC_GCR->clkcn = 0x640c2040; // 96MHz div1 mode
while ((MXC_GCR->clkcn & 0x08000000) == 0x00000000);
MXC_GCR->clkcn = 0x68082800;

while ((MXC_GPIO1->in & 0x00000200) == 0x00000000);

if (DISABLE_SIMO == 1) {
  (*(volatile unsigned int*)0x4000542c) |= 0x00000040;
} else {
  (*(volatile unsigned int*)0x4000542c) &= 0xffffffbf;
}

for (i=0; i<10; i++) {
//  dumb_delay(1000000); // ~25s
  dumb_delay(100000);  // ~2.5s
}

//while(1);

// Peripheral clock enables
MXC_GCR->perckcn0 = 0xFFFFFFFF;
MXC_GCR->perckcn1 = 0xFFFFFFFF;
if (BACKGROUND_MODE == 1) {
  MXC_GCR->perckcn1 = 0xfffffeff;    // Enable SDMA for BACKGROUND mode
  //MXC_GCR->perckcn1 = 0xffffffff;
} else {
  MXC_GCR->perckcn1 = 0xffffffff;
}
//---------------------------------------------------------------------------
// Cache
//---------------------------------------------------------------------------
//(*(volatile unsigned int*)0x4002a100) = 0x00000001; // instr. cache on
(*(volatile unsigned int*)0x4002a100) = 0x00000000; // instr. cache off

//---------------------------------------------------------------------------
// Power sequencer setup **WARNING** don't disable monitors for VCOREA/B
//---------------------------------------------------------------------------
// Bits  11 - Enable BG off in Low Power modes
// Bits  10 - Bypass BG ready for fast wake-up
// Bits 1:0 - Retain memories in Backup Mode

MXC_PWRSEQ->lpcn  = 0x00000800; // no system RAM retained
//MXC_PWRSEQ->lpcn  = 0x00000801; //  40KB retained
//MXC_PWRSEQ->lpcn  = 0x00000802; //  80KB retained
//MXC_PWRSEQ->lpcn  = 0x00000803; // 560KB retained (all)

// Write 1 to Clear
MXC_PWRSEQ->lpwkst0 = 0xFFFFFFFF;
MXC_PWRSEQ->lpwkst1 = 0xFFFFFFFF;
//MXC_PWRSEQ->lpwkst2 = 0xFFFFFFFF;
//MXC_PWRSEQ->lpwkst3 = 0xFFFFFFFF;
MXC_PWRSEQ->lppwst  = 0xFFFFFFFF;
 
MXC_PWRSEQ->lpwkst0 = 0xFFFFFFFF;
MXC_PWRSEQ->lpwkst1 = 0xFFFFFFFF;
//MXC_PWRSEQ->lpwkst2 = 0xFFFFFFFF;
//MXC_PWRSEQ->lpwkst3 = 0xFFFFFFFF;
MXC_PWRSEQ->lppwst  = 0xFFFFFFFF;

//---------------------------------------------------------------------------
// Clock sources and divide modes
//---------------------------------------------------------------------------
// Trim target frequency of 96MHz oscillator
//(*(volatile unsigned int*)0x40006c04) = 0x0000004d; // 96MHz
//(*(volatile unsigned int*)0x40006c04) = 0x0000017b; // 48MHz

// Trim target frequency of 8kHz oscillator
//(*(volatile unsigned int*)0x40005440) |= 0x80000000; // 9.0kHz
//(*(volatile unsigned int*)0x40005440) &= 0x9fffffff; // 9.0kHz
//(*(volatile unsigned int*)0x40005440) |= 0xc0000000; // 8.0kHz
//(*(volatile unsigned int*)0x40005440) &= 0xdfffffff; // 8.0kHz
//(*(volatile unsigned int*)0x40005440) |= 0x60000000; // 5.9kHz
//(*(volatile unsigned int*)0x40005440) &= 0x7fffffff; // 5.9kHz

// 96MHz oscillator, all other HF oscillators disabled
MXC_GCR->clkcn = 0x68082800; // div1   mode
//MXC_GCR->clkcn = 0x68082840; // div2   mode
//MXC_GCR->clkcn = 0x68082880; // div4   mode
//MXC_GCR->clkcn = 0x680828c0; // div8   mode
//MXC_GCR->clkcn = 0x68082900; // div16  mode
//MXC_GCR->clkcn = 0x68082940; // div32  mode
//MXC_GCR->clkcn = 0x68082980; // div64  mode
//MXC_GCR->clkcn = 0x680829c0; // div128 mode

// 60MHz oscillator, all other HF oscillators disabled
//MXC_GCR->clkcn = 0x64042000; // div1   mode
//MXC_GCR->clkcn = 0x64042040; // div2   mode
//MXC_GCR->clkcn = 0x64042080; // div4   mode
//MXC_GCR->clkcn = 0x640420c0; // div8   mode
//MXC_GCR->clkcn = 0x64042100; // div16  mode
//MXC_GCR->clkcn = 0x64042140; // div32  mode
//MXC_GCR->clkcn = 0x64042180; // div64  mode
//MXC_GCR->clkcn = 0x640421c0; // div128 mode

if (SWITCH_TO_32MXO == 1) {
  // Enable BTLE TX LDO to 1.1V and RX LDO to 0.9V
  (*(volatile unsigned int*)0x40000074) = 0x000000d9;
  dumb_delay(10);
  MXC_GCR->clkcn = 0x6d0d2800; // enable 32MHz XO
  dumb_delay(10);
  MXC_GCR->clkcn = 0x65052400; // select 32MHz XO, div1  mode
  //MXC_GCR->clkcn = 0x65052440; // select 32MHz XO, div2  mode
  //MXC_GCR->clkcn = 0x65052480; // select 32MHz XO, div4  mode
  //MXC_GCR->clkcn = 0x650524c0; // select 32MHz XO, div8  mode
  //MXC_GCR->clkcn = 0x65052500; // select 32MHz XO, div16 mode
  //MXC_GCR->clkcn = 0x65052540; // select 32MHz XO, div32 mode
}

//---------------------------------------------------------------------------
// Powering down domains and/or SIMO channels outputs
//---------------------------------------------------------------------------
// PD VDD5:VDD3 (bits [8:6])
//(*(volatile unsigned int*)0x40006844) = 0x000000E0;

// PD VDD5:VDD3 (bits [8:6]), VREGO_D, VCOREB (bits [1:0])
//(*(volatile unsigned int*)0x40006844) = 0x000000E3;

//---------------------------------------------------------------------------
// Changing SIMO VREGO_C (VCOREA) supply voltage for backup only
//---------------------------------------------------------------------------
//(*(volatile unsigned int*)0x4000440c) = 0x00000032; // 1.00V
//(*(volatile unsigned int*)0x4000440c) = 0x00000028; // 0.90V
//(*(volatile unsigned int*)0x4000440c) = 0x0000001e; // 0.80V
//(*(volatile unsigned int*)0x4000440c) = 0x00000014; // 0.70V
//(*(volatile unsigned int*)0x4000440c) = 0x00000000; // 0.50V

//---------------------------------------------------------------------------
// Changing SIMO VREGO_B (VCOREB) supply voltage
//---------------------------------------------------------------------------
//(*(volatile unsigned int*)0x40004408) = 0x00000032; // 1.00V
//(*(volatile unsigned int*)0x40004408) = 0x0000002d; // 0.95V
//(*(volatile unsigned int*)0x40004408) = 0x00000028; // 0.90V
//(*(volatile unsigned int*)0x40004408) = 0x00000023; // 0.85V
//(*(volatile unsigned int*)0x40004408) = 0x0000001f; // 0.81V
//(*(volatile unsigned int*)0x40004408) = 0x0000001e; // 0.80V
//(*(volatile unsigned int*)0x40004408) = 0x0000001d; // 0.79V
//(*(volatile unsigned int*)0x40004408) = 0x0000001c; // 0.78V
//(*(volatile unsigned int*)0x40004408) = 0x0000001b; // 0.77V
//(*(volatile unsigned int*)0x40004408) = 0x0000001a; // 0.76V
//(*(volatile unsigned int*)0x40004408) = 0x00000019; // 0.75V
//(*(volatile unsigned int*)0x40004408) = 0x00000000; // 0.50V

//--------------------------------------------------------------------------
// Changing SIMO VREGO_A and VREGO_D to 0.6 and 0.5V
//--------------------------------------------------------------------------
//(*(volatile unsigned int*)0x40004404) = 0x00000000;
//(*(volatile unsigned int*)0x40004410) = 0x00000000;

//--------------------------------------------------------------------------
// Setup wake-up and interrupt source to P1[1]
//--------------------------------------------------------------------------
MXC_GPIO1->int_en_set = 0x2;
MXC_GPIO1->int_mod    = 0x2;
MXC_GPIO1->int_clr    = 0x2;
NVIC_EnableIRQ(GPIO1_IRQn);
__enable_irq();

MXC_GCR->pm         |= 0x00000010; // Enable GPIO  as wake-up source
MXC_PWRSEQ->lpwken1  = 0x00000002; // Enable P1[1] as wake-up source

//---------------------------------------------------------------------------
// Definition for evaluating Low Power modes
//---------------------------------------------------------------------------
  if (BACKUP_MODE == 1) {
    // All 4 SIMO channels    on: ~19uA
    //(*(volatile unsigned int*)0x40006844) = 0x00000000;

    // VREG0_D (VTXIN/VRXIN) off: ~16.5uA (2.5uA saved)
    (*(volatile unsigned int*)0x40006844) |= 0x00000002;

    // VREGO_B (VCOREB)      off: ~10.7uA (6uA saved)
    (*(volatile unsigned int*)0x40006844) |= 0x00000001;
   
    // Switch to 60MHz oscillator in div 2 mode
    MXC_GCR->clkcn = 0x64042040;

    // Lower VCOREB and VCOREA to 0.80V (0.76V causes issues exitting)
    (*(volatile unsigned int*)0x40004408) = 0x0000001e; // VCOREB (21) (last:1e)
    (*(volatile unsigned int*)0x4000440c) = 0x0000001e; // VCOREA (21) (last:1e)
    (*(volatile unsigned int*)0x4000440c) = 0x0000001e; // VCOREA (21) (last:1e)
    (*(volatile unsigned int*)0x40004404) = 0x000000ea; // VDDA (ed) (last:ea)
    (*(volatile unsigned int*)0x40004404) = 0x000000ea; // VDDA (ed) (last:ea)
    //(*(volatile unsigned int*)0x40004410) = 0x00000038; // VTRXIN (21)
    //(*(volatile unsigned int*)0x40004410) = 0x00000038; // VTRXIN (21)

    //MXC_PWRSEQ->lpmemsd = 0x0000ff9f;

    // Enter Backup Mode
    MXC_GCR->pm |= 0x00000004;

  } else if (DEEPSLEEP_MODE == 1) {

    // Shutdown RAM
    //(*(volatile unsigned int*)0x40006840) = 0x0000139e;   // Caches and ROM

    // VREG0_D (VTXIN/VRXIN) off: ~16.5uA (2.5uA saved)
    (*(volatile unsigned int*)0x40006844) |= 0x00000002;

    // Switch to 60MHz oscillator in div 2 mode
    MXC_GCR->clkcn = 0x64042040;

    // Lower VCOREB to 0.80V
    (*(volatile unsigned int*)0x40004408) = 0x0000001e; // VCOREB (~0.8V) (last:1f)
    //(*(volatile unsigned int*)0x40004404) = 0x000000f2; // VDDA   (~1.7V)
    //(*(volatile unsigned int*)0x40004404) = 0x000000f2; // VDDA   (~1.7V)
    
    // Enter Deepsleep Mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    asm volatile ("WFI");               // Deepsleep/Background

  } else if (SLEEP_MODE == 1) {

    // Shutdown memories (caches, ROM)
    MXC_PWRSEQ->lpmemsd = 0x00007380;
    
    asm volatile ("WFI");               // Deepsleep/Background

  } else if (BACKGROUND_MODE == 1) {

    // VREG0_D (VTXIN/VRXIN) off: ~16.5uA (2.5uA saved)
    (*(volatile unsigned int*)0x40006844) |= 0x00000002;

    // Enable BACKGROUND
    MXC_PWRSEQ->lpcn |= 0x00000200;

    // Enable Deepsleep Mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;  // SLEEPDEEP=1      
  
    asm volatile ("WFI");               // Deepsleep/Background

  } else {
    // Proceed without going into any Low Power modes
    MXC_PWRSEQ->lpmemsd = 0x00007380;
    //MXC_PWRSEQ->lpmemsd = 0x0000ff9f;
    //(*(volatile unsigned int*)0x40004408) = 0x00000028; // 0.90V
  }

//---------------------------------------------------------------------------
// Line buffer instruction alignment
//---------------------------------------------------------------------------
                      // 4.14mA
asm volatile ("nop"); // 3.86mA
asm volatile ("nop"); // 3.91mA
asm volatile ("nop"); // 4.02mA
asm volatile ("nop"); // 4.05mA
asm volatile ("nop"); // 3.91
asm volatile ("nop"); // 3.95
asm volatile ("nop"); // 3.50
asm volatile ("nop"); // 3.49
//asm volatile ("nop"); // 3.87

//MXC_GCR->clkcn = 0x68082800; // div1   mode

//dumb_delay(5);
//toggle_p1_15(3);

if (DEEPSLEEP_MODE == 1) {
  goto back_to_checking_p1_9;
}
//asm volatile ("nop");

while(1);
return 0;
}
