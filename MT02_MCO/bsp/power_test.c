#include "all.h"

void EXTI1_IRQHandler(void)
{
	unsigned int i,j;
	
	EXTI_ClearITPendingBit(EXTI_Line1);				 //����жϱ�־
	for(j=0;j<100;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7����͵�ƽ������ARMLED	 
		for(i=0;i<300000;i++)
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7����͵�ƽ��Ϩ��ARMLED
		for(i=0;i<300000;i++)
		{}
	}
}



void Gpio_Exti_init()
{
	EXTI_InitTypeDef        EXTI_InitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;           //����NVIC��ʼ����Ҫ�����ݽṹ��
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOC�˿�
	
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7; //|GPIO_Pin_7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;          //EXTI0 �жϺ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //�����ȼ�����Ϊ0��ռ��0bit�������ȼ�������4bit��0-15��ѡ��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //����ԽС���ȼ�Խ�ߣ��������ȼ�һ�����������ܳ���4bit��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //���磺�������ȼ�Ϊ3��ռ��2bit�������ȼ�������2bit����ѡ����0-3
	NVIC_Init(&NVIC_InitStructure);   //����4������ǰ����õ����ݴ���NVIC_InitStructure����ṹ�壬�������ǰѽṹ�������д����Ӧ�Ĵ���

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

void GpioC7init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //����GPIOC�˿�
	
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7; //|GPIO_Pin_7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}


void AllClk_Enable()
{
	RCC->AHBENR = 0xff;
	//RCC->AHBENR = 0x55;
	RCC->APB2ENR = 0xfffff;
	RCC->APB1ENR = 0xffffffff;
}

void AllClk_Disable()
{
	RCC->AHBENR = 0x14;
	RCC->APB2ENR = 0x0;
	RCC->APB1ENR = 0x0;
}

void AHB_Prescale(unsigned int hpre)
{
	RCC->CFGR |= (hpre<<4);
}

void PVDTset()
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_PVDLevelConfig(PWR_PVDLevel_2V8);
	//PWR_PVDLevelConfig(PWR_PVDLevel_2V6);
	PWR_PVDCmd(ENABLE);
	while(1)
	{
		
		if(PWR_GetFlagStatus(PWR_FLAG_PVDO))
		{
			uart_printf("VDD<2.8V=PWR->CSR\r\n");
			break;
		}
	}
	uart_printf("PWR->CSR=0x%x\r\n",PWR->CSR);
	
	while(1)
	{
		if(PWR_GetFlagStatus(PWR_FLAG_PVDO)==0)
		{
			uart_printf("VDD>2.8V=PWR->CSR\r\n");
			break;
		}
	}
}

void sleep_test()
{
	unsigned int i,j;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//SCB->SCR = 0x0;
	//Gpio_Exti_init();
	GpioC7init();
	
	//GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6����͵�ƽ������ARMLED	
	///*
	for(j=0;j<3;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7����͵�ƽ������ARMLED	 
		for(i=0;i<300;i++)
		
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7���High��ƽ��Ϩ��ARMLED
		//for(i=0;i<10;i++)
		for(i=0;i<300;i++)
		{}
	}
	AHB_Prescale(0);//HPRE = sysclk
	//AHB_Prescale(15);//HPRE = sysclk/512
	//AllClk_Enable();
	AllClk_Disable();
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, ENABLE);
	/*
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	*/
	//__WFI();
	__WFE();
	
/*
	while(1)
	{
	uart_printf("EXTI->PR=0x%x\r\n",EXTI->PR);
	}
	
	*/
}

void DeepSleep_test()//ֹͣ״̬
{
	unsigned int i,j;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR->CR = PWR->CR & 0xfffd; //PDDS = 0;enter stop mode
	SCB->SCR |= 0x4;
	Gpio_Exti_init();
	
	for(j=0;j<5;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6����͵�ƽ������ARMLED	 
		for(i=0;i<300000;i++)
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //PA1����͵�ƽ��Ϩ��ARMLED
		for(i=0;i<300000;i++)
		{}
	}
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, DISABLE);
	//PWR->CR = PWR->CR |0x02;
	__WFI();
	
}

