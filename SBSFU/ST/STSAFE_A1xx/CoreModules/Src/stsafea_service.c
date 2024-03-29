/**
  ******************************************************************************
  * @file    stsafea_service.c
  * @author  SMD/AME application teams
  * @version V3.2.0
  * @brief   STSAFE-A Middleware Service module.
  *          Provide Low Level services such as communication bus, IOs etc such as:
  *           + Initialization and Configuration functions
  *           + Low Level operation functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * STSAFE DRIVER SOFTWARE LICENSE AGREEMENT (SLA0088)
  *
  * BY INSTALLING, COPYING, DOWNLOADING, ACCESSING OR OTHERWISE USING THIS SOFTWARE
  * OR ANY PART THEREOF (AND THE RELATED DOCUMENTATION) FROM STMICROELECTRONICS
  * INTERNATIONAL N.V, SWISS BRANCH AND/OR ITS AFFILIATED COMPANIES (STMICROELECTRONICS),
  * THE RECIPIENT, ON BEHALF OF HIMSELF OR HERSELF, OR ON BEHALF OF ANY ENTITY BY WHICH
  * SUCH RECIPIENT IS EMPLOYED AND/OR ENGAGED AGREES TO BE BOUND BY THIS SOFTWARE LICENSE
  * AGREEMENT.
  *
  * Under STMicroelectronics� intellectual property rights, the redistribution,
  * reproduction and use in source and binary forms of the software or any part thereof,
  * with or without modification, are permitted provided that the following conditions
  * are met:
  * 1.  Redistribution of source code (modified or not) must retain any copyright notice,
  *     this list of conditions and the disclaimer set forth below as items 10 and 11.
  * 2.  Redistributions in binary form, except as embedded into a microcontroller or
  *     microprocessor device or a software update for such device, must reproduce any
  *     copyright notice provided with the binary code, this list of conditions, and the
  *     disclaimer set forth below as items 10 and 11, in documentation and/or other
  *     materials provided with the distribution.
  * 3.  Neither the name of STMicroelectronics nor the names of other contributors to this
  *     software may be used to endorse or promote products derived from this software or
  *     part thereof without specific written permission.
  * 4.  This software or any part thereof, including modifications and/or derivative works
  *     of this software, must be used and execute solely and exclusively in combination
  *     with a secure microcontroller device from STSAFE family manufactured by or for
  *     STMicroelectronics.
  * 5.  No use, reproduction or redistribution of this software partially or totally may be
  *     done in any manner that would subject this software to any Open Source Terms.
  *     �Open Source Terms� shall mean any open source license which requires as part of
  *     distribution of software that the source code of such software is distributed
  *     therewith or otherwise made available, or open source license that substantially
  *     complies with the Open Source definition specified at www.opensource.org and any
  *     other comparable open source license such as for example GNU General Public
  *     License(GPL), Eclipse Public License (EPL), Apache Software License, BSD license
  *     or MIT license.
  * 6.  STMicroelectronics has no obligation to provide any maintenance, support or
  *     updates for the software.
  * 7.  The software is and will remain the exclusive property of STMicroelectronics and
  *     its licensors. The recipient will not take any action that jeopardizes
  *     STMicroelectronics and its licensors' proprietary rights or acquire any rights
  *     in the software, except the limited rights specified hereunder.
  * 8.  The recipient shall comply with all applicable laws and regulations affecting the
  *     use of the software or any part thereof including any applicable export control
  *     law or regulation.
  * 9.  Redistribution and use of this software or any part thereof other than as  permitted
  *     under this license is void and will automatically terminate your rights under this
  *     license.
  * 10. THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" AND ANY
  *     EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  *     WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
  *     OF THIRD PARTY INTELLECTUAL PROPERTY RIGHTS, WHICH ARE DISCLAIMED TO THE FULLEST
  *     EXTENT PERMITTED BY LAW. IN NO EVENT SHALL STMICROELECTRONICS OR CONTRIBUTORS BE
  *     LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  *     DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  *     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  *     THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  *     NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  *     ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * 11. EXCEPT AS EXPRESSLY PERMITTED HEREUNDER, NO LICENSE OR OTHER RIGHTS, WHETHER EXPRESS
  *     OR IMPLIED, ARE GRANTED UNDER ANY PATENT OR OTHER INTELLECTUAL PROPERTY RIGHTS OF
  *     STMICROELECTRONICS OR ANY THIRD PARTY.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stsafea_service.h"
#include <string.h>

/** MISRA C:2012 deviation rule has been granted for following rules:
  * - Rule-14.3_a - Medium: Conditional expression is always true.
  * - Rule-14.3_b - Medium: Conditional expression is always false.
  */


