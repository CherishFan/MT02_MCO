/**
  ******************************************************************************
  * @file  cc10x_dma.c
  * @author  MCD Application Team
  * @version  V1.0.0
  * @date  25/02/2014
  * @brief  This file provides all the DMA firmware functions.
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
#include "cc10x_dma.h"
#include "cc10x_rcc.h"

/** @addtogroup StdPeriph_Driver
  * @{
  */

/** @defgroup DMA 
  * @brief DMA driver modules
  * @{
  */ 

/** @defgroup DMA_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */

/** @defgroup DMA_Private_Defines
  * @{
  */

//wsf modify
/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_Channel1_IT_Mask    ((uint32_t)0x00000001)
#define DMA1_Channel2_IT_Mask    ((uint32_t)0x00000002)
#define DMA1_Channel3_IT_Mask    ((uint32_t)0x00000004)
#define DMA1_Channel4_IT_Mask    ((uint32_t)0x00000008)
#define DMA1_Channel5_IT_Mask    ((uint32_t)0x00000010)
#define DMA1_Channel6_IT_Mask    ((uint32_t)0x00000020)
#define DMA1_Channel7_IT_Mask    ((uint32_t)0x00000040)

/* DMA1 Channelx reset */
#define CH_MASK_Reset        		 ((uint32_t)0xFFFFFF00)

/* DMA CHANNEL ENABLE mask */
#define CH_ENABLE_Set          ((uint32_t)0x000000FF)
#define CH_ENABLE_Reset        ((uint32_t)0xFFFFFF00)

/* DMA registers Masks */
#define CTL_CLEAR_Mask           ((uint32_t)0xFF8FF801)

/* DMA1 Channelx ENABLE */
#define DMA1_Channel1_ENABLE_Set    ((uint32_t)0x00000001)
#define DMA1_Channel2_ENABLE_Set    ((uint32_t)0x00000002)
#define DMA1_Channel3_ENABLE_Set    ((uint32_t)0x00000004)
#define DMA1_Channel4_ENABLE_Set   	((uint32_t)0x00000008)
#define DMA1_Channel5_ENABLE_Set    ((uint32_t)0x00000010)
#define DMA1_Channel6_ENABLE_Set    ((uint32_t)0x00000020)
#define DMA1_Channel7_ENABLE_Set    ((uint32_t)0x00000040)

/* DMA ENABLE mask */
#define CfgReg_ENABLE_Set          ((uint32_t)0x00000001)
#define CfgReg_ENABLE_Reset        ((uint32_t)0xFFFFFFFE)

/**
  * @}
  */

/** @defgroup DMA_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_Variables
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_FunctionPrototypes
  * @{
  */

/**
  * @}
  */

/** @defgroup DMA_Private_Functions
  * @{
  */

