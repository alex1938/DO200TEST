#include "sleep.h"


//void MUC_SLEEP(void)
//{
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE); 
//GPIO_PinRemapConfig( GPIO_Remap_PD01 , ENABLE);
//RCC_LSEConfig(RCC_LSE_OFF);
//RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
//ADC_CONFIG(DISABLE);
//USART_DeInit(USART1);
//USART_DeInit(USART2);
//USART_DeInit(USART3);
//USART_DeInit(UART5);
//EXTI_INITIAL(ENABLE);
//GPIO_PIN_INITIAL(GPIOA,GPIO_Pin_All,GPIO_Mode_AIN,0); 
//GPIO_PIN_INITIAL(GPIOB,GPIO_Pin_All,GPIO_Mode_AIN,0); 
//GPIO_PIN_INITIAL(GPIOC,GPIO_Pin_All,GPIO_Mode_AIN,0); 
//GPIO_PIN_INITIAL(GPIOD,GPIO_Pin_All,GPIO_Mode_AIN,0);
////PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);//STOP??
//PWR_EnterSTANDBYMode();//standby?? 
//}