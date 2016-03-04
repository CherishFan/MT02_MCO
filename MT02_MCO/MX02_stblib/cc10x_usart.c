/**
  ******************************************************************************
  * @file  cc10x_usart.c
  * @author  MCD Application Team
  * @version  V1.0.0
  * @date  01/20/2014
  * @brief  This file provides all the USART firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "cc10x_usart.h"
#include "cc10x_rcc.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @defgroup USART 
  * @brief USART driver modules
  * @{
  */

/** @defgroup USART_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @defgroup USART_Private_Defines
  * @{
  */


//wsf 20140120 add
/* USART UE Mask */
#define GCR_UE_Set                ((uint16_t)0x0001)  /* USART Enable Mask */
#define GCR_UE_Reset              ((uint16_t)0xFFFE)  /* USART Disable Mask */

#define CCR_CLEAR_Mask       			((uint32_t)0xFFFFFF30)  /* USART CCR Mask */
#define GCR_CLEAR_Mask       			((uint32_t)0xFFFFFFE0)  /* USART GCR Mask */
/**
  * @}
  */

/** @defgroup USART_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup USART_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup USART_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup USART_Private_Functions
  * @{
  */

/**
  * @brief  Deinitializes the USARTx peripheral registers to their
  *   default reset values.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @retval : None
  */
void USART_DeInit(USART_TypeDef* USARTx)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  switch (*(uint32_t*)&USARTx)
  {
    case USART1_BASE:
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
      break;
    case USART2_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
      break;
    case USART3_BASE:
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
      break;       
    default:
      break;
  }
}

/**
  * @brief  Initializes the USARTx peripheral according to the specified
  *   parameters in the USART_InitStruct .
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @param USART_InitStruct: pointer to a USART_InitTypeDef structure
  *   that contains the configuration information for the
  *   specified USART peripheral.
  * @retval : None
  */
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  uint32_t tmpreg = 0x00, apbclock = 0x00;
  
  uint32_t usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

  usartxbase = (*(uint32_t*)&USARTx);
/*---------------------------- USART CCR Configuration -----------------------*/
  /* get USART CCR values */
  tmpreg = USARTx->CCR;
  /* Clear spb,psel,pen bits */
  tmpreg &= CCR_CLEAR_Mask;
  /* Configure the USART Word Length,the USART Stop Bits,Parity ------------*/
	/* Set the char bits according to USART_WordLength value */
  /* Set spb bit according to USART_StopBits value */
	/* Set PEN bit according to USART_Parity value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength |(uint32_t)USART_InitStruct->USART_StopBits |USART_InitStruct->USART_Parity;
  
  /* Write to USART CCR */
  USARTx->CCR = tmpreg;

/*---------------------------- USART GCR Configuration -----------------------*/
	/* get USART GCR values */  
  tmpreg = USARTx->GCR;
  /* Clear TXEN and RXEN ,autoflowen, mode ,uarten bits */
  tmpreg &= GCR_CLEAR_Mask;
  /* Set autorlowen bit according to USART_HardwareFlowControl value */
  /* Set rxen,txen bits according to USART_Mode value */
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl | USART_InitStruct->USART_Mode ;
  /* Write to USART GCR */
  USARTx->GCR = tmpreg;
/*---------------------------- USART BRR Configuration -----------------------*/
  /* Configure the USART Baud Rate -------------------------------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    //apbclock = 30000000L;//30000000L;
	apbclock = RCC_ClocksStatus.PCLK2_Frequency;
		
  }
  else
  {
		//apbclock = 8000000L;//30000000L;
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  /* Determine the usart_baud*/
  tmpreg = (apbclock / 16) /(USART_InitStruct->USART_BaudRate);
  /* Write to USART BRR */
  USARTx->BRR = tmpreg;
}

/**
  * @brief  Fills each USART_InitStruct member with its default value.
  * @param USART_InitStruct: pointer to a USART_InitTypeDef structure
  *   which will be initialized.
  * @retval : None
  */
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
  /* USART_InitStruct members default value */
  USART_InitStruct->USART_BaudRate = 9600;
  USART_InitStruct->USART_WordLength = USART_WordLength_8b;
  USART_InitStruct->USART_StopBits = USART_StopBits_1;
  USART_InitStruct->USART_Parity = USART_Parity_No ;
  USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	
}


/**
  * @brief  Enables or disables the specified USART peripheral.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @param NewState: new state of the USARTx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval : None
  */
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected USART by setting the uarten bit in the GCR register */
    USARTx->GCR |= GCR_UE_Set;
  }
  else
  {
    /* Disable the selected USART by clearing the uarten bit in the GCR register */
    USARTx->GCR &= GCR_UE_Reset;
  }
}

