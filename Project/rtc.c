#include "rtc.h"

void RTC_Configuration(void)
{
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//  if(PWR_GetFlagStatus(PWR_FLAG_SB) != RESET)
//  {
//    PWR_ClearFlag(PWR_FLAG_SB);
//    RTC_WaitForSynchro();
//  }
//  else
//  {
//    BKP_DeInit();
//    RCC_LSEConfig(RCC_LSE_ON);
//    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
//    {
//    }
//	/* 选择 LSE 为 RTC 时钟源 */
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
 //   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	 PWR_BackupAccessCmd(ENABLE);
	 RTC_ITConfig(RTC_IT_SEC, DISABLE);
	 RTC_SetAlarm(RTC_GetCounter() + 60); 
//    RCC_RTCCLKCmd(ENABLE);
//    RTC_WaitForSynchro(); 
//    RTC_SetPrescaler(32767);  
//    RTC_WaitForLastTask();
  


//  RTC_WaitForLastTask();
  RTC_ITConfig(RTC_IT_ALR, ENABLE);//使能闹钟中断
//  RTC_WaitForLastTask();
}



