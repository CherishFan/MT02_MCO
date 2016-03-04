/****************************************Copyright (c)****************************************************
** 
**                                     	  上海灵动微电子
**
**                                  
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			bsp_tim.c
** modified Date:  		2013-6-20
** Last Version:		V1.00
** Descriptions:		定时器中断初始化和中断服务程序
**	
*********************************************************************************************************/
#include "all.h"

uint16_t tim_flag = 0;
unsigned char capture1Flag = 0x0;
unsigned char capture3Flag = 0x0;
unsigned char capture2Flag = 0x0;
unsigned char capture4Flag = 0x0;
unsigned char triggerFlag = 0x0;
unsigned char updateFlag = 0x0;




//*********************timer3 test***********************************//

void TIM3_IRQHandler (void)
{
	
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)!= RESET)	 //check UIF位 
	{
		updateFlag = 0x01;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
	}	
	
	
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Trigger)!= RESET)	 //check TIF位 
	{
		triggerFlag = 0x01;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Trigger);
	}	

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC1)!= RESET)	 //check capture compare 1位 
	{
		capture1Flag = 0x1;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC1);
	}	

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC2)!= RESET)	 //check capture compare 2位 
	{
		capture2Flag = 0x1;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC2);
	}	

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC3)!= RESET)	 //check capture compare 2位 
	{
		capture3Flag = 0x01;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC3);
	}

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC4)!= RESET)	 //check capture compare 2位 
	{
		capture4Flag = 0x01;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC4);
	}

	//TIM_PrescalerConfig(TIM3,tim_flag,0x0000);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 
	//TIM3->RCR = 0x02;  //周期计数次数
 /* 
  if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_All))
	{
		//清除对应的ODRy位为0
		GPIO_ResetBits(GPIOA, GPIO_Pin_All);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_All); //设置对应的ODRy位为1 
	}
*/
}





//设置UG bit，手动产生更新事情
void Tim3_UPCount_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1的更新事件中断使能

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx, TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //自动重装载预装载允许、禁止
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 
	TIM_SetAutoreload(TIMx, counter);		  //计数值，计数等于该数则发生溢出  TIMx->ARR

	TIMx->RCR = 0x01;  //周期计数次数
	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		if(updateFlag)
		{
			updateFlag = 0x0;
			ReadCount(TIMx);
			//break;
			
		}
	}
}


//设置UG bit，手动产生更新事情
void Tim3_DOWNCount_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1的更新事件中断使能

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Down); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //自动重装载预装载允许、禁止
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, counter);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 

	TIMx->RCR = 0x01;  //周期计数次数
	

	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		ReadCount(TIMx);
		if(updateFlag)
		{
			updateFlag = 0x0;
			//break;
		}
	}
}

//中央对齐模式1
void Tim3_CentAlignMode1_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1的更新事件中断使能

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned1); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //自动重装载预装载允许、禁止	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, counter);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 

	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	//TIM_SetAutoreload(TIMx, 0x55);
	while(1)
	{
		ReadCount(TIMx);
		
		if(updateFlag)
		{
			updateFlag = 0x0;
			ReadCount(TIMx);
			//break;
		}
	}
}
//中央对齐模式2
void Tim3_CentAlignMode2_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1的更新事件中断使能

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned2); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, counter);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 

	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	TIM_SetAutoreload(TIMx, 0x55);
	
	while(1)
	{
		ReadCount(TIMx);
		if(updateFlag)
		{
			updateFlag = 0x0;
			ReadCount(TIMx);
			break;
		}
	}
}

//中央对齐模式3  + 重复向下计数	RCR的设置
void Tim3_CentAlignMode3_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM3的更新事件中断使能

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned3); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //自动重装载预装载允许、禁止	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, counter);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 
	//TIMx->RCR = 0x01;  //周期计数次数
	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		if(updateFlag)
		{
			updateFlag = 0x0;
			ReadCount(TIMx);
			break;
		}
	}
}

