
#include "all.h"


/********************************************************************************************************
**������Ϣ ��SystemClk_HSEInit (void)                        
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/

void SystemClk_HSEInit(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//HSE on
	//CR�Ĵ���BIT16λ��HSEONλ����1�������������ⲿʱ��HSE��Ϊϵͳʱ��
	RCC_HSEConfig(RCC_HSE_ON);
	while(1)
	{
		if(RCC_WaitForHSEStartUp()!=0)
		{
			break;
		}
	}
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);//ѡ���ⲿʱ����Ϊϵͳʱ��
	
// 	RCC_MCOConfig(RCC_MCO_SYSCLK);  //ͨ��PA8 pin �۲�Ƶ��
}

/********************************************************************************************************
**������Ϣ ��SystemClk_HSIInit (void)                        
**�������� ��ϵͳʱ�ӳ�ʼ����������ʼ��֮ǰ�ȸ�λ����ʱ��
**������� ����
**������� ����
********************************************************************************************************/

void SystemClk_HSIInit()
{
	unsigned int temp;
	
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
// 	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
// 	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//HSI on
	//RCC->CR |= (uint32_t)0x00000001;  //CR�Ĵ���BIT0λ��HSIONλ����1�������������ڲ�ʱ��HSI��Ϊϵͳʱ��
                                      //��������Ϊ���±ߵĲ�����׼��
	
	RCC_DeInit();
	//RCC->CR = (uint32_t)0x00000001;
	RCC_HSICmd(ENABLE);
	
	//RCC->CFGR = 0x00000000; //HSI CLK ��Ϊϵͳʱ�����
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//ѡ���ڲ�ʱ����Ϊϵͳʱ��
	RCC_MCOConfig(RCC_MCO_SYSCLK);  //ͨ��PA8 pin �۲�Ƶ��
}


/********************************************************************************************************
**������Ϣ ��SystemClkPll(unsigned int pllmul)                
**�������� ����ƵƵ����Ϊϵͳ����ʱ��
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16 
**������� ����
********************************************************************************************************/

void SystemClkPll(unsigned int pllmul)
{
	
	RCC_PLLCmd(ENABLE); //ʹ��PLL

	while(1)
	{
		if(RCC->CR&0x02000000) //��ѯPLL ready
		{
			
			break;
		}
	}
	//FLASH->ACR |= 0x02;
// 	FLASH_SetLatency(FLASH_Latency_2);//ʱ�ӵȴ�״̬ 
	
	//RCC->CFGR = 0x07010002; //PLL/2 PA8���
	//RCC->CFGR = 0x04010002; //PLL PA8���
//  	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//ѡ��PLLʱ����Ϊϵͳʱ��
 	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //ͨ��PA8 pin �۲�Ƶ��
}

 
/********************************************************************************************************
**������Ϣ ��HSE_Pll_test(unsigned int pllmul)                      
**�������� ���ⲿ������Ϊʱ��Դ
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16 
**������� ����
********************************************************************************************************/

void HSE_Pll_test(unsigned int pllmul)
{
	unsigned int n,m;
	SystemClk_HSEInit();
	//PLL ����
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul); //ѡ���ⲿʱ��Դ��Ϊ��Ƶʱ�ӵĻ���
	SystemClkPll(pllmul);
}

/********************************************************************************************************
**������Ϣ ��HSE_Pll_test(unsigned int pllmul)                      
**�������� ���ⲿ������Ϊʱ��Դ
**������� ����Ƶ��������ѡRCC_PLLMul_2 �� RCC_PLLMul_16 
**������� ����
********************************************************************************************************/

void HSI_Pll_test(unsigned int pllmul)
{
	unsigned int n,m;
	SystemClk_HSIInit();
	//PLL ����
	RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul); //ѡ���ڲ�ʱ��Դ��Ϊ��Ƶʱ�ӵĻ���
	SystemClkPll(pllmul);
}
 
/********************************************************************************************************
**������Ϣ ��LSI_clk()                      
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 40KHz
**������� ����
**������� ����
********************************************************************************************************/

void LSI_clk()
{
	unsigned int temp;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//RCC->CSR |= 0x01;   //open lsi on
	RCC_LSICmd(ENABLE);
	
	while(1)
	{
		if(RCC->CSR&0x00000002) //��ѯ�ڲ���Ƶʱ��ready
		{
			break;
		}
	}
	//RCC->CFGR = 0x02000000; //LSI CLK ��Ϊϵͳʱ�����   
  RCC_MCOConfig(RCC_MCO_LSI);  //ͨ��PA8 pin �۲�Ƶ��
}

 
/********************************************************************************************************
**������Ϣ ��LSE_clk()                      
**�������� ���ڲ���Ƶʱ�Ӳ�ͨ��ʾ�����۲�PA8 pin 32.786KHz
**������� ����
**������� ����
********************************************************************************************************/

void LSE_clk()
{
	unsigned int temp;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE); //ʹ��PWR_CR�е�DBPbit
	
	RCC_LSEConfig(RCC_LSE_ON);
	
	while(1)
	{
		if(RCC->BDCR&0x00000002) //��ѯ�ⲿ32.768KHz��Ƶʱ��ready
		{
			break;
		}
	}
	
	RCC_MCOConfig(RCC_MCO_LSE);  //ͨ��PA8 pin �۲�Ƶ��
}

/********************************************************************************************************
**������Ϣ ��HSE_HSI_Change()                      
**�������� ���ⲿʱ�����ڲ�ʱ��֮����л�
**������� ����
**������� ����
********************************************************************************************************/

void HSE_HSI_Change()
{
	unsigned char i;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOC�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	SystemClk_HSEInit();
	
	FLASH_Unlock();
	FLASH_EraseOptionBytes();
	FLASH_UserOptionByteConfig(OB_IWDG_HW, OB_STOP_NoRST, OB_STDBY_NoRST);
	
	RCC_ClockSecuritySystemCmd(ENABLE); //CSS on
	for(i=0;i<5;i++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		Delay(300000);
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		Delay(300000);
	}
	RCC->CR &= 0xfffeffff; //close HESON
  for(i=0;i<20;i++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		Delay(300000);
		GPIO_SetBits(GPIOC, GPIO_Pin_6);
		Delay(300000);
	}
	
}

void NMI_Handler (void)
{
	unsigned char i;
	
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOC�˿�
	
	//RCC->CIR &= 0xff7fffff; //clearCSSC
    RCC->CIR |= 0x00800000; //clearCSSC
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	for(i=0;i<20;i++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		Delay(300000);
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		Delay(300000);
	}
}