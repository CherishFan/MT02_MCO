#include "all.h"
#include "spi.h"
unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;

//#define   SPI_INT_MODE

void SPI1_IRQHandler (void)
{
	//uart_printf("spi1 iqr\r\n");
	//if(SPI_GetITStatus(SPI0, SPI_IT_TXEPT))
	if(SPI_GetITStatus(SPI1, SPI_IT_TX))
	{
		SPI_ClearITPendingBit(SPI1, SPI_IT_TX);
		spi0_tx_flag = 1;
	}
	
	if(SPI_GetITStatus(SPI1, SPI_IT_RX))
	{
		SPI_ClearITPendingBit(SPI1, SPI_IT_RX);		 //clear rx interrupt
		spi0_rx_flag = 1; 
	}		
}
/*
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
*/
void SPIM_CSLow()
{
	//Spi cs assign to this pin,select
	//GPIOA->DATA = 0x00000000;//Spi cs assign to this pin,select PA0 pin
	
	SPI_CSInternalSelected(SPI1, SPI_CS_BIT0,ENABLE);
}

void SPIM_CSHigh()
{
	//Spi cs release
	//GPIOA->DATA = 0x00000001; //Spi cs release PA0 pin
	SPI_CSInternalSelected(SPI1, SPI_CS_BIT0,DISABLE);
}

void SPIM_TXEn()
{
	//Transmit Enable bit TXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);
}

void SPIM_TXDisable()
{
	//disable TXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Disable_Tx);
}

void SPIM_RXEn()
{
	//enable RXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
}

void SPIM_RXDisable()
{
	//disable RXEN
	SPI_BiDirectionalLineConfig(SPI1, SPI_Disable_Rx);
}


void SPIMSendByte(unsigned char tx_data)
{
	//SPI mode0 and clk is fclk/32 and enable spi
	SPI_SendData(SPI1, tx_data);
	
#ifdef SPI_INT_MODE
	while (1)
	{
		if(spi0_tx_flag) 			//tx fifo and tx shift reg are all empty	 	
		{
			spi0_tx_flag = 0;	 	//clear flag 
			break;
		}
	}
#else	
	while (1)
	{
		
		if(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXEPT))	
		{
			break;
		}
	}

#endif	

}

unsigned char SPIM_Readbyte()
{
	unsigned int count = 0;
	unsigned char temp;
	//SPIM_RXEn(); 
#ifdef SPI_INT_MODE	  //中断接受函数
	while(1)
	{
		if(spi0_rx_flag)
	 	{
			spi0_rx_flag = 0;	 //clear interrupt
			break;
			//if(SPI_GetFlagStatus(SPI0, SPI_FLAG_RXAVL)) 				
		//	{
		//		temp = (unsigned char)SPI_ReceiveData(SPI0);
		//	break;
		//	}	
		}
	}	
#else
	while(1)
	{
		if(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXAVL)) 
		{
		
			break;
		}
		
	}
#endif
	temp = (unsigned char)SPI_ReceiveData(SPI1);
	//SPIM_RXDisable(); 
	return temp;
}	



void SPIM_Init(unsigned short spi_baud_div)
{
	unsigned long temp;
  SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //SPI1 clk enable
	SPIM_CSHigh();
	
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
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    // mode0 SPI_CPOL_Low, SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // mode3 SPI_CPOL_High,SPI_CPHA_1Edge
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
	spi0_rx_flag = 0;
	spi0_tx_flag = 0;
#endif

}

void SPIM_ReadID()
{
	unsigned char temp;
	unsigned int i;
	
	SPIM_CSLow();																			//Spi cs assign to this pin,select
	SPIM_TXEn(); 																			//Transmit Enable bit TXEN
	SPIMSendByte(RDID);
	SPIM_TXDisable(); 	//disable TXEN
	
	SPI_RxBytes(SPI1, 0x01);
	for(i=0;i<3;i++)
	{
		SPIM_RXEn();      //enable RXEN
		temp = SPIM_Readbyte();
		SPIM_RXDisable();     //disable RXEN	
    uart_printf("temp=0x%x\r\n",temp);		
	}

	SPIM_CSHigh();   																 //Spi cs release
}

