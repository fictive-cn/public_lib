//测试串口2，与WIFI模块链接   115200bps 22.1184MHZ
//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<Uart_2.h>
#include<IO.h>
#include "circle_queue.h"

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1

bit busy2=0;//COM2用到的发送忙标志

CircleQueue_t WIFI_Read_Buf;//WIFI接收缓冲区

unsigned char Mess_Count_WIFI=0;//接收的帧消息数目

unsigned char Reading_WIFI=0;//串口接收一个字节就自加1,用来中断里面断帧用
unsigned char Mess_Flag_WIFI=0;//开始断帧标志位

//串口2中断处理程序
void INT_Uart2(void) interrupt 8
{
 	if (S2CON & S2RI)
  {
    S2CON &= ~S2RI;         //清除S2RI位
		Reading_WIFI++;
		Mess_Flag_WIFI = 1;
		//SBUF = S2BUF;//测试用，转发WIFI收到的信息
		Queue_Put(&WIFI_Read_Buf,S2BUF);//数据进入队列
  }
   if (S2CON & S2TI)
   {
     S2CON &= ~S2TI;         //清除S2TI位
     busy2 = 0;               //清忙标志
   }
}
// 描述: 串口初始化程序.
void Init_Uart2(void)
{
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值 115200    22.1184MHZ
	AUXR |= 0x10;	//启动定时器2
	IE2 = 0x01;    //使能串口2中断
	
	Queue_Init(&WIFI_Read_Buf);
	
}
//COM2发送一个字节
void Send_Data2(unsigned char dat)
{
	busy2 = 1;
	S2BUF = dat;
	while(busy2);
	busy2 = 1;
}
//COM2发送一个字符串
void Send_Str2(unsigned char *s)
{
	do
		Send_Data2(*s++);
	while(*s);
}
//从接收缓冲区取出一个字节消息
unsigned char Get_Byte_WIFI(unsigned char *dat)
{
	return(Queue_Get(&WIFI_Read_Buf,dat));
}
//清空接收缓冲区的队列
void Clear_WIFI(void)
{
	Mess_Count_WIFI=0;
	Queue_Init(&WIFI_Read_Buf);
}
//返回
unsigned char Get_Mess_Count_WIFI(void)
{
	unsigned char i=0;
	i = Mess_Count_WIFI;
	if(Mess_Count_WIFI >= 1)
	{
		Mess_Count_WIFI--;
	}
	return i;
}
//返回
unsigned int Get_Byte_Count_WIFI(void)
{
	return (Queue_Length(&WIFI_Read_Buf));
}
//



