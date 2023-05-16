/*******************************************************************************
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
 * $Id: main.c 32120 2017-11-28 23:51:11Z lorne.smith $
 *
 *******************************************************************************
 */

/**
 * @file main.c
 * @brief USB Speed test example
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "mxc_delay.h"
#include "board.h"
#include "led.h"
#include "pb.h"
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "descriptors.h"
#include "vendor_class.h"

/***** Definitions *****/
#define EVENT_ENUM_COMP     MAXUSB_NUM_EVENTS
#define EVENT_REMOTE_WAKE   (EVENT_ENUM_COMP + 1)

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
static void button_callback(void *pb);

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

/******************************************************************************/
int main(void)
{
    maxusb_cfg_options_t usb_opts;
  
    printf("\n\n***** " TOSTRING(TARGET) " USB Speed Test *****\n");
    printf("Waiting for VBUS...\n");

    /* Initialize state */
    configured = 0;
    suspended = 0;
    event_flags = 0;
    remote_wake_en = 0;

    /* Options to USB driver: enable high-speed, delay function, system callbacks, etc. */
    usb_opts.enable_hs = 1;
    usb_opts.delay_us = delay_us;
    usb_opts.init_callback = usb_startup_cb;
    usb_opts.shutdown_callback = usb_shutdown_cb;
    
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
    if (usb_opts.enable_hs) {
	/* Two additional descriptors needed for high-speed operation */
	enum_register_descriptor(ENUM_DESC_OTHER_SPEED, (uint8_t*)&config_descriptor_hs, 0);
	enum_register_descriptor(ENUM_DESC_QUAL, (uint8_t*)&device_qualifier_descriptor, 0);
    }
    enum_register_descriptor(ENUM_DESC_STRING, lang_id_desc, 0);
    enum_register_descriptor(ENUM_DESC_STRING, mfg_id_desc, 1);
    enum_register_descriptor(ENUM_DESC_STRING, prod_id_desc, 2);
    enum_register_descriptor(ENUM_DESC_STRING, serial_id_desc, 3);

    /* Handle configuration */
    enum_register_callback(ENUM_SETCONFIG, setconfig_callback, NULL);

    /* Handle feature set/clear */
    enum_register_callback(ENUM_SETFEATURE, setfeature_callback, NULL);
    enum_register_callback(ENUM_CLRFEATURE, clrfeature_callback, NULL);

    /* Initialize the class driver */
    if (vendor_init() != 0) {
	printf("acm_init() failed\n");
	while (1);
    }
    
    /* Register callbacks */
    usb_event_enable(MAXUSB_EVENT_NOVBUS, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_VBUS, event_callback, NULL);

    /* Register callback for keyboard events */
    if (PB_RegisterCallback(0, button_callback) != E_NO_ERROR) {
        printf("PB_RegisterCallback() failed\n");
        while (1);
    }

    /* Start with USB in low power mode */
    usb_app_sleep();
    NVIC_EnableIRQ(USB_IRQn);

    /* Wait for events */
    while (1) {

	LED_Off(1);
	
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
	    } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_BRSTDN)) {
		MXC_CLRBIT(&event_flags, MAXUSB_EVENT_BRSTDN);
		printf("Bus Reset Done: %s speed\n", (usb_get_status() & MAXUSB_STATUS_HIGH_SPEED) ? "High" : "Full");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_SUSP)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_SUSP);
                printf("Suspended\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_DPACT)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_DPACT);
                printf("Resume\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_ENUM_COMP)) {
                MXC_CLRBIT(&event_flags, EVENT_ENUM_COMP);
                printf("Enumeration complete.\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_REMOTE_WAKE)) {
                MXC_CLRBIT(&event_flags, EVENT_REMOTE_WAKE);
                printf("Remote Wakeup\n");
            }
        }
    }
}

/******************************************************************************/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata)
{
    unsigned int out_ep, out_size, in_ep, in_size;
    
    /* Confirm the configuration value */
    if (sud->wValue == config_descriptor.config_descriptor.bConfigurationValue) {
        configured = 1;
        MXC_SETBIT(&event_flags, EVENT_ENUM_COMP);
	out_ep = config_descriptor.endpoint1_descriptor.bEndpointAddress & 0x7;
	out_size = (usb_get_status() & MAXUSB_STATUS_HIGH_SPEED) ?
	    config_descriptor_hs.endpoint1_descriptor.wMaxPacketSize :
	    config_descriptor.endpoint1_descriptor.wMaxPacketSize;
	in_ep = config_descriptor.endpoint2_descriptor.bEndpointAddress & 0x7f;
	in_size = (usb_get_status() & MAXUSB_STATUS_HIGH_SPEED) ?
	    config_descriptor_hs.endpoint2_descriptor.wMaxPacketSize :
	    config_descriptor.endpoint2_descriptor.wMaxPacketSize;
	return vendor_configure(out_ep, out_size, in_ep, in_size);
    } else if (sud->wValue == 0) {
        configured = 0;
        return vendor_deconfigure();
    }

    return -1;
}

