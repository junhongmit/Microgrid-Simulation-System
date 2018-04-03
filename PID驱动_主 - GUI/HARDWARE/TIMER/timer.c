#include "timer.h"
#include "pwm.h"
#include "sys.h"	
#include "lcd.h"
#include "pid.h"
#include <string.h>
#include "control.h"
#include "adc.h"
#include "math.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "arm_math.h"

#define MOD(a,b) ( (a) < 0 ? (a+b) : ( (a) >= (b) ? (a-b) : (a) ) )
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


extern const u16 sin_chart[1200],cos_chart[1200];
extern float MI,pwmA,pwmB,pwmC,W;
extern u16 nowpwm,Errortimer,Tb;
extern s16 Tdif;
extern u8 POWER,Update,P1;
extern u8 choose,Error;
extern float SetVoltage,integral1,integral2;
extern HPANEL *panel1;
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 period)//TIM3 定时器主频42MHz
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = period-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


void TIM4_Int_Init(u16 period)//TIM3 定时器主频42MHz
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM3时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = period-1; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u16 adc0,adc1,adc2,adc3,POWERTimer;
float volt1,volt2,amp1,amp2;
extern HEDIT *edit1,*edit2,*edit3,*edit4,*edit5,*edit6;
extern float PhaseShift,FreqWord;
u8 nowEXTI2,lastEXTI2,nowEXTI3,lastEXTI3;
float VoltageOut,CurrentOut;
float lastVoltageIn,lastCurrentIn,Phase;
static u16 update_time=0;
float A_p,B_p,C_p,A_n,B_n,C_n,phi;

