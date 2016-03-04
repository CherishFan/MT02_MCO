#include "all.h"
#include "spi.h"
unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi_rx_flag;
volatile unsigned char spi_tx_flag;

unsigned char dma3Flag = 0x0;
unsigned char dma4Flag = 0x0;

void SPI1_IRQHandler (void)
{
	//uart_printf("spi1 iqr\r\n");
	if(SPI_GetITStatus(SPI1, SPI_IT_TXEPT))
	{
		SPI_ClearITPendingBit(SPI1, SPI_IT_TXEPT);
		spi_tx_flag = 1;
	}
	
	if(SPI_GetITStatus(SPI1, SPI_IT_RX))
	{
		SPI_ClearITPendingBit(SPI1, SPI_IT_RX);		 //clear rx interrupt
		spi_rx_flag = 1; 
	}		
}

void SPI2_IRQHandler (void)
{
	//uart_printf("spi1 iqr\r\n");
	if(SPI_GetITStatus(SPI2, SPI_IT_TXEPT))
	{
		SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
		spi_tx_flag = 1;
	}
	
	if(SPI_GetITStatus(SPI2, SPI_IT_RX))
	{
		SPI_ClearITPendingBit(SPI2, SPI_IT_RX);		 //clear rx interrupt
		spi_rx_flag = 1; 
	}		
}

void DMA1_Channel3_IRQHandler()
{
	if(DMA_GetITStatus(DMA1_IT_TC3))
	{
			DMA_ClearITPendingBit(DMA1_IT_TC3);
			dma3Flag = 0x1;
	}
}

void DMA1_Channel4_IRQHandler()
{
	if(DMA_GetITStatus(DMA1_IT_TC4))
	{
			DMA_ClearITPendingBit(DMA1_IT_TC4);
			dma4Flag = 0x1;
	}
}



void SPIM_CSLow(SPI_TypeDef* SPIx)
{
	//Spi cs assign to this pin,select
	SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Reset);
}

void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
	//Spi cs release
	SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set);
}

void SPIM_TXEn(SPI_TypeDef* SPIx)
{
	//Transmit Enable bit TXEN
	SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

void SPIM_TXDisable(SPI_TypeDef* SPIx)
{
	//disable TXEN
	SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Tx);
}

void SPIM_RXEn(SPI_TypeDef* SPIx)
{
	//enable RXEN
	SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

void SPIM_RXDisable(SPI_TypeDef* SPIx)
{
	//disable RXEN
	SPI_BiDirectionalLineConfig(SPIx, SPI_Disable_Rx);
}


void SPIMSendByte(SPI_TypeDef* SPIx, unsigned char tx_data)
{
	//SPI mode0 and clk is fclk/32 and enable spi
	SPI_SendData(SPIx, tx_data);
	
#ifdef SPI_INT_MODE
	while (1)
	{
		if(spi_tx_flag) 			//tx fifo and tx shift reg are all empty	 	
		{
			spi_tx_flag = 0;	 	//clear flag 
			break;
		}
	}
#else	
	while (1)
	{
		
		if(SPI_GetITStatus(SPIx, SPI_IT_TXEPT))	
		{
			//clear tx_intf flag
			SPI_ClearITPendingBit(SPIx, SPI_IT_TXEPT);
			break;
		}
	}

#endif	

}

unsigned char SPIM_Readbytes(SPI_TypeDef* SPIx,unsigned int number)
{
	unsigned int count = 0;
	unsigned char temp;
	SPIM_RXEn(SPIx); 
#ifdef SPI_INT_MODE	  //中断接受函数
	while(1)
	{
		if(spi_rx_flag)
	 	{
			spi_rx_flag = 0;	 //clear interrupt
			
			if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) 				
			{
				temp = (unsigned char)SPI_ReceiveData(SPIx);
				break;
			}	
		}
	}	
#else
	while(1)
	{
		if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) 
		{
			temp = (unsigned char)SPI_ReceiveData(SPIx);
			break;
		}
		
	}
#endif
	SPIM_RXDisable(SPIx); 
	return temp;
}	

