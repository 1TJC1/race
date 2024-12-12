#include "main.h"
#include "intrins.h"
#include "uart2.h"
#include "zigbee.H"

sbit key =P3^3;
extern unsigned char flag_updata; //用于定时发数据，或者定时闪烁工作LED灯
extern unsigned int  time_updata;
void delay(unsigned int t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
		for(j=0;j<1000;j++);
}

unsigned char num_gw=3;
unsigned char num_sb=1;
system_zigbee Information={0x1100, 0x1000,0x11};  //ID  IP  CHANNL
system_zigbee Target_zigbee={0x1101, 0x1002,0x11};  //目标zigbee 的 ID  IP  CHANNL

unsigned char r[]				={0xFE,0x05,0x21,0x90,0x00,0x00,0x00,0xFF};//ZigBee设置时返回的数据
unsigned char check_ip[]={0xFE,0x05,0x90,0x21,0x00,0x00,0x01,0xFF};
unsigned char check_id[]={0xFE,0x05,0x90,0x21,0x00,0x00,0x02,0xFF};
unsigned char check_ch[]={0xFE,0x05,0x90,0x21,0x00,0x00,0x03,0xFF};
unsigned char return_ip[2];
unsigned char return_id[2];
unsigned char return_ch=0;
unsigned char s=0;//配置zigbee时的状态机
unsigned char flag_con=0;//如果zigbee有重新设置  要重启
unsigned char flag_zigbee=0;//0:普通的通讯   1：zigbee配置
void check_zigbeeconfig()
{
	switch (s)
  {
  	case 0://检查设备IP，若设备IP与工位拨码的值不一致，则重新设置设备IP
				SendDatas(check_ip,sizeof(check_ip));	delay(5000);
				flag_con=0;
				s=1;
  		break;
  	case 1:
				if(UART_CONFIG.ReceiveComplete==1)
				{
					UART_CONFIG.ReceiveComplete=0;
					if(UART_CONFIG.ReceiveBuff[6]==0x21)
					{
						return_ip[0]=UART_CONFIG.ReceiveBuff[8];
						return_ip[1]=UART_CONFIG.ReceiveBuff[7];
						if((return_ip[0]!=0x10)||(return_ip[1]!=num_gw))
						{
							Information.zigbee_ip=0x1000+num_gw;
							changeIP(Information.zigbee_ip); delay(1000);
							flag_con=1;
						}
						s=2;
					}
					else 
						s=0;
				}
  		break;
		case 2://检查ID号，若ID号与设备拨码的值不一致，则重新设置ID
				SendDatas(check_id,sizeof(check_id));	delay(1000);
				s=3;
  		break;
  	case 3:
				if(UART_CONFIG.ReceiveComplete==1)
				{
					UART_CONFIG.ReceiveComplete=0;
					if(UART_CONFIG.ReceiveBuff[6]==0x22)
					{
						return_id[0]=UART_CONFIG.ReceiveBuff[8];
						return_id[1]=UART_CONFIG.ReceiveBuff[7];
						if((return_id[0]!=0x11)||(return_id[1]!=num_sb))
						{
							Information.zigbee_id=0x1100+num_sb;
							changeID(Information.zigbee_id); delay(1000);
							flag_con=1;
						}
						s=4;
					}
					else 
						s=2;
				}
  		break;
		case 4:
				SendDatas(check_ch,sizeof(check_ch));	delay(1000);
				s=5;
  		break;
  	case 5:
				if(UART_CONFIG.ReceiveComplete==1)
				{
					UART_CONFIG.ReceiveComplete=0;
					if(UART_CONFIG.ReceiveBuff[6]==0x23)
					{
						return_ch=UART_CONFIG.ReceiveBuff[7];
						if(Information.zigbee_channel!=return_ch)
						{
							changeCHANNEL(Information.zigbee_channel); delay(1000);
							flag_con=1;
						}
						s=6;
					}
					else 
						s=4;
				}
  		break;
		case 6:
				if(flag_con==1)
				{
					flag_con=0;
					cq();
					delay(1000);
				}
				flag_zigbee=0;
				s=0;
				LED2 = 0;
  		break;		
  	default:
  		break;
  }
}
void zigbee_config()
{ 
	if(flag_zigbee==1)//配置参数
	{
		check_zigbeeconfig();
	}
	else if(flag_zigbee==0)//接收数据
	{
		if(UART_CONFIG.ReceiveComplete==1)
		{
			UART_CONFIG.ReceiveComplete=0;
			switch (UART_CONFIG.ReceiveBuff[2])
      {
      	case 0xA0://读取数据
						LED3 = ~LED3;
      		break;
      	
      	default:
      		break;
      }

		}
	}
}

void led()
{
	if(flag_updata==1)
	{
		flag_updata=0;
		LED1 = ~LED1;

	}
}

void main()
{
	unsigned char flag_key=0;
	unsigned char data_cmd[]={0x00,0x01};
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
	
	flag_zigbee=1;
	

	while(1)
	{
		led();
		zigbee_config();
		
		if(key == 0 && flag_key==0)
		{
			delay(10);
			if(key == 0)
			{
				flag_key=1;
				
				LED4 = ~LED4;
				sendmessage(0xa0,Target_zigbee.zigbee_ip,data_cmd,2);
			}
		}
		else if(key == 1)
		{
			flag_key=0;
		}
	}
}
