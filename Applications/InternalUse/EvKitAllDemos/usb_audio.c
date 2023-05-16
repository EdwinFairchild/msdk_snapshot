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
 * $Id: main.c 27069 2017-03-16 15:17:42Z zach.metzinger $
 *
 *******************************************************************************
 */

/**
 * @file    main.c
 * @brief   USB Audio Class Example
 * @details Creates a USB Audio Device with a Microphone and a Speaker, capable of
 *          funcitoning as a virtual loopback device.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "mxc_config.h"
#include "mxc_sys.h"
#include "board.h"
#include "led.h"
#include "uart.h"
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "descriptors.h"
#include "cdc_acm.h"
#include "pb.h"
#include "tmr_utils.h"
#include "audio.h"
//#include "audio_sample.h"

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

/* Define this to disable all printf()s for simulation, etc. */
// #define printf(...) {}


/***** Global Data *****/
volatile int configured;
volatile int write_configured;
volatile int read_configured;
volatile int suspended;
volatile int write_complete = 1;
volatile int read_complete = 1;
volatile int read_dump = 1;
volatile unsigned int event_flags;
volatile uint8_t sample_index = 0;

/***** Function Prototypes *****/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata);
static int setinterface_callback(usb_setup_pkt *sud, void *cbdata);
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int event_callback(maxusb_event_t evt, void *data);
static void audio_written_cb(void *cbdata);
static void audio_read_cb(void *cbdata);
void usb_dump_log(void *);

/***** File Scope Variables *****/
static const audio_cfg_t audio_cfg = {
    10,  /* In EP Number */
    96,  /* In EP Max Packet Size */
    11,  /* Out EP Number */
    96   /* Out EP Max Packet Size */
};
uint8_t tone[128] = {
    0x80, 0x00, 0x8c, 0x8b, 0x98, 0xf8, 0xa5, 0x27, 0xb0, 0xfb, 0xbc, 0x56, 0xc7, 0x1c, 0xd1, 0x33,
    0xda, 0x82, 0xe2, 0xf1, 0xea, 0x6d, 0xf0, 0xe2, 0xf6, 0x41, 0xfa, 0x7c, 0xfd, 0x89, 0xff, 0x61,
    0xff, 0xff, 0xff, 0x61, 0xfd, 0x89, 0xfa, 0x7c, 0xf6, 0x41, 0xf0, 0xe2, 0xea, 0x6d, 0xe2, 0xf1,
    0xda, 0x82, 0xd1, 0x33, 0xc7, 0x1c, 0xbc, 0x56, 0xb0, 0xfb, 0xa5, 0x27, 0x98, 0xf8, 0x8c, 0x8b,
    0x80, 0x00, 0x73, 0x74, 0x67, 0x07, 0x5a, 0xd8, 0x4f, 0x04, 0x43, 0xa9, 0x38, 0xe3, 0x2e, 0xcc,
    0x25, 0x7d, 0x1d, 0x0e, 0x15, 0x92, 0x0f, 0x1d, 0x09, 0xbe, 0x05, 0x83, 0x02, 0x76, 0x00, 0x9e,
    0x00, 0x00, 0x00, 0x9e, 0x02, 0x76, 0x05, 0x83, 0x09, 0xbe, 0x0f, 0x1d, 0x15, 0x92, 0x1d, 0x0e,
    0x25, 0x7d, 0x2e, 0xcc, 0x38, 0xe3, 0x43, 0xa9, 0x4f, 0x04, 0x5a, 0xd8, 0x67, 0x07, 0x73, 0x74
};
static volatile uint8_t readData[96];
static volatile int usb_read_complete;
static volatile int uart_read_complete;

/******************************************************************************/
void us_delay(unsigned us)
{
    TMR_Delay(MXC_TMR0, us);
}

