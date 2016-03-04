


/* Includes ------------------------------------------------------------------*/
#include "all.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DAC_DHR12R2_ADDRESS      0x40007414
#define DAC_DHR8R1_ADDRESS       0x40007410


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
DAC_InitTypeDef            DAC_InitStructure;
//DMA_InitTypeDef            DMA_InitStructure;


void DAC_Config(void) //软件使能开始
{
	uint16_t i;
	uint16_t readData = 0;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_Mode_Out_OD; //GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* DAC channel2 Configuration */
   DAC_DeInit(); 
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    /* DAC Channel1 Init     DAC Channel2 Init */
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    //DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* Enable DAC Channel2   Enable DAC Channel2 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
    //DAC_Cmd(DAC_Channel_2, ENABLE);
		
		while(1)
		{
		for(i=0;i<0xfff;i++)
			//for(i=0xfff;i>0;i--)
		{
			//DAC_SetChannel2Data(DAC_Align_12b_R, 0x0333);
			//DAC_SetChannel2Data(DAC_Align_12b_R, i);
			//DAC_SetChannel2Data(DAC_Align_12b_L, i);
			
			
			DAC_SetChannel1Data(DAC_Align_12b_R, i);
			
			//DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
			DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
			
			//readData = DAC_GetDataOutputValue(DAC_Channel_2);
			//uart_printf("ch2_readData=0x%x\r\n",readData);
			readData = DAC_GetDataOutputValue(DAC_Channel_1);
			uart_printf("ch1_readData=0x%x\r\n",readData);
		}
	}
}


void DAC_HardTrigger(void) //硬件触发
{
	uint16_t i;
	uint16_t readData = 0;
  GPIO_InitTypeDef GPIO_InitStructure;

  
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_Mode_Out_OD; //GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* DAC channel2 Configuration */
   DAC_DeInit(); 
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    /* DAC Channel1 Init     DAC Channel2 Init */
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* Enable DAC Channel2   Enable DAC Channel2 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
		
		while(1)
		{
		for(i=0;i<0xfff;i++)
		{
			//DAC_SetChannel2Data(DAC_Align_12b_R, 0x0333);
			//DAC_SetChannel2Data(DAC_Align_12b_R, i);
			DAC_SetChannel2Data(DAC_Align_12b_L, i);
			
			
			DAC_SetChannel1Data(DAC_Align_8b_R, i);
			
			DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
			DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
			
			readData = DAC_GetDataOutputValue(DAC_Channel_2);
			uart_printf("ch2_readData=0x%x\r\n",readData);
			readData = DAC_GetDataOutputValue(DAC_Channel_1);
			uart_printf("ch1_readData=0x%x\r\n",readData);
		}
	}
}


void ADC_To_DAC_Config(void) //软件使能开始
{
	uint16_t i;
	uint16_t readData = 0;
	uint16_t adconverdata = 0;
  GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef  ADC_InitStructure;//定义AD初始化结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	/* Configure PA.0 (ADC_INPUT0) as analog */
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_Mode_Out_OD; //GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);    //ADCFG 寄存器的ADEN位使能 
	
	
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
	ADC_Init(ADC1, &ADC_InitStructure);
		
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 0, 1); //ADCHS 通道选择寄存器
	 /* DAC channel2 Configuration */
   DAC_DeInit(); 
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    /* DAC Channel1 Init     DAC Channel2 Init */
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    //DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* Enable DAC Channel2   Enable DAC Channel2 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
    //DAC_Cmd(DAC_Channel_2, ENABLE);
		
		//ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ADCR寄存器的ADST位使能，软件转换开始
	i=0;
  while(1)      //转换通道0数据
	{
		
		while(1)
		{
				//uart_printf("ADC1->ADSTA=0x%x\r\n",ADCx->ADSTA);
		  if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
			{
				ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
				adconverdata = ADC_GetConversionValue(ADC1);
				
				DAC_SetChannel1Data(DAC_Align_12b_R, adconverdata);

				DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
				
				
				//readData = DAC_GetDataOutputValue(DAC_Channel_1);
				//uart_printf("ch1_readData=0x%x\r\n",readData);
				break;
			}
		}
	}
}


void DAC_DMA(void) //软件使能开始
{
	uint16_t i;
	uint16_t readData = 0;
  GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_Mode_Out_OD; //GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* DAC channel1 Configuration */
   DAC_DeInit(); 
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    /* DAC Channel1 Init*/
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
   
    /* Enable DAC Channel1   Enable DAC Channel2 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
   
		DMA_DeInit(DMA1_Channel3);
		for(i=0;i<0xfff;i++)
		{
			*((volatile unsigned int*)(0x20003000+i)) = i;
		}
		/*
		DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_BASE+0x08;
		DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
		DMA_InitStructure.DMA_BufferSize = 250;  //4096;
		DMA_InitStructure.DMA_PeripheralInc = DMA_DstInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_SrcInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
		DMA_InitStructure.DMA_DST_PER = DST_PER_ACK14; //according this request
		DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK0;
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);
		DMA_Cmd(DMA1_Channel3, ENABLE);
	*/
	
		DAC_DMACmd(DAC_Channel_1, ENABLE);//dma_en
	  DMAcheckStatus(DMA1_FLAG_TC3);
	  DMA_Cmd(DMA1_Channel3, DISABLE);
	
	
	  DAC_DMACmd(DAC_Channel_1, DISABLE);//dma_rxen
		for(i=0;i<0xfff;i++)
		{
			DAC_SetChannel1Data(DAC_Align_12b_R, i);
			
			//DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
			DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
			
			//readData = DAC_GetDataOutputValue(DAC_Channel_2);
			//uart_printf("ch2_readData=0x%x\r\n",readData);
			readData = DAC_GetDataOutputValue(DAC_Channel_1);
			uart_printf("ch1_readData=0x%x\r\n",readData);
		}
	
}


