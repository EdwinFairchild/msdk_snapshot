#include "mxc_config.h"
#include "tmr_regs.h"
#include <stdint.h>
#include <stdio.h>
#include "tmr_utils.h"
#include "icc.h"
#include "lp.h"
#include "mxc_sys.h"
#include "gcr_regs.h"
#include "pwrseq_regs.h"

// int SYS_Clock_Select(sys_system_clock_t clock, mxc_tmr_regs_t* tmr);

void display_banner(void);
void init_uart(void);
void init_icc_cache(int icacheEnable);
void execution_space_identify(void);
void debug_print_timer_loop(void);
void platform_wait_state(uint32_t state);
void output_clockMHZ_gpio(void);
void clock_gating_all_off(void);
void display_GCR_CLKCN_MEMCKCN(void);
void LowPowerShutdowns(void);
//
// platform_start_time
//
void platform_start_time(void) {

  MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
  // MXC_GCR->clkcn |= MXC_S_GCR_CLKCN_PSC_DIV4;
  SYS_Clock_Select(SYS_CLOCK_HIRC96, MXC_TMR0);
  
  Console_Init();

  display_banner();
  platform_wait_state(WAITSTATE);
  init_icc_cache(ICACHE);
  execution_space_identify();
  display_GCR_CLKCN_MEMCKCN();

  printf("SystemCoreClock %u\n", (unsigned int)SystemCoreClock);
  printf("PeripheralClock %u\n", (unsigned int)PeripheralClock);

  printf("Start the benchmark timer\n");
  LowPowerShutdowns();
  TMR_SW_Start(MXC_TMR0, NULL);

#if 0
  debug_print_timer_loop();
#endif
}

//
// platform_stop_time
//
uint32_t platform_stop_time(void) { return TMR_SW_Stop(MXC_TMR0); }

void LowPowerShutdowns(void) {
    // // Disable all peripheral clocks
    // MXC_GCR->perckcn0 |= (0xFFFFFFFF ^ (MXC_F_GCR_PERCKCN0_UART0D | MXC_F_GCR_PERCKCN0_T0D));
    MXC_GCR->perckcn0 |= 0xFFFF7BFF;
    MXC_GCR->perckcn1 |= 0xFFFFFFFF;
    // MXC_GCR->perckcn0 &= ~MXC_F_GCR_PERCKCN0_T0D;
    // MXC_GCR->perckcn0 &= ~MXC_F_GCR_PERCKCN0_UART0D;

    *(uint32_t *) 0x400B1410 = 0;
    *(uint32_t *) 0x400014A0 = 3;

    // Allow the USB Switch to be turned off in deepsleep and backup modes
    LP_USBSWLPDisable();

    // Shut down all unneeded power gates
    LP_ICacheXIPShutdown();
    LP_CryptoShutdown();
    LP_SysCacheShutdown();
    LP_USBFIFOShutdown();
    LP_ROMShutdown();
    LP_ROM1Shutdown();
    LP_ICache1Shutdown();
    LP_SysRam1Shutdown();
    LP_SysRam2Shutdown();
    LP_SysRam3Shutdown();
    LP_SysRam4Shutdown();
    LP_SysRam5Shutdown();
}

void display_banner(void) {
  printf("-------------------------------------------------\n");
  printf("--- ME14 Coremark Performance Test\n");
  printf("-------------------------------------------------\n");
}

void init_icc_cache(int icacheEnable) {
  if (icacheEnable)
    ICC_Enable();
  else
    ICC_Disable();
  printf("ICC cache is %s\n", (MXC_ICC->cache_ctrl & MXC_S_ICC_CACHE_CTRL_CACHE_EN_EN) ? "Enabled" : "Disabled");
}

void execution_space_identify(void) {
  unsigned int address;

  // Identify where execution is
  address = (unsigned int)&execution_space_identify;
  if ((address & 0xFF000000) == 0x00000000) {
    printf("Executing out of FLASH (0x%08x)\n", (address & 0xFF000000));
  } else
  if ((address & 0xFF000000) == 0x20000000) {
    printf("Executing out of SRAM (0x%08x)\n", (address & 0xFF000000));
  } else {
    printf("Executing from here (0x%08x)\n", address);
  }

  // Identify where the stack is
  address = (unsigned int)&address;
  if ((address & 0xFF000000) == 0x20000000) {
    printf("Stack is located in SRAM (0x%08x)\n", (address & 0xFF000000));
  } else {
    printf("Stack is here (0x%08x)\n", address);
  }
}

void debug_print_timer_loop(void) {
  uint32_t tick;
  float time;
  while (1) {
    tick = TMR_TO_Elapsed(MXC_TMR0);
    time = (float)tick / 1000000.0f;
    printf("time in seconds = %3.3f, tick = %ud\n",time,tick);
  }
}


void output_clockMHZ_gpio(void)
{ 
  volatile int delay = 100000;
  printf("not setting clock...\n");
  printf("send default clock / 8 to GPIO...\n");
  printf("spin...\n");

/*  gpio_cfg_t gpio1_0 = {PORT_1, PIN_0, GPIO_FUNC_OUT, GPIO_PAD_NONE};
  GPIO_Config(&gpio1_0);
  while (1) { 
    GPIO_OutClr(&gpio1_0);
    GPIO_OutSet(&gpio1_0);
  }*/
  while (delay > 0) delay--;

  *(uint32_t *)0x40000C00 = 1;            // TME=1
  *(uint32_t *)0x40000C08=0x00008002;   //Bring clock out to GPIO1.0
//  *(uint32_t *)0x40005418 = value;        // Set HIRC96M to 96MHz
  *(uint32_t *)0x40000008=0x6c0ca8c8;   // switch to HIRC96M/8
  //*(uint32_t *)0x40000008 = 0x6c0ca808;   // switch to HIRC96M
  while (1) { }
}



// MEMCKCN 2:0   - Flash Wait-State
void platform_wait_state(uint32_t state) {
  uint32_t value;

  value = MXC_GCR->memckcn & ~MXC_F_GCR_MEMCKCN_FWS;
  value |= state << MXC_F_GCR_MEMCKCN_FWS_POS;
  MXC_GCR->memckcn = value;
  printf("Waitstate is %d\n", MXC_GCR->memckcn & MXC_F_GCR_MEMCKCN_FWS);
}

void clock_gating_all_off(void) {
  printf("turning all clock gating off\n");
  MXC_GCR->memckcn  = 0x0;
  MXC_GCR->perckcn0 = 0x0;
  MXC_GCR->perckcn1 = 0x0;
  MXC_PWRSEQ->lpmemsd = 0x0;
}

void display_GCR_CLKCN_MEMCKCN(void) {
  printf("GCR CLKCN   (offset 0x%02x) = 0x%08x\n", ((uint32_t)&(MXC_GCR->clkcn))   - (uint32_t)MXC_GCR, MXC_GCR->clkcn);
  printf("GCR MEMCKCN (offset 0x%02x) = 0x%08x\n", ((uint32_t)&(MXC_GCR->memckcn)) - (uint32_t)MXC_GCR, MXC_GCR->memckcn);
}