/**
  * @brief  Enables or disables the specified USART interrupts.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @param USART_IT: specifies the USART interrupt sources to be
  *   enabled or disabled.
  *   This parameter can be one of the following values:
  * 
  * @arg USART_IT_ERR:  Error interrupt(Frame error,)
  * @arg USART_IT_PE:   Parity Error interrupt
	* @arg USART_OVER_ERR:  overrun Error interrupt
  * @arg USART_TIMEOUT_ERR:  timeout Error interrupt
  * @arg USART_IT_RXIEN: Receive Data register interrupt 
	* @arg USART_IT_TXIEN:  Tansmit Data Register empty interrupt
  * @param NewState: new state of the specified USARTx interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval : None
  */
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
  	 /* Enable the USART_IT interrupt */
    USARTx->IER  |= USART_IT;
  }
  else
  {
  	/* Disable the USART_IT interrupt */
    USARTx->IER  &= ~ USART_IT;
  }
}

/**
  * @brief  Enables or disables the USART’s DMA interface.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3 .  
  * @param USART_DMAReq: specifies the DMA request.
  *   This parameter can be any combination of the following values:
  * @arg USART_DMAReq_EN: USART DMA transmit request
  * 
  * @param NewState: new state of the DMA Request sources.
  *   This parameter can be: ENABLE or DISABLE.
  * @note The DMA mode is not available for UART5.  
  * @retval : None
  */
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_USART_1234_PERIPH(USARTx));
  assert_param(IS_USART_DMAREQ(USART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  if (NewState != DISABLE)
  {
    /* Enable the DMA transfer */
    USARTx->GCR |= USART_DMAReq;
  }
  else
  {
    /* Disable the DMA transfer */
    USARTx->GCR &= ~USART_DMAReq;
  }
}


/**
  * @brief  Transmits single data through the USARTx peripheral.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @param Data: the data to transmit.
  * @retval : None
  */
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->TDR = (Data & (uint16_t)0x00FF);
}

/**
  * @brief  Returns the most recent received data by the USARTx peripheral.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @retval : The received data.
  */
uint16_t USART_ReceiveData(USART_TypeDef* USARTx)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* Receive Data */
  return (uint16_t)(USARTx->RDR & (uint16_t)0x00FF);
}

  
/**
  * @brief  Checks whether the specified USART flag is set or not.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @param USART_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  * @arg USART_FLAG_TXEMPTY:Transmit data register empty flag 
  * @arg USART_FLAG_TXFULL:Transmit data buffer full 
  * @arg USART_FLAG_RXAVL:RX Buffer has a byte flag
  * @arg USART_FLAG_OVER:OverRun Error flag 
  * @arg USART_FLAG_TXEPT: tx and shifter are emptys flag
  * @retval : The new state of USART_FLAG (SET or RESET).
  */
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_FLAG(USART_FLAG));
  if ((USARTx->CSR & USART_FLAG) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  Clears the USARTx's pending flags.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param USART_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  * @arg USART_FLAG_TXEMPTY:Transmit data register empty flag 
  * @arg USART_FLAG_TXFULL:Transmit data buffer full 
  * @arg USART_FLAG_RXAVL:RX Buffer has a byte flag
  * @arg USART_FLAG_OVER:OverRun Error flag 
  * @arg USART_FLAG_TXEPT: tx and shifter are emptys flag
  * @retval : None
  */
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  
}

/**
  * @brief  Checks whether the specified USART interrupt has occurred or not.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3.
  * @param USART_IT: specifies the USART interrupt source to check.
  *   This parameter can be one of the following values:
  * @arg USART_IT_ERR:  Error interrupt(Frame error,)
  * @arg USART_IT_PE:   Parity Error interrupt
	* @arg USART_OVER_ERR:  overrun Error interrupt
  * @arg USART_TIMEOUT_ERR:  timeout Error interrupt
  * @arg USART_IT_RXIEN: Receive Data register interrupt 
	* @arg USART_IT_TXIEN:  Tansmit Data Register empty interrupt
  * @retval : The new state of USART_IT (SET or RESET).
  */
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT)
{
	FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_FLAG(USART_FLAG));
  assert_param(IS_USART_PERIPH_FLAG(USARTx, USART_FLAG)); /* The CTS flag is not available for UART4 and UART5 */   
  if ((USARTx->ISR & USART_IT) != (uint16_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  Clears the USARTx’s interrupt pending bits.
  * @param USARTx: Select the USART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param USART_IT: specifies the interrupt pending bit to clear.
  *   This parameter can be one of the following values:
  * @arg USART_IT_ERR:  Error interrupt(Frame error,)
  * @arg USART_IT_PE:   Parity Error interrupt
	* @arg USART_OVER_ERR:  overrun Error interrupt
  * @arg USART_TIMEOUT_ERR:  timeout Error interrupt
  * @arg USART_IT_RXIEN: Receive Data register interrupt 
	* @arg USART_IT_TXIEN:  Tansmit Data Register empty interrupt

  * @retval : None
  */
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_IT(USART_IT));
  assert_param(IS_USART_PERIPH_IT(USARTx, USART_IT)); /* The CTS interrupt is not available for UART4 and UART5 */
  /*clear USART_IT pendings bit*/
  USARTx->ICR = USART_IT;
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

/******************* (C) COPYRIGHT 2014 Mindmotion Microelectronics *****END OF FILE****/
