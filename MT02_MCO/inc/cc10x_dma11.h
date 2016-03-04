/**
  ******************************************************************************
  * @file  cc10x_dma.h
  * @author  MCD Application Team
  * @version  V1.0.0
  * @date  02/24/2014
  * @brief  This file contains all the functions prototypes for the DMA firmware 
  *         library.
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
  * <h2><center>&copy; COPYRIGHT 2014 MINDMOTION Microelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CC10x_DMA_H
#define __CC10x_DMA_H

/* Includes ------------------------------------------------------------------*/
#include "cc10x.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @addtogroup DMA
  * @{
  */

/** @defgroup DMA_Exported_Types
  * @{
  */

/** 
  * @brief  DMA Init structure definition
  */
/*
typedef struct
{
  uint32_t DMA_PeripheralBaseAddr;
  uint32_t DMA_MemoryBaseAddr;
  uint32_t DMA_DIR;
  uint32_t DMA_BufferSize;
  uint32_t DMA_PeripheralInc;
  uint32_t DMA_MemoryInc;
  uint32_t DMA_PeripheralDataSize;
  uint32_t DMA_MemoryDataSize;
  uint32_t DMA_Mode;
  uint32_t DMA_Priority;
  uint32_t DMA_M2M;
}DMA_InitTypeDef;
*/
typedef struct
{
  uint32_t DMA_PeripheralBaseAddr;
  uint32_t DMA_MemoryBaseAddr;
  uint32_t DMA_DIR;
  uint32_t DMA_BufferSize;
  uint32_t DMA_PeripheralInc;
  uint32_t DMA_MemoryInc;
  uint32_t DMA_PeripheralDataSize;
  uint32_t DMA_MemoryDataSize;
  uint32_t DMA_Mode;
  uint32_t DMA_Priority;
  uint32_t DMA_SRC_PER;
	uint32_t DMA_DST_PER;
}DMA_InitTypeDef;

/**
  * @}
  */

/** @defgroup DMA_Exported_Constants
  * @{
  */

#define IS_DMA_ALL_PERIPH(PERIPH) (((*(uint32_t*)&(PERIPH)) == DMA1_Channel1_BASE) || \
                                   ((*(uint32_t*)&(PERIPH)) == DMA1_Channel2_BASE)  || \
                                   ((*(uint32_t*)&(PERIPH)) == DMA1_Channel3_BASE)  || \
                                   ((*(uint32_t*)&(PERIPH)) == DMA1_Channel4_BASE)  || \
                                   ((*(uint32_t*)&(PERIPH)) == DMA1_Channel5_BASE)  || \
                                   ((*(uint32_t*)&(PERIPH)) == DMA1_Channel6_BASE)  || \
                                   ((*(uint32_t*)&(PERIPH)) == DMA1_Channel7_BASE))
                                   

/** @defgroup DMA_data_transfer_direction 
  * @{
  */

												 
#define DMA_DIR_PeripheralDST              ((uint32_t)0x00100000)   //MTOP
#define DMA_DIR_PeripheralSRC              ((uint32_t)0x00200000)		//PTOM	
#define DMA_DIR_M2M					               ((uint32_t)0x00000000)		//MTOM	
#define DMA_DIR_P2P					               ((uint32_t)0x00300000)		//PTOP	
#define IS_DMA_DIR(DIR) (((DIR) == DMA_DIR_PeripheralDST) || \
                         ((DIR) == DMA_DIR_PeripheralSRC) || \
												 ((DIR) == DMA_DIR_M2M)           || \
												 ((DIR) == DMA_DIR_P2P))
/**
  * @}
  */

/** @defgroup DMA_peripheral_incremented_mode 
  * @{
  */

#define DMA_DstInc_Enable           ((uint32_t)0x00000000)  //address increase
#define DMA_DstInc_Disable          ((uint32_t)0x00000100)  //address no change

#define IS_DMA_PERIPHERAL_INC_STATE(STATE) (((STATE) == DMA_DstInc_Enable) || \
                                            ((STATE) == DMA_DstInc_Enable))
/**
  * @}
  */

/** @defgroup DMA_memory_incremented_mode 
  * @{
  */

#define DMA_SrcInc_Enable               ((uint32_t)0x00000000)  //address increase
#define DMA_SrcInc_Disable              ((uint32_t)0x00000400)  //address no change
#define IS_DMA_MEMORY_INC_STATE(STATE) (((STATE) == DMA_SrcInc_Enable) || \
                                        ((STATE) == DMA_SrcInc_Enable))
