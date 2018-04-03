#ifndef __ADC_H
#define	__ADC_H


#include "sys.h"	 
#include "stdlib.h" 
void  Adc_Init(void);
u16 Get_Adc(ADC_TypeDef * ADCx,u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);

#endif /* __ADC_H */

