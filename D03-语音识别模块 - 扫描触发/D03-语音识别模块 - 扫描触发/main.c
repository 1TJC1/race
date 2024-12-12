/*
语言指令：   开灯  ->    led1亮
            关灯  ->    LED1灭

*/

#include "main.h"
#include "LD3320.h"

void GPIO_Init(void)
{
	P0M1 = 0x00;//设置P0端口模式
	P0M0 = 0xff;
	
	P1M1 = 0x00;//设置P1端口模式
	P1M0 = 0x00;
	            
	P2M1 = 0x00;//设置P2端口模式
	P2M0 = 0x00;
	            
	P3M1 = 0x00;//设置P3端口模式
	P3M0 = 0x00;
	            
	P4M1 = 0x00;//设置P4端口模式
	P4M0 = 0x00;
	            
	P5M1 = 0x00;//设置P5端口模式
	P5M0 = 0x00;  
}


void main()
{
	GPIO_Init();

    LD3320_main();
}