/**
  * @}
  */

/** @defgroup DMA_peripheral_data_size 
  * @{
  */

#define DMA_PeripheralDataSize_Byte        ((uint32_t)0x00000000)
#define DMA_PeripheralDataSize_HalfWord    ((uint32_t)0x00000002)
#define DMA_PeripheralDataSize_Word        ((uint32_t)0x00000004)
#define IS_DMA_PERIPHERAL_DATA_SIZE(SIZE) (((SIZE) == DMA_PeripheralDataSize_Byte) || \
                                           ((SIZE) == DMA_PeripheralDataSize_HalfWord) || \
                                           ((SIZE) == DMA_PeripheralDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_memory_data_size 
  * @{
  */

#define DMA_MemoryDataSize_Byte            ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord        ((uint32_t)0x00000010)
#define DMA_MemoryDataSize_Word            ((uint32_t)0x00000020)
#define IS_DMA_MEMORY_DATA_SIZE(SIZE) (((SIZE) == DMA_MemoryDataSize_Byte) || \
                                       ((SIZE) == DMA_MemoryDataSize_HalfWord) || \
                                       ((SIZE) == DMA_MemoryDataSize_Word))
/**
  * @}
  */

/** @defgroup DMA_circular_normal_mode 
  * @{
  */

#define DMA_Mode_Hardware                  ((uint32_t)0x00000000)
#define DMA_Mode_Software                  ((uint32_t)0x00000C00)
#define IS_DMA_MODE(MODE) (((MODE) == DMA_Mode_Hardware) || ((MODE) == DMA_Mode_Software))

/**
  * @}
  */

/** @defgroup DMA_priority_level 
  * @{
  */

#define DMA_Priority_VeryHigh              ((uint32_t)0x00000080)
#define DMA_Priority_High                  ((uint32_t)0x00000040)
#define DMA_Priority_Medium                ((uint32_t)0x00000020)
#define DMA_Priority_Low                   ((uint32_t)0x00000000)
#define IS_DMA_PRIORITY(PRIORITY) (((PRIORITY) == DMA_Priority_VeryHigh) || \
                                   ((PRIORITY) == DMA_Priority_High) || \
                                   ((PRIORITY) == DMA_Priority_Medium) || \
                                   ((PRIORITY) == DMA_Priority_Low))
																	 
#define FCMODE 					(0x00000001<<0)    //half fifo 
#define FIFO_MODE				(0x00000000<<1)    //half fifo 
#define PROTCTL					(0x00000001<<2)   //half fifo:bit[36:34]

/**
  * @}
  */

/** @defgroup DMA_HARDWARE_REQUEST 
  * @{
  */
#define SRC_PER_ACK0			(0x00000000<<7)  //Source use and dma_ack[0]:bit[42:39]
#define SRC_PER_ACK1			(0x00000001<<7)  //Source use and dma_ack[1]:bit[42:39]
#define SRC_PER_ACK2			(0x00000002<<7)  //Source use and dma_ack[2]:bit[42:39]
#define SRC_PER_ACK3			(0x00000003<<7)  //Source use and dma_ack[3]:bit[42:39]
#define SRC_PER_ACK4			(0x00000004<<7)
#define SRC_PER_ACK5			(0x00000005<<7)	 //Source use and dma_ack[5]:bit[42:39]
#define SRC_PER_ACK6			(0x00000006<<7)
#define SRC_PER_ACK7			(0x00000007<<7)
#define SRC_PER_ACK8			(0x00000008<<7)
#define	SRC_PER_ACK9			(0x00000009<<7)
#define SRC_PER_ACK10			(0x0000000a<<7)
#define SRC_PER_ACK11     (0x0000000b<<7)
#define SRC_PER_ACK12     (0x0000000c<<7)
#define SRC_PER_ACK13     (0x0000000d<<7)
#define SRC_PER_ACK14     (0x0000000e<<7)
#define SRC_PER_ACK15			(0x0000000f<<7)

