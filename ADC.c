//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#include<ADC.h>
#include<Uart_1.h>

#define	Con_ADC	0x80	//配置ADC寄存器
#define	Sta_ADC	0x88	//开始ADC转换
#define	VCC	3300 //5V系统是5000mV;3.3V系统是3300mV

//定义了一个共用体，用于传送ADC数据使用
union
{
	unsigned char a[2];
	unsigned int	Result;
} ADC;

unsigned int Adc_Result=0;//ADC转换结果
//初始化ADC功能
void	Init_ADC(void)
{
	P1ASF	=	0X10;//把对应的IO口设置为模拟输入
	CLK_DIV |= 0X20;//调整结果存放顺序RES 高两位 ，RES_LOW低八位  10位ADC方便
	ADC_CONTR	=	Con_ADC;//打开ADC电源，设置ADC速率
	EADC = 1;//打开ADC中断允许
	ADC_RES = 0;//清空ADC转化结果
}
//获取某通道ADC的电压值，单位mV
unsigned int	Get_Vol(unsigned char ch)
{
	unsigned int Vol;
	float	a;
	ADC_CONTR = Sta_ADC+ch;//打开指定通道的ADC
	while(ADC_CONTR == (Sta_ADC+ch));
	a = Adc_Result;//传送ADC结果
	a *= VCC;
	a /= 1024;
	Vol = a;
	return Vol;
}
//ADC采样中断
void	Isr_ADC(void)	interrupt	5
{
	ADC_CONTR =	Con_ADC;//清空ADC标志位
	ADC.a[0]	=	ADC_RES;
	ADC.a[1]	=	ADC_RESL;
	Adc_Result = ADC.Result;//把ADC结果直接传送过来
}
//计算温度，传感器型号是MCP9700。出来的温度数据直接是ASCII格式
void Get_Temp(unsigned char *Val)
{
	float temp;
	unsigned int adc_value;
	float Tem;
	
	temp = Get_Vol(4);
	
	Tem = temp;
	Tem -=500;
	Tem *= 10;//根据公式计算出温度
	if(Tem > 0)
	{
		Val[0] = '+'; // +
		adc_value = Tem;
	}
	else
	{
		Tem *= -1;
		Val[0] = '-'; // -
		adc_value = Tem;
	}
	Val[1] = adc_value/1000 + 0x30;
	Val[2] = adc_value/100%10 + 0x30;
	Val[3] = 0X2E; // .
	Val[4] = adc_value/10%10 + 0x30;
	Val[5] = 0x00;
	
	//Send_Str1(Val);//串口发送出去，调试时候使用。
	
}
/*
//进行一次ADC转换
void	Get_ADC(unsigned char ch)
{
	ADC_CONTR = Sta_ADC+ch;//打开指定通道的ADC
}
*/

