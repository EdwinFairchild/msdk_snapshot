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
 * $Id: main.c 36662 2018-08-06 17:57:26Z bryan.armstrong $
 *
 *******************************************************************************
 */

/**
 * @file    main.c
 * @brief   USB CDC-ACM loopback example
 * @details Creates a USB "virtual loopback" serial device, which will echo anything sent to it
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "board.h"
#include "led.h"
#include "uart.h"
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "cdc_acm.h"
#include "descriptors.h"
#include "pb.h"
#include "tmr_utils.h"
//#include "logger.h"

/***** Definitions *****/
#if (CONSOLE_UART == 0)
#define MXC_UARTn           MXC_UART1
#define UARTn_IRQn          UART1_IRQn
#define UARTn_IRQHandler    UART1_IRQHandler
#else
#define MXC_UARTn           MXC_UART0
#define UARTn_IRQn          UART0_IRQn
#define UARTn_IRQHandler    UART0_IRQHandler
#endif

#define EVENT_ENUM_COMP     MAXUSB_NUM_EVENTS
#define EVENT_REMOTE_WAKE   (EVENT_ENUM_COMP + 1)

#define BUFFER_SIZE  64

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/* Define this to disable all printf()s for simulation, etc. */
//#define printf(...) {}

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

void usb_dump_log(void *);

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

static uint8_t uart_tx_data[MXC_UART_FIFO_DEPTH];
static volatile int usb_read_complete;
static volatile int uart_read_complete;

/******************************************************************************/
void pb_wakeup(void *pb)
{
  printf("Wake!\n");
    usb_remote_wakeup();
}

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
    TMR_Delay(MXC_TMR0, usec, NULL);
}

/******************************************************************************/
int main(void)
{
    maxusb_cfg_options_t usb_opts;
/*
    while(1)
    {
	printf("UUU\n");
    }
 */   
    printf("\n\n***** " TOSTRING(TARGET) " USB CDC-ACM Example *****\n");

    printf("USB version %u.%u / EPInfo (%u IN, %u OUT) / RAMInfo (%d-bit bus, %d DMA channels)\n",
	   MXC_USBHS->hwvers >> 10, MXC_USBHS->hwvers & 0x3ff,
	   MXC_USBHS->epinfo & 0xf, MXC_USBHS->epinfo >> 4,
	   MXC_USBHS->raminfo & 0xf, MXC_USBHS->raminfo >> 4);
    
    printf("Waiting for VBUS...\n");
/*
    while(1)
    {
	    printf("UUUUUU\n");
    }
*/
    /* Initialize state */
    configured = 0;
    suspended = 0;
    event_flags = 0;
    remote_wake_en = 0;

    /* Enable the USB clock and power */
    //SYS_USB_Enable(1);

    /* Start out in full speed */
    usb_opts.enable_hs = 0;
    usb_opts.delay_us = delay_us; /* Function which will be used for delays */
    usb_opts.init_callback = usb_startup_cb;
    usb_opts.shutdown_callback = usb_shutdown_cb;

    /* Initialize the usb module */
    if (usb_init(&usb_opts) != 0) {
        printf("usb_init() failed\n");
        while (1);
    }

    usb_disconnect();

    /* Initialize the enumeration module */
    if (enum_init() != 0) {
        printf("enum_init() failed\n");
        while (1);
    }

    /* Register enumeration data */
    enum_register_descriptor(ENUM_DESC_DEVICE, (uint8_t*)&device_descriptor, 0);
    enum_register_descriptor(ENUM_DESC_CONFIG, (uint8_t*)&config_descriptor, 0);
    if (usb_opts.enable_hs) {
	/* Two additional descriptors needed for high-speed operation */
	enum_register_descriptor(ENUM_DESC_OTHER, (uint8_t*)&other_speed_config_descriptor, 0);
	enum_register_descriptor(ENUM_DESC_QUAL, (uint8_t*)&device_qualifier_descriptor, 0);
    }
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
    usb_event_enable(MAXUSB_EVENT_BRST, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_SUSP, event_callback, NULL);
    acm_register_callback(ACM_CB_SET_LINE_CODING, NULL);
    acm_register_callback(ACM_CB_READ_READY, usb_read_callback);
    usb_read_complete = 0;

    /* Register callback for pb wakeup  */
#if 0
    if (PB_RegisterCallback(0, pb_wakeup) != E_NO_ERROR) {
        printf("PB_RegisterCallback(0) failed\n");
        while (1);
    }
#endif
#if 0
    if (PB_RegisterCallback(1, usb_dump_log) != E_NO_ERROR) {
	printf("PB_RegisterCallback(1) failed\n");
	while (1);
    }
#endif

    /* Start with USB in low power mode */
    usb_app_sleep();
    NVIC_EnableIRQ(USB_IRQn);
#if 0
    NVIC_EnableIRQ(DMA0_IRQn);
    NVIC_EnableIRQ(DMA1_IRQn);
    NVIC_EnableIRQ(DMA2_IRQn);
    NVIC_EnableIRQ(DMA3_IRQn);
#endif
    NVIC_EnableIRQ(UARTn_IRQn);
    
    /* Wait for events */
    while (1) {

//      LED_On(0);
        echo_usb();
//	LED_Off(0);

	
        if (suspended || !configured) {
	  //LED_Off(7);
        } else {
	  //LED_On(7);
        }

        if (event_flags) {
            /* Display events */
            if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_NOVBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_NOVBUS);
                printf("VBUS Disconnect\n");
		usb_disconnect();
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_VBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_VBUS);
                printf("VBUS Connect\n");
		usb_connect();
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_BRST)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_BRST);
                printf("Bus Reset\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_SUSP)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_SUSP);
                //printf("Suspended\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_DPACT)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_DPACT);
                //printf("Resume\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_ENUM_COMP)) {
                MXC_CLRBIT(&event_flags, EVENT_ENUM_COMP);
                printf("Enumeration complete. Waiting for characters...\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_REMOTE_WAKE)) {
                MXC_CLRBIT(&event_flags, EVENT_REMOTE_WAKE);
                //printf("Remote Wakeup\n");
            }
        } else {
	  //LP_EnterLP2();
        }
    }
}

