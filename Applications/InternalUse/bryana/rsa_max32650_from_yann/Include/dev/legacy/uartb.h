/*
 * cobra_uart.h --
 *
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Maxim Integrated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MAXIM INTEGRATED ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* [INTERNAL] ------------------------------------------------------------------
 * Created on: June 8, 2010
 * Author: Gregory Romé <gregory.rome@maximintegrated.com>
 *
 * ---- Subversion keywords (need to set the keyword property)
 * $Revision:: 481      $:  Revision of last commit
 * $Author:: gregory.ro#$:  Author of last commit
 * $Date:: 2010-09-20 1#$:  Date of last commit
 * [/INTERNAL] -------------------------------------------------------------- */
#ifndef _UARTB_H_
#define _UARTB_H_

/* -------------------------------------------------------------------------- */
/* DEFINES */

/** RXTHD [0-3] R/W
 * This field specifies the depth of receive FIFO for interrupt generation
 * (values 0 and > 8 are ignored)
 */
#define UARTB_CR_RXTHD_MASK       0x0000000F
#define UARTB_CR_RXTHD_DEPTH1     0x00000001
#define UARTB_CR_RXTHD_DEPTH2     0x00000002
#define UARTB_CR_RXTHD_DEPTH3     0x00000003
#define UARTB_CR_RXTHD_DEPTH4     0x00000004
#define UARTB_CR_RXTHD_DEPTH5     0x00000005
#define UARTB_CR_RXTHD_DEPTH6     0x00000006
#define UARTB_CR_RXTHD_DEPTH7     0x00000007
#define UARTB_CR_RXTHD_DEPTH8     0x00000008

/** PAREN [4] R/W
 * If set to 1, the parity bit is enabled.
 * If set to 0, the parity bit is disabled.
 */
#define UARTB_CR_PAREN_MASK       0x00000010
#define UARTB_CR_PAREN_ENABLED    0x00000010

/** PAREO [5] R/W
 * If set to 1, the ODD parity is selected,
 * if set to 0, the EVEN parity is selected.
 */
#define UARTB_CR_PAREO_MASK      0x00000020
#define UARTB_CR_PAREO_ODD       0x00000020
#define UARTB_CR_PAREO_EVEN      0x00000000

/** PARMD [6] R/W
 * If set to 1, the parity is based on 0s in the frame.
 * If set to 0, the parity is based on 1s in the frame.
 */
#define UARTB_CR_PARMD_MASK      0x00000040
#define UARTB_CR_PARMD_0S        0x00000040
#define UARTB_CR_PARMD_1S        0x00000000

/** RFU1 [7] R
 * Reserved for future use (read as 0).
 */
#define UARTB_CR_RFU1_MASK       0x00000080

/** TXFLUSH [8] R/W
 * Flush the transmit FIFO. This bit is cleared by hardware.
 */
#define UARTB_CR_TXFLUSH_MASK    0x00000100
#define UARTB_CR_TXFLUSH_ENABLED 0x00000100

/** RXFLUSH [9] R/W
 * Flush the receive FIFO. This bit is cleared by hardware.
 */
#define UARTB_CR_RXFLUSH_MASK    0x00000200
#define UARTB_CR_RXFLUSH_ENABLED 0x00000200

/** SIZE [11:10] R/W
 * Transfer size, see the following:
 * @li 00 5-bit transfer
 * @li 01 6-bit transfer
 * @li 10 7-bit transfer
 * @li 11 8-bit transfer
 */
#define UARTB_CR_SIZE_MASK       0x00000C00

#define UARTB_CR_SIZE_5BIT       0x00000000
#define UARTB_CR_SIZE_6BIT       0x00000400
#define UARTB_CR_SIZE_7BIT       0x00000800
#define UARTB_CR_SIZE_8BIT       0x00000C00

/** STOP [12] R/W
 * If set to 1, 1.5 or 2 stop bits are generated.
 * If set to 0, one stop bit is generated.
 */
#define UARTB_CR_STOP_MASK       0x00000010
#define UARTB_CR_STOP_ENABLED    0x00000010

/** RTSCTSF [13] R/W
 * If set to 1, the hardware flow control is enabled.
 * If set to 0, the hardware flow control is disabled.
 */
#define UARTB_CR_RTSCTSF_MASK    0x00000010
#define UARTB_CR_RTSCTSF_ENABLED 0x00000010

/** RFU2 [31:14] R
 * Reserved for future use (read as 0)
 */
#define UARTB_CR_RFU2_MASK        0x00000010

/* Standard Bauderates
 */

#define UARTB_BR_9600 	{78,16}
#define UARTB_BR_57600 	{13,2}
#define UARTB_BR_115200	{6,65}

/* -------------------------------------------------------------------------- */
/* STRUCTURES */

/** UARTB */
typedef volatile struct {
	unsigned cr;	/** Control register.          (0x00 0x0000 0x0000). */
	unsigned sr;	/** Status register.           (0x04 0x0050 0x0050). */
	unsigned ier;	/** Interrupt enable register. (0x08 0x0000 0x0000). */
	unsigned isr;	/** Interrupt status register. (0x0C 0x0000 0x0000). */
	unsigned brr; 	/** Baudrate register 0.       (0x10 0x0000 0x0000). */
	unsigned rcr;	/** Baudrate register 1.       (0x14 0x0000 0x0000). */
	unsigned txr;	/** TX FIFO Output register.   (0x18 0x0000 0x0000). */
	unsigned pnr;	/** Pin register.              (0x1C 0x0003 0x0003). */
	unsigned dr;	/** Data register.             (0x20 0x0000 0x0000). */
	unsigned rr1;	/** Reserved register 1.       (0x28 0x0000 0x0000). */
	unsigned rr2;	/** Reserved register 2.       (0x2C 0x0000 0x0000). */
} cobra_uart_t;

typedef struct {
	unsigned brr;
	unsigned rcr;
} cobra_uart_brr_t;
/* -------------------------------------------------------------------------- */
/* FUNCTIONS */

/** Init an UART. */
int cobra_uart_init(cobra_uart_t *uart, unsigned config, cobra_uart_brr_t baud);

/* -------------------------------------------------------------------------- */
/* INLINE FUNCTIONS */

char cobra_uart_read_byte(cobra_uart_t *uart);
void cobra_uart_write_byte(cobra_uart_t *uart, char c);

#endif /* _UARTB_H_ */
