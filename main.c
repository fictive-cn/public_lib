//�ڲ�ʱ�� 22.1184MHZ
//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#include<STC15W.h>
#include<stdio.h>
#include<intrins.h>
#include<Uart_1.h> //����CLI����
#include<Uart_2.h> //����WIFI����
#include<Timer1.h> //��ʱ��
#include<ADC.h>  //������ȡ�¶�
#include<IO.h>  //��������LED����ȡ������
#include"IAP_EEPROM.h" //�ڲ�Flash��������
#include<CLI.h> //cli�����߼�
#include<WIFI.h>


void main(void)
{
	Init_IO();//��ʼ�����е�IO��
	Init_Uart1();//115200  ����CLI����
	Init_Uart2();//115200  ������WIFI����ͨ��
	Init_Timer1();//�򿪶�ʱ��1  10ms ��ʱ��
	Init_ADC();//��ʼ��ADC�ӿ�,������ȡ�¶ȴ���������ֵ
	EA = 1;//�򿪵�Ƭ��ȫ���ж�
	Delay1ms(50);
	Send_Str1("\r\n  Hello This is LingYao! \r\n");
	Send_Str1("\r\n  ALi-IOT LP Demo Based On 51-MCU. \r\n");
	Send_Str1("\r\n  CLI Starting... \r\n");
	Init_WIFI();//����WIFI��SSID,MQTT���� �Ȳ���
	MQTT_Connect();//����MQTT������
	while(1)
	{
		CLI();//ִ��CLI�߼�
		Send_Heart();//��ʱ����������
		Analyze_MQTT_Read();//�����������·�����Ϣ
		Pub_Temperature();//��ʱ�ϱ��¶���Ϣ
		
/******************************
//		Delay1ms(1000);
//		LED1 = ~LED1;
//		LED2 = ~LED2;
//		LED3 = ~LED3;
//		LED4 = ~LED4;
//		Relay = ~Relay;
/******************************/
		
	}
}
//



