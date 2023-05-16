/**
 * @file    main.c
 * @brief   SPI Master Demo
 * @details Shows Master loopback demo for QSPI0
 *          Read the printf() for instructions
 */

/*******************************************************************************
 * Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2020-06-29 09:56:17 -0500 (Mon, 29 Jun 2020) $
 * $Revision: 53984 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pb.h"
#include "mxc_config.h"
#include "led.h"
#include "mxc_pins.h"
#include "uart.h"
#include "pb.h"
#include "spi.h"
#include "hexdump.h"

/***** Definitions *****/
#define BLOCK_SIZE      0x0100  // Words
#define TEST_VALUE      0xA5A5  // This is for master mode only...
#define VALUE           0xFFFF
#define SPI_SPEED       10000  // Bit Rate


#define SPI SPI0
#define SPI_IRQ SPI0_IRQn

#define SPI_BYTEWIDTH (8)

// SELECT FROM BELOW
//#define NEW_API
//#define GPIO_SS
//#define MASTERASYNC   // Comment out line for Sync operation
//#define MASTERDMA
#define USE_QSPI

/* Aliases to simulate NEW_API. */
#ifdef NEW_API
#else
#ifdef MASTERDMA
#error "DMA required NEW_API."
#endif /* MASTERDMA */
#define MXC_SPI_MasterTransaction(req) SPI_MasterTrans(SPI, req)
#define MXC_SPI_MasterTransactionAsync(req) SPI_MasterTransAsync(SPI, req)
#define MXC_SPI_Shutdown(spi) SPI_Shutdown(spi)
#endif /* NEW_API */

/***** Globals *****/
uint8_t rx_data[BLOCK_SIZE];
uint8_t tx_data[BLOCK_SIZE];
volatile int spi_flag;
#ifdef MASTERDMA
volatile int dma_flag;
#endif /* MASTERDMA */
volatile int buttonPressed;

/***** RAM Commands *****/
const uint8_t RDMR = '\x05';
const uint8_t WRMR = '\x01';
const uint8_t READ = '\x03';
const uint8_t WRITE = '\x02';
const uint8_t EQIO = '\x38';
const uint8_t EDIO = '\x3B';
const uint8_t RSTQIO = '\x01';

// Mode register flags
#define SPI_RAM_MODE_HOLD      ((uint8_t)(0x01U))
#define SPI_RAM_MODE_HOLD_MASK ((uint8_t)(0x01U))
#define SPI_RAM_MODE_OP_MASK   ((uint8_t)(0xc0U))
#define SPI_RAM_MODE_OP_WORD   ((uint8_t)(0x00U))
#define SPI_RAM_MODE_OP_PAGE   ((uint8_t)(0x80U))
#define SPI_RAM_MODE_OP_BURST  ((uint8_t)(0x40U))

typedef struct st_RDMR_REQ_t
{
    uint8_t cmd;
    uint8_t null;
} st_RDMR_REQ;

typedef struct st_RDMR_RSP_t
{
    uint8_t null;
    uint8_t reg;
} st_RDMR_RSP;

typedef struct st_WRMR_REQ_t
{
    uint8_t cmd;
    uint8_t reg;
} st_WRMR_REQ;

typedef struct st_WRMR_RSP_t
{
    uint8_t null[2];
} st_WRMR_RSP;

typedef struct st_READ_REQ_t
{
    uint8_t cmd;
    uint8_t addr[3];
    uint8_t null[BLOCK_SIZE];
} st_READ_REQ;

typedef struct st_READ_RSP_t
{
    uint8_t null[4];
    uint8_t data[BLOCK_SIZE];
} st_READ_RSP;

typedef struct st_WRITE_REQ_t
{
    uint8_t cmd;
    uint8_t addr[3];
    uint8_t data[BLOCK_SIZE];
} st_WRITE_REQ;

typedef struct st_WRITE_RSP_t
{
    uint8_t null[4 + BLOCK_SIZE];
} st_WRITE_RSP;

typedef struct st_EQIO_REQ_t
{
    uint8_t cmd;
} st_EQIO_REQ;

typedef struct st_EQIO_RSP_t
{
    uint8_t null;
} st_EQIO_RSP;

typedef struct st_EDIO_REQ_t
{
    uint8_t cmd;
} st_EDIO_REQ;

typedef struct st_EDIO_RSP_t
{
    uint8_t null;
} st_EDIO_RSP;

typedef struct st_RSTQIO_REQ_t
{
    uint8_t cmd;
} st_RSTQIO_REQ;

typedef struct st_RSTQIO_RSP_t
{
    uint8_t null;
} st_RSTQIO_RSP;

