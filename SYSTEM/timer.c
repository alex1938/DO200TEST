#include "timer.h"

void TIM_Configuration(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3 |RCC_APB1Periph_TIM4,ENABLE);                 // 时钟使能TIM2
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); //初始化结构体

  // TIM3作为超声波的驱动发生器
  TIM_DeInit(TIM3);
  if (ULTRAFREZ == ULTRAFRE112) {
    TIM_TimeBaseStructure.TIM_Period = 159; // 36000/160/2=112khz
  }else if (ULTRAFREZ == ULTRAFRE200) {
    TIM_TimeBaseStructure.TIM_Period = 89; // 36000/90/2=200khz
  } else{
    TIM_TimeBaseStructure.TIM_Period = 159; // 
  }
  TIM_TimeBaseStructure.TIM_Prescaler = 1; // 36m

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM3->SR = (uint16_t)~TIM_IT_Update; //清除中断标志
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  // TIM4作为超声波的回波时间计数器
	
  TIM_DeInit(TIM4);
  TIM_TimeBaseStructure.TIM_Period = 59999; // 60ms中断
  TIM_TimeBaseStructure.TIM_Prescaler = 71; // 1us

  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  TIM4->SR = (uint16_t)~TIM_IT_Update; //清除中断标志
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
