


/* Includes ------------------------------------------------------------------*/
#include "all.h"

void PVU_CheckStatus()
{
	while(1)
	{
		if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)  //PVU为0，可以写IWDG_PR寄存器值
		{
			break;
		}
	}
}

void RVU_CheckStatus()
{
	while(1)
	{
		if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  //RVU为0，可以写IWDG_RLR寄存器值
		{
			break;
		}
	}
}

void Write_Iwdg_PR()
{
	PVU_CheckStatus();
	IWDG_SetPrescaler(IWDG_Prescaler_4);
}

void Write_Iwdg_RLR(uint16_t Reload)
{
	if(Reload>0xfff)
		uart_printf("reload value is wrong,too large\r\n");
	RVU_CheckStatus();
	IWDG_SetReload(Reload);
	
}

void Write_protectRegs()
{
	IWDG_WriteAccessCmd(0x5555); //允许访问IWDG_PR  IWDG_RLR 寄存器
	Write_Iwdg_PR();
	Write_Iwdg_RLR(0x125);
}

void Read_protectRegs()
{
	PVU_CheckStatus();
	uart_printf("IWDG_PR=0x%x\r\n",IWDG->PR);
	
	RVU_CheckStatus();
	uart_printf("IWDG->RLR=0x%x\r\n",IWDG->RLR); 
}

void Iwdg_test(void)
{
	//Lsi_clk();
	uart_printf("RCC->CSR=0x%x\r\n",RCC->CSR);
	uart_printf("IWDG->SR=0x%x\r\n",IWDG->SR); 
	//Read_protectRegs();
	
	//IWDG->KR = 0xcccc; //IWDG_Enable();  //使能IWDG计数，计数到0产生复位
	uart_printf("RCC->CSR=0x%x\r\n",RCC->CSR); //查看IWDG是否复位
	Delay(6000);
	uart_printf("RCC1->CSR=0x%x\r\n",RCC->CSR); //查看IWDG是否复位
	uart_printf("RCC2->CSR=0x%x\r\n",RCC->CSR); //查看IWDG是否复位
	//IWDG_ReloadCounter();
}

void IWDG_register_test()
{
	uart_printf("IWDG->KR=0x%x\r\n",IWDG->KR);  //0x0
	uart_printf("IWDG->PR=0x%x\r\n",IWDG->PR);  //0x0
	uart_printf("IWDG->RLR=0x%x\r\n",IWDG->RLR); //0xfff
	uart_printf("IWDG->SR=0x%x\r\n",IWDG->SR); //0x0
}
void Iwdg_reset(void)
{
	LSI_clk();
	IWDG->KR = 0xcccc; 
}