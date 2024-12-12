#include "LD3320.h"

unsigned char nAsrStatus = 0;
unsigned char nLD_Mode = LD_MODE_IDLE;//������¼��ǰ���ڽ���ASRʶ�����ڲ���MP3
unsigned char ucRegVal;
unsigned char nAsrRes = 0;

unsigned char text_flag[10];
unsigned char exti_flag = 0;

unsigned char flag = 0;
unsigned char nAsrResCount = 0;
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}
void LD_WriteReg(unsigned char address, unsigned char dataout)		//SPIд����
{
    unsigned char i = 0;
    unsigned char command = 0x04;
    CS_L();
    DELAY_NOP();
    CLK_H();
    //write command
    for(i = 0; i < 8; i++) {
        if((command & 0x80) == 0x80)
            SDI_H();
        else
            SDI_L();

        DELAY_NOP();
        CLK_L();
        command = (command << 1);
        DELAY_NOP();
        CLK_H();
    }
    //write address
    for(i = 0; i < 8; i++) {
        if((address & 0x80) == 0x80)
            SDI_H();
        else
            SDI_L();
        DELAY_NOP();
        CLK_L();
        address = (address << 1);
        DELAY_NOP();
        CLK_H();
    }
    //write data
    for(i = 0; i < 8; i++) {
        if((dataout & 0x80) == 0x80)
            SDI_H();
        else
            SDI_L();
        DELAY_NOP();
        CLK_L();
        dataout = (dataout << 1);
        DELAY_NOP();
        CLK_H();
    }
    DELAY_NOP();
    CS_H();
}

unsigned char LD_ReadReg(unsigned char address)		//SPI������
{
    unsigned char i = 0;
    unsigned char datain = 0 ;
    unsigned char temp = 0;
    unsigned char command = 0x05;
    CS_L();
    DELAY_NOP();
    CLK_H();
    //write command
    for(i = 0; i < 8; i++) {
        if((command & 0x80) == 0x80)
            SDI_H();
        else
            SDI_L();
        DELAY_NOP();
        CLK_L();
        command = (command << 1);
        DELAY_NOP();
        CLK_H();
    }

    //write address
    for(i = 0; i < 8; i++) {
        if((address & 0x80) == 0x80)
            SDI_H();
        else
            SDI_L();
        DELAY_NOP();
        CLK_L();
        address = (address << 1);
        DELAY_NOP();
        CLK_H();
    }
    DELAY_NOP();

    //Read data
    for(i = 0; i < 8; i++) {
        datain = (datain << 1);
        temp = READ_SDO();
        DELAY_NOP();
        CLK_L();
        if(temp == 1)
            datain |= 0x01;
        DELAY_NOP();
        CLK_H();
    }

    DELAY_NOP();
    CS_H();
    return datain;
}

static void LD_Init_Common(void)
{
    LD_ReadReg(0x06);
    LD_WriteReg(0x17, 0x35);
    Delay10ms();
    LD_ReadReg(0x06);

    LD_WriteReg(0x89, 0x03);
    Delay10ms();
    LD_WriteReg(0xCF, 0x43);
    Delay10ms();
    LD_WriteReg(0xCB, 0x02);

    /*PLL setting*/
    LD_WriteReg(0x11, LD_PLL_11);
    if(nLD_Mode == LD_MODE_MP3) {
    } else {
        LD_WriteReg(0x1E, 0x00);
        LD_WriteReg(0x19, LD_PLL_ASR_19);
        LD_WriteReg(0x1B, LD_PLL_ASR_1B);
        LD_WriteReg(0x1D, LD_PLL_ASR_1D);
    }
    Delay10ms();

    LD_WriteReg(0xCD, 0x04);
    LD_WriteReg(0x17, 0x4c);
    Delay10ms();
    LD_WriteReg(0xB9, 0x00);
    LD_WriteReg(0xCF, 0x4F);
//	LD_WriteReg(0x6F, 0xFF);
}

