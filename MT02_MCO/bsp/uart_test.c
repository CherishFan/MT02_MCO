
#include "all.h"

volatile unsigned char uart_tx_flag;
volatile unsigned char uart_rx_flag;

volatile unsigned char uart_rx_buf[32];
volatile unsigned char rx_count;
volatile unsigned char begin_tx;

void USART1_IRQHandler(void)
{
	unsigned char temp ;
	
	if(USART_GetITStatus(USART1, USART_IT_TXIEN))
	{
		USART_ClearITPendingBit(USART1, USART_IT_TXIEN);
		uart_tx_flag = 1;
	}
	else if(USART_GetITStatus(USART1, USART_IT_RXIEN))
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXIEN);
		uart_rx_flag = 1;
	}
	else
		uart_printf("UART tx and rxerror\r\n",temp);
}

void UARTRegs_test()
{
	uart_printf("USART1->TDR = 0x%x\r\n",USART1->TDR); //RW 0x0  ?
	uart_printf("USART1->RDR = 0x%x\r\n",USART1->RDR); //R  0x0
	uart_printf("USART1->CSR = 0x%x\r\n",USART1->CSR); //R  0x09  ?
	uart_printf("USART1->ISR = 0x%x\r\n",USART1->ISR); //R  0x0
	uart_printf("USART1->IER = 0x%x\r\n",USART1->IER); //RW 0x0
	uart_printf("USART1->ICR = 0x%x\r\n",USART1->ICR); //W  0x0
	uart_printf("USART1->GCR = 0x%x\r\n",USART1->GCR); //RW 0x0   ?
	uart_printf("USART1->CCR = 0x%x\r\n",USART1->CCR); //RW 0x30
	uart_printf("USART1->BRR = 0x%x\r\n",USART1->BRR); //RW 0x01  ?
														 
	uart_printf("USART2->TDR = 0x%x\r\n",USART2->TDR); //RW 0x0     
	uart_printf("USART2->RDR = 0x%x\r\n",USART2->RDR); //R  0x0     
	uart_printf("USART2->CSR = 0x%x\r\n",USART2->CSR); //R  0x09    
	uart_printf("USART2->ISR = 0x%x\r\n",USART2->ISR); //R  0x0     
	uart_printf("USART2->IER = 0x%x\r\n",USART2->IER); //RW 0x0     
	uart_printf("USART2->ICR = 0x%x\r\n",USART2->ICR); //W  0x0     
	uart_printf("USART2->GCR = 0x%x\r\n",USART2->GCR); //RW 0x0     
	uart_printf("USART2->CCR = 0x%x\r\n",USART2->CCR); //RW 0x30    
	uart_printf("USART2->BRR = 0x%x\r\n",USART2->BRR); //RW 0x01    
}


void Uart1Init(USART_TypeDef* USARTx)
{
	USART_InitTypeDef       USART_InitStructure;  
	NVIC_InitTypeDef        NVIC_InitStructure;           //定义NVIC初始化需要的数据结构体
	
	GPIO_InitTypeDef  GPIO_InitStructure;   
	
if(USARTx==USART1)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   //uart1_tx  pa9
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;  //uart1_rx  pa10
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入   
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
}	
 
if(USARTx==USART2)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;   //uart1_tx  pa2
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;  //uart1_rx  pa3
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入   
		GPIO_Init(GPIOA, &GPIO_InitStructure);
			
}	

if(USARTx==USART3)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;   //uart3_tx  pc10
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;  //uart3_rx  pc11
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入   
		GPIO_Init(GPIOC, &GPIO_InitStructure);
			
}	
	
#ifdef UART_DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	USART_DMACmd(USARTx, USART_DMAReq_EN, ENABLE);  //UART1_GCR = UART1_GCR | 0x0002;//dma mode
#endif
	
	USART_InitStructure.USART_BaudRate =  9600; //9600; 
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	
	USART_Init(USARTx, &USART_InitStructure);
	USART_Cmd(USARTx, ENABLE);  //UART 模块使能

#ifdef UART_INT_MODE
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;          //UART1中断号配置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //主优先级设置为0，占用0bit，从优先级可以有4bit（0-15）选择
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //数字越小优先级越高（主从优先级一共加起来不能超过4bit）
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //例如：若主优先级为3，占用2bit，从优先级可以有2bit可以选，即0-3
	NVIC_Init(&NVIC_InitStructure);   //上面4条语句是把设置的内容存入NVIC_InitStructure这个结构体，本函数是把结构体的内容写入相应寄存器
	uart_tx_flag = 0;
	uart_rx_flag = 0;
	
	USART_ITConfig(USARTx, USART_IT_TXIEN, ENABLE);		    //接收中断使能
	USART_ITConfig(USARTx, USART_IT_RXIEN, ENABLE);		    //接收中断使能
#else

	USART_ClearITPendingBit(USART1, 0xff); 
	USART_ClearITPendingBit(USART2, 0xff); 
#endif

//while(1)
//{
	uart_printf("1234\r\n");
//}
}

#ifdef UART_DMA
void outbyte(char c)
{
	DMA_InitTypeDef DMA_InitStructure;
	*((volatile unsigned int*)0x20003000) = (unsigned int)c;
	
  USART_DMACmd(USART1, USART_DMAReq_EN, ENABLE);   //DMA 使能
	
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_BASE;
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_DstInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_SrcInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK10;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK1;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	DMAcheckStatus(DMA1_FLAG_TC5);
	
#ifdef UART_INT_MODE
	
	while(1)
	{
		if(uart_tx_flag)
		{
			uart_tx_flag = 0;
			break;
		}
	}
#else

	while(1)
	{
		if(USART_GetITStatus(USART1, USART_IT_TXIEN))
		{
			 USART_ClearITPendingBit(USART1, USART_IT_TXIEN); 
			 break;
		}
		
	}
#endif

	DMA_Cmd(DMA1_Channel5, DISABLE);
	USART_DMACmd(USART1, USART_DMAReq_EN, DISABLE);   //disable DMA 

}

