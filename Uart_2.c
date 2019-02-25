//���Դ���2����WIFIģ������   115200bps 22.1184MHZ
//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#include<Uart_2.h>
#include<IO.h>
#include "circle_queue.h"

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1

bit busy2=0;//COM2�õ��ķ���æ��־

CircleQueue_t WIFI_Read_Buf;//WIFI���ջ�����

unsigned char Mess_Count_WIFI=0;//���յ�֡��Ϣ��Ŀ

unsigned char Reading_WIFI=0;//���ڽ���һ���ֽھ��Լ�1,�����ж������֡��
unsigned char Mess_Flag_WIFI=0;//��ʼ��֡��־λ

//����2�жϴ������
void INT_Uart2(void) interrupt 8
{
 	if (S2CON & S2RI)
  {
    S2CON &= ~S2RI;         //���S2RIλ
		Reading_WIFI++;
		Mess_Flag_WIFI = 1;
		//SBUF = S2BUF;//�����ã�ת��WIFI�յ�����Ϣ
		Queue_Put(&WIFI_Read_Buf,S2BUF);//���ݽ������
  }
   if (S2CON & S2TI)
   {
     S2CON &= ~S2TI;         //���S2TIλ
     busy2 = 0;               //��æ��־
   }
}
// ����: ���ڳ�ʼ������.
void Init_Uart2(void)
{
	S2CON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ 115200    22.1184MHZ
	AUXR |= 0x10;	//������ʱ��2
	IE2 = 0x01;    //ʹ�ܴ���2�ж�
	
	Queue_Init(&WIFI_Read_Buf);
	
}
//COM2����һ���ֽ�
void Send_Data2(unsigned char dat)
{
	busy2 = 1;
	S2BUF = dat;
	while(busy2);
	busy2 = 1;
}
//COM2����һ���ַ���
void Send_Str2(unsigned char *s)
{
	do
		Send_Data2(*s++);
	while(*s);
}
//�ӽ��ջ�����ȡ��һ���ֽ���Ϣ
unsigned char Get_Byte_WIFI(unsigned char *dat)
{
	return(Queue_Get(&WIFI_Read_Buf,dat));
}
//��ս��ջ������Ķ���
void Clear_WIFI(void)
{
	Mess_Count_WIFI=0;
	Queue_Init(&WIFI_Read_Buf);
}
//����
unsigned char Get_Mess_Count_WIFI(void)
{
	unsigned char i=0;
	i = Mess_Count_WIFI;
	if(Mess_Count_WIFI >= 1)
	{
		Mess_Count_WIFI--;
	}
	return i;
}
//����
unsigned int Get_Byte_Count_WIFI(void)
{
	return (Queue_Length(&WIFI_Read_Buf));
}
//



