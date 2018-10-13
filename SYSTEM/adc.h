#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#include "delay.h"

void ADC_Configuration(void);
uint16_t ADC_GetData(void);
#endif // __ADC_H
