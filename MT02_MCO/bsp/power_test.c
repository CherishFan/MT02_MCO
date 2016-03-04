#include "all.h"

void EXTI1_IRQHandler(void)
{
	unsigned int i,j;
	
	EXTI_ClearITPendingBit(EXTI_Line1);				 //清除中断标志
	for(j=0;j<100;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7输出低电平，点亮ARMLED	 
		for(i=0;i<300000;i++)
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7输出低电平，熄灭ARMLED
		for(i=0;i<300000;i++)
		{}
	}
}



void Gpio_Exti_init()
{
	EXTI_InitTypeDef        EXTI_InitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;           //定义NVIC初始化需要的数据结构体
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7; //|GPIO_Pin_7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;          //EXTI0 中断号配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //主优先级设置为0，占用0bit，从优先级可以有4bit（0-15）选择
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //数字越小优先级越高（主从优先级一共加起来不能超过4bit）
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //例如：若主优先级为3，占用2bit，从优先级可以有2bit可以选，即0-3
	NVIC_Init(&NVIC_InitStructure);   //上面4条语句是把设置的内容存入NVIC_InitStructure这个结构体，本函数是把结构体的内容写入相应寄存器

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

void GpioC7init()
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
	
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
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//SCB->SCR = 0x0;
	//Gpio_Exti_init();
	GpioC7init();
	
	//GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6输出低电平，点亮ARMLED	
	///*
	for(j=0;j<3;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7输出低电平，点亮ARMLED	 
		for(i=0;i<300;i++)
		
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7输出High电平，熄灭ARMLED
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

void DeepSleep_test()//停止状态
{
	unsigned int i,j;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR->CR = PWR->CR & 0xfffd; //PDDS = 0;enter stop mode
	SCB->SCR |= 0x4;
	Gpio_Exti_init();
	
	for(j=0;j<5;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6输出低电平，点亮ARMLED	 
		for(i=0;i<300000;i++)
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //PA1输出低电平，熄灭ARMLED
		for(i=0;i<300000;i++)
		{}
	}
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, DISABLE);
	//PWR->CR = PWR->CR |0x02;
	__WFI();
	
}

void Wait_test()//待机状态
{
	unsigned int i,j;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR->CR |= 0x0002; //PDDS = 1;enter wait mode
	SCB->SCR |= 0x4;
	Gpio_Exti_init();
	
	for(j=0;j<8;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6输出低电平，点亮ARMLED	 
		for(i=0;i<300000;i++)
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //PA1输出低电平，熄灭ARMLED
		for(i=0;i<300000;i++)
		{}
	}
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, DISABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, DISABLE);
	
	
	//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
//  	
//  	PWR_BackupAccessCmd(ENABLE); //使能PWR_CR中的DBPbit

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
			GPIO_ResetBits(GPIOC, gpio_pin);	   //Pc6输出低电平，点亮ARMLED
			for(i=0;i<delay;i++)
			{;}
			GPIO_SetBits(GPIOC, gpio_pin);       //Pc6输出高电平，熄灭ARMLED
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
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		RCC->CFGR = 0x04000000; //HSI CLK 作为系统时钟输出
	}
	else
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE); 
	}
		
}

void Hse_clk_set()
{
	unsigned int temp;
	//RCC->CR = RCC->CR | 0x00010000; //CR寄存器BIT16位（HSION位）置1，作用是连接外部时钟HSE作为系统时钟
	RCC->CR = 0x00010000; 
		while(1)
		{
			temp = RCC->CR;
			if(temp & 0x00020000)
			{
				break;
			}
		}
	
		RCC->CFGR = 0x00000001; //HSE CLK 作为系统时钟输出
}

