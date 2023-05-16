/****************************************************************************
 * Copyright (C) 2016 Maxim Integrated, All Rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 *     Description: ES08 Flash Controller test
 *             $Id$
 *
 ********************************************************************************
 */

/***** Includes *****/
#include "mxc_config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "flc.h"
#include "icc.h"
#include "flc_regs.h"
#include "gcr_regs.h"

/***** Definitions *****/

#define TP1 0xAAAAAAAA
#define TP2 0x55555555

/***** Globals *****/
uint32_t testaddr;
uint32_t testdata[16];
volatile uint32_t dummy;

#define PAGE_SIZE       0x400   // 4KB
#define PAGE_MASK       ~(PAGE_SIZE - 1)
int isr_cnt;
int isr_flags;
uint32_t failure_address;

int testcase;
int failed;
#if EMULATOR
#define MXC_FLASH_MEM_SIZE_TEST  0x00020000
#else 
#define MXC_FLASH_MEM_SIZE_TEST MXC_FLASH_MEM_SIZE
#endif
/***** Functions *****/

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
int check_mem(uint32_t startaddr, uint32_t length, uint32_t data)
{
    uint32_t * ptr;
    for (ptr = (uint32_t*)startaddr; ptr < (uint32_t*)(startaddr + length);
        ptr++) {

        if (*ptr != data)
        {
            failure_address = (uint32_t)ptr;
            return 0;
        }
    }

    return 1;
}

//******************************************************************************
int check_erased(uint32_t startaddr, uint32_t length)
{
    return check_mem(startaddr, length, 0xFFFFFFFF);
}

//******************************************************************************
int check_not_erased(uint32_t startaddr, uint32_t length)
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
                failure_address = (uint32_t)ptr;
                return 0;
            }
        }
    }

    return 1;
}
void flash_test_init(void)
{
    // Set flash clock divider to generate a 1MHz clock from the APB clock
    // APB clock is 20MHz on the emulator
    // APB clock is 54MHz on the real silicon
    #ifdef EMULATOR
      MXC_FLC0->clkdiv = 20;
      MXC_FLC1->clkdiv = 20;
    #else
      MXC_FLC0->clkdiv = 54;
      MXC_FLC1->clkdiv = 54;
    #endif

    // Setup and enable interrupt
    NVIC_EnableIRQ(FLC0_IRQn);
    __enable_irq();

    NVIC_EnableIRQ(FLC1_IRQn);
    __enable_irq();
    // Seed random number generator
    // prng32_init(PRNG_SEED);
}
void FLC_IRQHandler(void)
{
    uint32_t temp;
    isr_cnt++;
    temp = MXC_FLC0->intr;

    if (temp & MXC_F_FLC_INTR_DONE)
    {
        MXC_FLC0->intr &= ~MXC_F_FLC_INTR_DONE;
    }

    if (temp & MXC_F_FLC_INTR_AF)
    {
        MXC_FLC0->intr &= ~MXC_F_FLC_INTR_AF;
    }

    isr_flags = temp;
}

