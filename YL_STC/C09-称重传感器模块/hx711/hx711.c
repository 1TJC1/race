#include "hx711.h"

unsigned long ReadCount(void) 
{  
	unsigned long Count;  
	unsigned char i;  
	hx711_SCLK_LOW;//	ADSK=0;       //使能AD（PD_SCK 置低）  
	Count=0;  
	
	while(hx711_DAT==1){} //	while(ADDO);    //AD转换未结束则等待，否则开始读取  
	for (i=0;i<24;i++)  
	{    
		hx711_SCLK_HIGH;		//		ADSK=1;     //PD_SCK 置高（发送脉冲）    
		Count=Count<<1; //下降沿来时变量Count左移一位，右侧补零  
		hx711_SCLK_LOW;//		ADSK=0;     //PD_SCK 置低   
		if(hx711_DAT==1)//		if(ADDO) 
			Count++;  
	}  
	hx711_SCLK_HIGH;		//	ADSK=1;  
	Count=Count^0x800000;//第25个脉冲下降沿来时，转换数据  
	hx711_SCLK_LOW;	//	ADSK=0;  
	return(Count); 
}
