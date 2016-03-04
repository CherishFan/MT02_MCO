/****************************************Copyright (c)****************************************************
** 
**                                     	  �Ϻ��鶯΢����
**
**                                  
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			bsp_tim.c
** modified Date:  		2013-6-20
** Last Version:		V1.00
** Descriptions:		��ʱ���жϳ�ʼ�����жϷ������
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
	
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)!= RESET)	 //check UIFλ 
	{
		updateFlag = 0x01;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
	}	
	
	
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Trigger)!= RESET)	 //check TIFλ 
	{
		triggerFlag = 0x01;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Trigger);
	}	

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC1)!= RESET)	 //check capture compare 1λ 
	{
		capture1Flag = 0x1;
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC1);
	}	

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC2)!= RESET)	 //check capture compare 2λ 
	{
		capture2Flag = 0x1;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC2);
	}	

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC3)!= RESET)	 //check capture compare 2λ 
	{
		capture3Flag = 0x01;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC3);
	}

	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_CC4)!= RESET)	 //check capture compare 2λ 
	{
		capture4Flag = 0x01;
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC4);
	}

	//TIM_PrescalerConfig(TIM3,tim_flag,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 
	//TIM3->RCR = 0x02;  //���ڼ�������
 /* 
  if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_All))
	{
		//�����Ӧ��ODRyλΪ0
		GPIO_ResetBits(GPIOA, GPIO_Pin_All);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_All); //���ö�Ӧ��ODRyλΪ1 
	}
*/
}





//����UG bit���ֶ�������������
void Tim3_UPCount_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx, TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR

	TIMx->RCR = 0x01;  //���ڼ�������
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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


//����UG bit���ֶ�������������
void Tim3_DOWNCount_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Down); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR
	
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 

	TIMx->RCR = 0x01;  //���ڼ�������
	

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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

//�������ģʽ1
void Tim3_CentAlignMode1_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned1); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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
//�������ģʽ2
void Tim3_CentAlignMode2_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned2); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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

//�������ģʽ3  + �ظ����¼���	RCR������
void Tim3_CentAlignMode3_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM3�ĸ����¼��ж�ʹ��

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned3); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 
	//TIMx->RCR = 0x01;  //���ڼ�������
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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

//�ⲿʱ��Դģʽ1 �ⲿ��������	 ����ͨ��2
void Tim3_ExtClkMode1_test(TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	//tim2��ch2ͨ��  PA1 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	triggerFlag = 0x0;
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
*/
  TIM_TIxExternalClockConfig(TIMx, TIM_TIxExternalCLK1Source_TI2,TIM_ICPolarity_Rising, 0x0000);// ͨ��2 ����Ϊ���� IC2ӳ����TI2�� ,�����˲���������
	
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, 0xffff);		  //����ֵ���������ڸ����������  TIMx->ARR
	
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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


//�ⲿʱ��Դģʽ2 �ⲿ����ETR����
void Tim3_ExtClkMode2_test(TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	//tim3��etrͨ��  PD2 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//����ETF[3:0]=0  ETPS[1:0]=01  ETP=0 ECE=1
	TIM_ETRClockMode2Config(TIMx, TIM_ExtTRGPSC_DIV2, TIM_ExtTRGPolarity_NonInverted, 0x0000);

	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, 0xff);

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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


//����Ƚ�ͨ��
void Tim3_capture_compare_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	//tim3��ch1ͨ��  PA6   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ch2ͨ�� PA7 
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	capture2Flag = 0x0;
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIMͨ��1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIMͨ��2�Ĵ����ж�ʹ��
	*/
	TIM_SetAutoreload(TIMx, 0xff);

	TIM_ICInitStruct.TIM_Channel = channel;
  TIM_ICInitStruct.TIM_ICPolarity = polarity;
	TIM_ICInitStruct.TIM_ICSelection = selectPin;
  TIM_ICInitStruct.TIM_ICPrescaler = pscclk;
  TIM_ICInitStruct.TIM_ICFilter = filter;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	
	
	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle; // ֵƥ��ʱ  OCxREF ��״̬
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct.TIM_Pulse = 0x1234;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIMx, &TIM_OCInitStruct);
  
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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

//���벶��ģʽ
void Tim3_capture_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	unsigned int buffer[2] = {0,0};
	unsigned int i;
	uint16_t temp;

  GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
 

