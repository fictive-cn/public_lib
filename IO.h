//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#ifndef __IO_H__
#define __IO_H__

#include<STC15W.h>


#define LED1  P27
#define LED2  P26
#define LED3  P12
#define LED4  P13
#define Relay	P16
#define Key		P37


//��ʼ�����е�IO��Ϊ��ͨIO
void Init_IO(void);
//��ʱ����  ��������ʹ��
void Delay1ms(unsigned int a);	// 1ms 


#endif