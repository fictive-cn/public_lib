//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#ifndef __IO_H__
#define __IO_H__

#include<STC15W.h>


#define LED1  P27
#define LED2  P26
#define LED3  P12
#define LED4  P13
#define Relay	P16
#define Key		P37


//初始化所有的IO口为普通IO
void Init_IO(void);
//延时函数  主函数中使用
void Delay1ms(unsigned int a);	// 1ms 


#endif