/**
  * @brief  Deinitializes the DMAy Channelx registers to their default reset
  *   values.
  * @param DMAy_Channelx: where y can be 1  to select the DMA and
  *   x can be 1 to 7 for DMA1 to select the 
  *   DMA Channel.
  * @retval : None
  */
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* Disable the selected DMAy Channelx */
	DMA1->ChEnReg_LOW |= CH_ENABLE_Reset;
  /* Reset DMAy Channelx control register */
  DMA1->ChEnReg_LOW = 0;
	
  /* Reset DMAy Channelx remaining bytes register */
  DMAy_Channelx->CTL_LOW  = 0;
	DMAy_Channelx->CTL_HIGH  = 0;
  /* Reset DMAy Channelx peripheral address register */
  DMAy_Channelx->DAR_LOW  = 0;
  
  /* Reset DMAy Channelx memory address register */
  DMAy_Channelx->SAR_LOW = 0;
  switch (*(uint32_t*)&DMAy_Channelx)
  {
    case DMA1_Channel1_BASE:
      /* Reset interrupt pending bits for DMA1 Channel1 */
			DMA1->ClearTfr_LOW = DMA1_Channel1_IT_Mask;
		  DMA1->ClearBlock_LOW = DMA1_Channel1_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel1_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel1_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel1_IT_Mask;
      break;
    case DMA1_Channel2_BASE:
      /* Reset interrupt pending bits for DMA1 Channel2 */
			DMA1->ClearTfr_LOW = DMA1_Channel2_IT_Mask;
		  DMA1->ClearBlock_LOW = DMA1_Channel2_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel2_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel2_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel2_IT_Mask;
      break;
    case DMA1_Channel3_BASE:
      /* Reset interrupt pending bits for DMA1 Channel3 */
		  DMA1->ClearTfr_LOW = DMA1_Channel3_IT_Mask;
		  DMA1->ClearBlock_LOW	= DMA1_Channel3_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel3_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel3_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel3_IT_Mask;
      break;
    case DMA1_Channel4_BASE:
      /* Reset interrupt pending bits for DMA1 Channel4 */
		  DMA1->ClearTfr_LOW = DMA1_Channel4_IT_Mask;
		  DMA1->ClearBlock_LOW = DMA1_Channel4_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel4_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel4_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel4_IT_Mask;
      break;
    case DMA1_Channel5_BASE:
      /* Reset interrupt pending bits for DMA1 Channel5 */
		  DMA1->ClearTfr_LOW = DMA1_Channel5_IT_Mask;
		  DMA1->ClearBlock_LOW = DMA1_Channel5_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel5_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel5_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel5_IT_Mask;
      break;
    case DMA1_Channel6_BASE:
      /* Reset interrupt pending bits for DMA1 Channel6 */
		  DMA1->ClearTfr_LOW = DMA1_Channel6_IT_Mask;
		  DMA1->ClearBlock_LOW = DMA1_Channel6_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel6_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel6_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel6_IT_Mask;
      break;
    case DMA1_Channel7_BASE:
      /* Reset interrupt pending bits for DMA1 Channel7 */
		  DMA1->ClearTfr_LOW = DMA1_Channel7_IT_Mask;
		  DMA1->ClearBlock_LOW = DMA1_Channel7_IT_Mask;
		  DMA1->ClearSrcTran_LOW = DMA1_Channel7_IT_Mask;
		  DMA1->ClearDstTran_LOW = DMA1_Channel7_IT_Mask;
		  DMA1->ClearErr_LOW = DMA1_Channel7_IT_Mask;
      break;

    default:
      break;
  }
}

/**
  * @brief  Initializes the DMAy Channelx according to the specified
  *   parameters in the DMA_InitStruct.
  * @param DMAy_Channelx: where y can be 1  select the DMA and 
  *   x can be 1 to 7 for DMA1  to select the 
  *   DMA Channel.
  * @param DMA_InitStruct: pointer to a DMA_InitTypeDef structure that
  *   contains the configuration information for the specified
  *   DMA Channel.
  * @retval : None
  */
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
  assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));   
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
  assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
  assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