void SPIM_ReadID(SPI_TypeDef* SPIx)
{
	
	unsigned int i;
	
	SPIM_CSLow(SPIx);																			//Spi cs assign to this pin,select
	SPIM_TXEn(SPIx); 																			//Transmit Enable bit TXEN

	SPIMSendByte(SPIx,RDID);
	
	SPIM_TXDisable(SPIx); 																	//disable TXEN
	
	SPIM_Readbytes(SPIx,0x01); 	
	SPIM_Readbytes(SPIx,0x01);
	SPIM_Readbytes(SPIx,0x01);

	SPIM_CSHigh(SPIx);   																 //Spi cs release
}



void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
	SPIM_CSLow(SPIx); 																 //Spi cs assign to this pin,select
	SPIM_TXEn(SPIx); 																 //Transmit Enable bit TXEN
	SPIMSendByte(SPIx,WREN); 													 //SPDAT = WREN;CheckStatus();
	SPIM_TXDisable(SPIx); 														 //disable TXEN
	SPIM_CSHigh(SPIx); 															 //Spi cs release
}

void SPIM_checkStatus(SPI_TypeDef* SPIx)
{
	unsigned char temp;
	SPIM_CSLow(SPIx); 															 //Spi cs assign to this pin,select
	SPIM_TXEn(SPIx); 															//Transmit Enable bit TXEN
	SPIMSendByte(SPIx,RDSR);            						//SPDAT = RDSR; CheckStatus();
	
	SPIM_TXDisable(SPIx); 	//disable TXEN											
	
	while(1)
	{
		 temp = SPIM_Readbytes(SPIx,0x01);
	 	 if((temp&0x01)==0x0)
	 			break;
	}
	SPIM_CSHigh(SPIx);  											//Spi cs release
}

void SPIM_WriteDisable(SPI_TypeDef* SPIx)
{
	SPIM_CSLow(SPIx);  								//Spi cs assign to this pin,select
	SPIM_TXEn(SPIx); 									//Transmit Enable bit TXEN
	SPIMSendByte(SPIx,WRDI); 						//SPDAT = WREN;CheckStatus();
	SPIM_TXDisable(SPIx);						 //disable TXEN
	SPIM_CSHigh(SPIx);  							//Spi cs release
}

void SPIM_PageRead(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)  //page = 256 bytes
{
	unsigned char addr0,addr1,addr2;
	unsigned int i;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	address = address&0xffffff00; //page address
	addr0 = (unsigned char)(address>>16);
	addr1 = (unsigned char)(address>>8);
	addr2 = (unsigned char)address;
	
	SPIM_CSLow(SPIx); 								 //Spi cs assign to this pin,select
	SPIM_TXEn(SPIx);  								//Transmit Enable bit TXEN
	SPIMSendByte(SPIx,READ); 					//SPDAT = PP;CheckStatus();
	SPIMSendByte(SPIx,addr0);
	SPIMSendByte(SPIx,addr1);
	SPIMSendByte(SPIx,addr2);
	SPIM_TXDisable(SPIx);					 //disable TXEN

	for(i=0;i<256;i++)
	{
		rxtmpdata[i] = SPIM_Readbytes(SPIx,0x01);
		//uart_printf("rxtmpdata[%d]=0x%x\r\n",i,rxtmpdata[i]);
	}

	
	SPIM_CSHigh(SPIx);  					//Spi cs release
}

void SPIM_PageProgram(SPI_TypeDef* SPIx,unsigned long address,unsigned char *p,unsigned int number)
{
	unsigned int j;
	unsigned char addr0,addr1,addr2;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	address = address&0xffffff00; //page address
	addr0 = (unsigned char)(address>>16);
	addr1 = (unsigned char)(address>>8);
	addr2 = (unsigned char)address;
		
	SPIM_WriteEnable(SPIx);

	SPIM_CSLow(SPIx);  				//Spi cs assign to this pin,select
	SPIM_TXEn(SPIx); 				  //Transmit Enable bit TXEN
	SPIMSendByte(SPIx,PP); 			//SPDAT = PP;CheckStatus();
	SPIMSendByte(SPIx,addr0);
	SPIMSendByte(SPIx,addr1);
	SPIMSendByte(SPIx,addr2);

	for(j=0;j<number;j++)
	{
		SPIMSendByte(SPIx,*(p++));
	}
	
		
	SPIM_TXDisable(SPIx); 			//disable TXEN
	SPIM_CSHigh(SPIx);  				//Spi cs release

	SPIM_checkStatus(SPIx);
}