static void LD_Init_ASR(void)
{
    nLD_Mode = LD_MODE_ASR_RUN;
    LD_Init_Common();

    LD_WriteReg(0xBD, 0x00);
    LD_WriteReg(0x17, 0x48);
    Delay10ms();
    LD_WriteReg(0x3C, 0x80);
    LD_WriteReg(0x3E, 0x07);
    LD_WriteReg(0x38, 0xff);
    LD_WriteReg(0x3A, 0x07);

    LD_WriteReg(0x40, 0);
    LD_WriteReg(0x42, 8);
    LD_WriteReg(0x44, 0);
    LD_WriteReg(0x46, 8);
    Delay10ms();
}


unsigned char LD_Check_ASRBusyFlag_b2(void)
{
    unsigned char j;
    unsigned char flag = 0;
    for(j = 0; j < 10; j++) {
        if(LD_ReadReg(0xb2) == 0x21) {
            flag = 1;
            break;
        }
        Delay10ms();
    }
    return flag;
}

static unsigned char LD_AsrAddFixed(void)
{
    unsigned char k, flag;
    unsigned char nAsrAddLength;
#define DATE_A 5    //�����ά��ֵ
#define DATE_B 20		//����һά��ֵ
    //��ӹؼ��ʣ��û��޸�
    unsigned char  sRecog[DATE_A][DATE_B] = {
        "kai deng", \
        "guan deng", \
        "an jian chu fa", \
        "quan mie", \
        "zhuang tai"\

    };
    unsigned char  pCode[DATE_A] = {
        CODE_KD,	\
        CODE_SS,	\
        CODE_AJCF, \
        CODE_QM,	\
        CODE_JT		\
    };	//���ʶ���룬�û��޸�
    flag = 1;
    for(k = 0; k < DATE_A; k++) {
        if(LD_Check_ASRBusyFlag_b2() == 0) {
            flag = 0;
            break;
        }

        LD_WriteReg(0xc1, pCode[k]);
        LD_WriteReg(0xc3, 0);
        LD_WriteReg(0x08, 0x04);
        Delay10ms();
        LD_WriteReg(0x08, 0x00);
        Delay10ms();

        for(nAsrAddLength = 0; nAsrAddLength < DATE_B; nAsrAddLength++) {
            if(sRecog[k][nAsrAddLength] == 0)
                break;
            LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
        }
        LD_WriteReg(0xb9, nAsrAddLength);
        LD_WriteReg(0xb2, 0xff);
        LD_WriteReg(0x37, 0x04);
    }
    return flag;
}

static void LD_reset(void)
{
    RST_H();
    Delay10ms();
    RST_L();
    Delay10ms();
    RST_H();
    Delay10ms();
    CS_L();
    Delay10ms();
    CS_H();
    Delay10ms();
}

static unsigned char LD_AsrRun(void)
{
    LD_WriteReg(0x35, MIC_VOL);
    LD_WriteReg(0x1C, 0x09);
    LD_WriteReg(0xBD, 0x20);
    LD_WriteReg(0x08, 0x01);
    Delay10ms();
    LD_WriteReg(0x08, 0x00);
    Delay10ms();

    if(LD_Check_ASRBusyFlag_b2() == 0) {
        return 0;
    }

    LD_WriteReg(0xB2, 0xff);
    LD_WriteReg(0x37, 0x06);
    LD_WriteReg(0x37, 0x06);
    Delay10ms();
//	text_flag[1]=LD_ReadReg(0xbf);
    LD_WriteReg(0x1C, 0x0b);
    LD_WriteReg(0x29, 0x10);
    LD_WriteReg(0xBD, 0x00);
    return 1;
}