/*--------------------------- DMAy Channelx CCR Configuration -----------------*/
  /* Get the DMAy_Channelx CTL_LOW value */
	tmpreg = DMAy_Channelx->CTL_LOW;
  
  /* Clear TT_FC,SINC,DINC,SRC_T_WIDTH,DST_TR_WIDTH bits */
	tmpreg &= CTL_CLEAR_Mask; 
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode */
  /* Set DIR bit according to DMA_DIR value */
  /* Set PINC bit according to DMA_PeripheralInc value */
  /* Set MINC bit according to DMA_MemoryInc value */
  /* Set PSIZE bits according to DMA_PeripheralDataSize value */
  /* Set MSIZE bits according to DMA_MemoryDataSize value */
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_PeripheralInc | 
						DMA_InitStruct->DMA_MemoryInc |DMA_InitStruct->DMA_PeripheralDataSize | 
						DMA_InitStruct->DMA_MemoryDataSize;
  /*Write value to CTL_LOW*/
	DMAy_Channelx->CTL_LOW = tmpreg;
 
  /* Get the DMAy_Channelx LLP_LOW value */
  tmpreg = DMAy_Channelx->LLP_LOW;
	/* Write value to LLP_LOW */
	DMAy_Channelx->LLP_LOW = 0x0;
	
	 /* Get the DMAy_Channelx CFG_LOW value */
	tmpreg = DMAy_Channelx->CFG_LOW;
	/* Set HS_SEL_DST,HS_SEL_SRC bits according to DMA_Mode value */
	/* Set CH_PRIOR bits according to DMA_Priority value */
	tmpreg = DMA_InitStruct->DMA_Mode |DMA_InitStruct->DMA_Priority ;
	/* Write value to CFG_LOW */
	DMAy_Channelx->CFG_LOW = tmpreg;
	
	/* Get the DMAy_Channelx CFG_HIGH value */
	tmpreg = DMAy_Channelx->CFG_HIGH;
	/* Set SRC_PER bits according to DMA_SRC_PER value */
	/* Set DEST_PER bits according to DMA_DST_PER value */
	tmpreg |= DMA_InitStruct->DMA_SRC_PER |DMA_InitStruct->DMA_DST_PER | PROTCTL | FIFO_MODE | FCMODE;
	/* Write to DMAy Channelx CFG_HIGH */
  DMAy_Channelx->CFG_HIGH = tmpreg;
/*--------------------------- DMAy Channelx CNDTR Configuration ---------------*/
  /* Write to DMAy Channelx CNDTR */
  //DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;
	DMAy_Channelx->CTL_HIGH = DMA_InitStruct->DMA_BufferSize;
/*--------------------------- DMAy Channelx DAR_LOW,SAR_LOW Configuration ----------------*/
  
 tmpreg = DMAy_Channelx->CTL_LOW;
 tmpreg = (tmpreg>>20)<<20;
 if(tmpreg==DMA_DIR_PeripheralDST)
 {
	  /* Write to DMAy Channelx DAR_LOW */
	 DMAy_Channelx->DAR_LOW  =DMA_InitStruct->DMA_PeripheralBaseAddr;
	  /* Write to DMAy Channelx SAR_LOW */
	  DMAy_Channelx->SAR_LOW = DMA_InitStruct->DMA_MemoryBaseAddr;
 }
 else if(tmpreg==DMA_DIR_PeripheralSRC)
 {
	 /* Write to DMAy Channelx DAR_LOW */
		DMAy_Channelx->DAR_LOW = DMA_InitStruct->DMA_MemoryBaseAddr;
	 /* Write to DMAy Channelx SAR_LOW */
	  DMAy_Channelx->SAR_LOW = DMA_InitStruct->DMA_PeripheralBaseAddr;
 }
 else
 {
	 /* Write to DMAy Channelx DAR_LOW */
		DMAy_Channelx->DAR_LOW  =DMA_InitStruct->DMA_PeripheralBaseAddr;
	 /* Write to DMAy Channelx SAR_LOW */
		DMAy_Channelx->SAR_LOW = DMA_InitStruct->DMA_MemoryBaseAddr;
	}

}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param DMA_InitStruct : pointer to a DMA_InitTypeDef structure
  *   which will be initialized.
  * @retval : None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
  /* Initialize the DMA_MemoryBaseAddr member */
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;
  /* Initialize the DMA_DIR member */
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
  /* Initialize the DMA_BufferSize member */
  DMA_InitStruct->DMA_BufferSize = 0;
  /* Initialize the DMA_PeripheralInc member */
  DMA_InitStruct->DMA_PeripheralInc = DMA_DstInc_Enable;
  /* Initialize the DMA_MemoryInc member */
  DMA_InitStruct->DMA_MemoryInc = DMA_SrcInc_Enable;
  /* Initialize the DMA_PeripheralDataSize member */
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* Initialize the DMA_MemoryDataSize member */
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* Initialize the DMA_Mode member */
  DMA_InitStruct->DMA_Mode = DMA_Mode_Hardware;
  /* Initialize the DMA_Priority member */
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
  /* Initialize the DMA Hardware DEST resuesr member */
	DMA_InitStruct->DMA_DST_PER = DST_PER_ACK0;
	/* Initialize the DMA Hardware SRC resuesr member */
	DMA_InitStruct->DMA_SRC_PER = SRC_PER_ACK1;
}