//tim3��ch3ͨ��  PB0  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOB�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	capture3Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//ͨ��3�Ĵ����ж�ʹ��

	TIM_SetAutoreload(TIMx, 0x12ff);

	TIM_ICInitStruct.TIM_Channel = channel;
  TIM_ICInitStruct.TIM_ICPolarity = polarity;
  TIM_ICInitStruct.TIM_ICSelection = selectPin;
  TIM_ICInitStruct.TIM_ICPrescaler = pscclk;
  TIM_ICInitStruct.TIM_ICFilter = filter;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	
	
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
#ifdef TIME_DMA

	TIM_DMACmd(TIMx, TIM_DMA_CC3, ENABLE);  //DMA request enable
	TIM_DMAConfig(TIMx, TIM_DMABase_CCR3, 0x01);
	/*
	DMATimerConfigure(0x00000005,PtoM,(TIM1_BASE+0x3c),0x20004000,2); //Timer1_ccr3�ĵ�ַ  0x40012c3c  
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

//PWM����ģʽ
void Tim3_PWM_capture_test(TIM_TypeDef* TIMx)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	//tim2��ch1��ch2ͨ��  PA6  PA7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6&&GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	capture1Flag = 0x0;
	capture2Flag = 0x0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM3�Ĵ����ж�ʹ��

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

	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR�Ĵ�����TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);	//SMCR�Ĵ�����SMS[2:0]bits

	//TIM_GenerateEvent(TIMx, TIM_EventSource_CC1); // ��������Ƚ��¼�


	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
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
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	while(1)
	{
		if(capture1Flag==0x1)
		{
			capture1Flag = 0x0;
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_0))
			{
				//�����Ӧ��ODRyλΪ0
				GPIO_ResetBits(GPIOB, GPIO_Pin_0);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_0); //���ö�Ӧ��ODRyλΪ1 
			}
		}
		if(capture2Flag==0x1)
		{
			capture2Flag = 0x0;
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
			{
				//�����Ӧ��ODRyλΪ0
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_1); //���ö�Ӧ��ODRyλΪ1 
			}
		}
	}
//*/	
}

//����Ƚ�ģʽ
void Tim3_compare_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
  NVIC_InitTypeDef         NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	//tim2��ch4ͨ��  PB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	
	capture4Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	//TIM1�Ĵ����ж�ʹ��

#ifdef TIME_DMA
	*((volatile unsigned int *)0x20003000) = 0x1234;
	TIM_DMACmd(TIMx, TIM_DMA_CC4, ENABLE);  //DMA request enable
	TIM_DMAConfig(TIMx, TIM_DMABase_CCR4, 0x01);
	/*
	DMATimerConfigure(0x00000003,MtoP,0x20003000,(TIM1_BASE+0x40),1); //Timer1_ccr3�ĵ�ַ  0x40012c3c  
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
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned3); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, 0x2ff);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 

 
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Toggle; // ֵƥ��ʱ  OCxREF ��״̬
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x78;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);

  	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������

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

//PWM1 pwm2ģʽ����  ���
void Tim3_PWM_edgeUp_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	//tim3��ch4ͨ��  PB1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture4Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x007;    //0x04Ƶ��1.667MHz 600ns ����PPWM2ģʽ�£�����TIM PULSE��ֵ�ı䣬����ı䣬ֵԽС��highԽ��PWM1ģʽ��PWm2�෴ 
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);

	TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��

  	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
	while(1)
	{
		if(capture4Flag)
		{
			capture4Flag = 0x0;
			break;
		}	
	}
}


//pwm1 PWM2ģʽ���¼���  ���
void Tim3_PWM_edgeDown_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture3Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Down); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x1,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 


	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
   	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x007;   //0x04Ƶ��1.667MHz 600ns ���䣬pwm1ģʽ�£�����TIM PULSE��ֵ�ı䣬����ı䣬ֵԽС��lowԽ��PPWM2 ģʽ�෴��
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIMx, &TIM_OCInitStruct);

	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��

    TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������

	while(1)
	{
		if(capture3Flag)
		{
			capture3Flag = 0x0;
			break;
		}	
	}
	
}


//PWM1 PWM2ģʽ�������ģʽ  ���
void Tim3_PWM_CenterAligned_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned1); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0xf,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0002;          //8.6us 119KHz ģʽ1��2��3�£�ֵԽ��highԽ��
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIMx, &TIM_OCInitStruct);

	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��

  	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������

	while(1)
	{
		if(capture2Flag)
		{
			capture2Flag = 0x0;
			break;
		}	
	}
	
}

//������ģʽ
void Tim3_opm_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;            //PA7 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	capture1Flag = 0x0;
//	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1�Ĵ����ж�ʹ��
//*/
	TIM_SetAutoreload(TIMx, 0xff);		  		//����ֵ���������ڸ����������  TIMx->ARR
	TIM_SelectOnePulseMode(TIMx, TIM_OPMode_Single);  //set cr1 reg OPM bit

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���ϼ���
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Down); //���¼���
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = filter;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI2FP2);  //SMCR �Ĵ�����TS �� 110 ����TI2FP2
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger); //SMCR �Ĵ�����SMS �� 110 ����ģʽ
	
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // ֵƥ��ʱ  OCxREF ��״̬
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0065;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIMx, &TIM_OCInitStruct);
	// ����ģʽ��OCx����ʹ�ܣ������������ѡ��һ
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable); //OC1PE ����Ƚ� Ԥװ��ʹ��
	//TIM_OC1FastConfig(TIMx, TIM_OCFast_Enable);		  //OC1FE ����Ƚ�1 ����ʹ��

	TIM_PrescalerConfig(TIMx,0x5,0x0001);       //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	TIM_CtrlPWMOutputs(TIMx, ENABLE);   		// ����MOE bit	
	
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

