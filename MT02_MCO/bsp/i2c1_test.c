#include "all.h"

volatile unsigned char rx_flag = 0;
volatile unsigned char tx_flag = 0;
extern uint16_t I2C_DMA_DIR;

unsigned char tx_buffer0[16] = {0xaa,0x55,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
unsigned char tx_buffer1[16] = {0xf,0xe,0xd,0xc,0xb,0xa,9,8,7,6,5,4,3,2,1,0};
unsigned char tx_buffer2[16] = {0x55,0xaa,0x5a,0xa5,0x12,0x34,0x56,0x78,0x9a,0xbc,0xde,0xf0,0xff,0x00,0x50,0x80};
unsigned char tx_buffer3[16] = {0x55,0xaa,0x5a,0xa5,0x37,0x23,0x65,0x84,0xfe,0x79,0xb7,0xc6,0x93,0x28,0x06,0x99};
unsigned char rx_buffer0[16] = {0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa};


void I2C1_EV_IRQHandler (void)
{	
	if(I2C_GetITStatus(I2C1, I2C_IT_TX_EMPTY))
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_TX_EMPTY);
		tx_flag = 1;
	}
		
	if(I2C_GetITStatus(I2C1, I2C_IT_RX_FULL))
	{
		I2C_ClearITPendingBit(I2C1, I2C_IT_RX_FULL);
		rx_flag = 1;
	}

}



