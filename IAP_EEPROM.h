//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
//ʹ�õ�Ƭ���ڲ���Flash��ģ��EEPROMʹ��
//STC15W4K48S4 ��Ƭ��������ʹ��IAP�����д�ڲ�Flash
//��ʼ��ַ 0X0000 ���� 0X27FF  ���У�512Byteһ������
//һ��20������  һ���������Դ洢һ������������Ϳ���ֱ�Ӳ������������޸�����

#ifndef	__IAP_EEPROM__H_
#define	__IAP_EEPROM__H_
#include "STC15W.h"
#include "intrins.h"


#define	SSID_Addr							0X0000
#define	WIFI_Pass_Addr				0X0200
#define	MQTT_URL_Addr					0X0400

//�����ĸ���������������͵�,������Ч�����Ƕ�����,�������ݵ�ǰ�����ֽڱ�ʾ�����ĳ���
#define	MQTT_Connect_Addr			0x0600
#define	MQTT_Sub_Addr					0x0800
#define	MQTT_UnSub_Addr				0x0A00
#define	MQTT_Post_Addr				0X0C00




//�������ݵ�Flash
void Save_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int Len);
//��Flash��ȡ����
void Read_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int *Len);
//��ȡĳ��ַһ���ֽ�
unsigned char Read_One_Byte(unsigned int addr);



#endif