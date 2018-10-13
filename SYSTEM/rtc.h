#ifndef __RTC_H
#define __RTC_H	    
#include "includes.h"
#include "led.h"
#include "protocol.h"
#include "gpio.h"
#include "stm32f10x_bkp.h"
#include "system.h"
#include "timer.h"
#include "Lsm303Agr.h" 
#include "delay.h"
#include "usart.h"
u8 RTC_Init(uint32_t time_h);
void RTC_IRQHandler(void);


#endif

