/* Copyright (c) 2009-2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 *  \brief NVM service.
 */
/*************************************************************************************************/

#include "wsf_types.h"
#include "wsf_assert.h"
#include "wsf_nvm.h"
#include "pal_nvm.h"
#include "util/crc32.h"

#ifdef __CC_ARM
#include "sct_config.h"
#endif

#ifdef __CC_ARM
//uint32_t __pal_nvm_db_start__ =  PAL_NVM_DB_START__;
#endif

/**************************************************************************************************
  Macros
**************************************************************************************************/

/*! NVM data start address. */

#ifdef __GNUC__
extern uint32_t __pal_nvm_db_start__;
#define WSF_NVM_START_ADDR                       (uint32_t)&__pal_nvm_db_start__ 
#endif 

#ifdef __ICCARM__
extern uint32_t __ICFEDIT_region_NVM_start__;
#define WSF_NVM_START_ADDR                       (uint32_t)&__ICFEDIT_region_NVM_start__ 
#endif 

#ifdef __CC_ARM
extern uint32_t __pal_nvm_db_start__;
#define WSF_NVM_START_ADDR                       (uint32_t)__pal_nvm_db_start__ 
#endif

/*! Reserved filecode. */
#define WSF_NVM_RESERVED_FILECODE                 ((uint64_t)0)

/* Unused (erased) filecode. */
/* TODO: May depend on flash type */
#define WSF_NVM_UNUSED_FILECODE                   ((uint64_t)0xFFFFFFFFFFFFFFFF)

/*! Flash word size. */
#define WSF_FLASH_WORD_SIZE                       16

/*! Align value to word boundary. */
#define WSF_NVM_WORD_ALIGN(x)                     (((x) + (WSF_FLASH_WORD_SIZE - 1)) & \
                                                         ~(WSF_FLASH_WORD_SIZE - 1))

#define WSF_NVM_CRC_INIT_VALUE                    0xFEDCBA98

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Header. */
typedef struct
{
  uint64_t          id;         /*!< Stored data ID. */
  uint32_t          len;        /*!< Stored data length. */
  uint32_t          headerCrc;  /*!< CRC of this header. */
  uint32_t          dataCrc;    /*!< CRC of subsequent data. */
  uint32_t          pad[3];     /*!< Padding for 128-bit align. */
} WsfNvmHeader_t;

static struct
{
  uint32_t          availAddr;  /*!< Next available address for NVM write. */
  uint32_t          sectorSize; /*!< Size of erase sector. */
  uint32_t          totalSize;  /*!< Total size of NVM storage. */
} wsfNvmCb;

/**************************************************************************************************
  Global Functions
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief  Initialize the WSF NVM.
 *
 *  \return None.
 */
/*************************************************************************************************/
void WsfNvmInit(void)
{
  PalNvmInit(NULL);
  wsfNvmCb.totalSize = PalNvmGetTotalSize();
  wsfNvmCb.sectorSize = PalNvmGetSectorSize();

  WsfNvmHeader_t header;
  uint32_t storageAddr = WSF_NVM_START_ADDR;

  PalNvmRead(&header, sizeof(header), storageAddr);

  while(header.id != WSF_NVM_UNUSED_FILECODE)
  {
    WSF_ASSERT(storageAddr < wsfNvmCb.totalSize);

    /* Move to next stored data block and read header. */
    storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
    PalNvmRead(&header, sizeof(header), storageAddr);
  }

  wsfNvmCb.availAddr = storageAddr;
}

/*************************************************************************************************/
/*!
 *  \brief  Read data.
 *
 *  \param  id         Stored data ID.
 *  \param  pData      Buffer to read to.
 *  \param  len        Data length to read.
 *  \param  compCback  Read callback.
 *
 *  \return TRUE if NVM operation is successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfNvmReadData(uint64_t id, uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback)
{
  WsfNvmHeader_t header;
  uint32_t headerCrc, dataCrc;
  uint32_t storageAddr = WSF_NVM_START_ADDR;
  bool_t findId = FALSE;

  WSF_ASSERT(!((id == WSF_NVM_RESERVED_FILECODE) || (id == WSF_NVM_UNUSED_FILECODE)));

  /* Make sure that storage is present. */
  if (storageAddr != 0x00)
   {

    /* Read first header. */
    PalNvmRead(&header, sizeof(header), storageAddr);

    do
    {
      if (header.id == WSF_NVM_UNUSED_FILECODE)
      {
        /* Found unused entry at end of used storage. */
        break;
      }

      /* Iterate through stored data headers, looking for existing matching stored data header. */
      if (header.id != WSF_NVM_RESERVED_FILECODE)
      {
        /* Calculate CRC of header itself. */
        headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                (uint8_t *)&header);

        if (headerCrc != header.headerCrc)
        {
          /* Corrupt header. */
          /* TODO: Catastrophic failure? */
          break;
        }
        else if ((header.id == id) && (header.len == len))
        {
          /* Valid header and matching ID - read data after header. */
          storageAddr += sizeof(header);
          PalNvmRead(pData, header.len, storageAddr);
          dataCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, header.len, pData);
          if (dataCrc == header.dataCrc)
          {
            findId = TRUE;
          }
          break;
        }
      }

      /* Move to next stored data block and read header. */
      storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
      PalNvmRead(&header, sizeof(header), storageAddr);
    } while(1);

    if (compCback)
    {
      compCback(findId);
    }
  }
  
  return findId;
}

