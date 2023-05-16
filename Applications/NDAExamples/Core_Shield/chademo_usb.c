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

/***** Definitions *****/
#define EVENT_ENUM_COMP     MAXUSB_NUM_EVENTS

/***** Global Data *****/
volatile int configured;
volatile int suspended;
volatile unsigned int event_flags;

/***** Function Prototypes *****/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata);
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata);
static int event_callback(maxusb_event_t evt, void *data);

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
    /* Unsupported */
    return;
}

int chademo_usb(void)
{
    maxusb_cfg_options_t usb_opts;
    int abandon_usb_loop = 0;

    printf("Waiting for VBUS...\n");

    /* Initialize state */
    configured = 0;
    suspended = 0;
    event_flags = 0;

    /* Start out in full speed */
    usb_opts.enable_hs = 0;
    usb_opts.delay_us = delay_us; /* Function which will be used for delays */
    usb_opts.init_callback = usb_startup_cb;
    usb_opts.shutdown_callback = usb_shutdown_cb;   

    /* Initialize the usb module */
    if (usb_init(&usb_opts) != 0) {
        printf("usb_init() failed\n");
	return -1;
    }
    
    /* Initialize the enumeration module */
    if (enum_init() != 0) {
        printf("enum_init() failed\n");
	return -1;
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
    if (hidkbd_init(&config_descriptor.hid_descriptor, report_descriptor) != 0) {
        printf("hidkbd_init() failed\n");
	return -1;
    }
    
    /* Register callbacks */
    usb_event_enable(MAXUSB_EVENT_NOVBUS, event_callback, NULL);
    usb_event_enable(MAXUSB_EVENT_VBUS, event_callback, NULL);

    NVIC_EnableIRQ(USB_IRQn);

    /* Wait for events */
    while (!abandon_usb_loop) {
        if (event_flags) {
            /* Display events */
            if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_NOVBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_NOVBUS);
		hidkbd_deconfigure();
                printf("VBUS Disconnect\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_VBUS)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_VBUS);
                printf("VBUS Connect\n");
            } else if (MXC_GETBIT(&event_flags, MAXUSB_EVENT_BRST)) {
                MXC_CLRBIT(&event_flags, MAXUSB_EVENT_BRST);
		hidkbd_deconfigure();
                printf("Bus Reset\n");
            } else if (MXC_GETBIT(&event_flags, EVENT_ENUM_COMP)) {
                MXC_CLRBIT(&event_flags, EVENT_ENUM_COMP);
		printf("Enumeration complete, device configured.\n");
		abandon_usb_loop = 1;
            } 
        }
    }

    //NVIC_DisableIRQ(USB_IRQn);
    
    return 0;
}

/******************************************************************************/
static int setconfig_callback(usb_setup_pkt *sud, void *cbdata)
{
    /* Confirm the configuration value */
    if (sud->wValue == config_descriptor.config_descriptor.bConfigurationValue) {
        configured = 1;
        MXC_SETBIT(&event_flags, EVENT_ENUM_COMP);
	return hidkbd_configure(config_descriptor.endpoint_descriptor.bEndpointAddress & USB_EP_NUM_MASK);
    } else if (sud->wValue == 0) {
        configured = 0;
	return hidkbd_deconfigure();
    }

    return -1;
}

/******************************************************************************/
static int setfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    /* Unknown feature */
    return -1;
}

/******************************************************************************/
static int clrfeature_callback(usb_setup_pkt *sud, void *cbdata)
{
    /* Unknown feature */
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

