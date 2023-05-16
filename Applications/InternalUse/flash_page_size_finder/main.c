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
 * $Date: 2018-12-14 09:33:46 -0600 (Fri, 14 Dec 2018) $
 * $Revision: 39914 $
 *
 ******************************************************************************/

/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_config.h"
#include "mxc_errors.h"
#include "flc.h"

/***** Definitions *****/

/***** Globals *****/

/***** Functions *****/

// *****************************************************************************
int main(void)
{
    uint32_t flash_base = 0x10000000;
    uint32_t flash_range = 0x10000;
    uint32_t buffer[4];
    uint32_t *ptr;
    int i;
    volatile int page_address_result = -1;

    printf("Flash Page Size Finder\n");
    printf("Exec Addr: %08X\n",(uint32_t)main);
    printf("Flash Base Addr: %08X\n",flash_base);
    printf("Flash Test Size: %08X\n",flash_range);

    /* Erase whole flash. */
    if (FLC_MassErase() != E_NO_ERROR)
    {
        printf("Flash mass erase failed\n");
        while (1);
    }

    /* Write test data to flash from base address through flash test range. */
    memset(buffer,0x55,sizeof(buffer));
    for (i = 0;i < flash_range;i += 16)
    {
        /* Put flash address written into first 32-bits of buffer */
        buffer[0] = flash_base+i;
        if (FLC_Write128(flash_base+i,buffer) != E_NO_ERROR)
        {
            printf("Flash write failed\n");
            while (1);
        }
    }
    /* Erase first page. */
    if (FLC_PageErase(flash_base) != E_NO_ERROR)
    {
        printf("Flash page erase failed\n");
        while (1);
    }

    /* Find end of first page erase. */
    for (i = 0;i < flash_range;i += 16)
    {
        ptr = (uint32_t *)(flash_base+i);
        if (*ptr != 0xFFFFFFFF)
        {
            printf("Flash Page Size: %08X\n",i);
            page_address_result = flash_base+i;
            while(1);
        }
    }

    printf("Flash page size not found!\n");

    while(1);
}
