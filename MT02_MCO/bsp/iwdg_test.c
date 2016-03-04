


/* Includes ------------------------------------------------------------------*/
#include "all.h"

void PVU_CheckStatus()
{
	while(1)
	{
		if(IWDG_GetFlagStatus(IWDG_FLAG_PVU)==RESET)  //PVUΪ0������дIWDG_PR�Ĵ���ֵ
		{
			break;
		}
	}
}

void RVU_CheckStatus()
{
	while(1)
	{
		if(IWDG_GetFlagStatus(IWDG_FLAG_RVU)==RESET)  //RVUΪ0������дIWDG_RLR�Ĵ���ֵ
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
	IWDG_WriteAccessCmd(0x5555); //�������IWDG_PR  IWDG_RLR �Ĵ���
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
	
	//IWDG->KR = 0xcccc; //IWDG_Enable();  //ʹ��IWDG������������0������λ
	uart_printf("RCC->CSR=0x%x\r\n",RCC->CSR); //�鿴IWDG�Ƿ�λ
	Delay(6000);
	uart_printf("RCC1->CSR=0x%x\r\n",RCC->CSR); //�鿴IWDG�Ƿ�λ
	uart_printf("RCC2->CSR=0x%x\r\n",RCC->CSR); //�鿴IWDG�Ƿ�λ
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