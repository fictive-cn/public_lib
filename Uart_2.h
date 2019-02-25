//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
//测试串口2，与WIFI模块链接   115200bps 30MHZ
#ifndef __Uart_2_H__
#define __Uart_2_H__

#include<STC15W.h>
void Init_Uart2(void);// 描述: 串口初始化程序.
void Send_Data2(unsigned char dat);//COM1发送一个字节
void Send_Str2(unsigned char *s);//描述: COM1发送一个字符串

unsigned char Get_Byte_WIFI(unsigned char *dat);//从接收缓冲区取出一个字节消息
void Clear_WIFI(void);//清空接收缓冲区的队列
unsigned char Get_Mess_Count_WIFI(void);//返回CLI接收的帧数量
unsigned int Get_Byte_Count_WIFI(void);//返回串口1接收缓冲区数量


#endif