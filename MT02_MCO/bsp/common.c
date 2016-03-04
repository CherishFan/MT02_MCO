#include "all.h"
/*
#define NVIC_ISER0 	  *((volatile unsigned int*)(0xE000E100))
#define NVIC_ISER1	  *((volatile unsigned int*)(0xE000E104))
void NVIC1_IRQEnable(unsigned int irq)
{
	NVIC_ISER0 = 0x00000001<<irq;
}

void NVIC1_IRQDisable(unsigned int irq)
{
	NVIC_ISER0 =NVIC_ISER0&(~(0x00000001<<irq));
}

void NVIC2_IRQEnable(unsigned int irq)
{
	
	NVIC_ISER1 = 0x00000001<<(irq-32);
}

void NVIC2_IRQDisable(unsigned int irq)
{
	NVIC_ISER1 =NVIC_ISER1&(~(0x00000001<<(irq-32)));
}
*/
void Delay(unsigned long count)
{
	unsigned long i;

	for(i=0;i<count;i++)
		;
}

void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count)
{
	unsigned int i;
	
	for(i=0;i<count;i++)
	{
		if(p1[i] != p2[i])
			uart_printf("tx[%d]=0x%x,rx[%d]=0x%x\r\n",i,p1[i],i,p2[i]);	
	}
}
/*
void jtag_dubug()
{
	unsigned int i,j;
	Uart1Init(30000000L,38400L);
	//Tim1_CentAlignMode1_test3(TIM1,0xff);
	for(j=0;j<20;j++)
	{
			for(i=0;i<1024;i++)
			{
					//uart_printf("b=%x\r\n",buffer[i]);
					uart_printf("b1=%x\r\n",(unsigned char)(*((volatile unsigned int*)(0x20008000+(j*0x400)+i))));
			}
	}
}


void flashSize_test()
{
	unsigned short tmpdata0;
	tmpdata0 = *((volatile unsigned short*)(0x1FFFF7F8));
	uart_printf("flash reset size=0x%x\r\n",tmpdata0);
	FLASH_Unlock();
	FLASH->CR = FLASH->CR | 0xA5BA0000; //²Á³ýÏµÍ³´æ´¢Æ÷
	FLASH_EraseOptionBytes();
	FLASH->CR = FLASH->CR & 0x0000ffff; 
	
	tmpdata0 = *((volatile unsigned short*)(0x1FFFF7F8));
	uart_printf("flash erase size=0x%x\r\n",tmpdata0);
	FLASH->OPTKEYR = 0x45670123;
  FLASH->OPTKEYR = 0xCDEF89AB;
	FLASH->CR |= 0x00000010;
	*((volatile unsigned short*)(0x1FFFF7F8)) = tmpdata0 & 0xfcff; //fdff size is 64K ,fcff size is 32k
	tmpdata0 = *((volatile unsigned short*)(0x1FFFF7F8));
	uart_printf("flash modify size=0x%x\r\n",tmpdata0);
}

void flashReadDataProtection(FunctionalState NewState)
{
	FLASH_Unlock();
	FLASH_ReadOutProtection(NewState);
	
}

void read_flash()
{
	 uint32_t i;
	 uint32_t addr = 0x00000000;
	 for(i=0;i<128*1024;)
	{
		uart_printf("%8x",*((volatile unsigned int*)(addr+i+4)));
		uart_printf("%8x\r\n",*((volatile unsigned int*)(addr+i)));
		
		i = i + 8;
	}
}

void flashWriteProtection(uint32_t FLASH_Pages)
{
	FLASH_Unlock();
	FLASH_EnableWriteProtection(FLASH_Pages);
	
}



void sram_32To32_test()
{
	uint32_t i = 0;
	uint32_t addr = 0x20000000;
	uint32_t tmpdata = 0;
	for(i=0;i<20*1024;)
	{
			*((volatile unsigned int*)(addr+i)) = i;
		   i = i + 4;
		
	}
	for(i=0;i<20*1024;)
	{
			tmpdata = *((volatile unsigned int*)(addr+i));
			uart_printf("%x=%x\r\n",(addr+i),*((volatile unsigned int*)(addr+i)));
		  i = i + 4;
	}
}

void sram_8To8_test()
{
	uint32_t i = 0;
	uint32_t addr = 0x20000800;
	unsigned char tmpdata = 0;
	for(i=0;i<20*1024;)
	{
			*((volatile unsigned char*)(addr+i)) = i;
		   i = i + 1;
		
	}
	for(i=0;i<20*1024;)
	{
			//tmpdata = *((volatile unsigned char*)(addr+i));
			uart_printf("%x=%x\r\n",(addr+i),*((volatile unsigned char*)(addr+i)));
		  //outbyte(tmpdata);
		  i = i + 1;
	}
}

void sram_16To16_test()
{
	uint32_t i = 0;
	uint32_t addr = 0x200003a0;
	unsigned short tmpdata = 0;
	for(i=0;i<20*1024;)
	{
			*((volatile unsigned short*)(addr+i)) = 0xbbcc;
		   i = i + 2;
		
	}
	for(i=0;i<20*1024;)
	{
			//tmpdata = *((volatile unsigned char*)(addr+i));
			uart_printf("%x=%x\r\n",(addr+i),*((volatile unsigned short*)(addr+i)));
		  //outbyte(tmpdata);
		  i = i + 2;
	}
}
*/

void ReadCount(TIM_TypeDef* TIMx)
{
	uint16_t  temp = 0;
	temp = TIM_GetCounter(TIMx);
	uart_printf("CNT=0x%x\r\n",temp);
	
}