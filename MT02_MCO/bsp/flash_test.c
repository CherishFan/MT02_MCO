


/* Includes ------------------------------------------------------------------*/
#include "all.h"
/*
void Delay(unsigned int number)
{
    unsigned int i;
    for(i=0;i<number;i++)
    {}
}

void shfit(unsigned short number)
{
	unsigned char i;
    unsigned short tmpdata;
	for(i=16;i>0;i--)
	{
		tmpdata = (number>>(i-1)) & 0x01;
	}
}

void write_50bits(unsigned short CEb,unsigned short OEb,unsigned short WEb,unsigned short PROG,unsigned short PROG2,unsigned short ERASE,unsigned short CHIP,unsigned short NVR)
{
    unsigned short tempdata = 0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;

    tempdata = (CEb<<15) | (OEb<<14) | (WEb<<13) | (PROG<<12) | (PROG2<<11) | (ERASE<<10) | (CHIP<<9) | (NVR<<8);
    TDI_WriteControl(tempdata);
    TDI_WriteControl(0x0000);
    TDI_WriteControl(0x00f0);
    TDI_Writebit49_50(0x8000);
   

}



void  Strobe_Control()
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_1);//TCK = 0;
	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;
   	GPIO_SetBits(GPIOC, GPIO_Pin_1);  //TCK = 1;
	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

	GPIO_ResetBits(GPIOC, GPIO_Pin_1);//TCK = 0;
	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

    GPIO_SetBits(GPIOC, GPIO_Pin_1);  //TCK = 1;
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;
}

unsigned short TDI_ReadControl()
{
	unsigned char i;
    unsigned short tmpdata;
    unsigned short tmpdata1 = 0;

	for(i=0;i<16;i++)
	{
        GPIO_SetBits(GPIOC, GPIO_Pin_1);//TCK = 1;
   
		//tmpdata = TDI&0x01;
    	tmpdata = (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2));
        tmpdata1 |= ((tmpdata&0x01)<<i);
	    GPIO_ResetBits(GPIOC, GPIO_Pin_1);  //TCK = 0;     

	}
    return tmpdata1;
}


void  TDI_WriteControl(unsigned short value)
{
	unsigned char i;
    unsigned short tmpdata;

	for(i=16;i>0;i--)
	{
        tmpdata = (value>>(i-1)) & 0x01;
     
		GPIO_ResetBits(GPIOC, GPIO_Pin_1);  //TCK = 0;     
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, tmpdata);  //TDI = value;
		GPIO_SetBits(GPIOC, GPIO_Pin_1);//TCK = 1;
	}
}

void TDI_Writebit49_50(unsigned short value)
{
    unsigned char i;
    unsigned short tmpdata;
   	for(i=16;i>0;i--)
	{
        tmpdata = (value>>(i-1)) & 0x01;

		GPIO_ResetBits(GPIOC, GPIO_Pin_1);  //TCK = 0;     
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, tmpdata);  //TDI = value;
		GPIO_SetBits(GPIOC, GPIO_Pin_1);//TCK = 1;

        if(i==14)
        {
             GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;
        }
	}

}



void GPIO_Set()
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO initial
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //enable gpio clk
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


void write_50bits_Ctl(unsigned short CEb,unsigned short OEb,unsigned short WEb,unsigned short PROG,unsigned short PROG2,unsigned short ERASE,unsigned short CHIP,unsigned short NVR,unsigned short addr,unsigned short A_1,unsigned short A_2,unsigned short DIN,unsigned short MSA,unsigned short MSB,unsigned short MSC,unsigned short MSD)
{
    unsigned short tempdata0 = 0;
    unsigned short tempdata1 = 0;
    unsigned short tempdata2 = 0;
    unsigned short tmpaddr0 = 0;
    unsigned short tmpDin0 = 0;
    unsigned short tmpDin1 = 0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;
    tmpDin0 = (((DIN&0x0001)<<7) | ((DIN&0x0002)<<6) | ((DIN&0x0004)<<5)|((DIN&0x0008)<<4)|((DIN&0x0010)<<3)|((DIN&0x0020)<<2)|((DIN&0x0040)<<1)|((DIN&0x0080)<<0)) & 0x00ff; 
    tmpDin1 = (((DIN&0x0100)<<15) | ((DIN&0x0200)<<14) | ((DIN&0x0400)<<13)|((DIN&0x0800)<<12)|((DIN&0x1000)<<11)|((DIN&0x2000)<<10)|((DIN&0x4000)<<9)|((DIN&0x8000)<<8))&0xff00; 
    tmpaddr0 = (((addr&0x0001)<<6) | ((addr&0x0002)<<5)|((addr&0x0004)<<4)|((addr&0x0008)<<3)|((addr&0x0010)<<2)|((addr&0x0020)<<1)|((addr&0x0040)<<0))&0x007f; //addr0-addr6

    tempdata0 = (CEb<<15) | (OEb<<14) | (WEb<<13) | (PROG<<12) | (PROG2<<11) | (ERASE<<10) | (CHIP<<9) | (NVR<<8) | tmpaddr0; //CEb,OEb,WEb,PROG,PROG2,ERASE,CHIP,NVR,addr0-addr6

    tempdata1 = ((addr&0x0080)<<15) | ((addr&0x0100)<<14)|((addr&0x0200)<<13)|((addr&0x0400)<<12)|((addr&0x0800)<<11)|((addr&0x1000)<<10)|(A_1<<9)|(A_2<<8) | tmpDin0; //addr7-addr12,A_1,A_2,DIN0-DIN7
    //tempdata2 = tmpDin1 | 0x00f3;  //DIN8 - DIN15  msa ,msb,msc,msd, freq0,freq1,tprog_conf0,tprog_conf1
    tempdata2 = tmpDin1 | (MSA<<7)|(MSB<<6)|(MSC<<5)|(MSD<<4)| 0x0003;  //DIN8 - DIN15  msa ,msb,msc,msd, freq0,freq1,tprog_conf0,tprog_conf1

    TDI_WriteControl(tempdata0);
    TDI_WriteControl(tempdata1);
    TDI_WriteControl(tempdata2);
    TDI_Writebit49_50(0x8000);
   

}


void TDI_ReadData()
{
    unsigned short tmpdata0;
    unsigned short tmpdata1;
    unsigned short tmpdata2;
    unsigned short tmpdata3;
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO initial
   

    write_50bits_Ctl(1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0); //CEb=1,oeb=1,web=1,addr=0; send addr
    Delay(1);   //
    write_50bits_Ctl(0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0); //CEb=1,oeb=1,web=1,addr=0; send addr
    Delay(1); 

    GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    tmpdata0 = TDI_ReadControl();
    tmpdata1 = TDI_ReadControl();
    tmpdata2 = TDI_ReadControl();
    tmpdata3 = TDI_ReadControl();

    Delay(1); 
    write_50bits_Ctl(1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0); //CEb=1,oeb=1,web=1,addr=0;

    uart_printf("tmpdata0=0x%x\n",tmpdata0);
    uart_printf("tmpdata1=0x%x\n",tmpdata1);
    uart_printf("tmpdata2=0x%x\n",tmpdata2);
    uart_printf("tmpdata3=0x%x\n",tmpdata3);

}


void program_arry16()
{
    write_50bits_Ctl(1,0,1,0,0,0,0,0,0x0,0,0,0x0,0,1,0,0); //CEb=1,oeb=0,web=1,addr=0x0;DIN = 0x0
    write_50bits_Ctl(0,0,1,1,0,0,0,0,0x0,0,0,0x0,0,1,0,0); //CEb=0,oeb=0,web=1,prog=1,addr=0x0;DIN = 0x0
    Delay(16); //Tnvs
    write_50bits_Ctl(0,0,0,1,0,0,0,0,0x0,0,0,0x0,0,1,0,0); //CEb=0,oeb=0,web=0,prog=1,addr=0x0;DIN = 0x0
    //Delay(4); //Tpgs
    //Delay(30);
    write_50bits_Ctl(0,0,0,1,1,0,0,0,0x0,0,0,0x87,0,1,0,0); //CEb=0,oeb=0,web=0,prog=1,prog2=1,addr=0x0;DIN = 0x0
    //Delay(6); //Tprog
   // Delay(40);
    write_50bits_Ctl(0,0,0,1,0,0,0,0,0x0,0,0,0x0,0,1,0,0); //CEb=0,oeb=0,web=0,prog=1,prog2=0,addr=0x0;DIN = 0x0

    write_50bits_Ctl(0,0,0,1,0,0,0,0,0x3,0,0,0x88,0,1,0,0); //CEb=0,oeb=0,web=0,prog=1,prog2=0,addr=0x0;DIN = 0x0
    write_50bits_Ctl(0,0,0,1,1,0,0,0,0x3,0,0,0x88,0,1,0,0); //CEb=0,oeb=0,web=0,prog=1,prog2=1,addr=0x0;DIN = 0x0
    //Delay(48); //Tprog
    write_50bits_Ctl(0,0,0,1,0,0,0,0,0x3,0,0,0x88,0,1,0,0); //CEb=0,oeb=0,web=0,prog=1,prog2=0,addr=0x2;DIN = 0x87
    // Delay(1); //Tprog
    write_50bits_Ctl(0,0,1,1,0,0,0,0,0x3,0,0,0x88,0,1,0,0); //CEb=0,oeb=0,web=1,prog=1,prog2=0,addr=0x2;DIN = 0x87
   // Delay(40); //Trcv

     write_50bits_Ctl(0,0,1,0,0,0,0,0,0x3,0,0,0x88,0,1,0,0); //CEb=0,oeb=0,web=1,prog=0,prog2=0,addr=0x2;DIN = 0x87
     Delay(1); //Trw

}

void sector_erase()
{
    write_50bits_Ctl(0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1); //write_50bits(0,0,1,0,0,1,0,0);
    Delay(16);   //Tnvs  2us
    write_50bits_Ctl(0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1); //write_50bits(0,0,0,0,0,1,0,0);
    Delay(20000); //Terase 4ms
    write_50bits_Ctl(0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1); //write_50bits(0,0,1,0,0,1,0,0);
    Delay(400);  //Trcv  50us
    write_50bits_Ctl(0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1); //write_50bits(0,0,1,0,0,0,1,0);
	
}


void flash_OB_test(void)
{
	FLASH_Unlock();
	FLASH_EraseOptionBytes();
	FLASH_UserOptionByteConfig(OB_IWDG_HW, OB_STOP_NoRST, OB_STDBY_NoRST);
	uart_printf("Flash1=0x%x\r\n",FLASH->CR);
	uart_printf("OB=0x%x\r\n",OB->USER);
}


struct bist_struct {
    uint8_t   ceb;        //1 bit
    uint8_t   oeb;        //1 bit
    uint8_t   web;        //1 bit
    uint8_t   prog;       //1
    uint8_t   prog2;      //1
    uint8_t   erase;      //1
    uint8_t   chip;       //1
    uint8_t   nvr;        //1
    uint8_t   tmen;       //1
    uint16_t  addr;       //13
    uint8_t   a_1_2;      //2
    uint16_t  data;       //16
    uint8_t   ms;         //4
    uint8_t   freq;       //2
    uint8_t   tprog;      //2
    uint8_t   terase;     //2
} bist_data;

//bist_struct bist_data;


void delay(unsigned int number)
{
    unsigned int i;
    for(i=0;i<number;i++)
    {}
}

void toggle_tck(uint16_t num)
{
    uint16_t i;
    for(i=0;i<num;i++) 
    {
   	    //GPIO_SetBits(GPIOC, GPIO_Pin_1);  //TCK = 1;
        GPIOC->BSRR = GPIO_Pin_1;
        //GPIO_ResetBits(GPIOC, GPIO_Pin_1);//TCK = 0;
        GPIOC->BRR = GPIO_Pin_1;
    }
}

void output_bit(uint8_t obit)
{
   	//GPIO_SetBits(GPIOC, GPIO_Pin_1);  //TCK = 1;
    GPIOC->BSRR = GPIO_Pin_1;
    //delay(10);
    //GPIO_ResetBits(GPIOC, GPIO_Pin_1);//TCK = 0;
    GPIOC->BRR = GPIO_Pin_1;
    //GPIO_WriteBit(GPIOC, GPIO_Pin_2, obit);  //TDI = value;
    if (obit != 0)
        GPIOC->BSRR = GPIO_Pin_2;
    else
        GPIOC->BRR = GPIO_Pin_2;
}

void  Strobe_Control()
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_1);//TCK = 0;
	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;
   	GPIO_SetBits(GPIOC, GPIO_Pin_1);  //TCK = 1;
	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

	GPIO_ResetBits(GPIOC, GPIO_Pin_1);//TCK = 0;
	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

    GPIO_SetBits(GPIOC, GPIO_Pin_1);  //TCK = 1;
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;
}


void shift_in()
{
	uint8_t i;
    uint8_t sd[50];

    sd[0] = bist_data.ceb;
    sd[1] = bist_data.oeb;
    sd[2] = bist_data.web;
    sd[3] = bist_data.prog;
    sd[4] = bist_data.prog2;
    sd[5] = bist_data.erase;
    sd[6] = bist_data.chip;
    sd[7] = bist_data.nvr;
    sd[8] = bist_data.tmen;
    for(i=0;i<=12;i++) sd[i+9] = (bist_data.addr >> i) & 0x01;
    sd[22] = (bist_data.a_1_2 >> 1) & 0x01;
    sd[23] = bist_data.a_1_2 & 0x01;
    for(i=0;i<=15;i++) sd[i+24] = (bist_data.data >> i) & 0x01;
    for(i=0;i<=3;i++) sd[i+40] = (bist_data.ms >> i) & 0x01;
    sd[44] = bist_data.freq & 0x01;
    sd[45] = (bist_data.freq >> 1) & 0x01;
    sd[46] = bist_data.tprog & 0x01;
    sd[47] = (bist_data.tprog >> 1) & 0x01;
    sd[48] = bist_data.terase & 0x01;
    sd[49] = (bist_data.terase >> 1) & 0x01;

    Strobe_Control();

	for(i=0;i<50;i++) output_bit( sd[i] );
	for(i=0;i<2;i++)  output_bit( 0 );
}


void bist_init()
{
    bist_data.ceb = 1;
    bist_data.oeb = 1;
    bist_data.web = 1;
    bist_data.prog = 0;
    bist_data.prog2 = 0;
    bist_data.erase = 0;
    bist_data.chip = 0;
    bist_data.nvr = 0;
    bist_data.tmen = 0;
    bist_data.addr = 0;
    bist_data.a_1_2 = 0;
    bist_data.data = 0;
    bist_data.ms = 0xF;
    bist_data.freq = 0;     //dynamic TCK
    bist_data.tprog = 1;    //6us, actual 7.5us
    bist_data.terase = 1;   //4ms, actual 5ms
}

uint64_t BIST_ReadData(uint16_t addr, uint8_t nvr)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint64_t rdata;
    uint8_t i;
    uint64_t tmp;

    rdata = 0;

    bist_data.ceb = 0;
    bist_data.oeb = 0;
    bist_data.addr = addr;
    bist_data.nvr = nvr;
    bist_data.ms = 0x1;

    shift_in();

	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

    //toggle_tck(100);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    for(i=0;i<7;i++)
        output_bit(0);

    for(i=0;i<64;i++)
    {
        tmp = (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2));
        rdata |= ((tmp & 0x01)<<i);
        output_bit(0);
    }

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;

    bist_data.ceb = 1;
    bist_data.oeb = 1;
    bist_data.addr = 0;
    bist_data.nvr = 0;
    bist_data.ms = 0xF;

    //shift_in();
    //Strobe_Control();

    return rdata;
}

void BIST_WriteData(uint16_t addr, uint8_t a_1_2, uint16_t data, uint8_t nvr)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;
    uint8_t tmp;


    bist_data.ceb = 1;
    bist_data.addr = addr;
    bist_data.a_1_2 = a_1_2;
    bist_data.data = data;
    bist_data.nvr = nvr;
    bist_data.ms = 0xf;

    shift_in();
    Strobe_Control();

    bist_data.ceb = 0;
    bist_data.ms = 0xe;
    shift_in();

	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

    toggle_tck(4);
        GPIOC->BSRR = GPIO_Pin_1;
        GPIOC->BRR = GPIO_Pin_1;
    for(i=0;i<1;i++);
    toggle_tck(1);
    delay(1);
    toggle_tck(1);
    toggle_tck(30);
    
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;

    bist_data.ceb = 1;
    bist_data.addr = 0;
    bist_data.data = 0;
    bist_data.nvr = 0;
    bist_data.ms = 0xF;

    //shift_in();
    //Strobe_Control();

    return;
}

void BIST_SectorErase(uint8_t nvr)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    uint8_t i;
    uint8_t tmp;
    uint16_t sector_max;

    if(nvr)
        sector_max = 2;
    else
        sector_max = 64;

    bist_data.nvr = nvr;
    bist_data.ms = 0x8;

    shift_in();
    //Strobe_Control();

 	GPIO_SetBits(GPIOC, GPIO_Pin_3); //STROBE = 1;

    toggle_tck(2);

    for(i=0;i<sector_max;i++){
        toggle_tck(2);
        delay(4000);
        toggle_tck(1);
        delay(100);
        toggle_tck(1);
        delay(100);
    }
    
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//STROBE = 0;

    bist_data.nvr = 0;
    bist_data.ms = 0xF;

    //shift_in();
    //Strobe_Control();

    return;
}



void GPIO_Set()
{
	GPIO_InitTypeDef GPIO_InitStructure;//GPIO initial
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //enable gpio clk
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}


*/



