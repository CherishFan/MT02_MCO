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

extern uint16_t tim_flag;
extern unsigned char capture1Flag;
extern unsigned char capture3Flag;
extern unsigned char capture2Flag ;
extern unsigned char capture4Flag;
extern unsigned char triggerFlag;
extern unsigned char updateFlag ;


//ɲ���жϴ���
void TIM1_BRK_IRQHandler (void)
{

	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Break)!= RESET)	 //check BIF 
	{
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Break);
	}	
	
}


//�����жϴ���
void TIM1_TRG_COM_IRQHandler (void)
{

	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Trigger)!= RESET)	 //check TIF
	{
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Trigger);
	}	
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_COM)!= RESET)	 //check COMIF
	{
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_COM);
	}

}

void TIM1_UP_IRQHandler (void)
{
	
	//tim_flag = 1;
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update)!= RESET)	 //check UIFλ
	{
		updateFlag = 1;
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
	}	
	 //tim_flag ++;

//	TIM_PrescalerConfig(TIM1,tim_flag,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 
//	TIM1->RCR = 0x02; //���ڼ�������

}


void TIM1_CC_IRQHandler (void)
{

	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1)!= RESET)	 //check capture compare 1
	{
		capture1Flag = 0x1;
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC1);
	}	

	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC2)!= RESET)	 //check capture compare 2 
	{
		capture2Flag = 0x1;
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC2);
	}	

	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC3)!= RESET)	 //check capture compare 2
	{
		capture3Flag = 0x01;
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC3);
	}

	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC4)!= RESET)	 //check capture compare 2
	{
		capture4Flag = 0x01;
		TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC4);
	}
	
}

void Tim_channelOutput(TIM_TypeDef* TIMx,uint16_t channel)
{
	 TIMx->CCER = 0x0000;
	 TIMx->CCMR1 = 	TIMx->CCMR1 | 0x0000;
	 TIMx->CCER	= TIMx->CCER | channel;
}


//����������¼��������Ĵ�����ͣ���Զ���װ��ARR����ֵ��
void Tim1_UPCount_test1(TIM_TypeDef* TIMx,uint16_t counter)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx, TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ
	TIM_UpdateDisableConfig(TIMx, ENABLE);//UDIS bit ������ֹ����
	//TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Global); //URS ?��D????��?�� URS=0
	TIM_PrescalerConfig(TIMx,7,0x0000);     //��Ƶֵ,��ÿ30M/30=1us����һ��	 TIMx->PSC	TIMx->EGR�Ƿ��ֶ�������������
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR

	TIMx->RCR = 0x00;  //���ڼ�������
	TIM_Cmd(TIMx, ENABLE);   				  //ʹ��CEN_Set������������

	while(1)
	{
		ReadCount(TIMx);
	}
	
}


//��������¼�������APRE��ֵ��RCR��ֵ��PSC��ֵ
void Tim1_UPCount_test2(TIM_TypeDef* TIMx,uint16_t counter)
{
	unsigned char i;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx, TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, DISABLE);  //�Զ���װ��Ԥװ��������ֹ
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	//TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	TIM_PrescalerConfig(TIMx,7,0x0000);      //��Ƶֵ,��ÿ8M/8=1us����һ��	 TIMx->PSC
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR

	TIMx->RCR = 0x01;  //���ڼ�������
	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	TIM_SetAutoreload(TIMx, 0xff);		  //����ֵ���������ڸ����������  TIMx->ARR
	
	i = 0;
	while(1)
	{
		i++;
		if(TIM_GetFlagStatus(TIMx,TIM_FLAG_Update)!= RESET)	 //check UIFλ 
		{
			TIM_ClearITPendingBit(TIMx , TIM_FLAG_Update);
			break;
		}	
	}
	TIM_PrescalerConfig(TIMx,0x2,0x0000);      //��Ƶֵ,��ÿ30M/30=1us����һ��	 TIMx->PSC
	TIMx->RCR = 0x02;  //���ڼ�������
   while(1)
	{
		i++;
		if(TIM_GetFlagStatus(TIMx,TIM_FLAG_Update)!= RESET)	 //check UIFλ 
		{
			TIM_ClearITPendingBit(TIMx , TIM_FLAG_Update);
			break;
		}	
	}
	TIM_SetAutoreload(TIMx, 0x56);		  //����ֵ���������ڸ����������  TIMx->ARR
  	TIM_PrescalerConfig(TIMx,0x2,0x0000);      //��Ƶֵ,��ÿ30M/30=1us����һ��	 TIMx->PSC
	TIMx->RCR = 0x02;  //���ڼ�������
	
	uart_printf("i=0x%x\r\n",i);
	while(1)
	{
		ReadCount(TIMx);
	}
}

