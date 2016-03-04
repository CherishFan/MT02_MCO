#include "all.h"
void CRC_test(unsigned int calData)
{
	unsigned int temp = 0;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	CRC_ResetDR();
	CRC_CalcCRC(calData);
	temp = CRC_GetCRC();
	//temp = temp;		 //why ,��������仰�Ϳ�����temp����ʵֵ��
	uart_printf("crc=0x%x\r\n",temp);
}

void CRCallTset()
{
	CRC_test(0xffffffff);		//������crc����������
	CRC_test(0xfffffffe);		//������crc����������
	CRC_test(0x7fffffff);
	CRC_test(0xffffffef);
	CRC_test(0xffffffdf);
	CRC_test(0xffffffbf);
	CRC_test(0xffffff7f);
	CRC_test(0xfffffffc);
	CRC_test(0xfffffffa);
}

void CRC_Register_test()
{
	uart_printf("CRC->DR=0x%x\r\n",CRC->DR);     //0x0
	uart_printf("CRC->IDR=0x%x\r\n",CRC->IDR);   //0x0
	uart_printf("CRC->CR=0x%x\r\n",CRC->CR);    //0x0
}