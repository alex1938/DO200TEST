#include "rtc_alarm.h"


void RTC_IRQHandler(void)
{
     if ( RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
    }
    RTC_ClearITPendingBit(RTC_IT_SEC); 
    RTC_WaitForLastTask();

}
void RTCAlarm_IRQHandler(void)
{     if(RTC_GetITStatus(RTC_IT_ALR) != RESET)
      {

      }
  EXTI_ClearITPendingBit(EXTI_Line17);
  RTC_WaitForLastTask();
  RTC_ClearITPendingBit(RTC_IT_ALR);
  RTC_WaitForLastTask();
 }