//����UG bit���ֶ�������������
void Tim1_UPCount_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
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
	
	TIM_PrescalerConfig(TIMx,0x1,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG 
	
	TIM_SetAutoreload(TIMx, counter);		  //����ֵ���������ڸ����������  TIMx->ARR

	//TIMx->RCR = 0x01;  //���ڼ�������
	TIM_Cmd(TIMx, ENABLE);   				//ʹ��CEN_Set������������
	
	while(1)
	{
		if(updateFlag)
		{
			updateFlag = 0x0;
			ReadCount(TIMx);
		}
	}
	
}


//����UG bit���ֶ�������������
void Tim1_DOWNCount_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

//	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��
//*/
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
void Tim1_CentAlignMode1_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
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
void Tim1_CentAlignMode2_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
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
void Tim1_CentAlignMode3_test3(TIM_TypeDef* TIMx,uint16_t counter)
{
		NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	updateFlag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_CenterAligned3); //���������ģʽ
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

//�ⲿʱ��Դģʽ1 �ⲿ��������	 ����ͨ��2
void Tim1_ExtClkMode1_test(TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	//tim1��ch1ͨ��  PA8 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
  TIM_TIxExternalClockConfig(TIMx, TIM_TIxExternalCLK1Source_TI1,TIM_ICPolarity_Rising, 0x0000);// ͨ��1 ����Ϊ���� IC1ӳ����TI1�� ,�����˲���������
	
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
void Tim1_ExtClkMode2_test(TIM_TypeDef* TIMx)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	//TIM1��etrͨ��  PA12 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//����ETF[3:0]=0  ETPS[1:0]=01  ETP=0 ECE=1
	TIM_ETRClockMode2Config(TIMx, TIM_ExtTRGPSC_DIV2, TIM_ExtTRGPolarity_NonInverted, 0x0000);

	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Regular); //URS ��������Դ URS=1
	TIM_SetAutoreload(TIMx, 0xff);

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
	while(1)
	{
		ReadCount(TIMx);
	}

}



//����Ƚ�ͨ��
void Tim1_capture_compare_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	//TIM1��ch1ͨ��  PA8   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//ch2ͨ�� PA9 
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	capture2Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIMͨ��1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIMͨ��2�Ĵ����ж�ʹ��
	
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
			temp = TIM_GetCapture1(TIM1);
			ReadCount(TIMx);
			uart_printf("ccr1=0x%x\r\n",temp);
			//break;
		}
		if(capture2Flag)
		{
			capture2Flag = 0x0;
			temp = TIM_GetCounter(TIM1);
			uart_printf("cnt=0x%x\r\n",temp);
			break;
		}
	}
}

//���벶��ģʽ
void Tim1_capture_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	unsigned int buffer[2] = {0,0};
	unsigned int i;
	uint16_t temp;

  GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
 

//TIM1��ch3ͨ��  PA10  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	capture3Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_IRQn);          //TIM1 ���ն�
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
		DMA_InitStructure.DMA_PeripheralBaseAddr = (TIM1_BASE+0x3c);
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
			temp = TIM_GetCapture3(TIM1);
			uart_printf("ccr3=0x%x\r\n",temp);
			break;
		}
	}

}

