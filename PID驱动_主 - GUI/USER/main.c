#include "led.h"
#include "control.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "touch.h"
#include "pwm.h"
#include "adc.h"
#include "pid.h"
#include "exti.h"
#include "timer.h"
#include <string.h>
#include <math.h>
#include <arm_math.h>
#include "24l01.h"
const float pi=3.1415926F;
u8 *name1[15]={
"7","8","9","P1","P2",
"4","5","6","Rst","Enter",
"1","2","3","0",".",
};
// ADC1转换的电压值通过DMA方式传到flash
extern __IO u16 ADC_ConvertedValue;

// 局部变量，用于存从flash读到的电压值			 
__IO u16 ADC_ConvertedValueLocal; 
HBUTTON *but[3][5],*button1,*button2,*button3,*button4,*button5;
HTRACK *Freqtrack;
HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6,*edit7,*edit8,*edit9,*edit10;
HTAB *tab1;
HPANEL *panel1;
u8 choose,Update=0,Error=0,P1;
u16 sin_chart[1200],cos_chart[1200];
u16 nowpwm,Errortimer,Tb;
u8 POWER;
float MI=0.8,pwm=0.5,pwmA=0.5,pwmB=0.5,pwmC=0.5,PhaseShift,FreqWord,W;
u16 delay,delay_left;
float Tin,Tout;
float Period;
//ALIENTEK 探索者STM32F407开发板 实验13
//LCD显示实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
void GetSin()
{
	u16 i;
	for (i=0; i<1200; i++) {
		sin_chart[i]=Round(arm_sin_f32(2*pi*(i+1)/1200.0F)*840+840);
		cos_chart[i]=Round(arm_cos_f32(2*pi*(i+1)/1200.0F)*840+840);
	}
}
void Load_Draw_Dialog(void)
{
	int i,j;
	LCD_Clear(WHITE);//清屏   
 	POINT_COLOR=BLACK;//设置字体为蓝色 
	//LCD_ShowString(40,5,64,16,16,"Duty:");
	//LCD_ShowBigNum(80,0,(float)500/1000*100);
	//LCD_ShowString(200,8,8,16,16,"%");
	//CreateButton(0,0,120,160,"123",NULL,0);
	//dutytrack=CreateTrackBar(20,40,200,30,6000,201);
	//SetTrackPos(dutytrack,3000);
	//CreateButton(0,40,20,30,"-",0,000);
	//CreateButton(220,40,20,30,"+",0,001);
	//periodtrack=CreateTrackBar(20,100,200,30,65535,202);
	//SetTrackPos(periodtrack,6000);
	//CreateButton(0,100,20,30,"-",0,002);
	//CreateButton(220,100,20,30,"+",0,003);
	POINT_COLOR=BLACK;
	//LCD_ShowString(38,80,100,20,16,"Period:");
	//LCD_ShowBigNum(78,50+16+4,6000);
	edit1=CreateEdit(10,20,110,30,"0");//V
	edit2=CreateEdit(10,53,110,30,"0");//A
	edit3=CreateEdit(10,86,110,30,"0");//A
	//edit4=CreateEdit(10,125,110,30,"0");//A
	//edit5=CreateEdit(10,160,110,30,"0");//A
	tab1=CreateTab(5,120,230,195,1001);
	AddTabText(tab1,"SetFreq");
	AddTabText(tab1,"RF");
	AddTabText(tab1,"Wave");
	
	//SetFreq
	Freqtrack=CreateTrackBar(20,142,200,30,1000,201);
	SetTrackPos(Freqtrack,250);
	edit5=CreateEdit(10,175,220,40,"50");
	LCD_ShowString(5,2,60,16,16,"Input:");
	LCD_ShowString(125,26,8,16,16,"V");
	LCD_ShowString(125,61,8,16,16,"A");
	
	button1=CreateButton(140,10,90,35,"Open Loop",0,000);
	button2=CreateButton(140,55,90,35,"Close Loop",0,001);
	for (i=0; i<3; i++)
		for (j=0; j<5; j++) {
			if (i==0&&j==3) continue;
			if (i==0&&j==4) continue;
				//LCD_Color_Fill(9+46*j,172+36*i,40,30,gImage_button1);
				//LCD_ShowString(9+46*j+20-strlen(name1[i*5+j])*3,172+36*i+15-6,40,30,12,name1[i*5+j]);
			but[i][j]=CreateButton(9+46*j,223+31*i,40,25,(char*)name1[i*5+j],0,100+i*5+j);
		}
	SetButtonState(but[0][3],1);
	SetButtonState(button1,1);
	choose=0;
		
	//RF
	edit6=CreateEdit(90,150,110,30,"0");//A
	edit7=CreateEdit(90,190,110,30,"0");//A
	edit8=CreateEdit(90,230,110,30,"0");//A
	edit9=CreateEdit(90,270,110,30,"0");//A
	ShowWindow(edit6,SW_HIDE);
	ShowWindow(edit7,SW_HIDE);
	ShowWindow(edit8,SW_HIDE);
	ShowWindow(edit9,SW_HIDE);	
	
	//SimErr
//	button3=CreateButton(10,190,80,40,"Normal",0,201);
//	button4=CreateButton(10,250,80,40,"Error",0,202);
//	ShowWindow(button3,SW_HIDE);
//	ShowWindow(button4,SW_HIDE);
//	SetButtonState(button3,1);
//	Curtrack=CreateTrackBar(10,142,80,30,1000,501);
//	SetTrackPos(Curtrack,800);
//	Tbtrack=CreateTrackBar(100,142,130,30,1000,502);
//	SetTrackPos(Tbtrack,625);
//	LCD_ShowString(120,172,24,16,16,"Tb:");
//	Downtrack=CreateTrackBar(100,202,130,30,1000,503);
//	SetTrackPos(Downtrack,200);
//	LCD_ShowString(120,232,40,16,16,"Down:");
//	Ktrack=CreateTrackBar(100,262,130,30,1000,504);
//	SetTrackPos(Ktrack,509);
//	LCD_ShowString(120,292,16,16,16,"K:");
//	ShowWindow(Curtrack,SW_HIDE);
//	ShowWindow(Tbtrack,SW_HIDE);
//	ShowWindow(Downtrack,SW_HIDE);
//	ShowWindow(Ktrack,SW_HIDE);
//		
//	//Deg
//	Degtrack=CreateTrackBar(20,142,200,30,1000,301);
//	SetTrackPos(Degtrack,500);
//	edit6=CreateEdit(45,175,150,40,"0");
//	ShowWindow(Degtrack,SW_HIDE);
//	ShowWindow(edit6,SW_HIDE);
//		
//	//Unbal
//	Baltrack=CreateTrackBar(20,142,200,30,1000,401);
//	SetTrackPos(Baltrack,0);
//	edit7=CreateEdit(45,175,150,40,"0");
//	ShowWindow(Baltrack,SW_HIDE);
//	ShowWindow(edit7,SW_HIDE);
	
	panel1=CreatePanel(10,160,220,140,0);
		
	SetTabShow(tab1,0);
	UpdateWindow(tab1);
	SendMessage(tab1,WM_COMMAND,tab1->num,0);
  	POINT_COLOR=RED;//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
 //5个触控点的颜色												 
const u16 POINT_COLOR_TBL[CT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
void Check_Touch(void)
{ 
	static u8 down=0;
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if (!down) {
					SendMessage(0,WM_TOUCHDOWN,tp_dev.x[0],tp_dev.y[0]);
					down=1;
				} else {
					SendMessage(0,WM_TOUCHMOVE,tp_dev.x[0],tp_dev.y[0]);
				}
			}
		}else {
			if (down) {
				SendMessage(0,WM_TOUCHUP,tp_dev.x[0],tp_dev.y[0]);
				down=0;
			}
		}
}

 int main(void)
 {	 
	u8 tmp_buf[33];	
	delay_init(168);	    	 //延时函数初始化	  
	//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	TIM1_GPIO_Config();
	Off_PWM();
 	LED_Init();			     //LED端口初始化
	NRF24L01_Init(); 
	POWER=1; PhaseShift=0; FreqWord=6000;
	LCD_Init();		
	EXTIX_Init();
	//EXTIX_Init();
	//TP_PreAdjust();
 	tp_dev.init();
	Adc_Init();
	GetSin();
 	POINT_COLOR=RED;//设置字体为红色 
	Load_Draw_Dialog();
	NRF24L01_Check();
	 
	delay=1; delay_left=10000;
	TIM3_Int_Init(4200);
	TIM4_Int_Init(4200);
	TIM1_PWM_Init();
	On_PWM();

	NRF24L01_RX_Mode();
	while (1) {
		Check_Touch();
		if (Update) {
			Update=0;
			UpdateWindow(edit1);
			UpdateWindow(edit2);
			UpdateWindow(edit3);
			UpdateWindow(edit4);
			UpdateWindow(edit6);
			UpdateWindow(edit7);
			UpdateWindow(edit8);
			UpdateWindow(edit9);
			UpdateWindow(panel1);
		}
		if(NRF24L01_RxPacket(tmp_buf)==0)//一旦接收到信息,则显示出来.
		{
			tmp_buf[32]=0;
			strcpy((char*)edit6->text,(char*)tmp_buf);	
			strcpy((char*)edit7->text,(char*)tmp_buf+8);
			strcpy((char*)edit8->text,(char*)tmp_buf+16);
			strcpy((char*)edit9->text,(char*)tmp_buf+24);
		}	
	}
}
