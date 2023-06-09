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
 * $Date: 2021-09-10 09:46:58 -0500 (Fri, 10 Sep 2021) $
 * $Revision: 59682 $
 *
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_assert.h"
#include "mxc_sys.h"
#include "usbhs_regs.h"
#include "usb.h"

#define USBHS_M31_CLOCK_RECOVERY

typedef enum {
	      SETUP_IDLE,
	      SETUP_NODATA,
	      SETUP_DATA_OUT,
	      SETUP_DATA_IN
} setup_phase_t;

/* storage for active endpoint data request objects */
static usb_req_t *usb_request[MXC_USBHS_NUM_EP];
/* endpoint sizes */
static unsigned int ep_size[MXC_USBHS_NUM_EP];
/* MUSBHSFC does not track SETUP in hardware, so instantiate state variable */
static setup_phase_t setup_phase = SETUP_IDLE;
/* Driver options passed in during usb_init() */
static maxusb_cfg_options_t driver_opts;

static volatile uint8_t *get_fifo_ptr(unsigned int ep)
{
    volatile uint32_t *ptr;

    ptr = &MXC_USBHS->fifo0;
    ptr += ep; /* Pointer math: multiplies ep by sizeof(uint32_t) */

    return (volatile uint8_t *)ptr;
}

static void load_fifo(volatile uint8_t *fifoptr, const uint8_t *dataptr, unsigned int len)
{
    volatile uint32_t *fifoptr_32 = (uint32_t *)fifoptr;
    uint32_t *dataptr_32 = (uint32_t *)dataptr;
    unsigned int len_32;

    /* Calculate sizes to efficiently move data */
    len_32 = len >> 2;
    len &= 0x3;

    /* Load word-sized chunks */
    while (len_32--) {
	*fifoptr_32 = *dataptr_32++;
    }
    dataptr = (uint8_t *)dataptr_32;
    /* Load remainder as bytes */
    while (len--) {
	*fifoptr = *dataptr++;
    }
}

static void unload_fifo(uint8_t *dataptr, volatile uint8_t *fifoptr, unsigned int len)
{
    volatile uint32_t *fifoptr_32 = (uint32_t *)fifoptr;
    uint32_t *dataptr_32 = (uint32_t *)dataptr;
    unsigned int len_32;

    /* Calculate sizes to efficiently move data */
    len_32 = len >> 2;
    len &= 0x3;

    while (len_32--) {
	*dataptr_32++ = *fifoptr_32;
    }
    dataptr = (uint8_t *)dataptr_32;
    while (len--) {
	*dataptr++ = *fifoptr;
    }
}

int usb_init(maxusb_cfg_options_t *options)
{
    int i;

    /* Save the init options */
    if (options) {
        memcpy(&driver_opts, options, sizeof(maxusb_cfg_options_t));
    } else {
        driver_opts.enable_hs = 0;
    }

    /* Enable peripheral at the chip level */
    if (driver_opts.init_callback) {
        if (driver_opts.init_callback() != E_NO_ERROR) {
            return -1;
        }
    }

    /* Endpoint 0 is CONTROL, size fixed in hardware. Does not need configuration. */
    ep_size[0] = 64;

    /* Reset all other endpoints */
    for (i = 1; i < MXC_USBHS_NUM_EP; i++) {
        usb_reset_ep(i);
    }

    setup_phase = SETUP_IDLE;

    /* Start out disconnected */
    MXC_USBHS->power = 0;

    /* Disable all interrupts */
    MXC_USBHS->intrinen = 0;
    MXC_USBHS->introuten = 0;
    MXC_USBHS->intrusben = 0;

    /* Unsuspend the MAC */
    MXC_USBHS->mxm_suspend = 0;

    /* Configure PHY */
    MXC_USBHS->xcfgi0 = (0x1 << 3) | (0x1 << 11);
    MXC_USBHS->xcfgi1 = 0;
    MXC_USBHS->xcfgi2 = 0x1 << (72-64);
    MXC_USBHS->xcfgi3 = 0;

    // MXC_USBHS->usb_xcfg_ob_rsel = 3;

#ifdef USBHS_M31_CLOCK_RECOVERY
    MXC_USBHS->noncry_rstb = 1;
    MXC_USBHS->noncry_en = 1;
    MXC_USBHS->outclksel = 0;
    MXC_USBHS->coreclkin = 0;
    MXC_USBHS->xtlsel = 2; /* Select 25 MHz clock */
#else
    /* Use this option to feed the PHY a 30 MHz clock, which is them used as a PLL reference */
    /* As it depends on the system core clock, this should probably be done at the SYS level */
    MXC_USBHS->noncry_rstb = 0;
    MXC_USBHS->noncry_en = 0;
    MXC_USBHS->outclksel = 1;
    MXC_USBHS->coreclkin = 1;
    MXC_USBHS->xtlsel = 3; /* Select 30 MHz clock */
#endif
    MXC_USBHS->pll_en = 1;
    MXC_USBHS->oscouten = 1;

    /* Reset PHY */
    MXC_USBHS->ponrst = 0;
    MXC_USBHS->ponrst = 1;

    return 0;
}

