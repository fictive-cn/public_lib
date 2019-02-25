//内部时钟 22.1184MHZ
//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<STC15W.h>
#include<stdio.h>
#include<intrins.h>
#include<Uart_1.h> //用来CLI交互
#include<Uart_2.h> //用来WIFI交互
#include<Timer1.h> //定时器
#include<ADC.h>  //用来读取温度
#include<IO.h>  //用来驱动LED，读取按键等
#include"IAP_EEPROM.h" //内部Flash保存内容
#include<CLI.h> //cli交互逻辑
#include<WIFI.h>


void main(void)
{
	Init_IO();//初始化所有的IO口
	Init_Uart1();//115200  用来CLI交互
	Init_Uart2();//115200  用来和WIFI进行通信
	Init_Timer1();//打开定时器1  10ms 定时器
	Init_ADC();//初始化ADC接口,用来读取温度传感器的数值
	EA = 1;//打开单片机全局中断
	Delay1ms(50);
	Send_Str1("\r\n  Hello This is LingYao! \r\n");
	Send_Str1("\r\n  ALi-IOT LP Demo Based On 51-MCU. \r\n");
	Send_Str1("\r\n  CLI Starting... \r\n");
	Init_WIFI();//设置WIFI的SSID,MQTT参数 等参数
	MQTT_Connect();//链接MQTT服务器
	while(1)
	{
		CLI();//执行CLI逻辑
		Send_Heart();//定时发送心跳包
		Analyze_MQTT_Read();//解析服务器下发的消息
		Pub_Temperature();//定时上报温度信息
		
/******************************
//		Delay1ms(1000);
//		LED1 = ~LED1;
//		LED2 = ~LED2;
//		LED3 = ~LED3;
//		LED4 = ~LED4;
//		Relay = ~Relay;
/******************************/
		
	}
}
//



