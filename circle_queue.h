#ifndef __CIRCLE_QUEUE_H__
#define __CIRCLE_QUEUE_H__
//���е�ʵ�֣����ڸ���ͨ�ų��ϴ������ݡ�����SPI,IIC,UART��
//����:www.hongchangzidonghua.com ΢�Ź��ں�:0����1    ��������

//������еĳ���
#define QUEUESIZE   256
  
typedef unsigned char QueueElem_t ;
typedef unsigned short int uint16_t ;
  
typedef struct _CircleQueue  {  
    QueueElem_t data1[QUEUESIZE];
    uint16_t front;
    uint16_t rear;
    uint16_t count;
}CircleQueue_t;   
//
//��ʼ�����С�
unsigned char Queue_Init(CircleQueue_t *pQue);
//�ж϶����Ƿ��ǿյġ�
unsigned char Queue_IfEmpty(CircleQueue_t *pQue);
//�ж϶����Ƿ�������  
unsigned char Queue_IfFull(CircleQueue_t *pQue);
//���һ���ֽڡ��ŵ������ж������һ���µ��ֽ�������ӡ�
unsigned char Queue_Put(CircleQueue_t *pQue, QueueElem_t e);
//�Ӷ��л�ȡһ���ֽڡ������Ե�ַ����ʽ 
unsigned char Queue_Get(CircleQueue_t *pQue, QueueElem_t *e);
//��ȡ���е�ǰ�ĳ��� 
uint16_t Queue_Length(CircleQueue_t *pQue) ;
//��ն�����������
unsigned char Queue_Clear(CircleQueue_t *pQue )  ;


#endif	//__CIRCLE_QUEUE_H__

