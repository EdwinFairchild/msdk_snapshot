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
 * $Id: main.c 32120 2017-11-28 23:51:11Z lorne.smith $
 *
 *******************************************************************************
 */

/**
 * @file    main.c
 * @brief   USB CDC-ACM example
 * @details This project creates a virtual COM port, which loops back data sent to it.
 *          Load the project, connect a cable from the PC to the USB connector
 *          on the Evaluation Kit, and observe that the PC now recognizes a new COM port.
 *          A driver for the COM port, if needed, is located in the Driver/ subdirectory.
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "board.h"
#include "led.h"
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "cdc_acm.h"
#include "descriptors.h"

#include "pb.h"

// Added for SystemInit()
#include "icc_regs.h"
#include "pwrseq_regs.h"
extern void (* const __isr_vector[])(void);
extern void Reset_Handler(void);

// Go to DIV1 on entry to main()
#define CHANGE_TO_DIV1_ON_MAIN_ENTRY
// Go to DIV4 on entry to main()
//#define CHANGE_TO_DIV4_ON_MAIN_ENTRY
// Go to HIRC on entry to main()
//#define CHANGE_TO_HIRC_ON_MAIN_ENTRY
// Go to HIRC96 on entry to main()
//#define CHANGE_TO_HIRC96_ON_MAIN_ENTRY
// Replace standard SystemInit() with one that does not enable high speed clock.
#define REPLACE_SYSTEMINIT
// Toggle 96MHZ_EN on entry to main()
//#define TOGGLE_96MHZ_ON_OFF_AT_MAIN_ENTRY
// Run CPU off HIRC96 during usb_init(), then change to HIRC.
//#define RUN_ON_96MHZ_DURING_USB_INIT
// Pushbutton 1 behavior
//#define PUSHBUTTON1_ALL_DIVIDE_CYCLE
// Enable HIRC96 on entry to main()
//#define ENABLE_HIRC96_AT_MAIN_ENTRY

/***** Definitions *****/
#define EVENT_ENUM_COMP     MAXUSB_NUM_EVENTS
#define EVENT_REMOTE_WAKE   (EVENT_ENUM_COMP + 1)

#define BUFFER_SIZE  64

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/***** Global Data *****/
volatile int configured;
volatile int suspended;
volatile unsigned int event_flags;
int remote_wake_en;

/***** Function Prototypes *****/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata);
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int event_callback(maxusb_event_t evt, void *data);
static void usb_app_sleep(void);
static void usb_app_wakeup(void);
static int usb_read_callback(void);
static void echo_usb(void);

/***** File Scope Variables *****/

/* This EP assignment must match the Configuration Descriptor */
static const acm_cfg_t acm_cfg = {
  1,                  /* EP OUT */
  MXC_USBHS_MAX_PACKET, /* OUT max packet size */
  2,                  /* EP IN */
  MXC_USBHS_MAX_PACKET, /* IN max packet size */
  3,                  /* EP Notify */
  MXC_USBHS_MAX_PACKET, /* Notify max packet size */
};

static volatile int usb_read_complete;

int usb_startup_cb()
{
    const sys_cfg_usbhs_t sys_usbhs_cfg = NULL;
    return SYS_USBHS_Init(&sys_usbhs_cfg);
}

int usb_shutdown_cb()
{
    return SYS_USBHS_Shutdown();
}

/* User-supplied function to delay usec micro-seconds */
void delay_us(unsigned int usec)
{
    /* mxc_delay() takes unsigned long, so can't use it directly */
    mxc_delay(usec);
}

#ifdef REPLACE_SYSTEMINIT
/* This function is called just before control is transferred to main().
 *
 * You may over-ride this function in your program by defining a custom 
 *  SystemInit(), but care should be taken to reproduce the initialization
 *  steps or a non-functional system may result.
 */
void SystemInit(void)
{
    /* Configure the interrupt controller to use the application vector 
     * table in flash. Initially, VTOR points to the ROM's table.
     */
    SCB->VTOR = (unsigned long)&__isr_vector;
   
#if 0 
    // Switch to fast clock on startup
    MXC_GCR->clkcn &= ~(MXC_S_GCR_CLKCN_PSC_DIV128);
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_RDY));
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
    
    // Wait for clock switch and disable unused clocks
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));
    MXC_GCR->clkcn &= ~(MXC_F_GCR_CLKCN_HIRC_EN);
