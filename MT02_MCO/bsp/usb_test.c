//********************************************************************
//(Copyright 2015) Mindmotion All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//Mindmotion Microelectronics Co.,Ltd
//
//File name 	: usb_main.c
//Author		  : 
//Verision		: V0.1
//Created		  : 
//Modified		: sfw 2015.01.30
//Description	:
//				  
//********************************************************************
#include "all.h"

volatile uint8 usb_ep_stall[5];
volatile uint8 usb_ep_in_stall[5];
volatile uint8 usb_ep_out_stall[5];
volatile uint8 usb_setup_flag;
volatile uint8 usb_out_flag;
volatile uint8 usb_in_flag;
volatile uint8 usb_finish_flag[5];
volatile uint8 usb_reset_flag;
volatile uint8 usb_suspend_flag;
volatile uint8 usb_resume_flag;
volatile uint8 usb_state_suspend = 0;
volatile uint8 usb_state_resume = 0;
volatile USB_STATE_t usb_state;	
//volatile uint8 usb_ep1_finish;
volatile uint8 usb_ep_flag;
//
volatile uint8 usb_send_epn_stall[5];
extern uint32 debug_flag;

void usb_reset()
{
/*
	uint32 rstval;
	rstval = read_mreg32(SYS_SOFT_RST);
	rstval &= 0xFDF;   //bit5 1111 1101 1111
	write_mreg32(SYS_SOFT_RST,rstval);
	DelayMS(200);
	rstval = read_mreg32(SYS_SOFT_RST);
	rstval |= 0x020;   //bit5
	write_mreg32(SYS_SOFT_RST,rstval);
	DelayMS(200);
	write_mreg32(0xFFFF4014,0x01); //usb mode
*/
}

 
uint32 temp;
uint32 temp_ep;
uint32 temp_epn;


void USB_HP_CAN1_TX_IRQHandler(void) 
#ifdef USB_INTR_MODE
__irq
#endif 
{
	//uart_printf("1 \r\n");
	temp = read_mreg32(USB_INT_STATE);
	
	write_mreg32(USB_INT_STATE, temp);	//clear interrupt
	//uart_printf("USB_INT_STATE=0x%lx\r\n",temp);
	if(temp&USB_INT_DMA)
	{
		
		if(temp&USB_INT_DMA_CH0) 
		{
			dma_finished_flag[0]=1; //dma transfer done
			//uart_printf("d0\r\n");
		}
		if(temp&USB_INT_DMA_CH1) 
		{
			dma_finished_flag[1]=1; //dma transfer done
			//uart_printf("d1\r\n");
		}
		
	}
	else if(temp&USB_INTEP)
	{//got ep interrupt
		//uart_printf("21\r\n");
		temp_ep=read_mreg32(EP_INT_STATE);
		write_mreg32(EP_INT_STATE, temp_ep);
		//uart_printf("%1x\r\n",temp_ep&0xF);
		if(temp_ep&EP0_INT)
		{//got ep0 interrupt, ep0 has dual-direction
			//write_mreg32(EP_INT_STATE, EP0_INT);
			usb_ep_flag=USB_EP0;
			temp_epn=read_mreg32(EP0_INT_STATE);
			write_mreg32(EP0_INT_STATE,temp_epn);//clear interrupt
			if(temp_epn&EP_SETUP)
			{
				usb_setup_flag = TRUE;
				if(usb_send_epn_stall[0]==TRUE)
				{
					//uart_printf("%lx\r\n",read_mreg32(USB_EP_HALT));
				}
			}
			if(temp_epn&EP_IN_NACK)
			{
				usb_in_flag = TRUE;
				//disable in_nack interrupt
			    //write_mreg32(EP0_INT_EN,EP_SETUP|EP_FINISH|EP_OUT_ACK|EP_STALL);
			}
			if(temp_epn&EP_OUT_ACK)
				usb_out_flag = TRUE;			
			if(temp_epn&EP_STALL)
			{			
				usb_ep_stall[0] = TRUE;	
				//uart_printf("%lx\r\n",read_mreg32(USB_EP_HALT));
			}
			if(temp_epn&EP_IN_STALL)
			{			
				usb_ep_in_stall[0] = TRUE;
				//uart_printf("I0\r\n");				
			}
			if(temp_epn&EP_OUT_STALL)
			{			
				usb_ep_out_stall[0] = TRUE;	
				//uart_printf("O0\r\n");
			}
		}		
#if (!(EPOUT&EPIN))	 //if epin != epout,
		if(temp_ep&EPn_INT(EPOUT))
		{//got ep2 interrupt
			usb_ep_flag=USB_EPn(EPOUT);
			//EP2 only use for bulk OUT
			temp_epn=read_mreg32(EPn_INT_STATE(EPOUT));
			write_mreg32(EPn_INT_STATE(EPOUT), temp_epn);//clear interrupt
			if(temp_epn&EP_OUT_ACK)
			{
				usb_out_flag = TRUE;			
				//uart_printf("O\r\n");
			}
			if(temp_epn&EP_FINISH)
				usb_finish_flag[EPOUT] = TRUE;		
			if(temp_epn&EP_IN_NACK)
				usb_in_flag = TRUE;
			if(temp_epn&EP_STALL)
			{			
				usb_ep_stall[EPOUT] = TRUE;	
				//uart_printf("OUT%lx\r\n",read_mreg32(USB_EP_HALT));
			}
			if(temp_epn&EP_IN_STALL)
			{
				usb_ep_in_stall[EPOUT] = TRUE;
				//uart_printf("I2\r\n");
			}
			if(temp_epn&EP_OUT_STALL)
			{
				usb_ep_out_stall[EPOUT] = TRUE;
				//uart_printf("O2\r\n");
			}
		}
#endif
		if(temp_ep&EPn_INT(EPIN))
		{//got ep1 interrupt, ep1 is input
			usb_ep_flag=USB_EPn(EPIN);
			temp_epn=read_mreg32(EPn_INT_STATE(EPIN));
			write_mreg32(EPn_INT_STATE(EPIN),temp_epn);//clear interrupt
			if(temp_epn&EP_IN_NACK)
			{
				usb_in_flag = TRUE;
				write_mreg32(EPn_INT_EN(EPIN),(EP_FINISH|EP_OUT_ACK|EP_STALL));
				//uart_printf("I\r\n");
			}
			if(temp_epn&EP_OUT_ACK)
				usb_out_flag = TRUE;			
			if(temp_epn&EP_FINISH)
				usb_finish_flag[EPIN] = TRUE;
			if(temp_epn&EP_STALL)
			{			
				usb_ep_stall[EPIN] = TRUE;	
				uart_printf("IN%lx\r\n",read_mreg32(USB_EP_HALT));
			}
			if(temp_epn&EP_IN_STALL)
			{
				usb_ep_in_stall[EPIN] = TRUE;
				//uart_printf("I1\r\n");
			}
			if(temp_epn&EP_OUT_STALL)
			{
				usb_ep_out_stall[EPIN] = TRUE;
				//uart_printf("O1\r\n");
			}

		}
	}
	else if(temp & USB_INTHOSTRESET)
	{//got host reset interrupt
		//uart_printf("2\r\n");
		usb_reset_flag = TRUE;
	}else if(temp & USB_INTSUSPEND)
	{
		//uart_printf("3\r\n");
		usb_suspend_flag = TRUE;
	}else if(temp & USB_INTRESUME)
	{
		//uart_printf("4\r\n");
		usb_resume_flag = TRUE;
	}
}

