
#include "dma.h"

uint16_t AD_Value[15][2];

void DMA_Configuration(void) {
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // DMA OPEN

  DMA_DeInit(DMA1_Channel1);                                 // DMA1 RESET
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; // DMA--ADC1
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; // DMA1 目地地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;     //目的地址内存
  DMA_InitStructure.DMA_BufferSize = 30; // DMA通道缓存大小
  DMA_InitStructure.DMA_PeripheralInc =
      DMA_PeripheralInc_Disable;                          //外设地址不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
  DMA_InitStructure.DMA_PeripheralDataSize =
      DMA_PeripheralDataSize_HalfWord; //数据宽度16位
  DMA_InitStructure.DMA_MemoryDataSize =
      DMA_MemoryDataSize_HalfWord;                    //数据宽度16位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     //循环缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; // DMA通道 高优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // DMA通道未设置为内存到内存传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); //初始化DMA
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

/**************************************************
Function: DMA_GetADCValue();
Description: 读出DMA通道的ADC当前值
Return: 当前值
***************************************************/
uint16_t DMA_GetADCValue(uint8_t Channel) {
  uint8_t j;
  uint32_t temp = 0;
  if (Channel > 4) {
    return 0;
  }
  for (j = 0; j < 10; j++) {
    temp += AD_Value[j][Channel];
  }
  temp = temp / 10;
  return (uint16_t)temp;
}
