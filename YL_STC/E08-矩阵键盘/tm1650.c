#include <tm1650.H>

unsigned char code dig1[11]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x40};
//0��1��2��3��4��5��6��7��8��9��-//����С����

unsigned char code dig2[11]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef,0xc0};
//0��1��2��3��4��5��6��7��8��9��-//��С����

unsigned char key_data = 0; //����ɨ����

void Delay2us()		//@11.0592MHz
{
	unsigned char i;

	i = 3;
	while (--i);
}

void Delay10us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 25;
	while (--i);
}

// TM Start//
void TM_Start(void)
{
	SDA = 1;
	SCL = 1;
	Delay10us();
	SDA = 0;
}

// TM Stop//
void TM_Stop(void)
{
	SDA = 0;
	SCL = 1;
	Delay10us();
	SDA = 1;
}

void TM_Ack(void)
{
	unsigned char timeout=1;
	SCL=1;
	Delay10us();
	SCL=0;
	while((SDA) && (timeout <= 100))
	{
		timeout++;
	}
	Delay10us();
	SCL = 0;
}

void Write_TM_Byte(unsigned char TM_Byte)
{
	unsigned char i;
	SCL=0;
	Delay2us();
	for(i=0;i<8;i++)
	{
		if(TM_Byte & 0x80)
		{
			SDA=1;
		}
		else
		{
			SDA=0;
		}
		SCL=0;
		Delay10us();
		SCL=1;
		Delay10us();
		SCL=0;
		TM_Byte<<=1;
	}
}

unsigned char Read_TM_Byte(void)
{
	unsigned char i,j,TM_Byte;
	for(i=0;i<8;i++)
	{
		SCL=0;
		Delay10us();
		TM_Byte<<=1;
		j=SDA;
		TM_Byte+=j;
		SCL=1;
		Delay10us();
	}
	SCL=0;
	Delay2us();
	return TM_Byte;
}
//д����
void TM_WrDat(unsigned char add,unsigned char dat)
{
	TM_Start();
	Write_TM_Byte(add);//�Դ��ַ
	TM_Ack();
	Write_TM_Byte(dat);//��ʾ����
	TM_Ack();
	TM_Stop();
}

//д����
void TM_WrCmd(unsigned char Bri)
{
	TM_Start();
	Write_TM_Byte(0x48);//��ʾģʽ
	TM_Ack();
	Write_TM_Byte(Bri);
	TM_Ack();
	TM_Stop();
}

void TM_Init(void)
{
	TM_WrCmd(0x01);//1������//8����ʾ//����ʾ
	TM_WrDat(0x68,0xff);//DIG1
	TM_WrDat(0x6a,0xff);//DIG2
	TM_WrDat(0x6c,0xff);//DIG3
	TM_WrDat(0x6e,0xff);//DIG4
}

void Dis_TM(unsigned char dig1,unsigned char dig2,unsigned char dig3,unsigned char dig4)
{
	TM_WrDat(0x68,dig1);//DIG1
	TM_WrDat(0x6a,dig2);//DIG2
	TM_WrDat(0x6c,dig3);//DIG3
	TM_WrDat(0x6e,dig4);//DIG4
}

void TM_ReCmd()
{
TM_Start();
Write_TM_Byte(0x49);//��ȡģʽ
TM_Ack();
key_data = Read_TM_Byte();//��ȡ����ɨ����
TM_Ack();
TM_Stop();
}

//void Read_TM()
//{
////�������ʾ����ɨ����
//unsigned char a = 0;
//unsigned char b = 0;
//unsigned char c = 0;

//TM_ReCmd();//TM1650������

//a = key_data/100;
//b = key_data/10%10;
//c = key_data%10;
//Dis_TM(dig1[0],dig1[a],dig1[b],dig2[c]); //�������ʾ����ɨ����
//}

void Read_TM()
{
//�������ʾ����λ��
unsigned char a = 0;
unsigned char b = 0;

TM_ReCmd();//TM1650������

switch(key_data)
{
//��һ��
case 0x44: a = 1; b = 1; break;
case 0x4c: a = 1; b = 2; break;
case 0x54: a = 1; b = 3; break;
case 0x5c: a = 1; b = 4; break;
case 0x64: a = 1; b = 5; break;
case 0x6c: a = 1; b = 6; break;
case 0x74: a = 1; b = 7; break;

//�ڶ���
case 0x45: a = 2; b = 1; break;
case 0x4d: a = 2; b = 2; break;
case 0x55: a = 2; b = 3; break;
case 0x5d: a = 2; b = 4; break;
case 0x65: a = 2; b = 5; break;
case 0x6d: a = 2; b = 6; break;
case 0x75: a = 2; b = 7; break;

//������
case 0x46: a = 3; b = 1; break;
case 0x4e: a = 3; b = 2; break;
case 0x56: a = 3; b = 3; break;
case 0x5e: a = 3; b = 4; break;
case 0x66: a = 3; b = 5; break;
case 0x6e: a = 3; b = 6; break;
case 0x76: a = 3; b = 7; break;

//������
case 0x47: a = 4; b = 1; break;
case 0x4f: a = 4; b = 2; break;
case 0x57: a = 4; b = 3; break;
case 0x5f: a = 4; b = 4; break;
case 0x67: a = 4; b = 5; break;
case 0x6f: a = 4; b = 6; break;
case 0x77: a = 4; b = 7; break;

default: a = 0; b = 0; break;
}

Dis_TM(0x00,dig1[a],dig1[10],dig1[b]); //�������ʾ����λ��
}