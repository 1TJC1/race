#include <STC15F2K60S2.H>

void GPIO_Init(void)
{
	P0M1 = 0x00;//����P0�˿�ģʽ
	P0M0 = 0xff;
	
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

void main(void)
{
	GPIO_Init();
	while(1)
	{
		if(P13==1)
		{
			P02=0;
		}
		else if(P13==0)
		{
			P02=1;
		}
		if(P14==1)
		{
			P03=0;
		}
		else if(P14==0)
		{
			P03=1;
		}
		if(P15==1)
		{
			P04=0;
		}
		else if(P15==0)
		{
			P04=1;
		}
		if(P16==1)
		{
			P05=0;
		}
		else if(P16==0)
		{
			P05=1;
		}
		if(P17==1)
		{
			P06=0;
		}
		else if(P17==0)
		{
			P06=1;
		}
	}
}