void I2CRegs_test()
{
	uart_printf("I2C1->IC_CON  = 0x%x\r\n",I2C1->IC_CON);	 //bit0-bit15  bit6543210 1 0x7f
	uart_printf("I2C1->IC_TAR	 = 0x%x\r\n",I2C1->IC_TAR);	 //bit0-bit15  bit12 1       0x0055
	uart_printf("I2C1->IC_SAR	 = 0x%x\r\n",I2C1->IC_SAR);	 //bit0-bit15   						 0x055
	uart_printf("I2C1->IC_HS_MADDR = 0x%x\r\n",I2C1->IC_HS_MADDR);	//	 0x01
	uart_printf("I2C1->IC_DATA_CMD = 0x%x\r\n",I2C1->IC_DATA_CMD);		//0x0 
	uart_printf("I2C1->IC_SS_SCL_HCNT	= 0x%x\r\n",I2C1->IC_SS_SCL_HCNT);	//0x0190
	uart_printf("I2C1->IC_SS_SCL_LCNT	= 0x%x\r\n",I2C1->IC_SS_SCL_LCNT);	//0x01d6 
	uart_printf("I2C1->IC_FS_SCL_HCNT	= 0x%x\r\n",I2C1->IC_FS_SCL_HCNT);	//0x003c 
	uart_printf("I2C1->IC_FS_SCL_LCNT	= 0x%x\r\n",I2C1->IC_FS_SCL_LCNT);	 //0x0082
	uart_printf("I2C1->IC_HS_SCL_HCNT	= 0x%x\r\n",I2C1->IC_HS_SCL_HCNT);	 //0x0006 
	uart_printf("I2C1->IC_HS_SCL_LCNT	= 0x%x\r\n",I2C1->IC_HS_SCL_LCNT);	 //0x0010 
																																									
	uart_printf("I2C1->IC_INTR_STAT	= 0x%x\r\n",I2C1->IC_INTR_STAT);	 //0x0
	uart_printf("I2C1->IC_INTR_MASK	= 0x%x\r\n",I2C1->IC_INTR_MASK);	 //0x08ff
	uart_printf("I2C1->IC_RAW_INTR_STAT	= 0x%x\r\n",I2C1->IC_RAW_INTR_STAT);	 //0x0
	uart_printf("I2C1->IC_RX_TL	= 0x%x\r\n",I2C1->IC_RX_TL);                   //	 0x0
	uart_printf("I2C1->IC_TX_TL	= 0x%x\r\n",I2C1->IC_TX_TL);	                 //0x0 
	uart_printf("I2C1->IC_CLR_INTR	= 0x%x\r\n",I2C1->IC_CLR_INTR);		 //0x0
	uart_printf("I2C1->IC_CLR_RX_UNDER  = 0x%x\r\n",I2C1->IC_CLR_RX_UNDER);		 //0x0
	uart_printf("I2C1->IC_CLR_RX_OVER		= 0x%x\r\n",I2C1->IC_CLR_RX_OVER);	   //0x0
	uart_printf("I2C1->IC_CLR_TX_OVER		= 0x%x\r\n",I2C1->IC_CLR_TX_OVER);	   //0x0
	uart_printf("I2C1->IC_CLR_RD_REQ		= 0x%x\r\n",I2C1->IC_CLR_RD_REQ);	     //0x0
	uart_printf("I2C1->IC_CLR_TX_ABRT		= 0x%x\r\n",I2C1->IC_CLR_TX_ABRT);	   //0x0
																																									
	uart_printf("I2C1->IC_CLR_RX_DONE		= 0x%x\r\n",I2C1->IC_CLR_RX_DONE);	  //0x0
	uart_printf("I2C1->IC_CLR_ACTIVITY	= 0x%x\r\n",I2C1->IC_CLR_ACTIVITY);		//0x0
	uart_printf("I2C1->IC_CLR_STOP_DET	= 0x%x\r\n",I2C1->IC_CLR_STOP_DET);		//0x0 
	uart_printf("I2C1->IC_CLR_START_DET	= 0x%x\r\n",I2C1->IC_CLR_START_DET);	//0x0 
	uart_printf("I2C1->IC_CLR_GEN_CALL	= 0x%x\r\n",I2C1->IC_CLR_GEN_CALL);		//0x0 
	uart_printf("I2C1->IC_ENABLE	= 0x%x\r\n",I2C1->IC_ENABLE);	              //0x0
	uart_printf("I2C1->IC_STATUS	= 0x%x\r\n",I2C1->IC_STATUS);	              //0x06
	uart_printf("I2C1->IC_TXFLR		= 0x%x\r\n",I2C1->IC_TXFLR);	              //0x0
	uart_printf("I2C1->IC_RXFLR		= 0x%x\r\n",I2C1->IC_RXFLR);	              //0x0
	uart_printf("I2C1->IC_SDA_HOLD	= 0x%x\r\n",I2C1->IC_SDA_HOLD);		       //0x0001
	uart_printf("I2C1->IC_TX_ABRT_SOURCE	= 0x%x\r\n",I2C1->IC_TX_ABRT_SOURCE);//0x0
	uart_printf("I2C1->IC_SLV_DATA_NACK_ONLY	= 0x%x\r\n",I2C1->IC_SLV_DATA_NACK_ONLY	);//0x0
	uart_printf("I2C1->IC_DMA_CR	= 0x%x\r\n",I2C1->IC_DMA_CR);	 //0x0
	uart_printf("I2C1->IC_DMA_TDLR	= 0x%x\r\n",I2C1->IC_DMA_TDLR); //0x0		 
	uart_printf("I2C1->IC_DMA_RDLR	= 0x%x\r\n",I2C1->IC_DMA_RDLR);	//0x0	 
	uart_printf("I2C1->IC_SDA_SETUP	= 0x%x\r\n",I2C1->IC_SDA_SETUP);	//0x64
	uart_printf("I2C1->IC_ACK_GENERAL_CALL	= 0x%x\r\n",I2C1->IC_ACK_GENERAL_CALL);	//0x1
	uart_printf("I2C1->IC_ENABLE_STATUS = 0x%x\r\n",I2C1->IC_ENABLE_STATUS);	//0x0
	uart_printf("I2C1->IC_FS_SPKLEN  	= 0x%x\r\n",I2C1->IC_FS_SPKLEN);	//0x5
	uart_printf("I2C1->IC_HS_SPKLEN		= 0x%x\r\n",I2C1->IC_HS_SPKLEN); //0x1
	uart_printf("I2C1->IC_CLR_RESTART_DET	= 0x%x\r\n",I2C1->IC_CLR_RESTART_DET);	//0x0
	uart_printf("I2C1->IC_COMP_PARAM_1	= 0x%x\r\n",I2C1->IC_COMP_PARAM_1);	 //0x101EE
	uart_printf("I2C1->IC_COMP_VERSION	= 0x%x\r\n",I2C1->IC_COMP_VERSION);	 //0x3132312A
	uart_printf("I2C1->IC_COMP_TYPE	= 0x%x\r\n",I2C1->IC_COMP_TYPE);   //0x44570140
	
}


