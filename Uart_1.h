//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#ifndef __Uart_1_H__
#define __Uart_1_H__

#include<STC15W.h>

void Init_Uart1(void);// ����: ���ڳ�ʼ������.
void Send_Data1(unsigned char dat);//COM1����һ���ֽ�
void Send_Str1(unsigned char *s);//����: COM1����һ���ַ���
unsigned char Get_Byte_CLI(unsigned char *dat);//�ӽ��ջ�����ȡ��һ���ֽ���Ϣ
void Clear_CLI(void);//��ս��ջ������Ķ���
unsigned char Get_Mess_Count_CLI(void);//����CLI���յ�֡����
unsigned int Get_Byte_Count_CLI(void);//���ش���1���ջ���������
#endif