typedef struct st_SPI_RAM_state_t
{
    spi17y_width_t width;
    spi17y_sspol_t ssPol;
} st_SPI_RAM_state;

typedef union st_SPI_RAM_REQ_t {
    uint8_t cmd;
    st_RDMR_REQ rdmr;
    st_WRMR_REQ wrmr;
    st_READ_REQ read;
    st_WRITE_REQ write;
    st_EQIO_REQ eqio;
    st_EDIO_REQ edio;
    st_RSTQIO_REQ rstqio;
} st_SPI_RAM_REQ;

typedef union st_SPI_RAM_RSP_t {
    st_RDMR_RSP rdmr;
    st_WRMR_RSP wrmr;
    st_READ_RSP read;
    st_WRITE_RSP write;
    st_EQIO_RSP eqio;
    st_EDIO_RSP edio;
    st_RSTQIO_RSP rstqio;
} st_SPI_RAM_RSP;

typedef struct st_SPI_RAM_t
{
    st_SPI_RAM_state state;

    sys_cfg_spi_t spi17y_master_cfg;
    spi_req_t spiPeriphReq;
    gpio_cfg_t stGpioSS0Cfg;

    st_SPI_RAM_REQ req;
    st_SPI_RAM_RSP rsp;
} st_SPI_RAM;

/***** Functions *****/

void spi_cb(void *req, int error)
{
    spi_flag = error;
}

#ifdef MASTERDMA
void DMA0_IRQ_Handler(void)
{
    MXC_DMA_Handler();
}

void DMA1_IRQ_Handler(void)
{
    MXC_DMA_Handler();
    dma_flag = 1;
}
#endif /* MASTERDMA */

void SPI0_IRQHandler(void)
{
    SPI_Handler(SPI);
}

void buttonHandler()
{
    buttonPressed = 1;
}

#ifdef GPIO_SS
void spiRamSetSS(st_SPI_RAM * pSpiRam, int bEnable)
{
    int bHigh;

    // Determine how to set GPIO cs pin.
    if (pSpiRam->state.ssPol == SPI17Y_POL_HIGH)
    {
        bHigh = bEnable;
    }
    else
    {
        bHigh = ((bEnable == 0) ? -1 : 0 );
    }

    // Determine how to set GPIO cs pin.
    if (bHigh == 0)
        GPIO_OutClr(&pSpiRam->stGpioSS0Cfg);
    else
        GPIO_OutSet(&pSpiRam->stGpioSS0Cfg);
}
#else /* GPIO_SS */
void spiRamSetSS(st_SPI_RAM * pSpiRam, int bEnable)
{
}
#endif /* GPIO_SS */

void spiRamInit(st_SPI_RAM * pSpiRam)
{
    // Initialize the SPI state machine.
    memset(pSpiRam, '\0', sizeof(*pSpiRam));
    pSpiRam->state.width = SPI17Y_WIDTH_1;
    pSpiRam->state.ssPol = SPI17Y_POL_LOW;

    // Initialize the structures used to configure SPI peripheral.
#if (CONSOLE_UART == 0)
#warning "Console conflicts with SPI RAM. Reverting to Port1 mapping."
    pSpiRam->spi17y_master_cfg.map = MAP_A;
#else /* (CONSOLE_UART == 0) */
    pSpiRam->spi17y_master_cfg.map = MAP_B;
#endif /* (CONSOLE_UART == 0) */

#ifdef MASTERASYNC
    // Setup the interrupt
    NVIC_EnableIRQ(SPI_IRQ);
#endif

#ifdef GPIO_SS
    pSpiRam->spi17y_master_cfg.ss0 = Disable;
#else /* GPIO_SS */
    pSpiRam->spi17y_master_cfg.ss0 = Enable;
#endif /* GPIO_SS */
    pSpiRam->spi17y_master_cfg.ss1 = Disable;
    pSpiRam->spi17y_master_cfg.ss2 = Disable;
    pSpiRam->spi17y_master_cfg.num_io = 4;

    // Configure the SPI peripheral
    if (SPI_Init(SPI, 0, SPI_SPEED, pSpiRam->spi17y_master_cfg) != 0) {
        Console_Init();
        printf("Error configuring SPI\n");
        while (1) {}
    }

#ifdef GPIO_SS
    // Configure the SS pin using GPIO
#if (CONSOLE_UART == 0)
    pSpiRam->stGpioSS0Cfg.port = PORT_1;
#else /* (CONSOLE_UART == 0) */
    pSpiRam->stGpioSS0Cfg.port = PORT_0;
#endif /* (CONSOLE_UART == 0) */
    pSpiRam->stGpioSS0Cfg.mask = 0x100; // QSPI0_SS0 pin.
    pSpiRam->stGpioSS0Cfg.pad = GPIO_PAD_NONE;
    pSpiRam->stGpioSS0Cfg.func = GPIO_FUNC_OUT;

    // Set SS pin as an output pin.
    GPIO_Config(&pSpiRam->stGpioSS0Cfg);
#endif /* GPIO_SS */

    // Setup SPI peripheral with ss disabled.
    spiRamSetSS(pSpiRam, 0);
}