#define DST_PER_ACK0      (0x00000000<<11) //Destinatin use and dma_ack[-]:bit[46:43]
#define DST_PER_ACK1      (0x00000001<<11) //Destinatin use and dma_ack[-]:bit[46:43]
#define DST_PER_ACK2      (0x00000002<<11) //Destinatin use
#define DST_PER_ACK3			(0x00000003<<11)
#define DST_PER_ACK4			(0x00000004<<11) //Destinatin use 
#define DST_PER_ACK5			(0x00000005<<11)
#define DST_PER_ACK6 			(0x00000006<<11)
#define DST_PER_ACK7 			(0x00000007<<11)
#define DST_PER_ACK8			(0x00000008<<11)
#define DST_PER_ACK9			(0x00000009<<11)
#define DST_PER_ACK10		  (0x0000000a<<11)
#define DST_PER_ACK11		  (0x0000000b<<11)
#define DST_PER_ACK12			(0x0000000c<<11)
#define DST_PER_ACK13			(0x0000000d<<11)
#define DST_PER_ACK14			(0x0000000e<<11) //Destinatin use dma_ack[14]
#define DST_PER_ACK15			(0x0000000f<<11) //Destinatin use dma_ack[15]

/**
  * @}
  */

/** @defgroup DMA_interrupts_definition 
  * @{
  */

#define DMA_IT_EN                          ((uint32_t)0x00000001)
#define IS_DMA_CONFIG_IT(IT) ((((IT) & 0xFFFFFFFE) == 0x00) && ((IT) != 0x00))

/** 
  * @brief  For DMA1
  */

#define DMA1_IT_TC1                        ((uint32_t)0x00000001)
#define DMA1_IT_TC2                        ((uint32_t)0x00000002)
#define DMA1_IT_TC3                        ((uint32_t)0x00000004)
#define DMA1_IT_TC4                        ((uint32_t)0x00000008)
#define DMA1_IT_TC5                        ((uint32_t)0x00000010)
#define DMA1_IT_TC6                        ((uint32_t)0x00000020)
#define DMA1_IT_TC7                        ((uint32_t)0x00000040)

#define IS_DMA_CLEAR_IT(IT) ((((IT) & 0xFFFFFF00) == 0x00)  && ((IT) != 0x00))

#define IS_DMA_GET_IT(IT) (((IT) == DMA1_IT_TC1) || ((IT) == DMA1_IT_TC2) || \
                           ((IT) == DMA1_IT_TC3) || ((IT) == DMA1_IT_TC4) || \
                           ((IT) == DMA1_IT_TC5) || ((IT) == DMA1_IT_TC6) || \
                           ((IT) == DMA1_IT_TC7))
                           
                           

/**
  * @}
  */

/** @defgroup DMA_flags_definition 
  * @{
  */

/** 
  * @brief  For DMA1
  */

#define DMA1_FLAG_TC1           					 ((uint32_t)0x00000001)
#define DMA1_FLAG_TC2           					 ((uint32_t)0x00000002)
#define DMA1_FLAG_TC3           					 ((uint32_t)0x00000004)
#define DMA1_FLAG_TC4           					 ((uint32_t)0x00000008)
#define DMA1_FLAG_TC5           					 ((uint32_t)0x00000010)
#define DMA1_FLAG_TC6           					 ((uint32_t)0x00000020)
#define DMA1_FLAG_TC7	           					 ((uint32_t)0x00000040)


#define IS_DMA_CLEAR_FLAG(FLAG) ((((FLAG) & 0xFFFFFF00) == 0x00 ) && ((FLAG) != 0x00))

#define IS_DMA_GET_FLAG(FLAG) (((FLAG) == DMA1_FLAG_TC1) || ((FLAG) == DMA1_FLAG_TC2) || \
                               ((FLAG) == DMA1_FLAG_TC3) || ((FLAG) == DMA1_FLAG_TC4) || \
                               ((FLAG) == DMA1_FLAG_TC5) || ((FLAG) == DMA1_FLAG_TC6) || \
                               ((FLAG) == DMA1_FLAG_TC7)) 
/**
  * @}
  */

/** @defgroup DMA_Buffer_Size 
  * @{
  */

#define IS_DMA_BUFFER_SIZE(SIZE) (((SIZE) >= 0x1) && ((SIZE) < 0x10000))

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Exported_Functions
  * @{
  */

void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx);
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState);
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState);
//uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx);
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG);
void DMA_ClearFlag(uint32_t DMA_FLAG);
ITStatus DMA_GetITStatus(uint32_t DMA_IT);
void DMA_ClearITPendingBit(uint32_t DMA_IT);

#endif /*__CC10x_DMA_H */
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
