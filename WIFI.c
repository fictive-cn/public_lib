//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#include<WIFI.h>
#include<CLI.h>
#include<Uart_1.h> //����CLI����
#include<Uart_2.h> //����WIFI����
#include<Timer1.h> //��ʱ��
#include<ADC.h>  //������ȡ�¶�
#include<IO.h>  //��������LED����ȡ������
#include"IAP_EEPROM.h" //�ڲ�Flash��������

#define	WIFI_Send_Byte	Send_Data2
#define	WIFI_Send_Str		Send_Str2


unsigned char MQTT_Heart[]={0xc0,0x00}; 
unsigned char MQTT_Heart_Reply[]={0xd0,0x00};
unsigned char MQTT_DisConnect[]={ 0xe0,0x00,};

unsigned int MQTT_Heart_Count=0;
unsigned int MQTT_PUB_Count=0;

//����ATָ��.�����ȴ��ظ���OK,������ط�
void Send_AT(unsigned char *Str)
{
	unsigned char Dat=0;
	unsigned char Count=0;
	unsigned char Loop_Count=0;
	unsigned char ReSend_Count=0;
	WIFI_Send_Str(Str);//��ͨ�����ڷ��ͳ�ȥ
	while(1)//�ȴ��ظ�
	{
		Delay1ms(50);
		Loop_Count++;
		if(Loop_Count >= 100)
		{
			ReSend_Count++;
			if(ReSend_Count < 3)
			{
				Loop_Count = 0;
				Send_Str1("\r\n�ط�ָ��: ");
				Send_Str1(Str);
				Send_Str1("\r\n");
				WIFI_Send_Str(Str);//�ط�һ��
			}
			else
			{
					Send_Str1("\r\n����ʧ��: ");
					Send_Str1(Str);
					Send_Str1("\r\n");
				return;//�ط�ʧ�ܣ��˳�
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
					Send_Str1("\r\n�ɹ�ִ��һ��ָ��: ");
					Send_Str1(Str);
					Send_Str1("\r\n");
					return;
				}
			}
		}
	}	
}
//ƴ�������ַ�������2��ӵ�1�ĺ���
void Sum_Str_Tail(unsigned char *str1,unsigned char *str2)
{
	unsigned int Len1=0;
	unsigned int Len2=0;
	unsigned int i=0;
	
	do
		Len1 ++;
	while(str1[Len1]);//����str1�ĳ���
	
	do
		Len2 ++;
	while(str2[Len2]);//����str2�ĳ���
	
	for(i=0;i<Len2;i++)
	{
		str1[Len1+i] = str2[i];
	}
	str1[Len1+i] = 0;
}
//ƴ�������ַ�������2��ӵ�1��ǰ��
void Sum_Str_Header(unsigned char *str1,unsigned char *str2)
{
	unsigned int Len1=0;
	unsigned int Len2=0;
	unsigned int i=0;
	
	do
		Len1 ++;
	while(str1[Len1]);//����str1�ĳ���
	
	do
		Len2 ++;
	while(str2[Len2]);//����str2�ĳ���
	
	for(i=0;i<Len1;i++)//�Ȱ�str1�������
		str1[Len2+Len1-i-1] = str1[Len1-i-1];
	str1[Len2+Len1] = 0;
	
	for(i=0;i<Len2;i++)
	{
		str1[i] = str2[i];
	}
}
//��ʼ��WIFI,��������SSID�Ȳ���
void Init_WIFI(void)
{
	unsigned char DataBuf[256];
	unsigned char DataBuf2[30];//��Ҫ�Ǹ�WIFI�����õġ������޶���������󳤶���30
	unsigned int DataLen=0;
	unsigned int i=0;
	
	//�˳�͸��ģʽ
	Send_AT("+++");
	
	Send_AT("AT\r\n");
	Send_AT("AT\r\n");
	//�رջ���
	Send_AT("AT+UARTE=OFF\r\n");
	
	//��TCP��EVENT
	Send_AT("AT+WEVENT=ON\r\n");
	Send_AT("AT+CIPEVENT=ON\r\n");
	
	//���ȹر�TCP1ͨ��,���ڵ���ʹ��
	Send_AT("AT+CIPSTOP=1\r\n");
	
	//�鿴Flash���ݣ�WIFI��SSID���֣������FF��00����ʾ��һ�ο���
	DataLen = Read_One_Byte(SSID_Addr);
	DataLen <<= 8;
	DataLen += Read_One_Byte(SSID_Addr+1);
	if((DataLen == 0XFFFF)|(DataLen == 0))
	{
		Send_Str1("\r\n  �豸��һ�ο��������������! \r\n");
		while(1)
		{
			CLI();//ִ��CLI�߼�
		}
	}
	//����ģ����TCP Client �Ĳ���
	Read_Flash_Message(MQTT_URL_Addr,DataBuf,&DataLen);
	Sum_Str_Tail(DataBuf,",1883\r\n");//�ں������
	Sum_Str_Header(DataBuf,"AT+CIPSTART=1,tcp_client,");//��ǰ�����
	Send_AT(DataBuf);//ATָ����ȥ
	
	//����WIFI��·����   stationģʽ
	Read_Flash_Message(SSID_Addr,DataBuf,&DataLen);
	Read_Flash_Message(WIFI_Pass_Addr,DataBuf2,&DataLen);
	Sum_Str_Header(DataBuf,"AT+WJAP=");
	Sum_Str_Tail(DataBuf,",");
	Sum_Str_Tail(DataBuf,DataBuf2);
	Sum_Str_Tail(DataBuf,"\r\n");
	Send_AT(DataBuf);
	Clear_WIFI();
	//�ȴ�TCP���ӳɹ��¼�
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
				
				//�ر�EVENT֪ͨ
				Send_AT("AT+WEVENT=OFF\r\n");
				Send_AT("AT+CIPEVENT=OFF\r\n");
				//����͸��ģʽ
				Send_AT("AT+CIPSENDRAW\r\n");
				
				Clear_WIFI();
				Send_Str1("\r\n TCP Connect OK! \r\n");
				LED2 = 1;
				
				return;
			}
		}
		CLI();//ִ��CLI�߼�
	}
}
//����CONNECT���ģ����ȴ����������ش���
void MQTT_Connect(void)
{
	unsigned char DataBuf[256];
	unsigned int DataLen=0;
	unsigned char Dat=0;
	unsigned int i=0;
	Delay1ms(2000);
	//�ȷ��ͶϿ����� ��ֹ�ϴ����ӻ�����
	for(i=0;i<2;i++)
	{
		WIFI_Send_Byte(MQTT_DisConnect[i]);
	}
	Clear_WIFI();//���WIFI���ջ�����
	Delay1ms(2000);
	
	//����MQTT������
	Read_Flash_Message(MQTT_Connect_Addr,DataBuf,&DataLen);//��ȡMQTT����
	for(i=0;i<DataLen;i++)
	{
		WIFI_Send_Byte(DataBuf[i]);
	}
	//�ȴ�����������
	Delay1ms(2000);
	Dat = 0;
	while(Get_Byte_WIFI(&DataBuf[Dat++]));//��ȡ���ջ���������
	if((DataBuf[0]==0X20)&(DataBuf[1]==0X02))
	{
		Send_Str1("\r\n  ����MQTT�������ɹ�! \r\n");
		LED3 = 1;
	}
	else
	{
		Send_Str1("\r\n  ����MQTT������ʧ��! \r\n");
		for(i=0;i<DataLen;i++)
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
		Delay1ms(2000);
	}
	
	//��������  �����������豸����  �������շ������·�����Ϣ
	Read_Flash_Message(MQTT_Sub_Addr,DataBuf,&DataLen);//��ȡMQTT����
	for(i=0;i<DataLen;i++)
	{
		WIFI_Send_Byte(DataBuf[i]);
	}
	//�ȴ�����������
	Delay1ms(2000);
	Dat=0;
	while(Get_Byte_WIFI(&DataBuf[Dat++]));//��ȡ���ջ���������
	if((DataBuf[0]==0X90)&(DataBuf[1]==0X03))
	{
		Send_Str1("\r\n  �������Գɹ��� \r\n");
	}
	else
	{
		Send_Str1("\r\n  ��������ʧ�ܣ� \r\n");
		for(i=0;i<DataLen;i++)
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
		Delay1ms(2000);
	}
}
//�������� �ŵ���ѭ���У���ʱ����־λ���˾ͷ���������
void Send_Heart(void)
{
	unsigned char MQTT_Heart[]={0xc0,0x00}; 
	if(MQTT_Heart_Count > 3000)//10ms��ʱ��������Զ���1
	{
		MQTT_Heart_Count = 0;
		WIFI_Send_Byte(MQTT_Heart[0]);
		WIFI_Send_Byte(MQTT_Heart[1]);
		Send_Str1("\r\n  ����һ����������������IOTƽ̨�� \r\n");
	}
}
//����MQTT���������͹��������ݣ�����
void Analyze_MQTT_Read(void)
{
	unsigned int Len=0,i=0;
	unsigned char Dat[256];
	
	if(Get_Mess_Count_WIFI() == 0)
		return;
	
	Len=0;
	while(Get_Byte_WIFI(&Dat[Len++]));//��ȡ���ջ���������
	Dat[Len] = 0;
	
	for(i=0;i<Len;i++)
	{
		if((Dat[i] == 0XD0)&(Dat[i+1]==0x00))
			Send_Str1("\r\n  �յ�MQTT�������ظ��������ź�! \r\n");
		if(Dat[i] == 0)//��ǰ����Щ�����ƴ���ʱ��0ȥ��
			Dat[i] = 1;
	}
	
	if(Compare_str(Dat,"\"WorkSwitch\":1") != 255) 
	{
		Relay = 1;
		Send_Str1("\r\n  ƽ̨�·�����򿪼̵���! \r\n");
	}
	if(Compare_str(Dat,"\"WorkSwitch\":0") != 255) 
	{
		Relay = 0;
		Send_Str1("\r\n  ƽ̨�·�����رռ̵���! \r\n");
	}
}
//��ʱ�ϱ��¶�����
void Pub_Temperature(void)
{
	unsigned char DataBuf[256];
	unsigned int DataLen=0,i=0;
	unsigned char Temperature[7];//����¶���Ϣ
	
	if(MQTT_PUB_Count > 1000)
	{
		MQTT_PUB_Count = 0;
		Get_Temp(Temperature);
		Send_Str1("\r\n �����ϱ��¶�Ϊ��");
		Send_Str1(Temperature);
		Send_Str1(" ���϶�\r\n");
		
		LED4 = ~LED4;
		
		Read_Flash_Message(MQTT_Post_Addr,DataBuf,&DataLen);//�����ϱ�����
		for(i=0;i<132;i++)//����ǰ132���ֽڵı���
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
		for(i=0;i<5;i++)//����5���ֽڵ��¶�
		{
			WIFI_Send_Byte(Temperature[i]);
		}
		for(i=137;i<DataLen;i++)//����ʣ�µ�����
		{
			WIFI_Send_Byte(DataBuf[i]);
		}
	}
}
//



