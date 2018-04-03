#ifndef __PID_H
#define __PID_H 

#include "sys.h"

#define U_MAX  1.0
#define U_MIN  0.1

#define deltaVol 0.0025

typedef signed long pidDataType;
extern s16 umax,umin;
void PID_Clear(float *integral);
void PID(float feedBack, float setValue, float* DutyCycle, float* integral, float Max, float Min, float MaxErr,float KP,float KI,float KD,float DUTY_MAX,float DUTY_MIN);//抗积分饱和的PID
void MPPT(float U, float I, float lastU, float lastI, float *Duty);//电导增量法MPPT
#endif