//PWM����ģʽ
void Tim1_PWM_capture_test(TIM_TypeDef* TIMx)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	//TIM1��ch1��ch2ͨ��  PA8  PA9
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	capture1Flag = 0x0;
	capture2Flag = 0x0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_IRQn);          //TIM1 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1�Ĵ����ж�ʹ��

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

	while(1)
	{
		if(capture1Flag)
		{
			capture1Flag = 0x0;
			temp = TIM_GetCapture1(TIM1);
			*((volatile unsigned short*)0x20003000) = temp;
			//uart_printf("ccr1=0x%x\r\n",temp);
			break;
		}
	
		//if(capture2Flag)
		//{
		//	capture2Flag = 0x0;
		//	temp = TIM_GetCapture2(TIM1);
		//	uart_printf("ccr2=0x%x\r\n",temp);
			//break;
		//}
	}
	
	
	/*
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
*/	
}

//����Ƚ�ģʽ
void Tim1_compare_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	uint16_t temp = 0x0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
  NVIC_InitTypeDef         NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	//TIM1��ch4ͨ��  PA11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	
	capture4Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_IRQn);          //TIM1 ���ն�
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = (TIM1_BASE+0x40);
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
			temp = TIM_GetCounter(TIM1);
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
void Tim1_PWM_edgeUp_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	//TIM1��ch4ͨ��  PA11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture4Flag = 0x0;
	///*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
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
  	TIM_OCInitStruct.TIM_Pulse = 0x001;    //0x04Ƶ��1.667MHz 600ns ����PPWM2ģʽ�£�����TIM PULSE��ֵ�ı䣬����ı䣬ֵԽС��highԽ��PWM1ģʽ��PWm2�෴ 
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
void Tim1_PWM_edgeDown_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	//Tim1 ch3  pa10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture3Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
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
  	TIM_OCInitStruct.TIM_Pulse = 0x004;   //0x04Ƶ��1.667MHz 600ns ���䣬pwm1ģʽ�£�����TIM PULSE��ֵ�ı䣬����ı䣬ֵԽС��lowԽ��PPWM2 ģʽ�෴��
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
void Tim1_PWM_CenterAligned_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	//Tim1 ch2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
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



//�����������������
void Tim1_output_dead_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;    //PA8   ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_13;   //ch1n  PB13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_CC_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_CC1, ENABLE);	//TIM1�Ĳ�׽����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR ��ֵ�仯Ӱ��Ƶ��
	TIM_PrescalerConfig(TIMx,0xf,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0007;                 //���Ĵ�ֵ���仯������
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIMx, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��

  	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  	TIM_BDTRInitStruct.TIM_DeadTime = 0x0;   //��������Ƶ��4.8us 208.3KHz  
  	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
	
	 //configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
}

// ɲ������
void Tim1_output_break_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_12;  //tim1_etr PA12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;    //PA9  ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_14;   //ch2n  PB14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_CC_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	//TIM_ITConfig(TIMx, TIM_IT_CC2, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0xff);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0xf,0x0001);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0080;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Set ;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIMx, &TIM_OCInitStruct);

	TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��

  	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������

	Delay(0x1f);
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;
  	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;
  	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  	TIM_BDTRInitStruct.TIM_DeadTime = 0x01;
  	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable;
  	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
	
	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

}

//���ⲿ�¼�ʱ���OCxREF�ź�
void Tim1_ETR_OCREF_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_12;            //PA12 etr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_11;            //PA11 ch4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture4Flag = 0x0;
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
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

//����pwm�Ĳ���
void Tim1_set_com1_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10;    //PA10  ch3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;   //ch3n  PB15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture3Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_CC_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	//TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_COM, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x9,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0007;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIMx, &TIM_OCInitStruct);

	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��
	TIM_CCPreloadControl(TIMx, ENABLE); //set cr2 reg ccpc bit
		
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;
  	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  	TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
	Delay(0x22);
	TIM_CCxCmd(TIMx, channel, TIM_CCx_Disable);	//disable CC1E��bit
	TIM_ForcedOC3Config(TIMx, TIM_ForcedAction_InActive); //set ccmr2 oc3m[2:0]	is 100
	TIM_GenerateEvent(TIMx, 0x0020);//���� com�¼�

}

