#ifndef __PWM_H
#define	__PWM_H

#include "sys.h"	 
#include "stdlib.h" 

void TIM3_Mode_Config(u16 period);
void TIM3_PWM_Init(void);

void TIM1_GPIO_Config(void);
void TIM1_Mode_Config(u16 period);
void TIM1_PWM_Init(void);
void On_PWM(void);
void Off_PWM(void);

#endif /* __PWM_OUTPUT_H */

