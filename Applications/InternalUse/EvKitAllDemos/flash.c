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
 * $Date: 2018-08-10 09:36:39 -0500 (Fri, 10 Aug 2018) $
 * $Revision: 36825 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Flash Control Mass Erase & Write 32-bit enabled mode Example
 * @details This example shows how to mass erase the flash using the library
 *          and also how to Write and Verify 4 Words to the flash. 
 */

/***** Includes *****/
#include "mxc_config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "flc.h"
#include "flc_regs.h"
#include "gcr_regs.h"

/***** Definitions *****/
#define TESTSIZE        16
#if EMULATOR
#define MXC_FLASH_MEM_SIZE_TEST  0x00000040
#else 
#define MXC_FLASH_MEM_SIZE_TEST MXC_FLASH_MEM_SIZE
#endif

/***** Globals *****/
uint32_t testaddr;
uint32_t testdata[TESTSIZE];

// To check interrupts
int isr_cnt;
int isr_flags;

/***** Functions *****/

int flash_fill(uint32_t address, uint32_t size, uint32_t data)
{
    // Check if flash controller is busy
    if(MXC_FLC->cn & MXC_F_FLC_CN_PEND)
    {
        return 0;
    }

    // Unlock flash
    MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;
    MXC_FLC->cn |= MXC_S_FLC_CN_UNLOCK_UNLOCKED;

    // write in 32-bit units until we are 128-bit aligned
    MXC_FLC->cn &= 0xF7FFFFFF;
    FLC_MassErase();
    MXC_FLC->addr = address;

    if (size >= 4) {

        while (size >= 4) {
            FLC_Write32(address,data);
            address += 4;
            size -= 4;
        }
    }

    if (size > 0) {
        // write remaining bytes in a 32-bit unit

        uint32_t last_word = 0xffffffff;
        uint32_t mask = 0xff;

        while (size > 0) {
            last_word &= (data | ~mask);
            mask <<= 8;
            size--;
        }
        FLC_Write32(address, last_word);
    }

    /* If error interrupt is disabled, check access violations */
    if ( !(MXC_FLC->intr & MXC_F_FLC_INTR_AFIE) && (MXC_FLC->intr & MXC_F_FLC_INTR_AF) ) {
        MXC_FLC->intr &= ~MXC_F_FLC_INTR_AF;
        return 0;
    }

    return 1;
}
// *****************************************************************************

int flash_verify(uint32_t address, uint32_t length, uint8_t * data)
{
  volatile uint8_t * ptr;


  for (ptr = (uint8_t*)address; ptr < (uint8_t*)(address + length); ptr++, data++) {
    if (*ptr != *data) {
        printf("Verify failed at 0x%x (0x%x != 0x%x)\n", (unsigned int)ptr, (unsigned int)*ptr, (unsigned int)*data);
        return E_UNKNOWN;
    }
  }

  return E_NO_ERROR;
}
//******************************************************************************

int flash_check_mem(uint32_t startaddr, uint32_t length, uint32_t data)
{
    uint32_t * ptr;
    for (ptr = (uint32_t*)startaddr; ptr < (uint32_t*)(startaddr + length);
        ptr++) {

        if (*ptr != data)
        {
            return 0;
        }
    }

    return 1;
}

//******************************************************************************

int flash_check_erased(uint32_t startaddr, uint32_t length)
{
    return flash_check_mem(startaddr, length, 0xFFFFFFFF);
}

//******************************************************************************

int flash_check_not_erased(uint32_t startaddr, uint32_t length)
{
    uint32_t * ptr;
    int erasedvaluefound = 0;

    for (ptr = (uint32_t*)startaddr; ptr < (uint32_t*)(startaddr + length); ptr++)
    {
        if (*ptr == 0xFFFFFFFF)
        {
            if (!erasedvaluefound)
            {
                erasedvaluefound = 1;
            }
            else
            {
                return 0;
            }
        }
    }

    return 1;
}
//******************************************************************************

void flash_init(void)
{
    // Set flash clock divider to generate a 1MHz clock from the APB clock
    // APB clock is 20MHz on the emulator
    // APB clock is 54MHz on the real silicon
    #ifdef EMULATOR
      MXC_FLC->clkdiv = 20;
    #else
      MXC_FLC->clkdiv = 54;
    #endif

    // Setup and enable interrupt
    NVIC_EnableIRQ(FLC_IRQn);
    __enable_irq();

}
//******************************************************************************

void FLC_IRQHandler(void)
{
    uint32_t temp;
    isr_cnt++;
    temp = MXC_FLC->intr;

    if (temp & MXC_F_FLC_INTR_DONE)
    {
        MXC_FLC->intr &= ~MXC_F_FLC_INTR_DONE;
    }

    if (temp & MXC_F_FLC_INTR_AF)
    {
        MXC_FLC->intr &= ~MXC_F_FLC_INTR_AF;
    }

    isr_flags = temp;
}
//******************************************************************************

void flash_interrupt_enabler(mxc_flc_regs_t* regs) {
    regs->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);

}
//******************************************************************************

void flash_burst_mode_en(mxc_flc_regs_t* regs) {
    regs->cn |= MXC_F_FLC_CN_BRST;
}
//******************************************************************************


int flash_main(void)
{
    int error_status,i;

    // This is executing on RAM 
    printf("\n\n***** Flash Control Example *****\n");

    // Initialize the Flash
    flash_init();

    /* Clear and enable flash programming interrupts */
    flash_interrupt_enabler(MXC_FLC);
    isr_flags = 0;
    isr_cnt = 0;
    
    error_status = FLC_MassErase();
    // Mass Erase Flash's Content
    if (error_status == E_NO_ERROR) {
        printf("Flash erased.\n");
    } else if (error_status == E_BAD_STATE) {
        printf("Flash erase operation is not allowed in this state.\n");
    } else {
        printf("Fail to erase flash's content.\n");
    }

    // Checking Interrupt's occurence
    if ((isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
        printf("Interrupt did not occur\n");
    }

    // Check flash's content
    if (flash_check_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_MEM_SIZE_TEST)){
        printf("Flash mass erase is verified.\n");
    } else {
        printf("Flash mass erase failed.\n");
    }

    printf("Writing 32-bit to flash\n");
    printf("Size of testdata : %d\n", sizeof(testdata));
    // Initializing Test Data
    for (i = 0; i < TESTSIZE; i++) {
        testdata[i] = i;
    }
    
    i=0;                                        
    for (testaddr = MXC_FLASH_MEM_BASE; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += 4) {
        /* Clear and enable flash programming interrupts */
        
        isr_flags = 0;
        isr_cnt = 0;

        // Write a word
        if (FLC_Write32(testaddr, testdata[i]) != E_NO_ERROR) { 
            printf("Failure in writing a word.\n");
            break;
        } 
        else {
            printf("Word number %d : %u is written to the flash.\n", i, testdata[i]);
        }

        // Checking Interrupt
        if ((isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
            printf("Interrupt did not occur at 0x%08x\n", (unsigned int)testaddr);
        }

        // Verify that word is written properly
        if (flash_verify(testaddr, 4, (uint8_t*)&testdata[i]) != E_NO_ERROR) {
            printf("Word is not written properly.\n");
            break;
        }
        
        printf("Word number %d written properly and has been verified.\n", i);
        i++;
    }

    printf("End of example.\n");
    return 0;
}