//����pwm�Ĳ���
void Tim1_set_com2_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10;    //PA10  ch3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;   //ch3n  PB15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture3Flag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_CC_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	//TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_COM, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x9,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 

	TIM_OCInitStruct.TIM_OCMode = 0x0040; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Disable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0007;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIMx, &TIM_OCInitStruct);

	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��
	TIM_CCPreloadControl(TIMx, ENABLE); //set cr2 reg ccpc bit
		
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;
  	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  	TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
	Delay(0x22);
	TIM_CCxNCmd(TIMx, channel, TIM_CCxN_Enable); //enable CC1NE bit
	TIM_ForcedOC3Config(TIMx, TIM_ForcedAction_Active); //set ccmr2 oc3m[2:0]	is 100
	TIM_GenerateEvent(TIMx, 0x0020);//���� com�¼�
}

//����pwm�Ĳ���
void Tim1_set_com3_test(TIM_TypeDef* TIMx,uint16_t channel)
{
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //����GPIOA�˿�
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_10;    //PA10  ch3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;   //ch3n  PB15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	capture3Flag = 0x0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_CC_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	//TIM_ITConfig(TIMx, TIM_IT_CC3, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	TIM_ITConfig(TIMx, TIM_IT_COM, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	
	TIM_SetAutoreload(TIMx, 0x8);		  //����ֵ���������ڸ����������  TIMx->ARR
	TIM_PrescalerConfig(TIMx,0x9,0x0000);      //��Ƶֵ,	 TIMx->PSC	TIMx->EGR��bit0 UG �ֶ�����һ�������¼� 3us 333.3KHz

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 0x0007;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIMx, &TIM_OCInitStruct);

	TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);	//OCxPE ����Ƚ� Ԥװ��ʹ��
	TIM_CCPreloadControl(TIMx, ENABLE); //set cr2 reg ccpc bit
		
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;
  	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;
  	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  	TIM_BDTRInitStruct.TIM_DeadTime = 0x00;
  	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;
  	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;
  	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
	TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
	//configuer output
	TIM_CtrlPWMOutputs(TIMx, ENABLE);   // ����MOE bit

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
	Delay(0x38);
	TIM_CCxCmd(TIMx, channel, TIM_CCx_Disable);	//disable CC1E��bit
	TIM_CCxNCmd(TIMx, channel, TIM_CCxN_Disable); //enable CC1NE bit
	TIM_ForcedOC3Config(TIMx, TIM_ForcedAction_InActive); //set ccmr2 oc3m[2:0]	is 100
	TIM_GenerateEvent(TIMx, 0x0020);//���� com�¼�

}


//������ģʽ
void Tim1_opm_test(TIM_TypeDef* TIMx,uint16_t channel,uint16_t polarity,uint16_t selectPin,uint16_t pscclk,uint16_t filter)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;            //PA9 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;            //PA8 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture2Flag = 0x0;
	capture1Flag = 0x0;
//	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
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



//�������ӿ�ģʽ
void Tim1_program_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;            //PA8 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;            //PA9 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	capture2Flag = 0x0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
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
			count = TIM_GetCapture1(TIM1);
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
			
			count = TIM_GetCapture2(TIM1);
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
void Tim1_sreset_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;            //PA0 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIMx, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIMx, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIMx,TIM_UpdateSource_Global); //URS ��������Դ URS=0

	
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

	count = TIM_GetCounter(TIM1);
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
			//count = TIM_GetCapture1(TIM1);
			count = TIM_GetCounter(TIM1);
			uart_printf("count2=0x%x\r\n",count);
			//break;
		}
	}
}


//Tim1��ʱ�����ⲿ������  �ſ�ģʽ
void Tim1_gate_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;            //PA8 ch1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
	TIM_ITConfig(TIMx, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	TIM_SetAutoreload(TIMx, 0x0036);
	TIM_SetClockDivision(TIMx,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
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

	TIM_SelectInputTrigger(TIMx, TIM_TS_TI1FP1);  //SMCR�Ĵ�����TS[6:4] bits
	TIM_SelectSlaveMode(TIMx, TIM_SlaveMode_Gated);	//SMCR�Ĵ�����SMS[2:0]bits

	

	TIM_Cmd(TIMx, ENABLE);   				 //ʹ��CEN_Set������������
	
	count = TIM_GetCounter(TIM1);
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
			count = TIM_GetCounter(TIM1);
			uart_printf("count2=0x%x\r\n",count);
			//break;
		}
	}

}