void spiRamExec(st_SPI_RAM * pSpiRam)
{
#ifdef GPIO_SS
    volatile int delay;
#endif /* GPIO_SS */

    // Setup SPI peripheral with ss enabled.
    spiRamSetSS(pSpiRam, -1);

    // Perform SPI transaction.
#ifdef MASTERDMA
    dma_flag = 0;

    MXC_DMA_ReleaseChannel(0);
    MXC_DMA_ReleaseChannel(1);

    NVIC_EnableIRQ(DMA0_IRQn);
    NVIC_EnableIRQ(DMA1_IRQn);
    MXC_SPI_MasterTransactionDMA(&req);

    while (dma_flag == 0) {}
#else
#ifdef MASTERASYNC
    spi_flag = 1;

    MXC_SPI_MasterTransactionAsync(&pSpiRam->spiPeriphReq);
    while (spi_flag == 1);
#else /* MASTERASYNC */
    MXC_SPI_MasterTransaction(&pSpiRam->spiPeriphReq);
#endif /* MASTERASYNC */
#endif /* MASTERDMA */

    // Setup SPI peripheral with ss disabled.
    spiRamSetSS(pSpiRam, 0);

#ifdef GPIO_SS
    // Extend duration of ss deasserted.
    for (delay = 0; delay < 0x10000; delay ++) {}
#endif /* GPIO_SS */
}

void spiRamRdmrExec(st_SPI_RAM * pSpiRam)
{
    // Initialize TX/RX Data buffer.
    memset(&pSpiRam->req, '\0', sizeof(pSpiRam->req));
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));
    pSpiRam->req.cmd = RDMR;

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txLen = sizeof(pSpiRam->req.rdmr);
    pSpiRam->spiPeriphReq.rxLen = sizeof(pSpiRam->rsp.rdmr);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = sizeof(pSpiRam->req.rdmr);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert = 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txCnt = 0;
    pSpiRam->spiPeriphReq.rxCnt = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.completeCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);
}

void spiRamRdmrPrintRsp(st_SPI_RAM * pSpiRam)
{
    printf("Mode Register Value: 0x%02X\n", pSpiRam->rsp.rdmr.reg);
    printf("Hold Mode: ");
    if ((pSpiRam->rsp.rdmr.reg & SPI_RAM_MODE_HOLD_MASK) == SPI_RAM_MODE_HOLD)
        printf("Enabled");
    else
        printf("Disabled");
    printf("\n");
    printf("Operating Mode: ");
    switch (pSpiRam->rsp.rdmr.reg & SPI_RAM_MODE_OP_MASK)
    {
        case SPI_RAM_MODE_OP_WORD:
            printf("Word");
            break;
        case SPI_RAM_MODE_OP_PAGE:
            printf("Page");
            break;
        case SPI_RAM_MODE_OP_BURST:
            printf("Burst");
            break;
        default:
            printf("Unknown");
    }
    printf("\n");
}

void spiRamWrmrExec(st_SPI_RAM * pSpiRam, uint8_t u8ModeReg)
{
    // Initialize TX/RX Data buffer.
    memset(&pSpiRam->req, '\0', sizeof(pSpiRam->req));
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));
    pSpiRam->req.wrmr.cmd = WRMR;
    pSpiRam->req.wrmr.reg = u8ModeReg;

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txLen = sizeof(pSpiRam->req.rdmr);
    pSpiRam->spiPeriphReq.rxLen = sizeof(pSpiRam->rsp.rdmr);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = sizeof(pSpiRam->req.rdmr);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert = 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txCnt = 0;
    pSpiRam->spiPeriphReq.rxCnt = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.callbackCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);
}

