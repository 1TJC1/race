/************************************************
*����ģ�� ->С���ɿ�����		����˵��
*		IQ	 ->		PB15		�жϽ� MUC����
*		RST  ->		PB14		��λ�� MUC���
*		CS	 ->		PB13		Ƭѡ�� MUC���
*		SDI  ->		PA8	���ݽ� MCU���
*		SDO	 ->		PC9		���ݽ� MUC���
*		CLK	 ->		PC8		ʱ�ӽ� MUC���
*************************************************/


#include "string.h"
#include "stdio.h"
#include "main.h"


///��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE        0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3         0x40

///�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE         0x00	//��ʾû������ASRʶ��
#define LD_ASR_RUNING       0x01	//��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK      0x10	//��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO    0x11	//��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR        0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬

#define CLK_IN          22/* user need modify this value according to clock in */
#define LD_PLL_11       (unsigned char)((CLK_IN/2.0)-1)

#define LD_PLL_ASR_19 		(unsigned char)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

#define MIC_VOL 0x43

///ʶ���루�ͻ��޸Ĵ���
#define CODE_KD	1	 //����
#define CODE_SS	2	 //�ص�
#define CODE_AJCF	3	 //��������
#define CODE_QM	4	 //ȫ��
#define CODE_JT		5  //״̬

#define CS_H() P20 = 1
#define CS_L() P20 = 0

#define RST_H() P21 = 1
#define RST_L() P21 = 0

#define CLK_H() P22 = 1
#define CLK_L() P22 = 0

#define SDI_H() P24 = 1
#define SDI_L() P24 = 0

#define READ_SDO() P23

#define LED1_OFF()  LED1 = 1
#define LED1_ON()   LED1 = 0

#define LED2_OFF()  LED2 = 1
#define LED2_ON()   LED2 = 0

#define LED3_OFF()  LED3 = 1
#define LED3_ON()   LED3 = 0

#define DELAY_NOP()	_nop_()

void LD_WriteReg(unsigned char address, unsigned char dataout);		//SPIд����
static void LD_Init_Common(void);
static void LD_Init_ASR(void);
unsigned char LD_Check_ASRBusyFlag_b2(void);
static unsigned char LD_AsrAddFixed(void);
static void LD_reset(void);
static unsigned char LD_AsrRun(void);
static unsigned char RunASR(void);
static unsigned char LD_GetResult(void);
void LD3320_main(void);
void zhongduan(void);

//extern void zhongduan();
