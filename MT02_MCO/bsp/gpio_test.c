
#include "all.h"

void GPIO_LedTest()
{
	uint32_t i ,j;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //开启GPIOD端口

	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*//测试TDO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	uart_printf("GPIOB->CRL=0x%x\r\n",GPIOB->CRL);
	uart_printf("GPIOB->CRH=0x%x\r\n",GPIOB->CRH);
	uart_printf("AFIO->MAPR=0x%x\r\n",AFIO->MAPR);
	*/
	while(1)
		
	{
		//IWDG_ReloadCounter();
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);	   //Pc6输出低电平，点亮ARMLED
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		
		for(i=0;i<300000;i++)
		
		{}
		
		GPIO_SetBits(GPIOC, GPIO_Pin_6);       //PA1输出低电平，熄灭ARMLED
	  GPIO_SetBits(GPIOC, GPIO_Pin_7); 
	  GPIO_SetBits(GPIOC, GPIO_Pin_8); 
	  GPIO_SetBits(GPIOC, GPIO_Pin_9); 					  
		for(i=0;i<300000;i++)
		
		{}
		/*
		GPIO_ResetBits(GPIOA, GPIO_Pin_All);				  //PA输出低电平
		Delay(300);
		GPIO_SetBits(GPIOA, GPIO_Pin_All);					  
		Delay(300);
		GPIO_ResetBits(GPIOB, GPIO_Pin_All);				  //PA输出低电平
		Delay(300);
		GPIO_SetBits(GPIOB, GPIO_Pin_All);					  
		Delay(300);
		
		GPIO_ResetBits(GPIOD, GPIO_Pin_All);				  //PA输出低电平
		Delay(300);
		GPIO_SetBits(GPIOD, GPIO_Pin_All);					  
		Delay(300);
		GPIO_ResetBits(GPIOE, GPIO_Pin_All);				  //PA输出低电平
		Delay(300);
		GPIO_SetBits(GPIOE, GPIO_Pin_All);					  
		Delay(300);
		*/
	}

}


void GPIOC6_LedTest()
{
	uint32_t i ,j;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	for(j=0;j<6;j++)
	{
		//IWDG_ReloadCounter();
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);	   //Pc6输出低电平，点亮ARMLED
		
		for(i=0;i<3000000;i++)
		{}
		
		GPIO_SetBits(GPIOC, GPIO_Pin_6);       //PA1输出低电平，熄灭ARMLED
	  				  
		for(i=0;i<3000000;i++)	
		{}
		
	}
}

void GPIOC7_LedTest()
{
	uint32_t i ,j;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	for(j=0;j<10;j++)
	{
		//IWDG_ReloadCounter();
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);	   //Pc6输出低电平，点亮ARMLED
		
		for(i=0;i<3000000;i++)
		{}
		
		GPIO_SetBits(GPIOC, GPIO_Pin_7);       //PA1输出低电平，熄灭ARMLED
	  				  
		for(i=0;i<3000000;i++)	
		{}
		
	}

}

void GPIOC8_LedTest()
{
	uint32_t i ,j;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	for(j=0;j<10;j++)
	{
		//IWDG_ReloadCounter();
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);	   //Pc6输出低电平，点亮ARMLED
		
		for(i=0;i<3000000;i++)
		{}
		
		GPIO_SetBits(GPIOC, GPIO_Pin_8);       //PA1输出低电平，熄灭ARMLED
	  				  
		for(i=0;i<3000000;i++)	
		{}
		
	}

}

void GPIOC9_LedTest()
{
	uint32_t i ,j;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	for(j=0;j<10;j++)
	{
		//IWDG_ReloadCounter();
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);	   //Pc6输出低电平，点亮ARMLED
		
		for(i=0;i<3000000;i++)
		{}
		
		GPIO_SetBits(GPIOC, GPIO_Pin_9);       //PA1输出低电平，熄灭ARMLED
	  				  
		for(i=0;i<3000000;i++)	
		{}
		
	}

}

void GPIO_intput_output()
{
	uint32_t i ;
	uint16_t tmpdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All; //GPIOA为输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All; //GPIOB为输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	while(1)
	{
	tmpdata = GPIOA->IDR;
	GPIOB->ODR = tmpdata; //通过示波器观察PB pin的电压值变化
	}
	
}

//运放测试
void Op2_test()
{
	OPAMP_InitTypeDef OPAMP_InitTypeDefStructure;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);  //开启GPIOA端口
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //开启ADC1端口
	ADC_Cmd(ADC1, ENABLE);    //ADCFG 寄存器的ADEN位使能 

	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_5; //GPIOA为输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1|GPIO_Pin_5; //GPIOC
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// 把PC5、PC1连接在一起，给PA5一个输入方波，观察PC1的输出
	OPAMP_InitTypeDefStructure.OPAMP_NonInvertingInput = OPAMP_NonInvertingInput_IO1; //PA5 同相
  OPAMP_InitTypeDefStructure.OPAMP_InvertingInput = OPAMP_InvertingInput_IO1;  //PC5 反相
	
	OPAMP_Init(OPAMP_Selection_OPAMP2, &OPAMP_InitTypeDefStructure);
	OPAMP->CSR |= 0x100;
	//OPAMP_Cmd(OPAMP_Selection_OPAMP1, ENABLE);
	
}

void Op3_test()
{
	OPAMP_InitTypeDef OPAMP_InitTypeDefStructure;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);  //开启GPIOA端口
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //开启ADC1端口
	ADC_Cmd(ADC1, ENABLE);    //ADCFG 寄存器的ADEN位使能 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);  //开启ADC1端口
	ADC_Cmd(ADC2, ENABLE);    //ADCFG 寄存器的ADEN位使能 
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_1; //GPIOA为输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//PA5 同相   PA1 反相  PA3 输出
	OPAMP_InitTypeDefStructure.OPAMP_NonInvertingInput = OPAMP_NonInvertingInput_IO2; //PA5 同相
  OPAMP_InitTypeDefStructure.OPAMP_InvertingInput = OPAMP_InvertingInput_IO3;  //PA1 反相
	
	OPAMP_Init(OPAMP_Selection_OPAMP3, &OPAMP_InitTypeDefStructure);
	//OPAMP->CSR |= 0x100;
	OPAMP_Cmd(OPAMP_Selection_OPAMP3, ENABLE);
	
}