/*************************************************************************************************/
/*!
 *  \brief  Write data.
 *
 *  \param  id         Stored data ID.
 *  \param  pData      Buffer to write.
 *  \param  len        Data length to write.
 *  \param  compCback  Write callback.
 *
 *  \return TRUE if NVM operation is successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfNvmWriteData(uint64_t id, const uint8_t *pData, uint16_t len, WsfNvmCompEvent_t compCback)
{
  WsfNvmHeader_t header;
  uint32_t headerCrc, dataCrc;
  uint32_t storageAddr = WSF_NVM_START_ADDR;

  WSF_ASSERT(!((id == WSF_NVM_RESERVED_FILECODE) || (id == WSF_NVM_UNUSED_FILECODE)));
  WSF_ASSERT(wsfNvmCb.availAddr <= wsfNvmCb.totalSize);

  /* Read first header. */
  PalNvmRead(&header, sizeof(header), storageAddr);

  do
  {
    if (header.id == WSF_NVM_UNUSED_FILECODE)
    {
      /* Found unused entry at end of used storage. */
      break;
    }

    /* Iterate through stored data headers, looking for existing matching stored data header. */
    if (header.id != WSF_NVM_RESERVED_FILECODE)
    {
      /* Calculate CRC of header itself. */
      headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                            (uint8_t *)&header);

      if (headerCrc != header.headerCrc)
      {
        /* Corrupt header. */
        /* TODO: Catastrophic failure? */
        break;
      }
      else if (header.id == id)
      {
        dataCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, len, pData);
        if (dataCrc == header.dataCrc)
        {
          if (compCback)
          {
            compCback(TRUE);
          }
          return TRUE;
        }
        else
        {
          /* Valid header and matching ID - scratch header out. */
          header.id = WSF_NVM_RESERVED_FILECODE;
          header.headerCrc = 0;
          header.dataCrc = 0;
          PalNvmWrite(&header, sizeof(header), storageAddr);
        }
      }
    }

    /* Move to next stored data block and read header. */
    storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
    PalNvmRead(&header, sizeof(header), storageAddr);
  } while(1);

  /* After cycling through all headers, create a new stored data header and store data */
  header.id = id;
  header.len = len;
  header.headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                               (uint8_t *)&header);
  header.dataCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, len, pData);

  PalNvmWrite(&header, sizeof(header), storageAddr);
  PalNvmWrite((void *)pData, len, storageAddr + sizeof(header));

  /* Move to next empty flash. */
  storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
  wsfNvmCb.availAddr = storageAddr;

  if (compCback)
  {
    compCback(wsfNvmCb.availAddr <= wsfNvmCb.totalSize);
  }

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief  Erase data.
 *
 *  \param  id         Erase ID.
 *  \param  compCback  Write callback.
 *
 *  \return TRUE if NVM operation is successful, FALSE otherwise.
 */
/*************************************************************************************************/
bool_t WsfNvmEraseData(uint64_t id, WsfNvmCompEvent_t compCback)
{
  WsfNvmHeader_t header;
  uint32_t headerCrc;
  uint32_t storageAddr = WSF_NVM_START_ADDR;
  bool_t erased = FALSE;

  WSF_ASSERT(!((id == WSF_NVM_RESERVED_FILECODE) || (id == WSF_NVM_UNUSED_FILECODE)));

  /* Read first header. */
  PalNvmRead(&header, sizeof(header), storageAddr);

  do
  {
    if (header.id == WSF_NVM_UNUSED_FILECODE)
    {
      /* Found unused entry at end of used storage. */
      break;
    }

    /* Iterate through stored data headers, looking for existing matching stored data header. */
    if (header.id != WSF_NVM_RESERVED_FILECODE)
    {
      headerCrc = CalcCrc32(WSF_NVM_CRC_INIT_VALUE, sizeof(header.id) + sizeof(header.len),
                            (uint8_t *)&header);

      if (headerCrc != header.headerCrc)
      {
        /* Corrupt header. */
        /* TODO: Catastrophic failure? */
        break;
      }
      else if (header.id == id)
      {
        header.id = WSF_NVM_RESERVED_FILECODE;
        header.headerCrc = 0;
        header.dataCrc = 0;
        PalNvmWrite(&header, sizeof(header), storageAddr);
        erased = TRUE;
      }
    }

    /* Move to next stored data block and read header. */
    storageAddr += WSF_NVM_WORD_ALIGN(header.len) + sizeof(header);
    PalNvmRead(&header, sizeof(header), storageAddr);
  } while(1);

  if (compCback)
  {
    compCback(erased);
  }
  return erased;
}

/*************************************************************************************************/
/*!
 *  \brief  Erase all data located in NVM storage.
 *
 *  \param  compCback          Erase callback.
 *
 *  \return None.
 *
 *  \note   Security Risk Warning. NVM storage could be shared by multiple Apps.
 */
/*************************************************************************************************/
void WsfNvmEraseDataAll(WsfNvmCompEvent_t compCback)
{
  for (uint32_t eraseAddr = WSF_NVM_START_ADDR; eraseAddr < wsfNvmCb.availAddr; eraseAddr += wsfNvmCb.sectorSize)
  {
    PalNvmEraseSector(1, eraseAddr);
  }
  wsfNvmCb.availAddr = WSF_NVM_START_ADDR;

  if (compCback)
  {
    compCback(TRUE);
  }
}
