/**
  ******************************************************************************
  * @file  cc10x_usart.h
  * @author  MCD Application Team
  * @version  V1.0.0
  * @date  20/01/2014
  * @brief  This file contains all the functions prototypes for the USART 
  *         firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Mindmotion MICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2014 Mindmotion Microelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CC10x_USART_H
#define __CC10x_USART_H

/* Includes ------------------------------------------------------------------*/
#include "cc10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup USART
  * @{
  */ 

/** @defgroup USART_Exported_Types
  * @{
  */ 

/** 
  * @brief  USART Init Structure definition  
  */ 
  
typedef struct
{
  uint32_t USART_BaudRate;
  uint16_t USART_WordLength;
  uint16_t USART_StopBits;
  uint16_t USART_Parity;
  uint16_t USART_Mode;
  uint16_t USART_HardwareFlowControl; 
} USART_InitTypeDef;


/**
  * @}
  */ 

/** @defgroup USART_Exported_Constants
  * @{
  */ 
  
#define IS_USART_ALL_PERIPH(PERIPH) (((*(uint32_t*)&(PERIPH)) == USART1_BASE) || \
                                     ((*(uint32_t*)&(PERIPH)) == USART2_BASE) || \
                                     ((*(uint32_t*)&(PERIPH)) == USART3_BASE))
#define IS_USART_123_PERIPH(PERIPH) (((*(uint32_t*)&(PERIPH)) == USART1_BASE) || \
                                     ((*(uint32_t*)&(PERIPH)) == USART2_BASE) || \
                                     ((*(uint32_t*)&(PERIPH)) == USART3_BASE))

/** @defgroup USART_Word_Length 
  * @{
  */ 

#define USART_WordLength_5b                  ((uint16_t)0x0000)
#define USART_WordLength_6b                  ((uint16_t)0x0010)
#define USART_WordLength_7b                  ((uint16_t)0x0020)
#define USART_WordLength_8b                  ((uint16_t)0x0030)

                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_5b) || \
                                      ((LENGTH) == USART_WordLength_6b) || \
																			((LENGTH) == USART_WordLength_7b) || \
																			((LENGTH) == USART_WordLength_8b))
/**
  * @}
  */ 

/** @defgroup USART_Stop_Bits 
  * @{
  */ 
  
#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_2                     ((uint16_t)0x0004)

#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                    ((STOPBITS) == USART_StopBits_2)) 
                                     
/**
  * @}
  */ 

/** @defgroup USART_Parity 
  * @{
  */ 
  
#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0003)
#define USART_Parity_Odd                     ((uint16_t)0x0001) 
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))
/**
  * @}
  */ 

/** @defgroup USART_Mode 
  * @{
  */ 
  
#define USART_Mode_Rx                        ((uint16_t)0x0008)
#define USART_Mode_Tx                        ((uint16_t)0x0010)
#define IS_USART_MODE(MODE) ((((MODE) & (uint16_t)0xFFE7) == 0x00) && ((MODE) != (uint16_t)0x00))

#define USART_EN                             ((uint16_t)0x0001)

/**
  * @}
  */ 

/** @defgroup USART_Hardware_Flow_Control 
  * @{
  */ 
#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)

#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_CTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

/** @defgroup USART_Interrupt_definition 
  * @{
  */
 

#define USART_IT_ERR            ((uint16_t)0x0020)
#define USART_IT_PE             ((uint16_t)0x0010)
#define USART_OVER_ERR          ((uint16_t)0x0008)
#define USART_TIMEOUT_ERR       ((uint16_t)0x0004)
#define USART_IT_RXIEN          ((uint16_t)0x0002)
#define USART_IT_TXIEN          ((uint16_t)0x0001)

#define IS_USART_CONFIG_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXIEN) || \
                               ((IT) == USART_IT_RXIEN) || ((IT) == USART_IT_ERR))
                                 
#define IS_USART_GET_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXIEN) || \
                            ((IT) == USART_IT_ERR) || ((IT) == USART_IT_RXIEN) || \
                            ((IT) == USART_OVER_ERR) || ((IT) == USART_TIMEOUT_ERR))
                            
#define IS_USART_CLEAR_IT(IT) ((IT) == USART_IT_RXIEN)

                                           
/**
  * @}
  */

/** @defgroup USART_DMA_Requests 
  * @{
  */
#define USART_DMAReq_EN                      ((uint16_t)0x0002)

#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFFFD) == 0x00) && ((DMAREQ) != (uint16_t)0x00))

/**
  * @}
  */ 


/** @defgroup USART_Flags 
  * @{
  */


#define USART_FLAG_TXEMPTY                   ((uint16_t)0x0008)
#define USART_FLAG_TXFULL                    ((uint16_t)0x0004)
#define USART_FLAG_RXAVL                     ((uint16_t)0x0002)
#define USART_FLAG_TXEPT                     ((uint16_t)0x0001)


#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_TXEMPTY) || ((FLAG) == USART_FLAG_TXFULL) || \
                             ((FLAG) == USART_FLAG_RXAVL) || ((FLAG) == USART_FLAG_TXEPT))
                            
                              
#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0x00FF) == 0x00) && ((FLAG) != (uint16_t)0x00))

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))
#define IS_USART_ADDRESS(ADDRESS) ((ADDRESS) <= 0xF)
#define IS_USART_DATA(DATA) ((DATA) <= 0x1FF)

/**
  * @}
  */ 

/**
  * @}
  */ 

/** @defgroup USART_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USART_Exported_Functions
  * @{
  */

void USART_DeInit(USART_TypeDef* USARTx);
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState);
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState);
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
uint16_t USART_ReceiveData(USART_TypeDef* USARTx);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG);
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG);
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT);
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT);

#endif /* __CC10x_USART_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2014 Mindmotion Microelectronics *****END OF FILE****/
