//********************************************************************
//(Copyright 2012) Verisilicon Inc. All Rights Reserved
//Company Confidential and Proprietary information
//This information may not be disclosed to unauthorized individual
//********************************************************************
//
//Verisilicon Microelectronics Co.,Ltd
//
//File name 	: dmausb.c
//Author		: Houyue.Shi 
//Verision		: V0.1
//Created		: 2012.02.24
//Modified		: 
//Description	:
//				  
//********************************************************************
#include "usb.h"
//#include "dma.h"
/*
DMA_REQUST usb_dma_rx_cmd,usb_dma_tx_cmd;
//because RTL restriction
//EP1---->USB_CH0_REQ--->DMA_REQ(2)
//EP2---->USB_CH1_REQ--->DMA_REQ(3)
//for DMA configuration
//src_width*src_burst == dst_width*dst_burst
//

void usb_dma_init()
{
	uint32 req =0;
	DMAInit();	
	//
	dma_finished_flag[0] = 0;
	dma_finished_flag[1] = 0;
	//rx for out packet	
	req = (EPOUT== USB_EPn(2))?(3):(2);
	usb_dma_rx_cmd.status  	= 0;
	usb_dma_rx_cmd.enable	= 0;	
	usb_dma_rx_cmd.channel 	= DMA_RX_CH;
	usb_dma_rx_cmd.srcAddr 	= USB_EPn_FIFO(EPOUT);
	usb_dma_rx_cmd.dstAddr 	= NULL; 
	usb_dma_rx_cmd.transByteSize = EPn_MAX_PACKAGE_SIZE;
#ifdef USB_DMA_BURST1_8BIT	
	usb_dma_rx_cmd.chCtrl 	=
					(SRC_ADDR_FIX | DES_ADDR_INC
					| SRC_WIDTH_8BIT| DES_WIDTH_8BIT
					| SRC_BURST_1 	 | DES_BURST_1);	 //|DMA_TRANS_SIZE(usb_dma_rx_cmd.transByteSize>>0)
#endif

#ifdef USB_DMA_BURST1_16BIT
	usb_dma_rx_cmd.chCtrl 	=
					(SRC_ADDR_FIX | DES_ADDR_INC
					| SRC_WIDTH_16BIT| DES_WIDTH_16BIT
					| SRC_BURST_1 	 | DES_BURST_1);    //|DMA_TRANS_SIZE(usb_dma_rx_cmd.transByteSize>>1)
#endif

#ifdef USB_DMA_BURST1_32BIT
	usb_dma_rx_cmd.chCtrl 	=
					(SRC_ADDR_FIX | DES_ADDR_INC
					| SRC_WIDTH_32BIT| DES_WIDTH_32BIT
					| SRC_BURST_1 	 | DES_BURST_1);	//|DMA_TRANS_SIZE(usb_dma_rx_cmd.transByteSize>>2)
#endif

#ifdef DMA_FLOW_CTRL_IS_PER
	usb_dma_rx_cmd.chCfg   	= DMA_DIR_P2M_SP | DMA_SRC_REQ(req)|DMA_CH_EN;
#else
	usb_dma_rx_cmd.chCfg   	= DMA_DIR_P2M_DMA| DMA_SRC_REQ(req)|DMA_CH_EN;
#endif
	//tx for in packet
	req = (EPIN== USB_EPn(1))?(2):(3);
	usb_dma_tx_cmd.status  	= 0;
	usb_dma_tx_cmd.enable	= 0;	
	usb_dma_tx_cmd.channel 	= DMA_TX_CH;
	usb_dma_tx_cmd.srcAddr 	= NULL;
	usb_dma_tx_cmd.dstAddr 	= USB_EPn_FIFO(EPIN);
	usb_dma_tx_cmd.transByteSize = EPn_MAX_PACKAGE_SIZE;
	
#ifdef USB_DMA_BURST1_8BIT		
	usb_dma_tx_cmd.chCtrl 	=
					(SRC_ADDR_INC| DES_ADDR_FIX
					| SRC_WIDTH_8BIT| DES_WIDTH_8BIT
					| SRC_BURST_1 	 | DES_BURST_1
					|DMA_TRANS_SIZE(usb_dma_rx_cmd.transByteSize>>0));
#endif

#ifdef USB_DMA_BURST1_16BIT
	usb_dma_tx_cmd.chCtrl 	=
					(SRC_ADDR_INC| DES_ADDR_FIX
					| SRC_WIDTH_16BIT| DES_WIDTH_16BIT
					| SRC_BURST_1 	 | DES_BURST_1
					|DMA_TRANS_SIZE(usb_dma_rx_cmd.transByteSize>>0));
#endif

#ifdef USB_DMA_BURST1_32BIT
	usb_dma_tx_cmd.chCtrl 	=
					(SRC_ADDR_INC| DES_ADDR_FIX
					| SRC_WIDTH_32BIT| DES_WIDTH_32BIT
					| SRC_BURST_1 	 | DES_BURST_1
					|DMA_TRANS_SIZE(usb_dma_rx_cmd.transByteSize>>0));
#endif

#ifdef DMA_FLOW_CTRL_IS_PER
	usb_dma_tx_cmd.chCfg   	= DMA_DIR_M2P_DP| DMA_DST_REQ(req)|DMA_CH_EN;
#else
	usb_dma_tx_cmd.chCfg   	= DMA_DIR_M2P_DMA| DMA_DST_REQ(req)|DMA_CH_EN;
#endif
	//for usb register
	write_mreg32(USB_EP_DMA_DIR,EPn_DIR_IN(EPIN)|EPn_DIR_OUT(EPOUT));//ep1 is D2H(IN), ep2 is H2D(out)
	
#ifdef USB_DMA_BURST1_8BIT		
	write_mreg32(USB_AHB_DMA,USB_DMA_BURST_1BYTE(0)|USB_DMA_BURST_1BYTE(1)|USB_DMA_TIMEOUT);
#endif

#ifdef USB_DMA_BURST1_16BIT
	write_mreg32(USB_AHB_DMA,USB_DMA_BURST_2BYTE(0)|USB_DMA_BURST_2BYTE(1)|USB_DMA_TIMEOUT);
#endif

#ifdef USB_DMA_BURST1_32BIT
	write_mreg32(USB_AHB_DMA,USB_DMA_BURST_4BYTE(0)|USB_DMA_BURST_4BYTE(1)|USB_DMA_TIMEOUT);
#endif
}


void usb_dma_rx_start(uint32 srcAddr,uint32 dstAddr, uint32 byteCnt)
{
	uint32 set=0,clr=0 ;
	usb_dma_rx_cmd.srcAddr = srcAddr;
	usb_dma_rx_cmd.dstAddr = dstAddr;
	usb_dma_rx_cmd.transByteSize = byteCnt;

	clr = (EPOUT==USB_EPn(2))?(1):(2);
	set = (EPOUT==USB_EPn(2))?(2):(1); 
	write_mreg32(USB_DMA_BYTE_NUML(clr),0x00);
	write_mreg32(USB_DMA_BYTE_NUMH(clr),0x00);
	write_mreg32(USB_DMA_BYTE_NUML(set),byteCnt&0xFF);
	write_mreg32(USB_DMA_BYTE_NUMH(set),(byteCnt>>8)&0xFF);

	write_mreg32(USB_EP_DMA,(1<<(EPOUT)));//eanble ep_out dma mode
	write_mreg32(USB_EPn_CTRL(EPOUT),0x80);//EP will start transfer data when dma mode
	
#ifdef USB_DMA_BURST1_8BIT
	usb_dma_rx_cmd.chCtrl 	= (usb_dma_rx_cmd.chCtrl & 0xFFFFF000) | DMA_TRANS_SIZE(byteCnt>>0);
#endif	
#ifdef USB_DMA_BURST1_16BIT
	usb_dma_rx_cmd.chCtrl 	= (usb_dma_rx_cmd.chCtrl & 0xFFFFF000) | DMA_TRANS_SIZE(byteCnt>>1);
#endif	

#ifdef USB_DMA_BURST1_32BIT
	usb_dma_rx_cmd.chCtrl 	= (usb_dma_rx_cmd.chCtrl & 0xFFFFF000) | DMA_TRANS_SIZE(byteCnt>>2);
#endif	
	//uart_printf("chCtrl=0x%08lx\r\n",usb_dma_rx_cmd.chCtrl);
	//uart_printf("chCfg=0x%08lx\r\n",usb_dma_rx_cmd.chCfg);
	DMAOpen(&usb_dma_rx_cmd);
}

void usb_dma_tx_start(uint32 srcAddr,uint32 dstAddr, uint32 byteCnt)
{
	uint32 set=0,clr=0 ;
	usb_dma_tx_cmd.srcAddr = srcAddr;
	usb_dma_tx_cmd.dstAddr = dstAddr;
	usb_dma_tx_cmd.transByteSize = byteCnt;
	clr = (EPIN==USB_EPn(1))?(2):(1);
	set = (EPIN==USB_EPn(1))?(1):(2); 
	write_mreg32(USB_DMA_BYTE_NUML(clr),0x00);
	write_mreg32(USB_DMA_BYTE_NUMH(clr),0x00);
	write_mreg32(USB_DMA_BYTE_NUML(set),byteCnt&0xFF);
	write_mreg32(USB_DMA_BYTE_NUMH(set),(byteCnt>>8)&0xFF);
	write_mreg32(USB_EP_DMA,(1<<(EPIN)));//eanble ep1&ep2 dma mode
	write_mreg32(USB_EPn_CTRL(EPIN),0x80);//EP will start transfer data when dma mode
//
#ifdef USB_DMA_BURST1_8BIT
	usb_dma_tx_cmd.chCtrl 	= (usb_dma_tx_cmd.chCtrl & 0xFFFFF000) | DMA_TRANS_SIZE(byteCnt>>0);
#endif	
#ifdef USB_DMA_BURST1_16BIT
	usb_dma_tx_cmd.chCtrl 	= (usb_dma_tx_cmd.chCtrl & 0xFFFFF000) | DMA_TRANS_SIZE(byteCnt>>1);
#endif	

#ifdef USB_DMA_BURST1_32BIT
	usb_dma_tx_cmd.chCtrl 	= (usb_dma_tx_cmd.chCtrl & 0xFFFFF000) | DMA_TRANS_SIZE(byteCnt>>2);
#endif	
//	uart_printf("chCtrl=0x%08lx\r\n",usb_dma_tx_cmd.chCtrl);
//	uart_printf("chCfg=0x%08lx\r\n",usb_dma_tx_cmd.chCfg);
	DMAOpen(&usb_dma_tx_cmd);
}


void usb_dma_process(DMA_REQUST * dma_req)
{
	DMAProcess(dma_req);
}

void usb_dma_close(DMA_REQUST * dma_req)
{
	DMAClose(dma_req);
	write_mreg32(USB_EP_DMA,0x00);//disable dma mode
}

*/