void I2CInitMasterMode(I2C_TypeDef *I2Cx,unsigned long apb_mhz,unsigned long i2c_baud_rate) //unit is Khz
{
	I2C_InitTypeDef I2C_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	if(I2Cx==I2C1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //i2c1 clk enable
			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //dmaen
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;   //i2c1_scl  pb6  i2c1_sda  pb7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#ifdef I2C_INT_MODE
	NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	rx_flag = 0x0;
	tx_flag = 0x0;
#endif
	}
	if(I2Cx==I2C2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  //i2c1 clk enable
			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //dmaen
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;   //i2c2_scl  pb10  i2c2_sda  pb11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
#ifdef I2C_INT_MODE
	NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	rx_flag = 0x0;
	tx_flag = 0x0;
#endif
	}
	
  I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
  I2C_InitStructure.I2C_OwnAddress =  0xA8; //0xA8; 0xA0 for FPGA
  I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
	I2C_Init(I2Cx, &I2C_InitStructure);
	
}

void I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
{
	while(1)
	{
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TX_EMPTY))
		{
			break;
		}
	}
}

void I2CRXFullCheck(I2C_TypeDef *I2Cx)
{
	
	while(1)
	{
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL))
		{
			break;
		}	
	}
}

void I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)
{
	I2C_SendData(I2Cx, temp);
#ifdef I2C_INT_MODE
	I2C_ITConfig(I2Cx, I2C_IT_TX_EMPTY, ENABLE);
	while(1)
	{
		if(tx_flag)
		{
			tx_flag = 0;
			break;
		}
	}
#else
	I2CTXEmptyCheck(I2Cx);
#endif
}

unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
	unsigned short temp;

#ifdef I2C_INT_MODE
	
	I2C_ITConfig(I2Cx, I2C_IT_RX_FULL, ENABLE);
	
	while(1)
	{
		if(rx_flag)
		{
			rx_flag = 0;
			break;
		}
	}
	
#else
	I2CRXFullCheck(I2Cx);
#endif
	
	temp = I2C_ReceiveData(I2Cx);
	return (unsigned char)temp;
}



void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
	unsigned short temp;
	unsigned short i;
	unsigned char *p;
	p = tx_data;
	//I2C_Cmd(I2C1, DISABLE);
	I2C_Send7bitAddress(I2Cx, 0xA8, I2C_Direction_Transmitter);
	//I2C_Send7bitAddress(I2Cx, 0xA0, I2C_Direction_Transmitter);
	I2C_Cmd(I2Cx, ENABLE);
	temp = ((mem_byte_addr) & 0xff);
	I2CTXByte(I2Cx,CMD_WRITE,temp);  //tx memory addr

	for(i=0;i<tx_count;i++)
	{
		temp = *p;
		p++;
		I2CTXByte(I2Cx,CMD_WRITE,temp); //tx data

	}	
	
	while(1)
	{
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOP_DET))
		{
			temp = I2Cx->IC_CLR_STOP_DET;
			break;
		}	
	}
	Delay(0x000ffff); 
  
}


