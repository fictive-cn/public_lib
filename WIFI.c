//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<WIFI.h>
#include<CLI.h>
#include<Uart_1.h> //用来CLI交互
#include<Uart_2.h> //用来WIFI交互
#include<Timer1.h> //定时器
#include<ADC.h>  //用来读取温度
#include<IO.h>  //用来驱动LED，读取按键等
#include"IAP_EEPROM.h" //内部Flash保存内容

#define	WIFI_Send_Byte	Send_Data2
#define	WIFI_Send_Str		Send_Str2


unsigned char MQTT_Heart[]={0xc0,0x00}; 
unsigned char MQTT_Heart_Reply[]={0xd0,0x00};
unsigned char MQTT_DisConnect[]={ 0xe0,0x00,};

unsigned int MQTT_Heart_Count=0;
unsigned int MQTT_PUB_Count=0;

//发送AT指令.这里会等待回复的OK,否则会重发
void Send_AT(unsigned char *Str)
{
	unsigned char Dat=0;
	unsigned char Count=0;
	unsigned char Loop_Count=0;
	unsigned char ReSend_Count=0;
	WIFI_Send_Str(Str);//先通过串口发送出去
	while(1)//等待回复
	{
		Delay1ms(50);
		Loop_Count++;
		if(Loop_Count >= 100)
		{
			ReSend_Count++;
			if(ReSend_Count < 3)
			{
				Loop_Count = 0;
				Send_Str1("\r\n重发指令: ");
				Send_Str1(Str);
				Send_Str1("\r\n");
				WIFI_Send_Str(Str);//重发一遍
			}
			else
			{
					Send_Str1("\r\n发送失败: ");
					Send_Str1(Str);
					Send_Str1("\r\n");
				return;//重发失败，退出
			}
		}
		if(Get_Byte_WIFI(&Dat))
		{
			if (Dat == 'O')
			{
				Delay1ms(20);
				Get_Byte_WIFI(&Dat);
				if(Dat == 'K')
				{
					Send_Str1("\r\n成功执行一条指令: ");
					Send_Str1(Str);
					Send_Str1("\r\n");
					return;
				}
			}
		}
	}	
}
//拼接两条字符串，把2添加到1的后面
void Sum_Str_Tail(unsigned char *str1,unsigned char *str2)
{
	unsigned int Len1=0;
	unsigned int Len2=0;
	unsigned int i=0;
	
	do
		Len1 ++;
	while(str1[Len1]);//计算str1的长度
	
	do
		Len2 ++;
	while(str2[Len2]);//计算str2的长度
	
	for(i=0;i<Len2;i++)
	{
		str1[Len1+i] = str2[i];
	}
	str1[Len1+i] = 0;
}
//拼接两条字符串，把2添加到1的前面
void Sum_Str_Header(unsigned char *str1,unsigned char *str2)
{
	unsigned int Len1=0;
	unsigned int Len2=0;
	unsigned int i=0;
	
	do
		Len1 ++;
	while(str1[Len1]);//计算str1的长度
	
	do
		Len2 ++;
	while(str2[Len2]);//计算str2的长度
	
	for(i=0;i<Len1;i++)//先把str1整体后移
		str1[Len2+Len1-i-1] = str1[Len1-i-1];
	str1[Len2+Len1] = 0;
	
	for(i=0;i<Len2;i++)
	{
		str1[i] = str2[i];
	}
}
//初始化WIFI,包括设置SSID等参数
void Init_WIFI(void)
{
	unsigned char DataBuf[256];
	unsigned char DataBuf2[30];//主要是给WIFI密码用的。所以限定了密码最大长度是30
	unsigned int DataLen=0;
	unsigned int i=0;
	
	//退出透传模式
	Send_AT("+++");
	
	Send_AT("AT\r\n");
	Send_AT("AT\r\n");
	//关闭回显
	Send_AT("AT+UARTE=OFF\r\n");
	
	//打开TCP的EVENT
	Send_AT("AT+WEVENT=ON\r\n");
	Send_AT("AT+CIPEVENT=ON\r\n");
	
	//首先关闭TCP1通道,用于调试使用
	Send_AT("AT+CIPSTOP=1\r\n");
	
	//查看Flash内容，WIFI的SSID部分，如果是FF或00，表示第一次开机
	DataLen = Read_One_Byte(SSID_Addr);
	DataLen <<= 8;
	DataLen += Read_One_Byte(SSID_Addr+1);
	if((DataLen == 0XFFFF)|(DataLen == 0))
	{
		Send_Str1("\r\n  设备第一次开机，请进行设置! \r\n");
		while(1)
		{
			CLI();//执行CLI逻辑
		}
	}
	//设置模块做TCP Client 的参数
	Read_Flash_Message(MQTT_URL_Addr,DataBuf,&DataLen);
	Sum_Str_Tail(DataBuf,",1883\r\n");//在后面加上
	Sum_Str_Header(DataBuf,"AT+CIPSTART=1,tcp_client,");//在前面加上
	Send_AT(DataBuf);//AT指令发射出去
	
	//链接WIFI到路由器   station模式
	Read_Flash_Message(SSID_Addr,DataBuf,&DataLen);
	Read_Flash_Message(WIFI_Pass_Addr,DataBuf2,&DataLen);
	Sum_Str_Header(DataBuf,"AT+WJAP=");
	Sum_Str_Tail(DataBuf,",");
	Sum_Str_Tail(DataBuf,DataBuf2);
	Sum_Str_Tail(DataBuf,"\r\n");
	Send_AT(DataBuf);
	Clear_WIFI();
	//等待TCP链接成功事件
	while(1)
	{
		Delay1ms(10);
		if(Get_Mess_Count_WIFI())
		{
			DataLen=0;
			while(Get_Byte_WIFI(&DataBuf[DataLen++]))
			{
				Send_Data1(DataBuf[DataLen-1]);
			}
			DataBuf[DataLen++] = 0;
			if(Compare_str(DataBuf,"SERVER,CONNECTED") != 255 )
			{
				
				//关闭EVENT通知
				Send_AT("AT+WEVENT=OFF\r\n");
				Send_AT("AT+CIPEVENT=OFF\r\n");
				//进入透传模式
				Send_AT("AT+CIPSENDRAW\r\n");
				
				Clear_WIFI();
				Send_Str1("\r\n TCP Connect OK! \r\n");
				LED2 = 1;
				
				return;
			}
		}
		CLI();//执行CLI逻辑
	}
}
//发送CONNECT报文，并等待服务器返回代码
void MQTT_Connect(void)
{
	unsigned char DataBuf[256];
	unsigned int DataLen=0;
	unsigned char Dat=0;
	unsigned int i=0;
	Delay1ms(2000);
	//先发送断开链接 防止上次链接还在线
	for(i=0;i<2;i++)
	{
		WIFI_Send_Byte(MQTT_DisConnect[i]);
	}
	Clear_WIFI();//清空WIFI接收缓冲区
	Delay1ms(2000);
	
	//链接MQTT服务器
	Read_Flash_Message(MQTT_Connect_Addr,DataBuf,&DataLen);//读取MQTT报文
	for(i=0;i<DataLen;i++)
	{
		WIFI_Send_Byte(DataBuf[i]);
	}
	//等待服务器返回
	Delay1ms(2000);
	Dat = 0;
	while(Get_Byte_WIFI(&DataBuf[Dat++]));//获取接收缓冲区数据
	if((DataBuf[0]==0X20)&(DataBuf[1]==0X02))
	{
		Send_Str1("\r\n  链接MQTT服务器成功! \r\n");
		LED3 = 1;
	}
	else
	{
		Send_Str1("\r\n  链接MQTT服务器失败! \r\n");
		for(i=0;i<DataLen;i++)
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
		Delay1ms(2000);
	}
	
	//订阅主题  服务器设置设备属性  用来接收服务器下发的消息
	Read_Flash_Message(MQTT_Sub_Addr,DataBuf,&DataLen);//读取MQTT报文
	for(i=0;i<DataLen;i++)
	{
		WIFI_Send_Byte(DataBuf[i]);
	}
	//等待服务器返回
	Delay1ms(2000);
	Dat=0;
	while(Get_Byte_WIFI(&DataBuf[Dat++]));//获取接收缓冲区数据
	if((DataBuf[0]==0X90)&(DataBuf[1]==0X03))
	{
		Send_Str1("\r\n  订阅属性成功！ \r\n");
	}
	else
	{
		Send_Str1("\r\n  订阅属性失败！ \r\n");
		for(i=0;i<DataLen;i++)
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
		Delay1ms(2000);
	}
}
//发送心跳 放到主循环中，定时器标志位到了就发送心跳。
void Send_Heart(void)
{
	unsigned char MQTT_Heart[]={0xc0,0x00}; 
	if(MQTT_Heart_Count > 3000)//10ms定时器里面会自动加1
	{
		MQTT_Heart_Count = 0;
		WIFI_Send_Byte(MQTT_Heart[0]);
		WIFI_Send_Byte(MQTT_Heart[1]);
		Send_Str1("\r\n  发送一个心跳包到阿里云IOT平台！ \r\n");
	}
}
//接收MQTT服务器发送过来的数据，解析
void Analyze_MQTT_Read(void)
{
	unsigned int Len=0,i=0;
	unsigned char Dat[256];
	
	if(Get_Mess_Count_WIFI() == 0)
		return;
	
	Len=0;
	while(Get_Byte_WIFI(&Dat[Len++]));//获取接收缓冲区数据
	Dat[Len] = 0;
	
	for(i=0;i<Len;i++)
	{
		if((Dat[i] == 0XD0)&(Dat[i+1]==0x00))
			Send_Str1("\r\n  收到MQTT服务器回复的心跳信号! \r\n");
		if(Dat[i] == 0)//把前面那些二进制传输时的0去掉
			Dat[i] = 1;
	}
	
	if(Compare_str(Dat,"\"WorkSwitch\":1") != 255) 
	{
		Relay = 1;
		Send_Str1("\r\n  平台下发命令：打开继电器! \r\n");
	}
	if(Compare_str(Dat,"\"WorkSwitch\":0") != 255) 
	{
		Relay = 0;
		Send_Str1("\r\n  平台下发命令：关闭继电器! \r\n");
	}
}
//定时上报温度数据
void Pub_Temperature(void)
{
	unsigned char DataBuf[256];
	unsigned int DataLen=0,i=0;
	unsigned char Temperature[7];//存放温度信息
	
	if(MQTT_PUB_Count > 1000)
	{
		MQTT_PUB_Count = 0;
		Get_Temp(Temperature);
		Send_Str1("\r\n 主动上报温度为：");
		Send_Str1(Temperature);
		Send_Str1(" 摄氏度\r\n");
		
		LED4 = ~LED4;
		
		Read_Flash_Message(MQTT_Post_Addr,DataBuf,&DataLen);//属性上报报文
		for(i=0;i<132;i++)//发送前132个字节的报文
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
		for(i=0;i<5;i++)//发送5个字节的温度
		{
			WIFI_Send_Byte(Temperature[i]);
		}
		for(i=137;i<DataLen;i++)//发送剩下的内容
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
	}
}
//



