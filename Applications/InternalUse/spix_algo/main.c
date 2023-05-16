/*******************************************************************************
 * Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Id: main.c 60490 2021-10-14 14:42:07Z kevin.gillespie $
 *
 *******************************************************************************
 */

/***** Includes *****/
#include <stdio.h>
#include <string.h>
#include "mxc_config.h"
#include "spixf_regs.h"
#include "tmr.h"
#include "spixf.h"
#include "mx25.h"
#include "led.h"
#include "icc_regs.h"
#include "aesk_regs.h"
#include "spixfc.h"

/***** Definitions *****/

#define WORK_AREA_SIZE  16384  // bytes
//#define WORK_AREA_SIZE  0x888  // bytes
//#define WORK_AREA_SIZE  0x900  // bytes

#define STACK_SIZE              256
#define ENC_BUFFER_SIZE         32

/* 3, 4 byte values */
#define BUFFER_SIZE             (WORK_AREA_SIZE - ((3*4)+(ENC_BUFFER_SIZE)+STACK_SIZE))
#define TEST_ADDR               0x10
#define BASE_ADDR               (TEST_ADDR + 0x8000000)
#define TEST_LEN_BYTES          0x7ff0

#define MX25_EXP_ID             0xC22537

typedef struct {
    volatile uint8_t *write_ptr;
    volatile uint8_t *read_ptr;
    volatile uint8_t buffer[BUFFER_SIZE];
    volatile uint32_t spi_write_cmd;
    volatile uint32_t options;
    volatile uint8_t enc_buffers[ENC_BUFFER_SIZE];
    volatile uint8_t stack[STACK_SIZE];    // Build a small stack for the algorithm
} work_area_t;

/***** Globals *****/

//volatile uint32_t test_cnt = 5;
volatile uint32_t test_cnt;
work_area_t work_area;
uint8_t *work_area_start = (uint8_t*)&work_area;
uint8_t *work_area_end = (uint8_t*)((uint32_t)&work_area.stack + STACK_SIZE);

static uint8_t data8;

/***** Functions *****/
extern void algo_write(uint8_t *work_start, uint8_t *work_end, uint32_t len, uint32_t addr);


//*****************************************************************************
void prng8_init(uint8_t data)
{
  data8 = data;
}

//*****************************************************************************
// http://en.wikipedia.org/wiki/Linear_feedback_shift_register
uint8_t prng8(void)
{
  /* taps: 8 6 5 4; feedback polynomial: x^8 + x^6 + x^5 + x^4 + 1 */
  data8 = (data8 >> 1) ^ (-(data8 & 1u) & 0xB8u); 
  return data8;
}

/******************************************************************************/
void spixf_cfg_setup (spixf_req_t *req) {
  
    // cfg
    req->mode  = SPI_MODE_0;
    req->sspol = SSPOL_ACTIVE_LO;
    req->baud  = 10000000;  
    req->ssact = SYS_CLOCKS_2;
    req->ssiact = SYS_CLOCKS_3;
  
    // fetch_ctrl
    req->cmdval = 0x3;  // Flash Chip dependent... 
    req->cmdwth = SINGLE_SDIO_CMD;
    req->adrwth = SINGLE_SDIO_ADDR;
    req->datwth = SINGLE_SDIO_DATA;
    req->addr4  = THREE_BYTE_ADDR_MODE;
    
    // mode_ctrl
    req->mdclk = 0;  // Number of SPI clocks needed during mode/dummy phase of fetch
    req->nocmd = SND_RDCMD_EVRYTIME;
    
    // mode_data
    req->mddata = 0x0000;
    req->mdoe   = 0x0000;
    
    //fb_ctrl
    req->fbmd =  ENABLE_SCLK_FB;
    
    // req->busidle = 0x100;
    req->busidle = 0x0;

}

/******************************************************************************/
void MemManage_Handler(void)
{
    printf("MemManage_Handler\n");
    while(1) {}
}

/******************************************************************************/
void print_mem(uint32_t addr, uint32_t len)
{
    int i;

    volatile uint32_t *ptr32 = (uint32_t*)addr;

    for (i = 0; i < len/4; i++) {
        if ((i % 8) == 0) {
            if (i > 0) { printf("\n"); }
            printf("%08x:", ptr32);
        }
        printf(" %08x", *ptr32++);
    }
    printf("\n");
}