#else

void outbyte(char c)
{
#ifdef UART_INT_MODE
	USART_SendData(USART1, (uint16_t)c);
	while(1)
	{
		if(uart_tx_flag)
		{
			uart_tx_flag = 0;
			break;
		}
	}
#else

	while(1)
	{
		if(USART_GetFlagStatus(USART1, USART_FLAG_TXEPT))
			//if(USART_GetFlagStatus(USART2, USART_FLAG_TXEPT))
			break;
	}
	USART_SendData(USART1,(uint16_t)c);  
	//USART_SendData(USART2,(uint16_t)c);  
	while(1)
	{
		if(USART_GetITStatus(USART1, USART_IT_TXIEN))
		//	if(USART_GetITStatus(USART2, USART_IT_TXIEN))
		{
			 USART_ClearITPendingBit(USART1, USART_IT_TXIEN);
			//USART_ClearITPendingBit(USART2, USART_IT_TXIEN);
			 break;
		}
		
	}
#endif
}

#endif

#ifdef UART_DMA
unsigned char inbyte()
{
	unsigned char temp = 0;
	DMA_InitTypeDef DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel6);
	USART_DMACmd(USART1, USART_DMAReq_EN, ENABLE);   //DMA 使能
	DMA_InitStructure.DMA_PeripheralBaseAddr = (USART1_BASE + 0x04);
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20004000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_SrcInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_DstInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK0;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK11;
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6, ENABLE);
	
	DMAcheckStatus(DMA1_FLAG_TC6);
	
	DMA_Cmd(DMA1_Channel6, DISABLE);
	USART_DMACmd(USART1, USART_DMAReq_EN, DISABLE);   //disable DMA 

#else

unsigned char inbyte()
{
	unsigned char temp;

#ifdef UART_INT_MODE
	while(1)
	{
		if(uart_rx_flag)
		{
			uart_rx_flag = 0;
			break;
		}
	}	
#else
	while(1)
	{
		if(USART_GetITStatus(USART1, USART_IT_RXIEN))
		//	if(USART_GetITStatus(USART2, USART_IT_RXIEN))
		{
			USART_ClearITPendingBit(USART1, USART_IT_RXIEN);  //清除接受中断位
			//USART_ClearITPendingBit(USART2, USART_IT_RXIEN);  //清除接受中断位
			break;
		}
			
	}
#endif
	temp = (uint8_t)USART_ReceiveData(USART1); 
	//temp = (uint8_t)USART_ReceiveData(USART2); 
	return temp;
}
#endif


void Uart1RxTest()
{
	unsigned char i ;
	unsigned char temp;
	
	for(i=0;i<2;i++)
	{
		temp = inbyte();
		uart_printf("0x%x\r\n",temp);	
	}
					
}

void Uart1TXTest(unsigned long cpu_mhz,unsigned long uart_braud_rate,unsigned int parity,unsigned int stop,unsigned int charsbit)
{
	unsigned long temp;
	
	unsigned char i;
	temp = (cpu_mhz/16/uart_braud_rate);
   //RCC_APB2PhreialCLK(USART1CLK_EN);	//uart clk enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART1->CCR =  charsbit | parity | stop; 
 
	USART1->BRR = temp;
	USART1->GCR = USART1->GCR | 0x00000001; //UART 模块使能
#ifdef UART_INT_MODE
	NVIC2_IRQEnable(USART1_IRQn); //USART1_IRQn  37
	uart_tx_flag = 0;
	uart_rx_flag = 0;
	USART1->IER = 0x03;//TX and RX ????
#else
	USART1->IER = 0x00000000;
	USART1->ICR = 0xff;
#endif
	temp = cpu_mhz;
	temp = temp / 1000000;
	//uart_printf("System run at %dMHz\r\n",temp);
	
	for(i=0;i<2;i++)
	{
		uart_printf("uarttx=0x%x\r\n",i);
	}
	Uart1RxTest();
}


/*
void Uart1CtsTest(unsigned long cpu_mhz,unsigned long uart_braud_rate)
{
	unsigned long temp;
	unsigned char i;
	temp = (cpu_mhz/16/uart_braud_rate);
  	//RCC_APB2PhreialCLK(USART1CLK_EN);	//uart clk enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	

	UART1_CCR =  0x30; 
 
	UART1_BRR = temp;
	UART1_GCR = UART1_GCR | 0x00000005; //UART 模块使能 和 autoflow 使能
	UART1_IER = 0x00000000;
	UART1_ICR = 0xff;
	temp = cpu_mhz;
	temp = temp / 1000000;
	//uart_printf("System run at %dMHz\r\n",temp);
	
	for(i=0;i<2;i++)
	{
		uart_printf("uarttx=0x%x\r\n",i);
	}
	Uart1RxTest();
}

void Uart1Close()
{
	NVIC2_IRQDisable(USART1_IRQn); //USART1_IRQn  37
	uart_tx_flag = 0;
	uart_rx_flag = 0;
	UART1_IER = 0x00000000;
	UART1_ICR = 0xff;
	UART1_GCR = 0x00;
	
}
*/