void spiRamWriteExec(
        st_SPI_RAM * pSpiRam,
        uint32_t addr,
        const uint8_t * pData,
        size_t dataLen)
{
    // Bounds check.
    if (dataLen > BLOCK_SIZE)
        dataLen = BLOCK_SIZE;

    // Initialize TX/RX Data buffer.
    pSpiRam->req.write.cmd = WRITE;
    pSpiRam->req.write.addr[0] = ((addr >> 16) & 0xFF);
    pSpiRam->req.write.addr[1] = ((addr >> 8) & 0xFF);
    pSpiRam->req.write.addr[2] = (addr & 0xFF);
    memcpy(pSpiRam->req.write.data, pData, dataLen);
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txLen = (size_t)(&((st_SPI_RAM_REQ *)((size_t)0))->write.data[dataLen]);
    pSpiRam->spiPeriphReq.rxLen = (size_t)(&((st_SPI_RAM_REQ *)((size_t)0))->write.data[dataLen]);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = (size_t)(&((st_SPI_RAM_REQ *)((size_t)0))->write.data[dataLen]);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert = 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.callbackCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);
}

void spiRamReadExec(
        st_SPI_RAM * pSpiRam,
        uint32_t addr,
        uint8_t * pData,
        size_t dataLen)
{
    // Bounds check.
    if (dataLen > BLOCK_SIZE)
        dataLen = BLOCK_SIZE;

    // Initialize TX/RX Data buffer.
    memset(&pSpiRam->req, '\0', sizeof(pSpiRam->req));
    pSpiRam->req.read.cmd = READ;
    pSpiRam->req.read.addr[0] = ((addr >> 16) & 0xFF);
    pSpiRam->req.read.addr[1] = ((addr >> 8) & 0xFF);
    pSpiRam->req.read.addr[2] = (addr & 0xFF);
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txLen = (size_t)(&((st_SPI_RAM_REQ *)((size_t)0))->read.null[dataLen]);
    pSpiRam->spiPeriphReq.rxLen = (size_t)(&((st_SPI_RAM_REQ *)((size_t)0))->read.null[dataLen]);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = (size_t)(&((st_SPI_RAM_REQ *)((size_t)0))->read.null[dataLen]);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert= 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txCnt = 0;
    pSpiRam->spiPeriphReq.rxCnt = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.completeCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);

    // Copy the read content into the provided buffer.
    if (pData != NULL)
    {
        memcpy(pData, pSpiRam->rsp.read.data, dataLen);
    }
}

void spiRamEqioExec(st_SPI_RAM * pSpiRam)
{
    // Initialize TX/RX Data buffer.
    memset(&pSpiRam->req, '\0', sizeof(pSpiRam->req));
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));
    pSpiRam->req.cmd = EQIO;

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txLen = sizeof(pSpiRam->req.eqio);
    pSpiRam->spiPeriphReq.rxLen = sizeof(pSpiRam->rsp.eqio);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = sizeof(pSpiRam->req.eqio);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert = 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txNum = 0;
    pSpiRam->spiPeriphReq.rxNum = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.completeCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);

    // Once this request is serviced, we will be in qspi mode.
    pSpiRam->state.width = SPI17Y_WIDTH_4;
}

void spiRamEdioExec(st_SPI_RAM * pSpiRam)
{
    // Initialize TX/RX Data buffer.
    memset(&pSpiRam->req, '\0', sizeof(pSpiRam->req));
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));
    pSpiRam->req.cmd = EDIO;

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txLen = sizeof(pSpiRam->req.edio);
    pSpiRam->spiPeriphReq.rxLen = sizeof(pSpiRam->rsp.edio);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = sizeof(pSpiRam->req.edio);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert = 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txCnt = 0;
    pSpiRam->spiPeriphReq.rxCnt = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.completeCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);

    // Once this request is serviced, we will be in dual width spi mode.
    pSpiRam->state.width = SPI17Y_WIDTH_2;
}

void spiRamRstqioExec(st_SPI_RAM * pSpiRam)
{
    // Initialize TX/RX Data buffer.
    memset(&pSpiRam->req, '\0', sizeof(pSpiRam->req));
    memset(&pSpiRam->rsp, '\0', sizeof(pSpiRam->rsp));
    pSpiRam->req.cmd = RSTQIO;

    // Construct SPI transfer request.
#ifdef NEW_API
    pSpiRam->spiPeriphReq.spi = SPI;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txData = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rxData = (uint8_t *)&pSpiRam->rsp;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_data = (uint8_t *)&pSpiRam->req;
    pSpiRam->spiPeriphReq.rx_data = (uint8_t *)&pSpiRam->rsp;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.rxLen = sizeof(pSpiRam->req.rstqio);
    pSpiRam->spiPeriphReq.txLen = sizeof(pSpiRam->req.rstqio);
#else /* NEW_API */
    pSpiRam->spiPeriphReq.len = sizeof(pSpiRam->req.rstqio);
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.bits = SPI_BYTEWIDTH;
    pSpiRam->spiPeriphReq.width = pSpiRam->state.width;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssIdx = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.ssel = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.ssDeassert = 1;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.deass = 1;
#endif /* NEW_API */
    pSpiRam->spiPeriphReq.ssel_pol = pSpiRam->state.ssPol;
#ifdef NEW_API
    pSpiRam->spiPeriphReq.txCnt = 0;
    pSpiRam->spiPeriphReq.rxCnt = 0;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.tx_num = 0;
    pSpiRam->spiPeriphReq.rx_num = 0;
#endif /* NEW_API */
#ifdef NEW_API
    pSpiRam->spiPeriphReq.completeCB = (spi_complete_t)spi_cb;
#else /* NEW_API */
    pSpiRam->spiPeriphReq.callback = spi_cb;
#endif /* NEW_API */

    // Issue SPI transfer request.
    spiRamExec(pSpiRam);

    // Once this request is serviced, we will be in normal spi mode.
    pSpiRam->state.width = SPI17Y_WIDTH_1;
}