#endif

    /* Enable FPU on Cortex-M4, which occupies coprocessor slots 10 & 11
     * Grant full access, per "Table B3-24 CPACR bit assignments".
     * DDI0403D "ARMv7-M Architecture Reference Manual"
     */
    SCB->CPACR |= SCB_CPACR_CP10_Msk | SCB_CPACR_CP11_Msk;
    __DSB();
    __ISB();

    // Initialize backup mode entry point to safe default value.
    MXC_PWRSEQ->gp0 = (uint32_t)(Reset_Handler) | 1;

    // FIXME Pre-production parts: Enable TME, disable ICache Read Buffer, disable TME
    *(uint32_t *)0x40000c00 = 1;
    *(uint32_t *)0x4000040c = (1<<6);
    *(uint32_t *)0x40000c00 = 0;

    // Flush and enable instruction cache
    MXC_ICC0->invalidate = 1;
    while (!(MXC_ICC0->cache_ctrl & MXC_F_ICC_CACHE_CTRL_CACHE_RDY));
    MXC_ICC0->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_CACHE_EN;
    while (!(MXC_ICC0->cache_ctrl & MXC_F_ICC_CACHE_CTRL_CACHE_RDY));

    SystemCoreClockUpdate();

    // Set all GPIO to 25K pullup mode by default
    SYS_ClockEnable(SYS_PERIPH_CLOCK_GPIO0); 
    SYS_ClockEnable(SYS_PERIPH_CLOCK_GPIO1); 
    MXC_GPIO0->vssel |= 0xFFFFFFFF;
    MXC_GPIO0->ps |= 0xFFFFFFFF;
    MXC_GPIO0->pad_cfg1 |= 0xFFFFFFFF;
    MXC_GPIO0->pad_cfg2 &= ~(0xFFFFFFFF);
    MXC_GPIO1->vssel |= 0xFFFFFFFF;
    MXC_GPIO1->ps |= 0xFFFFFFFF;
    MXC_GPIO1->pad_cfg1 |= 0xFFFFFFFF;
    MXC_GPIO1->pad_cfg2 &= ~(0xFFFFFFFF);

    Board_Init();
}
#endif

int changespeed(uint32_t speed)
{
    // Make sure both clocks are on
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    switch (speed)
    {
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC:
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC);
            break;
        case MXC_S_GCR_CLKCN_CLKSEL_HIRC96:
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_CLKSEL, MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
            break;
        default:
            return -1;
            break;
    }

    // Wait for clock switch 
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CKRDY));

    // Update system clock variables
    SystemCoreClockUpdate();

    return 0;
}

