/*----------------------------------------------------
ALL.H

Header file for generic MT01 microcontroller.
-----------------------------------------------------*/

#ifndef __ALL_H__
#define __ALL_H__

#include "cc10x.h"
#include "misc.h"
#include "cc10x_rcc.h"
#include "cc10x_usart.h"
#include "cc10x_gpio.h"
#include "cc10x_spi.h"
#include "cc10x_dma.h"
#include "cc10x_i2c.h"
#include "cc10x_tim.h"
#include "cc10x_crc.h"
#include "cc10x_adc.h"
#include "cc10x_dac.h"
#include "cc10x_comp.h"
#include "cc10x_iwdg.h"
#include "cc10x_wwdg.h"
#include "cc10x_exti.h"
#include "cc10x_flash.h"
#include "cc10x_rtc.h"
#include "cc10x_pwr.h"
#include "cc10x_bkp.h"
#include "cc10x_can.h"
#include "cc10x_opamp.h"
//#include "mx01_i2s.h"
#include "usb.h"


//#define DBG

void Delay(unsigned long count);
void NVIC1_IRQEnable(unsigned int irq);
void NVIC1_IRQDisable(unsigned int irq);
void NVIC2_IRQEnable(unsigned int irq);
void NVIC2_IRQDisable(unsigned int irq);
void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count);
void jtag_dubug(void);
void uart_printf( const char * ctrl1, ...);
void SystemClkInit (void);
void ReadCount(TIM_TypeDef* TIMx);

//#define 	FLASH_DEVICE_ADDR		0xA8
#define 	FLASH_DEVICE_ADDR		0xA8
//#define   I2C_INT_MODE
//#define   SPI_INT_MODE	
//#define   SPI_DMA

#endif