//外部时钟源模式1 外部输入引脚	 测试通道2
void Tim3_ExtClkMode1_test(TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	//tim2的ch2通道  PA1 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	triggerFlag = 0x0;
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1的触发中断使能
*/
  TIM_TIxExternalClockConfig(TIMx, TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Rising, 0x0000);// 通道2 配置为输入 IC2映射在TI2上 ,不带滤波，上升沿
	
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, 0xffff);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	
	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		ReadCount(TIMx);
		/*
		if(triggerFlag)
		{
			triggerFlag = 0x0;
			ReadCount(TIMx);
			break;
		}
		*/
	}
}


//外部时钟源模式2 外部触发ETR引脚
void Tim3_ExtClkMode2_test(TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	//tim3的etr通道  PD2 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//配置ETF[3:0]=0  ETPS[1:0]=01  ETP=0 ECE=1
	TIM_ETRClockMode2Config(TIMx, TIM_ExtTRGPSC_DIV2, TIM_ExtTRGPolarity_NonInverted, 0x0000);

	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, 0xff);

	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		ReadCount(TIMx);
		/*
		if(triggerFlag)
		{
			triggerFlag = 0x0;
			ReadCount(TIMx);
			break;
		}
		*/
	}

}


//捕获比较通道
void Tim3_capture_compare_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	//tim3的ch1通道  PA6   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ch2通道 PA7 
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //复用推免输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	capture2Flag = 0x0;
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM通道1的触发中断使能
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM通道2的触发中断使能
	*/
	TIM_SetAutoreload(TIMx, 0xff);

	TIM_ICInitStruct.TIM_Channel = channel;
  TIM_ICInitStruct.TIM_ICPolarity = polarity;
	TIM_ICInitStruct.TIM_ICSelection = selectPin;
  TIM_ICInitStruct.TIM_ICPrescaler = pscclk;
  TIM_ICInitStruct.TIM_ICFilter = filter;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	
	
	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // 设置MOE bit

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle; // 值匹配时  OCxREF 的状态
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct.TIM_Pulse = 0x1234;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIMx, &TIM_OCInitStruct);
  
	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			temp = TIM_GetCapture1(TIM3);
			ReadCount(TIMx);
			uart_printf("ccr1=0x%x\r\n",temp);
			//break;
		}
		if(capture2Flag)
		{
			capture2Flag = 0x0;
			temp = TIM_GetCounter(TIM3);
			uart_printf("cnt=0x%x\r\n",temp);
			break;
		}
	}
}

//输入捕获模式
void Tim3_capture_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	unsigned int buffer[2] = {0,0};
	unsigned int i;
	uint16_t temp;

  GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
 

//tim3的ch3通道  PB0  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	capture3Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//通道3的触发中断使能

	TIM_SetAutoreload(TIMx, 0x12ff);

	TIM_ICInitStruct.TIM_Channel = channel;
  TIM_ICInitStruct.TIM_ICPolarity = polarity;
  TIM_ICInitStruct.TIM_ICSelection = selectPin;
  TIM_ICInitStruct.TIM_ICPrescaler = pscclk;
  TIM_ICInitStruct.TIM_ICFilter = filter;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	
	
	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
#ifdef TIME_DMA

	TIM_DMACmd(TIMx, TIM_DMA_CC3, ENABLE);  //DMA request enable
	TIM_DMAConfig(TIMx, TIM_DMABase_CCR3, 0x01);
	/*
	DMATimerConfigure(0x00000005,PtoM,(TIM1_BASE+0x3c),0x20004000,2); //Timer1_ccr3的地址  0x40012c3c  
	DMAenable(0x00000005);
	DMAChannelEnable(0x00000005);	
	*/
	DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (TIM3_BASE+0x3c);
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20004000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_SrcInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_DstInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK0;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK0;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	DMAcheckStatus(DMA1_FLAG_TC1);
	DMA_Cmd(DMA1_Channel1, DISABLE); //disable dma_rxen and txen 

	for(i=0;i<2;i++)
	{
	buffer[i] = *((volatile unsigned int *)0x20004000+i*4);
	uart_printf("a=0x%x\r\n",buffer[i] );	
	}	  
