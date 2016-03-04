


/* Includes ------------------------------------------------------------------*/
#include "all.h"

void BkpRtc_ReadWrite()
{
	uint32_t temp = 0;
	uint16_t i;
	uint16_t readdata = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	RCC->BDCR |= 0x1<<16; 
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	RCC->BDCR = RCC->BDCR & 0xfffeffff; 
	
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	PWR_BackupAccessCmd(ENABLE);
	uart_printf("PWR->CR=0x%x\r\n",PWR->CR);
	RCC_LSEConfig(RCC_LSE_ON);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	RCC_RTCCLKCmd(ENABLE);
	
	BKP->DR1 = 0xff66;
	
	BKP->DR2 = 0xff88;
	
	BKP->DR3 = 0xff99;
	uart_printf("BKP->DR3=0x%x\r\n",BKP->DR3);
	BKP->DR4 = 0xffaa;
	uart_printf("BKP->DR4=0x%x\r\n",BKP->DR4);
	BKP->DR5 = 0xffbb;
	uart_printf("BKP->DR5=0x%x\r\n",BKP->DR5);
	BKP->RTCCR = 0xf7;
	uart_printf("BKP->RTCCR=0x%x\r\n",BKP->RTCCR);
	BKP->CR = 0x02;
	uart_printf("BKP->CR=0x%x\r\n",BKP->CR);
	
	
	RTC_WaitForLastTask();
	
	RTC_SetPrescaler(0x8000);
	
	RTC_EnterConfigMode();

	RTC_SetCounter(0x05555);
	
	RTC_WaitForLastTask();
	
	
	
	uart_printf("RTC->CRL=0x%x\r\n",RTC->CRL);
	uart_printf("BKP->CSR=0x%x\r\n",BKP->CSR);
	uart_printf("BKP->DR1=0x%x\r\n",BKP->DR1);
	uart_printf("BKP->DR2=0x%x\r\n",BKP->DR2);
	uart_printf("BKP->DR3=0x%x\r\n",BKP->DR3);
	uart_printf("BKP->DR4=0x%x\r\n",BKP->DR4);
	uart_printf("BKP->DR5=0x%x\r\n",BKP->DR5);
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	
	uart_printf("RTC->PRLH=0x%x\r\n",RTC->PRLH);	
	uart_printf("RTC->PRLL=0x%x\r\n",RTC->PRLL);
	uart_printf("RTC->DIVH=0x%x\r\n",RTC->DIVH);	
	uart_printf("RTC->DIVL=0x%x\r\n",RTC->DIVL);	
	
	while(1)
	{
		 Delay(1000000);
	temp = RTC_GetCounter();
	uart_printf("RTC->COUNT=0x%x\r\n",temp);
	}
}

void BkpRtc_readonly(void) 
{
	uint32_t temp = 0;
	uint16_t i;
	uint16_t readdata = 0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	PWR_BackupAccessCmd(ENABLE);
	uart_printf("PWR->CR=0x%x\r\n",PWR->CR);
	RCC_LSEConfig(RCC_LSE_ON);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	RCC_RTCCLKCmd(ENABLE);
	
	uart_printf("RTC->CRL=0x%x\r\n",RTC->CRL);
	uart_printf("BKP->CSR=0x%x\r\n",BKP->CSR);
	
	uart_printf("BKP->DR1=0x%x\r\n",BKP->DR1);//0x0
	uart_printf("BKP->DR2=0x%x\r\n",BKP->DR2);//0x0
	uart_printf("BKP->DR3=0x%x\r\n",BKP->DR3);//0x0
	uart_printf("BKP->DR4=0x%x\r\n",BKP->DR4);//0x0
	uart_printf("BKP->DR5=0x%x\r\n",BKP->DR5);//0x0
	uart_printf("BKP->DR6=0x%x\r\n",BKP->DR6);//0x0
	uart_printf("BKP->DR7=0x%x\r\n",BKP->DR7);//0x0
	uart_printf("BKP->DR8=0x%x\r\n",BKP->DR8);//0x0
	uart_printf("BKP->DR9=0x%x\r\n",BKP->DR9);//0x0
	uart_printf("BKP->DR10=0x%x\r\n",BKP->DR10);//0x0
	
	uart_printf("BKP->RTCCR=0x%x\r\n",BKP->RTCCR);//0x0
	
	uart_printf("RCC->BDCR=0x%x\r\n",RCC->BDCR);
	
	uart_printf("RTC->PRLH=0x%x\r\n",RTC->PRLH);	
	uart_printf("RTC->PRLL=0x%x\r\n",RTC->PRLL);
	uart_printf("RTC->DIVH=0x%x\r\n",RTC->DIVH);	
	uart_printf("RTC->DIVL=0x%x\r\n",RTC->DIVL);	
	
	while(1)
	{
		 Delay(1000000);
	temp = RTC_GetCounter();
	uart_printf("RTC->COUNT=0x%x\r\n",temp);
	}
}