void SPIM_BlockErase(SPI_TypeDef* SPIx)
{
	SPIM_WriteEnable(SPIx);
	
	SPIM_CSLow(SPIx);  		//Spi cs assign to this pin,select
	SPIM_TXEn(SPIx); 			//Transmit Enable bit TXEN
	SPIMSendByte(SPIx,BE); 	//SPDAT = PP;CheckStatus();
	SPIM_TXDisable(SPIx); 	//disable TXEN
	SPIM_CSHigh(SPIx);  		//Spi cs release
	
	SPIM_checkStatus(SPIx);
}






void SPIM1_CSLow()
{
	//Spi cs assign to this pin,select
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Reset);
}

void SPIM1_CSHigh()
{
	//Spi cs release
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
}

void SPIM1_TXEn()
{
	//Transmit Enable bit TXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);
}

void SPIM1_TXDisable()
{
	//disable TXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Disable_Tx);
}

void SPIM1_RXEn()
{
	//enable RXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
}

void SPIM1_RXDisable()
{
	//disable RXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Disable_Rx);
}


void SPIM1SendByte(unsigned char tx_data)
{
	//SPI mode0 and clk is fclk/32 and enable spi
	SPI_SendData(SPI1, tx_data);
	
#ifdef SPI_INT_MODE
	while (1)
	{
		if(spi_tx_flag) 			//tx fifo and tx shift reg are all empty	 	
		{
			spi_tx_flag = 0;	 	//clear flag 
			break;
		}
	}
#else	
	while (1)
	{
		
		if(SPI_GetITStatus(SPI1, SPI_IT_TXEPT))	
		{
			//clear tx_intf flag
			SPI_ClearITPendingBit(SPI1, SPI_IT_TXEPT);
			break;
		}
	}

#endif	

}

unsigned char SPIM1_Readbytes(unsigned int number)
{
	unsigned int count = 0;
	unsigned char temp;
	SPIM1_RXEn(); 
#ifdef SPI_INT_MODE	  //中断接受函数
	while(1)
	{
		if(spi_rx_flag)
	 	{
			spi_rx_flag = 0;	 //clear interrupt
			
			if(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXAVL)) 				
			{
				temp = (unsigned char)SPI_ReceiveData(SPI1);
				break;
			}	
		}
	}	
#else
	while(1)
	{
		if(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXAVL)) 
		{
			temp = (unsigned char)SPI_ReceiveData(SPI1);
			break;
		}
		
	}
#endif
	SPIM1_RXDisable(); 
	return temp;
}	



void SPIM1_Init(unsigned short spi_baud_div)
{
	unsigned long temp;
  SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI1 clk enable
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;   //spi1_cs  pa4
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   //spi1_sck  pa5
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   //spi1_mosi  pa7
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //spi1_miso  pa6
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入   
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
#ifdef SPI_DMA	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //dmaen
	
#endif
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE); //Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率

#ifdef SPI_INT_MODE
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_ITConfig(SPI1, SPI_IT_TX|SPI_IT_RX, ENABLE);
	spi_rx_flag = 0;
	spi_tx_flag = 0;
#endif

}

void SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
{
	unsigned long temp;
  SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
		

if(SPIx==SPI1)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI1 clk enable
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;   //spi1_cs  pa4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   //spi1_sck  pa5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   //spi1_mosi  pa7
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //spi1_miso  pa6
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入   
	GPIO_Init(GPIOA, &GPIO_InitStructure);
#ifdef SPI_INT_MODE
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_ITConfig(SPI1, SPI_IT_TX|SPI_IT_RX, ENABLE);
	spi_rx_flag = 0;
	spi_tx_flag = 0;
#endif
}

if(SPIx==SPI2)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI2 clk enable
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   //spi2_cs  pb12
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   //spi2_sck  pb13
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   //spi2_mosi  pb15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 推免复用输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  //spi2_miso  pb14
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入   
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#ifdef SPI_INT_MODE
	NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	SPI_ITConfig(SPI2, SPI_IT_TX|SPI_IT_RX, ENABLE);
	spi_rx_flag = 0;
	spi_tx_flag = 0;
#endif
}

#ifdef SPI_DMA	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //dmaen
	
#endif
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPIx, &SPI_InitStructure);
	
	SPI_Cmd(SPIx, ENABLE); //Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率

}