void usb_gpio()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12;   //spi1_cs  pa4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //ƒ£ƒ‚ ‰»Î
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void usb_test(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,ENABLE); //USB CLK EN
	
	uart_printf("This is a USB Demo \r\n");
	
	usb_gpio();
#ifdef USB_INTR_MODE
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

	usb_init();

  	while(1)
    {
		#ifndef USB_INTR_MODE
			USB_HP_CAN1_TX_IRQHandler();
		#endif
			if(usb_setup_flag==TRUE)       // Setup packet received    0x0002
			{
				//uart_printf("s\r\n");
				usb_setup_flag = FALSE;
				usb_setup_handle();
			}
			else if(usb_out_flag==TRUE)         // OUT packet received      0x0008
			{	
				//uart_printf("o\r\n");
				usb_out_flag = FALSE;
				usb_out_handle();
			}
			else if(usb_in_flag==TRUE)          // IN request received      0x0004
			{
				//uart_printf("i\r\n");
				usb_in_handle();
				write_mreg32(EPn_INT_STATE(EPIN),EP_IN_NACK);
				usb_in_flag = FALSE;
				write_mreg32(EPn_INT_EN(EPIN),(EP_FINISH|EP_IN_NACK|EP_OUT_ACK|EP_STALL));
			}
			else if(usb_reset_flag==TRUE)       // Usb host reset
			{
				//uart_printf("reset\r\n");
				usb_reset_flag = FALSE;
				usb_reset_handle();
			}
			else if(usb_suspend_flag==TRUE)       // Usb suspend
			{
				//uart_printf("suspend\r\n");
				usb_suspend_flag = FALSE;
				//usb_state_suspend++;
				usb_suspend_handle();
			}
			else if(usb_resume_flag==TRUE)       // Usb resume
			{
				//uart_printf("resume\r\n");
				usb_resume_flag = FALSE;
				//usb_state_suspend++;
				usb_resume_handle();
			}
    }
}

uint32 read_mreg32( uint32 mreg)
{
	//uart_printf("2\r\n");
	return(*(volatile uint32 *)mreg);		
}

void write_mreg32( uint32 mreg, uint32 val)
{
	*(volatile uint32 *)mreg = (uint32)val;	
}


uint8 read_mreg8( uint32 mreg)
{
	return(*(volatile uint8 *)mreg);		
}

void write_mreg8( uint32 mreg, uint8 val)
{
	*(volatile uint8 *)mreg = (uint8)val;
}

void delay1ms(uint32 dly)
{
	uint32 cnt,i,j,k;

	for(cnt=0;cnt<dly;cnt++)
	{
		for(i=0;i<24;i++)//wait bus wakeup
		{
			for(j=0;j<2;j++)
			{
				for(k=0;k<100;k++);
			}
		}
	}

}

