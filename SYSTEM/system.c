#include "system.h"

void RCC_Configuration(void) {

  ErrorStatus HSEStartUpStatus;

  RCC_DeInit();                               // RCC 复位
  RCC_HSEConfig(RCC_HSE_ON);                  //开启HSE
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); // WAIT FOR HSE
  if (HSEStartUpStatus == SUCCESS) {
    FLASH_PrefetchBufferCmd(
        FLASH_PrefetchBuffer_Enable);                   // Eable Prefetch Buffer
    FLASH_SetLatency(FLASH_Latency_2);                  // Set 2 Latency cycles
    RCC_HCLKConfig(RCC_SYSCLK_Div1);                    // AHB clock = SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                     // APB2 clock = HCLK
    RCC_PCLK1Config(RCC_HCLK_Div2);                     // APB1 clock = HCLK/2
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9); // PLLCLK = 8 * 9 =72
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
      ;
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08)
      ;
  }
}



void NVIC_Configuration(void) {
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x08003000);


}
