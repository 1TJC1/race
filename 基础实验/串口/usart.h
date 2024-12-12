#ifndef __USART_H
#define __USART_H	

#include <STC15F2K60S2.H>
#include "stdio.h"

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
	
typedef unsigned char BYTE;
typedef unsigned int WORD;

#define FOSC 11059200L          //ϵͳƵ��
#define BAUD 115200             //���ڲ�����

#define NONE_PARITY     0       //��У��
#define ODD_PARITY      1       //��У��
#define EVEN_PARITY     2       //żУ��
#define MARK_PARITY     3       //���У��
#define SPACE_PARITY    4       //�հ�У��

#define PARITYBIT NONE_PARITY   //����У��λ

void UartInit(void);
	
#endif