void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
	unsigned char temp;
	unsigned short i;
	unsigned char *p;
	p = rx_data;

	
	I2C_Send7bitAddress(I2Cx, 0xA8, I2C_Direction_Transmitter);
	//I2C_Send7bitAddress(I2Cx, 0xA0, I2C_Direction_Transmitter);
	I2C_Cmd(I2Cx, ENABLE);
	temp = ((mem_byte_addr) & 0xff);
	I2CTXByte(I2Cx,CMD_WRITE,temp);  //tx memory addr
	while(1)
	{
		if(I2C_GetFlagStatus(I2Cx,  I2C_FLAG_STOP_DET))
		{
			temp = I2Cx->IC_CLR_STOP_DET;	  
			break;
		}
	}
  
	I2C_Send7bitAddress(I2Cx, 0xA8, I2C_Direction_Transmitter);
	//I2C_Send7bitAddress(I2Cx, 0xA0, I2C_Direction_Transmitter);
	I2C_Cmd(I2Cx, ENABLE);
	
	for(i=0;i<rx_count;i++)
	{
	
		temp = I2CRXByte(I2Cx);

		*p = temp;
		p++;
	
	}

	
//I2C_Cmd(I2C1, DISABLE);
}


void I2CMasterTest(I2C_TypeDef *I2Cx)
{
	unsigned int i,j;
	
	
		I2CInitMasterMode(I2Cx,8000000L,100000L);
		//I2C1InitMasterMode(30000000L,100000L);
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*0, 16, tx_buffer0 );
		
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, 16*0, 16, rx_buffer0 );

		DataCompare(tx_buffer0, rx_buffer0,16);
		
/*		
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*1, 16, tx_buffer1 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*2, 16, tx_buffer2 );
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, 16*2, 16, rx_buffer0 );
		DataCompare(tx_buffer2, rx_buffer0,16);
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*3, 16, tx_buffer3 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*4, 16, tx_buffer0 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*5, 16, tx_buffer1 );
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, 16*3, 16, rx_buffer0 );
		DataCompare(tx_buffer3, rx_buffer0,16);
		
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*6, 16, tx_buffer2 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*7, 16, tx_buffer3 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*8, 16, tx_buffer0 );
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, 16*6, 16, rx_buffer0 );
	  DataCompare(tx_buffer2, rx_buffer0,16);
		
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*9, 16, tx_buffer1 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*10, 16, tx_buffer2 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*11, 16, tx_buffer3 );
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, 16*9, 16, rx_buffer0 );
		DataCompare(tx_buffer1, rx_buffer0,16);
		
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*12, 16, tx_buffer0 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*13, 16, tx_buffer1 );
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, 16*14, 16, tx_buffer2 );
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, 16*13, 16, rx_buffer0 );
		DataCompare(tx_buffer1, rx_buffer0,16);
		

		uart_printf("I2Cx rush test\r\n");
		j = 0;
	for(i=0;i<64;i++)
	{
		I2CMasterWrite(I2Cx,FLASH_DEVICE_ADDR, j, 16, tx_buffer2 );
		I2CMasterRead(I2Cx,FLASH_DEVICE_ADDR, j, 16, rx_buffer0 );
		DataCompare(tx_buffer2, rx_buffer0,16);
		j = j + 16;
		
	}
*/
	//I2C1Close();
	uart_printf("I2Cx test over\r\n");

}


/*-------------------DMA----------------------------------------------
----------------------------------------------------------------------*/

