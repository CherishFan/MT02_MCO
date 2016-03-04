


/* Includes ------------------------------------------------------------------*/
#include "all.h"


void Comp_Config(void)
{
	uint32_t outState =0;
	
	COMP_InitTypeDef COMP_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_3_4VREFINT; //COMP_InvertingInput_VREFINT;  //;COMP_InvertingInput_IO1
  COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO2;
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_BlankingSrce = COMP_BlankingSrce_None;
  COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_No;
  COMP_InitStructure.COMP_Mode = COMP_Mode_UltraLowPower;
	COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);
	
	COMP_Cmd(COMP_Selection_COMP1, ENABLE);
	while(1)
	{
		outState = COMP_GetOutputLevel(COMP_Selection_COMP1);
		//outState = COMP1->CSR;
		GPIOB->ODR = (outState>>30);

		uart_printf("outState=0x%x\r\n",(outState>>30));
		//uart_printf("GPIOB->ODR=0x%x\r\n",GPIOB->ODR);
	}
}


