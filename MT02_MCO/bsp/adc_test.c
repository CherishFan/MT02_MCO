
#include "all.h"

void ADC_CheckStatus(ADC_TypeDef* ADCx)
{
	while(1)
	{
		if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
		{
			ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
			break;
		}
	}
	
}

void ADC1_SingleChannel(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 0, 1); //ADCHS 通道选择寄存器
	
	
  while(1)      //转换通道0数据
	{
		ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		
		while(1)
		{
				
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}
		ADC_SoftwareStartConvCmd(ADCx, DISABLE); //ADCR寄存器的ADST位使能，软件转换开始
		adconverdata = ADC_GetConversionValue(ADCx);
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
	}
}

void ADC1_ContinueChannel(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_8;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 0, 1); //ADCHS 通道选择寄存器
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
	//for(i=0;i<50000;i++)
	//{}
  while(1)      //转换通道0数据
	{
		while(1)
		{
				//uart_printf("ADC1->ADSTA=0x%x\r\n",ADCx->ADSTA);
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}

		//adconverdata = ADC_GetConversionValue(ADCx);
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
	}
}


void ADC1_SinglePeriod(ADC_TypeDef* ADCx)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
 //  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_All, 0, 1); //ADCHS 通道选择寄存器
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		

		while(1)
		{
				//uart_printf("ADC1->ADSTA=0x%x\r\n",ADCx->ADSTA);
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}

		
		ADC_SoftwareStartConvCmd(ADCx, DISABLE); //ADCR寄存器的ADST位使能，软件转换开始
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
		uart_printf("addr0=0x%x\r\n",ADC1->ADDR0);
		uart_printf("addr1=0x%x\r\n",ADC1->ADDR1);
		uart_printf("addr2=0x%x\r\n",ADC1->ADDR2);
		uart_printf("addr3=0x%x\r\n",ADC1->ADDR3);
		uart_printf("addr4=0x%x\r\n",ADC1->ADDR4);
		uart_printf("addr5=0x%x\r\n",ADC1->ADDR5);
		uart_printf("addr6=0x%x\r\n",ADC1->ADDR6);
		uart_printf("addr7=0x%x\r\n",ADC1->ADDR7);
		uart_printf("addr8=0x%x\r\n",ADC1->ADDR8);

}

void ADC1_compare(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开启GPIOA端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 
  ADC_AnalogWatchdogCmd(ADCx, ADC_AnalogWatchdog_SingleRegEnable);  //adc 窗口比较使能
	ADC_AnalogWatchdogSingleChannelConfig(ADCx, ADC_Channel_0);   //选择比较通道0转换结果

	ADC_AnalogWatchdogThresholdsConfig(ADCx, 0xaaa,0x333);  //比较数值上限和下限
	
	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 0, 1); //ADCHS 通道选择寄存器
	
	
  while(1)      //转换通道0数据
	{
		ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		
		while(1)
		{
				
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}
		
		ADC_SoftwareStartConvCmd(ADCx, DISABLE); //ADCR寄存器的ADST位使能，软件转换开始
		adconverdata = ADC_GetConversionValue(ADCx);
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
		
		//转换的值>=上限的值，以及<下限的值，比较标志位置位
		while(1)
		{
				
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_AWD))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_AWD);
				uart_printf("converdata is not specia range\r\n");
				break;
			}
		}
	}
}



void ADC2_SingleChannel(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 0, 0); //ADCHS 通道选择寄存器
	
  while(1)      //转换通道0数据
	{
		ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		
		//ADC_CheckStatus(ADCx);
		while(1)
		{
			if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}
		ADC_SoftwareStartConvCmd(ADCx, DISABLE);
		adconverdata = ADC_GetConversionValue(ADCx);
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
	}
}


void ADC2_ContinueChannel(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_8;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
  //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 0, 1); //ADCHS 通道选择寄存器
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
	//for(i=0;i<50000;i++)
	//{}
  while(1)      //转换通道0数据
	{
		while(1)
		{
				//uart_printf("ADC1->ADSTA=0x%x\r\n",ADCx->ADSTA);
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}

		//adconverdata = ADC_GetConversionValue(ADCx);
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
	}
}


