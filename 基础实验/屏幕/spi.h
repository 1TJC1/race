#ifndef __SPI_H
#define __SPI_H

#define SCLK P12 // ʱ����
#define MOSI P55 // ���豸������豸������
#define CS P47   // Ƭѡ��

void SPI_Init();
void SPI_SendByte(unsigned char dat);
void SPI_SendData(unsigned char *pData, unsigned int length);

#endif
