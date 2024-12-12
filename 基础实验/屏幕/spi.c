#include <STC15F2K60S2.H>
#include "spi.h"

void SPI_Init()
{
	SCLK = 1; // ʱ���߳�ʼ״̬Ϊ�ߵ�ƽ
	MOSI = 1; // MOSI�߳�ʼ״̬Ϊ�ߵ�ƽ
	CS = 1;   // Ƭѡ�߳�ʼ״̬Ϊ�ߵ�ƽ
}

void SPI_SendByte(unsigned char dat)
{
	unsigned char i;

	for (i = 0; i < 8; i++) // 8λ����
	{
		MOSI = (dat & 0x80); // �����λд��MOSI��
		dat <<= 1;          // ����һλ
		SCLK = 0;           // ʱ����������
		SCLK = 1;           // ʱ�����½���
	}
	MOSI = 1;
}

void SPI_SendData(unsigned char *pData, unsigned int length)
{
    unsigned int i;

    CS = 0; // ѡ������
    for (i = 0; i < length; i++)
    {
        SPI_SendByte(pData[i]); // ��������
    }
    CS = 1; // ȡ��ѡ������
}