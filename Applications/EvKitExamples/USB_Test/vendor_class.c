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
 * $Date: 2021-09-02 12:58:48 -0500 (Thu, 02 Sep 2021) $
 * $Revision: 59557 $
 *
 ******************************************************************************/

#include <stdint.h>
#include <string.h>
#include "led.h"
#include "usb.h"
#include "usb_event.h"
#include "enumerate.h"
#include "vendor_class.h"

#define BUF_SIZE 0x200 /* Must be at least as large as the max endpoint size */

/***** File Scope Data *****/
static uint8_t out_ep;
static uint8_t in_ep;
static usb_req_t outreq;
static usb_req_t inreq;
static uint8_t outbuf[BUF_SIZE];
static uint8_t inbuf[BUF_SIZE];

/***** Prototypes *****/
static int class_req(usb_setup_pkt *sud, void *cbdata);
static void out_callback(void *cbdata);
static void in_callback(void *cbdata);

/******************************************************************************/
int vendor_init(void)
{
    out_ep = 0;
    in_ep = 0;

    /* Handle class-specific SETUP requests */
    return enum_register_callback(ENUM_CLASS_REQ, class_req, NULL);
}

/******************************************************************************/
int vendor_configure(unsigned int out_endpoint, unsigned int out_size, unsigned int in_endpoint, unsigned int in_size)
{
    int err;

    if ((out_size > BUF_SIZE) || (in_size > BUF_SIZE)) {
        return -1;
    }

    out_ep = out_endpoint;
    if ((err = usb_config_ep(out_ep, MAXUSB_EP_TYPE_OUT, out_size)) != 0) {
        vendor_deconfigure();
        return err;
    }

    in_ep = in_endpoint;
    if ((err = usb_config_ep(in_ep, MAXUSB_EP_TYPE_IN, in_size)) != 0) {
        vendor_deconfigure();
        return err;
    }

    memset(&outreq, 0, sizeof(usb_req_t));
    outreq.ep = out_ep;
    outreq.data = outbuf;
    outreq.reqlen = BUF_SIZE;
    outreq.callback = out_callback;
    outreq.cbdata = &outreq;
    outreq.type = MAXUSB_TYPE_PKT;

    memset(&inreq, 0, sizeof(usb_req_t));
    inreq.ep = in_ep;
    inreq.data = inbuf;
    inreq.reqlen = BUF_SIZE;
    inreq.callback = in_callback;
    inreq.cbdata = &inreq;
    inreq.type = MAXUSB_TYPE_PKT;

    usb_read_endpoint(&outreq);
    usb_write_endpoint(&inreq);

    return 0;
}

/******************************************************************************/
int vendor_deconfigure(void)
{
    usb_remove_request(&outreq);
    usb_remove_request(&inreq);

    /* deconfigure EPs */
    if (out_ep != 0) {
        usb_reset_ep(out_ep);
        out_ep = 0;
    }

    if (in_ep != 0) {
        usb_reset_ep(in_ep);
        in_ep = 0;
    }

    return 0;
}

/******************************************************************************/
static void out_callback(void *cbdata)
{
    /* Discard data, re-tender the read request */
    usb_read_endpoint(&outreq);
    LED_On(1);
}

/******************************************************************************/
static void in_callback(void *cbdata)
{
    /* Send same data again */
    inreq.actlen = 0;
    usb_write_endpoint(&inreq);
    LED_On(1);
}

/******************************************************************************/
static int class_req(usb_setup_pkt *sud, void *cbdata)
{
    int result = -1;

    switch (sud->bRequest) {
        default:
            /* Unexpected message received, will stall */
            break;
    }

    return result;
}