/** @addtogroup STSAFE_A1XX_CORE_MODULES
  * @{
  */

/** @addtogroup SERVICE
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @addtogroup SERVICE_Private_Constants
  * @{
  */
#define STSAFEA_I2C_POLLING_SEND      ((uint8_t)3)  /*!< Polling duration as count value for send i2c operation */
#define STSAFEA_I2C_POLLING_RECEIVE   ((uint8_t)11) /*!< Polling duration as count value for receive i2c operation */

/**
  * @brief Table for the CRC16 X.25
  *      - Polynomial           : 0x1021
  *      - Degree of polynomial	: 16
  *      - Generator polynomial	: G(x) = x^16 + x^12 + x^5 + 1
  *      - Input order          : Reflected
  *      - Result order         : Reflected
  *      - Initial value        : 0xFFFF
  *      - Final XOR mask       : 0xFFFF
  */
#if (STSAFEA_USE_OPTIMIZATION_CRC_TABLE)
#define STSAFEA_CRC16_X25_REFLECTED_LOOKUP_TABLE \
  0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7, \
  0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876, \
  0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5, \
  0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974, \
  0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB, 0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3, \
  0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A, 0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72, \
  0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1, \
  0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738, 0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70, \
  0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF, \
  0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E, \
  0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD, \
  0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C, \
  0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB, \
  0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232, 0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A, \
  0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1, 0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9, \
  0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
#endif /* STSAFEA_USE_OPTIMIZATION_CRC_TABLE */

/**
  * @}
  */


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/** @addtogroup SERVICE_Private_Variables
  * @{
  */
static STSAFEA_HW_t HwCtx;
/**
  * @}
  */


/* Global variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static uint16_t StSafeA_Crc16_ccitt(uint8_t Header, uint8_t *pData, uint16_t Length);
static int8_t   StSafeA_SendBytes(StSafeA_TLVBuffer_t *pInBuffer);
static int8_t   StSafeA_ReceiveBytes(StSafeA_TLVBuffer_t *pOutBuffer);
int32_t CRC16X25_Init(void);
uint32_t CRC_Compute(uint8_t *pData1, uint16_t Length1, uint8_t *pData2, uint16_t Length2);
static void StSafeA_Crc16(StSafeA_TLVBuffer_t *pTLV_Buffer);

/* Functions Definition ------------------------------------------------------*/
/** @addtogroup SERVICE_Exported_Functions
  * @{
  */

/** @addtogroup SERVICE_Exported_Functions_Group1 Initialization / Configuration APIs
  *  @brief    STSAFE-A Service APIs related to to initialization and configuration
  *
@verbatim
 ===============================================================================
              ##### Initialization/Configuration functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  StSafeA_HW_Init
  *         Initialize the communication bus and the IO pins needed to operate the STSAFE-A1xx device
  * @note   No other Service APIs can be called/used if this one has never been successfully called first.
  *         The __weak StSafeA_HW_Probe function must have been implemented at interface layer.
  *
  * @param  None
  * @retval 0 if success, an error code otherwise.
  */