int usb_shutdown(void)
{
    /* Disconnect and disable HS, too. */
    MXC_USBHS->power = 0;

    /* Disable all interrupts */
    MXC_USBHS->intrinen = 0;
    MXC_USBHS->introuten = 0;
    MXC_USBHS->intrusben = 0;

    /* Suspend the MAC */
    MXC_USBHS->mxm_suspend = 3;

    /* Shut down PHY */
    MXC_USBHS->ponrst = 0;

    /* Disable peripheral at chip level -- ignore failures */
    if (driver_opts.shutdown_callback) {
        driver_opts.shutdown_callback();
    }

    return 0;
}

int usb_connect(void)
{
    /* Should high-speed negotiation be attempted? */
    if (driver_opts.enable_hs) {
        MXC_USBHS->power |= MXC_F_USBHS_POWER_HS_ENABLE;
    } else {
        MXC_USBHS->power &= ~MXC_F_USBHS_POWER_HS_ENABLE;
    }

    /* Connect to bus, if present */
    MXC_USBHS->power |= MXC_F_USBHS_POWER_SOFTCONN;

    setup_phase = SETUP_IDLE;

    return 0;
}

int usb_disconnect(void)
{
    /* Disconnect from bus */
    MXC_USBHS->power &= ~MXC_F_USBHS_POWER_SOFTCONN;

    setup_phase = SETUP_IDLE;

    return 0;
}

unsigned int usb_get_status(void)
{
    int status = 0;

    /* VBUS */
    if (MXC_USBHS->mxm_reg_a4 & MXC_F_USBHS_MXM_REG_A4_VRST_VDDB_N_A) {
	status |= MAXUSB_STATUS_VBUS_ON;
    }
    
    /* High-speed state */
    if (MXC_USBHS->power & MXC_F_USBHS_POWER_HS_MODE) {
	status |= MAXUSB_STATUS_HIGH_SPEED;
    }

    return status;
}

int usb_config_ep(unsigned int ep, maxusb_ep_type_t type, unsigned int size)
{
    if (!ep || (ep >= MXC_USBHS_NUM_EP) || (size > MXC_USBHS_MAX_PACKET)) {
        /* Can't configure this endpoint, invalid endpoint, or size too big */
        return -1;
    }

    /* Default to disabled */
    usb_reset_ep(ep);

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    /* Select register index for endpoint */
    MXC_USBHS->index = ep;

    switch (type) {
        case MAXUSB_EP_TYPE_DISABLED:
            break;
        case MAXUSB_EP_TYPE_OUT:
            MXC_USBHS->outcsrl = MXC_F_USBHS_OUTCSRL_CLRDATATOG;
            MXC_USBHS->outcsru = MXC_F_USBHS_OUTCSRU_DPKTBUFDIS;
            MXC_USBHS->outmaxp = size;
            ep_size[ep] = size;
            MXC_USBHS->introuten &= ~(1 << ep);
            break;
        case MAXUSB_EP_TYPE_IN:
            MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_CLRDATATOG;
            MXC_USBHS->incsru = MXC_F_USBHS_INCSRU_DPKTBUFDIS | MXC_F_USBHS_INCSRU_MODE;
            MXC_USBHS->inmaxp = size;
            ep_size[ep] = size;
            MXC_USBHS->intrinen |= (1 << ep);
            break;
        default:
            MAXUSB_EXIT_CRITICAL();
            return -1;
    }

    MAXUSB_EXIT_CRITICAL();
    return 0;
}

int usb_is_configured(unsigned int ep)
{
    return !!(ep_size[ep]);
}

int usb_stall(unsigned int ep)
{
    usb_req_t *req;

    if (!usb_is_configured(ep)) {
        /* Can't stall an unconfigured endpoint */
        return -1;
    }

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    MXC_USBHS->index = ep;

    if (ep == 0) {
        MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_SERV_OUTPKTRDY | MXC_F_USBHS_CSR0_SEND_STALL;
        setup_phase = SETUP_IDLE;
    } else {
        if (MXC_USBHS->incsru & MXC_F_USBHS_INCSRU_MODE) {
            /* IN endpoint */
            MXC_USBHS->incsrl |= MXC_F_USBHS_INCSRL_SENDSTALL;
        } else {
            /* Otherwise, must be OUT endpoint */
            MXC_USBHS->outcsrl |= MXC_F_USBHS_OUTCSRL_SENDSTALL;
        }
    }

    /* clear pending requests */
    req = usb_request[ep];
    usb_request[ep] = NULL;

    if (req) {
        /* complete pending requests with error */
        req->error_code = -1;
        if (req->callback) {
            req->callback(req->cbdata);
        }
    }

    MAXUSB_EXIT_CRITICAL();
    return 0;
}