/******************************************************************************/
static void echo_usb(void)
{
  int chars, i;
  uint8_t *ptr;

  if ((chars = acm_canread()) > 0) {

    if (chars > sizeof(uart_tx_data)) {
      chars = sizeof(uart_tx_data);
    }

    // Read the data from USB
    if (acm_read(uart_tx_data, chars) != chars) {
      printf("acm_read() failed\n");
      return;
    }
/*
    printf("chars recv: %d\n",chars);
    for (i = 0;i < chars;i++)
    {
      printf("%c",uart_tx_data[i]);
    }
 */   
    // Echo it back
    ptr = uart_tx_data;
    while (acm_present() && (chars > 0)) {
      i = acm_write(ptr, chars);
      if (i > 0) {
	ptr += i;
	chars -= i;
      } else {
	/* All data lost */
	printf("acm_write() failed!\n");
	break;
      }
    }
//FIXME: Doing bad USB things to test something.
    if (uart_tx_data[0] == 'Z')
    {
      printf("Z detected, Disconnecting USB\n");
      usb_disconnect();
      printf("Disconnected, jump $\n");
      while(1);
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
    suspended = 1;
}

/******************************************************************************/
static void usb_app_wakeup(void)
{
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

#if 0
/******************************************************************************/
void DMA0_IRQHandler(void)
{
    usb_dma_isr(0);
}
/******************************************************************************/
void DMA1_IRQHandler(void)
{
    usb_dma_isr(1);
}
/******************************************************************************/
void DMA2_IRQHandler(void)
{
    usb_dma_isr(2);
}
/******************************************************************************/
void DMA3_IRQHandler(void)
{
    usb_dma_isr(3);
}
#endif

/******************************************************************************/
void UARTn_IRQHandler(void)
{
    UART_Handler(MXC_UARTn);
}