void AllCLK_EN_sleep_test()
{
	unsigned int i,j;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//SCB->SCR = 0x0;
	//Gpio_Exti_init();
	GpioC7init();
	
	for(j=0;j<3;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7输出低电平，点亮ARMLED	 
		for(i=0;i<300000;i++)
		
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7输出High电平，熄灭ARMLED
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
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	//SCB->SCR = 0x0;
	//Gpio_Exti_init();
	GpioC7init();
	
	for(j=0;j<3;j++)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc7输出低电平，点亮ARMLED	 
		for(i=0;i<300000;i++)
		
		{}
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //Pc7输出High电平，熄灭ARMLED
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
	if(Hse)  //pc9 led闪烁
	{
		//HSE外部时钟，io口不显示频率
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE 
		wait_led(GPIO_Pin_9,number,delay); //led PC9 闪2次
		
		//HSE外部时钟，io口显示频率
		number++;
		Hse_clk_set();
		MCO_Output(1);  //HSE + PA8
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪3次
		
		//HSE外部时钟，io口显示频率，all clk Enable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		AllClk_Enable();
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪4次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		AllClk_Disable();
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪5次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪6次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		GPIO_LedTest(); 
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪7次
		
			//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);      
		I2C1MasterTest();
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪8次
		
		
			//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);         
		SPIM1_Test();   
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪9次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		AllClk_Disable();
		Uart1Init(USART1);         
		Tim1_CentAlignMode3_test3(TIM1,0x1111);   
		wait_led(GPIO_Pin_9,number,delay); //led PC9  闪10次
		
	}
	number = 0;
	if(Pll_48MHz)
	{
		//HSE外部时钟，io口不显示频率
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE 
		SystemClkPll(0x2c100000); //48MHz
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪1次
		
		//HSE外部时钟，io口显示频率
		number++;
		Hse_clk_set();
		MCO_Output(1);  //HSE + PA8
		SystemClkPll(0x2c100000); //48MHz
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪2次
		
		//HSE外部时钟，io口显示频率，all clk Enable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0x2c100000); //48MHz
		AllClk_Enable();
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪3次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0x2c100000); //48MHz
		AllClk_Disable();
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪4次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz		
		AllClk_Disable();
		Uart1Init(USART1);
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪5次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz				
		AllClk_Disable();
		GPIO_LedTest(); 
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪6次
		
			//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz				
		AllClk_Disable();
		Uart1Init(USART1);      
		I2C1MasterTest();
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪7次
		
		
	//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0x2c100000); //48MHz				
		AllClk_Disable();
		Uart1Init(USART1);         
		SPIM1_Test();   
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪8次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		SystemClkPll(0x2c100000); //48MHz		
		AllClk_Disable();
		Uart1Init(USART1);         
		Tim1_CentAlignMode3_test3(TIM1,0x1111);   
		wait_led(GPIO_Pin_8,number,delay); //led PC8  闪9次
		
	}
	
	number = 20;
	if(Pll_96MHz)
	{
		//HSE外部时钟，io口不显示频率
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE 
		SystemClkPll(0xbc300000);  //96MHz
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪21次
		
		//HSE外部时钟，io口显示频率
		number++;
		Hse_clk_set();
		MCO_Output(1);  //HSE + PA8
		SystemClkPll(0xbc300000);  //96MHz
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪22次
		
		//HSE外部时钟，io口显示频率，all clk Enable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0xbc300000);  //96MHz
		AllClk_Enable();
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪23次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  //HSE + PA8
		SystemClkPll(0xbc300000);  //96MHz
		AllClk_Disable();
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪24次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz	
		AllClk_Disable();
		Uart1Init(USART1);
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪25次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz			
		AllClk_Disable();
		GPIO_LedTest(); 
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪26次
		
			//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz	
		AllClk_Disable();
		Uart1Init(USART1);      
		I2C1MasterTest();
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪27次
		
		
	//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);
		SystemClkPll(0xbc300000);  //96MHz	
		AllClk_Disable();
		Uart1Init(USART1);         
		SPIM1_Test();   
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪28次
		
		//HSE外部时钟，io口显示频率，all clk Disable
		number++;
		Hse_clk_set();
		MCO_Output(0);  
		SystemClkPll(0xbc300000);  //96MHz
		AllClk_Disable();
		Uart1Init(USART1);         
		Tim1_CentAlignMode3_test3(TIM1,0x1111);   
		wait_led(GPIO_Pin_7,number,delay); //led PC7  闪29次
		
	}
	number = 1;
	if(Hsi)
	{
		//内部时钟，io口不显示频率
		RCC->CR = (uint32_t)0x00000001; 
		wait_led(GPIO_Pin_6,number,delay); //led PC6  闪1次
		
		//内部时钟，io pa8 显示频率
		number++;
		MCO_Output(1);  //HSI + MCO PA8
		wait_led(GPIO_Pin_6,number,delay); //led PC6  闪2次
		
		//内部时钟，io口不显示频率，Hpre = sysclk,all clk Enable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Enable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   闪3次
		
		
		//内部时钟，io口不显示频率，Hpre = sysclk/512,all clk Enable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Enable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   闪4次
		
		
		//内部时钟，io口不显示频率，Hpre = sysclk,all clk Disable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Disable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   闪5次
		
		
		//内部时钟，io口不显示频率，Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   闪6次
		
		
/-------------- sleep  test ----------------------------/
		//内部时钟，io口不显示频率，Hpre = sysclk,all clk Enable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Enable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6闪7次
		sleep_test();      //pc7 led闪烁3次
		
		//内部时钟，io口不显示频率，Hpre = sysclk/512,all clk Enable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Enable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6闪8次
		sleep_test();      //pc7 led闪烁3次
		
		//内部时钟，io口不显示频率，Hpre = sysclk,all clk Disable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Disable();
		AHB_Prescale(0);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6闪9次
		sleep_test();     //pc7 led闪烁3次
		
		//内部时钟，io口不显示频率，Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6闪9次
		sleep_test();     //pc7 led闪烁3次
		
		//内部时钟，io口不显示频率，Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6  pc6闪10次
		DeepSleep_test();    //pc7 led闪烁5次
		
		//内部时钟，io口不显示频率，Hpre = sysclk/512,all clk Disable
		number++;
		MCO_Output(0); //MCO 不在IOpin 显示
		AllClk_Disable();
		AHB_Prescale(0xf);
		wait_led(GPIO_Pin_6,number,delay); //led PC6   pc6闪11次
		Wait_test();    //pc7 led闪烁8次
	}
}
*/