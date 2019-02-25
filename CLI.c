//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<CLI.h>
#include<Uart_1.h>
#include<IO.h>
#include "IAP_EEPROM.h"

//从数据流中检测出的一帧信息
static unsigned char Read_CMD[256]; //一帧信息的缓冲区
static unsigned char CMD_Count=0;//计数器

//判断str1里面是否包含str2，返回位置(注意，长度不超过255)
unsigned char Compare_str(unsigned char *str1,unsigned char *str2)
{
	unsigned char *str2_copy;
	unsigned char str1_long=0,str2_long=0;
	str2_copy = str2;//保留str2的字符串地址
	do
	{
		if(str1[str1_long] == str2_copy[0])
		{
			str2_long = str1_long;
			do
			{
				if(str1[str2_long++] != *str2_copy++)//如果有一个字符串不匹配，就跳出这个循环
				{
					str2_long = 0;
					break;
				}
			}while(*str2_copy);
			
			if(str2_long != 0)
				return str1_long;//如果可以执行到这里，表示有匹配的字符串
		}
		str2_copy = str2;
	}while(str1[str1_long++]);
	
	return 255;//如果上述循环自动退出了，那么匹配失败
}
//检测数据流中是否有一帧信息   读取的CMD；数据流的缓冲区
static unsigned char Check_CMD(void)
{
	if(Get_Mess_Count_CLI() == 0)
		return 0;
	//执行到这里表示缓冲区有信息
	while(Get_Byte_CLI(&Read_CMD[CMD_Count++]))  //读到环形队列为空
	{
		if(CMD_Count >=254 )//表示超出了数据处理的范围了
		{
			CMD_Count = 0;
			Clear_CLI();
			Send_Str1("Error:CMD Too Long!\r\n");
			return 0;
		}
		if(Read_CMD[CMD_Count-1] == 0x0a)//如果是回车，表示一帧完整的信息完成  对方发送信息结尾是  0x0d 0x0a
		{
			Read_CMD[CMD_Count-2] = 0;//字符串结尾都是0
			Clear_CLI();//每次阅读后就清除一次接收缓冲区
			return 1;
		}
	}
	Clear_CLI();//每次阅读后就清除一次接收缓冲区
	return 1;
}
//写入某Key到Flash  从读取缓冲区匹配Key
static void Write_Key(unsigned char *Key,unsigned char *Dat,unsigned int Key_Addr)
{
	unsigned char Comp_res=0; 
	unsigned char Key_Len=0;
	unsigned int Dat_Len=0;
	unsigned char *p;
	
	Key_Len=0;
	Comp_res=0;
	Dat_Len=0;
	p = Key;    //计算key的长度 
	while(*p++)
		Key_Len++;
	
	Comp_res = Compare_str(Read_CMD,Key);  //对比接收缓冲区内是否有与key匹配的字符串
	if(Comp_res != 255)   //没有匹配则返回255，有则返回位置
	{
		Comp_res += Key_Len;
		Dat_Len = 0;
		do
		{
			Dat[Dat_Len++] = Read_CMD[Comp_res];
		}while(Read_CMD[Comp_res++]);
		Dat[Dat_Len] = 0;
		Save_Flash_Message(Key_Addr,Dat,Dat_Len);//存储进入Flash的长度带最后的  0  ,内容也带 0 
		
		Send_Str1("Write ");
		Send_Str1(Key);
		Send_Str1(" OK:");
		Send_Str1(Dat);
		Send_Str1("\r\n");
	}
}
//写入某Key到Flash 不过是上位机下发的二进制消息,除去KEY后的前两个字节是后续的长度
static void Write_Key_HEX(unsigned char *Key,unsigned char *Dat,unsigned int Key_Addr)
{
	unsigned char Comp_res=0; 
	unsigned char Key_Len=0;
	unsigned int Dat_Len=0;
	unsigned int i=0;
	unsigned char *p;
	
	Key_Len=0;
	Comp_res=0;
	Dat_Len=0;
	p = Key;
	while(*p++)
		Key_Len++;
	
	Comp_res = Compare_str(Read_CMD,Key);
	if(Comp_res != 255)
	{
		Comp_res += Key_Len; //计算出第一个有效字节的位置
		
		//提取出剩余字节的长度信息
		Dat_Len = Read_CMD[Comp_res++];
		Dat_Len <<= 8;
		Dat_Len += Read_CMD[Comp_res++];
		
		i=0;
		do
		{
			Dat[i++] = Read_CMD[Comp_res++];
		}while(Dat_Len --);
		Dat[i] = 0;
		Save_Flash_Message(Key_Addr,Dat,i);//存储进入Flash的长度带最后的  0  ,内容也带 0 
		
		Send_Str1("Write HEX ");
		Send_Str1(Key);
		Send_Str1(" OK!\r\n");
	}
}
//读取某KEY的命令  读取Flash,直接发送到CLI
static void Read_Key(unsigned char *Key,unsigned char *Dat,unsigned int Key_Addr)
{
	unsigned char Comp_res=0;
	unsigned int DataLen=0;
	
	Comp_res = Compare_str(Read_CMD,Key);
	if(Comp_res != 255)
	{
		Read_Flash_Message(Key_Addr,Dat,&DataLen);
		Send_Str1("{\"");
		
		do
		{
			if(*Key != '?')
				Send_Data1(*Key);
			Key++;
		}
		while(*Key);
		
		Send_Str1("\":");
		Send_Str1("\"");
		
		Send_Str1(Dat);
		
		Send_Str1("\"}");
		Send_Str1("\r\n");
	}
	
}
//读取某KEY的命令  读取Flash,直接发送到CLI 直接发送对应区域的二进制
static void Read_Key_HEX(unsigned char *Key,unsigned char *Dat,unsigned int Key_Addr)
{
	unsigned char Comp_res=0;
	unsigned int DataLen=0;
	unsigned int i=0;
	Comp_res = Compare_str(Read_CMD,Key);
	if(Comp_res != 255)
	{
		Read_Flash_Message(Key_Addr,Dat,&DataLen);
		for(i=0;i<DataLen;i++)
			Send_Data1(Dat[i]);
	}
}

