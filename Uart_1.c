//���Դ���0����USBת����ֱ������   115200/22.1184MHZ
//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#include<Uart_1.h>
#include "circle_queue.h"
bit busy1=0;//COM1�õ��ķ���æ��־

CircleQueue_t CLI_Read_Buf;//CLI���ջ�����

unsigned char Mess_Count_CLI=0;//���յ�֡��Ϣ��Ŀ

unsigned char Reading_CLI=0;//���ڽ���һ���ֽھ��Լ�1,�����ж������֡��
unsigned char Mess_Flag_CLI=0;//��ʼ��֡��־λ

//����1�жϴ������
void INT_Uart1(void) interrupt 4
{
	if(RI)//��ն��жϱ�־λ
	{
		RI = 0;
		Reading_CLI++;
		Mess_Flag_CLI = 1;
		//S2BUF = SBUF;//�ѽ��յ�������ֱ�ӷ��ͳ�ȥ
		if(SBUF == 0XFA)//�յ� 0XFA ����Ƭ����λ����д��
			IAP_CONTR = 0x60;
		Queue_Put(&CLI_Read_Buf,SBUF);//���ݽ������
	}
	if(TI)//��շ����жϱ�־λ
	{
		TI = 0;
		busy1 = 0;
	}
}
// ����: ���ڳ�ʼ������.
void Init_Uart1(void)
{
	SCON = 0x50;//
	AUXR |= 0x01;//��ʱ��2��1T
	AUXR |= 0x04;
	T2L = 0xD0;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ  115200    22.1184MHZ
	AUXR |= 0x10;		//������ʱ��2
	ES = 1;//����COM1���п��ж�
	TI = 1;
	//PS = 1;//COM1������ȼ��ж�		
	
	Queue_Init(&CLI_Read_Buf);
	
}
//COM1����һ���ֽ�
void Send_Data1(unsigned char dat)
{
	busy1 = 1;
	SBUF = dat;
	while(busy1);
	busy1 = 1;
	
}
//COM1����һ���ַ���
void Send_Str1(unsigned char *s)
{
	do
		Send_Data1(*s++);
	while(*s);
}
//�ӽ��ջ�����ȡ��һ���ֽ���Ϣ
unsigned char Get_Byte_CLI(unsigned char *dat)
{
	return(Queue_Get(&CLI_Read_Buf,dat));
}
//��ս��ջ������Ķ���
void Clear_CLI(void)
{
	Mess_Count_CLI = 0;
	Queue_Init(&CLI_Read_Buf);
}
//����CLI���յ�֡����
unsigned char Get_Mess_Count_CLI(void)
{
	unsigned char i=0;
	i = Mess_Count_CLI;
	if(Mess_Count_CLI >= 1)
	{
		Mess_Count_CLI--;
	}
	return i;
}
//���ش���1���ջ���������
unsigned int Get_Byte_Count_CLI(void)
{
	return (Queue_Length(&CLI_Read_Buf));
}
//












