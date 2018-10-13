//超声波控制等函数
#include "ultra.h"

ULTRA_TypeDef ULTRA_Structure;

uint8_t ULTRA_Send_Count = 0;

void ULTRA_StructInit(void)
{
		memset(ULTRA_Structure.Echo_Time, 0, 3); //回波时间 单位us
		ULTRA_Structure.Echo_Count = 0;           //回波数量计算
		ULTRA_Structure.IRQFlag = 0;             //
		ULTRA_Structure.Average_Time = 0;         //平均时间
}

//超声波开启定时器启动探头进行测量
void ULTRA_StartDetect(void)
{
		EXTI->IMR &= ~EXTI_Line11; //关闭11线的外部中断
		TIM4->CNT = 0;
		TIM4->CR1 |= TIM_CR1_CEN; //打开中断
		TIM3->CR1 |= TIM_CR1_CEN; //打开中断
}

/**********************
*   function: Enable or DisAble Ultra pwer

*   input:   ENABLE     //  Enable Ultra
*            DISABLE     //  Disable Ultra
*   Output: null
**********************/
void Ultra_Power( uint8_t  mode)
{
      if( mode )
     {   
		     GPIO_SetBits(GPIOB, GPIO_Pin_12);
			   GPIO_SetBits(GPIOB, GPIO_Pin_9);
		     delay_ms(10);
		 }else
     {
			   GPIO_ResetBits(GPIOB, GPIO_Pin_12);
			   GPIO_ResetBits(GPIOB, GPIO_Pin_9);
			   delay_ms(10);
		 }
}


void UltraTest( uint8_t  number)
{

}

void ULTRA_ReadAirHeight(uint16_t *Air_Height) 
{
		float Speed = 0;
		float Temperature = 24.0;
		uint16_t Air_Temp = 0;
	  ULTRA_Structure.IRQFlag = 0;  
	  Ultra_Power(ENABLE);
    do{
				 ULTRA_StartDetect(); 
		     delay_ms(50);
		     Air_Temp++;   
		}while(( ULTRA_Structure.IRQFlag != 1) && ( Air_Temp < 5 ));

	  Ultra_Power(DISABLE);	
	
		NTC_GetTemperature(&Temperature);
	  printf("Temperature :%.2f\r\n",Temperature);
		if(Temperature < -35.0) 
		{
			Temperature = 24.0;
		}
		
		Speed = (331.0 + 0.6 * Temperature) / 20.0; 
		Air_Temp = (uint16_t)(ULTRA_Structure.Average_Time * Speed / 100);
		ULTRA_Structure.Average_Time = 0;
		ULTRA_Structure.IRQFlag = 0 ;
		if(Air_Temp > ULTRARANGE) 
		{ 
			*Air_Height = ULTRARANGE;
		} else 
		{
			*Air_Height = Air_Temp;
		}
	
}

//处理回波信息
void ULTRA_EchoHandler(void) 
{
		ULTRA_Structure.Echo_Time[ULTRA_Structure.Echo_Count] = TIM4->CNT;
		ULTRA_Structure.Echo_Count++;
		if (ULTRA_Structure.Echo_Count == 3) //收到3次后计算一次平均
		{ 
			  ULTRA_Structure.IRQFlag = 1 ;
				Bubble_Sort(ULTRA_Structure.Echo_Time, 3); 
				ULTRA_Structure.Average_Time =
				Get_Average(&ULTRA_Structure.Echo_Time[1], 1);
				printf("AverageTime: %d us\r\n",ULTRA_Structure.Average_Time);
			  memset(ULTRA_Structure.Echo_Time, 0, 3);
				ULTRA_Structure.Echo_Count = 0; //清零计数
			  
		}
}


// TIM3中断驱动超声波
void TIM3_IRQHandler(void) 
{
		if (TIM3->SR & TIM_IT_Update) 
		{
				TIM3->SR = (uint16_t)~TIM_IT_Update; //清除中断标志
				ULTRA_Send_Count++;
				if (ULTRA_Send_Count <= ULTRACOUNT)		
				{
						if (ULTRA_Send_Count % 2 == 0) 
						{
							GPIOB->BSRR = GPIO_Pin_8;//GPIOB->BSRR = GPIO_Pin_8;
						} else 
						{
							 GPIOB->BRR = GPIO_Pin_8;//  GPIOB->BRR = GPIO_Pin_8;
						}
				} else 
				{
						TIM3->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN)); //关闭计数
						EXTI->IMR |= EXTI_Line11; //打开11线的外部中断
						ULTRA_Send_Count = 0;
				}
		}
}

// TIM4 中断回波时间计算
void TIM4_IRQHandler(void) 
{
		if (TIM4->SR & TIM_IT_Update) 
		{
				TIM4->SR = (uint16_t)~TIM_IT_Update; //清除中断标志
		}
}

//超声波回波中断
void EXTI15_10_IRQHandler(void) 
{
		if (EXTI->PR & EXTI_Line11) 
		{
				if (( TIM4->CNT > BLIND_TIME ) && ( TIM4->CNT <= MAX_TIME ) ) 
				{
						TIM4->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN)); 
						EXTI->IMR &= ~EXTI_Line11; 
						ULTRA_EchoHandler();
				}
				EXTI->PR = EXTI_Line11;
		}
}
