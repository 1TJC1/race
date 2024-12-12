#include "hx711.h"

unsigned long ReadCount(void) 
{  
	unsigned long Count;  
	unsigned char i;  
	hx711_SCLK_LOW;//	ADSK=0;       //ʹ��AD��PD_SCK �õͣ�  
	Count=0;  
	
	while(hx711_DAT==1){} //	while(ADDO);    //ADת��δ������ȴ�������ʼ��ȡ  
	for (i=0;i<24;i++)  
	{    
		hx711_SCLK_HIGH;		//		ADSK=1;     //PD_SCK �øߣ��������壩    
		Count=Count<<1; //�½�����ʱ����Count����һλ���Ҳಹ��  
		hx711_SCLK_LOW;//		ADSK=0;     //PD_SCK �õ�   
		if(hx711_DAT==1)//		if(ADDO) 
			Count++;  
	}  
	hx711_SCLK_HIGH;		//	ADSK=1;  
	Count=Count^0x800000;//��25�������½�����ʱ��ת������  
	hx711_SCLK_LOW;	//	ADSK=0;  
	return(Count); 
}
