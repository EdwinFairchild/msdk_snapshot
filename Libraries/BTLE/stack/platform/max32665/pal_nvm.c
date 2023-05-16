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
 * $Date: 2019-07-31 17:25:58 -0500 (Wed, 31 Jul 2019) $ 
 * $Revision: 45172 $
 *
 ******************************************************************************/

#include <string.h>
#include "wsf_types.h"
#include "max32665.h"
#include "flc_regs.h"
#include "gcr_regs.h"
#include "pal_nvm.h"
#include "pal_crypto.h"

#ifdef __CC_ARM
#include "sct_config.h"

uint32_t __pal_nvm_db_start__ =  PAL_NVM_DB_START__;
uint32_t __pal_nvm_db_end__   =  PAL_NVM_DB_END__;
#endif

#ifdef __ICCARM__

/* Dummy holder for IAR Linker (NVRAM)    */
/* Everything below the allocated NVRAM   */
/* address is for NVRAM                   */

typedef struct
{
       unsigned int    nvram_dummy[2];
} nvram_tail_t;

#pragma location=".nvram_loc"
__root const nvram_tail_t dummy_holder = 
{
        .nvram_dummy[0] = 0xDEADC0DE, 
};
#endif


/**************************************************************************************************
  Macros
**************************************************************************************************/
#define FLASH_WORD_SIZE         16

/**************************************************************************************************
  Global Variables
**************************************************************************************************/

/******************************************************************************/
void PalNvmInit(PalNvmCback_t actCback)
{

}

/******************************************************************************/
void PalNvmDeInit(void)
{

}

/******************************************************************************/
PalNvmState_t PalNvmGetState(void)
{
#ifdef __GNUC__  
extern uint32_t __pal_nvm_db_end__, __pal_nvm_db_start__;	
   if (((uint32_t)&__pal_nvm_db_end__ - (uint32_t)&__pal_nvm_db_start__)!=0)
#elif __ICCARM__
extern uint32_t __ICFEDIT_region_NVM_end__, __ICFEDIT_region_NVM_start__;	
   if (((uint32_t)&__ICFEDIT_region_NVM_end__ - (uint32_t)&__ICFEDIT_region_NVM_start__)!=0)
#elif __CC_ARM
extern uint32_t __pal_nvm_db_end__, __pal_nvm_db_start__;	
   if (((uint32_t)__pal_nvm_db_end__ - (uint32_t)__pal_nvm_db_start__)!=0)
#endif
   {
	return PAL_NVM_STATE_READY;
   }
  else
  {
    return PAL_NVM_STATE_UNINIT;
  }
}

/******************************************************************************/
uint32_t PalNvmGetSectorSize(void)
{
    return MXC_FLASH_PAGE_SIZE;
}

/******************************************************************************/
uint32_t PalNvmGetTotalSize(void)
{
#ifdef __GNUC__ 
extern uint32_t __pal_nvm_db_end__, __pal_nvm_db_start__;
      return ((uint32_t)&__pal_nvm_db_end__ - (uint32_t)&__pal_nvm_db_start__);
#elif __ICCARM__
extern uint32_t __ICFEDIT_region_NVM_end__, __ICFEDIT_region_NVM_start__;	
      return ((uint32_t)&__ICFEDIT_region_NVM_end__ - (uint32_t)&__ICFEDIT_region_NVM_start__);
#elif __CC_ARM
extern uint32_t __pal_nvm_db_end__, __pal_nvm_db_start__;	
      return ((uint32_t)__pal_nvm_db_end__ - (uint32_t)__pal_nvm_db_start__);
#endif 	
	
}

/******************************************************************************/
void PalNvmRead(void *pBuf, uint32_t size, uint32_t srcAddr)
{
    uint32_t *src = (uint32_t*)srcAddr;
    memcpy(pBuf, src, size);
}

/******************************************************************************/
static void flush_cache(void)
{
    /* Flush all instruction caches */
    MXC_GCR->scon |= MXC_F_GCR_SCON_CCACHE_FLUSH;
    /* Wait for flush to complete */
    while(MXC_GCR->scon & MXC_F_GCR_SCON_CCACHE_FLUSH) {}
}