void BkpRtc_register_test()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	
	uart_printf("BKP->DR1=0x%x\r\n",BKP->DR1);
	uart_printf("BKP->DR2=0x%x\r\n",BKP->DR2);
	uart_printf("BKP->DR3=0x%x\r\n",BKP->DR3);
	uart_printf("BKP->DR4=0x%x\r\n",BKP->DR4);
	uart_printf("BKP->DR5=0x%x\r\n",BKP->DR5);
	uart_printf("BKP->DR6=0x%x\r\n",BKP->DR6);//0x0
	uart_printf("BKP->DR7=0x%x\r\n",BKP->DR7);//0x0
	uart_printf("BKP->DR8=0x%x\r\n",BKP->DR8);//0x0
	uart_printf("BKP->DR9=0x%x\r\n",BKP->DR9);//0x0
	uart_printf("BKP->DR10=0x%x\r\n",BKP->DR10);//0x0
	
	uart_printf("BKP->RTCCR=0x%x\r\n",BKP->RTCCR);//0x0
	uart_printf("BKP->CR=0x%x\r\n",BKP->CR);//0x0
	uart_printf("BKP->CSR=0x%x\r\n",BKP->CSR);
	
	uart_printf("RTC->CRL=0x%x\r\n",RTC->CRL); //0x20
	uart_printf("RTC->CRH=0x%x\r\n",RTC->CRH); //0x0
	uart_printf("RTC->PRLH=0x%x\r\n",RTC->PRLH);//0x0	
	uart_printf("RTC->PRLL=0x%x\r\n",RTC->PRLL);//0x8000
	uart_printf("RTC->DIVH=0x%x\r\n",RTC->DIVH);//0x0
	uart_printf("RTC->DIVL=0x%x\r\n",RTC->DIVL); //0x8000	
	uart_printf("RTC->CNTH=0x%x\r\n",RTC->CNTH); //0x0
	uart_printf("RTC->CNTL=0x%x\r\n",RTC->CNTL); //0x0
	uart_printf("RTC->ALRH=0x%x\r\n",RTC->ALRH); //0xffff
	uart_printf("RTC->ALRL=0x%x\r\n",RTC->ALRL); //0xffff
	
}


void TAMPER_IRQHandler (void)
{	
	if(BKP_GetITStatus())
	{
		BKP_ClearITPendingBit();
		
	}
	uart_printf("TAMPER interrupt OK!\r\n");	
}


void Tamper_check_test()
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
  //GPIOC 13 作为侵入检测引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TAMPER_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	
	PWR_BackupAccessCmd(ENABLE);
	
	BKP_TamperPinLevelConfig(BKP_TamperPinLevel_High); //BKP_CR TPAL bit
	BKP_ITConfig(ENABLE); //BKP_CSR  TPIE bit enable
	BKP_TamperPinCmd(ENABLE); //BKP_CR TPE bit enable
}