// void flash_bist_test()
// {
// 	uint32_t data32;
//     uint16_t data16;
//     uint64_t data64;
//     uint32_t addr;
//     uint32_t nvr;
//     int i;
//     
// 	uart_printf("GPIO to Flash serial BIST.\r\n");
// 	bist_init();
//    	GPIO_Set();

//     nvr = 1;
//     addr = 0x80;
     //addr = 0x80;
		//addr = 0x1FFFF800;
		//addr = 0x4002201c;
    //full erase
  //  BIST_SectorErase(nvr);
/*
    //program
    BIST_WriteData(addr, 0, 0x1234, nvr);        //bit 15:0
    BIST_WriteData(addr, 1, 0x01fe, nvr);        //bit 31:16
    BIST_WriteData(addr, 2, 0x1357, nvr);        //bit 47:32
    BIST_WriteData(addr, 3, 0x2468, nvr);        //bit 63:48
*/
/*
	  BIST_WriteData(addr, 0, 0x5aa5, nvr);        //bit 15:0
    BIST_WriteData(addr, 1, 0x01fe, nvr);        //bit 31:16 bit16 Ñ¡ÔñÎª0
    BIST_WriteData(addr, 2, 0xffff, nvr);        //bit 47:32 
    BIST_WriteData(addr, 3, 0xffff, nvr);        //bit 63:48
*/

    //read data
		//for(i=0;i<65536;i=i+8)
