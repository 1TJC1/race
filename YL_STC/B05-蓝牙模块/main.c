#include <STC15F2K60S2.H>
#include <stdio.H>

bit busy;

void SendData(unsigned char dat);
void SendString(char *s);

void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0x00;
	
	P1M1 = 0x00;//设置P1端口模式
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//设置P2端口模式
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//设置P3端口模式
	P3M0 = 0xfc;
	            
	P4M1 = 0x00;//设置P4端口模式
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//设置P5端口模式
	P5M0 = 0x00;  
	

}

void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xFE;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
}

void main(void)
{
	GPIO_Init();
	UartInit();
	ES = 1;
	EA = 1;
	P32=1;
	while(1)
	{
		
	}
}

void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
				SendData(SBUF);
    }
}

void SendData(unsigned char dat)
{
    SBUF = dat;                 //写数据到UART数据寄存器
    while (!TI);               //等待前面的数据发送完成
		TI=0;
}

/*----------------------------
发送字符串
----------------------------*/
void SendString(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        SendData(*s++);         //发送当前字符
    }
}
