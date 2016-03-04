


/* Includes ------------------------------------------------------------------*/
#include "all.h"

void EXTI0_IRQHandler (void)
{	
	//if(EXTI_GetFlagStatus(EXTI_Line0))
	//{
		EXTI_ClearFlag(EXTI_Line0);
	//}
	uart_printf("Exti PA0 interrupt OK!\r\n");	
}

void Exti_test(void)
{
	//使能外设的中断
	// 使能SEVONPEND
}

void exti_PA0_Falling()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //开启GPIOD端口

	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	AFIO->EXTICR[0] = 0x0; //PA0 pin generate EXTI0
	
	EXTI->IMR = 0x01; //21个中断线的屏蔽位  PA0 
	EXTI->FTSR = 0x01; //中断线的触发选择
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}