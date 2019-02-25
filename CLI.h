//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作
#ifndef ___CLI_H__
#define ___CLI_H__
#include<STC15W.h>

//接收到了一帧CMD，开始处理。
unsigned char CLI(void);

//判断str1里面是否包含str2，返回位置(注意，长度不超过255)
unsigned char Compare_str(unsigned char *str1,unsigned char *str2);



#endif 


