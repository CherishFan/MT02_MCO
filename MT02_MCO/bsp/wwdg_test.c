


/* Includes ------------------------------------------------------------------*/
#include "all.h"

void WWDG_IRQHandler (void)
{	
	if(WWDG_GetFlagStatus())
	{
		WWDG_ClearFlag();
		WWDG_SetCounter(0x66);
	}
	uart_printf("WWDG interrupt OK!\r\n");	
}


void Wwdg_reset_test(void)
{
	unsigned short value = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	uart_printf("RCC->CSR=0x%x\r\n",RCC->CSR);
	uart_printf("WWDG->CR=0x%x\r\n",WWDG->CR);
	uart_printf("WWDG->CFR=0x%x\r\n",WWDG->CFR);  
  uart_printf("WWDG->SR=0x%x\r\n",WWDG->SR); 	
  
	uart_printf("IWDG->PR=0x%x\r\n",IWDG->PR); 
	uart_printf("IWDG->RLR=0x%x\r\n",IWDG->RLR); 
	uart_printf("IWDG->SR=0x%x\r\n",IWDG->SR); 
	
	WWDG_Enable(0x66);
	
	value = WWDG->CR;
	uart_printf("WWDG->CR=0x%x\r\n",WWDG->CR);
	uart_printf("RCC1->CSR=0x%x\r\n",RCC->CSR);
	uart_printf("RCC2->CSR=0x%x\r\n",RCC->CSR);
}


void Wwdg_ExterslReloadReset_test(void)
{
	unsigned short value = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	uart_printf("RCC->CSR=0x%x\r\n",RCC->CSR);
	uart_printf("WWDG->CR=0x%x\r\n",WWDG->CR);
	uart_printf("WWDG->CFR=0x%x\r\n",WWDG->CFR);  
  uart_printf("WWDG->SR=0x%x\r\n",WWDG->SR); 	
  
	uart_printf("IWDG->PR=0x%x\r\n",IWDG->PR); 
	uart_printf("IWDG->RLR=0x%x\r\n",IWDG->RLR); 
	uart_printf("IWDG->SR=0x%x\r\n",IWDG->SR); 
	
	WWDG_Enable(0x66);
	WWDG_SetWindowValue(0x77);
	
	value = WWDG->CR;
	uart_printf("WWDG->CR=0x%x\r\n",WWDG->CR);
	uart_printf("RCC1->CSR=0x%x\r\n",RCC->CSR);
	uart_printf("RCC2->CSR=0x%x\r\n",RCC->CSR);
}


void Wwdg_irq_test(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	unsigned short value = 0;
	
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	uart_printf("RCC->CSR=0x%x\r\n",RCC->CSR);
	
	WWDG_EnableIT();
	WWDG_Enable(0x66);
	
	
}

void WWDG_Register_test()
{
	uart_printf("WWDG->CR=0x%x\r\n",WWDG->CR);  //0x7f
	uart_printf("WWDG->CFR=0x%x\r\n",WWDG->CFR);  //0x7f
  uart_printf("WWDG->SR=0x%x\r\n",WWDG->SR); 	//0x0
}