int8_t StSafeA_HW_Init(void)
{
  int8_t ret = StSafeA_HW_Probe(&HwCtx);

  if (STSAFEA_HW_OK == ret)
  {
    /* Initialize the STSAFE-A  IO */
    ret = HwCtx.IOInit();
  }

  if (STSAFEA_HW_OK == ret)
  {
    /* Initialize the STSAFE-A communication Bus */
    ret = HwCtx.BusInit();
  }

  if (STSAFEA_HW_OK == ret)
  {
    /* Initialize the STSAFE-A CRC */
    ret = HwCtx.CrcInit();
  }

  return ret;
}

/**
  * @}
  */


/** @addtogroup SERVICE_Exported_Functions_Group2 Low Level operation APIs
  *  @brief    STSAFE-A Service APIs related to low level operations
  *
@verbatim
 ===============================================================================
              ##### Low Level operation functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief   StSafeA_Transmit
  *          Prepare the command to be transmitted and call the low level bus to execute.
  *          Compute and concatenate CRC if supported.
  *
  * @param   pTLV_Buffer : Tag-Length-Value structure pointer to be transmitted over the bus.
  * @param   CrcSupport  : 0 if CRC is not supported, any other values otherwise.
  * @retval  STSAFEA_OK if success,  an error code otherwise.
  */
StSafeA_ResponseCode_t StSafeA_Transmit(StSafeA_TLVBuffer_t *pTLV_Buffer,  uint8_t CrcSupport)
{
  StSafeA_ResponseCode_t status_code = STSAFEA_INVALID_PARAMETER;
  if (pTLV_Buffer != NULL)
  {
    /* Compute & concatenate CRC to buffer */
    if (CrcSupport != 0U)
    {
      StSafeA_Crc16(pTLV_Buffer);
    }

    status_code = (StSafeA_SendBytes(pTLV_Buffer) == STSAFEA_HW_OK) ?
                  STSAFEA_OK : STSAFEA_COMMUNICATION_ERROR;
  }

  return status_code;
}

/**
  * @brief   StSafeA_Receive
  *          Receive data from STSAFE-A1xx  using the low level bus functions to retrieve it.
  *          Check the CRC, if supported.
  *
  * @param   pTLV_Buffer : Tag-Length-Value structure pointer to be filled  with received data
  * @param   CrcSupport  : 0 if CRC is not supported, any other values otherwise.
  * @retval  STSAFEA_OK if success,  an error code otherwise.
  */
StSafeA_ResponseCode_t StSafeA_Receive(StSafeA_TLVBuffer_t *pTLV_Buffer,  uint8_t CrcSupport)
{
  StSafeA_ResponseCode_t status_code = STSAFEA_INVALID_PARAMETER;

  if (pTLV_Buffer != NULL)
  {
    /* Increase buffer size in case of CRC */
    if (CrcSupport != 0U)
    {
      pTLV_Buffer->LV.Length += STSAFEA_CRC_LENGTH;
    }

    status_code = (StSafeA_ReceiveBytes(pTLV_Buffer) == STSAFEA_HW_OK) ?
                  (StSafeA_ResponseCode_t)(uint8_t)((uint8_t)pTLV_Buffer->Header & ~(uint8_t)STSAFEA_CMD_HEADER_RMACEN) :
                  STSAFEA_COMMUNICATION_ERROR;

    /* Check CRC */
    if ((CrcSupport != 0U) && (status_code == STSAFEA_OK))
    {
      uint16_t crc;
      pTLV_Buffer->LV.Length -= STSAFEA_CRC_LENGTH;
      (void)memcpy(&crc, &pTLV_Buffer->LV.Data[pTLV_Buffer->LV.Length], sizeof(crc));
      StSafeA_Crc16(pTLV_Buffer);

      if (memcmp(&crc, &pTLV_Buffer->LV.Data[pTLV_Buffer->LV.Length - (uint16_t)2],
                 sizeof(crc)) != 0)
      {
        status_code = STSAFEA_INVALID_CRC;
      }
      else
      {
        pTLV_Buffer->LV.Length -= STSAFEA_CRC_LENGTH;
      }
    }
  }

  return status_code;
}

/**
  * @brief   StSafeA_Delay
  *          Provide a delay in milliseconds.
  *
  * @param   msDelay : desired delay in milliseconds.
  * @retval  None
  */
