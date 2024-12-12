#include "main.h"
#include "intrins.h"
#include "uart2.h"
#include "XFS5051.h"

extern unsigned char flag_updata; //用于定时发数据，或者定时闪烁工作LED灯
extern unsigned int  time_updata;

void led()
{
	if(flag_updata==1)
	{
		flag_updata=0;
		LED1 = ~LED1;
		XFS_FrameInfo((unsigned char *)"欢迎使用亚龙派",sizeof"欢迎使用亚龙派");
	}
}

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;

		init_uart2();
		init_time0();
    while(1)
		{
			led();
		}
}
