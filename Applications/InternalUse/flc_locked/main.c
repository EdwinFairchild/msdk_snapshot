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
//#include "prng.h"
//#include "flc_test.h"
#include "flc.h"

#include "flc_regs.h"

/***** Definitions *****/
#ifdef EMULATOR
    #define FLASH_FULL_MEM_SIZE  0x00040000
#else
    #define FLASH_FULL_MEM_SIZE  MXC_FLASH_MEM_SIZE
#endif

#define PAGE_SIZE       MXC_FLASH_PAGE_SIZE   
#define PAGE_MASK       ~(PAGE_SIZE - 1)

#define FLC_CN_UNLOCK_VALUE   0x20000000UL
int isr_cnt;
int isr_flags;
uint32_t failure_address;
/***** Globals *****/
uint32_t testaddr;
uint32_t testdata[12];
volatile uint32_t dummy;

int testcase;
int failed;
/***** Functions *****/
static int prepare_flc(void)
{
  uint32_t fcntl;

  // Set flash clock divider to generate a 1MHz clock from the APB clock
  MXC_FLC->clkdiv = SystemCoreClock / 1000000;

  /* Check if the flash controller is busy */
  fcntl = MXC_FLC->cn;
  if(FLC_Busy()){
    return E_BUSY;
  }

  /* Clear stale errors */
  if (MXC_FLC->intr & MXC_F_FLC_INT_AF) {
    MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
  }

  /* Unlock flash */
  MXC_FLC->cn = (MXC_FLC->cn & ~MXC_F_FLC_CN_UNLOCK) | FLC_CN_UNLOCK_VALUE;

  return E_NO_ERROR;
}
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
int flash_page_erase_locked(uint32_t address)
{
  int err;

  if ((err = prepare_flc()) != E_NO_ERROR)
    return err;

  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  /* Write page erase code */
  MXC_FLC->cn = (MXC_FLC->cn & ~MXC_F_FLC_CN_ERASE_CODE) | MXC_S_FLC_CN_ERASE_CODE_ERASEPAGE;
  /* Issue page erase command */
  MXC_FLC->addr = address;
  MXC_FLC->cn |= MXC_F_FLC_CN_PGE;

  /* Wait until flash operation is complete */
  while (FLC_Busy());

  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  /* Check access violations */
  if (MXC_FLC->intr & MXC_F_FLC_INT_AF) {
    MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
    return E_BAD_STATE;
  }
  return E_NO_ERROR;
}

//******************************************************************************
int flash_mass_erase_locked(void)
{
  int err;

  if ((err = prepare_flc()) != E_NO_ERROR)
    return err;

  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  /* Write mass erase code */
  MXC_FLC->cn = (MXC_FLC->cn & ~MXC_F_FLC_CN_ERASE_CODE) | MXC_S_FLC_CN_ERASE_CODE_ERASEALL;

  /* Issue mass erase command */
  MXC_FLC->cn |= MXC_F_FLC_CN_ME;

  /* Wait until flash operation is complete */
  while(FLC_Busy());
  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  /* Check access violations */
  if (MXC_FLC->intr & MXC_F_FLC_INT_AF) {
    MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
    return E_BAD_STATE;
  }
  return E_NO_ERROR;
}
//******************************************************************************
int flash_write32_locked(uint32_t address, uint32_t data)
{
  int err;
  volatile uint32_t *line_addr;
  volatile uint32_t line;

  if (address & 0x3) {
    return E_BAD_PARAM;
  }

  if (address >= (MXC_FLASH_MEM_BASE + FLASH_FULL_MEM_SIZE))
    return E_BAD_PARAM;

  if ((err = prepare_flc()) != E_NO_ERROR)
    return err;

  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  // write in 32-bit units
  MXC_FLC->cn |= MXC_F_FLC_CN_WDTH;
  MXC_FLC->cn &= ~MXC_F_FLC_CN_BRST;

  // write the data
  MXC_FLC->addr = address;
  MXC_FLC->data[0] = data;
  MXC_FLC->cn |= MXC_F_FLC_CN_WR;


  /* Wait until flash operation is complete */
  while (FLC_Busy()) {}

  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  /* Check access violations */
  if (MXC_FLC->intr & MXC_F_FLC_INT_AF) {
    MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
    return E_BAD_STATE;
  }

  SYS_Flash_Operation();

  return E_NO_ERROR;
}

