#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ov7725.h"
 
 int main(void)
 {	 

	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。 
  	while(1) 
	{		 
		if(OV7725_Init()==0)
		{
			LCD_ShowString(30,130,200,12,12,"OV7725 Init OK       ");
		}else{
			LCD_ShowString(30,130,200,12,12,"OV7725 Init Failed   ");
		}

		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^"); 
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
    					 
		LED0=!LED0;				   		 
		delay_ms(1000);	
	} 
}
 