void SPIM1_ReadID()
{
	
	unsigned int i;
	
	SPIM1_CSLow();																			//Spi cs assign to this pin,select
	SPIM1_TXEn(); 																			//Transmit Enable bit TXEN

	SPIM1SendByte(RDID);
	
	SPIM1_TXDisable(); 																	//disable TXEN
	
	SPIM1_Readbytes(0x01); 	
	SPIM1_Readbytes(0x01);
	SPIM1_Readbytes(0x01);

	SPIM1_CSHigh();   																 //Spi cs release
}



void SPIM1_WriteEnable()
{
	SPIM1_CSLow(); 																 //Spi cs assign to this pin,select
	SPIM1_TXEn(); 																 //Transmit Enable bit TXEN
	SPIM1SendByte(WREN); 													 //SPDAT = WREN;CheckStatus();
	SPIM1_TXDisable(); 														 //disable TXEN
	SPIM1_CSHigh(); 															 //Spi cs release
}

void SPIM1_checkStatus()
{
	unsigned char temp;
	SPIM1_CSLow(); 															 //Spi cs assign to this pin,select
	SPIM1_TXEn(); 															//Transmit Enable bit TXEN
	SPIM1SendByte(RDSR);            						//SPDAT = RDSR; CheckStatus();
	
	SPIM1_TXDisable(); 	//disable TXEN											
	
	while(1)
	{
		 temp = SPIM1_Readbytes(0x01);
	 	 if((temp&0x01)==0x0)
	 			break;
	}
	SPIM1_CSHigh();  											//Spi cs release
}

void SPIM1_WriteDisable()
{
	SPIM1_CSLow();  								//Spi cs assign to this pin,select
	SPIM1_TXEn(); 									//Transmit Enable bit TXEN
	SPIM1SendByte(WRDI); 						//SPDAT = WREN;CheckStatus();
	SPIM1_TXDisable();						 //disable TXEN
	SPIM1_CSHigh();  							//Spi cs release
}

void SPIM1_PageRead(unsigned long address,unsigned char *p,unsigned int number)  //page = 256 bytes
{
	unsigned char addr0,addr1,addr2;
	unsigned int i;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	address = address&0xffffff00; //page address
	addr0 = (unsigned char)(address>>16);
	addr1 = (unsigned char)(address>>8);
	addr2 = (unsigned char)address;
	
	SPIM1_CSLow(); 								 //Spi cs assign to this pin,select
	SPIM1_TXEn();  								//Transmit Enable bit TXEN
	SPIM1SendByte(READ); 					//SPDAT = PP;CheckStatus();
	SPIM1SendByte(addr0);
	SPIM1SendByte(addr1);
	SPIM1SendByte(addr2);
	SPIM1_TXDisable();					 //disable TXEN
#ifdef SPI_DMA
	//uart_printf("DMAR\r\n");
	DMA_DeInit(DMA1_Channel4);
	
	SPI_DMACmd(SPI1,SPI_DMAReq_Rx, ENABLE);	//dma_rxen
	
#ifdef DMA_INT_MODE
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		DMA_ITConfig(DMA1_Channel4, 0x00000001, ENABLE);				
		dma4Flag = 0x0;
#endif
	DMA_InitStructure.DMA_PeripheralBaseAddr = (SPI1_BASE + 0x04);
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20004000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = number;
  DMA_InitStructure.DMA_PeripheralInc = DMA_SrcInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_DstInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK0;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK3;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	
	for(i=0;i<256;i++)
	{
		SPIM1_RXEn();
		SPIM1_RXDisable();
	}
	
	DMAcheckStatus(DMA1_FLAG_TC4);
	
	DMA_Cmd(DMA1_Channel4, DISABLE); //disable dma_rxen and txen 
	
	SPI_DMACmd(SPI1,SPI_DMAReq_Rx, DISABLE);	//disable spi dma request
#else	
	for(i=0;i<256;i++)
	{
		rxtmpdata[i] = SPIM1_Readbytes(0x01);
		//uart_printf("rxtmpdata[%d]=0x%x\r\n",i,rxtmpdata[i]);
	}
	
#endif
	
	SPIM1_CSHigh();  					//Spi cs release
}