// Medical devices don't have the 128-bit write
#if 1
//******************************************************************************
int flash_write128_locked(uint32_t address, uint32_t data[4])
{
  int err;
  volatile uint32_t *line_addr;
  volatile uint32_t line;

  if (address & 0x3) {
    return E_BAD_PARAM;
  }

  if ( (address + 12) >= (MXC_FLASH_MEM_BASE + FLASH_FULL_MEM_SIZE))
    return E_BAD_PARAM;

  if ((err = prepare_flc()) != E_NO_ERROR)
    return err;

  // write 128-bits
  MXC_FLC->cn &= ~MXC_F_FLC_CN_WDTH;

  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  // write the data
  MXC_FLC->addr = address;
  MXC_FLC->data[0] = data[0];
  MXC_FLC->data[1] = data[1];
  MXC_FLC->data[2] = data[2];
  MXC_FLC->data[3] = data[3];
  MXC_FLC->cn |= MXC_F_FLC_CN_WR;

  /* Wait until flash operation is complete */
  while (FLC_Busy());

  /* Lock flash */
  MXC_FLC->cn &= ~MXC_F_FLC_CN_UNLOCK;

  /* Check access violations */
  if (MXC_FLC->intr & MXC_F_FLC_INT_AF) {
    MXC_FLC->intr &= ~MXC_F_FLC_INT_AF;
    return E_BAD_STATE;
  }

  SYS_Flash_Operation();
  
  return E_NO_ERROR;
}
#endif

//******************************************************************************
int main(void)
{

  printf("\n\n\n*************** Flash Controller Roadtest ***************\n");

  flash_test_init();

  testcase = 0;
    /***** Test 1: Mass erase and verify *****/
    printf("Test 1: Mass erase and verify\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC->intr = (MXC_F_FLC_INT_DONEIE | MXC_F_FLC_INT_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    if (FLC_MassErase() != E_NO_ERROR)
      failed = 1;

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_DONEIE)) {
      printf("Interrupt did not occur\n");
      failed = 2;
    }

    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, FLASH_FULL_MEM_SIZE))
      failed = 3;

    if (failed)
    {
      printf("Test Failed, failed = %d\r\n",failed);
      //Fail();
    }
    else
      printf("Passed\n");
      //Pass();

  /***** Test 2: 32-bit write without "Unlock" *****/

    // get random test address
    testaddr = ((20 << 2) % FLASH_FULL_MEM_SIZE) + MXC_FLASH_MEM_BASE;

    printf("Test 2: 32-bit write @%x without \"Unlock\"\n", (unsigned int)testaddr);
    testcase++;
    failed = 0;

    // get test data
    testdata[0] = 20;

    /* Clear and disable flash programming interrupts */
    MXC_FLC->intr = 0;
    
    // Try to write word. This should fail.
    if (flash_write32_locked(testaddr, testdata[0]) == E_NO_ERROR) {
      printf("Operation should have failed but didn't\n");
      failed = 1;
    }

    // Check for AF flag
    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_AFIE)) {
      printf("AF Interrupt did not occur\n");
      failed = 2;
    }

    // Check that word is not written
    if (!failed && check_mem(testaddr, 4, testdata[0]))
      failed = 3;

    if (failed)
    {
      printf("Test Failed, failed = %d\r\n",failed);
      //Fail();
    }
    else
      printf("Passed\n");
      //Pass();

#if 1
  /***** Test 3: 128-bit write without "Unlock" *****/

    // get random test address
    testaddr = ((20 << 2) % FLASH_FULL_MEM_SIZE) + MXC_FLASH_MEM_BASE;

    printf("Test 3: 128-bit write @%x without \"Unlock\"\n", (unsigned int)testaddr);
    testcase++;
    failed = 0;

    // get test data
    testdata[0] = 20;
    testdata[1] = 20;
    testdata[2] = 20;
    testdata[3] = 20;

    /* Clear and disable flash programming interrupts */
    MXC_FLC->intr = 0;

    // Try to write word. This should fail.
    if (flash_write128_locked(testaddr, &testdata[0]) == E_NO_ERROR) {
      printf("Operation should have failed but didn't\n");
      failed = 1;
    }

    // Check for AF flag
    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_AFIE)) {
      printf("AF Interrupt did not occur\n");
      failed = 2;
    }

    // Check that word is not written
    if (!failed && check_mem(testaddr, 16, &testdata[0]))
      failed = 3;

    if (failed)
    {
      printf("Test Failed, failed = %d\r\n",failed);
      //Fail();
    }
    else
      printf("Passed\n");
      //Pass();