/******************************************************************************/
int usb_main(void)
{
    maxusb_cfg_options_t opts;

    printf("\n\n***** MAX32650 USB AUDIO Example *****\n");

    printf("USB version %u.%u / EPInfo (%u IN, %u OUT) / RAMInfo (%d-bit bus, %d DMA channels)\n",
           MXC_USBHS->hwvers >> 10, MXC_USBHS->hwvers & 0x3ff,
           MXC_USBHS->epinfo & 0xf, MXC_USBHS->epinfo >> 4,
           MXC_USBHS->raminfo & 0xf, MXC_USBHS->raminfo >> 4);

    printf("Waiting for VBUS...\n");

    /* Initialize state */
    configured = 0;
    suspended = 0;
    event_flags = 0;

    /* Enable the USB clock and power */
    //SYS_USB_Enable(1);

    /* Initialize the usb module */
    opts.enable_hs = 0;
    opts.delay_us = us_delay;
    if (usb_init(&opts) != 0) {
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
    if (opts.enable_hs) {
        /* Two additional descriptors needed for high-speed operation */
        enum_register_descriptor(ENUM_DESC_OTHER, (uint8_t*)&other_speed_config_descriptor, 0);
        enum_register_descriptor(ENUM_DESC_QUAL, (uint8_t*)&device_qualifier_descriptor, 0);
    }
    enum_register_descriptor(ENUM_DESC_STRING, lang_id_desc, 0);
    enum_register_descriptor(ENUM_DESC_STRING, mfg_id_desc, 1);
    enum_register_descriptor(ENUM_DESC_STRING, prod_id_desc, 2);
    enum_register_descriptor(ENUM_DESC_STRING, terminal_in_id_desc, 3);
    enum_register_descriptor(ENUM_DESC_STRING, terminal_out_id_desc, 4);

    /* Handle configuration */
    enum_register_callback(ENUM_SETCONFIG, setconfig_callback, NULL);
    enum_register_callback(ENUM_SETINTERFACE, setinterface_callback, NULL);

    /* Handle feature set/clear */
    enum_register_callback(ENUM_SETFEATURE, setfeature_callback, NULL);
    enum_register_callback(ENUM_CLRFEATURE, clrfeature_callback, NULL);

    /* Initialize Class Driver */
    audio_init();

    /* Register callbacks */
    usb_event_enable(MAXUSB_EVENT_NOVBUS, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_VBUS, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_BRST, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_SUSP, event_callback, NULL);

    NVIC_EnableIRQ(USB_IRQn);
    NVIC_EnableIRQ(UARTn_IRQn);

    /* Wait for everything to enumerate/'settle' */
    TMR_Delay(MXC_TMR0, MSEC(5000));

    /* Wait for events */
    while (1) {
        /* While the read interface is configured, read stream continuously */
        if(read_configured && read_complete) {
            // printf("Read the following audio data\n");
            // for(int i = 0; i < 2; i++) {
            //     for(int j = 0; j < 16; j=j+2) {
            //         printf("%02x%02x ", readData[i*16+j], readData[i*16+j+1]);                
            //     }
            //     printf("\n");
            // }
            read_complete = 0;
            audio_read((uint8_t *)&readData, sizeof(readData), audio_read_cb);
        }
        /* While the write interface is configured, send sample continuously */
        if(write_configured && write_complete) {
            write_complete = 0;
            //Use to simulate a busy device that might miss packets 
            //by testing, (i < 10000) skips ~1 packet
            __disable_irq();
            for(int i = 0; i < 10000; i++);
            __enable_irq();
            audio_write((uint8_t *)&readData, sizeof(readData), audio_written_cb);      //Use for Audio Loopback (audio_sample.h must be included)
            // audio_write((uint8_t *)&rawData, sizeof(rawData), audio_written_cb);     //Use to Send Audio Sample via Microphone
        }
        if(!read_configured && !read_dump) {
            printf("Read the following audio data\n");
            for(int i = 0; i < 2; i++) {
                for(int j = 0; j < 16; j=j+2) {
                    printf("%02x%02x ", readData[i*16+j], readData[i*16+j+1]);                
                }
                printf("\n");
            }
            read_dump = 1;
        }
        if (event_flags) {
            /* Display events */
            if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_NOVBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_NOVBUS);
                while(UART_NumWriteAvail(MXC_UART0));
                printf("VBUS Disconnect\n");
                usb_disconnect();
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_VBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_VBUS);
                while(UART_NumWriteAvail(MXC_UART0));
                printf("VBUS Connect\n");
                usb_connect();
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_BRST)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_BRST);
                while(UART_NumWriteAvail(MXC_UART0));
                printf("Bus Reset\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_SUSP)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_SUSP);
                printf("Suspended\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_DPACT)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_DPACT);
                printf("Resume\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_ENUM_COMP)) {
                MXC_CLRBIT(&event_flags, EVENT_ENUM_COMP);
                while(UART_NumWriteAvail(MXC_UART0));
                printf("Enumeration complete.\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_REMOTE_WAKE)) {
                MXC_CLRBIT(&event_flags, EVENT_REMOTE_WAKE);
                printf("Remote Wakeup\n");
            }
        }
    }
}