#endif

	while(1)
	{
		if(capture3Flag)
		{
			capture3Flag = 0x0;
			temp = TIM_GetCapture3(TIM3);
			uart_printf("ccr3=0x%x\r\n",temp);
			break;
		}
	}

}

//PWM输入模式
void Tim3_PWM_capture_test(TIM_TypeDef* TIMx)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	//tim2的ch1、ch2通道  PA6  PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6&&GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	capture1Flag = 0x0;
	capture2Flag = 0x0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1的触发中断使能
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM3的触发中断使能

	TIM_SetAutoreload(TIMx, 0xffff);

	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICFilter = 0x0;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);

	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR寄存器的TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);	//SMCR寄存器的SMS[2:0]bits

	//TIM_GenerateEvent(TIMx, TIM_EventSource_CC1); // 产生捕获比较事件


	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
/*
	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			temp = TIM_GetCapture1(TIM3);
			uart_printf("ccr1=0x%x\r\n",temp);
			//break;
		}
	
		if(capture2Flag)
		{
			capture2Flag = 0x0;
			temp = TIM_GetCapture2(TIM3);
			uart_printf("ccr2=0x%x\r\n",temp);
			//break;
		}
	}
	*/
///*
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	while(1)
	{
		if(capture1Flag==0x1)
		{
			capture1Flag = 0x0;
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0))
			{
				//清除对应的ODRy位为0
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_0); //设置对应的ODRy位为1 
			}
		}
		if(capture2Flag==0x1)
		{
			capture2Flag = 0x0;
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
			{
				//清除对应的ODRy位为0
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_1); //设置对应的ODRy位为1 
			}
		}
	}
//*/	
}

//输出比较模式
void Tim3_compare_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;
  NVIC_InitTypeDef         NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	//tim2的ch4通道  PB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推免输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	
	capture4Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	//TIM1的触发中断使能

#ifdef TIME_DMA
	*((volatile unsigned int *)0x20003000) = 0x1234;
	TIM_DMACmd(TIMx, TIM_DMA_CC4, ENABLE);  //DMA request enable
	TIM_DMAConfig(TIMx, TIM_DMABase_CCR4, 0x01);
	/*
	DMATimerConfigure(0x00000003,MtoP,0x20003000,(TIM1_BASE+0x40),1); //Timer1_ccr3的地址  0x40012c3c  
	DMAenable(0x00000003);
	DMAChannelEnable(0x00000003);		
  */
	DMA_DeInit(DMA1_Channel7);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (TIM3_BASE+0x40);
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_SrcInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_DstInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK12;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK0;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel7, ENABLE);
	DMAcheckStatus(DMA1_FLAG_TC7);
	DMA_Cmd(DMA1_Channel7, DISABLE); //disable dma_rxen and txen 
	
#endif
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned3); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //自动重装载预装载允许、禁止	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS 更新请求源 URS=1
	TIM_SetAutoreload(TIMx, 0x2ff);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 

 
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle; // 值匹配时  OCxREF 的状态
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x78;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);

  	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器

	while(1)
	{
		if(capture4Flag)
		{
			capture4Flag = 0x0;
			temp = TIM_GetCounter(TIM3);
			uart_printf("cnt=0x%x\r\n",temp);
			break;
		}
		
		
	}
#ifdef TIME_DMA
		Delay(0xffff);
		temp = TIM_GetCapture4(TIMx);
		uart_printf("temp=0x%x\r\n",temp);	
#endif
	
}