// 		for(i=0;i<4;i=i+1)
// 		{
//     data64 = BIST_ReadData(addr+i, nvr);

//     data32 = (data64>>32);
//     uart_printf("read data high is %x", data32);
//     data32 = data64;
//     uart_printf(" %x\r\n", data32);
//		uart_printf("read data low 0x%x  \r\n", (addr+i));
//		}
//}

void flash_OB_test(void)
{
	FLASH_Unlock();
	FLASH_EraseOptionBytes();
	FLASH_UserOptionByteConfig(OB_IWDG_HW, OB_STOP_NoRST, OB_STDBY_NoRST);
}


void read_flash(uint32_t Page_Address)
{
	 uint32_t i;
	 uint32_t addr = Page_Address & 0xffffff00;
	// for(i=0;i<4096;)
	//{
		//uart_printf("add0x%x = 0x%x\r\n",(addr+i),*((volatile unsigned int*)(addr+i)));
		uart_printf("add = 0x%x\r\n",*((volatile unsigned int*)(addr)));
	  uart_printf("0x1FFFF7F8 = 0x%x\r\n",*((volatile unsigned int*)(0x1FFFF7F8)));
		//i = i + 4;
	//}
}


void flash_write(uint32_t Page_Address,uint32_t data1)
{
	uint32_t i;
	uint32_t pageAddress = Page_Address & 0xffffff00;
	uint32_t tmpdata = 0x0;
	FLASH_Unlock();
	FLASH_ErasePage(pageAddress);
	//for(i=0;i<1024;i=i+1)
	//{
		FLASH_ProgramWord((pageAddress), (data1));
	//}
	FLASH_SetLatency(FLASH_Latency_2);
	read_flash(pageAddress);
}