void SPIM_WriteEnable()
{
	SPIM_CSLow(); 																 //Spi cs assign to this pin,select
	SPIM_TXEn(); 																 //Transmit Enable bit TXEN
	SPIMSendByte(WREN); 													 //SPDAT = WREN;CheckStatus();
	SPIM_TXDisable(); 														 //disable TXEN
	SPIM_CSHigh(); 															 //Spi cs release
}

void SPIM_checkStatus()
{
	unsigned char temp;
	SPIM_CSLow(); 															 //Spi cs assign to this pin,select
	SPIM_TXEn(); 															//Transmit Enable bit TXEN
	SPIMSendByte(RDSR);            						//SPDAT = RDSR; CheckStatus();
	
	SPIM_TXDisable(); 	//disable TXEN											
	
	SPI_RxBytes(SPI1, 0x01);
	while(1)
	{
		 SPIM_RXEn(); 												//enable RXEN
		 temp = SPIM_Readbyte();
		 SPIM_RXDisable();										//disable RXEN
	 	 if((temp&0x01)==0x0)
	 			break;
	}
	SPIM_CSHigh();  											//Spi cs release
}

void SPIM_WriteDisable()
{
	SPIM_CSLow();  								//Spi cs assign to this pin,select
	SPIM_TXEn(); 									//Transmit Enable bit TXEN
	SPIMSendByte(WRDI); 						//SPDAT = WREN;CheckStatus();
	SPIM_TXDisable();						 //disable TXEN
	SPIM_CSHigh();  							//Spi cs release
}

void SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)  //page = 256 bytes
{
	unsigned char addr0,addr1,addr2;
	unsigned int i;
	//DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	address = address&0xffffff00; //page address
	addr0 = (unsigned char)(address>>16);
	addr1 = (unsigned char)(address>>8);
	addr2 = (unsigned char)address;
	
	SPIM_CSLow(); 								 //Spi cs assign to this pin,select
	SPIM_TXEn();  								//Transmit Enable bit TXEN
	SPIMSendByte(READ); 					//SPDAT = PP;CheckStatus();
	SPIMSendByte(addr0);
	SPIMSendByte(addr1);
	SPIMSendByte(addr2);
	SPIM_TXDisable();					 //disable TXEN

SPI_RxBytes(SPI1, 0x01);
	for(i=0;i<256;i++)
	{
		SPIM_RXEn(); 							//enable RXEN
		rxtmpdata[i] = SPIM_Readbyte();
		SPIM_RXDisable();				 //disable RXEN
		//uart_printf("rxtmpdata[%d]=0x%x\r\n",i,rxtmpdata[i]);
	}

	
	SPIM_CSHigh();  					//Spi cs release
}

void SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
{
	unsigned int j;
	unsigned char addr0,addr1,addr2;
	//DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	address = address&0xffffff00; //page address
	addr0 = (unsigned char)(address>>16);
	addr1 = (unsigned char)(address>>8);
	addr2 = (unsigned char)address;
		
	SPIM_WriteEnable();

	SPIM_CSLow();  				//Spi cs assign to this pin,select
	SPIM_TXEn(); 				  //Transmit Enable bit TXEN
	SPIMSendByte(PP); 			//SPDAT = PP;CheckStatus();
	SPIMSendByte(addr0);
	SPIMSendByte(addr1);
	SPIMSendByte(addr2);
	
	for(j=0;j<number;j++)
	{
		SPIMSendByte(*(p++));
	}

		
	SPIM_TXDisable(); 			//disable TXEN
	SPIM_CSHigh();  				//Spi cs release

	SPIM_checkStatus();
}

void SPIM_SectorErase(unsigned long address)
{
	unsigned char addr0,addr1,addr2;
	address = address & 0xffff0000;
	addr0 = ((unsigned char)(address>>16))&0xff;
	addr1 = ((unsigned char)(address>>8))&0xff;
	addr2 = ((unsigned char)address)&0xff;
	
	SPIM_WriteEnable();
	
	SPIM_CSLow();  			//Spi cs assign to this pin,select
	SPIM_TXEn(); 				//Transmit Enable bit TXEN
	SPIMSendByte(SE); 		//SPDAT = PP;CheckStatus();
	SPIMSendByte(addr0);
	SPIMSendByte(addr1);
	SPIMSendByte(addr2);
	SPIM_TXDisable(); 		//disable TXEN
	SPIM_CSHigh();  			//Spi cs release
	
	SPIM_checkStatus();
}