void StSafeA_Delay(uint32_t msDelay)
{
  /* Redirect to the TimeDelay function that has been assigned into the HW Context */
  if (HwCtx.TimeDelay != NULL)
  {
    HwCtx.TimeDelay(msDelay);
  }
}
/**
  * @}
  */

/**
  * @}
  */


/* Interface weak functions definitions --------------------------------------*/

/** @addtogroup SERVICE_Interface_Functions
  *  @brief    These functions MUST be implemented at application layer
  * @{
  */

/**
  * @brief   StSafeA_HW_Probe
  *          Initialize the STSAFE-A1xx low level STSAFEA_HW_t context structure according to the platform (MCU, BSP, etc).
  * @note    This is a weak function that MUST be implemented at application interface level.
  *          A specific example template stsafea_service_interface_template.c is provided with this Middleware.
  *
  * @param   pCtx : generic void pointer, pointing to the STSAFEA_HW_t context structure to be initialized.
  * @retval  0 if success, an error code otherwise.
  */
__weak int8_t StSafeA_HW_Probe(void  *pCtx)
{
  STSAFEA_UNUSED_PTR(pCtx);
  return STSAFEA_HW_ERR;
}

/**
  * @}
  */


/* Private functions definitions ---------------------------------------------*/
/** @addtogroup SERVICE_Private_Functions
  * @{
  */

/**
  * @brief   StSafeA_SendBytes
  *          Send data to the STSAFE_A1xx device over the configured Bus .
  *
  * @param   pInBuffer : TLV Structure pointer containing the data to be transmitted
  * @retval  0 if success, an error code otherwise
  */
static int8_t StSafeA_SendBytes(StSafeA_TLVBuffer_t *pInBuffer)
{
  int8_t status_code = STSAFEA_HW_ERR;
  uint8_t loop = 1;

  uint16_t i2c_length = pInBuffer->LV.Length + STSAFEA_HEADER_LENGTH;

  /* In order to avoid excess data sending over I2C */
  /* pInBuffer->LV.Length should not exceed the max allowed size */
  if ((pInBuffer->LV.Data != NULL) && (i2c_length <=  STSAFEA_BUFFER_DATA_PACKET_SIZE))
  {

    /* To optimize stack size and avoid to allocate memory for a dedicated transmit
       buffer, the same pInBuffer.Data is used to send over the Bus. The
       pInBuffer.Data has to be adjusted in the proper way to be used for this scope,
       in order to include the Header as well. LV.Data must be 1-byte greater than expected.
       LV.Data is in fact allocated with 4 additional spare bytes (see STSAFEA_BUFFER_DATA_PACKET_SIZE) */
    (void)memmove((uint8_t *)&pInBuffer->LV.Data[1], (uint8_t *)&pInBuffer->LV.Data[0], pInBuffer->LV.Length);
    pInBuffer->LV.Data[0] = pInBuffer->Header;


    /* Send to STSAFE-A1xx */
    while ((status_code != 0) && (loop <= STSAFEA_I2C_POLLING_SEND))
    {
      status_code = HwCtx.BusSend(((uint16_t)HwCtx.DevAddr) << 1,
                                   (uint8_t *)pInBuffer->LV.Data, i2c_length);

      if (status_code != STSAFEA_HW_OK)
      {
        (void)HwCtx.BusDeInit();
        (void)HwCtx.BusInit();

        HwCtx.TimeDelay(2U * (uint32_t)loop);
      }

      loop++;
    }

    /* Restore initial LV.Data, in case caller expects to re-use it */
    /* At the moment of this implementation Callers functions never need the transmitted LV.Data
       so the following restore can be commented for speed optimization */
    (void)memmove((uint8_t *)&pInBuffer->LV.Data[0], (uint8_t *)&pInBuffer->LV.Data[1], pInBuffer->LV.Length);
  }
  return (status_code);
}