/******************************************************************************/
void button_callback(void *pb)
{
    static int i = 0;
    int count = 0;
    int button0_pressed = 0;
    int button1_pressed = 0;

    //determine if interrupt triggered by bounce or a true button press
    count = 0;
    while (PB_Get(0) && !button0_pressed) {
        count++;
    
        if (count > 1000)
            button0_pressed = 1;
    }
  
    count = 0;
    while (PB_Get(1) && !button1_pressed) {
        count++;
    
        if (count > 1000)
            button1_pressed = 1;
    }

    if (button0_pressed) {
        // Make sure both clocks are on
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC_EN;
        MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
        if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL) == MXC_S_GCR_CLKCN_CLKSEL_HIRC) {
            printf("switch to high\n");
            changespeed(MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
        } else {
            printf("switch to low\n");
            changespeed(MXC_S_GCR_CLKCN_CLKSEL_HIRC);
        }

        // Reinit UART at new clock
        Console_Init();

        // Light up LED to show new speed
        if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_CLKSEL) == MXC_S_GCR_CLKCN_CLKSEL_HIRC) {
            MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_HIRC96M_EN;
            printf("At low speed: %d\n",SystemCoreClock);
            LED_Off(1);
        } else {
            printf("At high speed: %d\n",SystemCoreClock);
            LED_On(1);
        }
    }

    if (button1_pressed) {
#ifdef PUSHBUTTON1_ALL_DIVIDE_CYCLE
            printf("new code\n");
        static int direction = 1;
        uint32_t newdivide = MXC_S_GCR_CLKCN_PSC_DIV1;
        switch (MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC)
        {
            case MXC_S_GCR_CLKCN_PSC_DIV1:
                newdivide = MXC_S_GCR_CLKCN_PSC_DIV2;
                direction = 1;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV2:
                newdivide = (direction)?MXC_S_GCR_CLKCN_PSC_DIV4:MXC_S_GCR_CLKCN_PSC_DIV1;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV4:
                direction = 0; // Oops, USB gets broken at 24MHz.
                newdivide = (direction)?MXC_S_GCR_CLKCN_PSC_DIV8:MXC_S_GCR_CLKCN_PSC_DIV2;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV8:
                newdivide = (direction)?MXC_S_GCR_CLKCN_PSC_DIV16:MXC_S_GCR_CLKCN_PSC_DIV4;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV16:
                newdivide = (direction)?MXC_S_GCR_CLKCN_PSC_DIV32:MXC_S_GCR_CLKCN_PSC_DIV8;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV32:
                newdivide = (direction)?MXC_S_GCR_CLKCN_PSC_DIV64:MXC_S_GCR_CLKCN_PSC_DIV16;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV64:
                newdivide = (direction)?MXC_S_GCR_CLKCN_PSC_DIV128:MXC_S_GCR_CLKCN_PSC_DIV32;
                break;
            case MXC_S_GCR_CLKCN_PSC_DIV128:
                newdivide = MXC_S_GCR_CLKCN_PSC_DIV64;
                direction = 0;
                break;
        }
        MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, newdivide);
#else // PUSHBUTTON1_ALL_DIVIDE_CYCLE
        if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC) == MXC_S_GCR_CLKCN_PSC_DIV2) {
            printf("switch to DIV1\n");
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV1);
        } else {
            printf("switch to DIV2\n");
            MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV2);
        }
#endif // PUSHBUTTON1_ALL_DIVIDE_CYCLE

        // Update system clock variables
        SystemCoreClockUpdate();
        // Reinit UART at new clock
        Console_Init();

        printf("New speed: %d\n",SystemCoreClock);
        // Light up LED to show new speed
        if ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_PSC) == MXC_S_GCR_CLKCN_PSC_DIV2) {
            LED_Off(0);
        } else {
            LED_On(0);
        }
    }
}

// USB works (enumerates)
// CPU on HIRC96 DIV1
// CPU on HIRC96 DIV2
// CPU initialize on HIRC96 (DIV1), then switch to HIRC (DIV1)

// USB broken (no enumeration)
// CPU on HIRC DIV1 (sometimes breaks)
// CPU on HIRC DIV2

// USB broken (non-enumeration failure)
// CPU on HIRC96, then DIV4, data transfer stops
// CPU on HIRC96 (DIV1) data xfer OK, then HIRC (DIV1) data xfer OK, then DIV2 data transfer stops

/******************************************************************************/
int main(void)
{
    maxusb_cfg_options_t usb_opts;

#ifdef CHANGE_TO_DIV1_ON_MAIN_ENTRY
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV1);
    // Update system clock variables
    SystemCoreClockUpdate();
    // Reinit UART at new clock
    Console_Init();
#endif // CHANGE_TO_DIV1_ON_MAIN_ENTRYS

#ifdef CHANGE_TO_DIV4_ON_MAIN_ENTRY
    MXC_SETFIELD(MXC_GCR->clkcn, MXC_F_GCR_CLKCN_PSC, MXC_S_GCR_CLKCN_PSC_DIV4);
    // Update system clock variables
    SystemCoreClockUpdate();
    // Reinit UART at new clock
    Console_Init();
#endif // CHANGE_TO_DIV1_ON_MAIN_ENTRYS

#ifdef ENABLE_HIRC96_AT_MAIN_ENTRY
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    // Wait for 96MHz to start 
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_RDY));
#endif

#ifdef TOGGLE_96MHZ_ON_OFF_AT_MAIN_ENTRY
    MXC_GCR->clkcn |= MXC_F_GCR_CLKCN_HIRC96M_EN;
    // Wait for 96MHz to start 
    while (!(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_RDY));
    MXC_GCR->clkcn &= ~MXC_F_GCR_CLKCN_HIRC96M_EN;
    // Wait for 96MHz to stop
    while ((MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_RDY));
