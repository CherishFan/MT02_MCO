/**
  ******************************************************************************
  * @file    cc10x_opamp.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    14-04-2014
  * @brief   This file contains all the functions prototypes for the operational
  *          amplifiers (OPAMP) firmware library.         
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 Mindmotion Microelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CC10x_OPAMP_H
#define __CC10x_OPAMP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cc10x.h"

/** @addtogroup CC10x_StdPeriph_Driver
  * @{
  */

/** @addtogroup OPAMP
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  OPAMP Init structure definition  
  */
  
typedef struct
{

  uint32_t OPAMP_InvertingInput;     /*!< Selects the inverting input of the operational amplifier.
                                          This parameter can be a value of @ref OPAMP_InvertingInput */

  uint32_t OPAMP_NonInvertingInput;  /*!< Selects the non inverting input of the operational amplifier.
                                         This parameter can be a value of @ref OPAMP_NonInvertingInput */

}OPAMP_InitTypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup OPAMP_Exported_Constants
  * @{
  */ 

/** @defgroup OPAMP_Selection
  * @{
  */

#define OPAMP_Selection_OPAMP1                    ((uint32_t)0x00000000) /*!< OPAMP1 Selection */
#define OPAMP_Selection_OPAMP2                    ((uint32_t)0x00000001) /*!< OPAMP2 Selection */
#define OPAMP_Selection_OPAMP3                    ((uint32_t)0x00000002) /*!< OPAMP3 Selection */
#define OPAMP_Selection_OPAMP4                    ((uint32_t)0x00000003) /*!< OPAMP4 Selection */

#define IS_OPAMP_ALL_PERIPH(PERIPH) (((PERIPH) == OPAMP_Selection_OPAMP1) || \
                                     ((PERIPH) == OPAMP_Selection_OPAMP2) || \
                                     ((PERIPH) == OPAMP_Selection_OPAMP3) || \
                                     ((PERIPH) == OPAMP_Selection_OPAMP4))
 
/**
  * @}
  */

/** @defgroup OPAMP_InvertingInput
  * @{
  */

#define OPAMP_InvertingInput_IO1          ((uint32_t)0x00000000) /*!< IO1 (PA7 for OPAMP1,PC5 for OPAMP2, PA5 for OPAMP3, PA4 for OPAMP4)
                                                                     connected to OPAMPx inverting input */
#define OPAMP_InvertingInput_IO2          ((uint32_t)0x10101010)  /*!< IO2 (PA5 for OPAMP1, PA5 for OPAMP2, PA4 for OPAMP3, PA5 for OPAMP4)
                                                                      connected to OPAMPx inverting input */
#define OPAMP_InvertingInput_IO3          ((uint32_t)0x20202020)   /*!< IO3 (PA3 for OPAMP1, PA3 for OPAMP2, PA1 for OPAMP3, PA6 for OPAMP4)
                                                                      connected to OPAMPx inverting input */
#define OPAMP_InvertingInput_IO4          ((uint32_t)0x30303030)    /*!< IO4 (PA1 for OPAMP1, PA1 for OPAMP2, PB0 for OPAMP3, PA7 for OPAMP4)
                                                                      connected to OPAMPx inverting input */lower mode) */

#define IS_OPAMP_INVERTING_INPUT(INPUT) (((INPUT) == OPAMP_InvertingInput_IO1) || \
                                         ((INPUT) == OPAMP_InvertingInput_IO2) || \
                                         ((INPUT) == OPAMP_InvertingInput_IO3) || \
                                         ((INPUT) == OPAMP_InvertingInput_IO4))
/**
  * @}
  */

/** @defgroup OPAMP_NonInvertingInput
  * @{
  */

#define OPAMP_NonInvertingInput_IO1          ((uint32_t)0x00000000) /*!< IO1 (PA7 for OPAMP1, PA5 for OPAMP2, PA3 for OPAMP3, PC4 for OPAMP4)
                                                                        connected to OPAMPx non inverting input */
#define OPAMP_NonInvertingInput_IO2          ((uint32_t)0x04040404) /*!< IO2 (PA5 for OPAMP1, PB0 for OPAMP2, PA5 for OPAMP3, PA4 for OPAMP4)
                                                                         connected to OPAMPx non inverting input */
#define OPAMP_NonInvertingInput_IO3          ((uint32_t)0x08080808) /*!< IO3 (PA3 for OPAMP1, PB1 for OPAMP2, PA1 for OPAMP3, PB0 for OPAMP4)
                                                                         connected to OPAMPx non inverting input */
#define OPAMP_NonInvertingInput_IO4          ((uint32_t)0x0C0C0C0C) /*!< IO4 (PA1 for OPAMP1, PA4 for OPAMP2, PB0 for OPAMP3, PB1 for OPAMP4)
                                                                         connected to OPAMPx non inverting input */

#define IS_OPAMP_NONINVERTING_INPUT(INPUT) (((INPUT) == OPAMP_NonInvertingInput_IO1) || \
                                            ((INPUT) == OPAMP_NonInvertingInput_IO2) || \
                                            ((INPUT) == OPAMP_NonInvertingInput_IO3) || \
                                            ((INPUT) == OPAMP_NonInvertingInput_IO4))

/**
  * @}
  */

/** @defgroup OPAMP_enable
  * @{
  */
#define OPAMP1_CSR_OPAMP1EN               ((uint32_t)0x00000001) /*!< OPAMP1 enable */
#define OPAMP1_CSR_OPAMP2EN               ((uint32_t)0x00000100) /*!< OPAMP2 enable */
#define OPAMP1_CSR_OPAMP3EN               ((uint32_t)0x00010000) /*!< OPAMP3 enable */
#define OPAMP1_CSR_OPAMP4EN               ((uint32_t)0x01000000) /*!< OPAMP4 enable */

/**
  * @}
  */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/*  Function used to set the OPAMP configuration to the default reset state ***/
void OPAMP_DeInit(uint32_t OPAMP_Selection);

/* Initialization and Configuration functions *********************************/
void OPAMP_Init(uint32_t OPAMP_Selection, OPAMP_InitTypeDef* OPAMP_InitStruct);
void OPAMP_StructInit(OPAMP_InitTypeDef* OPAMP_InitStruct);
void OPAMP_Cmd(uint32_t OPAMP_Selection, FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__CC10x_OPAMP_H */

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT Mindmotion Microelectronics *****END OF FILE****/
