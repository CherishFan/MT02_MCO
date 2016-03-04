


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
	//ʹ��������ж�
	// ʹ��SEVONPEND
}

void exti_PA0_Falling()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOB�˿�
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOC�˿�
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //����GPIOD�˿�

	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	AFIO->EXTICR[0] = 0x0; //PA0 pin generate EXTI0
	
	EXTI->IMR = 0x01; //21���ж��ߵ�����λ  PA0 
	EXTI->FTSR = 0x01; //�ж��ߵĴ���ѡ��
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}