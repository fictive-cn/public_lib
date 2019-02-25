//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<IO.h>

//初始化所有的IO口为普通IO
void Init_IO(void)
{
	P0M1=0;P0M0=0;P1M1=0;P1M0=0;
	P2M1=0;P2M0=0;P3M1=0;P3M0=0;
	P4M1=0;P4M0=0;P5M1=0;P5M0=0;//上电初始化所有IO口为普通IO
	
	LED1 = 0;LED2 = 0;LED3 = 0;
	LED4 = 0;Relay = 0;Key = 1;//按键端口作为只读的，需要首先写入1.
	
}
//延时函数  主函数中使用
void Delay1ms(unsigned int a)	// 1ms @30.000MHz
{
	unsigned char i, j;
	unsigned int b=0;
	for(b=0;b<a;b++)
	{
		i = 30;
		j = 43;
		do
		{
			while (--j);
		} while (--i);
	}
}
//




