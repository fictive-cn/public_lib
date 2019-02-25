//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#ifndef __Uart_1_H__
#define __Uart_1_H__

#include<STC15W.h>

void Init_Uart1(void);// 描述: 串口初始化程序.
void Send_Data1(unsigned char dat);//COM1发送一个字节
void Send_Str1(unsigned char *s);//描述: COM1发送一个字符串
unsigned char Get_Byte_CLI(unsigned char *dat);//从接收缓冲区取出一个字节消息
void Clear_CLI(void);//清空接收缓冲区的队列
unsigned char Get_Mess_Count_CLI(void);//返回CLI接收的帧数量
unsigned int Get_Byte_Count_CLI(void);//返回串口1接收缓冲区数量
#endif