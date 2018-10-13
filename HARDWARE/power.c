#include "power.h"


/**********************
*   function：Enable or DisAble check battery power

*   input：  ENABLE     //  Enable check battery
*            DISABLE     //  Disable check battery
*   Output： null
**********************/
void Battery_Power(uint8_t mode)
{
     if( mode )
     { 
				 GPIO_SetBits(GPIOB, GPIO_Pin_7);
				 delay_ms(200);
		 }else
     {
		     GPIO_ResetBits(GPIOB, GPIO_Pin_7);
			   delay_ms(200);
		 }


}


 //电池电量采集
void BatLevel(uint8_t* Bat, uint16_t	*Volt)
{  
	    float adc_temp;
      Battery_Power(ENABLE);
			adc_temp = ((DMA_GetADCValue(2) * 2.50 * 2 / 4095 ) + 0.05 );//得出电压值3.0-3.6v电压值
			printf("Battery: %.2f V\r\n",adc_temp);
	    * Bat = ( ( adc_temp-3.0 ) / 0.6 ) * 100;
      *Volt = adc_temp*100;
		  Battery_Power(DISABLE);
}