void ADC2_SinglePeriod(ADC_TypeDef* ADCx)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
 //  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_All, 0, 1); //ADCHS 通道选择寄存器
	
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		

		while(1)
		{
				//uart_printf("ADC1->ADSTA=0x%x\r\n",ADCx->ADSTA);
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}

		
		ADC_SoftwareStartConvCmd(ADCx, DISABLE); //ADCR寄存器的ADST位使能，软件转换开始
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
		uart_printf("addr0=0x%x\r\n",ADC1->ADDR0);
		uart_printf("addr1=0x%x\r\n",ADC1->ADDR1);
		uart_printf("addr2=0x%x\r\n",ADC1->ADDR2);
		uart_printf("addr3=0x%x\r\n",ADC1->ADDR3);
		uart_printf("addr4=0x%x\r\n",ADC1->ADDR4);
		uart_printf("addr5=0x%x\r\n",ADC1->ADDR5);
		uart_printf("addr6=0x%x\r\n",ADC1->ADDR6);
		uart_printf("addr7=0x%x\r\n",ADC1->ADDR7);
		uart_printf("addr8=0x%x\r\n",ADC1->ADDR8);

}

void ADC2_compare(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 
  ADC_AnalogWatchdogCmd(ADCx, ADC_AnalogWatchdog_SingleRegEnable);  //adc 窗口比较使能
	ADC_AnalogWatchdogSingleChannelConfig(ADCx, ADC_Channel_0);   //选择比较通道0转换结果

	ADC_AnalogWatchdogThresholdsConfig(ADCx, 0xaaa,0x333);  //比较数值上限和下限
	
	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
//  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 0, 1); //ADCHS 通道选择寄存器
	
	
  while(1)      //转换通道0数据
	{
		ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		
		while(1)
		{
				
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}
		
		ADC_SoftwareStartConvCmd(ADCx, DISABLE); //ADCR寄存器的ADST位使能，软件转换开始
		adconverdata = ADC_GetConversionValue(ADCx);
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
		
		//转换的值>=上限的值，以及<下限的值，比较标志位置位
		while(1)
		{
				
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_AWD))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_AWD);
				uart_printf("converdata is not specia range\r\n");
				break;
			}
		}
	}
}


void ADC1_SinglePeriod_DMA(ADC_TypeDef* ADCx)
{
	unsigned int i;
	uint16_t adconverdata = 0;
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //dmaen
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	ADC_Cmd(ADCx, ENABLE);    //ADCFG 寄存器的ADEN位使能 


	/* Initialize the ADC_PRESCARE values */
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_2;
  /* Initialize the ADC_Mode member */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Single_Period;
	/* Initialize the ADC_TRGEN member */
	ADC_InitStructure.ADC_TRGEN = ADC_TRG_Disable;
  /* Initialize the ADC_ExternalTrigConv member */
 //  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* Initialize the ADC_DataAlign member */
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADCx, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADCx, ADC_Channel_All, 0, 1); //ADCHS 通道选择寄存器
	
	DMA_DeInit(DMA1_Channel4);
			
	ADC_DMACmd(ADCx, ENABLE);  //dma_en
	/*
	DMA_InitStructure.DMA_PeripheralBaseAddr = (ADC1_BASE + 0x18);
	DMA_InitStructure.DMA_MemoryBaseAddr = 0x20004000;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 8;
	DMA_InitStructure.DMA_PeripheralInc = DMA_SrcInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_DstInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_DST_PER = DST_PER_ACK0;
	DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK0;  //主要看这个 请求信号
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	*/
	ADC_SoftwareStartConvCmd(ADCx, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
		

		while(1)
		{
				//uart_printf("ADC1->ADSTA=0x%x\r\n",ADCx->ADSTA);
		  if(ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADCx, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADCx);
				break;
			}
		}

		
		ADC_SoftwareStartConvCmd(ADCx, DISABLE); //ADCR寄存器的ADST位使能，软件转换开始
		uart_printf("adconverdata=0x%x\r\n",adconverdata);
		uart_printf("addr0=0x%x\r\n",ADC1->ADDR0);
		uart_printf("addr1=0x%x\r\n",ADC1->ADDR1);
		uart_printf("addr2=0x%x\r\n",ADC1->ADDR2);
		uart_printf("addr3=0x%x\r\n",ADC1->ADDR3);
		uart_printf("addr4=0x%x\r\n",ADC1->ADDR4);
		uart_printf("addr5=0x%x\r\n",ADC1->ADDR5);
		uart_printf("addr6=0x%x\r\n",ADC1->ADDR6);
		uart_printf("addr7=0x%x\r\n",ADC1->ADDR7);
		uart_printf("addr8=0x%x\r\n",ADC1->ADDR8);
	
	
	DMAcheckStatus(DMA1_FLAG_TC4);
	
	DMA_Cmd(DMA1_Channel4, DISABLE); //disable dma_rxen and txen 
	
	ADC_DMACmd(ADCx, DISABLE);  //disable dma request
}
