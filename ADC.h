//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#ifndef __ADC_H__
#define __ADC_H__

#include<STC15W.h>

#define	A0 5
#define	A1 4
#define	A2 3
#define	A3 2
#define	A4 1
#define	A5 0

//初始化ADC功能
void	Init_ADC(void);
//获取某通道ADC的电压值，单位mV
unsigned int	Get_Vol(unsigned char ch);
//进行一次ADC转换
//void	Get_ADC(unsigned char ch);

void Get_Temp(unsigned char *Val);


#endif