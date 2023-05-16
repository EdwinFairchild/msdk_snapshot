/*
 * Copyright 2006, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */
#ifndef _ZA9_UART_H_
#define _ZA9_UART_H_

#include "bsp_types.h"

/*
 * UART register offsets
 */
#define UART_CR         0x00000000  // Control Register
#define UART_SR         0x00000004  // Status Register
#define UART_IER        0x00000008  // Interrupt enable register
#define UART_ISR        0x0000000C  //Interrupt status register
#define UART_BRR        0x00000010  // Baudrate register 0
#define UART_BRR1       0x00000014  // Baudrate register 1
#define UART_TXR        0x00000018  // Tx FIFO Output register
#define UART_PNR        0x0000001C  // Pin register
#define UART_DR         0x00000020  // Data Register
#define UART_RR0        0x00000024  // Reserved Register 0
#define UART_RR1        0x00000028  // Reserved Register 1
#define UART_RR2        0x0000002C  // Reserved Register 2



/*
 * UART Port 0 Registers
 */
#define UART0_CR	(UART0_BASE | UART_CR)
#define UART0_SR	(UART0_BASE | UART_SR)
#define UART0_IER	(UART0_BASE | UART_IER)
#define UART0_ISR	(UART0_BASE | UART_ISR)
#define UART0_BRR	(UART0_BASE | UART_BRR)
#define UART0_BRR1	(UART0_BASE | UART_BRR1)
#define UART0_TXR	(UART0_BASE | UART_TXR) 
#define UART0_PNR (UART0_BASE | UART_PNR)
#define UART0_DR	(UART0_BASE | UART_DR)
#define UART0_RR0	(UART0_BASE | UART_RR0)
#define UART0_RR1	(UART0_BASE | UART_RR1)
#define UART0_RR2	(UART0_BASE | UART_RR2)

/*
 * UART Port 1 Registers
 */
#define UART1_CR	(UART1_BASE | UART_CR)
#define UART1_SR	(UART1_BASE | UART_SR)
#define UART1_IER	(UART1_BASE | UART_IER)
#define UART1_ISR	(UART1_BASE | UART_ISR)
#define UART1_BRR	(UART1_BASE | UART_BRR)
#define UART1_BRR1	(UART1_BASE | UART_BRR1)
#define UART1_TXR	(UART1_BASE | UART_TXR) 
#define UART1_PNR   (UART1_BASE | UART_PNR)
#define UART1_DR	(UART1_BASE | UART_DR)
#define UART1_RR0	(UART1_BASE | UART_RR0)
#define UART1_RR1	(UART1_BASE | UART_RR1)
#define UART1_RR2	(UART1_BASE | UART_RR2)

/*
 * UART Port 2 Registers
 */
#define UART2_CR	(UART2_BASE | UART_CR)
#define UART2_SR	(UART2_BASE | UART_SR)
#define UART2_IER	(UART2_BASE | UART_IER)
#define UART2_ISR	(UART2_BASE | UART_ISR)
#define UART2_BRR	(UART2_BASE | UART_BRR)
#define UART2_BRR1	(UART2_BASE | UART_BRR1)
#define UART2_TXR	(UART2_BASE | UART_TXR) 
#define UART2_PNR   (UART2_BASE | UART_PNR)
#define UART2_DR	(UART2_BASE | UART_DR)
#define UART2_RR0	(UART2_BASE | UART_RR0)
#define UART2_RR1	(UART2_BASE | UART_RR1)
#define UART2_RR2	(UART2_BASE | UART_RR2)

typedef   volatile struct    UART_HW_BLOCK_S {
    UINT32      Uart_ControlReg;
    UINT32      Uart_StatusReg;
    UINT32      Uart_InterruptEnReg;
    UINT32      Uart_InterruptStReg;
    UINT32      Uart_BdRt0Reg;
    UINT32      Uart_BDRt1Reg;
    UINT32      Uart_TxFifoOutReg;
    UINT32      Uart_PnrReg;
    UINT32      Uart_DataReg;
    UINT32      Uart_ResReg0;
    UINT32      Uart_ResReg1;
    UINT32      Uart_ResReg2;
} UART_HW_BLOCK;
#endif 

