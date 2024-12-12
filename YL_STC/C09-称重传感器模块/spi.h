#ifndef __SPI_H
#define __SPI_H

#define SCLK P12 // 时钟线
#define MOSI P55 // 主设备输出从设备输入线
#define CS P47   // 片选线

void SPI_Init();
void SPI_SendByte(unsigned char dat);
void SPI_SendData(unsigned char *pData, unsigned int length);

#endif