/******************************************************************************/
static mxc_flc_regs_t* get_flc(uint32_t addr)
{
    if (addr < MXC_FLASH1_MEM_BASE && addr >= MXC_FLASH0_MEM_BASE) {
        return MXC_FLC0;
    } else {
        return MXC_FLC1;
    }
}

/******************************************************************************/
static void flc_write128(uint32_t* data, uint32_t addr)
{
    mxc_flc_regs_t *flc = get_flc(addr);

    /* Unlock flash */
    flc->cn = (flc->cn & ~MXC_F_FLC_CN_UNLOCK) | MXC_S_FLC_CN_UNLOCK_UNLOCKED;

    // write the data
    flc->addr = addr;
    flc->data[0] = data[0];
    flc->data[1] = data[1];
    flc->data[2] = data[2];
    flc->data[3] = data[3];
    flc->cn |= MXC_F_FLC_CN_WR;

    /* Wait for operation to finish */
    while(flc->cn & (MXC_F_FLC_CN_WR | MXC_F_FLC_CN_ME | MXC_F_FLC_CN_PGE)) {}

    /* Lock flash */
    flc->cn &= ~MXC_F_FLC_CN_UNLOCK;
}

/******************************************************************************/
void PalNvmWrite(void *pBuf, uint32_t size, uint32_t dstAddr)
{
    uint8_t* pBuf8 = (uint8_t*)pBuf;
    // Assuming that size is 128 bit aligned
    while(size) {
        uint32_t data[4];
        uint32_t write_len;

        // Make sure we pad out the data;
        if(size < 16) {
            memset(data, 0xFF, 16);
            memcpy(data, pBuf8, size);
            write_len = size;
        } else {
            memcpy(data, pBuf8, 16);
            write_len = 16;
        }
	
        // Write the data
        flc_write128(data, dstAddr);

        // Increment the counters
        size -= write_len;
        dstAddr += FLASH_WORD_SIZE;
        pBuf8 += FLASH_WORD_SIZE;
    }

    flush_cache();
}

/******************************************************************************/
static void flc_erase_sector(uint32_t addr)
{
    mxc_flc_regs_t *flc = get_flc(addr);

    /* Unlock flash */
    flc->cn = (flc->cn & ~MXC_F_FLC_CN_UNLOCK) | MXC_S_FLC_CN_UNLOCK_UNLOCKED;

    /* Write page erase code */
    flc->cn = (flc->cn & ~MXC_F_FLC_CN_ERASE_CODE) | MXC_S_FLC_CN_ERASE_CODE_ERASEPAGE;

    /* Issue page erase command */
    flc->addr = addr;
    flc->cn |= MXC_F_FLC_CN_PGE;

    /* Wait for operation to finish */
    while(flc->cn & (MXC_F_FLC_CN_WR | MXC_F_FLC_CN_ME | MXC_F_FLC_CN_PGE)) {}
}

/******************************************************************************/
void PalNvmEraseSector(uint32_t size, uint32_t startAddr)
{
    // Assuming that size is page aligned
    while(size) {
        flc_erase_sector(startAddr);
        size -= MXC_FLASH_PAGE_SIZE;
        startAddr += MXC_FLASH_PAGE_SIZE;
    }

    flush_cache();
}

/******************************************************************************/
uint32_t PalCryptoCcmDec(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pCypherText, uint16_t textLen,
                         uint8_t *pClear, uint16_t clearLen, uint8_t *pMic, uint8_t micLen,
                         uint8_t *pResult, uint8_t handlerId, uint16_t param, uint8_t event)
{
  return 0;
}

/******************************************************************************/
void PalCryptoCcmEnc(const uint8_t *pKey, uint8_t *pNonce, uint8_t *pPlainText, uint16_t textLen,
              uint8_t *pClear, uint16_t clearLen, uint8_t micLen, uint8_t *pResult,
              uint8_t handlerId, uint16_t param, uint8_t event)
{
}

/******************************************************************************/
void PalCryptoInit(void)
{
}