void flash_SYSTEM_test()
{
	unsigned int temp;
	FLASH_Unlock();
	//uart_printf("FLASH->OBR0=0x%x\r\n",FLASH->OBR);
	//temp = FLASH_GetUserOptionByte();
	//uart_printf("temp1=0x%x\r\n",temp);
	//FLASH_EraseOptionBytes();
	
	FLASH->CR = FLASH->CR | 0xA5BA0000; //²Á³ýÏµÍ³´æ´¢Æ÷ 0x1ffff400
	
	//FLASH_EraseOptionBytes();
	//FLASH_ProgramHalfWord(0x1FFFF7F8, 0xefffffff);
	//temp = *((volatile unsigned int*)(0x1FFFF7F8));
	//uart_printf("temp=0x%x\r\n",temp);
	
	
	//temp = FLASH_GetUserOptionByte();
	//uart_printf("temp2=0x%x\r\n",temp);
	//uart_printf("OB->USER=0x%x\r\n",OB->USER);
	//uart_printf("FLASH->OBR1=0x%x\r\n",FLASH->OBR);
}


void flash_SYSTEM_erase()
{
	FLASH_Unlock();
	FLASH->CR = FLASH->CR | 0xA5BA0000; //²Á³ýÏµÍ³´æ´¢Æ÷ 0x1ffff400
	FLASH_EraseOptionBytes();
	
}

void falsh_SYSTEM_IDProgram(unsigned int id0,unsigned int id1,unsigned int id2)
{
	unsigned int temp;
	
	FLASH_Unlock();
	FLASH->CR = FLASH->CR | 0xA5BA0000; //²Á³ýÏµÍ³´æ´¢Æ÷ 0x1ffff400
	
	FLASH_ProgramWord(0x1FFFF7E8, id0);
	temp = *((volatile unsigned int*)(0x1FFFF7E8));
	
	
	FLASH_ProgramWord(0x1FFFF7EC, id1);
	temp = *((volatile unsigned int*)(0x1FFFF7EC));

	FLASH_ProgramWord(0x1FFFF7F0, id2);
	temp = *((volatile unsigned int*)(0x1FFFF7F0));
	
}


void flash_readprotection_release()
{
	FLASH_Unlock();
	FLASH_EraseOptionBytes();
	FLASH_ReadOutProtection(DISABLE);
}