/**
  * @brief  Enables or disables the specified DMAy Channelx.
  * @param DMAy_Channelx: where y can be 1 or 2 to select the DMA and 
  *   x can be 1 to 7 for DMA1 to select the 
  *   DMA Channel.
  * @param NewState: new state of the DMAy Channelx. 
  *   This parameter can be: ENABLE or DISABLE.
  * @retval : None
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected DMAy Channelx */
		 DMA1->DmaCfgReg_LOW = CfgReg_ENABLE_Set;  
  }
  else
  {
    /* Disable the selected DMAy Channelx */
		DMA1->DmaCfgReg_LOW = CfgReg_ENABLE_Reset; 
  }
	  switch (*(uint32_t*)&DMAy_Channelx)
		{
    case DMA1_Channel1_BASE:
      /* enable selected channel1 for DMA1 */
      DMA1->ChEnReg_LOW = (DMA1_Channel1_ENABLE_Set<<8)|DMA1_Channel1_ENABLE_Set;
      break;
    case DMA1_Channel2_BASE:
			 /* enable selected channel2 for DMA1 */
     DMA1->ChEnReg_LOW = (DMA1_Channel2_ENABLE_Set<<8)|DMA1_Channel2_ENABLE_Set;
      break;
    case DMA1_Channel3_BASE:
			/* enable selected channel3 for DMA1 */
      DMA1->ChEnReg_LOW = (DMA1_Channel3_ENABLE_Set<<8)|DMA1_Channel3_ENABLE_Set;
      break;
    case DMA1_Channel4_BASE:
			/* enable selected channel4 for DMA1 */
      DMA1->ChEnReg_LOW = (DMA1_Channel4_ENABLE_Set<<8)|DMA1_Channel4_ENABLE_Set;
      break;
    case DMA1_Channel5_BASE:
			/* enable selected channel5 for DMA1 */
      DMA1->ChEnReg_LOW = (DMA1_Channel5_ENABLE_Set<<8)|DMA1_Channel5_ENABLE_Set;
      break;
    case DMA1_Channel6_BASE:
			/* enable selected channel6 for DMA1 */
      DMA1->ChEnReg_LOW = (DMA1_Channel6_ENABLE_Set<<8)|DMA1_Channel6_ENABLE_Set;
      break;
    case DMA1_Channel7_BASE:
			/* enable selected channel7 for DMA1 */
      DMA1->ChEnReg_LOW = (DMA1_Channel7_ENABLE_Set<<8)|DMA1_Channel7_ENABLE_Set;
      break;

    default:
			/* reset  for DMA1 channels*/
			 DMA1->ChEnReg_LOW = CH_ENABLE_Reset;
      break;
  }
}