void Wait_test()//����״̬
{
	unsigned int i,j;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR->CR |= 0x0002; //PDDS = 1;enter wait mode
	SCB->SCR |= 0x4;
	Gpio_Exti_init();
	
	for(j=0;j<8;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6����͵�ƽ������ARMLED	 
		for(i=0;i<300000;i++)
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //PA1����͵�ƽ��Ϩ��ARMLED
		for(i=0;i<300000;i++)
		{}
	}
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, DISABLE);
	
	
	//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
//  	
//  	PWR_BackupAccessCmd(ENABLE); //ʹ��PWR_CR�е�DBPbit

// // 	RCC->BDCR = 0x1<<16; 
// // 	for(i=0;i<10;i++);
// //  	RCC->BDCR &= ~(0x1<<16);
// // 	for(i=0;i<10;i++);
//  	RCC_LSEConfig(RCC_LSE_ON);
// 	for(i=0;i<100;i++);
//  	RCC_LSEConfig(RCC_LSE_OFF);
	
	__WFI();
	
}

void wait_led(unsigned short gpio_pin,unsigned int number,unsigned int delay)
{
	unsigned int i,j;
	GPIO_InitTypeDef  GPIO_InitStructure;
		//wait for time
	for(i=0;i<delay;i++)
	{;}
			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
		GPIO_InitStructure.GPIO_Pin  =  gpio_pin; //GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
			
		for(j=0;j<number;j++)
		{
			GPIO_ResetBits(GPIOC, gpio_pin);	   //Pc6����͵�ƽ������ARMLED
			for(i=0;i<delay;i++)
			{;}
			GPIO_SetBits(GPIOC, gpio_pin);       //Pc6����ߵ�ƽ��Ϩ��ARMLED
			for(i=0;i<delay;i++)
			{;}
		}
}

void MCO_Output(unsigned int IoEn)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	if(IoEn)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
		GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // ���⸴�����
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		RCC->CFGR = 0x04000000; //HSI CLK ��Ϊϵͳʱ�����
	}
	else
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE); 
	}
		
}

void Hse_clk_set()
{
	unsigned int temp;
	//RCC->CR = RCC->CR | 0x00010000; //CR�Ĵ���BIT16λ��HSIONλ����1�������������ⲿʱ��HSE��Ϊϵͳʱ��
	RCC->CR = 0x00010000; 
		while(1)
		{
			temp = RCC->CR;
			if(temp & 0x00020000)
			{
				break;
			}
		}
	
		RCC->CFGR = 0x00000001; //HSE CLK ��Ϊϵͳʱ�����
}

void AllCLK_EN_sleep_test()
{
	unsigned int i,j;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//SCB->SCR = 0x0;
	//Gpio_Exti_init();
	GpioC7init();
	
	for(j=0;j<3;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7����͵�ƽ������ARMLED	 
		for(i=0;i<300000;i++)
		
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7���High��ƽ��Ϩ��ARMLED
		//for(i=0;i<10;i++)
		for(i=0;i<300000;i++)
		{}
	}
	//AHB_Prescale(0);//HPRE = sysclk
	//AHB_Prescale(15);//HPRE = sysclk/512
	AllClk_Enable();
	//AllClk_Disable();
	

	//__WFI();
	__WFE();

}

