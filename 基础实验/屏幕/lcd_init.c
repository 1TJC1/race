#include "lcd_init.h"
#include "spi.h"
#include "intrins.h"

/******************************************************************************
      ����˵����ms����ʱ
      ������ݣ�x Ҫ��ʱ��ʱ��
      ����ֵ��  ��
******************************************************************************/
void Delayxms(unsigned int x)	//@11.0592MHz
{
	unsigned char data i, j;
	while(--x)
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
/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{
//    LCD_CS_Clr();
//		SPI_SendByte(dat);
//		SPI_SendData(&dat, 1);
//    HAL_SPI_Transmit(&userSPI,&dat,1,0xffff);
//  HAL_SPI_Transmit_DMA(&userSPI,&dat,1);
//    LCD_CS_Set();
	
			u8 i;
			LCD_CS_Clr();
			for(i=0;i<8;i++)
			{			  
				LCD_SCLK_Clr();
				if(dat&0x80)
				{
					 LCD_MOSI_Set();
				}
				else
				{
					 LCD_MOSI_Clr();
				}
				LCD_SCLK_Set();
				dat<<=1;
			}	
			LCD_CS_Set();	
}
/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
    LCD_DC_Set();//д����
    LCD_CS_Clr();
		SPI_SendByte(dat);
//		SPI_SendData(&dat, 1);
//    HAL_SPI_Transmit(&userSPI, &dat, 1, 0xffff);
//  HAL_SPI_Transmit_DMA(&userSPI, &dat, 1);
    LCD_CS_Set() ;

}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
//    u8 a[2];

//    a[0] = dat>>8;
//    a[1] = (u8)dat;

//    LCD_DC_Set();//д����
//    LCD_CS_Clr();
//		SPI_SendByte(a[0]);
//		SPI_SendByte(a[1]);
////		SPI_SendData(a, 2);
////    HAL_SPI_Transmit(&userSPI, a, 2, 0x1000);
////  HAL_SPI_Transmit_DMA(&userSPI, a, 2);
//    LCD_CS_Set() ;
	
			LCD_Writ_Bus(dat>>8);
			LCD_Writ_Bus(dat);
}

/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
    LCD_DC_Clr();//д����
    LCD_Writ_Bus(dat);
    LCD_DC_Set();//д����
}

/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
    if(USE_HORIZONTAL==0)
    {
        LCD_WR_REG(0x2a);//�е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//�е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//������д
    }
    else if(USE_HORIZONTAL==1)
    {
        LCD_WR_REG(0x2a);//�е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//�е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//������д
    }
    else if(USE_HORIZONTAL==2)
    {
        LCD_WR_REG(0x2a);//�е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//�е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//������д
    }
    else
    {
        LCD_WR_REG(0x2a);//�е�ַ����
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//�е�ַ����
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//������д
    }
}
 
void LCD_Init(void)
{
    LCD_RES_Clr();//��λ
    Delayxms(100);
    LCD_RES_Set();
    Delayxms(100);

    LCD_BLK_Set();//�򿪱���
	Delayxms(100);

    LCD_WR_REG(0x11);                                             //�޴�ָ�����������ʼ��оƬ������ʾ
    Delayxms(120);
    LCD_WR_REG(0x36);                                             //�����ڴ�ɨ�跽��0X00����ɨ�裬�������£��������ң�RGB��ʽ
    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
    else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
    else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
    else LCD_WR_DATA8(0xA0);

    LCD_WR_REG(0x3A);                                             //���ݸ�ʽ��65Kɫ,565
    LCD_WR_DATA8(0x05);

    LCD_WR_REG(0xB2);                                             //֡Ƶ����
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);

    LCD_WR_REG(0xB7);                                             //GATE ����
    LCD_WR_DATA8(0x35);

    LCD_WR_REG(0xBB);                                             //VCOM����
    LCD_WR_DATA8(0x19);

    LCD_WR_REG(0xC0);                                             //LCM����,Ĭ��0x2c
    LCD_WR_DATA8(0x2C);

    LCD_WR_REG(0xC2);                                             //VDV&VRH SET ,Ĭ��0x01
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xC3);                                             //VRHS SET��Ĭ��0x0b
    LCD_WR_DATA8(0x12);                                           //�˴�����ʵ���������

    LCD_WR_REG(0xC4);                                             //VDV SET��Ĭ��0x20
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC6);                                             // FR SET, Ĭ��0x0F
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0xD0);                                             //��Դ����1
    LCD_WR_DATA8(0xA4);                                           //�ò�������
    LCD_WR_DATA8(0xA1);                                           //�˴�����ʵ������޸�

    LCD_WR_REG(0xE0);                                             //������GAMMA����
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x54);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x0B);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0xE1);                                              //������GAMMA����
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x44);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x23);

    LCD_WR_REG(0x21);                                             //���Կ���Ĭ����0X20������ģʽ

    LCD_WR_REG(0x11);

    LCD_WR_REG(0x29);                                             //��ʾ�����ȴ�MCU������
}
