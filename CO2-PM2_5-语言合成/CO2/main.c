#include "main.h"
#include "intrins.h"
#include "spi.h"
#include "lcd.h"
#include "lcd_init.h"
#include "uart2.h"

extern unsigned char flag_updata; //用于定时发数据，或者定时闪烁工作LED灯
extern unsigned int  time_updata;


unsigned char read_co2[]={0xff,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

void led()
{
	if(flag_updata==1)
	{
		flag_updata=0;
		LED1 = ~LED1;
		SendDatas(read_co2,sizeof(read_co2));
	}
}
unsigned int co2_data=0;
void read_co2_data()
{
	static unsigned char co2_value_pro=0;
	static unsigned char cnt1_value=1;
	if(UART_CONFIG.ReceiveComplete==1)
	{
		UART_CONFIG.ReceiveComplete=0;
		
		co2_data=UART_CONFIG.ReceiveBuff[2]*256+UART_CONFIG.ReceiveBuff[3];
		display_printf(80,145,GREEN,BLACK,16,0," %5d",co2_data);

		LCD_DrawLine(20+(cnt1_value-1),140-co2_value_pro,20+cnt1_value,140-co2_data/50,GREEN);//温度的折线图
		co2_value_pro=co2_data/50;
		
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
			read_co2_data();
		}
}
