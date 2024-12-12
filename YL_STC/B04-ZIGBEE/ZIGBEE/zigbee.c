#include "zigbee.H"
#include "uart2.h"
#include "string.h"

unsigned char zigbee_IP[]			={0xFE,0x07,0x90,0x21,0x00,0x00,0x11,0x1F,0x00,0xFF};//IP改成001f   	0001-fffe
unsigned char zigbee_ID[]			={0xFE,0x07,0x90,0x21,0x00,0x00,0x12,0x91,0x19,0xFF};//网络ID改成1991	0001-fffe
unsigned char CHANNEL[]	={0xFE,0x06,0x90,0x21,0x00,0x00,0x13,0x11,0xFF};//信道改为0x12					0b-1a
unsigned char root[]					={0xFE,0x05,0x90,0x21,0x00,0x00,0x10,0xFF};//重启

void check_config(unsigned char config)
{
	unsigned char a[10];
	a[0]=0xfe;
	a[1]=0x05;
	a[2]=0x90;
	a[3]=0x21;
	a[4]=0x00;
	a[5]=0x00;
	a[6]=config;
	a[7]=0xff;
	SendDatas(a,8);
}
void changeIP(unsigned int n)
{
	zigbee_IP[7]=(unsigned char)(n&0xff);
	zigbee_IP[8]=(unsigned char)(n>>8);
	SendDatas(zigbee_IP,sizeof(zigbee_IP));
}
void changeID(unsigned int n)
{
	zigbee_ID[7]=(unsigned char)(n&0xff);
	zigbee_ID[8]=(unsigned char)(n>>8);
	SendDatas(zigbee_ID,sizeof(zigbee_ID));
}
void changeCHANNEL(unsigned char n)
{
	CHANNEL[7]=n;
	SendDatas(CHANNEL,sizeof(CHANNEL));
}
void cq()//重启
{
	SendDatas(root,sizeof(root));
}

void sendmessage(unsigned char port,unsigned int add,unsigned char *d,unsigned char len)//发送至指定地址的节点
{
	unsigned char a[50];
	a[0]=0xfe;
	a[1]=len+4;
	a[2]=port;
	a[3]=port;
	a[4]=(unsigned char)(add&0xff);
	a[5]=(unsigned char)(add>>8);
	memcpy(a+6,d,len);
	a[6+len]=0xff;
	SendDatas(a,7+len);
}
void sendmessages(unsigned char port,unsigned char *d,unsigned char len)//发送至所有的本网络的节点
{
	unsigned char a[50];
	a[0]=0xfe;
	a[1]=len+4;
	a[2]=port;
	a[3]=port;
	a[4]=0xfe;
	a[5]=0xfd;
	a[6]=0xfe;
	a[7]=0xfd;
	memcpy(a+8,d,len);
	a[8+len]=0xff;
	SendDatas(a,9+len);
}



