#ifndef REG_RW_H
#define REG_RW_H


#define SDCK P22  //SPI ʱ���ź�
#define SDO  P23  //SPI �������
#define SDI  P24  //SPI ��������
#define SCS  P20  //оƬƬѡ�ź�
#define RSTB P21 	/*��λ�˿�*/


//��������
void LD_WriteReg( unsigned char address, unsigned char dataout );
unsigned char LD_ReadReg( unsigned char address );


#endif
