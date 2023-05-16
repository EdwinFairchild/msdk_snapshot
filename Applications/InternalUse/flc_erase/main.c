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
 *             $Id: main.c 25341 2016-12-01 19:34:58Z kevin.gillespie $
 *
 ********************************************************************************
 */

/***** Includes *****/
#include <stdio.h>
#include "mxc_config.h"
#include "flc.h"

/***** Definitions *****/
#define TEST_START      (uint32_t)&_test_page
#define TEST_END        (uint32_t)&_etest_page
#define TEST_BUFFER     &erase_buffer[0x8]

/***** Globals *****/

extern uint32_t _test_page;
extern uint32_t _etest_page;

uint8_t erase_buffer[MXC_FLASH_PAGE_SIZE + 0x10];

// Writing 0, 1, 2, ... by byte
//******************************************************************************
int flash_fill(uint32_t start, uint32_t end, unsigned value)
{
    int retval;
    uint32_t i, length, addr;
    uint8_t buffer[4];

    buffer[0] = value + 0;
    buffer[1] = value + 1;
    buffer[2] = value + 2;
    buffer[3] = value + 3;

    //  Use FLC_Write until we're aligned
    length = (4 - (start % 4));
    retval = FLC_Write(start, length, buffer);
    if(retval != E_NO_ERROR) {
        return retval;
    }

    // Shift the data
    buffer[0] = buffer[0] + length;
    buffer[1] = buffer[1] + length;
    buffer[2] = buffer[2] + length;
    buffer[3] = buffer[3] + length;

    // Align the length
    addr = start + length;
    length = end - start - length;
    length = length - (length % 4);
    for(i = 0; i < length; i+=4) {

        retval = FLC_Write(addr, 4, buffer);
        if(retval != E_NO_ERROR) {
            return retval;
        }
        addr += 4;

        // Shift the data
        buffer[0] = buffer[0] + 4;
        buffer[1] = buffer[1] + 4;
        buffer[2] = buffer[2] + 4;
        buffer[3] = buffer[3] + 4;
    }

    // Write the remaining data
    length = end % 4;
    retval = FLC_Write(addr, length, buffer);
    if(retval != E_NO_ERROR) {
        return retval;
    }

    return E_NO_ERROR;
}

// Verifying 0xFF by byte
//******************************************************************************
int flash_check_erased(uint32_t start, uint32_t end)
{
    uint32_t addr;

    for(addr = start; addr < end; addr++) {

        if((uint8_t)*(uint32_t*)(addr) != 0xFF) {
            return -1;
        }
    }
    return E_NO_ERROR;
}

// Verifying flash_fill
//******************************************************************************
int flash_verify(uint32_t start, uint32_t end, unsigned value)
{
    uint32_t addr;
    uint8_t i = value;

    for(addr = start; addr < end; addr++) {

        if((uint8_t)*(uint32_t*)(addr) != i++) {
            return -1;
        }
    }
    return E_NO_ERROR;
}

//******************************************************************************
int flash_erase_test()
{
    return FLC_Erase((uint32_t)&_test_page, (uint32_t)(&_etest_page) - MXC_FLASH_PAGE_SIZE);
}

//******************************************************************************
int check_cannaries(void)
{
    int i;
    for(i = 0; i < 8; i++) {
        if((erase_buffer[i] != 0xAA) || (erase_buffer[MXC_FLASH_PAGE_SIZE+i] != 0xAA)) {
            return -1;
        }
    }

    return E_NO_ERROR;
}

//******************************************************************************
int test0(void)
{
    // Test the flash fill on page aligned boundaries 
    flash_fill(TEST_START, TEST_END, 0);

    if(flash_verify(TEST_START, TEST_END, 0) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -1;
    }

    flash_erase_test();
    return E_NO_ERROR;
}

