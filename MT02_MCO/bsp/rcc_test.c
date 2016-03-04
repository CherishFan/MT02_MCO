
#include "all.h"


/********************************************************************************************************
**函数信息 ：SystemClk_HSEInit (void)                        
**功能描述 ：系统时钟初始化函数，初始化之前先复位所有时钟
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void SystemClk_HSEInit(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//HSE on
	//CR寄存器BIT16位（HSEON位）置1，作用是连接外部时钟HSE作为系统时钟
	RCC_HSEConfig(RCC_HSE_ON);
	while(1)
	{
		if(RCC_WaitForHSEStartUp()!=0)
		{
			break;
		}
	}
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);//选择外部时钟作为系统时钟
	
// 	RCC_MCOConfig(RCC_MCO_SYSCLK);  //通过PA8 pin 观察频率
}

/********************************************************************************************************
**函数信息 ：SystemClk_HSIInit (void)                        
**功能描述 ：系统时钟初始化函数，初始化之前先复位所有时钟
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void SystemClk_HSIInit()
{
	unsigned int temp;
	
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
// 	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
// 	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_10MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
// 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//HSI on
	//RCC->CR |= (uint32_t)0x00000001;  //CR寄存器BIT0位（HSION位）置1，作用是连接内部时钟HSI作为系统时钟
                                      //这样做是为了下边的操作做准备
	
	RCC_DeInit();
	//RCC->CR = (uint32_t)0x00000001;
	RCC_HSICmd(ENABLE);
	
	//RCC->CFGR = 0x00000000; //HSI CLK 作为系统时钟输出
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);//选择内部时钟作为系统时钟
	RCC_MCOConfig(RCC_MCO_SYSCLK);  //通过PA8 pin 观察频率
}


/********************************************************************************************************
**函数信息 ：SystemClkPll(unsigned int pllmul)                
**功能描述 ：倍频频率作为系统工作时钟
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16 
**输出参数 ：无
********************************************************************************************************/

void SystemClkPll(unsigned int pllmul)
{
	
	RCC_PLLCmd(ENABLE); //使能PLL

	while(1)
	{
		if(RCC->CR&0x02000000) //查询PLL ready
		{
			
			break;
		}
	}
	//FLASH->ACR |= 0x02;
// 	FLASH_SetLatency(FLASH_Latency_2);//时钟等待状态 
	
	//RCC->CFGR = 0x07010002; //PLL/2 PA8输出
	//RCC->CFGR = 0x04010002; //PLL PA8输出
//  	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//选择PLL时钟作为系统时钟
 	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);  //通过PA8 pin 观察频率
}

 
/********************************************************************************************************
**函数信息 ：HSE_Pll_test(unsigned int pllmul)                      
**功能描述 ：外部晶振作为时钟源
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16 
**输出参数 ：无
********************************************************************************************************/

void HSE_Pll_test(unsigned int pllmul)
{
	unsigned int n,m;
	SystemClk_HSEInit();
	//PLL 配置
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, pllmul); //选择外部时钟源作为倍频时钟的基础
	SystemClkPll(pllmul);
}

/********************************************************************************************************
**函数信息 ：HSE_Pll_test(unsigned int pllmul)                      
**功能描述 ：外部晶振作为时钟源
**输入参数 ：倍频参数，可选RCC_PLLMul_2 － RCC_PLLMul_16 
**输出参数 ：无
********************************************************************************************************/

void HSI_Pll_test(unsigned int pllmul)
{
	unsigned int n,m;
	SystemClk_HSIInit();
	//PLL 配置
	RCC_PLLConfig(RCC_PLLSource_HSI_Div4, pllmul); //选择内部时钟源作为倍频时钟的基础
	SystemClkPll(pllmul);
}
 
/********************************************************************************************************
**函数信息 ：LSI_clk()                      
**功能描述 ：内部低频时钟并通过示波器观察PA8 pin 40KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void LSI_clk()
{
	unsigned int temp;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//RCC->CSR |= 0x01;   //open lsi on
	RCC_LSICmd(ENABLE);
	
	while(1)
	{
		if(RCC->CSR&0x00000002) //查询内部低频时钟ready
		{
			break;
		}
	}
	//RCC->CFGR = 0x02000000; //LSI CLK 作为系统时钟输出   
  RCC_MCOConfig(RCC_MCO_LSI);  //通过PA8 pin 观察频率
}

 
/********************************************************************************************************
**函数信息 ：LSE_clk()                      
**功能描述 ：内部低频时钟并通过示波器观察PA8 pin 32.786KHz
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void LSE_clk()
{
	unsigned int temp;
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //mco  pa8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);

	PWR_BackupAccessCmd(ENABLE); //使能PWR_CR中的DBPbit
	
	RCC_LSEConfig(RCC_LSE_ON);
	
	while(1)
	{
		if(RCC->BDCR&0x00000002) //查询外部32.768KHz低频时钟ready
		{
			break;
		}
	}
	
	RCC_MCOConfig(RCC_MCO_LSE);  //通过PA8 pin 观察频率
}

/********************************************************************************************************
**函数信息 ：HSE_HSI_Change()                      
**功能描述 ：外部时钟与内部时钟之间的切换
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/

void HSE_HSI_Change()
{
	unsigned char i;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
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
	
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
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