/**
  * @brief  Enables or disables the specified DMAy Channelx interrupts.
  * @param DMAy_Channelx: where y can be 1 or 2 to select the DMA and 
  *   x can be 1 to 7 for DMA1 to select the 
  *   DMA Channel.
  * @param DMA_IT: specifies the DMA interrupts sources to be enabled
  *   or disabled. 
  *   This parameter can be any combination of the following values:
	* @arg DMA_IT_EN:  Transfer interrupt mask
  * @param NewState: new state of the specified DMA interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval : None
  */
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* Enable the selected DMA interrupts */
		DMAy_Channelx->CTL_LOW |= DMA_IT;
  }
  else
  {
    /* Disable the selected DMA interrupts */
		DMAy_Channelx->CTL_LOW &= ~DMA_IT;
  }
	switch (*(uint32_t*)&DMAy_Channelx)
		{
    case DMA1_Channel1_BASE:  
		  /* Enable the DMA1 selected  channel1 interrupts */
      DMA1->MaskTfr_LOW = (DMA1_Channel1_IT_Mask<<8)|DMA1_Channel1_IT_Mask;
      break;
    case DMA1_Channel2_BASE:
			 /* Enable the DMA1 selected  channel2 interrupts */
     DMA1->MaskTfr_LOW = (DMA1_Channel2_IT_Mask<<8)|DMA1_Channel2_IT_Mask;
      break;
    case DMA1_Channel3_BASE:
			 /* Enable the DMA1 selected  channel3 interrupts */
      DMA1->MaskTfr_LOW = (DMA1_Channel3_IT_Mask<<8)|DMA1_Channel3_IT_Mask;
      break;
    case DMA1_Channel4_BASE:
			 /* Enable the DMA1 selected  channel4 interrupts */
      DMA1->MaskTfr_LOW = (DMA1_Channel4_IT_Mask<<8)|DMA1_Channel4_IT_Mask;
      break;
    case DMA1_Channel5_BASE:
			 /* Enable the DMA1 selected  channel5 interrupts */
      DMA1->MaskTfr_LOW = (DMA1_Channel5_IT_Mask<<8)|DMA1_Channel5_IT_Mask;
      break;
    case DMA1_Channel6_BASE:
			 /* Enable the DMA1 selected  channel6 interrupts */
      DMA1->MaskTfr_LOW = (DMA1_Channel6_IT_Mask<<8)|DMA1_Channel6_IT_Mask;
      break;
    case DMA1_Channel7_BASE:
			 /* Enable the DMA1 selected  channel7 interrupts */
      DMA1->MaskTfr_LOW = (DMA1_Channel7_IT_Mask<<8)|DMA1_Channel7_IT_Mask;
      break;

    default:
			 DMA1->MaskTfr_LOW = CH_MASK_Reset;
      break;
  }
}


/**
  * @brief  Checks whether the specified DMAy Channelx flag is set or not.
  * @param DMA_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  * @arg DMA1_FLAG_TC1: DMA1 Channel1 transfer complete flag.       
  * @arg DMA1_FLAG_TC2: DMA1 Channel2 transfer complete flag.        
  * @arg DMA1_FLAG_TC3: DMA1 Channel3 transfer complete flag.            
  * @arg DMA1_FLAG_TC4: DMA1 Channel4 transfer complete flag.         
  * @arg DMA1_FLAG_TC5: DMA1 Channel5 transfer complete flag.       
  * @arg DMA1_FLAG_TC6: DMA1 Channel6 transfer complete flag.             
  * @arg DMA1_FLAG_TC7: DMA1 Channel7 transfer complete flag. 
  * @retval : The new state of DMA_FLAG (SET or RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_DMA_GET_FLAG(DMA_FLAG));
  
	tmpreg = DMA1->RawTf_LOW;
  /* Check the status of the specified DMA flag */
  if ((tmpreg & DMA_FLAG) != (uint32_t)RESET)
  {
    /* DMA_FLAG is set */
    bitstatus = SET;
  }
  else
  {
    /* DMA_FLAG is reset */
    bitstatus = RESET;
  }
  
  /* Return the DMA_FLAG status */
  return  bitstatus;
}

