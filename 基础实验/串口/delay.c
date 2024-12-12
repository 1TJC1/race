#include "delay.h"

void delay_ms(unsigned int xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms--)
	{
		_nop_();
		_nop_();
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}

void delay_us(unsigned int xms)		//@11.0592MHz
{
	while(xms--)
	{
		_nop_();
		_nop_();
		_nop_();
	}
}