//返回某个数字的ASCII码,用来格式化HEX数据
unsigned char HEX_TO_ASCII(unsigned char Num)
{
	if(Num == 0)
		return '0';
	else if(Num == 1)
		return '1';
	else if(Num == 2)
		return '2';
	else if(Num == 3)
		return '3';
	else if(Num == 4)
		return '4';
	else if(Num == 5)
		return '5';
	else if(Num == 6)
		return '6';
	else if(Num == 7)
		return '7';
	else if(Num == 8)
		return '8';
	else if(Num == 9)
		return '9';
	else if(Num == 10)
		return 'A';
	else if(Num == 11)
		return 'B';
	else if(Num == 12)
		return 'C';
	else if(Num == 13)
		return 'D';
	else if(Num == 14)
		return 'E';
	else if(Num == 15)
		return 'F';
	else
		return 'G';
}
//获取单片机唯一ID
void Get_MCU_ID(unsigned char *Buf)
{
	unsigned char idata *p;
	unsigned char i=0;
	p = 0XF1;
	for(i=0;i<14;)
	{
		Buf[i++] = HEX_TO_ASCII((*p)>>4);
		Buf[i++] = HEX_TO_ASCII((*p)&0X0F);
		p++;
	}
	Buf[i++] = 0;
}
//接收到了一帧CMD，开始处理。
unsigned char CLI(void)
{
	unsigned char i=0;
	unsigned char DataBuf[512];//提取出来的有效信息
	unsigned int DataLen=0;//提取出来的有效信息的长度
	unsigned char Comp_res=0;
	
	if(Check_CMD() == 0) //如果上位机下发数据有误，则返回
		return 0;
	//能执行到这一步，表示接收了一帧有效CMD
	//Send_Str1(Read_CMD);//调试用，看缓冲区的消息
	
	Write_Key("SSID:",DataBuf,SSID_Addr);
	Read_Key("SSID?",DataBuf,SSID_Addr);
	
	Write_Key("WIFI-PASSWORD:",DataBuf,WIFI_Pass_Addr);
	Read_Key("WIFI-PASSWORD?",DataBuf,WIFI_Pass_Addr);
	
	Write_Key("MQTT-URL:",DataBuf,MQTT_URL_Addr);
	Read_Key("MQTT-URL?",DataBuf,MQTT_URL_Addr);
	
	Write_Key_HEX("MQTT-CONNECT:",DataBuf,MQTT_Connect_Addr);
	Read_Key_HEX("MQTT-CONNECT?",DataBuf,MQTT_Connect_Addr);
	
	Write_Key_HEX("MQTT-SUB:",DataBuf,MQTT_Sub_Addr);
	Read_Key_HEX("MQTT-SUB?",DataBuf,MQTT_Sub_Addr);
	
	Write_Key_HEX("MQTT-UNSUB:",DataBuf,MQTT_UnSub_Addr);
	Read_Key_HEX("MQTT-UNSUB?",DataBuf,MQTT_UnSub_Addr);
	
	Write_Key_HEX("MQTT-POST:",DataBuf,MQTT_Post_Addr);
	Read_Key_HEX("MQTT-POST?",DataBuf,MQTT_Post_Addr);
	
	
	//下面表示读取全部参数
	Comp_res = Compare_str(Read_CMD,"READ-ALL");
	if(Comp_res != 255)
	{
		
		Read_Flash_Message(SSID_Addr,DataBuf,&DataLen);
		Send_Str1("\r\n");
		Send_Str1("{\"SSID\":");
		Send_Str1("\"");
		Send_Str1(DataBuf);
		Send_Str1("\"}");
		Send_Str1("\r\n");
		Delay1ms(500);
		
		Read_Flash_Message(WIFI_Pass_Addr,DataBuf,&DataLen);
		Send_Str1("\r\n");
		Send_Str1("{\"WIFI_Pass\":");
		Send_Str1("\"");
		Send_Str1(DataBuf);
		Send_Str1("\"}");
		Send_Str1("\r\n");
		Delay1ms(500);
		
		Read_Flash_Message(MQTT_URL_Addr,DataBuf,&DataLen);
		Send_Str1("\r\n");
		Send_Str1("{\"MQTT_URL\":");
		Send_Str1("\"");
		Send_Str1(DataBuf);
		Send_Str1("\"}");
		Send_Str1("\r\n");
		Delay1ms(500);
		
		Get_MCU_ID(DataBuf);//获取全球唯一ID
		Send_Str1("\r\n");
		Send_Str1("{\"MCUID\":");
		Send_Str1("\"");
		Send_Str1(DataBuf);
		Send_Str1("\"}");
		Send_Str1("\r\n");
		Delay1ms(500);
		
	}
	
	//下面表示心跳交互
	Comp_res = Compare_str(Read_CMD,"HEART");
	if(Comp_res != 255)
	{
		Send_Str1("\r\n");Send_Str1("{\"C51-HEART-C51\":1}\r\n");
	}
	
	//复位
	Comp_res = Compare_str(Read_CMD,"RESET:1");
	if(Comp_res != 255)
	{
		Send_Str1("\r\n");Send_Str1("C51 Reset !\r\n");
		IAP_CONTR = 0x60;
	}
	
	//下面表示获取单片机唯一ID
	Comp_res = Compare_str(Read_CMD,"MCU-ID");
	if(Comp_res != 255)
	{
		Get_MCU_ID(DataBuf);//获取全球唯一ID
		Send_Str1("\r\n");
		Send_Str1("{\"MCUID\":");
		Send_Str1("\"");
		Send_Str1(DataBuf);
		Send_Str1("\"}");
		Send_Str1("\r\n");
	}
	
	//清空CMD缓冲区
	for(i=0;i<255;i++)
		Read_CMD[i] = 0;
	CMD_Count = 0;
	return 1;
}
//
