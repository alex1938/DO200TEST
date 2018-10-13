#ifndef __POWER_H
#define __POWER_H
#include "stm32f10x.h"
#include "stdio.h"
#include "dma.h"
#include "delay.h"

//使能或者失能检测电量电源
void Battery_Power(uint8_t mode);
 //电池电量采集
void BatLevel(uint8_t* Bat, uint16_t	*Volt);

#endif 
