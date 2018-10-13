#ifndef __GPIO_H
#define __GPIO_H
#include <stdio.h>
#include "stm32f10x.h"

void GPIO_Configuration(void);
void EXTI_Configuration(void);
void GPIO_OFF(void);
void GPIO_ON(void);
void MCU_SLEEP(void);
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#endif
