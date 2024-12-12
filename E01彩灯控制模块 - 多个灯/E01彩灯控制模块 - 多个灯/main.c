#include <STC15F2K60S2.H>
#include "intrins.h"

sbit Led_Pin = P1^5;    //���ƵƵ�����

#include "intrins.h"
#define WS2812_NUM 45  //�Ƶ�����

unsigned short G_0=10;//��ˮ�Ƶ�һ����ɫ����
unsigned short R_0=0;
unsigned short B_0=0;
unsigned short G_1=0;//��ˮ�Ƶڶ�����ɫ����
unsigned short B_1=10;
unsigned short R_1=0;
unsigned short G_2=0;//��ˮ�Ƶ�������ɫ����
unsigned short B_2=0;
unsigned short R_2=10;

void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0x00;
	
	P1M1 = 0x00;//����P1�˿�ģʽ
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//����P2�˿�ģʽ
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//����P3�˿�ģʽ
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//����P4�˿�ģʽ
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//����P5�˿�ģʽ
	P5M0 = 0x00;  
}
void delay_ms(unsigned int ms)  //@24.000MHz
{
    unsigned char data i, j;
    while(ms--){
        _nop_();
        i = 24;
        j = 85;
    do
    {
        while (--j);
    } while (--i);
}
}
void Delay75us(void)    //@24.000MHz
{
    unsigned char data i, j;

    i = 2;
    j = 189;
    do
    {
            while (--j);
    } while (--i);
}

void ws2812_writebit1(void){
    Led_Pin=1;
    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
    Led_Pin=!Led_Pin;
//_nop_();_nop_();_nop_();
}

void ws2812_writebit0(void){
    Led_Pin=1;
    _nop_();_nop_();_nop_();_nop_();_nop_();
    Led_Pin=!Led_Pin;
    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
    _nop_();_nop_();_nop_();_nop_();
}
void ws2812_writecolor(unsigned short dat){
//����STCִ�����ƻ������Ƶ���λ������ʱ̫�ã�2us���޷��Ŷ����Ͷ�������������ʱ���������������ԭʼ��if-else������д���
    if(dat&0x80)
        ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x40)
             ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x20)
             ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x10)
             ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x08)
             ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x04)
             ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x02)
             ws2812_writebit1();
    else
      ws2812_writebit0();
    if(dat&0x01)
             ws2812_writebit1();
    else
      ws2812_writebit0();
}
void ws2812_writeled(unsigned short green,unsigned short red,unsigned short blue)
{
    ws2812_writecolor(green);
    ws2812_writecolor(red);
    ws2812_writecolor(blue);
}       
void ws2812_rest()
{
    Led_Pin=0;

    Delay75us();
}
void ws2812_clear()
{
    static int j=0;
    for(j=0;j<WS2812_NUM;j++)
    {
            ws2812_writeled(0,0,0);
    }
    ws2812_rest();
    ws2812_rest();
    ws2812_rest();
}

void WS2812_DIS()
{
    static int j=0,k=0;
    int i;
    j++;
    j%=WS2812_NUM;
    if(j==0)k++;if(k>=3)k=0;
    for(i=0;i<WS2812_NUM;i++)
    {
        if(i>j)
        {
            switch (k)
            {
                case 0:
                    ws2812_writeled(G_0,R_0,B_0);
                    break;
                case 1:
                    ws2812_writeled(G_1,R_1,B_1);   
                    break;
                case 2:
                    ws2812_writeled(G_2,R_2,B_2);
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (k)
            {
                case 0:
                    ws2812_writeled(G_1,R_1,B_1);   
                    break;
                case 1:
                    ws2812_writeled(G_2,R_2,B_2);
                    break;
                case 2:
                    ws2812_writeled(G_0,R_0,B_0);
                    break;
                default:
                    break;
            }
            
        }
          
    }
}

void main(void)
{
    GPIO_Init();
    ws2812_clear();
    while(1)
    {
        WS2812_DIS();
        delay_ms(50);
    }
}