/**
  * @brief   StSafeA_ReceiveBytes
  *          Receive data from the STSAFE_A1xx device over the configured Bus .
  *
  * @param   pOutBuffer : TLV Structure pointer to be filled with the received data
  * @retval  0 if success, an error code otherwise
  */
static int8_t StSafeA_ReceiveBytes(StSafeA_TLVBuffer_t *pOutBuffer)
{
  uint16_t response_length = pOutBuffer->LV.Length;
  int8_t status_code = STSAFEA_HW_ERR;
  uint16_t loop = 1;

  /* In order to avoid excess data sending over I2C */
  /* pInBuffer->LV.Length should not exceed the max allowed size */
  if ((pOutBuffer->LV.Data != NULL) &&
      ((response_length + STSAFEA_HEADER_LENGTH + STSAFEA_BUFFER_LENGTH_SIZE) <=  STSAFEA_BUFFER_DATA_PACKET_SIZE))
  {
    /* To optimize stack size and avoid to allocate memory for a dedicated receive
       buffer, the pOutBuffer.Data is used to receive over the Bus. Than the
       pOutBuffer structure is re-adjusted in the proper way */
    while ((status_code != 0) && (loop <= STSAFEA_I2C_POLLING_RECEIVE))
    {

      status_code = HwCtx.BusRecv(((uint16_t)HwCtx.DevAddr) << 1,
                                   pOutBuffer->LV.Data,
                                   response_length + STSAFEA_HEADER_LENGTH + STSAFEA_BUFFER_LENGTH_SIZE);

      /* At this point the pOutBuffer.Header, Length, Data is re-adjusted in the proper way*/
      pOutBuffer->Header = pOutBuffer->LV.Data[0];
      pOutBuffer->LV.Length = ((uint16_t)pOutBuffer->LV.Data[1] << 8) + pOutBuffer->LV.Data[2];
      (void)memcpy(pOutBuffer->LV.Data, &pOutBuffer->LV.Data[3], response_length);


      if (status_code != STSAFEA_HW_OK)
      {
        (void)HwCtx.BusDeInit();
        (void)HwCtx.BusInit();

        HwCtx.TimeDelay(2U * (uint32_t)loop);
      }
      loop++;
    }

    /* If STSAFE returns a length higher than expected, a new read with the
       updated bytes length is executed */
    if ((pOutBuffer->LV.Length > response_length) && (status_code == 0))
    {
      status_code = STSAFEA_HW_ERR;
      loop = 1;

      while ((status_code != STSAFEA_HW_OK) && (loop <= STSAFEA_I2C_POLLING_RECEIVE))
      {
        status_code = HwCtx.BusRecv(((uint16_t)HwCtx.DevAddr) << 1,
                                     pOutBuffer->LV.Data,
                                     pOutBuffer->LV.Length + STSAFEA_HEADER_LENGTH + STSAFEA_BUFFER_LENGTH_SIZE);

        pOutBuffer->Header = pOutBuffer->LV.Data[0];
        pOutBuffer->LV.Length = ((uint16_t)pOutBuffer->LV.Data[1] << 8) + pOutBuffer->LV.Data[2];
        (void)memcpy(pOutBuffer->LV.Data, &pOutBuffer->LV.Data[3], pOutBuffer->LV.Length);


        if (status_code != STSAFEA_HW_OK)
        {
          (void)HwCtx.BusDeInit();
          (void)HwCtx.BusInit();

          HwCtx.TimeDelay(2U * (uint32_t)loop);
        }

        loop++;
      }
    }
  }
  return (status_code);
}

/**
  * @brief   CRC16X25_Init
  *          Initializes CRC X25.
  * @retval  BSP status
  */
__weak int32_t CRC16X25_Init(void)
{
  return STSAFEA_HW_OK;
}

/**
  * @brief   CRC_Compute
  *          Computes the CRC using software solution.
  *          CRC is computed firsly using 1st data starting with initialization value.
  *          CRC is computed secondly using 2nd data starting with the previously computed CRC.
  *
  * @param   pData1  : Pointer to 1st input data buffer.
  * @param   Length1 : Size of 1st input data buffer.
  * @param   pData2  : Pointer to 2nd input data buffer.
  * @param   Length2 : Size of 2nd input data buffer.
  * @retval  uint32_t CRC (returned value LSBs for CRC)
  */
