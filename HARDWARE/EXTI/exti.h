#ifndef __EXTI_H
#define __EXIT_H
#include "sys.h"

void EXTIX_Init(void);	//�ⲿ�жϳ�ʼ��
void EXTI8_Init(void);

#define OV7670_VSYNC  	PAin(8)			//ͬ���źż��IO
#define OV7670_WRST		PDout(6)		//дָ�븴λ
#define OV7670_WREN		PBout(3)		//д��FIFOʹ��
#define OV7670_RCK_H	GPIOB->BSRR=1<<4//���ö�����ʱ�Ӹߵ�ƽ
#define OV7670_RCK_L	GPIOB->BRR=1<<4	//���ö�����ʱ�ӵ͵�ƽ
#define OV7670_RRST		PGout(14)  		//��ָ�븴λ
#define OV7670_CS		PGout(15)  		//Ƭѡ�ź�(OE)

#endif

























