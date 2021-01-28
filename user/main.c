#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ov7725.h"
#include "string.h"
#include "timer.h"
#include "exti.h"


//���������ֺ궨��
#define  OV7725 1

//����OV7725��������װ��ʽԭ��,OV7725_WINDOW_WIDTH�൱��LCD�ĸ߶ȣ�OV7725_WINDOW_HEIGHT�൱��LCD�Ŀ��
//ע�⣺�˺궨��ֻ��OV7725��Ч
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

extern u8 ov_sta;	//��exit.c�� �涨��
extern u8 ov_frame;	//��timer.c���涨��

//����LCD��ʾ(OV7725)
void OV7725_camera_refresh(void) {
	u32 i,j;
	u16 color;
	if(ov_sta) { //��֡�жϸ���
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//����ʾ�������õ���Ļ����
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//����ʾ�������õ���Ļ����
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		OV7725_RRST=0;				//��ʼ��λ��ָ��
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//��λ��ָ�����
		OV7725_RCK_H;
		for(i=0; i<OV7725_WINDOW_HEIGHT; i++) {
			for(j=0; j<OV7725_WINDOW_WIDTH; j++) {
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H;
				color<<=8;
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//������
				OV7725_RCK_H;
				LCD->LCD_RAM=color;
			}
		}
		ov_sta=0;					//����֡�жϱ��
		ov_frame++;
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽��
	}
}

int main(void) {

	u8 lcd_id[12];			//���LCD ID�ַ���

	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;

	u8 sensor=0;
	u8 key;
	u8 i=0;
	u8 msgbuf[15];//��Ϣ������
	u8 tm=0;

	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();

	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣

	while(1) {
		if(OV7725_Init()==0) {
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGAģʽ���
			//OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,1);//VGAģʽ���
			LCD_ShowString(30,210,200,16,16,"OV7725 Init OK       ");

			OV7725_Light_Mode(lightmode);
			OV7725_Color_Saturation(saturation);
			OV7725_Brightness(brightness);
			OV7725_Contrast(contrast);
			OV7725_Special_Effects(effect);
			OV7725_CS=0;
			break;
		} else {
			LCD_ShowString(30,210,200,16,16,"OV7725_OV7670 Error!!");
			delay_ms(200);
			LCD_Fill(30,210,239,246,WHITE);
			delay_ms(200);
		}
	}

	//TIM6_Int_Init(10000,7199);			//10Khz����Ƶ��,1�����ж�
	EXTI8_Init();						//ʹ���ⲿ�ж�8,����֡�ж�
	LCD_Clear(BLACK);
	//LCD_Fill(0,0,459,600,WHITE);
	while(1) {
		OV7725_camera_refresh();		//������ʾ
		delay_ms(40);
	}
}