#endif

#ifdef CHANGE_TO_HIRC_ON_MAIN_ENTRY
    // Immediately change to low speed.
    changespeed(MXC_S_GCR_CLKCN_CLKSEL_HIRC);
    // Reinit UART at new clock
    Console_Init();
#endif

#ifdef CHANGE_TO_HIRC96_ON_MAIN_ENTRY
    // Immediately change to low speed.
    changespeed(MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
    // Reinit UART at new clock
    Console_Init();
#endif

    printf("\n\n***** " TOSTRING(TARGET) " CPU on CRYPTO/HIRC clock USB Test *****\n");
    printf("SystemCoreClock: %d\n",SystemCoreClock);
    printf("CLKCN: %08X\n",MXC_GCR->clkcn);
    printf("SCON: %08X\n",MXC_GCR->scon);
    printf("HIRC96 is: %s\n",(MXC_GCR->clkcn & MXC_F_GCR_CLKCN_HIRC96M_EN)?"on":"off");

    printf("Waiting for VBUS...\n");

    /* Initialize state */
    configured = 0;
    suspended = 0;
    event_flags = 0;
    remote_wake_en = 0;

    /* Start out in full speed */
    usb_opts.enable_hs = 0;
    usb_opts.delay_us = delay_us; /* Function which will be used for delays */
    usb_opts.init_callback = usb_startup_cb;
    usb_opts.shutdown_callback = usb_shutdown_cb;

#ifdef RUN_ON_96MHZ_DURING_USB_INIT
    changespeed(MXC_S_GCR_CLKCN_CLKSEL_HIRC96);
#endif // RUN_ON_96MHZ_DURING_USB_INIT
    /* Initialize the usb module */
    if (usb_init(&usb_opts) != 0) {
        printf("usb_init() failed\n");
        while (1);
    }

    /* Initialize the enumeration module */
    if (enum_init() != 0) {
        printf("enum_init() failed\n");
        while (1);
    }

    /* Register enumeration data */
    enum_register_descriptor(ENUM_DESC_DEVICE, (uint8_t*)&device_descriptor, 0);
    enum_register_descriptor(ENUM_DESC_CONFIG, (uint8_t*)&config_descriptor, 0);
    enum_register_descriptor(ENUM_DESC_STRING, lang_id_desc, 0);
    enum_register_descriptor(ENUM_DESC_STRING, mfg_id_desc, 1);
    enum_register_descriptor(ENUM_DESC_STRING, prod_id_desc, 2);

    /* Handle configuration */
    enum_register_callback(ENUM_SETCONFIG, setconfig_callback, NULL);

    /* Handle feature set/clear */
    enum_register_callback(ENUM_SETFEATURE, setfeature_callback, NULL);
    enum_register_callback(ENUM_CLRFEATURE, clrfeature_callback, NULL);

    /* Initialize the class driver */
    if (acm_init() != 0) {
        printf("acm_init() failed\n");
        while (1);
    }

    /* Register callbacks */
    usb_event_enable(MAXUSB_EVENT_NOVBUS, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_VBUS, event_callback, NULL);
    acm_register_callback(ACM_CB_READ_READY, usb_read_callback);
    usb_read_complete = 0;

    /* Start with USB in low power mode */
    usb_app_sleep();
    NVIC_EnableIRQ(USB_IRQn);

#ifdef RUN_ON_96MHZ_DURING_USB_INIT
    changespeed(MXC_S_GCR_CLKCN_CLKSEL_HIRC);
#endif // RUN_ON_96MHZ_DURING_USB_INIT

    /* Register callback for keyboard events */
    if (PB_RegisterCallback(0, button_callback) != E_NO_ERROR) {
        printf("PB_RegisterCallback() failed\n");
        while (1);
    }
    if (PB_RegisterCallback(1, button_callback) != E_NO_ERROR) {
        printf("PB_RegisterCallback() failed\n");
        while (1);
    }

    /* Wait for events */
    while (1) {

        echo_usb();

        if (suspended || !configured) {
            LED_Off(0);
        } else {
            LED_On(0);
        }

        if (event_flags) {
            /* Display events */
            if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_NOVBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_NOVBUS);
                printf("VBUS Disconnect\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_VBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_VBUS);
                printf("VBUS Connect\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_BRST)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_BRST);
                printf("Bus Reset\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_SUSP)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_SUSP);
                printf("Suspended\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_DPACT)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_DPACT);
                printf("Resume\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_ENUM_COMP)) {
                MXC_CLRBIT(&event_flags, EVENT_ENUM_COMP);
                printf("Enumeration complete. Waiting for characters...\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_REMOTE_WAKE)) {
                MXC_CLRBIT(&event_flags, EVENT_REMOTE_WAKE);
                printf("Remote Wakeup\n");
            }
        }
    }
}

/******************************************************************************/
static void echo_usb(void)
{
  int chars;
  uint8_t buffer[BUFFER_SIZE];
  
  if ((chars = acm_canread()) > 0) {

    if (chars > BUFFER_SIZE) {
      chars = BUFFER_SIZE;
    }

    // Read the data from USB
    if (acm_read(buffer, chars) != chars) {
      printf("acm_read() failed\n");
      return;
    }

    // Echo it back
    if (acm_present()) {
      if (acm_write(buffer, chars) != chars) {
        printf("acm_write() failed\n");
      }
    }
  }
}

/******************************************************************************/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata)
{
    /* Confirm the configuration value */
    if (sud->wValue == config_descriptor.config_descriptor.bConfigurationValue) {
        configured = 1;
        MXC_SETBIT(&event_flags, EVENT_ENUM_COMP);
        return acm_configure(&acm_cfg); /* Configure the device class */
    } else if (sud->wValue == 0) {
        configured = 0;
        return acm_deconfigure();
    }

    return -1;
}

/******************************************************************************/
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    if(sud->wValue == FEAT_REMOTE_WAKE) {
        remote_wake_en = 1;
    } else {
        // Unknown callback
        return -1;
    }

    return 0;
}

/******************************************************************************/
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    if(sud->wValue == FEAT_REMOTE_WAKE) {
        remote_wake_en = 0;
    } else {
        // Unknown callback
        return -1;
    }
    
    return 0;
}

/******************************************************************************/
static void usb_app_sleep(void)
{
    /* TODO: Place low-power code here */
    suspended = 1;
}

/******************************************************************************/
static void usb_app_wakeup(void)
{
    /* TODO: Place low-power code here */
    suspended = 0;
}

/******************************************************************************/
static int event_callback(maxusb_event_t evt, void *data)
{
    /* Set event flag */
    MXC_SETBIT(&event_flags, evt);

    switch (evt) {
        case MAXUSB_EVENT_NOVBUS:
            usb_event_disable(MAXUSB_EVENT_BRST);
            usb_event_disable(MAXUSB_EVENT_SUSP);
            usb_event_disable(MAXUSB_EVENT_DPACT);
            usb_disconnect();
            configured = 0;
            enum_clearconfig();
            acm_deconfigure();
            usb_app_sleep();
            break;
        case MAXUSB_EVENT_VBUS:
            usb_event_clear(MAXUSB_EVENT_BRST);
            usb_event_enable(MAXUSB_EVENT_BRST, event_callback, NULL);
            usb_event_clear(MAXUSB_EVENT_SUSP);
            usb_event_enable(MAXUSB_EVENT_SUSP, event_callback, NULL);
            usb_connect();
            usb_app_sleep();
            break;
        case MAXUSB_EVENT_BRST:
            usb_app_wakeup();
            enum_clearconfig();
            acm_deconfigure();
            configured = 0;
            suspended = 0;
            break;
        case MAXUSB_EVENT_SUSP:
            usb_app_sleep();
            break;
        case MAXUSB_EVENT_DPACT:
            usb_app_wakeup();
            break;
        default:
            break;
    }

    return 0;
}


/******************************************************************************/
static int usb_read_callback(void)
{
    usb_read_complete = 1;
    return 0;
}

/******************************************************************************/
void USB_IRQHandler(void)
{
    usb_event_handler();
}

/******************************************************************************/
void SysTick_Handler(void)
{
    mxc_delay_handler();
}
