#ifndef REG_RW_H
#define REG_RW_H


#define SDCK P22  //SPI 时钟信号
#define SDO  P23  //SPI 数据输出
#define SDI  P24  //SPI 数据输入
#define SCS  P20  //芯片片选信号
#define RSTB P21 	/*复位端口*/


//函数声明
void LD_WriteReg( unsigned char address, unsigned char dataout );
unsigned char LD_ReadReg( unsigned char address );


#endif