void SPIM_BlockErase()
{
	SPIM_WriteEnable();
	
	SPIM_CSLow();  		//Spi cs assign to this pin,select
	SPIM_TXEn(); 			//Transmit Enable bit TXEN
	SPIMSendByte(BE); 	//SPDAT = PP;CheckStatus();
	SPIM_TXDisable(); 	//disable TXEN
	SPIM_CSHigh();  		//Spi cs release
	
	SPIM_checkStatus();
}

void SPIM_Close()
{
	SPIM_CSHigh();  		//Spi cs release
	SPI_RxBytes(SPI1, 0x01);
}

void SPI_print_reg()
{
	uart_printf("SPI0->TXREG=0x%x\r\n",SPI1->TXREG);      //0x0	
	uart_printf("SPI0->RXREG=0x%x\r\n",SPI1->RXREG);      //0x0	
	uart_printf("SPI0->CSTAT=0x%x\r\n",SPI1->CSTAT);      //0x1	
	uart_printf("SPI0->INTSTAT=0x%x\r\n",SPI1->INTSTAT);  //0x0	
	uart_printf("SPI0->INTEN=0x%x\r\n",SPI1->INTEN);      //0x0	
	uart_printf("SPI0->INTCLR=0x%x\r\n",SPI1->INTCLR);    //0x0	
	uart_printf("SPI0->GCTL=0x%x\r\n",SPI1->GCTL);        //0x04
	uart_printf("SPI0->CCTL=0x%x\r\n",SPI1->CCTL);        //0x8	
	uart_printf("SPI0->SPBRG=0x%x\r\n",SPI1->SPBRG);      //0x2	
	uart_printf("SPI0->RXDNR=0x%x\r\n",SPI1->RXDNR);      //0x1	
	uart_printf("SPI0->SCSR=0x%x\r\n",SPI1->SCSR);        //0xff	
	
	
	
}

void SPIM_Test()
{
	unsigned int i,j;
	for(i=0;i<256;i++)
	{
		tmpdata[i] = i;
		tmpdata1[i] = 0x5a;
	}
	uart_printf("spi0 test\r\n");
	SPIM_Init(0x3);
 	SPIM_ReadID();	

 	//SPIM_BlockErase();
	SPIM_SectorErase(0x0000);

 	SPIM_PageProgram(0,tmpdata,256);
	for(i=0;i<256;i++)
	{
		rxtmpdata[i] = 0x0;
	}
	SPIM_PageRead(0,rxtmpdata,256); 			//把写进去的一页256字节读出来
	
	DataCompare(tmpdata, rxtmpdata,256);

	SPIM_PageProgram(0x100,tmpdata1,256);
	SPIM_PageProgram(0x200,tmpdata,256);
 	SPIM_PageRead(0x100,rxtmpdata,256); //把写进去的一页256字节读出来

	DataCompare(tmpdata1, rxtmpdata,256);
/*	
	SPIM_PageProgram(0x300,tmpdata,256);
	SPIM_PageProgram(0x400,tmpdata1,256);
	SPIM_PageRead(0x400,rxtmpdata,256);
	
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM_PageProgram(0x500,tmpdata1,256);
	SPIM_PageProgram(0x600,tmpdata,256);
	SPIM_PageRead(0x600,rxtmpdata,256);
	DataCompare(tmpdata, rxtmpdata,256);
	
	SPIM_PageProgram(0x700,tmpdata,256);
	SPIM_PageProgram(0x800,tmpdata1,256);
	SPIM_PageRead(0x800,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM_PageProgram(0x1000,tmpdata,256);
	SPIM_PageProgram(0x1100,tmpdata1,256);
	SPIM_PageRead(0x1100,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM_PageProgram(0x1200,tmpdata,256);
	SPIM_PageProgram(0x1300,tmpdata1,256);
	SPIM_PageProgram(0x1400,tmpdata,256);
	SPIM_PageProgram(0x1500,tmpdata1,256);
	
	SPIM_PageRead(0x1200,rxtmpdata,256);
	DataCompare(tmpdata, rxtmpdata,256);
	
	SPIM_PageRead(0x1300,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);
	
	SPIM_PageRead(0x1400,rxtmpdata,256);
	DataCompare(tmpdata, rxtmpdata,256);
	
	SPIM_PageRead(0x1500,rxtmpdata,256);
	DataCompare(tmpdata1, rxtmpdata,256);

	uart_printf("spi rush test\r\n");
	SPIM_BlockErase();
	j = 0;
	for(i=0;i<4096;i++)
	{
		SPIM_PageProgram(j,tmpdata,256);
		SPIM_PageRead(j,rxtmpdata,256);
		DataCompare(tmpdata, rxtmpdata,256);
		j = j + 0x100;
	}
*/
	uart_printf("spi test over\r\n");

}



