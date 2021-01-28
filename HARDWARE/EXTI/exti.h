#ifndef __EXTI_H
#define __EXIT_H
#include "sys.h"

void EXTIX_Init(void);	//外部中断初始化
void EXTI8_Init(void);

#define OV7670_VSYNC  	PAin(8)			//同步信号检测IO
#define OV7670_WRST		PDout(6)		//写指针复位
#define OV7670_WREN		PBout(3)		//写入FIFO使能
#define OV7670_RCK_H	GPIOB->BSRR=1<<4//设置读数据时钟高电平
#define OV7670_RCK_L	GPIOB->BRR=1<<4	//设置读数据时钟低电平
#define OV7670_RRST		PGout(14)  		//读指针复位
#define OV7670_CS		PGout(15)  		//片选信号(OE)

#endif

























