/*******************************************************************************
 * Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
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
 * $Date: 2019-12-09 10:40:53 -0600 (Mon, 09 Dec 2019) $
 * $Revision: 49656 $
 *
 ******************************************************************************/

#include <string.h>
#include "mxc_config.h"
#include "hash.h"
#include "flc.h"

/**************************************************************************************************
  Macros
**************************************************************************************************/
#define SHA256_BYTES            (256/8)
#define FLASH_UPPER_START       (MXC_FLASH1_MEM_BASE)
#define FLASH_LOWER_START       (MXC_FLASH0_MEM_BASE + MXC_FLASH_PAGE_SIZE)
#define FLASH_ERASED_WORD       0xFFFFFFFF

/**************************************************************************************************
  Local Variables
**************************************************************************************************/

/**************************************************************************************************
  Functions
**************************************************************************************************/

/* Defined in boot_lower.S */
extern void Boot_Lower(void);

void bootError(void)
{
    NVIC_SystemReset();
}

int flashPageErased(uint32_t* addr)
{
    /* Determine if this page is totally erased */
    int i;
    for(i = 0; i < (MXC_FLASH_PAGE_SIZE/4); i++) {
        if(*(addr+i) != FLASH_ERASED_WORD) {
            return 0;
        }
    }

    return 1;
}

uint32_t findUpperLen(void)
{
    uint32_t* flashPagePointer = (uint32_t*)FLASH_UPPER_START;

    /* Find the first erased page in the upper flash*/
    while(1) {

        if(*flashPagePointer == FLASH_ERASED_WORD) {
            /* Make sure the entire page is erased */
            if(flashPageErased(flashPagePointer)) {
                break;
            }
        }

        flashPagePointer += (MXC_FLASH_PAGE_SIZE/4);
    }

    /* Length is 0 */
    if(flashPagePointer == (uint32_t*)FLASH_UPPER_START) {
        return 0;
    }

    /* search backwards for the first bytes that isn't erased */
    while(*(flashPagePointer--) == FLASH_ERASED_WORD) {}
    flashPagePointer+= 2;

    /* return the starting address of the SHA digest, last address of the image */
    return (uint32_t)(flashPagePointer - (SHA256_BYTES/4) - (FLASH_UPPER_START/4));
}


/* Program the digests of the image, used to test addressing */
void programDigest(uint8_t *addr, uint32_t len)
{
    char digest[SHA256_BYTES];

    /* Calculate the digest */
    TPU_Hash_Reset();
    TPU_Hash_Config(TPU_HASH_SHA256);
    TPU_SHA((const char*)addr, TPU_HASH_SHA256, len, digest);
    TPU_Hash_Shutdown();

    /* Write the digest at the end of the image */
    FLC_Write((uint32_t)(addr+len), SHA256_BYTES, (uint32_t*)digest);
}

int main(void)
{

    char digest[SHA256_BYTES];
    const char *addr = (const char*)FLASH_UPPER_START;

    /* disable interrupts to prevent these operations from being interrupted */
    __disable_irq();

    /* Get the length of the image in the upper flash array */
    uint32_t len = findUpperLen();

    /* Attempt to verify the upper image if we get a valid length */
    if(len) {

        /* Used for testing, remove for actual application */
        /* len += SHA256_BYTES; */
        /* programDigest((uint8_t *)addr, len); */

        /* Calculate the digest */
        TPU_Hash_Reset();
        TPU_Hash_Config(TPU_HASH_SHA256);
        TPU_SHA(addr, TPU_HASH_SHA256, len, digest);
        TPU_Hash_Shutdown();

        /* Check the calculated digest against what was received */
        if(memcmp(digest, (const char *)(addr + len), SHA256_BYTES) == 0) {

            /* Erase the destination pages */
            if(FLC_MultiPageErase(FLASH_LOWER_START, (FLASH_LOWER_START + len)) != E_NO_ERROR) {
                /* Failed to erase pages */
                bootError();
            }

            /* Copy the new firmware image */
            if(FLC_Write(FLASH_LOWER_START, len, (uint32_t*)FLASH_UPPER_START) != E_NO_ERROR) {
                /* Failed to write new image */
                bootError();
            }

            /* Erase the upper pages */
            if(FLC_MultiPageErase(FLASH_UPPER_START, (FLASH_UPPER_START+ len)) != E_NO_ERROR) {
                /* Failed to erase update pages, we can still boot from the lower image */
            }
        }
    }

    /* Boot from lower image */
    Boot_Lower();

    while (1) {}
}