void SPIM1_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
{
	unsigned int j;
	unsigned char addr0,addr1,addr2;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	address = address&0xffffff00; //page address
	addr0 = (unsigned char)(address>>16);
	addr1 = (unsigned char)(address>>8);
	addr2 = (unsigned char)address;
		
	SPIM1_WriteEnable();

	SPIM1_CSLow();  				//Spi cs assign to this pin,select
	SPIM1_TXEn(); 				  //Transmit Enable bit TXEN
	SPIM1SendByte(PP); 			//SPDAT = PP;CheckStatus();
	SPIM1SendByte(addr0);
	SPIM1SendByte(addr1);
	SPIM1SendByte(addr2);
#ifdef SPI_DMA
	//uart_printf("DMAW\r\n");
	DMA_DeInit(DMA1_Channel3);
	for(j=0;j<number;j++)
	{
		//*((volatile unsigned int*)(0x20002000+j*4)) = j+4;
		*((volatile unsigned int*)(0x20003000+j*4)) = *(p++);
	}
#ifdef DMA_INT_MODE
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		DMA_ITConfig(DMA1_Channel3, 0x00000001, ENABLE);				
		dma3Flag = 0x0;
#endif
	DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_BASE;
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
  DMA_InitStructure.DMA_BufferSize = number;
  DMA_InitStructure.DMA_PeripheralInc = DMA_DstInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_SrcInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK2;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK1;
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	SPI_DMACmd(SPI1,SPI_DMAReq_Tx, ENABLE);	//dma_rxen
	DMAcheckStatus(DMA1_FLAG_TC3);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	SPI_DMACmd(SPI1,SPI_DMAReq_Tx, DISABLE);	//dma_rxen
#else
	for(j=0;j<number;j++)
	{
		SPIM1SendByte(*(p++));
	}
	
#endif
		
	SPIM1_TXDisable(); 			//disable TXEN
	SPIM1_CSHigh();  				//Spi cs release

	SPIM1_checkStatus();
}

void SPIM1_SectorErase(unsigned long address)
{
	unsigned char addr0,addr1,addr2;
	address = address & 0xffff0000;
	addr0 = ((unsigned char)(address>>16))&0xff;
	addr1 = ((unsigned char)(address>>8))&0xff;
	addr2 = ((unsigned char)address)&0xff;
	
	SPIM1_WriteEnable();
	
	SPIM1_CSLow();  			//Spi cs assign to this pin,select
	SPIM1_TXEn(); 				//Transmit Enable bit TXEN
	SPIM1SendByte(SE); 		//SPDAT = PP;CheckStatus();
	SPIM1SendByte(addr0);
	SPIM1SendByte(addr1);
	SPIM1SendByte(addr2);
	SPIM1_TXDisable(); 		//disable TXEN
	SPIM1_CSHigh();  			//Spi cs release
	
	SPIM1_checkStatus();
}

void SPIM1_BlockErase()
{
	SPIM1_WriteEnable();
	
	SPIM1_CSLow();  		//Spi cs assign to this pin,select
	SPIM1_TXEn(); 			//Transmit Enable bit TXEN
	SPIM1SendByte(BE); 	//SPDAT = PP;CheckStatus();
	SPIM1_TXDisable(); 	//disable TXEN
	SPIM1_CSHigh();  		//Spi cs release
	
	SPIM1_checkStatus();
}

void SPIM1_Close()
{
	SPIM1_CSHigh();  		//Spi cs release
}