//定时器3中断服务函数
void TIM3_IRQHandler(void)//10kHz T=10e-5s
{
	u16 nowpwm;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
//		Phase=Phase+Round(FreqWord)*0.001F;
//		Phase=MOD(Phase,1200);
//		A_p=(sin_chart[Round(MOD(Phase,1200))]-840)*MI;
//		B_p=(sin_chart[Round(MOD(Phase+400,1200))]-840)*MI;
//		C_p=(sin_chart[Round(MOD(Phase+800,1200))]-840)*MI;
//		
//		A_n=A_p+840;
//		B_n=C_p;
//		C_n=B_p;
//		
//		A_p=A_p+840;
//		B_p=B_p+840;
//		C_p=C_p+840;

//		
//		pwmA=A_p;
//		pwmB=B_p;
//		pwmC=C_p;
		//pwmA=pwmB=pwmC=840;
		//nowpwm=Round(Phase+PhaseShift>=1000?Phase+PhaseShift-1000:(Phase+PhaseShift<0?Phase+PhaseShift+1000:Phase+PhaseShift));
		//TIM_SetCompare1(TIM1,Round(840+(chart[nowpwm]-840)*pwm));
		//TIM_SetCompare2(TIM1,Round(840+(chart[nowpwm]-840)*pwm));
		TIM_SetCompare1(TIM1,pwmA);
		TIM_SetCompare2(TIM1,pwmB);
		TIM_SetCompare3(TIM1,pwmC);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
u16 nowpoint,timer=0,data_timer;
u8 head=0,tail=9;
float LastVoltage[10]={0};
float LastCurrent[10]={0};
float VoltageAB,VoltageABIO,VoltageBC,VoltageBCIO,VoltageCA,Ua,Ub,Uc;
float VoltageABSum=0,VoltageABAvr=0;
float VoltageBCSum=0,VoltageBCAvr=0;
float Current,CurrentOutAvr=10,CurrentOutSum,CurrentMax=0,CurrentShow=0;
float Ialpha,Ibeta,Id,Iq,theta=0,Pd,Pq,Id_save,Iq_save,Ua_fang,Ub_fang,Uc_fang,Uab_fang,Ubc_fang,Uca_fang,time;
float VoltageA,VoltageB,VoltageC,Voltage_AB,Voltage_BC,Voltage_CA;
u8 first=1;
//定时器4中断服务函数
void TIM4_IRQHandler(void) //10kHz T=1e-4s
{
	u16 adc;
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		if (POWER) {	
			adc3=Get_Adc_Average(3,30);
			adc2=Get_Adc_Average(2,30);
			
			VoltageABIO=adc3/4096.0F*3.301F;
			VoltageABSum=VoltageABSum+VoltageABIO;
			
			VoltageBCIO=adc2/4096.0F*3.301F;
			VoltageBCSum=VoltageBCSum+VoltageBCIO;
			
			if (!first) {
				VoltageAB=((adc3/4096.0F*3.301F-VoltageABAvr)*24.671F + 0.0918F);
				VoltageBC=((adc2/4096.0F*3.301F-VoltageBCAvr)*24.671F + 0.0918F);//*25.216F - 0.1022F);
				VoltageCA=-(VoltageAB+VoltageBC);
				panel1->data[data_timer][0]=VoltageAB*1.5;
				panel1->data[data_timer][1]=VoltageBC*1.5;
				panel1->data[data_timer][2]=VoltageCA*1.5;
				data_timer++;
				if (data_timer>=235) data_timer=0;
				Ua=-0.33333333F*(2*VoltageAB+VoltageBC);
				Ub=-0.33333333F*(2*VoltageBC+VoltageCA);
			}
			LastVoltage[tail]=VoltageAB;
			tail++; if (tail==10) tail=0;
			head++; if (head==10) head=0;
			if (LastVoltage[tail]>0.0001F&&LastVoltage[head]<-0.0001F) {//过零
				LED0=!LED0;
				panel1->data_sum=data_timer;
				data_timer=0;
				VoltageA=sqrt(Ua_fang/time);
				VoltageB=sqrt(Ub_fang/time);
				VoltageC=sqrt(Uc_fang/time);
				Voltage_AB=sqrt(Uab_fang/time);
				Voltage_BC=sqrt(Ubc_fang/time);
				Ua_fang=0; Ub_fang=0; Uc_fang=0; 
				Uab_fang=0; Ubc_fang=0; Uca_fang=0;
				time=0;
			}
			timer++;
			Ua_fang=Ua_fang+Ua*Ua*1.0e-4F;
			Ub_fang=Ub_fang+Ub*Ub*1.0e-4F;
			Uc_fang=Uc_fang+Uc*Uc*1.0e-4F;
			Uab_fang=Uab_fang+VoltageAB*VoltageAB*1.0e-4F;
			Ubc_fang=Ubc_fang+VoltageBC*VoltageBC*1.0e-4F;
			time=time+1.0e-4F;
			
			//adc=Get_Adc_Average(3,30);
			//Current=((adc/4096.0F*3.29F));//-1.514F)/0.185F;
//			CurrentOutSum=CurrentOutSum+Current;
//			CurrentOut=Current-CurrentOutAvr;
//			if (CurrentOut>CurrentMax) CurrentMax=CurrentOut;
			
			arm_clarke_f32(Ua,Ub,&Ialpha,&Ibeta);
			arm_park_f32(Ialpha,Ibeta,&Id,&Iq,(sin_chart[Round(theta)]-840)/840.0F,(cos_chart[Round(theta)]-840)/840.0F);
			theta=theta+W*1e-4F*1200;
			theta=MOD(theta,1200);
			Id_save=Id; Iq_save=Iq;
			if (choose) {
				PID(/*feedBack*/Id, /*setValue*/19.5959, /*DutyCycle*/&Pd, /*integral*/&integral1, /*Max*/50, /*Min*/-50, /*MaxErr*/100,/*KP*/0.00005,/*KI*/0.00005,/*KD*/0,/*DUTY_MAX*/0.95,/*DUTY_MIN*/0);
				PID(/*feedBack*/Iq, /*setValue*/0, /*DutyCycle*/&Pq, /*integral*/&integral2, /*Max*/50, /*Min*/-50, /*MaxErr*/100,/*KP*/0.00005,/*KI*/0.00005,/*KD*/0,/*DUTY_MAX*/0.95,/*DUTY_MIN*/-0.5);
			} else {
				Pd=0.5; Pq=0;
			}
			arm_inv_park_f32(Pd,Pq,&Ialpha,&Ibeta,sin_chart[Round(theta)]-840,cos_chart[Round(theta)]-840);
			arm_inv_clarke_f32(Ialpha,Ibeta,&pwmA,&pwmB);
			pwmC=-(pwmA+pwmB);
			pwmA=pwmA+840;
			pwmB=pwmB+840;
			pwmC=pwmC+840;
			if (pwmA>1680) pwmA=1680;
			if (pwmA<0) pwmA=0;
			if (pwmB>1680) pwmB=1680;
			if (pwmB<0) pwmB=0;
			if (pwmC>1680) pwmC=1680;
			if (pwmC<0) pwmC=0;
			
			//adc=Get_Adc_Average(1,30);
			//Voltage=(adc/4096.0*3.300)*3.3386+28.314;
			
			/*if (VoltageIn<25) {  
				POWERTimer=0;
				POWER=0;
				pwm=0;
				//欠压保护
			}*/
//			if (CurrentShow>2.5F&&!first) {  
//				POWERTimer=0;
//				POWER=0;
//				MI=0;
//				//过流保护
//			}
		} else {
			POWERTimer++;
			VoltageABAvr=0;  VoltageBCAvr=0;
			VoltageOut=0; CurrentOut=0;
			if (POWERTimer>10000) {
				POWER=1; PID_Clear(&integral1);
				first=1;
			}
		}
		update_time++;
		if (update_time==3999) {	//Update Frequency 5Hz
			update_time=0;
			strcpy((char*)edit1->text,(char*)RealToStr(Voltage_AB));
			strcpy((char*)edit2->text,(char*)RealToStr(Voltage_BC));
			strcpy((char*)edit3->text,(char*)RealToStr(VoltageA));
			VoltageABAvr=VoltageABSum/timer;
			VoltageBCAvr=VoltageBCSum/timer;
			VoltageABSum=0;
			VoltageBCSum=0;
			first=0; timer=0;
			Update=1;
		}
//		nowpoint++;
//		if (nowpoint==799) {
//			nowpoint=0; first=0;
//			VoltageOutAvr=VoltageOutSum/800.0F;
//			VoltageOutSum=0;
//				
//			CurrentOutAvr=CurrentOutSum/800.0F;
//			CurrentOutSum=0;
//			CurrentShow=CurrentMax/1.4142F*4.1949F - 0.0035F; CurrentMax=0;
//			VoltageShow=VoltageMax/1.4142F; VoltageMax=0; //VoltageMin=1000;
//		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