int usb_unstall(unsigned int ep)
{
    if (!usb_is_configured(ep)) {
        /* Can't unstall an unconfigured endpoint */
        return -1;
    }

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    MXC_USBHS->index = ep;

    if (ep != 0) {
        if (MXC_USBHS->incsru & MXC_F_USBHS_INCSRU_MODE) {
            /* IN endpoint */
            if (MXC_USBHS->incsrl & MXC_F_USBHS_INCSRL_INPKTRDY) {
                /* Per musbhsfc_pg, only flush FIFO if IN packet loaded */
                MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_CLRDATATOG | MXC_F_USBHS_INCSRL_FLUSHFIFO;
            } else {
                MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_CLRDATATOG;
            }
        } else {
            /* Otherwise, must be OUT endpoint */
            if (MXC_USBHS->outcsrl & MXC_F_USBHS_OUTCSRL_OUTPKTRDY) {
                /* Per musbhsfc_pg, only flush FIFO if OUT packet is ready */
                MXC_USBHS->outcsrl = MXC_F_USBHS_OUTCSRL_CLRDATATOG | MXC_F_USBHS_OUTCSRL_FLUSHFIFO;
            } else {
                MXC_USBHS->outcsrl = MXC_F_USBHS_OUTCSRL_CLRDATATOG;
            }
        }
    }

    MAXUSB_EXIT_CRITICAL();
    return 0;
}

int usb_is_stalled(unsigned int ep)
{
    unsigned int stalled;

    MXC_USBHS->index = ep;

    if (ep) {
        if (MXC_USBHS->incsru & MXC_F_USBHS_INCSRU_MODE) {
            /* IN endpoint */
            stalled = !!(MXC_USBHS->incsrl & MXC_F_USBHS_INCSRL_SENDSTALL);
        } else {
            /* Otherwise, must be OUT endpoint */
            stalled = !!(MXC_USBHS->outcsrl & MXC_F_USBHS_OUTCSRL_SENDSTALL);
        }
    } else {
        /* Control (EP 0) */
        stalled = !!(MXC_USBHS->csr0 & MXC_F_USBHS_CSR0_SEND_STALL);
    }

    return stalled;
}

int usb_reset_ep(unsigned int ep)
{
    usb_req_t *req;

    if (ep >= MXC_USBHS_NUM_EP) {
        return -1;
    }

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    /* clear pending requests */
    req = usb_request[ep];
    usb_request[ep] = NULL;

    if (ep) {
        ep_size[ep] = 0;

        /* Select register index for endpoint */
        MXC_USBHS->index = ep;

        /* Default to disabled */
        MXC_USBHS->intrinen &= ~(1 << ep);
        MXC_USBHS->introuten &= ~(1 << ep);

        if (MXC_USBHS->incsrl & MXC_F_USBHS_INCSRL_INPKTRDY) {
            /* Per musbhsfc_pg, only flush FIFO if IN packet loaded */
            MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_FLUSHFIFO;
        }
        MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_SENDSTALL;

        MXC_USBHS->incsru = MXC_F_USBHS_INCSRU_DPKTBUFDIS;
        MXC_USBHS->inmaxp = 0;

        if (MXC_USBHS->outcsrl & MXC_F_USBHS_OUTCSRL_OUTPKTRDY) {
            /* Per musbhsfc_pg, only flush FIFO if OUT packet is ready */
            MXC_USBHS->outcsrl = MXC_F_USBHS_OUTCSRL_FLUSHFIFO;
        }
        MXC_USBHS->outcsrl = MXC_F_USBHS_OUTCSRL_SENDSTALL;

        MXC_USBHS->outcsru = MXC_F_USBHS_OUTCSRU_DPKTBUFDIS;
        MXC_USBHS->outmaxp = 0;

        MAXUSB_EXIT_CRITICAL();

        /* We specifically do not complete SETUP callbacks, as this causes undesired SETUP status-stage STALLs */
        if (req) {
            /* complete pending requests with error */
            req->error_code = -1;
            if (req->callback) {
                req->callback(req->cbdata);
            }
        }
    } else {
        MAXUSB_EXIT_CRITICAL();
    }

    return 0;
}