//Tim1��ʱ�����ⲿ������  ����ģʽ
void Tim1_trigger_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_9;            //PA9 ch2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
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
			count = TIM_GetCounter(TIM1);
			uart_printf("count1=0x%x\r\n",count);
			//break;
		}
	}

}

//�ⲿʱ��ģʽ2 + ����ģʽ
void Tim1_extclk2_trigger_test(TIM_TypeDef* TIMx)
{
	uint16_t count = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	TIM_ICInitTypeDef TIM_ICInitStruct;
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_DeInit(TIMx);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //����GPIOA�˿�
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_8;            //PA8 ch1 etr
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);          //Tim1 ���ն�
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
		ReadCount(TIM1);
	}
	
}


/*--------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------
---------------------------------��ʱ��ͬ��---------------------------------------------------
----------------------------------------------------------------------------------------------
*/
//tim1��ʱ���ĸ����¼� Ϊ Tim2��Ԥ��Ƶ��
void Timx_syn_test1()
{
	uint16_t count = 0;
	//TIM_ICInitTypeDef TIM_ICInitStruct;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);

	tim_flag = 0x0;
	triggerFlag = 0x0;
	
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM1_UP_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	//NVIC1_IRQEnable(TIM2_IRQn);          //TIM2 ���ж�ʹ��
	TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	
	TIM_SetAutoreload(TIM1, 0x0089);
	TIM_SetClockDivision(TIM1,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIM1, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIM1, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_GenerateEvent(TIMx, TIM_EventSource_Update);//����  update�¼�
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);   //select  tim_cr2 ��mmsģʽ


	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0); //ѡ�� tim2��SMCR�Ĵ�����TS bitsΪ tim1

	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_External1);	//SMCR�Ĵ�����SMS[2:0]bits

	
	TIM_CounterModeConfig(TIM2,TIM_CounterMode_Up); //���������ģʽ
	TIM_SetAutoreload(TIM2, 0xff);		  //����ֵ���������ڸ����������  TIMx->ARR

	TIM_Cmd(TIM1, ENABLE);   				 //ʹ��CEN_Set������������
	TIM_Cmd(TIM2, ENABLE);   				 //ʹ��CEN_Set������������
/*
	while(1)
	{
		if(tim_flag)
		{
			tim_flag = 0x0;
			break;
		}	
	}

	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			count = TIM_GetCounter(TIM2);
			uart_printf("count=0x%x\r\n",count);
			break;
		}
	}
	
*/
}


//tim1��ʱ���� ʹ��  Tim2��ʱ��
void Timx_syn_test2()
{
	uint16_t count = 0;
	//TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);

	capture1Flag = 0x0;
	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	//NVIC1_IRQEnable(TIM1_CC_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);	//TIM1�ıȽ��ж�ʹ��
	
	//NVIC1_IRQEnable(TIM2_IRQn);          //TIM2 ���ж�ʹ��
	TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	
	TIM_SetAutoreload(TIM1, 0x0092);
	TIM_SetClockDivision(TIM1,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIM1, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIM1, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_GenerateEvent(TIMx, TIM_EventSource_Update);//����  update�¼�
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC1Ref);   //select  tim_cr2 ��mmsģʽ

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // ֵƥ��ʱ  OCxREF ��״̬  PWM1 ģʽ �Թ�����ģʽ
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0x004;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);


	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0); //ѡ�� tim2��SMCR�Ĵ�����TS bitsΪ tim1

	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);	//SMCR�Ĵ�����SMS[2:0]bits

	TIM_CounterModeConfig(TIM2,TIM_CounterMode_Up); //���������ģʽ
	TIM_SetAutoreload(TIM2, 0x55);		  //����ֵ���������ڸ����������  TIMx->ARR

	TIM_GenerateEvent(TIM1, TIM_EventSource_Update);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_SetCounter(TIM2, 0xE7);

	TIM_Cmd(TIM2, ENABLE);   				 //ʹ��CEN_Set������������
	TIM_Cmd(TIM1, ENABLE);   				 //ʹ��CEN_Set������������
	//Delay(0x0f);
   TIM_Cmd(TIM1, DISABLE);
   //TIM_Cmd(TIM2, DISABLE); 