//���ⲿ�¼�ʱ���OCxREF�ź�
void Tim3_ETR_OCREF_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;            //PD2 etr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_1;            //PB1 ch4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture4Flag = 0x0;
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC4, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0xf,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ �Թ�����ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0004;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC4Init(TIMx, &TIM_OCInitStruct);

	//TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��

  	TIM_ClearOC4Ref(TIMx, TIM_OCClear_Enable); //set OC4CE bit
	TIM_ETRConfig(TIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted,0x00);  //set SMCR  ETPS[1:0]��ETP��ETF

  	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
}

//�������ӿ�ģʽ
void Tim3_program_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;            //PA7 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	capture2Flag = 0x0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0xfff);		  		//����ֵ���������ڸ����������  TIMx->ARR

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
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

	TIM_SelectSlaveMode(TIMx, 0x0002); //SMCR �Ĵ�����SMS �� 011 ������ģʽ3

	
    
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������

	while(1)
	{
		if(capture1Flag==0x1)
		{
			capture1Flag = 0x0;
			count = TIM_GetCapture1(TIM3);
			//uart_printf("count1=0x%x\r\n",count);
				if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0))
				{
					//�����Ӧ��ODRyλΪ0
					GPIO_ResetBits(GPIOB, GPIO_Pin_0);
					
				}
				else
				{
					GPIO_SetBits(GPIOB, GPIO_Pin_0); //���ö�Ӧ��ODRyλΪ1 
				}
		}
		if(capture2Flag==0x1)
		{
			capture2Flag = 0x0;
			
			count = TIM_GetCapture2(TIM3);
			//uart_printf("count2=0x%x\r\n",count);
			if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_1))
			{
				//�����Ӧ��ODRyλΪ0
				GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			}
			else
			{
				GPIO_SetBits(GPIOB, GPIO_Pin_1); //���ö�Ӧ��ODRyλΪ1 
			}
		}
	 }
}


//Tim1��ʱ�����ⲿ������ ͬ��ģʽ  ֮��λģʽ
void Tim3_sreset_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM3�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0

	//TI1_Config(TIMx, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI,0x00);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR�Ĵ�����TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Reset);	//SMCR�Ĵ�����SMS[2:0]bits

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������

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


//Tim1��ʱ�����ⲿ������  �ſ�ģʽ
void Tim3_gate_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	//TI1_Config(TIMx, TIM_ICPolarity_Falling, TIM_ICSelection_DirectTI,0x00);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	

	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR�Ĵ�����TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Gated);	//SMCR�Ĵ�����SMS[2:0]bits

	

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
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

//Tim1��ʱ�����ⲿ������  ����ģʽ
void Tim3_trigger_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_7;            //PA1 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0

	//TI2_Config(TIMx, TIM_ICPolarity_Falling, TIM_ICSelection_DirectTI,0x0000); 
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);
	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI2FP2);  //SMCR�Ĵ�����TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger);	//SMCR�Ĵ�����SMS[2:0]bits

	//TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
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

//�ⲿʱ��ģʽ2 + ����ģʽ
void Tim3_extclk2_trigger_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;            //PA6 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_2;            //PD2 etr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM3_IRQn);          //Tim3 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0x0fff);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_GenerateEvent(TIMx, TIM_EventSource_Update);//����  update�¼�

	TIM_ETRClockMode2Config(TIMx, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0000);
	//TI1_Config(TIMx, TIM_ICPolarity_Rising, TIM_ICSelection_DirectTI,0x00);
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIMx, &TIM_ICInitStruct);	

	
	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR�Ĵ�����TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Trigger);	//SMCR�Ĵ�����SMS[2:0]bits

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

