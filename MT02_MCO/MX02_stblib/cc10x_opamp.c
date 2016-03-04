/**
  ******************************************************************************
  * @file    cc10x_opamp.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    14-04-2014
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the operational amplifiers (OPAMP1,...OPAMP4) peripheral: 
  *           + OPAMP Configuration
  *           + OPAMP calibration
  *
  @verbatim
  
  ==============================================================================
                        ##### OPAMP Peripheral Features #####
  ==============================================================================
                   
  [..]
      The device integrates 4 operational amplifiers OPAMP1, OPAMP2, OPAMP3 and OPAMP4:
              
      (+) The OPAMPs non inverting input can be selected among the list shown by
          table below.
  
      (+) The OPAMPs inverting input can be selected among the list shown by
          table below.
  
      (+) The OPAMPs outputs can be internally connected to the inverting input 
          (follower mode)
      (+) The OPAMPs outputs can be internally connected to resistor feedback
          output (Programmable Gain Amplifier mode)
  
      (+) The OPAMPs outputs can be internally connected to ADC
  
      (+) The OPAMPs can be calibrated to compensate the offset compensation
  
      (+) Timer-controlled Mux for automatic switch of inverting and
          non-inverting input
  
  OPAMPs inverting/non-inverting inputs:
    +--------------------------------------------------------------+     
    |                 |        | OPAMP1 | OPAMP2 | OPAMP3 | OPAMP4 |
    |-----------------|--------|--------|--------|--------|--------|
    |                 | IO1    |  PA7   |  PC5   |  PA5   |  PA4   |
    | Inverting Input | IO2    |  PA5   |  PA5   |  PA4   |  PA5   |
    |                 | IO3    |  PA3   |  PA3   |  PA1   |  PA6   |
    |                 | IO4    |  PA1   |  PA1   |  PB0   |  PA7   |
    |-----------------|--------|--------|--------|--------|--------|
    |                 | IO1    |  PA7   |  PA5   |  PA3   |  PC4   |
    |  Non Inverting  | IO2    |  PA5   |  PB0   |  PA5   |  PA4   |
    |    Input        | IO3    |  PA3   |  PB1   |  PA1   |  PB0   |
    |                 | IO4    |  PA1   |  PA4   |  PB0   |  PB1   |
    +--------------------------------------------------------------+  
  
                        ##### How to use this driver #####
  ==============================================================================
  [..]
  This driver provides functions to configure and program the OPAMP 
  of all STM32F30x devices.
  
  To use the OPAMP, perform the following steps:
 
  (#) Enable the SYSCFG APB clock to get write access to OPAMP
      register using RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  (#) Configure the OPAMP input in analog mode using GPIO_Init()
  
  (#) Configure the OPAMP using OPAMP_Init() function:
      (++) Select the inverting input
      (++) Select the non-inverting inverting input
    
  (#) Enable the OPAMP using OPAMP_Cmd() function
    
  @endverbatim
      
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 Mindmotion Microelectronics</center></h2>
  *
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "cc10x_opamp.h"

/** @addtogroup cc10x_StdPeriph_Driver
  * @{
  */

/** @defgroup OPAMP 
  * @brief OPAMP driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define OPAMP_CSR_DEFAULT_MASK                    ((uint32_t)0xC3C3C3C3)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup OPAMP_Private_Functions
  * @{
  */

/** @defgroup OPAMP_Group1 Initialization and Configuration functions
 *  @brief   Initialization and Configuration functions 
 *
@verbatim   
 ===============================================================================
             ##### Initialization and Configuration functions #####
 ===============================================================================  

@endverbatim
  * @{
  */
   
/**
  * @brief  Deinitializes OPAMP peripheral registers to their default reset values.
  * @note   Deinitialization can't be performed if the OPAMP configuration is locked.
  *         To unlock the configuration, perform a system reset.
  * @param  OPAMP_Selection: the selected OPAMP. 
  *          This parameter can be OPAMP_Selection_OPAMPx where x can be 1 to 4
  *          to select the OPAMP peripheral.
  * @param  None
  * @retval None
  */
void OPAMP_DeInit(uint32_t OPAMP_Selection)
{
  /*!< Set OPAMP_CSR register to reset value */
  OPAMP->CSR = ((uint32_t)0x00000000);
}

