#ifndef __POWER_H
#define __POWER_H
#include "stm32f10x.h"
#include "stdio.h"
#include "dma.h"
#include "delay.h"

//ʹ�ܻ���ʧ�ܼ�������Դ
void Battery_Power(uint8_t mode);
 //��ص����ɼ�
void BatLevel(uint8_t* Bat, uint16_t	*Volt);

#endif 