/******************************************************************************/
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    volatile uint32_t *compliance_en = (uint32_t *)(MXC_BASE_USBHS + 0x420);
    volatile uint32_t *compliance_dac_adj = (uint32_t *)(MXC_BASE_USBHS + 0x424);
    volatile uint32_t *compliance_dac_adj_en = (uint32_t *)(MXC_BASE_USBHS + 0x428);

    /* De-bounce registers */
    uint32_t b0, b1;
    
    if (sud->wValue == FEAT_REMOTE_WAKE) {
        remote_wake_en = 1;
    } else if (sud->wValue == FEAT_TEST_MODE) {
	/* No further USB transactions will occur until the device is reset */
	NVIC_DisableIRQ(USB_IRQn);
	*compliance_en = 1;
	printf("compliance_en = 0x%08x\n", compliance_en);
	usb_ackstat(0);
	printf("Entering test mode %u\n", sud->wIndex >> 8);
	usb_test_mode(sud->wIndex >> 8);
	printf("Press SW2 to decrease M31 compliance_dac_adj, or SW3 to increase M31 compliance_dac_adj\n");
	printf("First press will set compliance_dac_adj_en to 1\n");
	b0 = b1 = 0;
	do {
	    b0 = (!!PB_Get(0)) | (b0 << 1);
	    if (b0 == 0xffffffff) {
		*compliance_dac_adj_en = 1;
		if (*compliance_dac_adj > 0) {
		    (*compliance_dac_adj)--;
		}
		printf("compliance_dac_adj = 0x%08x (%s)\n", *compliance_dac_adj, compliance_dac_adj_en ? "Enabled" : "DISABLED!");
		do {
		    b0 = (!!PB_Get(0)) | (b0 << 1);
		} while (b0);
	    }
	    b1 = (!!PB_Get(1)) | (b1 << 1);
	    if (b1 == 0xffffffff) {
		*compliance_dac_adj_en = 1;
		if (*compliance_dac_adj < 15) {
		    (*compliance_dac_adj)++;
		}
		printf("compliance_dac_adj = 0x%08x (%s)\n", *compliance_dac_adj, compliance_dac_adj_en ? "Enabled" : "DISABLED!");
		do {
		    b1 = (!!PB_Get(1)) | (b1 << 1);
		} while (b1);
	    }
	    __NOP();
	} while (1);
    } else {
        // Unknown callback
        return -1;
    }

    return 0;
}

/******************************************************************************/
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    if (sud->wValue == FEAT_REMOTE_WAKE) {
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
            usb_app_sleep();
            break;
        case MAXUSB_EVENT_VBUS:
            usb_event_clear(MAXUSB_EVENT_BRST);
            usb_event_enable(MAXUSB_EVENT_BRST, event_callback, NULL);
	    usb_event_clear(MAXUSB_EVENT_BRSTDN);
	    usb_event_enable(MAXUSB_EVENT_BRSTDN, event_callback, NULL);
            usb_event_clear(MAXUSB_EVENT_SUSP);
            usb_event_enable(MAXUSB_EVENT_SUSP, event_callback, NULL);
            usb_connect();
            usb_app_sleep();
            break;
        case MAXUSB_EVENT_BRST:
            usb_app_wakeup();
            enum_clearconfig();
            configured = 0;
            suspended = 0;
            break;
	case MAXUSB_EVENT_BRSTDN:
	    if (usb_get_status() & MAXUSB_STATUS_HIGH_SPEED) {
		enum_register_descriptor(ENUM_DESC_CONFIG, (uint8_t*)&config_descriptor_hs, 0);
		enum_register_descriptor(ENUM_DESC_OTHER_SPEED, (uint8_t*)&config_descriptor, 0);
	    } else {
		enum_register_descriptor(ENUM_DESC_CONFIG, (uint8_t*)&config_descriptor, 0);
		enum_register_descriptor(ENUM_DESC_OTHER_SPEED, (uint8_t*)&config_descriptor_hs, 0);
	    }
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
void button_callback(void *pb)
{
    int count = 0;
    int button_pressed = 0;
  
    //determine if interrupt triggered by bounce or a true button press
    while (PB_Get(0) && !button_pressed) {
	count++;
    
	if (count > 1000)
	    button_pressed = 1;
    }
}

/******************************************************************************/
void USB_IRQHandler(void)
{
    usb_event_handler();
}
