
#ifndef	__SYSTEM_H
#define	__SYSTEM_H

#include "stm32f10x.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"

void RCC_Configuration(void);
void NVIC_Configuration(void);
void RTC_Configuration(void);
#endif

