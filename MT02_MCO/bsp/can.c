
//-----------------------函数声明，变量定义--------------------------------------------------------
#include "all.h"

//-----------------------定义SJA1000读写缓冲区的数据结构--------------------------------------------------------
//                                IDH IDL RTR  length     DATA0-8 
unsigned char  TX_Frame_Data[12]= {0x13,0x0,0x0,0x08,0x0,0x55,0x2,0x43,0x4,0x5,0x6,0x7};
unsigned char  RX1_Frame_Data[12];


void CAN0_lib_test()
{
	unsigned int i;
	unsigned char temp;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_Basic_InitTypeDef CAN_Basic_InitStructure;
	CAN_Basic_FilterInitTypeDef CAN_Basic_FilterInitStructure;
	CanBasicRxMsg CanBasicRxMsgStructure;
	CanBasicTxMsg CanBasicTxMsgStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;   //can rx  pb8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;   //can tx  pb9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	

	/*
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	*/
	CAN_Mode_Cmd(CAN1, CAN_BASICMode);
	CAN_ResetMode_Cmd(CAN1,ENABLE); //CAN0_enter_resetmode();// 进入复位模式
	
	CAN_Basic_InitStructure.BRP = 0xf; //calcuate tx and rx rate CAN0_set_rate(0x0f,0x07);
	                                   //设置波特率通信  25KHz 8MHz system clk
	CAN_Basic_InitStructure.SJW = 0x0;
	CAN_Basic_InitStructure.TESG1 = 0x07; //calcuate tx and rx rate CAN0_set_rate(0x0f,0x07);
	                                   //设置波特率通信  25KHz 8MHz system clk
  CAN_Basic_InitStructure.TESG2 = 0x0;
  CAN_Basic_InitStructure.SAM = RESET;
  CAN_Basic_InitStructure.GTS = DISABLE;
  CAN_Basic_InitStructure.CDCLK = 0x05; //CAN0_set_CLK(0x05);//CDR 配置时钟分频寄存器
  CAN_Basic_InitStructure.CLOSE_OPEN_CLK = 0x0;
  CAN_Basic_InitStructure.RXINTEN = 0x0;
  CAN_Basic_InitStructure.CBP = 0x0;
	CAN_Init(CAN1, &CAN_Basic_InitStructure);
	
	CAN_Basic_FilterInitStructure.CAN_FilterId = 0x13;
	CAN_Basic_FilterInitStructure.CAN_FilterMaskId = 0xff;
	CAN_FilterInit(&CAN_Basic_FilterInitStructure);  	//CAN0_set_dataselect(IDH,0xff); //ACR,AMR 配置验收代码/屏蔽寄存器
	
	CAN_ResetMode_Cmd(CAN1,DISABLE);	//CAN0_quit_resetmode();   //退出复位模式,进入工作模式
	

	CanBasicTxMsgStructure.IDL = 0x0;
	CanBasicTxMsgStructure.IDH = 0x13;
	CanBasicTxMsgStructure.DLC = 0x08;
	CanBasicTxMsgStructure.RTR = 0x0;
	CanBasicTxMsgStructure.Data[0] = 0x1;
	CanBasicTxMsgStructure.Data[1] = 0x2;
	CanBasicTxMsgStructure.Data[2] = 0x3;
	CanBasicTxMsgStructure.Data[3] = 0x4;
	CanBasicTxMsgStructure.Data[4] = 0x5;
	CanBasicTxMsgStructure.Data[5] = 0x6;
	CanBasicTxMsgStructure.Data[6] = 0x7;
	CanBasicTxMsgStructure.Data[7] = 0x8;
	CAN_Transmit(CAN1,&CanBasicTxMsgStructure);
	
	while(1)
	{
		while(1)
		{
			if(CAN_GetFlagStatus(CAN1,CAN_STATUS_RBS))//接收状态查询
			{
				break;
			}
		
		}
		//CAN0_read_SJAreceiveBUF();
		CAN_Receive(CAN1,&CanBasicRxMsgStructure);
		uart_printf("CanBasicRxMsgStructure.id=0x%x\r\n",CanBasicRxMsgStructure.ID);
		for(i=0;i<8;i++)
		{
			uart_printf("RX Data[%d]=0x%x\r\n",i,CanBasicRxMsgStructure.Data[i]);
		 	
		} 
		break;
	}
	

}
 
