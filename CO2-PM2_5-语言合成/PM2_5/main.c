#include "main.h"
#include "intrins.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "uart2.h"

void led()
{
	if(flag_updata==1)
	{
		flag_updata=0;
		LED1 = ~LED1;
	}
}
unsigned int PM2_5_data=0;
unsigned int PM10_0_data=0;
void read_PM_data()
{
	static unsigned char PM2_5_value_pro=0;
	static unsigned char PM10_0_value_pro=0;
	static unsigned char cnt1_value=1;
	if(UART_CONFIG.ReceiveComplete==1)
	{
		LED2 = ~LED2;
		UART_CONFIG.ReceiveComplete=0;
		
		PM2_5_data=UART_CONFIG.ReceiveBuff[12]*256+UART_CONFIG.ReceiveBuff[13];
		display_printf(20,145,GREEN,BLACK,16,0,"PM2.5:%5d",PM2_5_data);

		PM10_0_data=UART_CONFIG.ReceiveBuff[14]*256+UART_CONFIG.ReceiveBuff[15];
		display_printf(20,165,BLUE,BLACK,16,0,"PM10 :%5d",PM10_0_data);
		
		LCD_DrawLine(20+(cnt1_value-1),140-PM2_5_value_pro,20+cnt1_value,140-PM2_5_data,GREEN);//温度的折线图
		PM2_5_value_pro=PM2_5_data;
		
		LCD_DrawLine(20+(cnt1_value-1),140-PM10_0_value_pro,20+cnt1_value,140-PM10_0_data,BLUE);//温度的折线图
		PM10_0_value_pro=PM10_0_data;
		
		cnt1_value++;
		if(cnt1_value>=180)
		{
			cnt1_value=1;
			LCD_Fill( 20,40,200,140,BLACK);//清
			
			LCD_DrawLine(20,40,20,140,BROWN);		//坐标轴
			LCD_DrawLine(20,140,200,140,BROWN);
		}
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
		
		SPI_Init();

		LCD_Init();//LCD初始化
		LCD_Clear1(BLACK);
		LCD_DrawLine(20,40,20,140,BROWN);		//坐标轴
		LCD_DrawLine(20,140,200,140,BROWN);

    while(1)
		{
			led();
			read_PM_data();
		}
}