static unsigned char RunASR(void)
{
    unsigned char i = 0;
    unsigned char asrflag = 0;
    for(i = 0; i < 5; i++) {	//��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
        LD_Init_ASR();			//��ʼ��ASR
        Delay10ms();
        if(LD_AsrAddFixed() == 0) {	//��ӹؼ����ﵽLD3320оƬ��
            LD_reset();				//LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            Delay10ms();	//���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }
//		text_flag[0]=LD_ReadReg(0xbf);
        Delay10ms();
        if(LD_AsrRun() == 0) {
            LD_reset();			 //LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
            Delay10ms();//���ӳ�ʼ����ʼ����ASRʶ������
            continue;
        }
        asrflag = 1;
        break;						//ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
    }
    return asrflag;
}

static unsigned char LD_GetResult(void)
{
    return LD_ReadReg(0xc5);
}
unsigned char text1_flag = 0;

void tiaoshi()
{
    LD_ReadReg(0x06);
    LD_WriteReg(0x35, 0x33);
    LD_WriteReg(0x1b, 0x55);
    LD_WriteReg(0xb3, 0xaa);
    text_flag[0] = LD_ReadReg(0x35);
    text_flag[1] = LD_ReadReg(0x1b);
    text_flag[2] = LD_ReadReg(0xb3);
}

unsigned char LD_Check_ASR(void)
{
    unsigned char j;
    unsigned char flag = 0;

    for(j = 0; j < 10; j++) {
        if(LD_ReadReg(0xb2) == 0x21) {
            flag = 1;
            break;
        }
        Delay10ms();
    }
    return flag;
}

void LD3320_main(void)
{
    LD_reset();
    nAsrStatus = LD_ASR_NONE;//��ʼ״̬��û������ASR
    Delay10ms();
//    tiaoshi();
//    text_flag[9] = LD_ReadReg(0xb2);
    while(1) {
				if(nAsrStatus != LD_ASR_NONE)  //��ѯ��ʽ
				{
					flag = LD_Check_ASR();
				}else
				{
					flag = 0;
				}
				if(flag == 1)  //��ʶ�����
				{
					nAsrResCount = LD_ReadReg(0xba);

					if(nAsrResCount > 0 && nAsrResCount <= 4) {
							nAsrStatus = LD_ASR_FOUNDOK;
					} else {
							nAsrStatus = LD_ASR_FOUNDZERO;
					}
				}
				
				
        switch(nAsrStatus) {
        case LD_ASR_RUNING:
        case LD_ASR_ERROR:
            break;
        case LD_ASR_NONE:
            nAsrStatus = LD_ASR_RUNING;
            if(RunASR() == 0) { //����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����
                nAsrStatus = LD_ASR_ERROR;
            }
            break;
        case LD_ASR_FOUNDOK:
            nAsrRes = LD_GetResult(); //һ��ASRʶ�����̽�����ȥȡASRʶ����

            switch(nAsrRes) {	   			//�Խ��ִ����ز���,�ͻ��޸�
            case CODE_KD://������ơ�
                LED1_ON();
//                printf("����\r\n");

                break;
            case CODE_SS:	//����صơ�
              
                LED1_OFF();
//                printf("�ص�\r\n");
                break;
            case CODE_AJCF:					//�������������
                break;
            case CODE_QM:						//���ȫ��
                break;
            case CODE_JT:						//���״̬��

            default:
                break;
            }

            nAsrStatus = LD_ASR_NONE;
            break;
        case LD_ASR_FOUNDZERO:
        default:
            nAsrStatus = LD_ASR_NONE;
            break;
        }
    }
}

void zhongduan()
{
    unsigned char nAsrResCount = 0;
    exti_flag += 1;
    
    ucRegVal = LD_ReadReg(0x2B);

// ����ʶ��������ж�
//�����������룬����ʶ��ɹ���ʧ�ܶ����жϣ�
    LD_WriteReg(0x29, 0) ;
    LD_WriteReg(0x02, 0) ;

    if((ucRegVal & 0x10) && LD_ReadReg(0xb2) == 0x21 && LD_ReadReg(0xbf) == 0x35) {
        nAsrResCount = LD_ReadReg(0xba);

        if(nAsrResCount > 0 && nAsrResCount <= 4) {
            nAsrStatus = LD_ASR_FOUNDOK;
        } else {
            nAsrStatus = LD_ASR_FOUNDZERO;
        }
    } else {
        nAsrStatus = LD_ASR_FOUNDZERO; //ִ��û��ʶ��
    }

    LD_WriteReg(0x2b, 0);
    LD_WriteReg(0x1C, 0); //д0:ADC������
    LD_WriteReg(0x29, 0);
    LD_WriteReg(0x02, 0);
    LD_WriteReg(0x2B, 0);
    LD_WriteReg(0xBA, 0);
    LD_WriteReg(0xBC, 0);
    LD_WriteReg(0x08, 1); //���FIFO_DATA
    LD_WriteReg(0x08, 0); //���FIFO_DATA�� �ٴ�д0
}
