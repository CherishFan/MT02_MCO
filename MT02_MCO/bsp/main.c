/****************************************Copyright (c)****************************************************
** 
**                                      
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			main.c
** modified Date:  		2013-6-20
** Last Version:		V0.1
** Descriptions:		MT01  main ��������
**	
*********************************************************************************************************/
#include "all.h"
 
///********************************************************************************************************
//**������Ϣ ��int main (void)                          
//**�������� ��������ARMLED����
//**������� ��
//**������� ��
//********************************************************************************************************/
//#define RCC_PLLMul_17                    ((uint32_t)0x00380000)

int main(void)
{

	unsigned int addr ;
	unsigned int dn = 0;
	unsigned int dm = 0;
	__IO unsigned int tmp; 
	unsigned int i;
//  GPIO_LedTest();
	
// 	  SystemClk_HSEInit();
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
  	RCC_PLLDMDNConfig(RCC_PLLSource_HSI_Div4, 1, 1);
	
//  	SystemClkPll(0);
	
//RCC_MCO_PLLCLK_Div2
//	RCC_MCOConfig(RCC_MCO_SYSCLK);
	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);

//FLASH_Unlock();
//FLASH_EraseOptionBytes();
//FLASH_Unlock();
//FLASH_ProgramHalfWord(0x1FFFF802, 0x1FE);
//tmp = *(u32 *)0x1FFFF800;

  while(1)              //����ѭ��
	{
		
		
	}
	
}