void DAC_to_Compare(void) //软件使能开始
{
	uint16_t i;
	uint16_t readData = 0;
	uint32_t outState =0;
	
	COMP_InitTypeDef COMP_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);
	
  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	
  /* Configure PA.04 PA.05(DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_Mode_Out_OD; //GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PB作为 输出观察 PIN
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 /* DAC channel2 Configuration */
   DAC_DeInit(); 
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    /* DAC Channel1 Init     DAC Channel2 Init */
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* Enable DAC Channel2   Enable DAC Channel2 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
		
		
	COMP_InitStructure.COMP_InvertingInput = COMP_InvertingInput_DAC1; //COMP_InvertingInput_VREFINT;  //;COMP_InvertingInput_IO1
  COMP_InitStructure.COMP_NonInvertingInput = COMP_NonInvertingInput_IO6; //PA5  正相
  COMP_InitStructure.COMP_Output = COMP_Output_None;
  COMP_InitStructure.COMP_BlankingSrce = COMP_BlankingSrce_None;
  COMP_InitStructure.COMP_OutputPol = COMP_OutputPol_NonInverted;
  COMP_InitStructure.COMP_Hysteresis = COMP_Hysteresis_High;
  COMP_InitStructure.COMP_Mode = COMP_Mode_UltraLowPower;
	COMP_Init(COMP_Selection_COMP1, &COMP_InitStructure);
	
	
		//DAC->CR = 0x00030003;
		//while(1)
		//{
		for(i=0;i<0xfff;i++)
			//for(i=0xfff;i>0;i--)
		{
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1369); //0mV上身
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1368); //0mV下降
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1383);//9mV  上身沿
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1353);//9mV 下降沿
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1408);//18mV  上身沿
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1331);//18mV 下降沿
			//DAC_SetChannel2Data(DAC_Align_12b_R, 1425);//27mV  上身沿
			DAC_SetChannel2Data(DAC_Align_12b_R, 1316);//27mV 下降沿
			//DAC_SetChannel2Data(DAC_Align_12b_R, i);
			//DAC_SetChannel2Data(DAC_Align_12b_R, (0xfff-i));
			//DAC_SetChannel2Data(DAC_Align_12b_L, i);
			
			DAC_SetChannel1Data(DAC_Align_12b_R, 0x0555);
			//DAC_SetChannel1Data(DAC_Align_12b_R, (0xfff-i));
			
			DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
			DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
			
			Delay(100);
			COMP_Cmd(COMP_Selection_COMP1, ENABLE);
			outState = COMP_GetOutputLevel(COMP_Selection_COMP1);
		//outState = COMP1->CSR;
		GPIOB->ODR = (outState>>30);
		
		//uart_printf("outState%d=0x%x\r\n",i,(outState>>30));
		uart_printf("outState%d=0x%x\r\n",(0xfff-i),(outState>>30));
		}
	//}
}


void DAC_to_OP(void) //软件使能开始
{
	uint16_t i;
	uint16_t readData = 0;
	uint32_t outState =0;
	
	OPAMP_InitTypeDef OPAMP_InitTypeDefStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* DAC Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_COMP, ENABLE);  //开启GPIOA端口
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //开启ADC1端口
	
  /* GPIOA clock enable */
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //开启GPIOB端口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //开启GPIOC端口
	
  /* Configure PA.04 PA.05(DAC_OUT1) as analog */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_Mode_Out_OD; //GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADC_Cmd(ADC1, ENABLE);    //ADCFG 寄存器的ADEN位使能 

	
	//PB作为 输出观察 PIN
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 /* DAC channel2 Configuration */
   DAC_DeInit(); 
   DAC_InitStructure.DAC_Trigger = DAC_Trigger_None ;
   DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
   DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

    /* DAC Channel1 Init     DAC Channel2 Init */
		DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* Enable DAC Channel2   Enable DAC Channel2 */
		DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
		
	//PA4 输出  PA4、PA7短接,PA5由DA输出 一个 三角波
	OPAMP_InitTypeDefStructure.OPAMP_NonInvertingInput = OPAMP_NonInvertingInput_IO1; //PA7 正向
  OPAMP_InitTypeDefStructure.OPAMP_InvertingInput = OPAMP_InvertingInput_IO2;  //PA5 反相
	
	OPAMP_Init(OPAMP_Selection_OPAMP1, &OPAMP_InitTypeDefStructure);
	
		while(1)
		{
		OPAMP_Cmd(OPAMP_Selection_OPAMP1, ENABLE);
		for(i=0;i<0xfff;i++)
		{
			DAC_SetChannel2Data(DAC_Align_12b_R, i);
			DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
		
		}
		for(i=0xfff;i>0;i--)
		{
			DAC_SetChannel2Data(DAC_Align_12b_R, i);
			DAC_SoftwareTriggerCmd(DAC_Channel_2, ENABLE);
		
		}
	}
}
