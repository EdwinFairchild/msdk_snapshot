/**
 * @file        main.c
 * @brief       Clock test example
 * @details     
 */

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
 * $Date: 2019-05-14 09:10:58 -0700 (Tue, 14 May 2019) $
 * $Revision: 43356 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include <tmr_utils.h>
#include <gcr_regs.h>
#include <rtc_regs.h>
#include <mcr_regs.h>
#include <icc_regs.h>
#include <core_cm4.h>
#include <max32665.h>
#include <lp.h>
#include <simo.h>
#include "mxc_sys.h"
#include "mxc_config.h"
#include "board.h"
#include "gpio.h"
#include "max32665.h"

#include "pwrseq_regs.h"
#include "usbhs_regs.h"
#include "gcr_regs.h"

/***** Definitions *****/
#undef USE_UART
#define SET_LOCK_PAD_STATE (GPIO_PAD_NONE) // High impedance
//#define SET_LOCK_PAD_STATE (GPIO_PAD_PULL_UP)
#define USE_DISABLE_CLOCKS
#define USE_DISABLE_PER_CLOCKS
#define USE_DISABLE_RAM
//#define USE_LOCK
#define CORE_VOLTAGE(x)                (((x) == 0) ? (1000) : (810))
#define USE_RST_PULLUP (1)
//#define USE_DISABLE_ICACHE
#define USE_USB_PHY_SUSPEND
#define USE_QUIET_RTC

#define STATE_ENTRY (0)
#define STATE_INIT (0)
#define STATE_COUNT (3)
#define STATE_96_0_MHZ (0)
#define STATE_12_0_MHZ (1)
#define STATE_7_4_MHZ (2)

#define GPIO_PORT_INTERRUPT_IN      PORT_1
#define GPIO_PIN_INTERRUPT_IN       PIN_7 
#define GPIO_INDEX_INTERRUPT_IN     (0)

#define GPIO_PORT_LOCK_IN           PORT_1
#define GPIO_PIN_LOCK_IN            PIN_6
#define GPIO_INDEX_LOCK_IN          (1)

#define GPIO_COUNT_INPUT            (2)

#define GPIO_COUNT_CLOCK_STATE      (2)
#define GPIO_PORT_CLOCK_STATE(i)    (PORT_1)
#define GPIO_PIN_CLOCK_STATE(i)     ((i == 0) ? (PIN_14) : \
                                                (PIN_15))
#define DISPLAY_DELAY               (get_clock() >> 5)
#define DEBOUNCE_DELAY              (DISPLAY_DELAY >> 2);

typedef struct clock_state_t
{
    unsigned int state;
    volatile unsigned int lockout_counter;
    volatile unsigned int interrupt_enabled;
    gpio_cfg_t gpio[GPIO_COUNT_CLOCK_STATE + GPIO_COUNT_INPUT];
} CLOCK_STATE, *P_CLOCK_STATE;

/***** Globals *****/

/***** Functions *****/
unsigned int get_clock(void)
{
    mxc_gcr_regs_t *gcr_regs = MXC_GCR;
    volatile uint32_t * p_clkcn_reg = &(gcr_regs->clkcn);
    uint32_t clkcn_val = *p_clkcn_reg ;

    unsigned int clock_div;
    unsigned int clock_osc;

    clock_div =
            ((clkcn_val & MXC_F_GCR_CLKCN_PSC) >> MXC_F_GCR_CLKCN_PSC_POS);

    switch ((clkcn_val & MXC_F_GCR_CLKCN_CLKSEL) >> MXC_F_GCR_CLKCN_CLKSEL_POS)
    {
        case MXC_V_GCR_CLKCN_CLKSEL_XTAL32M:
            clock_osc = 32000000UL;
            break;
        case MXC_V_GCR_CLKCN_CLKSEL_LIRC8:
            clock_osc = 8000UL;
            break;
        case MXC_V_GCR_CLKCN_CLKSEL_HIRC96:
            clock_osc = 96000000UL;
            break;
        case MXC_V_GCR_CLKCN_CLKSEL_HIRC8:
            clock_osc = 7372800UL;
            break;
        case MXC_V_GCR_CLKCN_CLKSEL_XTAL32K:
            clock_osc = 32768UL;
            break;
        case MXC_V_GCR_CLKCN_CLKSEL_HIRC: ;
        default:
            clock_osc = 60000000UL;
    }
    return (clock_osc >> clock_div);
}