/*
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
			count = TIM_GetCounter(TIM2);
			uart_printf("count=0x%x\r\n",count);
			break;
		}
	}
	
*/
	
}

//tim1��ʱ���ĸ����¼� ���� Tim2��ʱ��
void Timx_syn_test3()
{
	uint16_t count = 0;
	//TIM_ICInitTypeDef TIM_ICInitStruct;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);

	tim_flag = 0x0;
	triggerFlag = 0x0;
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQHandler;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/
	
	//NVIC1_IRQEnable(TIM1_UP_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��

	//NVIC1_IRQEnable(TIM2_IRQn);          //TIM2 ���ж�ʹ��
	TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	
	TIM_SetAutoreload(TIM1, 0x0089);
	TIM_SetClockDivision(TIM1,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIM1, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIM1, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_GenerateEvent(TIMx, TIM_EventSource_Update);//����  update�¼�
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Update);   //select  tim_cr2 ��mmsģʽ


	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0); //ѡ�� tim2��SMCR�Ĵ�����TS bitsΪ tim1

	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);	//SMCR�Ĵ�����SMS[2:0]bits

	
	TIM_CounterModeConfig(TIM2,TIM_CounterMode_Up); //���������ģʽ
	TIM_SetAutoreload(TIM2, 0xff);		  //����ֵ���������ڸ����������  TIMx->ARR

	TIM_Cmd(TIM1, ENABLE);   				 //ʹ��CEN_Set������������
	Delay(0xf);
	TIM_Cmd(TIM2, DISABLE);
/*
	while(1)
	{
		if(tim_flag)
		{
			tim_flag = 0x0;
			break;
		}	
	}

	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			count = TIM_GetCounter(TIM2);
			uart_printf("count=0x%x\r\n",count);
			break;
		}
	}
	
*/
}

//һ���ⲿ���� ͬ��������������ʱ��
void Timx_syn_test4()
{
	uint16_t count = 0;
	//TIM_ICInitTypeDef TIM_ICInitStruct;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO��ʼ���ṹ�����
	NVIC_InitTypeDef         NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);

	tim_flag = 0x0;
	triggerFlag = 0x0;

/*	
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQHandler;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//NVIC1_IRQEnable(TIM1_UP_IRQn);	   //cm3��tim1 ���жϺ�ʹ��
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);	//TIM1�ĸ����¼��ж�ʹ��


	//NVIC1_IRQEnable(TIM2_IRQn);          //TIM2 ���ж�ʹ��
	TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	
	
	TIM_SetAutoreload(TIM1, 0x0089);
	TIM_SetClockDivision(TIM1,TIM_CKD_DIV1);  //������Ƶ	 TIMx->CR1
	//TIM_CounterModeConfig(TIMx,TIM_CounterMode_Up); //���������ģʽ
	TIM_ARRPreloadConfig(TIM1, ENABLE);  //�Զ���װ��Ԥװ��������ֹ	APRE  ʹ��APRE
	TIM_UpdateDisableConfig(TIM1, DISABLE);//UDIS bit ������ֹ����
	TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Global); //URS ��������Դ URS=0
	//TIM_GenerateEvent(TIMx, TIM_EventSource_Update);//����  update�¼�
	TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_Enable);   //select  tim_cr2 ��mmsģʽ
	TIM_SelectInputTrigger(TIM1, TIM_TS_TI1F_ED);
	TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Trigger);	//SMCR�Ĵ�����SMS[2:0]bits
	TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable); //SMCR�Ĵ�����MSM bit

	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0); //ѡ�� tim2��SMCR�Ĵ�����TS bitsΪ tim1

	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);	//SMCR�Ĵ�����SMS[2:0]bits

	
	TIM_CounterModeConfig(TIM2,TIM_CounterMode_Up); //���������ģʽ
	TIM_SetAutoreload(TIM2, 0xff);		  //����ֵ���������ڸ����������  TIMx->ARR

	//TIM_Cmd(TIM1, ENABLE);   				 //ʹ��CEN_Set������������
	//Delay(0xf);
	//TIM_Cmd(TIM2, DISABLE);
/*
	while(1)
	{
		if(tim_flag)
		{
			tim_flag = 0x0;
			break;
		}	
	}

	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			count = TIM_GetCounter(TIM2);
			uart_printf("count=0x%x\r\n",count);
			break;
		}
	}
	
*/
}

