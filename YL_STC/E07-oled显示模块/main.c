#include <STC15F2K60S2.H>
#include <oled.H>

void main(void)
{
	OLED_Init();
	OLED_ShowString(0,0,"123",16);
	while(1)
	{
		
	}
}