/**
  * @brief  Clears the DMAy Channelx's pending flags.
  * @param DMA_FLAG: specifies the flag to clear.
  *   This parameter can be any combination (for the same DMA) of 
  *   the following values:
  * @arg DMA1_FLAG_TC1: DMA1 Channel1 transfer complete flag.       
  * @arg DMA1_FLAG_TC2: DMA1 Channel2 transfer complete flag.        
  * @arg DMA1_FLAG_TC3: DMA1 Channel3 transfer complete flag.            
  * @arg DMA1_FLAG_TC4: DMA1 Channel4 transfer complete flag.         
  * @arg DMA1_FLAG_TC5: DMA1 Channel5 transfer complete flag.       
  * @arg DMA1_FLAG_TC6: DMA1 Channel6 transfer complete flag.             
  * @arg DMA1_FLAG_TC7: DMA1 Channel7 transfer complete flag. 
  * @retval : None
  */
void DMA_ClearFlag(uint32_t DMA_FLAG)
{
  /* Check the parameters */
  assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));
 
	DMA1->ClearTfr_LOW = DMA_FLAG;
	DMA1->ClearBlock_LOW	= DMA_FLAG;
	DMA1->ClearSrcTran_LOW = DMA_FLAG;
	DMA1->ClearDstTran_LOW = DMA_FLAG;
  DMA1->ClearErr_LOW = DMA_FLAG;
}

/**
  * @brief  Checks whether the specified DMAy Channelx interrupt has 
  *   occurred or not.
  * @param DMA_IT: specifies the DMA interrupt source to check. 
  *   This parameter can be one of the following values:
  * @arg DMA1_IT_TC1: DMA1 Channel1 transfer complete interrupt.  
* @arg DMA1_IT_TC2: DMA1 Channel2 transfer complete interrupt.  
* @arg DMA1_IT_TC3: DMA1 Channel3 transfer complete interrupt.  
* @arg DMA1_IT_TC4: DMA1 Channel4 transfer complete interrupt.  
* @arg DMA1_IT_TC5: DMA1 Channel5 transfer complete interrupt.  
* @arg DMA1_IT_TC6: DMA1 Channel6 transfer complete interrupt.  
* @arg DMA1_IT_TC7: DMA1 Channel7 transfer complete interrupt.  
  * @retval : The new state of DMA_IT (SET or RESET).
  */
ITStatus DMA_GetITStatus(uint32_t DMA_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_DMA_GET_IT(DMA_IT));
 	
	tmpreg = DMA1->RawTf_LOW;
  /* Check the status of the specified DMA flag */
  if ((tmpreg & DMA_IT) != (uint32_t)RESET)
  {
    /* DMA_IT is set */
    bitstatus = SET;
  }
  else
  {
    /* DMA_IT is reset */
    bitstatus = RESET;
  }
  
  /* Return the DMA_IT status */
  return  bitstatus;
}

/**
  * @brief  Clears the DMAy Channelx’s interrupt pending bits.
  * @param DMA_IT: specifies the DMA interrupt pending bit to clear.
  *   This parameter can be any combination (for the same DMA) of
  *   the following values:
  * @arg DMA1_IT_TC1: DMA1 Channel1 transfer complete interrupt.  
* @arg DMA1_IT_TC2: DMA1 Channel2 transfer complete interrupt.  
* @arg DMA1_IT_TC3: DMA1 Channel3 transfer complete interrupt.  
* @arg DMA1_IT_TC4: DMA1 Channel4 transfer complete interrupt.  
* @arg DMA1_IT_TC5: DMA1 Channel5 transfer complete interrupt.  
* @arg DMA1_IT_TC6: DMA1 Channel6 transfer complete interrupt.  
* @arg DMA1_IT_TC7: DMA1 Channel7 transfer complete interrupt.  
  * @retval : None
  */
void DMA_ClearITPendingBit(uint32_t DMA_IT)
{
  /* Check the parameters */
  assert_param(IS_DMA_CLEAR_IT(DMA_IT));
	
	DMA1->ClearTfr_LOW = DMA_IT;
	DMA1->ClearBlock_LOW	= DMA_IT;
	DMA1->ClearSrcTran_LOW = DMA_IT;
	DMA1->ClearDstTran_LOW = DMA_IT;
  DMA1->ClearErr_LOW = DMA_IT;
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
