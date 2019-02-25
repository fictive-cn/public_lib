//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#ifndef __WIFI_H__
#define __WIFI_H__

#include<STC15W.h>

//初始化WIFI,包括设置SSID等参数
void Init_WIFI(void);
//发送CONNECT报文，并等待服务器返回代码
void MQTT_Connect(void);
//发送心跳 放到主循环中，定时器标志位到了就发送心跳。
void Send_Heart(void);
//接收MQTT服务器发送过来的数据，解析
void Analyze_MQTT_Read(void);
//定时上报温度数据
void Pub_Temperature(void);



#endif