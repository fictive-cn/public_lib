//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#ifndef __WIFI_H__
#define __WIFI_H__

#include<STC15W.h>

//��ʼ��WIFI,��������SSID�Ȳ���
void Init_WIFI(void);
//����CONNECT���ģ����ȴ����������ش���
void MQTT_Connect(void);
//�������� �ŵ���ѭ���У���ʱ����־λ���˾ͷ���������
void Send_Heart(void);
//����MQTT���������͹��������ݣ�����
void Analyze_MQTT_Read(void);
//��ʱ�ϱ��¶�����
void Pub_Temperature(void);



#endif