int usb_ackstat(unsigned int ep)
{
    uint32_t saved_index;

    if (ep) {
        /* Only valid for endpoint 0 */
        return -1;
    }

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    saved_index = MXC_USBHS->index;
    MXC_USBHS->index = 0;

    /* On this hardware, only setup transactions with no data stage need to be explicitly ACKed */
    if (setup_phase == SETUP_NODATA) {
        MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_SERV_OUTPKTRDY | MXC_F_USBHS_CSR0_DATA_END;
    }

    setup_phase = SETUP_IDLE;

    MXC_USBHS->index = saved_index;

    MAXUSB_EXIT_CRITICAL();

    return 0;
}

void usb_dma_isr(void)
{
    /* Not implemented */
}

/* sent packet done handler*/
static void event_in_data(uint32_t irqs)
{
    uint32_t ep, buffer_bit, data_left;
    usb_req_t *req;
    unsigned int len;

    /* Loop for each data endpoint */
    for (ep = 0; ep < MXC_USBHS_NUM_EP; ep++) {

        buffer_bit = (1 << ep);
        if ((irqs & buffer_bit) == 0) { /* Not set, next Endpoint */
            continue;
        }

        /* If an interrupt was received, but no request on this EP, ignore. */
        if (!usb_request[ep]) {
            continue;
        }

        /* This function is called within interrupt context, so no need for a critical section */

        MXC_USBHS->index = ep;

        req = usb_request[ep];
        data_left = req->reqlen - req->actlen;

        /* Check for more data left to transmit */
        if (data_left) {

            if (data_left >= ep_size[ep]) {
                len = ep_size[ep];
            } else {
                len = data_left;
            }

            /* Fill FIFO with data */
            load_fifo(get_fifo_ptr(ep), (req->data + req->actlen), len);
            req->actlen += len;

            if (!ep) {
                if (usb_request[ep]->actlen == usb_request[ep]->reqlen) {
                    /* Implicit status-stage ACK, move state machine back to IDLE */
                    setup_phase = SETUP_IDLE;
                    MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_INPKTRDY | MXC_F_USBHS_CSR0_DATA_END;

                    /* free request */
                    usb_request[ep] = NULL;

                    /* set done return value */
                    if (req->callback) {
			req->callback(req->cbdata);
                    }
                } else {
                    MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_INPKTRDY;
                }
            } else {
                /* Arm for transmit to host */
                MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_INPKTRDY;
            }

        } else {
            /* all done sending data */

            /* free request */
            usb_request[ep] = NULL;

            /* set done return value */
            if (req->callback) {
                req->callback(req->cbdata);
            }
        }
    }
}

/* received packet */
static void event_out_data(uint32_t irqs)
{
    uint32_t ep, buffer_bit, reqsize;
    usb_req_t *req;

    /* Loop for each data endpoint */
    for (ep = 0; ep < MXC_USBHS_NUM_EP; ep++) {
	
        buffer_bit = (1 << ep);
        if ((irqs & buffer_bit) == 0) {
            continue;
        }

        /* If an interrupt was received, but no request on this EP, ignore. */
        if (!usb_request[ep]) {
            continue;
        }

        req = usb_request[ep];

        /* This function is called within interrupt context, so no need for a critical section */

        /* Select this endpoint for banked registers */
        MXC_USBHS->index = ep;

        if (!ep) {
            if (!(MXC_USBHS->csr0 & MXC_F_USBHS_CSR0_OUTPKTRDY)) {
                continue;
            }
            if (MXC_USBHS->count0 == 0) {
                /* ZLP */
                usb_request[ep] = NULL;
                /* Let the callback do the status stage */
                /* call it done */
                if (req->callback) {
                    req->callback(req->cbdata);
                }
                continue;
            } else {
                /* Write as much as we can to the request buffer */
                reqsize = MXC_USBHS->count0;
                if (reqsize > (req->reqlen - req->actlen)) {
                    reqsize = (req->reqlen - req->actlen);
                }
            }
        } else {
            if (!(MXC_USBHS->outcsrl & MXC_F_USBHS_OUTCSRL_OUTPKTRDY)) {
                /* No packet on this endpoint? */
                continue;
            }
            if (MXC_USBHS->outcount == 0) {
                /* ZLP */
                /* Clear out request */
                usb_request[ep] = NULL;

                /* Signal to H/W that FIFO has been read */
                MXC_USBHS->outcsrl &= ~MXC_F_USBHS_OUTCSRL_OUTPKTRDY;

                /* Disable interrupt for this endpoint */
                MXC_USBHS->introuten &= ~(1 << ep);

                /* Complete request */
                if (req->callback) {
                    req->callback(req->cbdata);
                }
                continue;

            } else {
                /* Write as much as we can to the request buffer */
                reqsize = MXC_USBHS->outcount;
                if (reqsize > (req->reqlen - req->actlen)) {
                    reqsize = (req->reqlen - req->actlen);
                }
            }
        }

        unload_fifo(&req->data[req->actlen], get_fifo_ptr(ep), reqsize);

        req->actlen += reqsize;

        if (!ep) {
            if (req->actlen == req->reqlen) {
                /* No more data */
                MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_SERV_OUTPKTRDY | MXC_F_USBHS_CSR0_DATA_END;
                /* Done */
                usb_request[ep] = NULL;

                if (req->callback) {
                    req->callback(req->cbdata);
                }
            } else {
                /* More data */
                MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_SERV_OUTPKTRDY;
            }
        } else {
            /* Signal to H/W that FIFO has been read */
            MXC_USBHS->outcsrl &= ~MXC_F_USBHS_OUTCSRL_OUTPKTRDY;

            if ((req->type == MAXUSB_TYPE_PKT) || (req->actlen == req->reqlen)) {
                /* Done */
                usb_request[ep] = NULL;

                /* Disable interrupt for this endpoint */
                MXC_USBHS->introuten &= ~(1 << ep);

                /* Complete request */
                if (req->callback) {
                    req->callback(req->cbdata);
                }

            }
        }
    }
}

