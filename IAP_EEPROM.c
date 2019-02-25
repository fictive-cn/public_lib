#include "IAP_EEPROM.h"
#include<Uart_1.h> //用来CLI交互
//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
//STC15W4K48S4 单片机，可以使用IAP命令擦写内部Flash
//起始地址 0X0000 结束 0X27FF  其中，512Byte一个扇区

//关闭IAP功能
void Close_IAP(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0;
	IAP_ADDRL = 0;
}
//擦除某地址所在的扇区
void Erase_IAP(unsigned int addr)
{
		IAP_CONTR = 0x82;//允许IAP操作，设置IAP操作速度         
    IAP_CMD = 0x03;//扇区擦除命令            
    IAP_ADDRL = addr;              
    IAP_ADDRH = addr >> 8;//地址赋值          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;//触发命令               
    _nop_();_nop_();_nop_();            
    Close_IAP();//清空IAP操作寄存器等
}
//在某地址写入一个字节
void Write_One_Byte(unsigned int addr,unsigned char dat)
{
		IAP_CONTR = 0x82;//允许IAP操作，设置IAP操作速度   
    IAP_CMD = 0x02;//IAP写命令
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;//地址赋值
    IAP_DATA = dat;//更新写寄存器
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;//触发
    _nop_();
    Close_IAP();//清空IAP操作寄存器等
}
//读取某地址一个字节
unsigned char Read_One_Byte(unsigned int addr)
{
		unsigned char dat=0;//定义一个待返回的数据
    IAP_CONTR = 0x82;//允许IAP
    IAP_CMD = 0x01;//IAP读命令
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;//地址赋值
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;//触发
    _nop_();_nop_();_nop_();_nop_();
    dat = IAP_DATA;
		Close_IAP();//清空IAP操作寄存器等
    return dat;
}
//保存内容到Flash
void Save_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int Len)
{
	unsigned int i=0;
	Erase_IAP(Addr);//擦除对应扇区
	
	if(Len >= 511)
		Len = 511;
	
	Write_One_Byte(Addr,Len>>8);//长度写入首地址
	Write_One_Byte(Addr+1,(Len&0X00FF));//长度写入首地址
	for(i=0;i<Len;i++) //顺序存储有效信息
	{
		Write_One_Byte((Addr+2+i),Buf[i]);
	}
}
//从Flash读取内容
void Read_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int *Len)
{
	unsigned int i=0;
	*Len = Read_One_Byte(Addr); //首先读取长度信息
	*Len <<= 8;
	*Len += Read_One_Byte(Addr+1);
	
	if(*Len >= 511)
		*Len = 511;
	
	for(i=0;i<*Len;i++)
	{
		Buf[i] = Read_One_Byte(Addr+2+i);
	}
}
//




