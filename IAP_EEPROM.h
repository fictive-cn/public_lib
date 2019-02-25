//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
//使用单片机内部的Flash，模拟EEPROM使用
//STC15W4K48S4 单片机，可以使用IAP命令擦写内部Flash
//起始地址 0X0000 结束 0X27FF  其中，512Byte一个扇区
//一共20个扇区  一个扇区可以存储一组参数，这样就可以直接擦出整个扇区修改内容

#ifndef	__IAP_EEPROM__H_
#define	__IAP_EEPROM__H_
#include "STC15W.h"
#include "intrins.h"


#define	SSID_Addr							0X0000
#define	WIFI_Pass_Addr				0X0200
#define	MQTT_URL_Addr					0X0400

//下述四个参数是由软件发送的,而且有效内容是二进制,所以内容的前两个字节表示后续的长度
#define	MQTT_Connect_Addr			0x0600
#define	MQTT_Sub_Addr					0x0800
#define	MQTT_UnSub_Addr				0x0A00
#define	MQTT_Post_Addr				0X0C00




//保存内容到Flash
void Save_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int Len);
//从Flash读取内容
void Read_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int *Len);
//读取某地址一个字节
unsigned char Read_One_Byte(unsigned int addr);



#endif