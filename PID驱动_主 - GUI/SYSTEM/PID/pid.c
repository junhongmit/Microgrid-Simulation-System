#include"pid.h"
#include"sys.h"
#include"math.h"
float integral1 = 0, integral2 = 0, integral3 = 0;
float err;
//float err_last;
float tempDuty  ;
//float err_last;

void PID_Clear(float *integral)
{
	*integral=0;
}
void PID(float feedBack, float setValue, float* DutyCycle, float* integral, float Max, float Min, float MaxErr,float KP,float KI,float KD,float DUTY_MAX,float DUTY_MIN)//抗积分饱和的PID
{
	u8 index;
	err = setValue - feedBack;
	if (feedBack>Max) {
		if (fabs(err)>MaxErr) index=0;
		else {
			index=1;
			if (err<0) {
				*integral+=err;
			}
		}
	} else if (feedBack<Min) {
		if (fabs(err)>MaxErr) index=0;
		else {
			index=1;
			if (err>0) {
				*integral+=err;
			}
		}
	} else {
		if (fabs(err)>MaxErr) index=0;
		else {
			index=1;
			*integral+=err;
		}
	}
	/*tempDuty = *dutyCycle;
	delta = KP*err + KI*errTotal;
	
	delta = delta / FACTOR;
	tempDuty += delta;
	errTotal = err;*/
	tempDuty=KP*err+KI*(*integral)*index;//+KD*(err-err_last);
	
	if (tempDuty > DUTY_MAX)	
		tempDuty = DUTY_MAX;
	if (tempDuty < DUTY_MIN)
		tempDuty = DUTY_MIN;
	
	*DutyCycle = tempDuty;
	//err_last=err;
}
float deltaI,deltaU,p1,p2;
void MPPT(float U, float I, float lastU, float lastI, float *Uref)//电导增量法MPPT
{
	deltaI=(I-lastI);
	deltaU=(U-lastU);
	if (fabs(deltaU)<1e-6) {
		if (fabs(deltaI)<1e-6) return;
		if (deltaI>0) *Uref=*Uref-deltaVol;
		else *Uref=*Uref+deltaVol;
		if (*Uref > U_MAX)	
			*Uref = U_MAX;
		if (*Uref < U_MIN)
			*Uref = U_MIN;
		return;
	}
	p1=deltaI/deltaU;
	p2=-I/U;
	if (fabs(p1-p2)<1e-6) return;
	if (p1>p2) *Uref=*Uref-deltaVol;
	else if(p1<p2) *Uref=*Uref+deltaVol;
	if (*Uref > U_MAX)	
		*Uref = U_MAX;
	if (*Uref < U_MIN)
		*Uref = U_MIN;
}