void DMA_I2C1MasterWrite(unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
	unsigned short temp;
	unsigned short i;
	unsigned char *p;
	DMA_InitTypeDef DMA_InitStructure;
	p = tx_data;
	
	for(i=0;i<tx_count;i++)
	{
		*((volatile unsigned int*)(0x20003000+i*4)) = *(p++);
	}
	
	//I2C_Cmd(I2C1, DISABLE);
	I2C_Send7bitAddress(I2C1, 0xA8, I2C_Direction_Transmitter);
	
	I2C_Cmd(I2C1, ENABLE);
	//send byte mem addr
	temp = ((mem_byte_addr) & 0xff);
	I2CTXByte(I2C1,CMD_WRITE,temp);  //tx memory addr
	
	
	I2C_DMA_DIR = TDMAE_SET;
	I2C_DMACmd(I2C1, ENABLE);
	
	DMA_DeInit(DMA1_Channel7);
	
	/*
	DMA_InitStructure.DMA_PeripheralBaseAddr = (I2C1_BASE + 0x10);
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20003000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
  DMA_InitStructure.DMA_BufferSize = tx_count;
  DMA_InitStructure.DMA_PeripheralInc = DMA_DstInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_SrcInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK6;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK1;
	*/
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (I2C1_BASE + 0x10);
  DMA_InitStructure.DMA_MemoryBaseAddr =0x20003000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST ;
  DMA_InitStructure.DMA_BufferSize = tx_count;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel7, ENABLE);

	DMAcheckStatus(DMA1_FLAG_TC7);

	I2CTXEmptyCheck(I2C1);

	//dec stop
	while(1)
	{
		if(I2C_GetITStatus(I2C1, I2C_FLAG_STOP_DET))
		{
				temp = I2C1->IC_CLR_STOP_DET;  
			break;
		}
	}
	DMA_Cmd(DMA1_Channel7, DISABLE);

	I2C_DMACmd(I2C1, DISABLE);
	Delay(0x000ffff); 
	
  //I2C_Cmd(I2C1, DISABLE);
}

void DMA_I2C1MasterRead(unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
	unsigned char temp;
	unsigned short i;
	unsigned char *p;
	DMA_InitTypeDef DMA_InitStructure;
	p = rx_data;

	//I2C_Cmd(I2C1, DISABLE);
	I2C_Send7bitAddress(I2C1, 0xA8, I2C_Direction_Transmitter);
	I2C_Cmd(I2C1, ENABLE);
	
	//send byte mem addr
	temp = ((mem_byte_addr) & 0xff);
	I2CTXByte(I2C1,CMD_WRITE,temp);  //tx memory addr

	while(1)
	{
	
		if(I2C_GetITStatus(I2C1, I2C_FLAG_STOP_DET))
		{
				  
			break;
		}
	}
  
	//I2C_Cmd(I2C1, DISABLE);
	I2C_Send7bitAddress(I2C1, 0xA8, I2C_Direction_Receiver);
	I2C_Cmd(I2C1, ENABLE);
	
	DMA_DeInit(DMA1_Channel2);
	/*
	DMA_InitStructure.DMA_PeripheralBaseAddr = (I2C1_BASE + 0x10);
  DMA_InitStructure.DMA_MemoryBaseAddr = 0x20004000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = rx_count;
  DMA_InitStructure.DMA_PeripheralInc = DMA_SrcInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_DstInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Hardware;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_DST_PER = DST_PER_ACK0;
  DMA_InitStructure.DMA_SRC_PER = SRC_PER_ACK7;
	*/
	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (I2C1_BASE + 0x10);
  DMA_InitStructure.DMA_MemoryBaseAddr =0x20004000;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = rx_count;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);	
	
	
	DMA_Cmd(DMA1_Channel2, ENABLE);
	
	I2C_DMA_DIR = RDMAE_SET;
	I2C_DMACmd(I2C1, ENABLE);
	
	for(i=0;i<rx_count;i++)
	{
		I2C1->IC_DATA_CMD = 0x0100;
		
		while(1)
		{
			if(I2C_GetITStatus(I2C1, I2C_IT_TX_EMPTY))
			{
				break;
			}
		}
	}
	
	DMAcheckStatus(DMA1_FLAG_TC2);
	DMA_Cmd(DMA1_Channel2, DISABLE);	
	
	I2C_DMACmd(I2C1, DISABLE);
    //I2C_Cmd(I2C1, DISABLE);
}