void usb_irq_handler(maxusb_usbio_events_t *evt)
{
    uint32_t saved_index;
    uint32_t in_flags, out_flags, usb_flags, usb_mxm_flags;
    int i, aborted = 0;
    
#ifdef __ICCARM__
    uint8_t  temp_intrusben = MXC_USBHS->intrusben;
    uint16_t temp_intrinen  = MXC_USBHS->intrinen;
    uint16_t temp_introuten = MXC_USBHS->introuten;
    uint32_t temp_mxm_int_en = MXC_USBHS->mxm_int_en;
#endif    
    /* Save current index register */
    saved_index = MXC_USBHS->index;

#ifdef __ICCARM__    // This is done to handle IAR warning Pa082
    /* Note: Hardware clears these after read, so we must process them all or they are lost */
    usb_flags = MXC_USBHS->intrusb & temp_intrusben;
    in_flags = MXC_USBHS->intrin & temp_intrinen;
    out_flags = MXC_USBHS->introut & temp_introuten;

    /* These USB interrupt flags are W1C. */
    usb_mxm_flags = MXC_USBHS->mxm_int & temp_mxm_int_en;
    MXC_USBHS->mxm_int = usb_mxm_flags;    
#else    
    /* Note: Hardware clears these after read, so we must process them all or they are lost */
    usb_flags = MXC_USBHS->intrusb & MXC_USBHS->intrusben;
    in_flags = MXC_USBHS->intrin & MXC_USBHS->intrinen;
    out_flags = MXC_USBHS->introut & MXC_USBHS->introuten;

    /* These USB interrupt flags are W1C. */
    usb_mxm_flags = MXC_USBHS->mxm_int & MXC_USBHS->mxm_int_en;
    MXC_USBHS->mxm_int = usb_mxm_flags;
#endif

    /* Map hardware-specific signals to generic stack events */
    evt->dpact  = !!(usb_flags & MXC_F_USBHS_INTRUSB_SOF_INT);
    evt->rwudn  = 0; /* Not supported by this hardware */
    evt->bact   = !!(usb_flags & MXC_F_USBHS_INTRUSB_SOF_INT);
    evt->brst   = !!(usb_flags & MXC_F_USBHS_INTRUSB_RESET_INT);
    evt->susp   = !!(usb_flags & MXC_F_USBHS_INTRUSB_SUSPEND_INT);
    evt->novbus = !!(usb_mxm_flags & MXC_F_USBHS_MXM_INT_NOVBUS);
    evt->vbus   = !!(usb_mxm_flags & MXC_F_USBHS_MXM_INT_VBUS);
    evt->brstdn = !!(usb_flags & MXC_F_USBHS_INTRUSB_RESET_INT); /* Hardware does not signal this, so simulate it */
    evt->sudav = 0; /* Overwritten, if necessary, below */

    /* Handle control state machine */
    if (in_flags & MXC_F_USBHS_INTRIN_EP0_IN_INT) {
        /* Select endpoint 0 */
        MXC_USBHS->index = 0;
        /* Process all error conditions */
        if (MXC_USBHS->csr0 & MXC_F_USBHS_CSR0_SENT_STALL) {
	    /* Clear stall indication, go back to IDLE */
	    MXC_USBHS->csr0 &= ~(MXC_F_USBHS_CSR0_SENT_STALL);
	    /* Remove this from the IN flags so that it is not erroneously processed as data */
	    in_flags &= ~MXC_F_USBHS_INTRIN_EP0_IN_INT;
	    setup_phase = SETUP_IDLE;
	    aborted = 1;
        }
        if (MXC_USBHS->csr0 & MXC_F_USBHS_CSR0_SETUP_END) {
	    /* Abort pending requests, clear early end-of-control-transaction bit, go back to IDLE */
	    MXC_USBHS->csr0 |= (MXC_F_USBHS_CSR0_SERV_SETUP_END);
	    setup_phase = SETUP_IDLE;

	    /* Remove this from the IN flags so that it is not erroneously processed as data */
	    in_flags &= ~MXC_F_USBHS_INTRIN_EP0_IN_INT;
	    usb_reset_ep(0);
	    aborted = 1;
        }
        /* Now, check for a SETUP packet */
        if (!aborted) {
            if ((setup_phase == SETUP_IDLE) && (MXC_USBHS->csr0 & MXC_F_USBHS_CSR0_OUTPKTRDY)) {
                /* Flag that we got a SETUP packet */
                evt->sudav = 1;
                /* Remove this from the IN flags so that it is not erroneously processed as data */
                in_flags &= ~MXC_F_USBHS_INTRIN_EP0_IN_INT;
	    } else {
                /* Otherwise, we are in endpoint 0 data IN/OUT */
                /* Fix interrupt flags so that OUTs are processed properly */
                if (setup_phase == SETUP_DATA_OUT) {
                    in_flags &= ~MXC_F_USBHS_INTRIN_EP0_IN_INT;
                    out_flags |= MXC_F_USBHS_INTRIN_EP0_IN_INT;
                }
                /* SETUP_NODATA is silently ignored by event_in_data() right now.. could fix this later */
            }
        }
    }
    /* do cleanup in cases of bus reset */
    if (evt->brst) {
        setup_phase = SETUP_IDLE;
        /* kill any pending requests */
        for (i = 0; i < MXC_USBHS_NUM_EP; i++) {
	    usb_reset_ep(i);
        }
        /* no need to process events after reset */
        return;
    }

    if (in_flags) {
        event_in_data(in_flags);
    }

    if (out_flags) {
        event_out_data(out_flags);
    }

    /* Restore register index before exiting ISR */
    MXC_USBHS->index = saved_index;

}

