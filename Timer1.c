//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#include<Timer1.h>
#include<Uart_1.h>
#include<Uart_2.h>
#include<IO.h>  //��������LED����ȡ������
//10ms��ʱ��,������֡.���������������ݳ���10msû��,��Ϊ��һ֡��Ϣ


extern unsigned char Reading_CLI;//���ڽ���һ���ֽھ��Լ�1,�����ж������֡��
unsigned char Reading_CLI2=0;//�ж������֡��
extern unsigned char Mess_Count_CLI;//���յ�֡��Ϣ��Ŀ
extern unsigned char Mess_Flag_CLI;//��ʼ��֡��־λ

extern unsigned char Reading_WIFI;//���ڽ���һ���ֽھ��Լ�1,�����ж������֡��
unsigned char Reading_WIFI2=0;//�ж������֡��
extern unsigned char Mess_Count_WIFI;//���յ�֡��Ϣ��Ŀ
extern unsigned char Mess_Flag_WIFI;//��ʼ��֡��־λ

extern unsigned int MQTT_Heart_Count;//MQTT������ʱ��
extern unsigned int MQTT_PUB_Count;//��ʱ�ϱ��¶���

unsigned char Led_Counter=0;//����LED��

void Timer1_Isr() interrupt 3 using 1
{
	if(Mess_Flag_CLI)
	{
		if(Reading_CLI2 != Reading_CLI) //���ڽ����ж��б仯
		{
			Reading_CLI2 = Reading_CLI;
		}
		else //��ʾ������10msû���յ�������Ϣ��,��Ϊһ֡��Ϣ����
		{
			Mess_Count_CLI++;
			Mess_Flag_CLI=0;
		}
	}
	if(Mess_Flag_WIFI)
	{
		if(Reading_WIFI2 != Reading_WIFI) //���ڽ����ж��б仯
		{
			Reading_WIFI2 = Reading_WIFI;
		}
		else //��ʾ������10msû���յ�������Ϣ��,��Ϊһ֡��Ϣ����
		{
			Mess_Count_WIFI++;
			Mess_Flag_WIFI=0;
		}
	}
	
	Led_Counter++;
	if(Led_Counter > 20)
	{
		Led_Counter=0;
		LED1 = ~LED1;
	}
	
	MQTT_Heart_Count++;//MQTT������������ʱ
	
	MQTT_PUB_Count++;//MQTT�ϱ��¶��ö�ʱ��
	
}
//��ʼ����ʱ��1,10ms����һ���ж�
void Init_Timer1(void)
{
	AUXR &= 0xBF;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0x00;		//���ö�ʱ��ֵ
	TH1 = 0xB8;		//���ö�ʱ��ֵ
	TF1 = 0;		//���TF1��־
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	ET1 = 1;   //ʹ�ܶ�ʱ��1�ж�
}
//




//