void I2C1MasterDMATest()
{
	unsigned int i,j,temp;
	I2C_DeInit(I2C1);
	uart_printf("i2c1 dma test start\r\n");
	I2CInitMasterMode(I2C1,30000000L,40000L);
	
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*0, 16, tx_buffer1 );

	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*0, 16, rx_buffer0 );
	
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}

	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*1, 16, tx_buffer1 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*2, 16, tx_buffer2 );
	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*2, 16, rx_buffer0 );
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}
		
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*3, 16, tx_buffer3 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*4, 16, tx_buffer0 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*5, 16, tx_buffer1 );
	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*5, 16, rx_buffer0 );
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}
    
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*6, 16, tx_buffer2 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*7, 16, tx_buffer3 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*8, 16, tx_buffer0 );
	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*7, 16, rx_buffer0 );
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}
	
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*9, 16, tx_buffer1 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*10, 16, tx_buffer2 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*11, 16, tx_buffer3 );
	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*2, 16, rx_buffer0 );
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*12, 16, tx_buffer0 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*13, 16, tx_buffer1 );
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*14, 16, tx_buffer2 );
	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*2, 16, rx_buffer0 );
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}
	DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16*15, 16, tx_buffer3 );
	
	DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16*0, 16, rx_buffer0 );
	for(i=0;i<16;i++)
	{
		temp = *((volatile unsigned int *)(0x20004000+i*4));
		uart_printf("temp%d=0x%x\r\n",i,temp);
	}
		
		
	uart_printf("I2C1 dma rush test\r\n");
	j = 0;
	for(i=0;i<64;i++)
	{
		DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, j, 16, tx_buffer2 );
		DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, j, 16, rx_buffer0 );
		
		j = j + 16;
		
	}

	uart_printf("i2c1 dma test over\r\n");

}

void I2C1Close(I2C_TypeDef *I2Cx) 
{
	 I2C_Cmd(I2Cx, DISABLE);
}