int usb_irq_enable(maxusb_event_t event)
{
    if (event >= MAXUSB_NUM_EVENTS) {
        return -1;
    }

    switch (event) {
        case MAXUSB_EVENT_BACT:
            /* Bus Active */
            MXC_USBHS->intrusben |= MXC_F_USBHS_INTRUSBEN_SOF_INT_EN;
            break;

        case MAXUSB_EVENT_BRST:
            /* Bus Reset */
            MXC_USBHS->intrusben |= MXC_F_USBHS_INTRUSBEN_RESET_INT_EN;
            break;

        case MAXUSB_EVENT_SUSP:
            /* Suspend */
            MXC_USBHS->intrusben |= MXC_F_USBHS_INTRUSBEN_SUSPEND_INT_EN;
            break;

        case MAXUSB_EVENT_SUDAV:
            /* Setup Data Available */
            MXC_USBHS->intrinen |= MXC_F_USBHS_INTRINEN_EP0_INT_EN;
            break;

        case MAXUSB_EVENT_VBUS:
            /* VBUS Detect */
            MXC_USBHS->mxm_int_en |= MXC_F_USBHS_MXM_INT_EN_VBUS;
            break;

        case MAXUSB_EVENT_NOVBUS:
            /* NOVBUS Detect */
            MXC_USBHS->mxm_int_en |= MXC_F_USBHS_MXM_INT_EN_NOVBUS;
            break;

        default:
            /* Other events not supported by this hardware */
            break;
    }

    return 0;
}