//���������� �ӿ�	 ��Ҫ��������ʱ�����ʹ��
void Tim1_passSensor_test()
{
	uint16_t count = 0;
	NVIC_InitTypeDef         NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIM1);
	TIM_DeInit(TIM2);

	triggerFlag = 0x0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//NVIC1_IRQEnable(TIM2_IRQn);          //TIM3 ���ж�ʹ��
	TIM_ITConfig(TIM2, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
	/*
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);             //ʹ�õ����ȼ�Ⱥ�飬������0-4                                             
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQHandler;           //�����ж�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //���ʹ�����ȼ�Ⱥ��0�������ȼ�������0-15
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/
	//NVIC1_IRQEnable(TIM1_UP_IRQHandler);	   //cm3��tim1 ���жϺ�ʹ��
	TIM_ITConfig(TIM1, TIM_IT_Trigger, ENABLE);	//TIM1�Ĵ����ж�ʹ��
   	//TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_OC2Ref); //CR2  ��MMS �� 101 OC2REF
    //TIM_SelectHallSensor(TIM2, ENABLE);//CR2 �Ĵ�����TI1s λ Ϊ1��ch1 ch2 ch3 �ܽž��������TI1S
    TIM2->CR2|=0xd0;//TI1S=1;ch1,ch2,ch3 input xor,MMS=101,OC2REF as trigger output
	TIM_SetAutoreload(TIM2, 0xffff);	
    
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset); //SMCR �Ĵ�����SMS �� 100 ��λģʽ
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI1F_ED);  //SMCR �Ĵ�����TS �� 100 ����TI2FP2
	
 
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
	//TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;   // TIM_ICPolarity_Rising TIM_ICPolarity_Falling
  	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_TRC;
  	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStruct.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM2, &TIM_ICInitStruct);	

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // ǿ��Ϊ��
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Disable;
  	TIM_OCInitStruct.TIM_Pulse = 0xc600;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);

	TIM_SetCounter(TIM2, 0xc000);
	TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	TIM_Cmd(TIM2, ENABLE);   				 //ʹ��CEN_Set������������

	TIM_SetAutoreload(TIM1, 0x77);	
	//TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset); //SMCR �Ĵ�����SMS �� 100 ��λģʽ
	TIM_SelectInputTrigger(TIM1, TIM_TS_ITR1);  //SMCR �Ĵ�����TS �� 100 ����TI2FP2
	

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // ǿ��Ϊ��
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 0x44;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Inactive; // ǿ��Ϊ��
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 0x44;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Active; // ǿ��Ϊ��
  	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputState_Enable;
  	TIM_OCInitStruct.TIM_Pulse = 0x44;
  	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);

	TIM_CCPreloadControl(TIM1, ENABLE); //CR2 CCPC bit
	TIM_SelectCOM(TIM1, ENABLE);  //CR2 CCUS bit
    
	TIM_CtrlPWMOutputs(TIM1, ENABLE);   		// ����MOE bit	 BDTR  �Ĵ���

    TIM_Cmd(TIM1, ENABLE);

	while(1)
	{
		if(triggerFlag == 0x01)
		{
			triggerFlag = 0x0;
			count = TIM_GetCapture1(TIM2);
			uart_printf("ccr1=0x%x\r\n",count);
			break;
		}
	}

   	count = TIM_GetCapture1(TIM1);
	uart_printf("count=0x%x\r\n",count);

	Delay(0x1f);
	count = TIM_GetCapture1(TIM1);
	uart_printf("count=0x%x\r\n",count);
			
}