//******************************************************************************
int test1(void)
{
    uint32_t start, end, value;
    // Test the flash fill on unaligned page boundaries 

    start = TEST_START + 0x5;
    end = TEST_START + 0x102 + 0x5;
    value = 3;
    if(flash_fill(start, end, value) != E_NO_ERROR) {
        printf("Error writing flash\n");
        flash_erase_test();
        return -1;
    }
    if(flash_verify(start, end, value) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -2;
    }
    flash_erase_test();

    start = TEST_START;
    end = TEST_START + MXC_FLASH_PAGE_SIZE + 0x102;
    value = 3;
    if(flash_fill(start, end, value) != E_NO_ERROR) {
        printf("Error writing flash\n");
        flash_erase_test();
        return -1;
    }
    if(flash_verify(start, end, value) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -2;
    }
    flash_erase_test();

    start = TEST_START + 0x5;
    end = TEST_END;
    value = 3;
    if(flash_fill(start, end, value) != E_NO_ERROR) {
        printf("Error writing flash\n");
        flash_erase_test();
        return -1;
    }
    if(flash_verify(start, end, value) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -2;
    }
    flash_erase_test();

    return E_NO_ERROR;
}

//******************************************************************************
int test2(void)
{
    uint32_t start, end, value, length;
    // Test the flash write on aligned page boundaries

    // Fill 4 of the pages to use as a buffer
    start = TEST_START;
    end = TEST_START + (4 * MXC_FLASH_PAGE_SIZE);
    value = 0;
    if(flash_fill(start, end, value) != E_NO_ERROR) {
        printf("Error writing flash\n");
        flash_erase_test();
        return -1;
    }

    length = end - start;
    start = end;
    end = start + length;
    if(FLC_Write(start, length, (uint8_t*)TEST_START) != E_NO_ERROR) {
        printf("Error writing to flash\n");
    }
    if(flash_verify(start, end, value) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -2;
    }
    if(FLC_Erase(start, end - MXC_FLASH_PAGE_SIZE) != E_NO_ERROR) {
        printf("Error erasing start: 0x%x end 0x%x\n", start, end);
        return -3;
    }
    if(flash_check_erased(start, end) != E_NO_ERROR) {
        printf("Error verifying erased start: 0x%x end 0x%x\n", start, end);
        return -4;
    }

    start = TEST_START + (4 * MXC_FLASH_PAGE_SIZE) + 0x127;
    end = TEST_END;
    length = end - start;
    if(FLC_Write(start, length, (uint8_t*)TEST_START) != E_NO_ERROR) {
        printf("Error writing to flash\n");
    }
    if(flash_verify(start, end, value) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -2;
    }
    if(FLC_Erase(TEST_START + (4 * MXC_FLASH_PAGE_SIZE), TEST_END - MXC_FLASH_PAGE_SIZE) != E_NO_ERROR) {
        printf("Error erasing start: 0x%x end 0x%x\n", start, end);
        return -3;
    }
    if(flash_check_erased(start, end) != E_NO_ERROR) {
        printf("Error verifying erased start: 0x%x end 0x%x\n", start, end);
        return -4;
    }

    start = TEST_START + (4 * MXC_FLASH_PAGE_SIZE) + 0x127;
    end = TEST_END - 0x353;
    length = end - start;
    if(FLC_Write(start, length, (uint8_t*)TEST_START) != E_NO_ERROR) {
        printf("Error writing to flash\n");
    }
    if(flash_verify(start, end, value) != E_NO_ERROR) {
        printf("Error verifying flash\n");
        flash_erase_test();
        return -2;
    }
    if(FLC_Erase(TEST_START + (4 * MXC_FLASH_PAGE_SIZE), TEST_END - MXC_FLASH_PAGE_SIZE) != E_NO_ERROR) {
        printf("Error erasing start: 0x%x end 0x%x\n", start, end);
        return -3;
    }
    if(flash_check_erased(start, end) != E_NO_ERROR) {
        printf("Error verifying erased start: 0x%x end 0x%x\n", start, end);
        return -4;
    }

    flash_erase_test();

    return E_NO_ERROR;
}