int usb_irq_disable(maxusb_event_t event)
{
    if (event >= MAXUSB_NUM_EVENTS) {
        return -1;
    }

    switch (event) {
        case MAXUSB_EVENT_BACT:
            /* Bus Active */
            MXC_USBHS->intrusben &= ~MXC_F_USBHS_INTRUSBEN_SOF_INT_EN;
            break;

        case MAXUSB_EVENT_BRST:
            /* Bus Reset */
            MXC_USBHS->intrusben &= ~MXC_F_USBHS_INTRUSBEN_RESET_INT_EN;
            break;

        case MAXUSB_EVENT_SUSP:
            /* Suspend */
            MXC_USBHS->intrusben &= ~MXC_F_USBHS_INTRUSBEN_SUSPEND_INT_EN;
            break;

        case MAXUSB_EVENT_SUDAV:
            /* Setup Data Available */
            MXC_USBHS->intrinen &= ~MXC_F_USBHS_INTRINEN_EP0_INT_EN;
            break;

        case MAXUSB_EVENT_VBUS:
            /* VBUS Detect */
            MXC_USBHS->mxm_int_en &= ~MXC_F_USBHS_MXM_INT_EN_VBUS;
            break;

        case MAXUSB_EVENT_NOVBUS:
            /* NOVBUS Detect */
            MXC_USBHS->mxm_int_en &= ~MXC_F_USBHS_MXM_INT_EN_NOVBUS;
            break;

        default:
            /* Other events not supported by this hardware */
            break;
    }

    return 0;
}

int usb_irq_clear(maxusb_event_t event)
{
    /* No-op on this hardware, as reading the interrupt flag register clears it */

    return 0;
}

int usb_get_setup(usb_setup_pkt *sud)
{
    volatile uint8_t *fifoptr = (uint8_t *)&MXC_USBHS->fifo0;

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    /* Select endpoint 0 */
    MXC_USBHS->index = 0;

    if ((sud == NULL) || !(MXC_USBHS->csr0 & MXC_F_USBHS_CSR0_OUTPKTRDY)) {
	MAXUSB_EXIT_CRITICAL();
        return -1;
    }

    /* Pull SETUP packet out of FIFO */
    sud->bmRequestType = *fifoptr;
    sud->bRequest = *fifoptr;
    sud->wValue = *fifoptr;
    sud->wValue += (*fifoptr) << 8;
    sud->wIndex = *fifoptr;
    sud->wIndex += (*fifoptr) << 8;
    sud->wLength = *fifoptr;
    sud->wLength += (*fifoptr) << 8;

    /* Check for follow-on data and advance state machine */
    if (sud->wLength > 0) {
        MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_SERV_OUTPKTRDY;
        /* Determine if IN or OUT data follows */
        if (sud->bmRequestType & RT_DEV_TO_HOST) {
            setup_phase = SETUP_DATA_IN;
        } else {
            setup_phase = SETUP_DATA_OUT;
        }
    } else {
        /* SERV_OUTPKTRDY is set using usb_ackstat() */
        setup_phase = SETUP_NODATA;
    }

    MAXUSB_EXIT_CRITICAL();
    return 0;
}

int usb_set_func_addr(unsigned int addr)
{
    if (addr > 0x7f) {
        return -1;
    }

    MXC_USBHS->faddr = addr;

    return 0;
}

usb_req_t *usb_get_request(unsigned int ep)
{
    return usb_request[ep];
}

int usb_remove_request(usb_req_t *req)
{
    if (req->ep >= MXC_USBHS_NUM_EP) {
        return -1;
    }

    if (usb_request[req->ep] != req) {
        return -1;
    }

    /* Delete request */
    usb_request[req->ep] = NULL;

    /* complete pending request with error */
    req->error_code = -1;
    if (req->callback) {
        req->callback(req->cbdata);
    }

    return 0;
}

int usb_write_endpoint(usb_req_t *req)
{
    unsigned int ep  = req->ep;
    unsigned int len = req->reqlen;
    unsigned int armed;

    if (ep >= MXC_USBHS_NUM_EP) {
        return -1;
    }

    /* EP must be enabled (configured) */
    if (!usb_is_configured(ep)) {
        return -1;
    }

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    MXC_USBHS->index = ep;

    /* if pending request; error */
    if (usb_request[ep] || (MXC_USBHS->incsrl & MXC_F_USBHS_INCSRL_INPKTRDY)) {
        MAXUSB_EXIT_CRITICAL();
        return -1;
    }

    /* assign req object */
    usb_request[ep] = req;

    /* clear errors */
    req->error_code = 0;

    /* Placeholder for DMA code */
    armed = 0;

    if (!armed) {
        /* EP0 or no free DMA channel found, fall back to PIO */

        /* Determine how many bytes to be sent */
        if (len > ep_size[ep]) {
            len = ep_size[ep];
        }
        usb_request[ep]->actlen = len;

        load_fifo(get_fifo_ptr(ep), req->data, len);

        if (!ep) {
            if (usb_request[ep]->actlen == usb_request[ep]->reqlen) {
                /* Implicit status-stage ACK, move state machine back to IDLE */
                setup_phase = SETUP_IDLE;
                MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_INPKTRDY | MXC_F_USBHS_CSR0_DATA_END;
            } else {
                MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_INPKTRDY;
            }
        } else {
            /* Arm for transmit to host */
            MXC_USBHS->incsrl = MXC_F_USBHS_INCSRL_INPKTRDY;
        }
    }

    MAXUSB_EXIT_CRITICAL();
    return 0;
}