/*------------------------------------SPI loop------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/


void SPI_master_loop()
{
	unsigned int i;
	unsigned char temp[20] = {0x55,0xaa,0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x56,0x23,0xff,0xaa,0x87,0x89,0x88,0x20,0x34,0x38,0xcd};
	unsigned char rxdata[20];
	for(i=0;i<20;i++)
	{
		rxdata[i] = 0x0;
	}
	
	SPIM_Init(0x3);
	SPIM_TXDisable();       ////disable TXEN	clear fifo
	SPIM_RXDisable(); 			 //disable RXEN	clear fifo
	SPI_RxBytes(SPI1, 0x01); //SPI1_RXDNR = 0x01;
	SPIM_CSLow();  				//Spi cs assign to this pin,select
	SPIM_TXEn(); 				  //Transmit Enable bit TXEN
	SPIM_RXEn();
	
	for(i=0;i<20;i++)
	{
		SPIMSendByte(temp[i]); 			//SPDAT = PP;CheckStatus();
		rxdata[i] = SPIM_Readbyte();
		uart_printf("rxdata[%d]=0x%x\r\n",i,rxdata[i]);
	}
	for(i=0;i<20;i++)
	{
		if(rxdata[i]!=temp[i])	
		uart_printf("err\r\n");
		else
		uart_printf("OK\r\n");	
	}
	
	SPIM_TXDisable();
	SPIM_RXDisable(); 			 //disable RXEN	
	
}


/*------------------------------------SPI Slave------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/

void SSI_print_reg()
{
	uart_printf("SPI2->TXREG   =0x%x\r\n",SPI2->TXREG);      //0x0	         
	uart_printf("SPI2->RXREG	 =0x%x\r\n",SPI2->RXREG);      //0x0	     
	uart_printf("SPI2->CSTAT	 =0x%x\r\n",SPI2->CSTAT);      //0x1	     
	uart_printf("SPI2->INTSTAT =0x%x\r\n",SPI2->INTSTAT);  //0x0	     
	uart_printf("SPI2->INTEN	 =0x%x\r\n",SPI2->INTEN);      //0x0	     
	uart_printf("SPI2->INTCLR	 =0x%x\r\n",SPI2->INTCLR);    //0x0	     
	uart_printf("SPI2->GCTL	 =0x%x\r\n",SPI2->GCTL);        //0x0	 0x04
	uart_printf("SPI2->CCTL	 =0x%x\r\n",SPI2->CCTL);        //0x8	     
	uart_printf("SPI2->SPBRG	 =0x%x\r\n",SPI2->SPBRG);      //0x2	     
	uart_printf("SPI2->RXDNR	 =0x%x\r\n",SPI2->RXDNR);      //0x1	     
	uart_printf("SPI2->SCSR	 =0x%x\r\n",SPI2->SCSR);        //0xff
	
}

void SSI_CSLow()
{
	//SSI->SCSR = 0x00000000;//Spi cs low

  SPI_CSInternalSelected(SPI2,SPI_CS_BIT0,ENABLE);

}

void SSI_CSHigh()
{
	//SSI->SCSR = 0x00000001; //Spi cs release high
	SPI_CSInternalSelected(SPI2,SPI_CS_BIT0,DISABLE);
}

void SSI_TXEn()
{
	//SSI->GCTL = SSI->GCTL | 0x0008; //Transmit Enable bit TXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
}

void SSI_TXDisable()
{
	//SSI->GCTL = SSI->GCTL & 0xfff7; //disable TXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Tx);
}

void SSI_RXEn()
{
	//SSI->GCTL = SSI->GCTL | 0x10; //enable RXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
}

void SSI_RXDisable()
{
	//SSI->GCTL = SSI->GCTL & 0xffef; //disable RXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Rx);
}

void SSI_Init()
{
	unsigned long temp;
	 SPI_InitTypeDef SPI_InitStructure;
	SPI2->GCTL = 0x0;
	
	//SPI0 clk enable
	//SYSTEM_PeriphClockCmd(SYSTEM_Periph_SSI, ENABLE);	
	
	//GPIO_PinRemapConfig(GPIOB,GPIO_Remap_Pin_15, GPIO_Remap_Function2); //SSI_CLK
//	GPIO_PinRemapConfig(GPIOB,GPIO_Remap_Pin_16, GPIO_Remap_Function2); //SSI_MISO
	//GPIO_PinRemapConfig(GPIOB,GPIO_Remap_Pin_17, GPIO_Remap_Function2); //SSI_MOSI
	
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    // mode0 SPI_CPOL_Low, SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // mode3 SPI_CPOL_High,SPI_CPHA_1Edge
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_BaudRatePrescaler = 0x1;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE); //Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率

	SPI_SlaveAdjust(SPI2, SPI_SlaveAdjust_FAST);//SSI_CCTL = SSI_CCTL | 0x0020;	   //bit 5 txedge=1 
  SPI_RxBytes(SPI2, 0x01); //SSI_RXDNR = 0x01;
	
	SSI_CSHigh();
	
}


void SSI_SendByte(unsigned char tx_data)
{
	//SSI_TXREG = tx_data;    //SPI mode0 and clk is fclk/32 and enable spi
	SPI_SendData(SPI2, tx_data);
}

unsigned char SSI_Readbyte()
{
	unsigned char temp = 0;

	while(1)
	{
		//if((SSI_INTSTAT&0x0002)) 
		if(SPI_GetITStatus(SPI2, SPI_IT_RX))
		{
		   	//SSI_INTSTAT = SSI_INTSTAT | 0x0002;	 //clear interrupt
			SPI_ClearITPendingBit(SPI2, SPI_IT_RX);
				break;
		}
		
	}
	//temp = SSI_RXREG;
	temp = SPI_ReceiveData(SPI2);
	return temp;
}


void SPIM2_Init(unsigned short spi_baud_div)
{
	unsigned long temp;
  SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//初始化GPIOB P1口为输出，作为cs来片选外面的flash
	//GPIOA->CTRL = 0x0; //software
	//GPIOA->DIR  = 0xffffffff; //output
	
 //SPI0 clk enable
	//SYSTEM_PeriphClockCmd(SYSTEM_Periph_SPI1, ENABLE);	
	
	//GPIO_PinRemapConfig(GPIOB,GPIO_Remap_Pin_0, GPIO_Remap_Function2); //SPI1_CLK
	//GPIO_PinRemapConfig(GPIOB,GPIO_Remap_Pin_1, GPIO_Remap_Function2); //SPI1_MISO
	//GPIO_PinRemapConfig(GPIOB,GPIO_Remap_Pin_2, GPIO_Remap_Function2); //SPI1_MOSI
	
	
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    // mode0 SPI_CPOL_Low, SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  // mode3 SPI_CPOL_High,SPI_CPHA_1Edge
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI2, &SPI_InitStructure);
	
	SPI_Cmd(SPI2, ENABLE); //Enables the specified SPI peripheral SPI使能、主机模式 8位数据模式 SPI 的波特率

}

void SPIM2_TXDisable()
{
	//disable TXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Tx);
}

void SPIM2_TXEn()
{
	//Transmit Enable bit TXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
}

void SPIM2_RXEn()
{
	//enable RXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
}

void SPIM2_RXDisable()
{
	//disable RXEN
	SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Rx);
}

void SPIM2SendByte(unsigned char tx_data)
{
	//SPI mode0 and clk is fclk/32 and enable spi
	SPI_SendData(SPI2, tx_data);
	
	while (1)
	{
		if(SPI_GetFlagStatus(SPI2, SPI_FLAG_TXEPT))	
		{
			break;
		}
	}
}

unsigned char SPIM2_Readbyte()
{
	unsigned int count = 0;
	unsigned char temp;
	
	while(1)
	{
		if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL)) 
		{
		
			break;
		}
		
	}
	temp = (unsigned char)SPI_ReceiveData(SPI2);
	return temp;
}	


void SPI2Master_TX()
{
	unsigned char txdata[10] = {0x9f,0x55,0xaa,0xff,0x23,0x45,0x78,0x67,0x39,0xbb};
	SPIM2_Init(0x08);
	SPIM2_TXDisable();       ////disable TXEN	clear fifo
	//判断接收字节是否为读ID，并发送id 给主
	SPIM2_TXEn();
	SPIM2SendByte(txdata[0]);
	//SPIM2SendByte(txdata[1]);
	//SPIM2SendByte(txdata[2]);
	//SPIM2SendByte(txdata[3]);
	//SPIM2SendByte(txdata[4]);
	//SPIM2SendByte(txdata[5]);
	//SPIM2SendByte(txdata[6]);

	//SPIM2_TXDisable();       ////disable TXEN	clear fifo
}

void SSISlave_RX_test()
{
  unsigned char rxdata[10];
	unsigned char i;
	for(i=0;i<10;i++)
	{
		rxdata[i] = 0;
	}
	
	//SPIM2_Init(0x08);
	SSI_Init();
	//SPIM2_TXDisable();       ////disable TXEN	clear fifo
	SSI_RXDisable(); 			 //disable RXEN	clear fifo
	SSI_RXEn();
	
	rxdata[0] = SSI_Readbyte();		
	//rxdata[1] = SSI_Readbyte();		
	//rxdata[2] = SSI_Readbyte();		
	//rxdata[3] = SSI_Readbyte();		
	//rxdata[4] = SSI_Readbyte();		
	//rxdata[5] = SSI_Readbyte();		
	//rxdata[6] = SSI_Readbyte();		

	//SSI_RXDisable(); 			 //disable RXEN	clear fifo

	for(i=0;i<7;i++)
	{
		
		uart_printf("rxdata[%d]=0x%x\r\n",i,rxdata[i]);
		
	}
}
	

void SPI2Master_RX()
{
 unsigned char rxdata[10];
	unsigned char i;
	for(i=0;i<10;i++)
	{
		rxdata[i] = 0;
	}
	SPIM2_Init(0x08);
	SPI_RxBytes(SPI2,0x08); //SPI1_RXDNR = 0x08;
	SPIM2_RXEn();
	
	rxdata[0] = SPIM2_Readbyte();
	rxdata[1] = SPIM2_Readbyte();		
	rxdata[2] = SPIM2_Readbyte();		
	rxdata[3] = SPIM2_Readbyte();		
	rxdata[4] = SPIM2_Readbyte();		
	rxdata[5] = SPIM2_Readbyte();		
	rxdata[6] = SPIM2_Readbyte();
	rxdata[7] = SPIM2_Readbyte();
	
	SPIM2_RXDisable(); 			 //disable RXEN	clear fifo

	for(i=0;i<8;i++)
	{
		uart_printf("rxdata[%d]=0x%x\r\n",i,rxdata[i]);
		
	}
	
	SPI_Cmd(SPI2, DISABLE); //SPI1_GCTL = SPI1_GCTL & 0xfffe;	
}

void SSISlave_TX_test() //ssi tx  SPI1 RX
{
	unsigned char txdata[10] = {0x9f,0x55,0xaa,0xff,0x23,0x45,0x78,0x67,0x39,0xbb};
  unsigned char rxdata[10];
	unsigned char i;
	for(i=0;i<10;i++)
	{
		rxdata[i] = 0;
	}
	
	SSI_Init();
	//SPIM1_Init(8000000,1000000);
	//SPIM2_Init(0x08);
	SSI_TXDisable();       //disable TXEN	clear fifo
	
	SSI_TXEn();
	SSI_SendByte(txdata[0]);
	SSI_SendByte(txdata[1]);
	SSI_SendByte(txdata[2]);
	SSI_SendByte(txdata[3]);
	SSI_SendByte(txdata[4]);
	SSI_SendByte(txdata[5]);
	SSI_SendByte(txdata[6]);
	SSI_SendByte(txdata[7]);
	
  while (1)
	{
		//if((SSI_CSTAT&0x0001)==0x0001) 				//if((SPI1_INTSTAT&0x01)){SPI1_INTCLR = SPI1_INTCLR | 0x01;break; //clear tx int flag}	 	
		if(SPI_GetFlagStatus(SPI2,SPI_FLAG_TXEPT))
		{
			break;
		}
	}	

	SSI_TXDisable();       ////disable TXEN	clear fifo

  SPI_Cmd(SPI2, DISABLE); //SSI_GCTL = SSI_GCTL & 0xfffe;
}