//PWM1 pwm2模式向上  输出
void Tim3_PWM_edgeUp_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	//tim3的ch4通道  PB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推免输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture4Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 手动产生一个更新事件 

	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // 值匹配时  OCxREF 的状态  PWM1 模式
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x007;    //0x04频率1.667MHz 600ns 不变PWM2模式下，随着TIM PULSE的值改变，脉宽改变，值越小，high越宽。PWM1模式与PWm2相反 
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);

	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE 输出比较 预装载使能

  	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	while(1)
	{
		if(capture4Flag)
		{
			capture4Flag = 0x0;
			break;
		}	
	}
}


//pwm1 PWM2模式向下计数  输出
void Tim3_PWM_edgeDown_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推免输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture3Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Down); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 手动产生一个更新事件 


	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // 值匹配时  OCxREF 的状态  PWM1 模式
   	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x007;   //0x04频率1.667MHz 600ns 不变，pwm1模式下，随着TIM PULSE的值改变，脉宽改变，值越小，low越宽PWM2 模式相反。
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIMx, &TIM_OCInitStruct);

	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE 输出比较 预装载使能

    TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器

	while(1)
	{
		if(capture3Flag)
		{
			capture3Flag = 0x0;
			break;
		}	
	}
	
}


//PWM1 PWM2模式中央对齐模式  输出
void Tim3_PWM_CenterAligned_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推免输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned1); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0xf,0x0001);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 手动产生一个更新事件 

	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // 值匹配时  OCxREF 的状态  PWM1 模式
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0002;          //8.6us 119KHz 模式1、2、3下，值越大，high越宽
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIMx, &TIM_OCInitStruct);

	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE 输出比较 预装载使能

  	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器

	while(1)
	{
		if(capture2Flag)
		{
			capture2Flag = 0x0;
			break;
		}	
	}
	
}

//单脉冲模式
void Tim3_opm_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;            //PA7 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推免输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	capture1Flag = 0x0;
//	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1的触发中断使能
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1的触发中断使能
//*/
	TIM_SetAutoreload(TIMx, 0xff);		  		//计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);  //set cr1 reg OPM bit

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //向上计数
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Down); //向下计数
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = filter;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI2FP2);  //SMCR 寄存器的TS ＝ 110 配置TI2FP2
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger); //SMCR 寄存器的SMS ＝ 110 触发模式
	
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // 值匹配时  OCxREF 的状态
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0065;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIMx, &TIM_OCInitStruct);
	// 正常模式和OCx快速使能，下面两句语句选其一
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable); //OC1PE 输出比较 预装载使能
	//TIM_OC1FastConfig(TIMx, TIM_OCFast_Enable);		  //OC1FE 输出比较1 快速使能

	TIM_PrescalerConfig(TIMx,0x5,0x0001);       //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 手动产生一个更新事件 

	TIM_CtrlPWMOutputs(TIMx, ENABLE);   		// 设置MOE bit	
	
	 while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			break;
		}
		if(capture2Flag)
		{
			capture2Flag = 0x0;
			break;
		}
				
	}
}

//在外部事件时清除OCxREF信号
void Tim3_ETR_OCREF_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;            //PD2 etr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;            //PB1 ch4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推免输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture4Flag = 0x0;
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //计数值，计数等于该数则发生溢出  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0xf,0x0001);      //分频值,	 TIMx->PSC	TIMx->EGR的bit0 UG 手动产生一个更新事件 

	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // 设置MOE bit

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // 值匹配时  OCxREF 的状态  PWM1 模式 试过多种模式
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0004;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);

	//TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE 输出比较 预装载使能

  	TIM_ClearOC4Ref(TIMx, TIM_OCClear_Enable); //set OC4CE bit
	TIM_ETRConfig(TIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted,0x00);  //set SMCR  ETPS[1:0]、ETP、ETF

  	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
}