#ifdef USE_UART
void enable_uart_clock(void)
{
    mxc_gcr_regs_t *gcr_regs = MXC_GCR;
    volatile uint32_t * p_clkcn_reg = &(gcr_regs->clkcn);

    *p_clkcn_reg |= MXC_F_GCR_CLKCN_HIRC8M_EN;
    while((*p_clkcn_reg & MXC_F_GCR_CLKCN_HIRC8M_RDY) == 0);
}
#endif /* USE_UART */

void select_clock_config(unsigned int state)
{
    mxc_gcr_regs_t *gcr_regs = MXC_GCR;
    volatile uint32_t * p_clkcn_reg = &(gcr_regs->clkcn);
    uint32_t clkcn_val;
    uint32_t clkcn_assn_val, clkcn_assn_mask;
    uint32_t clkcn_rdy_mask;

    // Enable clocks that are required.
    switch(state)
    {
        case STATE_96_0_MHZ: ;
        case STATE_12_0_MHZ:
            clkcn_assn_mask = MXC_F_GCR_CLKCN_HIRC96M_EN;
            clkcn_assn_val = MXC_F_GCR_CLKCN_HIRC96M_EN;
            clkcn_rdy_mask = MXC_F_GCR_CLKCN_HIRC96M_RDY;
            break;
        case STATE_7_4_MHZ:
            clkcn_assn_mask = MXC_F_GCR_CLKCN_HIRC8M_EN;
            clkcn_assn_val = MXC_F_GCR_CLKCN_HIRC8M_EN;
            clkcn_rdy_mask = MXC_F_GCR_CLKCN_HIRC8M_RDY;
            break;
        default:
            return;
    }
    if ((*p_clkcn_reg & clkcn_assn_mask) != clkcn_assn_val)
    {
        *p_clkcn_reg |= clkcn_assn_val;
    }
    while ((*p_clkcn_reg & clkcn_rdy_mask) == 0);

    // Select clock oscillator.
    clkcn_val = *p_clkcn_reg;
    clkcn_val &= ~MXC_F_GCR_CLKCN_CLKSEL;
    switch(state)
    {
        case STATE_96_0_MHZ: ;
        case STATE_12_0_MHZ:
            clkcn_val |= (MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
            break;
        case STATE_7_4_MHZ:
            clkcn_val |= (MXC_S_GCR_CLKCN_CLKSEL_HIRC8);
            break;
    }

    // Apply and wait.
    *p_clkcn_reg = clkcn_val;
    while ((clkcn_val & MXC_F_GCR_CLKCN_CKRDY) == 0);

    // Select clock prescaler divider.
    clkcn_val = *p_clkcn_reg;
    clkcn_val &= ~MXC_F_GCR_CLKCN_PSC;
    switch(state)
    {
        case STATE_12_0_MHZ:
            clkcn_val |= (MXC_S_GCR_CLKCN_PSC_DIV8);
            break;
        case STATE_96_0_MHZ: ;
        case STATE_7_4_MHZ:
            clkcn_val |= (MXC_S_GCR_CLKCN_PSC_DIV1);
            break;
    }

    // Apply and wait.
    *p_clkcn_reg = clkcn_val;

    // Disable unused clocks.
    // Add all clocks that may be disabled to mask.
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_HIRC96M_EN;
#ifndef USE_UART
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_HIRC8M_EN;
#endif /* USE_UART */
#ifdef USE_DISABLE_CLOCKS
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_X32M_EN;
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_X32K_EN;
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_HIRC_EN;
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    clkcn_assn_mask |= MXC_F_GCR_CLKCN_HIRC8M_EN;
#endif /* USE_DISABLE_CLOCKS */

    // Disable any clocks that aren't enabled above.
    clkcn_val = *p_clkcn_reg;
    clkcn_val = (clkcn_val & ~clkcn_assn_mask) | clkcn_assn_val;
    *p_clkcn_reg = clkcn_val;
}

#ifdef CORE_VOLTAGE
void set_core_voltage(unsigned int state)
{
    uint32_t U32Voltage;
    static uint32_t U32Last = CORE_VOLTAGE(STATE_INIT);
    unsigned int clock;
    volatile unsigned int iter;
    unsigned int delay;

    clock = get_clock();

    U32Voltage = CORE_VOLTAGE(state);

    // Make sure we don't lower core voltage until after core clock is lowered.
    if ((U32Voltage < 900) && (clock > 20000000UL)) return;

    if (U32Last != U32Voltage)
    {
        U32Last = U32Voltage;

        SIMO_setVregO_C(U32Voltage); // VCOREA
        SIMO_setVregO_B(U32Voltage); // VCOREB

        delay = (clock >> 10); // ~1ms
        for (iter = 0; iter < delay; iter ++)
        {
            if (SIMO_getOutReadyC() && SIMO_getOutReadyB()) break;
        }
    }
}
#endif /* CORE_VOLTAGE */

void gpio_isr(void *cbdata)
{
    P_CLOCK_STATE pst_clock_state;
#ifdef USE_LOCK
    gpio_cfg_t * pstGpioCfg;
#endif /* USE_LOCK */
    unsigned int gpio_clock_index;
    unsigned int delay;
    int lock_selected;
   
    /* Cast argument pointer to clock state structure passed from main(). */
    pst_clock_state = (P_CLOCK_STATE)cbdata;

    /* Read additional inputs. */
#ifdef USE_LOCK
    pstGpioCfg = &pst_clock_state->gpio[GPIO_COUNT_CLOCK_STATE + GPIO_INDEX_LOCK_IN];
    lock_selected = (GPIO_InGet(pstGpioCfg) ? 0 : 1);
#else /* USE_LOCK */
    lock_selected = 0;
#endif /* USE_LOCK */

    /* Make sure enough time has passed between button presses. */
    if (pst_clock_state->lockout_counter == 0)
    { 
        // Perform state transition if lock isn't selected.
        if (!lock_selected)
        {
            // Don't allow the button press to be processed more frequently than provided by DEBOUNCE_DELAY.
            pst_clock_state->lockout_counter = DEBOUNCE_DELAY;

            // Transition to next state.
            pst_clock_state->state ++;
            if (pst_clock_state->state >= (STATE_COUNT + STATE_INIT))
                pst_clock_state->state = STATE_INIT;

            // Make sure the clocks requried for the UART are enabled.
#ifdef USE_UART
            enable_uart_clock();
#endif /* USE_UART */

            // Also display state on console.
#ifdef USE_UART
            printf("state: %u\n", pst_clock_state->state);
            fflush(stdout);
#endif /* USE_UART */

#ifdef CORE_VOLTAGE
	    // Will raise/lower core voltage if possible.
            set_core_voltage(pst_clock_state->state);
#endif /* CORE_VOLTAGE */

            // Apply new clock.
            select_clock_config(pst_clock_state->state);

#ifdef CORE_VOLTAGE
	    // Will lower core voltage if not previously done.
            set_core_voltage(pst_clock_state->state);
#endif /* CORE_VOLTAGE */

            // Display state using LEDs.
            for (gpio_clock_index = 0; gpio_clock_index < GPIO_COUNT_CLOCK_STATE; gpio_clock_index ++)
            {
                if ((pst_clock_state->state & (1U << gpio_clock_index)) != 0)
                {
                    GPIO_OutClr(&pst_clock_state->gpio[gpio_clock_index]);
                }
                else
                {
                    GPIO_OutSet(&pst_clock_state->gpio[gpio_clock_index]);
                }
            }

            // Wait a period of time to allow user to view LEDs.
            delay = DISPLAY_DELAY;
            for(volatile int i = 0; i < delay; i++);

            // Shut off LEDs for power measuerments.
            for (gpio_clock_index = 0; gpio_clock_index < GPIO_COUNT_CLOCK_STATE; gpio_clock_index ++)
            {
                GPIO_OutSet(&pst_clock_state->gpio[gpio_clock_index]);
            }
        }
        else
        {
            pst_clock_state->interrupt_enabled = 0;
        }
    }
}

void turnOffUnused(void)
{
    SYS_ClockEnable(SYS_PERIPH_CLOCK_USB);
    MXC_USBHS->m31_phy_ponrst = 0;

    // Peripheral clock disables
    MXC_GCR->perckcn0  &= ~(0x000000003); // Clear GPIO0 and GPIO1 Disable
    //MXC_GCR->perckcn1 &= ~(0x000400800); // Clear ICACHE0 and ICACHE1 Disable

#ifdef USE_USB_PHY_SUSPEND
    // USB PHY Power On Reset and Suspend
    MXC_USBHS->m31_phy_ponrst = 0x00000000; // PONRST=0
    MXC_USBHS->mxm_suspend = 0x00000003; // SUSPEND=1

    // Miscellaneous settings
#ifdef USE_RST_PULLUP
    MXC_MCR->ctrl = 0x0000020b; // debug bit 8
#else /* USE_RST_PULLUP */
    MXC_MCR->ctrl = 0x0000000b; // debug bit 8
#endif /* USE_RST_PULLUP */
#endif /* USE_USB_PHY_SUSPEND */

#ifdef USE_QUIET_RTC
    // RTC set to Quiet Mode always (RTCCN, 0x40006010)
    MXC_RTC->ctrl = 0x00008000;
    MXC_RTC->ctrl = 0x00008801;
    MXC_RTC->ctrl = 0x00008801;
#endif /* USE_QUIET_RTC */

    // Disable all peripheral clocks
    MXC_GCR->perckcn0 = 0xFFFFFFFF;
    MXC_GCR->perckcn1 = 0xFFFFFFFF;

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

#ifdef USE_DISABLE_ICACHE
    // Cache
    MXC_ICC0->cache_ctrl = 0x00000000; // instr. cache off
#endif /* USE_DISABLE_ICACHE */
}

void turnOffConsole(void) {
    MXC_GCR->perckcn0 &= ~MXC_F_GCR_PERCKCN0_T0D;
    TMR_Delay(MXC_TMR0, MSEC(100), NULL);

    Console_Shutdown();

    MXC_GCR->perckcn0 |= MXC_F_GCR_PERCKCN0_T0D | MXC_F_GCR_PERCKCN0_UART0D;
}

void disable_peripherals(void)
{
    gpio_cfg_t stGpioCfg;
    unsigned int portIter, pinIter;
    volatile int iter;

    turnOffConsole();

    turnOffUnused();

    for (iter = 0; iter < 0xFFFFF; iter++) {}

#ifdef SET_LOCK_PAD_STATE 
    for (portIter = 0; portIter < 32; portIter ++)
    {
        stGpioCfg.port = ((portIter == 0) ? PORT_0 : PORT_1);
        for (pinIter = 0; pinIter < ((portIter == 0) ? 32 : 16); pinIter ++)
        {
            stGpioCfg.mask = (1UL << pinIter);
            stGpioCfg.pad = SET_LOCK_PAD_STATE;
            stGpioCfg.func = GPIO_FUNC_IN;

#if 0
            /* P1_12 is pulled to 3.3V on evkit. Set this pin to high impedance. */
            if (portIter == 1 && pinIter == 12)
                stGpioCfg.pad = GPIO_PAD_NONE;
#endif /* 0 */

            GPIO_Config(&stGpioCfg);
        }
    }
#endif /* SET_LOCK_PAD_STATE  */
}

int main(void)
{
    CLOCK_STATE st_clock_state;
    gpio_cfg_t * pstGpioCfg;
    unsigned int gpio_index;
#ifdef USE_LOCK
    unsigned int lockout_counter;
#endif /* USE_LOCK */
    
#ifdef USE_UART
    printf("\n\n***** Clock Test ******\n\n");
    printf("1. An interrupt is set up on P1.07 (SW3).\n");
    printf("P1.14 (LED0) and P1.15 (LED1) display clock state as follows:\n");
    printf("0) LED0<OFF> LED1<OFF> 96MHz\n");
    printf("1) LED0<ON>  LED1<OFF> 12MHz\n");
    printf("2) LED0<OFF> LED1<ON>  7.3728MHz\n\n");
    printf("If P1.06 is held low when P1.07 is grounded,\n");
    printf("then all further input is disabled.\n\n");
#endif /* USE_UART */
    
    /* Setup clock state structure (non-gpio related). */
    memset(&st_clock_state, 0, sizeof(st_clock_state));
    st_clock_state.lockout_counter = 0;
#ifdef USE_LOCK
    st_clock_state.interrupt_enabled = 1;
#else /* USE_LOCK */
    st_clock_state.interrupt_enabled = 0;
#endif /* USE_LOCK */
    st_clock_state.state = STATE_ENTRY;

    // Apply initial clock. (96 MHz)
    select_clock_config(st_clock_state.state);

    /* Setup clock state led indication pins as an output so we can set on each interrupt. */
    for (gpio_index = 0; gpio_index < GPIO_COUNT_CLOCK_STATE; gpio_index ++)
    {
        st_clock_state.gpio[gpio_index].port = GPIO_PORT_CLOCK_STATE(gpio_index);
        st_clock_state.gpio[gpio_index].mask = GPIO_PIN_CLOCK_STATE(gpio_index);
#ifdef USE_LOCK
        st_clock_state.gpio[gpio_index].pad = GPIO_PAD_NONE;
        st_clock_state.gpio[gpio_index].func = GPIO_FUNC_OUT;
        GPIO_Config(&st_clock_state.gpio[gpio_index]);
        GPIO_OutSet(&st_clock_state.gpio[gpio_index]);
#endif /* USE_LOCK */
    }
    
    /* Set P1.06 up as a non-interrupt input pin. */
    pstGpioCfg = &st_clock_state.gpio[GPIO_COUNT_CLOCK_STATE + GPIO_INDEX_LOCK_IN];
    pstGpioCfg->port = GPIO_PORT_LOCK_IN;
    pstGpioCfg->mask = GPIO_PIN_LOCK_IN;
#ifdef USE_LOCK
    pstGpioCfg->pad = GPIO_PAD_PULL_UP;
    pstGpioCfg->func = GPIO_FUNC_IN;
    GPIO_Config(pstGpioCfg);
#endif /* USE_LOCK */

    /*   Set up interrupt on P1.07.
     *   Switch on EV kit is open when non-pressed, and grounded when pressed.  Use an internal pull-up so pin 
     *     reads high when button is not pressed. 
     */
    pstGpioCfg = &st_clock_state.gpio[GPIO_COUNT_CLOCK_STATE + GPIO_INDEX_INTERRUPT_IN];
    pstGpioCfg->port = GPIO_PORT_INTERRUPT_IN;
    pstGpioCfg->mask = GPIO_PIN_INTERRUPT_IN;
#ifdef USE_LOCK
    pstGpioCfg->pad = GPIO_PAD_PULL_UP;
    pstGpioCfg->func = GPIO_FUNC_IN;
    GPIO_Config(pstGpioCfg);
    GPIO_RegisterCallback(pstGpioCfg, gpio_isr, &st_clock_state);
    GPIO_IntConfig(pstGpioCfg, GPIO_INT_EDGE, GPIO_INT_FALLING);
    GPIO_IntEnable(pstGpioCfg);
    NVIC_EnableIRQ(MXC_GPIO_GET_IRQ(GPIO_PORT_INTERRUPT_IN));
#endif /* USE_LOCK */

#ifdef USE_LOCK
    /* Decrement lockout counter whenever set by interrupt. */
    while (st_clock_state.interrupt_enabled)
    {
        lockout_counter = st_clock_state.lockout_counter;
        if (lockout_counter > 0)
            st_clock_state.lockout_counter = lockout_counter - 1;
    }
#endif /* USE_LOCK */

    /* Disable interrupts. */
    NVIC_DisableIRQ(MXC_GPIO_GET_IRQ(GPIO_PORT_INTERRUPT_IN));
    pstGpioCfg = &st_clock_state.gpio[GPIO_COUNT_CLOCK_STATE + GPIO_INDEX_INTERRUPT_IN];
    GPIO_IntDisable(pstGpioCfg);

#ifdef SET_LOCK_PAD_STATE
    /* Configure all pins as high impedance. */
    for (gpio_index = 0; gpio_index < (GPIO_COUNT_CLOCK_STATE + GPIO_COUNT_INPUT); gpio_index ++)
    {
        pstGpioCfg = &st_clock_state.gpio[gpio_index];
        pstGpioCfg->pad = SET_LOCK_PAD_STATE;
        pstGpioCfg->func = GPIO_FUNC_IN;
        GPIO_Config(pstGpioCfg);
    }
#endif /* SET_LOCK_PAD_STATE */

#ifndef USE_LOCK
    st_clock_state.state = 0;

    pstGpioCfg = &st_clock_state.gpio[GPIO_COUNT_CLOCK_STATE + GPIO_INDEX_LOCK_IN];
    if (GPIO_InGet(pstGpioCfg))
    {
        st_clock_state.state |= 1;
    }

    pstGpioCfg = &st_clock_state.gpio[GPIO_COUNT_CLOCK_STATE + GPIO_INDEX_INTERRUPT_IN];
    if (GPIO_InGet(pstGpioCfg))
    {
        st_clock_state.state |= 2;
    }

    select_clock_config(st_clock_state.state);
#endif /* USE_LOCK */

#ifdef CORE_VOLTAGE
    /* Lower core voltage if possible. */
    set_core_voltage(st_clock_state.state);
#endif /* CORE_VOLTAGE */

#ifdef USE_DISABLE_PER_CLOCKS
    /* Disable all unnecessary peripheral clocks. */
    disable_peripherals();
#endif /* USE_DISABLE_PER_CLOCKS */

#ifdef USE_DISABLE_RAM
    MXC_PWRSEQ->lpmemsd = 0x00007380;
#endif /* USE_DISABLE_RAM */

    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    asm volatile ("nop");
    //asm volatile ("nop");

    /* Once lock is selected, only spin. */
    while(1);
}