static void audio_written_cb(void *cbdata)
{
    write_complete = 1;
    // usb_req_t *wreq = (usb_req_t *)cbdata;
    // printf("Wrote %d\n", wreq   ->actlen);
}

static void audio_read_cb(void *cbdata)
{
    read_complete = 1;
    // usb_req_t *rreq = (usb_req_t *)cbdata;
    // printf("Read %d\n", rreq->actlen);
}

/******************************************************************************/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata)
{
    int err;
    /* Confirm the configuration value */
    if (sud->wValue == config_descriptor.config_descriptor.bConfigurationValue) {
        configured = 1;
        MXC_SETBIT(&event_flags, EVENT_ENUM_COMP);
        err = audio_configure(&audio_cfg);
        if(err != 0) {
            printf("Audio Configure Failed! Code: %d\n", err);
            return err;
        }
        printf("Configured with configuration %d\n", sud->wValue);
        return 0;

    } else if (sud->wValue == 0) {
        printf("Received invalid configuration value %d\n", sud->wValue);
        configured = 0;
        audio_deconfigure();
        return 0;
    }

    return -1;
}
static int setinterface_callback(usb_setup_pkt *sud, void *cbdata)
{
    int valid = 0;
    /* Confirm the request is configuring a valid interface/alt */
    if(sud->wIndex == 0 && sud->wValue == 0) {
        valid = 1;
    } else if(sud->wIndex == 1 && (sud->wValue == 0 || sud->wValue == 1)) {
        valid = 1;
        if(sud->wValue == 1) {
            write_configured = 1;
        } else {
            write_configured = 0;
        }
    } else if(sud->wIndex == 2 && (sud->wValue == 0 || sud->wValue == 1)) {
        valid = 1;
        if(sud->wValue == 1) {
            read_configured = 1;
        } else {
            read_configured = 0;
            read_dump = 0;
        }
    } else {
        valid = -1;
    }
    if(valid == 1) {
        printf("Set Interface %d to Alt %d\n",sud->wIndex, sud->wValue);
        return 0;
    }
    printf("Invalid Interface %d, Alt %d\n",sud->wIndex, sud->wValue);
    return -1;
}

/******************************************************************************/
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    printf("Set Feature Called\n");
    usb_stall(0);   //This is required for unimplemented audio class get/set requests
    return -1;
}

/******************************************************************************/
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    printf("Clear Feature Called\n");
    usb_stall(0);   //This is required for unimplemented audio class get/set requests
    return -1;
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
            break;
        case MAXUSB_EVENT_VBUS:
            usb_event_clear(MAXUSB_EVENT_BRST);
            usb_event_enable(MAXUSB_EVENT_BRST, event_callback, NULL);
            usb_event_clear(MAXUSB_EVENT_SUSP);
            usb_event_enable(MAXUSB_EVENT_SUSP, event_callback, NULL);
            usb_connect();
            break;
        case MAXUSB_EVENT_BRST:
            enum_clearconfig();
            configured = 0;
            suspended = 0;
            break;
        case MAXUSB_EVENT_SUSP:
            break;
        case MAXUSB_EVENT_DPACT:
            break;
        default:
            break;
    }

    return 0;
}

/******************************************************************************/
void USB_IRQHandler(void)
{
    usb_event_handler();
}

/******************************************************************************/
void UARTn_IRQHandler(void)
{
    UART_Handler(MXC_UARTn);
}
