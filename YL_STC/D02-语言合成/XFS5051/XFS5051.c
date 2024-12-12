#include "XFS5051.h"
#include "string.h"

unsigned char Frame_Info[50]; //������ı�����

extern void SendDatas(unsigned char *s,unsigned char len);//���ڷ���ָ��
/***********************************************************
* ��    �ƣ�  YS-XFS5051 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���XFS5051оƬ���кϳɲ���
* ��ڲ�����  *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo���������Ƶ��ӡ�����
**********************************************************/
void XFS_FrameInfo(unsigned char *HZdata,unsigned char len)
{
/****************��Ҫ���͵��ı�**********************************/ 
//	uint8_t Frame_Info[50]; //������ı�����
	unsigned int  HZ_Length;  
	HZ_Length =len; 			//��Ҫ�����ı��ĳ���
 		 
/*****************֡�̶�������Ϣ**************************************/           
	Frame_Info[0] = 0xFD ; 			//����֡ͷFD
	Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
	Frame_Info[2] = HZ_Length+2; 		//�������������ȵĵ��ֽ�
	Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
	Frame_Info[4] = 0x01;       //�ı������ʽ��GBK 
 
/*******************����֡��Ϣ***************************************/		  
	memcpy(Frame_Info+5, HZdata, HZ_Length);
//		 PrintCom(Frame_Info,5+HZ_Length); //����֡����
	SendDatas(Frame_Info,5+HZ_Length);

}

/**************оƬ��������*********************/
unsigned char XFS_StopCom[]={0xFD,0X00,0X01,0X02};//ֹͣ�ϳ�
unsigned char XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//��ͣ�ϳ�
unsigned char XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//�ָ��ϳ�
unsigned char XFS_ChackCom[]={0XFD,0X00,0X01,0X21};//״̬��ѯ
unsigned char XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//����POWER DOWN ״̬����
unsigned char XFS_Sound[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x34,0x5D};
unsigned char XFS_man[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x33,0x5D };
////����1֡���ֽ��� 9
//-> 0xFD 0x00 0x06 0x01 0x01 0x5B 0x76 0x34 0x5D 
//����1֡���ֽ��� 9
//-> 0xFD 0x00 0x06 0x01 0x01 0x5B 0x76 0x33 0x5D 
/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ���� 
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��					 
* ���÷�����ͨ�������Ѿ�������������������á� 
**********************************************************/
void YS_XFS_Set(unsigned char *Info_data,unsigned char len)
{
	unsigned char Com_Len;
	Com_Len =len;
	SendDatas(Info_data,Com_Len);
}


/**********************************************************/
void YS_Sound_Set(unsigned char n)
{
	unsigned char Com_Len;
	Com_Len =9;
	XFS_Sound[7]=n+0x30;
	SendDatas(XFS_Sound,Com_Len);
}
/**********************************************************/
void YS_MANSound_Set(void)
{
	SendDatas(XFS_man,sizeof(XFS_man));
}