void AllCLK_DiS_sleep_test()
{
	unsigned int i,j;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//SCB->SCR = 0x0;
	//Gpio_Exti_init();
	GpioC7init();
	
	for(j=0;j<3;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7����͵�ƽ������ARMLED	 
		for(i=0;i<300000;i++)
		
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7���High��ƽ��Ϩ��ARMLED
		//for(i=0;i<10;i++)
		for(i=0;i<300000;i++)
		{}
	}
		AllClk_Disable();
	

	//__WFI();
	__WFE();
	

}
/*
void AllPowerTest(unsigned int Hsi,unsigned int Hse,unsigned int Pll_48MHz,unsigned int Pll_96MHz,unsigned int delay)
{
	unsigned int i;
	unsigned int number = 1;
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	//here if need reset all clk
	if(Hse)  //pc9 led��˸
	{
		//HSE�ⲿʱ�ӣ�io�ڲ���ʾƵ��
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE 
		wait_led(GPIO_Pin_9,number,delay); //led PC9 ��2��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ��
		number++;
		Hse_clk_set();
		MCO_Output(1);  //HSE + PA8
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��3��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Enable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		AllClk_Enable();
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��4��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		AllClk_Disable();
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��5��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��6��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		GPIO_LedTest(); 
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��7��
		
			//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);      
		I2C1MasterTest();
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��8��
		
		
			//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);         
		SPIM1_Test();   
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��9��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);         
		Tim1_CentAlignMode3_test3(TIM1,0x1111);   
		wait_led(GPIO_Pin_9,number,delay); //led PC9  ��10��
		
	}
	number = 0;
	if(Pll_48MHz)
	{
		//HSE�ⲿʱ�ӣ�io�ڲ���ʾƵ��
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE 
		SystemClkPll(0x2c100000); //48MHz
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��1��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ��
		number++;
		Hse_clk_set();
		MCO_Output(1);  //HSE + PA8
		SystemClkPll(0x2c100000); //48MHz
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��2��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Enable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0x2c100000); //48MHz
		AllClk_Enable();
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��3��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0x2c100000); //48MHz
		AllClk_Disable();
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��4��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz		
		AllClk_Disable();
		Uart1Init(USART1);
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��5��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz				
		AllClk_Disable();
		GPIO_LedTest(); 
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��6��
		
			//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz				
		AllClk_Disable();
		Uart1Init(USART1);      
		I2C1MasterTest();
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��7��
		
		
	//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz				
		AllClk_Disable();
		Uart1Init(USART1);         
		SPIM1_Test();   
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��8��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		SystemClkPll(0x2c100000); //48MHz		
		AllClk_Disable();
		Uart1Init(USART1);         
		Tim1_CentAlignMode3_test3(TIM1,0x1111);   
		wait_led(GPIO_Pin_8,number,delay); //led PC8  ��9��
		
	}
	
	number = 20;
	if(Pll_96MHz)
	{
		//HSE�ⲿʱ�ӣ�io�ڲ���ʾƵ��
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE 
		SystemClkPll(0xbc300000);  //96MHz
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��21��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ��
		number++;
		Hse_clk_set();
		MCO_Output(1);  //HSE + PA8
		SystemClkPll(0xbc300000);  //96MHz
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��22��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Enable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0xbc300000);  //96MHz
		AllClk_Enable();
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��23��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0xbc300000);  //96MHz
		AllClk_Disable();
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��24��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz	
		AllClk_Disable();
		Uart1Init(USART1);
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��25��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz			
		AllClk_Disable();
		GPIO_LedTest(); 
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��26��
		
			//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz	
		AllClk_Disable();
		Uart1Init(USART1);      
		I2C1MasterTest();
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��27��
		
		
	//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz	
		AllClk_Disable();
		Uart1Init(USART1);         
		SPIM1_Test();   
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��28��
		
		//HSE�ⲿʱ�ӣ�io����ʾƵ�ʣ�all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		SystemClkPll(0xbc300000);  //96MHz
		AllClk_Disable();
		Uart1Init(USART1);         
		Tim1_CentAlignMode3_test3(TIM1,0x1111);   
		wait_led(GPIO_Pin_7,number,delay); //led PC7  ��29��
		
	}
	number = 1;
	if(Hsi)
	{
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ��
		RCC->CR = (uint32_t)0x00000001; 
		wait_led(GPIO_Pin_6,number,delay); //led PC6  ��1��
		
		//�ڲ�ʱ�ӣ�io pa8 ��ʾƵ��
		number++;
		MCO_Output(1);  //HSI + MCO PA8
		wait_led(GPIO_Pin_6,number,delay); //led PC6  ��2��
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk,all clk Enable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Enable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   ��3��
		
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk/512,all clk Enable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Enable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   ��4��
		
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk,all clk Disable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Disable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   ��5��
		
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   ��6��
		
		
/-------------- sleep  test ----------------------------/
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk,all clk Enable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Enable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6��7��
		sleep_test();      //pc7 led��˸3��
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk/512,all clk Enable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Enable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6��8��
		sleep_test();      //pc7 led��˸3��
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk,all clk Disable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Disable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6��9��
		sleep_test();     //pc7 led��˸3��
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6��9��
		sleep_test();     //pc7 led��˸3��
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6  pc6��10��
		DeepSleep_test();    //pc7 led��˸5��
		
		//�ڲ�ʱ�ӣ�io�ڲ���ʾƵ�ʣ�Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO ����IOpin ��ʾ
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6��11��
		Wait_test();    //pc7 led��˸8��
	}
}
*/