//******************************************************************************
int test3(void)
{
    uint32_t start, end, value;
    // Test the buffered erase function

    start = TEST_START;
    end = TEST_END;
    value = 0;
    if(flash_fill(start, end, value) != E_NO_ERROR) {
        printf("Error filling flash\n");
        return -1;
    }
    if(FLC_BufferErase(start, end, TEST_BUFFER, MXC_FLASH_PAGE_SIZE) != E_NO_ERROR)  {
        printf("Error erasing flash\n");
        return -2;
    }
    if(flash_check_erased(start, end) != E_NO_ERROR) {
        printf("Error verifying erased start: 0x%x end 0x%x\n", start, end);
        return -3;
    }
    if(check_cannaries() != E_NO_ERROR) {
        printf("Error: erase buffer overflow\n");
        return -4;
    }
    flash_erase_test();

    start = TEST_START+0x123;
    end = TEST_END - 0x479;
    value = 0;
    if(flash_fill(TEST_START, TEST_END, value) != E_NO_ERROR) {
        printf("Error filling flash\n");
        return -1;
    }
    if(FLC_BufferErase(start, end, TEST_BUFFER, MXC_FLASH_PAGE_SIZE) != E_NO_ERROR)  {
        printf("Error erasing flash\n");
        return -2;
    }
    if(flash_check_erased(start, end) != E_NO_ERROR) {
        printf("Error verifying erased start: 0x%x end 0x%x\n", start, end);
        return -3;
    }
    if(check_cannaries() != E_NO_ERROR) {
        printf("Error: erase buffer overflow\n");
        return -4;
    }
    if(flash_verify(TEST_START, start, 0) != E_NO_ERROR) {
        printf("Error verifying first page\n");
        return -5;
    }
    if(flash_verify(end, TEST_END-1, end%0x100) != E_NO_ERROR) {
        printf("Error verifying last page\n");
        return -6;
    }
    flash_erase_test();

    start = TEST_START+0x123;
    end = TEST_START + MXC_FLASH_PAGE_SIZE - 0x479;
    value = 0;
    if(flash_fill(TEST_START, TEST_END, value) != E_NO_ERROR) {
        printf("Error filling flash\n");
        return -1;
    }
    if(FLC_BufferErase(start, end, TEST_BUFFER, MXC_FLASH_PAGE_SIZE) != E_NO_ERROR)  {
        printf("Error erasing flash\n");
        return -2;
    }
    if(flash_check_erased(start, end) != E_NO_ERROR) {
        printf("Error verifying erased start: 0x%x end 0x%x\n", start, end);
        return -3;
    }
    if(check_cannaries() != E_NO_ERROR) {
        printf("Error: erase buffer overflow\n");
        return -4;
    }
    if(flash_verify(TEST_START, start, 0) != E_NO_ERROR) {
        printf("Error verifying first page\n");
        return -5;
    }
    if(flash_verify(end, TEST_END-1, end%0x100) != E_NO_ERROR) {
        printf("Error verifying last page\n");
        return -6;
    }

    flash_erase_test();
    return E_NO_ERROR;
}

//******************************************************************************
int main(void)
{
    printf("***** FLC Erase test *****\n");

    if(flash_erase_test() != E_NO_ERROR) {
        printf("Error erasing test page");
        while(1) {}
    }

    // Write canaries in the erase buffer
    erase_buffer[0] = 0xAA;
    erase_buffer[1] = 0xAA;
    erase_buffer[2] = 0xAA;
    erase_buffer[3] = 0xAA;
    erase_buffer[4] = 0xAA;
    erase_buffer[5] = 0xAA;
    erase_buffer[6] = 0xAA;
    erase_buffer[7] = 0xAA;

    erase_buffer[MXC_FLASH_PAGE_SIZE+0] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+1] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+2] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+3] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+4] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+5] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+6] = 0xAA;
    erase_buffer[MXC_FLASH_PAGE_SIZE+7] = 0xAA;

#if 1
    if(test0() != E_NO_ERROR) {
        printf("Error with test0\n");
    }
#endif

#if 1
    if(test1() != E_NO_ERROR) {
        printf("Error with test1\n");
    }
#endif

#if 1
    if(test2() != E_NO_ERROR) {
        printf("Error with test2\n");
    }
#endif

#if 1
    if(test3() != E_NO_ERROR) {
        printf("Error with test3\n");
    }
#endif
    
    printf("Done testing\n");
    while(1);
    return 0;
}