void FLC1_IRQHandler(void)
{
    uint32_t temp;
    isr_cnt++;
    temp = MXC_FLC1->intr;

    if (temp & MXC_F_FLC_INTR_DONE)
    {
        MXC_FLC1->intr &= ~MXC_F_FLC_INTR_DONE;
    }

    if (temp & MXC_F_FLC_INTR_AF)
    {
        MXC_FLC1->intr &= ~MXC_F_FLC_INTR_AF;
    }

    isr_flags = temp;
}
//******************************************************************************
int main(void)
{

    printf("\n\n\n*************** Flash Controller Roadtest ***************\n");
    flash_test_init();
    testcase = 0;
ICC_Disable();
    #if 1
    /***** Test 1: Mass erase and verify *****/
    printf("Test 1: Mass erase and verify\n");

    testcase++;
    failed = 0;
    failure_address = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    if (FLC_MassErase() != E_NO_ERROR){
        failed = 1;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
        printf("Interrupt did not occur\n");
        failed = 2;
    }

    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_MEM_SIZE_TEST)){
        failed = 3;
    }

    if (failed) {
        printf("Test 1 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 1
    /***** Test 2: 32-bit write *****/
    printf("Test 2: 32-bit write\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    memset(testdata, 0xFF, sizeof(testdata));

    for (testaddr = MXC_FLASH_MEM_BASE; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += 4) {

        /* Clear and enable flash programming interrupts */
        MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
        isr_flags = 0;
        isr_cnt = 0;

        // get test data
        testdata[0] = testdata[1];
        testdata[1] = 0x02;

        // Write word
        if (FLC_Write32(testaddr, testdata[1]) != E_NO_ERROR) {
            failed = 1;
            break;
        }

        if (testaddr == MXC_FLASH_MEM_BASE) {
            // Check that word is written and following word is not modified
            if (flash_verify(testaddr, 8, (uint8_t*)&testdata[1]) != E_NO_ERROR) {
                failed = 2;
                break;
            }
        } else if (testaddr == (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST - 4)) {
            // Check that word is written and previous word is not modified
            if (flash_verify(testaddr - 4, 8, (uint8_t*)&testdata[0]) != E_NO_ERROR) {
                failed = 3;
                break;
            }
        } else {
            // Check that word is written and previous and following words are not modified
            if (flash_verify(testaddr - 4, 12, (uint8_t*)testdata) != E_NO_ERROR) {
                failed = 4;
                break;
            }
        }

        if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
            printf("Interrupt did not occur at 0x%08x\n", (unsigned int)testaddr);
            failed = 5;
        }
    }

    if (failed) {
        printf("Test 2 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 1
    /***** Test 3: Mass Erase *****/
    printf("Test 3: Mass Erase\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    /* Clear and disable flash programming interrupts */
    MXC_FLC0->intr = 0;
    isr_flags = 0;
    isr_cnt = 0;

    if (FLC_MassErase() != E_NO_ERROR){
        failed = 1;
    }

    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_MEM_SIZE_TEST)){
        failed = 2;
    }

    if (!failed && (isr_cnt != 0)) {
        printf("Interrupt occurred\n");
        failed = 3;
    }

    if (failed) {
        printf("Test 3 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 1
    /***** Test 4: 128-bit write with burst disabled *****/
    printf("Test 4: 128-bit write with burst disabled\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    memset(testdata, 0xFF, sizeof(testdata));
    MXC_FLC0->cn &= ~MXC_F_FLC_CN_BRST;  // disable burst mode

    for (testaddr = MXC_FLASH_MEM_BASE; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += 16) {

        /* Clear and enable flash programming interrupts */
        MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
        isr_flags = 0;
        isr_cnt = 0;

        // get test data
        testdata[0] = testdata[4];
        testdata[1] = testdata[5];
        testdata[2] = testdata[6];
        testdata[3] = testdata[7];
        testdata[4] = 0x02;
        testdata[5] = 0x03;
        testdata[6] = 0x04;
        testdata[7] = 0x05;

        // Write word
        if (FLC_Write128(testaddr, &testdata[4]) != E_NO_ERROR) {
            failed = 1;
            break;
        }

        if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
            printf("Interrupt did not occur at 0x%08x\n", (unsigned int)testaddr);
            failed = 2;
        }

        if (testaddr == MXC_FLASH_MEM_BASE) {
            // Check that word is written and following word is not modified
            if (flash_verify(testaddr, 32, (uint8_t*)&testdata[4]) != E_NO_ERROR) {
                failed = 3;
                break;
            }
        } else if (testaddr == (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST - 16)) {
            // Check that word is written and previous word is not modified
            if (flash_verify(testaddr - 16, 32, (uint8_t*)&testdata[0]) != E_NO_ERROR) {
                failed = 4;
                break;
            }
        } else {
            // Check that word is written and previous and following words are not modified
            if (flash_verify(testaddr - 16, 48, (uint8_t*)testdata) != E_NO_ERROR) {
                failed = 5;
                break;
            }
        }
    }

    if (failed) {
        printf("Failed = %d\n", failed);
        
    }
    else
        printf("Passed\n");
    #endif
    #if 0
    /***** Test 5: Page Erase *****/
    printf("Test 5: Page Erase\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // Fill flash
    if (!flash_fill(MXC_FLASH_MEM_BASE, MXC_FLASH_MEM_SIZE_TEST, TP1)){
        failed = 1;
    }

    isr_flags = 0;
    isr_cnt = 0;

    // Erase 2nd page
    if (!failed && FLC_PageErase(MXC_FLASH_MEM_BASE + MXC_FLASH_PAGE_SIZE) != E_NO_ERROR){
        failed = 2;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
        failed = 3;
    }

    // Check that page is erased
    if (!failed && !check_erased(MXC_FLASH_MEM_BASE + MXC_FLASH_PAGE_SIZE, MXC_FLASH_PAGE_SIZE)){
        failed = 4;
    }

    // Check that previous page is not erased
    if (!failed && !check_not_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_PAGE_SIZE)){
        failed = 5;
    }

    // Check that following page is not erased
    if (!failed && !check_not_erased(MXC_FLASH_MEM_BASE + (2 * MXC_FLASH_PAGE_SIZE), MXC_FLASH_PAGE_SIZE)){
        failed = 6;
    }

    /* Clear and disable flash programming interrupts */
    MXC_FLC0->intr = 0;
    isr_flags = 0;
    isr_cnt = 0;

    // Erase 1st page
    if (FLC_PageErase(MXC_FLASH_MEM_BASE) != E_NO_ERROR){
        failed = 7;
    }

    // Check that page is erased
    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_PAGE_SIZE)){
        failed = 8;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != 0)) {
        printf("Interrupt did not occur\n");
        failed = 9;
    }

    if ((failed == 0)) {
        for (testaddr = MXC_FLASH_MEM_BASE + (2 * MXC_FLASH_PAGE_SIZE); testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += MXC_FLASH_PAGE_SIZE) {

            /* Clear and enable flash programming interrupts */
            MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
            isr_flags = 0;
            isr_cnt = 0;

            // Erase page
            if (FLC_PageErase(testaddr) != E_NO_ERROR) {
                failed = 10;
                break;
            }

            if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
                printf("Interrupt did not occur at 0x%08x\n", (unsigned int)testaddr);
                failed = 11;
            }

            // Check that page is erased
            if (!check_erased(testaddr, MXC_FLASH_PAGE_SIZE)) {
                failed = 12;
                break;
            }

            if ( testaddr != (MXC_FLASH_MEM_BASE + (MXC_FLASH_MEM_SIZE_TEST - MXC_FLASH_PAGE_SIZE)) ) {
                // Check that following page is not erased
                if (!check_not_erased(testaddr + MXC_FLASH_PAGE_SIZE, MXC_FLASH_PAGE_SIZE)) {
                    failed = 13;
                    break;
                }
            }
        }
    }

    if (failed) {
        printf("Test 5 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 1
    /***** Test 6: 128-bit write with burst enabled *****/
    printf("Test 6: 128-bit write with burst enabled\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    FLC_MassErase();
    memset(testdata, 0xFF, sizeof(testdata));
    MXC_FLC0->cn |= MXC_F_FLC_CN_BRST;  // disable burst mode

    for (testaddr = MXC_FLASH_MEM_BASE; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += 16) {

        /* Clear and enable flash programming interrupts */
        MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
        isr_flags = 0;
        isr_cnt = 0;

        // get test data
        testdata[0] = testdata[4];
        testdata[1] = testdata[5];
        testdata[2] = testdata[6];
        testdata[3] = testdata[7];
        testdata[4] = 0x02;
        testdata[5] = 0x03;
        testdata[6] = 0x04;
        testdata[7] = 0x05;

        // Write word
        if (FLC_Write128(testaddr, &testdata[4]) != E_NO_ERROR) {
            failed = 1;
            break;
        }

        if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
            printf("Interrupt did not occur at 0x%08x\n", (unsigned int)testaddr);
            failed = 2;
        }

        if (testaddr == MXC_FLASH_MEM_BASE) {
            // Check that word is written and following word is not modified
            if (flash_verify(testaddr, 32, (uint8_t*)&testdata[4]) != E_NO_ERROR) {
                failed = 3;
                break;
            }
        } else if (testaddr == (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST - 16)) {
            // Check that word is written and previous word is not modified
            if (flash_verify(testaddr - 16, 32, (uint8_t*)&testdata[0]) != E_NO_ERROR) {
                failed = 4;
                break;
            }
        } else {
            // Check that word is written and previous and following words are not modified
            if (flash_verify(testaddr - 16, 48, (uint8_t*)testdata) != E_NO_ERROR) {
                failed = 5;
                break;
            }
        }
    }

    if (failed)        
        printf("Failed\n");
    else
        printf("Passed\n");
    #endif
    #if 0
    /***** Test 7: Mass erase and verify *****/
    printf("Test 7: Mass erase and verify\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    if (FLC_MassErase() != E_NO_ERROR){
        failed = 1;
    }

    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_MEM_SIZE_TEST)){
        failed = 2;
    }

    if (failed) {
        printf("Test 7 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 0
    /***** Test 8 : Write not erased 32-bit location *****/
    printf("Test 8: Write not erased 32-bit location\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    // get test data
    testdata[0] = 0x02;
    testdata[1] = 0x03;

    // get random test address
    testaddr = ((0x02 << 2) % MXC_FLASH_MEM_SIZE_TEST) + MXC_FLASH_MEM_BASE;

    /* Clear and enable flash programming interrupts */
    MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // Write word
    if (FLC_Write32(testaddr, testdata[0]) != E_NO_ERROR){
        failed = 1;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
        printf("Interrupt did not occur\n");
        failed = 2;
    }

    // Check that word is written
    if (!failed && !check_mem(testaddr, 4, testdata[0])){
        failed = 3;
    }

    /* Clear and enable flash programming interrupts */
    MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // Write word again
    // Since interrupts are enabled, this function will not detect the error
    if (!failed && (FLC_Write32(testaddr, testdata[1]) != E_NO_ERROR)){
        failed = 4;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_AF)) {
        printf("Interrupt did not occur\n");
        failed = 5;
    }

    // Check that word is not changed from initial write
    if (!failed && !check_mem(testaddr, 4, testdata[0])){
        failed = 6;
    }

    if (failed) {
        printf("Test 8 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 1
    /***** Test 9: Erase test page *****/
    printf("Test 9: Erase test page\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    // Erase page
    if (FLC_PageErase(testaddr & PAGE_MASK) != E_NO_ERROR){
        failed = 1;
    }

    // Check that page is erased
    if (!failed && !check_erased(testaddr & PAGE_MASK, MXC_FLASH_PAGE_SIZE)){
        failed = 2;
    }

    if (failed) {
        printf("Test 9 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 0
    /***** Test 10: Write not erased 128-bit location *****/
    printf("Test 10: Write not erased 128-bit location\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    // get test data
    testdata[0] = 0x02;
    testdata[1] = 0x03;
    testdata[2] = 0x04;
    testdata[3] = 0x05;
    testdata[4] = 0x06;
    testdata[5] = 0x07;
    testdata[6] = 0x08;
    testdata[7] = 0x09;

    // get random test address
    testaddr = ((0x02 << 4) % MXC_FLASH_MEM_SIZE_TEST) + MXC_FLASH_MEM_BASE;

    /* Clear and enable flash programming interrupts */
    MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // Write words
    if (FLC_Write128(testaddr, &testdata[0]) != E_NO_ERROR){
        failed = 1;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_DONE)) {
        printf("Interrupt did not occur\n");
        failed = 2;
    }

    // Check that words are written
    if (!failed && (flash_verify(testaddr, 16, (uint8_t*)&testdata[0]) != E_NO_ERROR)){
        failed = 3;
    }

    /* Clear and enable flash programming interrupts */
    MXC_FLC0->intr = (MXC_F_FLC_INTR_DONEIE | MXC_F_FLC_INTR_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // Write words again
    // Since interrupts are enabled, this function will not detect the error
    if (!failed && FLC_Write128(testaddr, &testdata[1]) != E_NO_ERROR){
        failed = 4;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INTR_AF)) {
        printf("Interrupt did not occur\n");
        failed = 5;
    }

    // Check that words are not changed from initial write
    if (!failed && (flash_verify(testaddr, 16, (uint8_t*)&testdata[0]) != E_NO_ERROR)){
        failed = 6;
    }

    if (failed)
        printf("Failed\n");
    else
        printf("Passed\n");
    #endif
    #if 1
    /***** Test 11: Mass erase and verify *****/
    printf("Test 11: Mass erase and verify\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    if (FLC_MassErase() != E_NO_ERROR){
        failed = 1;
    }

    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, MXC_FLASH_MEM_SIZE_TEST)){
        failed = 2;
    }

    if (failed) {
        printf("Test 11 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");
    #endif
    #if 0
    /***** Test 12: Flash flip *****/
    printf("Test 12: Page flip\n");
    testcase++;
    failed = 0;
    failure_address = 0;


    //Write test pattern lower half
    for(testaddr = MXC_FLASH_MEM_BASE; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST/2); testaddr += 0x4) {
        testdata[0] = 0x02;

        if (FLC_Write32(testaddr, testdata[0]) != E_NO_ERROR) {
            failed = 1;
            break;
        }
    }

    //Write test pattern to upper half
    for(testaddr = MXC_FLASH_MEM_BASE+MXC_FLASH_MEM_SIZE_TEST/2; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += 0x4) {
        testdata[0] = 0x02;

        if (FLC_Write32(testaddr, testdata[0]) != E_NO_ERROR) {
            failed = 2;
            break;
        }
    }

    //Flip the flash page
    MXC_GCR->scon = MXC_F_GCR_SCON_FLASH_PAGE_FLIP;

    //Read test pattern from upper half
    for(testaddr = MXC_FLASH_MEM_BASE+MXC_FLASH_MEM_SIZE_TEST/2; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST); testaddr += 0x4) {
        testdata[0] = 0x02;

        if (flash_verify(testaddr, 0x4, (uint8_t*)testdata) != E_NO_ERROR) {
            failed = 3;
            break;
        }
    }

    //Read test pattern from lower half
    for(testaddr = MXC_FLASH_MEM_BASE; testaddr < (MXC_FLASH_MEM_BASE + MXC_FLASH_MEM_SIZE_TEST/2); testaddr += 0x4) {
        testdata[0] = 0x02;

        if (flash_verify(testaddr, 0x4, (uint8_t*)testdata) != E_NO_ERROR) {
            failed = 4;
            break;
        }
    }

    if (failed) {
        printf("Test 12 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");

    //Restore the flash page
    MXC_GCR->scon = MXC_F_GCR_SCON_FLASH_PAGE_FLIP;
    #endif

    #if 1

    /***** Test 13: Flash Write function *****/
    printf("Test 13: Flash write\n");
    testcase++;
    failed = 0;
    failure_address = 0;
    uint32_t erased = 0xFFFFFFFF;

    FLC_MassErase();

    testdata[0] = testdata[4];
    testdata[1] = testdata[5];
    testdata[2] = testdata[6];
    testdata[3] = testdata[7];
    testdata[4] = 0x02;
    testdata[5] = 0x03;
    testdata[6] = 0x04;
    testdata[7] = 0x05;
    testdata[8] = 0x06;
    testdata[9] = 0x07;
    testdata[10] = 0x08;
    testdata[11] = 0x09;
    testdata[12] = 0x10;
    testdata[13] = 0x11;
    testdata[14] = 0x12;
    testdata[15] = 0x13;
    // Write the test data on an unaligned address
    testaddr = MXC_FLASH_MEM_BASE + 0x0;

    // Write word
    if (FLC_Write(testaddr, 16,(uint32_t*)testdata) != E_NO_ERROR) {
        failed = 1;
    }

    // Verify the data
    if(flash_verify(testaddr, 16, (uint8_t*)(uint8_t*)testdata) != E_NO_ERROR) {
        failed = 2;
    }

    // Make sure the previous bytes are still erased
    else if(check_erased(MXC_FLASH_MEM_BASE, 0x3) != E_NO_ERROR) {
        failed = 3;
    } 

    // Make sure next bytes are still erased
    else if(memcmp((void*)(MXC_FLASH_MEM_BASE+64), &erased, 3) != 0) {
        failed = 4;
    }
    if (failed) {
        printf("Test 13 Failed, failed = %d\r\n",failed);
        
    } else
        printf("Passed\n");

    #endif

    /***** Test Complete *****/

    //Erase the flash
    // FLC_MassErase();
       
ICC_Enable();
    printf("Done testing\n");
    while(1);
    return 0;
}