/*-------------------I2C master  slave  test----------------------------
------------------------------------------------------------------------
------------------------------------------------------------------------*/
void I2C_Master_Init(I2C_TypeDef *I2Cx) //unit is Khz
{
	unsigned char tmpdata = 0x55;
	unsigned int tempClear;
	I2C_InitTypeDef I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	if(I2Cx==I2C1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //i2c1 clk enable	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;   //i2c1_scl  pb6  i2c1_sda  pb7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	}
	if(I2Cx==I2C2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  //i2c2 clk enable	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;   //i2c2_scl  pb10  i2c2_sda  pb11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	
	
	I2Cx->IC_ENABLE = 0x0;

	I2Cx->IC_CON = 0x163;
	I2Cx->IC_TAR = 0x12>>1;
	I2Cx->IC_SS_SCL_HCNT = 0x77;
	I2Cx->IC_SS_SCL_LCNT = 0x55;
	I2Cx->IC_RX_TL=0x0;//set Rx FIFO threshold level 
  I2Cx->IC_TX_TL=0x0;//set Tx FIFO threschold level 
  I2Cx->IC_INTR_MASK=0;//disable all inttrupt
  I2Cx->IC_ENABLE=0x1;
	

}


void I2CSlave_init(I2C_TypeDef *I2Cx)
{
	I2C_InitTypeDef I2C_InitStructure;

	GPIO_InitTypeDef  GPIO_InitStructure;
	
	if(I2Cx==I2C1)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //i2c1 clk enable	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;   //i2c1_scl  pb6  i2c1_sda  pb7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);

	}
	if(I2Cx==I2C2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);  //i2c2 clk enable	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;   //i2c2_scl  pb10  i2c2_sda  pb11
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; // 复用开漏输出
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	
	I2Cx->IC_ENABLE = 0x0;
	I2Cx->IC_CON = 0x122;
	I2Cx->IC_SAR = 0x12>>1;
	I2Cx->IC_SS_SCL_HCNT = 0x77;
	I2Cx->IC_SS_SCL_LCNT = 0x55;
	I2Cx->IC_RX_TL=0x0;//set Rx FIFO threshold level 
  I2Cx->IC_TX_TL=0x0;//set Tx FIFO threschold level 
  I2Cx->IC_INTR_MASK=0;//disable all inttrupt
  I2Cx->IC_ENABLE=0x1;
}



void I2CMaster_TX(I2C_TypeDef *I2Cx)
{
	unsigned int tempClear = 0x0;
	unsigned int i;
	I2C_Master_Init(I2Cx);
	for(i=0;i<10;i++)
	{
		I2Cx->IC_DATA_CMD = 0x00aa + i;
	//I2C->IC_DATA_CMD = 0xaa;
		while(1)
		{
					if((I2Cx->IC_RAW_INTR_STAT & 0x0010)==0x0010)//tx_empty=1
						break;
		}
		//uart_printf("tx 0k\r\n");
   }	
	
	while(1)
		{
			if((I2Cx->IC_STATUS & 0x20)==0x20)//MST-ACTIVITY=0 check idle 
            break;
		}
	  while(1)
    {
        if((I2Cx->IC_RAW_INTR_STAT & 0x0200)==0x0200)//stop flag detect
				tempClear = I2Cx->IC_CLR_STOP_DET;  //read this reg clear stop flag
        break;
    }	
}

void I2CMaster_RX(I2C_TypeDef *I2Cx)
{
	unsigned int tempClear = 0x0;
	unsigned int i;
	unsigned int rxdata[10] = {0x0};
	I2C_Master_Init(I2Cx);
	
	for(i=0;i<10;i++)
	{
		I2Cx->IC_DATA_CMD = 0x100;
		while(1)
			{
				 if((I2Cx->IC_RAW_INTR_STAT & 0x0004)==0x0004)
						 break;
			}
			rxdata[i] = I2Cx->IC_DATA_CMD; 
			uart_printf("rxdata%d = 0x%x\r\n",i,rxdata[i]);
	}	
}

void I2CSlave_RX_test(I2C_TypeDef *I2Cx)
{
	unsigned int rxdata[10] = {0x0};
	unsigned int tempClear = 0x0;
  //unsigned int rxdata = 0x0;
	unsigned int i;
	
	I2CSlave_init(I2Cx);
	for(i=0;i<10;i++)
	{
		I2Cx->IC_DATA_CMD |= 0x100;
	
			while(1)
				{
					 if((I2Cx->IC_RAW_INTR_STAT & 0x0004)==0x0004)
							 break;
				}
				rxdata[i] = I2Cx->IC_DATA_CMD; 
				

	}
	for(i=0;i<10;i++)
	{
		uart_printf("rxdata[%d] = 0x%x\r\n",i,rxdata[i]);
	}

}

void I2CSlave_Tx_test(I2C_TypeDef *I2Cx)
{
	unsigned int tempClear = 0x0;
  unsigned int rxdata = 0x0;
	unsigned int i;
	
	I2CSlave_init(I2Cx);
	
	for(i=0;i<10;i++)
	{
		while(1)
		{
			if((I2Cx->IC_RAW_INTR_STAT & 0x0020)==0x0020)//slave RD_REQ
			break;
		}
		
		I2Cx->IC_DATA_CMD = 0x78+i;
		tempClear =  I2Cx->IC_CLR_RD_REQ ;
		
		while(1)
		{
				if((I2Cx->IC_RAW_INTR_STAT & 0x0010)==0x0010)//tx_empty=1
				break;
		}
			
	}
	
	while(1)
		{
			if((I2Cx->IC_STATUS & 0x20)==0x20)//MST-ACTIVITY=0 check idle 
            break;
		}
	  while(1)
    {
        if((I2Cx->IC_RAW_INTR_STAT & 0x0200)==0x0200)//stop flag detect
				tempClear = I2Cx->IC_CLR_STOP_DET;  //read this reg clear stop flag
        break;
    }	

	
}