/**
  * @brief  Initializes the OPAMP peripheral according to the specified parameters
  *         in OPAMP_InitStruct
  * @note   If the selected OPAMP is locked, initialization can't be performed.
  *         To unlock the configuration, perform a system reset.
  * @param  OPAMP_Selection: the selected OPAMP. 
  *          This parameter can be OPAMP_Selection_OPAMPx where x can be 1 to 4
  *          to select the OPAMP peripheral.
  * @param  OPAMP_InitStruct: pointer to an OPAMP_InitTypeDef structure that contains 
  *         the configuration information for the specified OPAMP peripheral.
  *           - OPAMP_InvertingInput specifies the inverting input of OPAMP
  *           - OPAMP_NonInvertingInput specifies the non inverting input of OPAMP
  * @retval None
  */
void OPAMP_Init(uint32_t OPAMP_Selection, OPAMP_InitTypeDef* OPAMP_InitStruct)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_OPAMP_ALL_PERIPH(OPAMP_Selection));
  assert_param(IS_OPAMP_INVERTING_INPUT(OPAMP_InitStruct->OPAMP_InvertingInput));
  assert_param(IS_OPAMP_NONINVERTING_INPUT(OPAMP_InitStruct->OPAMP_NonInvertingInput));

  /*!< Get the OPAMPx_CSR register value */
  tmpreg = OPAMP->CSR;

  /*!< Clear the inverting and non inverting bits selection bits */
  tmpreg &= (uint32_t) (OPAMP_CSR_DEFAULT_MASK);

  /*!< Configure OPAMP: inverting and non inverting inputs */
  tmpreg |= (uint32_t)(OPAMP_InitStruct->OPAMP_InvertingInput | OPAMP_InitStruct->OPAMP_NonInvertingInput);

  /*!< Write to OPAMPx_CSR register */
  OPAMP->CSR = tmpreg;
}

/**
  * @brief  Fills each OPAMP_InitStruct member with its default value.
  * @param  OPAMP_InitStruct: pointer to an OPAMP_InitTypeDef structure which will 
  *         be initialized.
  * @retval None
  */
void OPAMP_StructInit(OPAMP_InitTypeDef* OPAMP_InitStruct)
{
  OPAMP_InitStruct->OPAMP_NonInvertingInput = OPAMP_NonInvertingInput_IO1;
  OPAMP_InitStruct->OPAMP_InvertingInput = OPAMP_InvertingInput_IO1;
}


/**
  * @brief  Enable or disable the OPAMP peripheral.
  * @note   If the selected OPAMP is locked, enable/disable can't be performed.
  *         To unlock the configuration, perform a system reset.
  * @param  OPAMP_Selection: the selected OPAMP. 
  *          This parameter can be OPAMP_Selection_OPAMPx where x can be 1 to 4
  *          to select the OPAMP peripheral.
  * @param  NewState: new state of the OPAMP peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void OPAMP_Cmd(uint32_t OPAMP_Selection, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_OPAMP_ALL_PERIPH(OPAMP_Selection));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
		switch(OPAMP_Selection)
		{
			case OPAMP_Selection_OPAMP1:
						/* Enable the selected OPAMPx peripheral */
						OPAMP->CSR |= (uint32_t) (OPAMP1_CSR_OPAMP1EN);
						break;
			case OPAMP_Selection_OPAMP2:
						/* Enable the selected OPAMPx peripheral */
						OPAMP->CSR |= (uint32_t) (OPAMP1_CSR_OPAMP2EN);
						break;
			case OPAMP_Selection_OPAMP3:
						/* Enable the selected OPAMPx peripheral */
						OPAMP->CSR |= (uint32_t) (OPAMP1_CSR_OPAMP3EN);
						break;
			case OPAMP_Selection_OPAMP4:
						/* Enable the selected OPAMPx peripheral */
						OPAMP->CSR |= (uint32_t) (OPAMP1_CSR_OPAMP4EN);
						break;
			default:
						break;
		}
  }
  else
  {
		switch(OPAMP_Selection)
		{
			case OPAMP_Selection_OPAMP1:
						 /* Disable the selected OPAMPx peripheral */
						OPAMP->CSR &= (uint32_t) (~OPAMP1_CSR_OPAMP1EN);
						break;
			case OPAMP_Selection_OPAMP2:
						 /* Disable the selected OPAMPx peripheral */
						OPAMP->CSR &= (uint32_t) (~OPAMP1_CSR_OPAMP2EN);
						break;
			case OPAMP_Selection_OPAMP3:
						 /* Disable the selected OPAMPx peripheral */
						OPAMP->CSR &= (uint32_t) (~OPAMP1_CSR_OPAMP3EN);
						break;
			case OPAMP_Selection_OPAMP4:
						 /* Disable the selected OPAMPx peripheral */
						OPAMP->CSR &= (uint32_t) (~OPAMP1_CSR_OPAMP4EN);
						break;
			default:
						break;
		}
  }
}


/************************ (C) COPYRIGHT Mindmotion Microelectronics *****END OF FILE****/
