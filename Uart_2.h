//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
//���Դ���2����WIFIģ������   115200bps 30MHZ
#ifndef __Uart_2_H__
#define __Uart_2_H__

#include<STC15W.h>
void Init_Uart2(void);// ����: ���ڳ�ʼ������.
void Send_Data2(unsigned char dat);//COM1����һ���ֽ�
void Send_Str2(unsigned char *s);//����: COM1����һ���ַ���

unsigned char Get_Byte_WIFI(unsigned char *dat);//�ӽ��ջ�����ȡ��һ���ֽ���Ϣ
void Clear_WIFI(void);//��ս��ջ������Ķ���
unsigned char Get_Mess_Count_WIFI(void);//����CLI���յ�֡����
unsigned int Get_Byte_Count_WIFI(void);//���ش���1���ջ���������


#endif