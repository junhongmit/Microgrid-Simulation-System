#include "timer.h"
#include "pwm.h"
#include "sys.h"	
#include "lcd.h"
#include "pid.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
extern float MI,pwmA,pwmB,pwmC,K;
extern u16 nowpwm;
extern s16 Tdif;
extern u8 POWER,Update,Send;
extern u8 choose,Error,SendData[33];
extern float SetVoltage,integral1,integral2,integral3;
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

u16 adc0,adc1,adc2,adc3,adc4,adc5,adc6,POWERTimer;
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
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
//		Phase=Phase+Round(FreqWord)*0.001F;
//		Phase=MOD(Phase,1200);
//		if (Errortimer>=10000&&Errortimer<=16250) {
//			phi=Deg/360.0F*1200;
//		} else phi=0;
//		A_p=(sin_chart[Round(MOD(Phase+phi,1200))]-840)*MI;
//		B_p=(sin_chart[Round(MOD(Phase+400+phi,1200))]-840)*MI;
//		C_p=(sin_chart[Round(MOD(Phase+800+phi,1200))]-840)*MI;
//		
//		A_n=A_p;
//		B_n=C_p;
//		C_n=B_p;
//		
//		if (P1||(Errortimer>=10000&&Errortimer<=16250)) {
//			A_p=1/(1+balance)*A_p+balance/(1+balance)*A_n+840;
//			B_p=1/(1+balance)*B_p+balance/(1+balance)*B_n+840;
//			C_p=1/(1+balance)*C_p+balance/(1+balance)*C_n+840;
//		} else {
//			A_p=A_p+840;
//			B_p=B_p+840;
//			C_p=C_p+840;
//		}
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
u16 nowpoint,timer=0,data_timer,Locked_timer,Link_timer;
u8 head=0,tail=9,SetClose=0;
float LastVoltage[10]={0};
float LastCurrent[10]={0};
float VoltageAB,VoltageABIO,VoltageBC,VoltageBCIO,VoltageCA,Ua,Ub,Uc,W=50;
float VoltageABSum=0,VoltageABAvr=0;
float VoltageBCSum=0,VoltageBCAvr=0;
float Current1ASum=0,Current1AAvr=0,Current1AIO;
float Current1BSum=0,Current1BAvr=0,Current1BIO;
float Current2ASum=0,Current2AAvr=0,Current2AIO;
float Current2BSum=0,Current2BAvr=0,Current2BIO;
float Current1A,Current1B,Current2A,Current2B;
float I1A_fang,I1B_fang,I2A_fang,I2B_fang,Current_1A,Current_1B,Current_2A,Current_2B;
float Ialpha,Ibeta,Phased,Phaseq,theta=0,Pd,Pq,Id_save,Iq_save,Ua_fang,Ub_fang,Uc_fang,Uab_fang,Ubc_fang,Uca_fang,time;
float VoltageA,VoltageB,VoltageC,Voltage_AB,Voltage_BC,Voltage_CA,VoltageIn;
float I2dref,I2qref,I2d,I2q;
u8 first=1,Locked=0,Link=0;
float KP=5,KI=0.005;
float sin_one,cos_one;
//定时器4中断服务函数
void TIM4_IRQHandler(void) //20kHz T=5e-5s
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		if (POWER) {	
			adc2=Get_Adc_Average(2,10);//输出AB线电压
			VoltageABIO=adc2*2.4414e-4F*3.301F;
			VoltageABSum=VoltageABSum+VoltageABIO;
			
			adc0=Get_Adc_Average(0,10);//输出BC线电压
			VoltageBCIO=adc0*2.4414e-4F*3.301F;
			VoltageBCSum=VoltageBCSum+VoltageBCIO;
			
			adc1=Get_Adc_Average(1,10);//从A相电流
			Current2AIO=adc1*2.4414e-4F*3.301F;
			Current2ASum=Current2ASum+Current2AIO;
			
			adc3=Get_Adc_Average(3,10);//从B相电流
			Current2BIO=adc3*2.4414e-4F*3.301F;
			Current2BSum=Current2BSum+Current2BIO;
			
			adc5=Get_Adc_Average(5,10);//主A相电流
			Current1AIO=adc5*2.4414e-4F*3.301F;
			Current1ASum=Current1ASum+Current1AIO;
			
			adc6=Get_Adc_Average(6,10);//主B相电流
			Current1BIO=adc6*2.4414e-4F*3.301F;
			Current1BSum=Current1BSum+Current1BIO;
			
			adc4=Get_Adc_Average(4,10);//输入电压
			VoltageIn=((adc4*2.4414e-4F*3.301F)*21.5F);
			
			if (!first) {
				Current2A=-((adc1*2.4414e-4F*3.301F-Current2AAvr)*5.5236F + 0.0137F);
				Current2B=-((adc3*2.4414e-4F*3.301F-Current2BAvr)*5.6653F - 0.0058F);
				Current1A=-((adc5*2.4414e-4F*3.301F-Current1AAvr)*5.55F + 0.0154F);
				Current1B=-((adc6*2.4414e-4F*3.301F-Current1BAvr)*5.5786F - 0.0309F);
				
				VoltageAB=((adc2*2.4414e-4F*3.301F-VoltageABAvr)*24.671F + 0.0918F);
				VoltageBC=((adc0*2.4414e-4F*3.301F-VoltageBCAvr)*24.671F + 0.0918F);//*25.216F - 0.1022F);
				VoltageCA=-(VoltageAB+VoltageBC);
				
//				panel1->data[data_timer][0]=VoltageAB;
//				panel1->data[data_timer][1]=VoltageBC;
//				panel1->data[data_timer][2]=VoltageCA;
				data_timer++;
				Ua=-0.33333333F*(2*VoltageAB+VoltageBC);
				Ub=-0.33333333F*(2*VoltageBC+VoltageCA);
			}
			LastVoltage[tail]=VoltageAB;
			tail++; if (tail==10) tail=0;
			head++; if (head==10) head=0;
			if (LastVoltage[tail]>0.0001F&&LastVoltage[head]<-0.0001F) {//过零
				data_timer=0;
//				VoltageA=sqrt(Ua_fang/time);
//				VoltageB=sqrt(Ub_fang/time);
				Current_1A=sqrt(I1A_fang/time);
				Current_1B=sqrt(I1B_fang/time);
				Current_2A=sqrt(I2A_fang/time);
				Current_2B=sqrt(I2B_fang/time);
//				Voltage_AB=sqrt(Uab_fang/time);
//				Voltage_BC=sqrt(Ubc_fang/time);
				Ua_fang=0; Ub_fang=0; I1A_fang=0; I1B_fang=0; I2A_fang=0; I2B_fang=0;
				time=0;
			}
			timer++;
//			Ua_fang=Ua_fang+Ua*Ua*5.0e-5F;
//			Ub_fang=Ub_fang+Ub*Ub*5.0e-5F;
			I1A_fang=I1A_fang+Current1A*Current1A*5.0e-5F;
			I1B_fang=I1B_fang+Current1B*Current1B*5.0e-5F;
			I2A_fang=I2A_fang+Current2A*Current2A*5.0e-5F;
			I2B_fang=I2B_fang+Current2B*Current2B*5.0e-5F;
//			Uab_fang=Uab_fang+VoltageAB*VoltageAB*5.0e-5F;
//			Ubc_fang=Ubc_fang+VoltageBC*VoltageBC*5.0e-5F;
			time=time+5.0e-5F;
			sin_one=(sin_chart[Round(theta)]-840)*1.1905e-3F;
			cos_one=(cos_chart[Round(theta)]-840)*1.1905e-3F;
			arm_clarke_f32(Ua,Ub,&Ialpha,&Ibeta);
			arm_park_f32(Ialpha,Ibeta,&Phased,&Phaseq,sin_one,cos_one);
			PID(/*feedBack*/-Phaseq, /*setValue*/0, /*DutyCycle*/&W, /*integral*/&integral1, /*Max*/50, /*Min*/-50, /*MaxErr*/100,/*KP*/KP,/*KI*/KI,/*KD*/0,/*DUTY_MAX*/80,/*DUTY_MIN*/20);
			if (fabs(Phaseq)<4.0F) {
				Locked_timer++;
				if (Locked_timer>50000) {
					Locked=1; LED1=0; Locked_timer=0;
					if (!SetClose) {
						choose=1; SetClose=1;
					}
					On_PWM();
				}
			} else {
				Locked=0; LED1=1; Locked_timer=0;
				//Off_PWM();
			}
			if (choose) {
				if (Locked) {
					if (!Link) {
						RELAY=1; Link=1; Link_timer=0;
					} else {
						if (Link_timer>399) {
							arm_clarke_f32(Current1A/K,Current1B/K,&Ialpha,&Ibeta);
							arm_park_f32(Ialpha,Ibeta,&I2dref,&I2qref,sin_one,cos_one);
							arm_clarke_f32(Current2A,Current2B,&Ialpha,&Ibeta);
							arm_park_f32(Ialpha,Ibeta,&I2d,&I2q,sin_one,cos_one);
							Id_save=I2d; Iq_save=I2q;
							PID(/*feedBack*/I2d, /*setValue*/I2dref, /*DutyCycle*/&Pd, /*integral*/&integral2, /*Max*/50, /*Min*/-50, /*MaxErr*/100,/*KP*/0.05,/*KI*/0.00005,/*KD*/0,/*DUTY_MAX*/0.95,/*DUTY_MIN*/0);
							PID(/*feedBack*/I2q, /*setValue*/I2qref, /*DutyCycle*/&Pq, /*integral*/&integral3, /*Max*/50, /*Min*/-50, /*MaxErr*/100,/*KP*/0.05,/*KI*/0.00005,/*KD*/0,/*DUTY_MAX*/1,/*DUTY_MIN*/-1);
						} else {
							Link_timer++;
						}
					}
				}
			} else {
				if (Link) {
					RELAY=0; Link=0;
				}
				Pd=0.5; Pq=0;
			}
			arm_inv_park_f32(Pd,Pq,&Ialpha,&Ibeta,sin_chart[Round(MOD(theta+17,1200))]-840,cos_chart[Round(MOD(theta+17,1200))]-840);
			arm_inv_clarke_f32(Ialpha,Ibeta,&pwmA,&pwmB);
			theta=theta+W*5e-5F*1200;
			theta=MOD(theta,1200);
			
			pwmC=-(pwmA+pwmB);
			pwmA=pwmA+840;
			pwmB=pwmB+840;
			pwmC=pwmC+840;
			if (pwmA>1680) pwmA=1680;
			else if (pwmA<0) pwmA=0;
			if (pwmB>1680) pwmB=1680;
			else if (pwmB<0) pwmB=0;
			if (pwmC>1680) pwmC=1680;
			else if (pwmC<0) pwmC=0;
			
			
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
			
			VoltageOut=0; CurrentOut=0;
			if (POWERTimer>10000) {
				POWER=1; PID_Clear(&integral1);
				first=1;
			}
		}
		update_time++;
		if (update_time==3999) {	//Update Frequency 5Hz
			update_time=0;
			strcpy((char*)edit1->text,(char*)RealToStr(Current_1A));
			strcpy((char*)edit2->text,(char*)RealToStr(Current_2A));
			strcpy((char*)edit3->text,(char*)RealToStr(Phased));
			VoltageABAvr=VoltageABSum/timer;
			VoltageBCAvr=VoltageBCSum/timer;
			Current1AAvr=Current1ASum/timer;
			Current1BAvr=Current1BSum/timer;
			Current2AAvr=Current2ASum/timer;
			Current2BAvr=Current2BSum/timer;
			VoltageABSum=0;
			VoltageBCSum=0;
			Current1ASum=0;
			Current1BSum=0;
			Current2ASum=0;
			Current2BSum=0;
			Send=1;
			first=0; timer=0;
			Update=1;
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
