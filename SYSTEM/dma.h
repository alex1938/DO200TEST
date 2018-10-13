#ifndef __DMA_H
#define __DMA_H
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stdio.h"
#include "calct.h"
void DMA_Configuration(void);
uint16_t DMA_GetADCValue(uint8_t Channel);
#endif // __DMA_H
