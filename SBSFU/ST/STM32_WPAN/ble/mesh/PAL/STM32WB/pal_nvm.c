/**
******************************************************************************
* @file    appli_mesh.c
* @author  BLE Mesh Team
* @version V1.08.000
* @date    10-July-2018
* @brief   Flash management for the Controller
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*   1. Redistributions of source code must retain the above copyright notice,
*      this list of conditions and the following disclaimer.
*   2. Redistributions in binary form must reproduce the above copyright notice,
*      this list of conditions and the following disclaimer in the documentation
*      and/or other materials provided with the distribution.
*   3. Neither the name of STMicroelectronics nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Initial BlueNRG-Mesh is built over Motorola�s Mesh over Bluetooth Low Energy 
* (MoBLE) technology. The present solution is developed and maintained for both 
* Mesh library and Applications solely by STMicroelectronics.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "pal_nvm.h"
#include "pal_if.h"  
#include <string.h>

#include "ble.h"

extern const void* mobleNvmBase;
#define NVM_BASE ((unsigned int)mobleNvmBase)
#define NVM_SIZE 0x00002000
#define FLASH_SECTOR_SIZE 0x1000

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static MOBLEUINT32 GetPage(MOBLEUINT32 Addr)
{
  MOBLEUINT32 page = 0;

  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }

  return page;
}

/**
* @brief  returns NVM write protect status
* @param  None
* @retval TRUE if flash is write protected
*/
MOBLEBOOL MoblePalNvmIsWriteProtected(void)
{
  FLASH_OBProgramInitTypeDef ob;
  
  ob.WRPArea = 0;
  HAL_FLASHEx_OBGetConfig(&ob);
  
  const int sector = (NVM_BASE - FLASH_BASE) / FLASH_SECTOR_SIZE;
  const int sectorMask = 1 << (sector & 0x1f);
  
  return ob.WRPStartOffset & sectorMask ? MOBLE_TRUE : MOBLE_FALSE;
}

/**
* @brief  Read NVM
* @param  offset: offset wrt start address of nvm
* @param  buf: copy of read content
* @param  size: size of memory to be read
* @param  backup: If read from backup memory
* @retval Result of read operation
*/
MOBLE_RESULT MoblePalNvmRead(MOBLEUINT32 offset, void *buf, MOBLEUINT32 size, MOBLEBOOL backup)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmRead >>>\r\n");  

  if (offset > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    memmove(buf, (void *)(NVM_BASE + offset), size);
  }
  
//  printf("MoblePalNvmRead <<<\r\n");  
  return result;
}

/**
* @brief  Compare with NVM
* @param  offset: offset wrt start address of nvm
* @param  buf: copy of content
* @param  size: size of memory to be compared
* @param  comparison: outcome of comparison
* @retval Result
*/
MOBLE_RESULT MoblePalNvmCompare(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size, MOBLE_NVM_COMPARE* comparison)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmCompare >>>\r\n");
  
  if ((comparison == NULL) || (buf == NULL))
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (offset > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (offset & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    *comparison = MOBLE_NVM_COMPARE_EQUAL;
    size >>= 3;
    
    uint64_t* src = (uint64_t*)buf;
    uint64_t* dst = (uint64_t*)(NVM_BASE + offset);
    
    for (MOBLEUINT32 i=0; i<size; ++i)
    {
      if ((src[i<<3] != dst[i<<3]) && (*comparison == MOBLE_NVM_COMPARE_EQUAL))
      {
        *comparison = MOBLE_NVM_COMPARE_NOT_EQUAL;
      }
        if ((src[i<<3] & dst[i<<3]) != dst[i<<3])
        {
          *comparison = MOBLE_NVM_COMPARE_NOT_EQUAL_ERASE;
          break;
        }
    }
  }
  
//  printf("MoblePalNvmCompare <<<\r\n");
  
  return result;
}

/**
* @brief  Erase NVM
* @param  None
* @retval Result
*/
MOBLE_RESULT MoblePalNvmErase(MOBLEUINT32 offset)
{
  HAL_StatusTypeDef status = HAL_OK;

//  printf("MoblePalNvmErase >>>\r\n");
  
  while( LL_HSEM_1StepLock( HSEM, CFG_HW_FLASH_SEMID ) );
  HAL_FLASH_Unlock();
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);
  
  FLASH_EraseInitTypeDef erase;
  erase.TypeErase = FLASH_TYPEERASE_PAGES;
  erase.Page = GetPage(NVM_BASE + offset); /* 126 or 127 */;
  erase.NbPages = FLASH_SECTOR_SIZE >> 12;
  
  uint32_t pageError = 0;
  while(LL_FLASH_IsActiveFlag_OperationSuspended());
  status = HAL_FLASHEx_Erase(&erase, &pageError);
  while(LL_FLASH_IsActiveFlag_OperationSuspended());
  HAL_FLASH_Lock();
  LL_HSEM_ReleaseLock( HSEM, CFG_HW_FLASH_SEMID, 0 );
  
//  printf("MoblePalNvmErase <<<\r\n");
  
  return status == HAL_OK ? MOBLE_RESULT_SUCCESS : MOBLE_RESULT_FAIL;
}

/**
* @brief  Write to NVM
* @param  offset: offset wrt start address of nvm
* @param  buf: copy of write content
* @param  size: size of memory to be written
* @retval Result
*/
MOBLE_RESULT MoblePalNvmWrite(MOBLEUINT32 offset, void const *buf, MOBLEUINT32 size)
{
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  
//  printf("MoblePalNvmWrite >>>\r\n");
  
  if (offset > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size == 0)
  {
    result = MOBLE_RESULT_FALSE;
  }
  else if (offset + size > NVM_SIZE)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (offset & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else if (size & 3)
  {
    result = MOBLE_RESULT_INVALIDARG;
  }
  else
  {
    size >>= 3;
    
    uint64_t* src = (uint64_t*)buf;
    uint64_t* dst = (uint64_t*)(NVM_BASE + offset);
    
    HAL_StatusTypeDef status = HAL_OK;
    
    while( LL_HSEM_1StepLock( HSEM, CFG_HW_FLASH_SEMID ) );
    HAL_FLASH_Unlock();
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);
    for (size_t i = 0; i < size && status == HAL_OK; i++)
    {
      if (src[i<<3] != dst[i<<3])
      {
        while(LL_FLASH_IsActiveFlag_OperationSuspended());
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, NVM_BASE + offset + (i <<3), src[i]);
        if (status != HAL_OK)
        {
          break;
        }
      }
    }
    HAL_FLASH_Lock();
    LL_HSEM_ReleaseLock( HSEM, CFG_HW_FLASH_SEMID, 0 );
    
    if (HAL_OK != status)
    {
      result = MOBLE_RESULT_FAIL;
    }
  }
//  printf("MoblePalNvmWrite <<<\r\n");
  
  return result;
}

/**
* @brief  NVM process
* @param  None
* @retval Result
*/
MOBLE_RESULT MoblePalNvmProcess(void)
{
    /* do nothing */
    return MOBLE_RESULT_SUCCESS;
}

/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