//编码器接口模式
void Tim3_program_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;            //PA7 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	capture2Flag = 0x0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1的触发中断使能
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1的触发中断使能
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1的触发中断使能
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetAutoreload(TIMx, 0xfff);		  		//计数值，计数等于该数则发生溢出  TIMx->ARR

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);

	TIM_SelectSlaveMode(TIMx, 0x0002); //SMCR 寄存器的SMS ＝ 011 编码器模式3

	
    
	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器

	while(1)
	{
		if(capture1Flag==0x1)
		{
			capture1Flag = 0x0;
			count = TIM_GetCapture1(TIM3);
			//uart_printf("count1=0x%x\r\n",count);
				if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0))
				{
					//清除对应的ODRy位为0
					GPIO_ResetBits(GPIOB, GPIO_Pin_0);
					
				}
				else
				{
					GPIO_SetBits(GPIOB, GPIO_Pin_0); //设置对应的ODRy位为1 
				}
		}
		if(capture2Flag==0x1)
		{
			capture2Flag = 0x0;
			
			count = TIM_GetCapture2(TIM3);
			//uart_printf("count2=0x%x\r\n",count);
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
			{
				//清除对应的ODRy位为0
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_1); //设置对应的ODRy位为1 
			}
		}
	 }
}


//Tim1定时器和外部触发的 同步模式  之复位模式
void Tim3_sreset_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM3的触发中断使能
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0

	//TI1_Config(TIMx, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI,0x00);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR寄存器的TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);	//SMCR寄存器的SMS[2:0]bits

	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器

	count = TIM_GetCounter(TIM3);
	uart_printf("count1=0x%x\r\n",count);
	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			
			break;
		}	
	}
	
	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			//count = TIM_GetCapture1(TIM3);
			count = TIM_GetCounter(TIM3);
			uart_printf("count2=0x%x\r\n",count);
			//break;
		}
	}
}


//Tim1定时器和外部触发的  门控模式
void Tim3_gate_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	
	//TI1_Config(TIMx, TIM_ICPolarity_Falling, TIM_ICSelection_DirectTI,0x00);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	

	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR寄存器的TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Gated);	//SMCR寄存器的SMS[2:0]bits

	

	TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	
	count = TIM_GetCounter(TIM3);
	uart_printf("count1=0x%x\r\n",count);
	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			break;
		}	
	}

	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			count = TIM_GetCounter(TIM3);
			uart_printf("count2=0x%x\r\n",count);
			//break;
		}
	}

}

//Tim1定时器和外部触发的  触发模式
void Tim3_trigger_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;            //PA1 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0

	//TI2_Config(TIMx, TIM_ICPolarity_Falling, TIM_ICSelection_DirectTI,0x0000); 
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI2FP2);  //SMCR寄存器的TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger);	//SMCR寄存器的SMS[2:0]bits

	//TIM_Cmd(TIMx, ENABLE);   				 //使能CEN_Set，开启计数器
	/*
	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			break;
		}	
	}
*/
	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			count = TIM_GetCounter(TIM3);
			uart_printf("count1=0x%x\r\n",count);
			//break;
		}
	}

}

//外部时钟模式2 + 触发模式
void Tim3_extclk2_trigger_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;            //PD2 etr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //使用的优先级群组，可以是0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //设置中断向量
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //如果使用优先级群组0，主优先级可以是0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //如果使用优先级群组0，从优先级可以是0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 的终端
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1的触发中断使能
	
	TIM_SetAutoreload(TIMx, 0x0fff);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //采样分频	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //计数方向和模式
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //自动重装载预装载允许、禁止	APRE  使能APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit 允许、禁止更新
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS 更新请求源 URS=0
	//TIM_GenerateEvent(TIMx, TIM_EventSource_Update);//产生  update事件

	TIM_ETRClockMode2Config(TIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0000);
	//TI1_Config(TIMx, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI,0x00);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	

	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR寄存器的TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger);	//SMCR寄存器的SMS[2:0]bits

	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			break;
		}	
	}

	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
		
			break;
		}
	}

	//Delay(0x1f);
	
	while(1)
	{
		ReadCount(TIM3);
	}
	
}

