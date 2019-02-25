#include "IAP_EEPROM.h"
#include<Uart_1.h> //����CLI����
//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
//STC15W4K48S4 ��Ƭ��������ʹ��IAP�����д�ڲ�Flash
//��ʼ��ַ 0X0000 ���� 0X27FF  ���У�512Byteһ������

//�ر�IAP����
void Close_IAP(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0;
	IAP_ADDRL = 0;
}
//����ĳ��ַ���ڵ�����
void Erase_IAP(unsigned int addr)
{
		IAP_CONTR = 0x82;//����IAP����������IAP�����ٶ�         
    IAP_CMD = 0x03;//������������            
    IAP_ADDRL = addr;              
    IAP_ADDRH = addr >> 8;//��ַ��ֵ          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;//��������               
    _nop_();_nop_();_nop_();            
    Close_IAP();//���IAP�����Ĵ�����
}
//��ĳ��ַд��һ���ֽ�
void Write_One_Byte(unsigned int addr,unsigned char dat)
{
		IAP_CONTR = 0x82;//����IAP����������IAP�����ٶ�   
    IAP_CMD = 0x02;//IAPд����
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;//��ַ��ֵ
    IAP_DATA = dat;//����д�Ĵ���
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;//����
    _nop_();
    Close_IAP();//���IAP�����Ĵ�����
}
//��ȡĳ��ַһ���ֽ�
unsigned char Read_One_Byte(unsigned int addr)
{
		unsigned char dat=0;//����һ�������ص�����
    IAP_CONTR = 0x82;//����IAP
    IAP_CMD = 0x01;//IAP������
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;//��ַ��ֵ
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;//����
    _nop_();_nop_();_nop_();_nop_();
    dat = IAP_DATA;
		Close_IAP();//���IAP�����Ĵ�����
    return dat;
}
//�������ݵ�Flash
void Save_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int Len)
{
	unsigned int i=0;
	Erase_IAP(Addr);//������Ӧ����
	
	if(Len >= 511)
		Len = 511;
	
	Write_One_Byte(Addr,Len>>8);//����д���׵�ַ
	Write_One_Byte(Addr+1,(Len&0X00FF));//����д���׵�ַ
	for(i=0;i<Len;i++) //˳��洢��Ч��Ϣ
	{
		Write_One_Byte((Addr+2+i),Buf[i]);
	}
}
//��Flash��ȡ����
void Read_Flash_Message(unsigned int Addr,unsigned char *Buf,unsigned int *Len)
{
	unsigned int i=0;
	*Len = Read_One_Byte(Addr); //���ȶ�ȡ������Ϣ
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




