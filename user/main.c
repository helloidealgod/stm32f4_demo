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


//传感器名字宏定义
#define  OV7725 1

//由于OV7725传感器安装方式原因,OV7725_WINDOW_WIDTH相当于LCD的高度，OV7725_WINDOW_HEIGHT相当于LCD的宽度
//注意：此宏定义只对OV7725有效
#define  OV7725_WINDOW_WIDTH		320 // <=320
#define  OV7725_WINDOW_HEIGHT		240 // <=240

extern u8 ov_sta;	//在exit.c里 面定义
extern u8 ov_frame;	//在timer.c里面定义

//更新LCD显示(OV7725)
void OV7725_camera_refresh(void) {
	u32 i,j;
	u16 color;
	if(ov_sta) { //有帧中断更新
		LCD_Scan_Dir(U2D_L2R);		//从上到下,从左到右
		LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT);//将显示区域设置到屏幕中央
		if(lcddev.id==0X1963)
			LCD_Set_Window((lcddev.width-OV7725_WINDOW_WIDTH)/2,(lcddev.height-OV7725_WINDOW_HEIGHT)/2,OV7725_WINDOW_HEIGHT,OV7725_WINDOW_WIDTH);//将显示区域设置到屏幕中央
		LCD_WriteRAM_Prepare();     //开始写入GRAM
		OV7725_RRST=0;				//开始复位读指针
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RCK_L;
		OV7725_RRST=1;				//复位读指针结束
		OV7725_RCK_H;
		for(i=0; i<OV7725_WINDOW_HEIGHT; i++) {
			for(j=0; j<OV7725_WINDOW_WIDTH; j++) {
				OV7725_RCK_L;
				color=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H;
				color<<=8;
				OV7725_RCK_L;
				color|=GPIOC->IDR&0XFF;	//读数据
				OV7725_RCK_H;
				LCD->LCD_RAM=color;
			}
		}
		ov_sta=0;					//清零帧中断标记
		ov_frame++;
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向
	}
}

int main(void) {

	u8 lcd_id[12];			//存放LCD ID字符串

	u8 lightmode=0,effect=0;
	s8 saturation=0,brightness=0,contrast=0;

	u8 sensor=0;
	u8 key;
	u8 i=0;
	u8 msgbuf[15];//消息缓存区
	u8 tm=0;

	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	LED_Init();			     //LED端口初始化
	LCD_Init();

	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。

	while(1) {
		if(OV7725_Init()==0) {
			OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,0);//QVGA模式输出
			//OV7725_Window_Set(OV7725_WINDOW_WIDTH,OV7725_WINDOW_HEIGHT,1);//VGA模式输出
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

	//TIM6_Int_Init(10000,7199);			//10Khz计数频率,1秒钟中断
	EXTI8_Init();						//使能外部中断8,捕获帧中断
	LCD_Clear(BLACK);
	//LCD_Fill(0,0,459,600,WHITE);
	while(1) {
		OV7725_camera_refresh();		//更新显示
		delay_ms(40);
	}
}