int usb_read_endpoint(usb_req_t *req)
{
    unsigned int ep  = req->ep;
    uint32_t reqsize;
    unsigned int armed;

    if (ep >= MXC_USBHS_NUM_EP) {
        return -1;
    }

    /* Interrupts must be disabled while banked registers are accessed */
    MAXUSB_ENTER_CRITICAL();

    /* EP must be enabled (configured) and not stalled */
    if (!usb_is_configured(ep)) {
        MAXUSB_EXIT_CRITICAL();        
        return -1;
    }

    if (usb_is_stalled(ep)) {
        MAXUSB_EXIT_CRITICAL();
        return -1;
    }

    /* if pending request; error */
    if (usb_request[ep]) {
        MAXUSB_EXIT_CRITICAL();
        return -1;
    }

    /* clear errors */
    req->error_code = 0;

    /* reset length */
    req->actlen = 0;

    /* assign the req object */
    usb_request[ep] = req;

    /* Select endpoint */
    MXC_USBHS->index = ep;

    /* Since the OUT interrupt for EP 0 doesn't really exist, only do this for others */
    if (ep) {
	/* Placeholder for DMA code */
	armed = 0;
	
	if (!armed) {
	    /* No free DMA channel found, fall back to PIO */
	    
	    /* See if data already in FIFO for this EP */
	    if (MXC_USBHS->outcsrl & MXC_F_USBHS_OUTCSRL_OUTPKTRDY) {
		reqsize = MXC_USBHS->outcount;
		if (reqsize > (req->reqlen - req->actlen)) {
		    reqsize = (req->reqlen - req->actlen);
		}
		
		unload_fifo(&req->data[req->actlen], get_fifo_ptr(ep), reqsize);
		
		req->actlen += reqsize;
		
		/* Signal to H/W that FIFO has been read */
		MXC_USBHS->outcsrl &= ~MXC_F_USBHS_OUTCSRL_OUTPKTRDY;
		
		if ((req->type == MAXUSB_TYPE_PKT) || (req->actlen == req->reqlen)) {
		    /* Done with request, callback fires if configured */
		    usb_request[ep] = NULL;
		    
		    if (req->callback) {
			req->callback(req->cbdata);
		    }
		    MAXUSB_EXIT_CRITICAL();            
		    return 0;
		} else {
		    /* Not done, more data requested */
		    MXC_USBHS->introuten |= (1 << ep);
		}
	    } else {
		/* No data, will need an interrupt to service later */
		MXC_USBHS->introuten |= (1 << ep);
	    }
	}
    }

    MAXUSB_EXIT_CRITICAL();
    return 0;
}

void usb_remote_wakeup(void)
{
    if (driver_opts.delay_us) {
        MXC_USBHS->power |= MXC_F_USBHS_POWER_RESUME;
        driver_opts.delay_us(10000);
        MXC_USBHS->power &= ~MXC_F_USBHS_POWER_RESUME;
    }
}

int usb_test_mode(unsigned int value)
{
    const uint8_t test_packet[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
				   0xAA, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE,
				   0xEE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
				   0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xDF,
				   0xEF, 0xF7, 0xFB, 0xFD, 0xFC, 0x7E, 0xBF, 0xDF,
				   0xEF, 0xF7, 0xFB, 0xFD, 0x7E};
	
    switch (value) {
	case 0x01:
	    /* Test_J */
	    MXC_USBHS->testmode = MXC_F_USBHS_TESTMODE_TEST_J;
	    break;
	case 0x02:
	    /* Test_K */
	    MXC_USBHS->testmode = MXC_F_USBHS_TESTMODE_TEST_K;
	    break;
	case 0x03:
	    /* Test_SE0_NAK */
	    MXC_USBHS->testmode = MXC_F_USBHS_TESTMODE_TEST_SE0_NAK;
	    break;
	case 0x04:
	    /* Test_Packet */
	    /* Load EP 0 with data provided by section 11.4 of musbhsfc_pg.pdf */
	    /* sizeof() considered safe, since we use uint8_t explicitly */
	    load_fifo(get_fifo_ptr(0), test_packet, sizeof(test_packet));
	    MXC_USBHS->csr0 |= MXC_F_USBHS_CSR0_INPKTRDY;
	    MXC_USBHS->testmode = MXC_F_USBHS_TESTMODE_TEST_PKT;
	    break;
	default:
	    /* Unsupported */
	    return -1;
    }

    return 0;
}