void SPIM1_Test()
{
	unsigned int i,j;
	for(i=0;i<256;i++)
	{
		tmpdata[i] = i;
		tmpdata1[i] = 0x5a;
	}
	uart_printf("spi1 test\r\n");
	SPIM1_Init(0x3);
 	SPIM1_ReadID();	

	
 	SPIM1_BlockErase();
	//SPIM1_SectorErase(0x0000);

 	SPIM1_PageProgram(0,tmpdata,256);
	for(i=0;i<256;i++)
	{
		rxtmpdata[i] = 0x0;
	}
	SPIM1_PageRead(0,rxtmpdata,256); 			//把写进去的一页256字节读出来
	
	DataCompare(tmpdata, rxtmpdata,256);

	SPIM1_PageProgram(0x100,tmpdata1,256);
	SPIM1_PageProgram(0x200,tmpdata,256);
 	SPIM1_PageRead(0x200,rxtmpdata,256); //把写进去的一页256字节读出来

	DataCompare(tmpdata, rxtmpdata,256);
		
	SPIM1_PageProgram(0x300,tmpdata,256);
	SPIM1_PageProgram(0x400,tmpdata1,256);
	SPIM1_PageRead(0x400,rxtmpdata,256);
	
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM1_PageProgram(0x500,tmpdata1,256);
	SPIM1_PageProgram(0x600,tmpdata,256);
	SPIM1_PageRead(0x600,rxtmpdata,256);
	DataCompare(tmpdata, rxtmpdata,256);
	
	SPIM1_PageProgram(0x700,tmpdata,256);
	SPIM1_PageProgram(0x800,tmpdata1,256);
	SPIM1_PageRead(0x800,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM1_PageProgram(0x1000,tmpdata,256);
	SPIM1_PageProgram(0x1100,tmpdata1,256);
	SPIM1_PageRead(0x1100,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM1_PageProgram(0x1200,tmpdata,256);
	SPIM1_PageProgram(0x1300,tmpdata1,256);
	SPIM1_PageProgram(0x1400,tmpdata,256);
	SPIM1_PageProgram(0x1500,tmpdata1,256);
	
	SPIM1_PageRead(0x1200,rxtmpdata,256);
	DataCompare(tmpdata, rxtmpdata,256);
	
	SPIM1_PageRead(0x1300,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM1_PageRead(0x1400,rxtmpdata,256);
	DataCompare(tmpdata, rxtmpdata,256);
	
	SPIM1_PageRead(0x1500,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);

	uart_printf("spi1 rush test\r\n");
	SPIM1_BlockErase();
	j = 0;
	for(i=0;i<4096;i++)
	{
		SPIM1_PageProgram(j,tmpdata,256);
		SPIM1_PageRead(j,rxtmpdata,256);
		DataCompare(tmpdata, rxtmpdata,256);
		j = j + 0x100;
	}

	uart_printf("spi1 test over\r\n");

}


//打开spi.h头文件中的SPI_DMA 宏，就测试dma模式
void SPIM1_DMA_Test()
{
	unsigned int i,j;
	for(i=0;i<256;i++)
	{
		tmpdata[i] = i;
		tmpdata1[i] = 0x5a;
	}
	
	SPIM1_Init(0x2);
 	SPIM1_ReadID();	
	
 	SPIM1_BlockErase();
	//SPIM1_SectorErase(0x0000);
	uart_printf("SPI1 DMA test\r\n");
 	SPIM1_PageProgram(0,tmpdata,256);
	SPIM1_PageRead(0,rxtmpdata,256); 			//把写进去的一页256字节读出来

	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}

	SPIM1_PageProgram(0x100,tmpdata1,256);
	SPIM1_PageProgram(0x200,tmpdata,256);
 	SPIM1_PageRead(0x200,rxtmpdata,256); //把写进去的一页256字节读出来
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	
			
	SPIM1_PageProgram(0x300,tmpdata,256);
	SPIM1_PageProgram(0x400,tmpdata1,256);
	SPIM1_PageRead(0x400,rxtmpdata,256);
	
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("3\r\n");
	SPIM1_PageProgram(0x500,tmpdata1,256);
	SPIM1_PageProgram(0x600,tmpdata,256);
	SPIM1_PageRead(0x600,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("4\r\n");
	SPIM1_PageProgram(0x700,tmpdata,256);
	SPIM1_PageProgram(0x800,tmpdata1,256);
	SPIM1_PageRead(0x800,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("5\r\n");
	
	SPIM1_PageProgram(0x1000,tmpdata,256);
	SPIM1_PageProgram(0x1100,tmpdata1,256);
	SPIM1_PageRead(0x1100,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("6\r\n");
	
	SPIM1_PageProgram(0x1200,tmpdata,256);
	SPIM1_PageProgram(0x1300,tmpdata1,256);
	SPIM1_PageProgram(0x1400,tmpdata,256);
	SPIM1_PageProgram(0x1500,tmpdata1,256);
	
	SPIM1_PageRead(0x1200,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("7\r\n");
	
	SPIM1_PageRead(0x1300,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("8\r\n");
	
	SPIM1_PageRead(0x1400,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("9\r\n");
	
	SPIM1_PageRead(0x1500,rxtmpdata,256);
	for(i=0;i<256;i++)
	{
		j = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,j);
	}
	uart_printf("10\r\n");
	
	uart_printf("spi1 rush test\r\n");
	SPIM1_BlockErase();
	j = 0;
	for(i=0;i<4096;i++)
	{
		SPIM1_PageProgram(j,tmpdata,256);
		SPIM1_PageRead(j,rxtmpdata,256);
		//DataCompare(tmpdata, rxtmpdata,256);
		j = j + 0x100;
	}

	uart_printf("spi1 DMA test over\r\n");

}


void SPIM1_Send_Test()
{
	SPIM1_Init(0x4);
	SPIM1_CSLow();																			//Spi cs assign to this pin,select
	SPIM1_TXEn(); 																			//Transmit Enable bit TXEN
while(1)
{
	SPIM1SendByte(RDID);
}
	
	
	SPIM1_TXDisable(); 																	//disable TXEN
	
}

void SPI_print_reg()
{
	uart_printf("SPI2->TXREG=0x%x\r\n",SPI2->TXREG);      //0x0	
	uart_printf("SPI2->RXREG=0x%x\r\n",SPI2->RXREG);      //0x0	

	uart_printf("SPI2->INTSTAT=0x%x\r\n",SPI2->INTSTAT);  //0x0	
	uart_printf("SPI2->INTEN=0x%x\r\n",SPI2->INTEN);      //0x0	

	uart_printf("SPI2->GCTL=0x%x\r\n",SPI2->GCTL);        //0x04
	uart_printf("SPI2->CCTL=0x%x\r\n",SPI2->CCTL);        //0x8	
	uart_printf("SPI2->SPBRG=0x%x\r\n",SPI2->SPBRG);      //0x2	
	uart_printf("SPI2->RXDNR=0x%x\r\n",SPI2->RXDNR);      //0x1	

	
	uart_printf("SPI1->TXREG=0x%x\r\n",SPI1->TXREG);      //0x0	
	uart_printf("SPI1->RXREG=0x%x\r\n",SPI1->RXREG);      //0x0	

	uart_printf("SPI1->INTSTAT=0x%x\r\n",SPI1->INTSTAT);  //0x0	
	uart_printf("SPI1->INTEN=0x%x\r\n",SPI1->INTEN);      //0x0	
	
	uart_printf("SPI1->GCTL=0x%x\r\n",SPI1->GCTL);        //0x04
	uart_printf("SPI1->CCTL=0x%x\r\n",SPI1->CCTL);        //0x8	
	uart_printf("SPI1->SPBRG=0x%x\r\n",SPI1->SPBRG);      //0x2	
	uart_printf("SPI1->RXDNR=0x%x\r\n",SPI1->RXDNR);      //0x1	
	
	
}


void SPIM2_Test()
{
	unsigned int i,j;
	for(i=0;i<256;i++)
	{
		tmpdata[i] = i;
		tmpdata1[i] = 0x5a;
	}
	uart_printf("spi1 test\r\n");
	//SPIM2_Init(0x3);
	SPIM_Init(SPI2, 0x3);
 	SPIM_ReadID(SPI2);	

	
 	SPIM_BlockErase(SPI2);
	//SPIM1_SectorErase(0x0000);

 	SPIM_PageProgram(SPI2,0,tmpdata,256);
	for(i=0;i<256;i++)
	{
		rxtmpdata[i] = 0x0;
	}
	SPIM_PageRead(SPI2,0,rxtmpdata,256); 			//把写进去的一页256字节读出来
	
	DataCompare(tmpdata, rxtmpdata,256);

	SPIM_PageProgram(SPI2,0x100,tmpdata1,256);
	SPIM_PageProgram(SPI2,0x200,tmpdata,256);
 	SPIM_PageRead(SPI2,0x200,rxtmpdata,256); //把写进去的一页256字节读出来

	DataCompare(tmpdata, rxtmpdata,256);
		
	SPIM_PageProgram(SPI2,0x300,tmpdata,256);
	SPIM_PageProgram(SPI2,0x400,tmpdata1,256);
	SPIM_PageRead(SPI2,0x400,rxtmpdata,256);
	
	DataCompare(tmpdata1, rxtmpdata,256);
}