int main(void)
{
    st_SPI_RAM spiRam;

    size_t byteIndex;
    uint32_t testValue;

    while(1) {
        // Start console.
        Console_Init();
        
        // Print header and instructions.
        printf("\n************** SPI RAM Demo ****************\n");
        printf("This example configures the SPI to communicate with the EvKit's Serial SRAM.\n");
        printf("This example uses the following pins as follows:\n");
        printf("\tP0.8  - QSPI0_SS0\n");
        printf("\tP0.9  - QSPI0_MOSI0/SDIO0\n");
        printf("\tP0.10 - QSPI0_MISO0/SDIO1\n");
        printf("\tP0.11 - QSPI0_SCK\n");
        printf("\tP0.12 - QSPI0_SDIO2\n");
        printf("\tP0.13 - QSPI0_SDIO3\n");
        printf("\tP0.13 - QSPI0_SDIO3\n");
        printf("On RevC of the EvKit, jumper JP7 needs to be set to SPIRAM.\n");
        printf("Push SW2 to continue\n");
        
        // Wait for button press.
        buttonPressed = 0;
        PB_RegisterCallback(0, (pb_callback)buttonHandler);
        while(!buttonPressed);
        
        // Configure the SPI RAM state structure.
        spiRamInit(&spiRam);

        // Read mode register.
        spiRamRdmrExec(&spiRam);

        // Display result.
        spiRamRdmrPrintRsp(&spiRam);

        // Set to burst mode if not already set.
        if ((spiRam.rsp.rdmr.reg & SPI_RAM_MODE_OP_MASK) != SPI_RAM_MODE_OP_BURST)
        {
            // Write mode register.
            spiRamWrmrExec(&spiRam,
                    (spiRam.rsp.rdmr.reg & ~SPI_RAM_MODE_OP_MASK) |
                    SPI_RAM_MODE_OP_BURST);

            // Read mode register.
            spiRamRdmrExec(&spiRam);

            // Display result.
            spiRamRdmrPrintRsp(&spiRam);
        }

#ifdef USE_QSPI
        // Enable QSPI width.
        spiRamEqioExec(&spiRam);
#endif /* USE_QSPI */

        // Display result.
        // Create a page of data to write.
        testValue = 0xA5A5A5A5;
        for (byteIndex = 0;
                byteIndex < (sizeof(spiRam.req.write.data) & ~(size_t)0x3UL);
                testValue++)
        {
            tx_data[byteIndex ++] = (uint8_t)((testValue      ) & 0xFF);
            tx_data[byteIndex ++] = (uint8_t)((testValue >>  8) & 0xFF);
            tx_data[byteIndex ++] = (uint8_t)((testValue >> 16) & 0xFF);
            tx_data[byteIndex ++] = (uint8_t)((testValue >> 24) & 0xFF);
        }

        // Write the data.
        spiRamWriteExec(&spiRam, 0x000000U, &tx_data[0], sizeof(tx_data));

        // Read back the data.
        spiRamReadExec(&spiRam, 0x000000U, &rx_data[0], sizeof(rx_data));

        // Display result.
        if (memcmp(tx_data, rx_data, sizeof(tx_data)) != 0)
        {
            printf("TX and RX data do not match:\n");
            printf("TX Data:\n");
            hexdump((const char *)tx_data, sizeof(tx_data));
            printf("RX Data:\n");
            hexdump((const char *)rx_data, sizeof(rx_data));
        }
        
#ifdef USE_QSPI
        // Reset bit width.
        spiRamRstqioExec(&spiRam);
#endif /* USE_QSPI */

        // Shutdown SPI.
        MXC_SPI_Shutdown(SPI);

        printf("\n Done testing \n");
    }
}