/******************************************************************************/
void TMR0_IRQHandler(void)
{
    volatile uint8_t *next_ptr;

    LED_On(0);

    TMR_IntClear(MXC_TMR0);

    if ((work_area.write_ptr >= &work_area.buffer[BUFFER_SIZE]) || (work_area.write_ptr < work_area.buffer)) {
        printf("Invalid write_ptr %08x\n", work_area.write_ptr);
        printf("Expected range %08x - %08x\n", work_area.buffer, &work_area.buffer[BUFFER_SIZE]);
        TMR_Disable(MXC_TMR0);
        NVIC_DisableIRQ(TMR0_IRQn);
        while(1);
    }

    if ((work_area.read_ptr >= &work_area.buffer[BUFFER_SIZE]) || (work_area.read_ptr < work_area.buffer)) {
        printf("Invalid read_ptr %08x\n", work_area.read_ptr);
        printf("Expected range %08x - %08x\n", work_area.buffer, &work_area.buffer[BUFFER_SIZE]);
        TMR_Disable(MXC_TMR0);
        NVIC_DisableIRQ(TMR0_IRQn);
        while(1);
    }

    while (test_cnt < TEST_LEN_BYTES) {

        next_ptr = work_area.write_ptr + 1;
        if (next_ptr >= &work_area.buffer[BUFFER_SIZE]) {
            next_ptr = work_area.buffer;
        }

        if (next_ptr == work_area.read_ptr) {
            // Buffer full
            LED_Off(0);
            TMR_IntClear(MXC_TMR0);
            return;
        }

#if 0
        if ((test_cnt % 0x200) == 0) {
            printf("writing byte 0x%06x to buffer\n", test_cnt);
        }
#endif

        *work_area.write_ptr = prng8();
        test_cnt++;
        work_area.write_ptr = next_ptr;
    }

    LED_Off(0);

    TMR_Disable(MXC_TMR0);
    NVIC_DisableIRQ(TMR0_IRQn);
}

