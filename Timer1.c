//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<Timer1.h>
#include<Uart_1.h>
#include<Uart_2.h>
#include<IO.h>  //用来驱动LED，读取按键等
//10ms定时器,用来断帧.串口连续接收数据超过10ms没有,认为是一帧消息


extern unsigned char Reading_CLI;//串口接收一个字节就自加1,用来中断里面断帧用
unsigned char Reading_CLI2=0;//中断里面断帧用
extern unsigned char Mess_Count_CLI;//接收的帧消息数目
extern unsigned char Mess_Flag_CLI;//开始断帧标志位

extern unsigned char Reading_WIFI;//串口接收一个字节就自加1,用来中断里面断帧用
unsigned char Reading_WIFI2=0;//中断里面断帧用
extern unsigned char Mess_Count_WIFI;//接收的帧消息数目
extern unsigned char Mess_Flag_WIFI;//开始断帧标志位

extern unsigned int MQTT_Heart_Count;//MQTT心跳定时用
extern unsigned int MQTT_PUB_Count;//定时上报温度用

unsigned char Led_Counter=0;//点亮LED用

void Timer1_Isr() interrupt 3 using 1
{
	if(Mess_Flag_CLI)
	{
		if(Reading_CLI2 != Reading_CLI) //串口接收中断有变化
		{
			Reading_CLI2 = Reading_CLI;
		}
		else //表示至少有10ms没有收到串口信息了,认为一帧信息到来
		{
			Mess_Count_CLI++;
			Mess_Flag_CLI=0;
		}
	}
	if(Mess_Flag_WIFI)
	{
		if(Reading_WIFI2 != Reading_WIFI) //串口接收中断有变化
		{
			Reading_WIFI2 = Reading_WIFI;
		}
		else //表示至少有10ms没有收到串口信息了,认为一帧信息到来
		{
			Mess_Count_WIFI++;
			Mess_Flag_WIFI=0;
		}
	}
	
	Led_Counter++;
	if(Led_Counter > 20)
	{
		Led_Counter=0;
		LED1 = ~LED1;
	}
	
	MQTT_Heart_Count++;//MQTT发送心跳包定时
	
	MQTT_PUB_Count++;//MQTT上报温度用定时器
	
}
//初始化定时器1,10ms进入一次中断
void Init_Timer1(void)
{
	AUXR &= 0xBF;		//定时器时钟12T模式
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x00;		//设置定时初值
	TH1 = 0xB8;		//设置定时初值
	TF1 = 0;		//清除TF1标志
	TR1 = 1;		//定时器1开始计时
	ET1 = 1;   //使能定时器1中断
}
//




//

