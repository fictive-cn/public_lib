//测试串口0，与USB转串口直接连接   115200/22.1184MHZ
//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<Uart_1.h>
#include "circle_queue.h"
bit busy1=0;//COM1用到的发送忙标志

CircleQueue_t CLI_Read_Buf;//CLI接收缓冲区

unsigned char Mess_Count_CLI=0;//接收的帧消息数目

unsigned char Reading_CLI=0;//串口接收一个字节就自加1,用来中断里面断帧用
unsigned char Mess_Flag_CLI=0;//开始断帧标志位

//串口1中断处理程序
void INT_Uart1(void) interrupt 4
{
	if(RI)//清空读中断标志位
	{
		RI = 0;
		Reading_CLI++;
		Mess_Flag_CLI = 1;
		//S2BUF = SBUF;//把接收到的数据直接发送出去
		if(SBUF == 0XFA)//收到 0XFA ，单片机复位到烧写区
			IAP_CONTR = 0x60;
		Queue_Put(&CLI_Read_Buf,SBUF);//数据进入队列
	}
	if(TI)//清空发送中断标志位
	{
		TI = 0;
		busy1 = 0;
	}
}
// 描述: 串口初始化程序.
void Init_Uart1(void)
{
	SCON = 0x50;//
	AUXR |= 0x01;//定时器2，1T
	AUXR |= 0x04;
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值  115200    22.1184MHZ
	AUXR |= 0x10;		//启动定时器2
	ES = 1;//允许COM1串行口中断
	TI = 1;
	//PS = 1;//COM1最高优先级中断		
	
	Queue_Init(&CLI_Read_Buf);
	
}
//COM1发送一个字节
void Send_Data1(unsigned char dat)
{
	busy1 = 1;
	SBUF = dat;
	while(busy1);
	busy1 = 1;
	
}
//COM1发送一个字符串
void Send_Str1(unsigned char *s)
{
	do
		Send_Data1(*s++);
	while(*s);
}
//从接收缓冲区取出一个字节消息
unsigned char Get_Byte_CLI(unsigned char *dat)
{
	return(Queue_Get(&CLI_Read_Buf,dat));
}
//清空接收缓冲区的队列
void Clear_CLI(void)
{
	Mess_Count_CLI = 0;
	Queue_Init(&CLI_Read_Buf);
}
//返回CLI接收的帧数量
unsigned char Get_Mess_Count_CLI(void)
{
	unsigned char i=0;
	i = Mess_Count_CLI;
	if(Mess_Count_CLI >= 1)
	{
		Mess_Count_CLI--;
	}
	return i;
}
//返回串口1接收缓冲区数量
unsigned int Get_Byte_Count_CLI(void)
{
	return (Queue_Length(&CLI_Read_Buf));
}
//