/******************************************************************************/
int main(void)
{
    volatile unsigned int waitcnt;
    int err;
    uint32_t id, ticks;
    tmr_cfg_t tmr_cfg;
    spixf_req_t req;
    
    for (waitcnt = 0; waitcnt < 100000; waitcnt++);

    printf("\n\n***** Flash Write Algorithm Test *****\n");
    printf("SystemCoreClock = %u\n", SystemCoreClock);
    printf("PeripheralClock = %u\n", PeripheralClock);

    // Disable iCache
    MXC_ICC0->cache_ctrl &= ~MXC_F_ICC_CACHE_CTRL_CACHE_EN;
    MXC_ICC1->cache_ctrl &= ~MXC_F_ICC_CACHE_CTRL_CACHE_EN;

    // Initialize the SPIXFC registers and set the appropriate output pins 
    if (MX25_Init() != E_NO_ERROR) {
        printf("BoardInit Fail\n");
        printf("Example Failed\n");
        while(1);
    }
    printf("MX25 Initialized.\n");

    // Get the ID of the MX25
    if ((id = MX25_ID()) == MX25_EXP_ID) {
       printf("MX25 ID verified.\n");
    } else {
       printf("Error verifying MX25 ID: 0x%06x\n", id);
       printf("Example Failed\n");
       while (1);
    }
    
    // Erase Test Sector
    printf("Erasing first 64k sector\n");
    if ((err = MX25_Erase(0, MX25_Erase_32K)) != E_NO_ERROR) {
        printf("Error erasing flash: %d\n", err);
        printf("Example Failed\n");
        while(1);
    } else {
        printf("Erased first sector\n");
    }

    printf("Erasing second 64k sector\n");
    if ((err = MX25_Erase(0x8000, MX25_Erase_32K)) != E_NO_ERROR) {
        printf("Error erasing flash: %d\n", err);
        printf("Example Failed\n");
        while(1);
    } else {
        printf("Erased second sector\n");
    }


#if 0
    printf("Erasing Entire Flash. This may take many seconds\n");
    MX25_bulk_erase();
#else
    printf("Erasing Block 0\n");
    // MX25_erase(0, MX25_ERASE_32K);
    printf("Erasing Block 1\n");
    // MX25_erase(0x8000, MX25_ERASE_32K);
#endif

    work_area.read_ptr = work_area.buffer;
    work_area.write_ptr = work_area.buffer;
    work_area.options = 0;
    work_area.spi_write_cmd = 0x2;
    printf("work_area w:%08x r:%08x bs:%08x be:%08x enc:%08x end:%08x\n", &work_area.write_ptr, 
        &work_area.read_ptr, work_area.buffer, &work_area.buffer[BUFFER_SIZE], work_area.enc_buffers, work_area_end);

    // work_area.spim_base = MXC_BASE_SPIM0;
    // work_area.spim_tx = (MXC_BASE_SPIM0_FIFO+0x0);
    // work_area.spim_rx = (MXC_BASE_SPIM0_FIFO+0x4);

    test_cnt = 0;
    prng8_init(0x12);
    // tmr_init(MXC_TMR0, 0);
    TMR_Init(MXC_TMR0, TMR_PRES_1, 0);
    TMR_GetTicks(MXC_TMR0, 500, TMR_UNIT_MICROSEC, &ticks);

    tmr_cfg.mode = TMR_MODE_CONTINUOUS;
    tmr_cfg.cmp_cnt = ticks;
    tmr_cfg.pol = 0;
    TMR_Config(MXC_TMR0, &tmr_cfg);
    NVIC_EnableIRQ(TMR0_IRQn);

    printf("\nWriting %u bytes from %06x to %06x\n", TEST_LEN_BYTES, TEST_ADDR, TEST_ADDR + TEST_LEN_BYTES);
#if 1
    // Enable decryption and integrity
    // MXC_SPIX->memseccn |= MXC_F_SPIX_SCK_MEM_SEC_CN_DEC_EN;
    // MXC_SPIX->memseccn &= ~MXC_F_SPIX_SCK_MEM_SEC_CN_DEC_EN;
    // MXC_SPIX->memseccn &= ~MXC_F_SPIX_SCK_MEM_SEC_CN_AUTH_DIS;
    // MXC_SPIX->memseccn |= MXC_F_SPIX_SCK_MEM_SEC_CN_AUTH_DIS;

    // work_area.crypto_base = MXC_BASE_CRYPTO;
    // work_area.integrity = 0x1;
#endif
#if 0
    // Enable decryption, disable integrity
    MXC_SPIX->memseccn |= MXC_F_SPIX_SCK_MEM_SEC_CN_DEC_EN;
    MXC_SPIX->memseccn |= MXC_F_SPIX_SCK_MEM_SEC_CN_AUTH_DIS;

    work_area.crypto_base = MXC_BASE_CRYPTO;
    work_area.integrity = 0x0;
#endif
#if 0
    // No encryption or integrity
    work_area.crypto_base = 0x0;
    work_area.integrity = 0x0;
#endif


#if 0
    // Zero out the AES keys, must be in test mode
    MXC_TM->tm = 1;
    int i;
    for(i = 0; i < 4; i++) {
        MXC_AESK->qspi_enc[i] = 0;
        MXC_AESK->qspi_int[i] = 0;
    }

#endif

    LED_On(1);
    LED_Off(1);
    TMR_Enable(MXC_TMR0);
    algo_write(work_area_start, work_area_end, TEST_LEN_BYTES, TEST_ADDR);        
    printf("algo_write() returned\n");

    /* Shutdown the SPIM, setup the memory mapped SPI */
    SPIXFC_Shutdown(MXC_SPIXFC);
    MXC_SPIXFC->gen_ctrl = 0;

    for (waitcnt = 0; waitcnt < 100000; waitcnt++);

    spixf_cfg_setup(&req);
    SPIXF_setup(MXC_SPIXF, &req);

    for (waitcnt = 0; waitcnt < 100000; waitcnt++);

    /* Flush the caches */
    MXC_GCR->scon |= MXC_F_GCR_SCON_CCACHE_FLUSH;
    while(MXC_GCR->scon & MXC_F_GCR_SCON_CCACHE_FLUSH) {}

    /* Disable SPI cache */
    MXC_ICC1->cache_ctrl |= MXC_F_ICC_CACHE_CTRL_CACHE_EN;
    MXC_ICC1->cache_ctrl &= ~MXC_F_ICC_CACHE_CTRL_CACHE_EN;

    for (waitcnt = 0; waitcnt < 100000; waitcnt++);

    // print_mem(0x8000000, 16);
    // print_mem(0x8000020, 16);
    // print_mem(0x8000000, 16);
    // print_mem(0x8000000, 64);
    // print_mem(0x8000000, 64);
    // print_mem(0x8000060, 64);
    // print_mem(0x8000FE0, 64);
    // print_mem(0x8001FE0, 64);
    // print_mem(0x8002FE0, 64);
    // print_mem(0x8003FE0, 64);
    // print_mem(0x8004FE0, 64);
    // print_mem(0x8005FE0, 64);
    // print_mem(0x8006FE0, 64);
    // print_mem(0x8007FE0, 64);

    // prng8_init(0x12);
    // uint8_t *ptr8, actval, expval;
    // for (ptr8 = (uint8_t*)BASE_ADDR; ptr8 < (uint8_t*)(BASE_ADDR + TEST_LEN_BYTES); ptr8++) {
    //     actval = *ptr8;
    //     expval = prng8();
    //     if (actval != expval) {
    //         printf("FAIL @%08x: %08x != %08x\n", ptr8, actval, expval);
    //         while(1) {}
    //     }
    // }

    printf("Complete\n");

    while (1);

    return 0;
}

/*****************************************************************/
void HardFault_Handler(void)
{
  printf("\nFaultISR: CFSR %08X, BFAR %08x\n", (unsigned int)SCB->CFSR, (unsigned int)SCB->BFAR);

  // Loop forever
  while(1);
}
