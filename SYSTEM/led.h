#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h" 	
#include "gpio.h"
#include "stm32f10x_bkp.h"
#include "system.h"
#include "timer.h"

void WKUP_Init(void); 			//PA0 WKUPªΩ–—≥ı ºªØ
void LED(uint8_t Mode);
#endif