#endif

  /***** Test 4: Page erase without "Unlock" *****/

    // get random test address
    // testaddr = ((20 * PAGE_SIZE) % FLASH_FULL_MEM_SIZE) + MXC_FLASH_MEM_BASE;
    testaddr = (3*PAGE_SIZE) + MXC_FLASH_MEM_BASE;

    printf("Test 4: Page erase @%x without \"Unlock\"\n", (unsigned int)testaddr);
    testcase++;
    failed = 0;

    /* Clear and disable flash programming interrupts */
    MXC_FLC->intr = 0;

    // fill test page
    if (!flash_fill(testaddr, PAGE_SIZE, 20))
      failed = 1;

    if (!flash_fill(testaddr-PAGE_SIZE, PAGE_SIZE, 20))
      failed = 1;

    if (!flash_fill(testaddr+PAGE_SIZE, PAGE_SIZE, 20))
      failed = 1;

    // Try to erase page. The erase should not occur, but no error is flagged.
    if (!failed)
      flash_page_erase_locked(testaddr & PAGE_MASK);

    // Check for AF flag
    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_AFIE)) {
      printf("AF Interrupt did not occur\n");
      failed = 2;
    }

    // Check that page is not erased
    if (!failed && !check_not_erased(testaddr & PAGE_MASK, PAGE_SIZE))
    {
      printf("Page was erased\r\n");
      failed = 3;
    }

    if (failed)
    {
      printf("Test Failed, failed = %d\r\n",failed);
      //Fail();
    }
    else
      printf("Passed\n");
      //Pass();

  /***** Test 5: Mass erase without "Unlock" *****/
    printf("Test 5: Mass erase without \"Unlock\"\n");
    testcase++;
    failed = 0;

    /* Clear and disable flash programming interrupts */
    MXC_FLC->intr = 0;

    // erase and fill flash
    if (FLC_MassErase() != E_NO_ERROR)
      failed = 1;
    if (!failed && !flash_fill(MXC_FLASH_MEM_BASE, FLASH_FULL_MEM_SIZE, 20))
      failed = 2;

    // Try to mass erase. The erase should not occur, but no error is flagged.
    if (!failed)
      flash_mass_erase_locked();

    // Check for AF flag
    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_AFIE)) {
      printf("AF Interrupt did not occur\n");
      failed = 3;
    }

    // Check that flash is not erased
    if (!failed && !check_not_erased(MXC_FLASH_MEM_BASE, FLASH_FULL_MEM_SIZE))
      failed = 4;

    if (failed)
    {
      printf("Test Failed, failed = %d\r\n",failed);
      //Fail();
    }
    else
      printf("Passed\n");
      //Pass();

  /***** Test 6: Mass erase and verify *****/
    printf("Test 6: Mass erase and verify\n");
    testcase++;
    failed = 0;
    failure_address = 0;

    /* Clear and enable flash programming interrupts */
    MXC_FLC->intr = (MXC_F_FLC_INT_DONEIE | MXC_F_FLC_INT_AFIE);
    isr_flags = 0;
    isr_cnt = 0;

    if (FLC_MassErase() != E_NO_ERROR)
      failed = 1;

    if (!failed && (isr_cnt != 1) && (isr_flags != MXC_F_FLC_INT_DONEIE)) {
      printf("Interrupt did not occur\n");
      failed = 2;
    }

    if (!failed && !check_erased(MXC_FLASH_MEM_BASE, FLASH_FULL_MEM_SIZE))
      failed = 3;

    if (failed)
    {
      printf("Test Failed, failed = %d\r\n",failed);
      //Fail();
    }
    else
      printf("Passed\n");
      //Pass();

  /***** Test Complete *****/

  // if (g_FailCount == 0)
  //   printf("\nAll tests PASSED\n");
  // else
  //   printf("\n%u tests FAILED\n", g_FailCount);

  // Done();
  return 0;
}