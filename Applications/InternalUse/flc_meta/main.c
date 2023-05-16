/****************************************************************************
 * Copyright (C) 2012 Maxim Integrated, All Rights Reserved.
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
 *             $Id: main.c 23558 2016-07-01 22:07:17Z kevin.gillespie $
 *
 ********************************************************************************
 */

/***** Includes *****/
#include "mxc_config.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

//#include "roadtest.h"
// #include "prng.h"
//#include "flc_test.h"
#include "flc.h"

#include "flc_regs.h"

/***** Definitions *****/

#define TP1 0xAAAAAAAA
#define TP2 0x55555555

/***** Globals *****/
uint32_t testaddr;
uint32_t testdata[12];
volatile uint32_t dummy;

int isr_cnt;
int isr_flags;
uint32_t failure_address;

int testcase;
int failed;

#ifdef EMULATOR
    #define FLASH_FULL_MEM_SIZE  0x00040000
#else
    #define FLASH_FULL_MEM_SIZE  MXC_FLASH_MEM_SIZE
#endif

extern uint32_t _test_page;
extern uint32_t _etest_page;

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
    FLC_Mass_Erase();
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
  if ( !(MXC_FLC->intr & MXC_F_FLC_INT_AFIE) && (MXC_FLC->intr & MXC_F_FLC_INT_AF) ) {
    MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
    return 0;
  }

  return 1;
}
int flash_verify(uint32_t address, uint32_t length, uint32_t * data)
{
  volatile uint32_t * ptr;


  for (ptr = (uint32_t*)address; ptr < (uint32_t*)(address + length); ptr++, data++) {
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
      MXC_FLC->clkdiv = 20;
    #else
      MXC_FLC->clkdiv = 54;
    #endif

    // Setup and enable interrupt
    NVIC_EnableIRQ(FLC_IRQn);
    __enable_irq();

    // Seed random number generator
    // prng32_init(PRNG_SEED);
}
void FLC_IRQHandler(void)
{
    uint32_t temp;
    isr_cnt++;
    temp = MXC_FLC->intr;

    if (temp & MXC_F_FLC_INT_DONE)
    {
        MXC_FLC->intr &= ~MXC_F_FLC_INT_DONE;
    }

    if (temp & MXC_F_FLC_INT_AF)
    {
        MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
    }

    isr_flags = temp;
}
//******************************************************************************
int main(void)
{

    printf("\n\n\n*************** Flash Controller Roadtest ***************\n");

    flash_test_init();

    testcase = 0;

    /***** Test 1: Page erase and verify *****/
    printf("Test 1: Page erase and verify, 0x%x\n", &_test_page);
    testcase++;
    failed = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC->intr = (MXC_F_FLC_INT_DONEIE | MXC_F_FLC_INT_AFIE);
    isr_flags = 0;
    isr_cnt = 0;
    
    if (FLC_PageErase((uint32_t)&_test_page) != E_NO_ERROR)
        failed = 1;

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_DONE)) {
        printf("Interrupt did not occur\n");
        failed = 2;
    }

    if (!failed && !check_erased((uint32_t)&_test_page, MXC_FLASH_PAGE_SIZE))
        failed = 3;

    if (failed) {
        printf("Test 1 Failed, failed = %d\r\n",failed);
        //Fail();
    } else
        printf("Passed\n");
        //Pass();

    /***** Test 2: 32-bit write *****/
    printf("Test 2: 32-bit write\n");
    testcase++;
    failed = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC->intr = (MXC_F_FLC_INT_DONEIE | MXC_F_FLC_INT_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // get test data
    testdata[0] = 20;
    testdata[1] = 0xFFFFFFFF;
    testaddr = (uint32_t)&_test_page + 0x10;

    // Write word
    if (FLC_Write(testaddr, 4, &testdata[0]) != E_NO_ERROR) {
        failed = 1;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_DONEIE)) {
        printf("Interrupt did not occur\n");
        failed = 2;
    }

    // Check that word is written and previous and following words are not modified
    if (flash_verify(testaddr, 4, &testdata[0]) != E_NO_ERROR) {
        failed = 3;
    }
    if (flash_verify(testaddr+4, 4, &testdata[1]) != E_NO_ERROR) {
        failed = 4;
    }
    if (flash_verify(testaddr-4, 4, &testdata[1]) != E_NO_ERROR) {
        failed = 5;
    }

    if (failed) {
        printf("Test 2 Failed, failed = %d\r\n",failed);
        //Fail();
    } else
        printf("Passed\n");
        //Pass();

#if 1
    /***** Test 3: 128-bit write *****/
    printf("Test 3: 128-bit write\n");
    testcase++;
    failed = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC->intr = (MXC_F_FLC_INT_DONEIE | MXC_F_FLC_INT_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    // get test data
    testdata[0] = 20;
    testdata[1] = 20;
    testdata[2] = 20;
    testdata[3] = 20;
    testdata[4] = 0xFFFFFFFF;
    testdata[5] = 0xFFFFFFFF;
    testdata[6] = 0xFFFFFFFF;
    testdata[7] = 0xFFFFFFFF;

    testaddr = (uint32_t)&_test_page + 0x40;

    // Write word
    if (FLC_Write128(testaddr, &testdata[0]) != E_NO_ERROR) {
        failed = 1;
    }

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_DONEIE)) {
        printf("Interrupt did not occur\n");
        failed = 2;
    }

    // Check that word is written and previous and following words are not modified
    if (flash_verify(testaddr, 16, &testdata[0]) != E_NO_ERROR) {
        failed = 3;
    }
    if (flash_verify(testaddr+16, 16, &testdata[4]) != E_NO_ERROR) {
        failed = 4;
    }
    if (flash_verify(testaddr-16, 16, &testdata[4]) != E_NO_ERROR) {
        failed = 5;
    }

    if (failed) {
        printf("Test 3 Failed, failed = %d\r\n",failed);
        //Fail();
    } else
        printf("Passed\n");
        //Pass();

#endif
    /***** Test Complete *****/

    // if (g_FailCount == 0)
    //     printf("\nAll tests PASSED\n");
    // else
    //     printf("\n%u tests FAILED\n", g_FailCount);

    // Done();
    return 0;
}