__weak uint32_t CRC_Compute(uint8_t *pData1, uint16_t Length1, uint8_t *pData2, uint16_t Length2)
{
  (void)Length1;
  uint16_t crc16 = 0;
  if ((pData1 != NULL) && (pData2 != NULL))
  {
    crc16 = StSafeA_Crc16_ccitt(pData1[0], pData2, Length2);

    crc16 = (uint16_t)SWAP2BYTES(crc16);
    crc16 ^= 0xFFFFU;
  }
  return (uint32_t)crc16;
}

/**
  * @brief   StSafeA_Crc16
  *          Computes the CRC16 over the Data passed into the TLV structure.
  *
  * @param   pTLV_Buffer : TLV Structure pointer containing the data to calculate the CRC on.
  * @retval  None
  */
void StSafeA_Crc16(StSafeA_TLVBuffer_t *pTLV_Buffer)
{
  if ((pTLV_Buffer != NULL))
  {
    uint16_t crc16;

    crc16 = (uint16_t)HwCtx.CrcCompute(&(pTLV_Buffer->Header),
                                       1U,
                                       pTLV_Buffer->LV.Data,
                                       pTLV_Buffer->LV.Length);

    (void)memcpy(&pTLV_Buffer->LV.Data[pTLV_Buffer->LV.Length], &crc16, sizeof(crc16));
    pTLV_Buffer->LV.Length += STSAFEA_CRC_LENGTH;
  }
}

/**
  * @brief   StSafeA_Crc16_ccitt
  *          Computes the CRC16 CCITT over the data passed (header & data pointer).
  *
  * @param   Header : Payload's Header.
  * @param   pData  : Payload's data.
  * @param   Length : Length of payload's data.
  * @retval  uint16_t containing the calculated CRC
  */
static uint16_t StSafeA_Crc16_ccitt(uint8_t Header, uint8_t *pData, uint16_t Length)
{
#if (!STSAFEA_USE_OPTIMIZATION_CRC_TABLE)
  static uint16_t crc16_reflected_lookup_table[256] = {0};
#else
  static const uint16_t crc16_reflected_lookup_table[256] = {STSAFEA_CRC16_X25_REFLECTED_LOOKUP_TABLE};
#endif /* STSAFEA_USE_OPTIMIZATION_CRC_TABLE */

  uint16_t crc = 0xFFFF;
  if (pData != NULL)
  {
    uint8_t ndx;
    uint16_t i;

#if (!STSAFEA_USE_OPTIMIZATION_CRC_TABLE)
    /* Instead of reflecting the data coming in, and the CRC coming in, and
       the CRC going out, simply reflect the polynomial and the operations.
       Only need to do that oncethrough the code.
       The reflected polynomial is 0x8408. */
    if (crc16_reflected_lookup_table[1] == 0x0000U)
    {
      uint16_t  remainder;
      for (int dividend = 0; dividend < 256; ++dividend)
      {
        remainder = dividend;
        for (uint8_t bit = 8; bit > 0; --bit)
        {
          if (remainder & 1)
          {
            remainder = (remainder >> 1) ^ 0x8408;
          }
          else
          {
            remainder = (remainder >> 1);
          }
        }
        crc16_reflected_lookup_table[dividend] = remainder;
      }
    }
#endif /* STSAFEA_USE_OPTIMIZATION_CRC_TABLE */

    ndx = (uint8_t)(0xFFU & (crc ^ Header));
    crc = (crc16_reflected_lookup_table[ndx] ^ 0xFFU);

    for (i = 0; i < Length; i++)
    {
      ndx = (uint8_t)(0xFFU & (crc ^ pData[i]));
      crc = (crc16_reflected_lookup_table[ndx] ^ (crc >> 8));
    }
  }

  return crc;
}

/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

