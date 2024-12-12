#ifndef _TM1650_H
#define _TM1650_H

#include <STC15F2K60S2.H>
#include "intrins.h"

sbit SCL = P1^3;
sbit SDA = P1^4;

extern unsigned char code dig1[11]; //无小数点
extern unsigned char code dig2[11]; //有小数点
extern unsigned char key_data; //键盘扫描码

void Delay5us();
void TM_Start(void);
void TM_Stop(void);
void TM_Ack(void);
void TM_Init(void);
void Dis_TM(unsigned char dig1,unsigned char dig2,unsigned char dig3,unsigned char dig4);
void TM_ReCmd();
void Read_TM();

#endif