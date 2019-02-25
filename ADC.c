//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������
#include<ADC.h>
#include<Uart_1.h>

#define	Con_ADC	0x80	//����ADC�Ĵ���
#define	Sta_ADC	0x88	//��ʼADCת��
#define	VCC	3300 //5Vϵͳ��5000mV;3.3Vϵͳ��3300mV

//������һ�������壬���ڴ���ADC����ʹ��
union
{
	unsigned char a[2];
	unsigned int	Result;
} ADC;

unsigned int Adc_Result=0;//ADCת�����
//��ʼ��ADC����
void	Init_ADC(void)
{
	P1ASF	=	0X10;//�Ѷ�Ӧ��IO������Ϊģ������
	CLK_DIV |= 0X20;//����������˳��RES ����λ ��RES_LOW�Ͱ�λ  10λADC����
	ADC_CONTR	=	Con_ADC;//��ADC��Դ������ADC����
	EADC = 1;//��ADC�ж�����
	ADC_RES = 0;//���ADCת�����
}
//��ȡĳͨ��ADC�ĵ�ѹֵ����λmV
unsigned int	Get_Vol(unsigned char ch)
{
	unsigned int Vol;
	float	a;
	ADC_CONTR = Sta_ADC+ch;//��ָ��ͨ����ADC
	while(ADC_CONTR == (Sta_ADC+ch));
	a = Adc_Result;//����ADC���
	a *= VCC;
	a /= 1024;
	Vol = a;
	return Vol;
}
//ADC�����ж�
void	Isr_ADC(void)	interrupt	5
{
	ADC_CONTR =	Con_ADC;//���ADC��־λ
	ADC.a[0]	=	ADC_RES;
	ADC.a[1]	=	ADC_RESL;
	Adc_Result = ADC.Result;//��ADC���ֱ�Ӵ��͹���
}
//�����¶ȣ��������ͺ���MCP9700���������¶�����ֱ����ASCII��ʽ
void Get_Temp(unsigned char *Val)
{
	float temp;
	unsigned int adc_value;
	float Tem;
	
	temp = Get_Vol(4);
	
	Tem = temp;
	Tem -=500;
	Tem *= 10;//���ݹ�ʽ������¶�
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
	
	//Send_Str1(Val);//���ڷ��ͳ�ȥ������ʱ��ʹ�á�
	
}
/*
//����һ��ADCת��
void	Get_ADC(unsigned char ch)
{
	ADC_CONTR = Sta_ADC+ch;//��ָ��ͨ����ADC
}
*/

