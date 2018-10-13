#include "rtc.h" 	

extern  PROTOCOL_TypeDef PROTOCOL_Structure;
extern  uint16_t Ttimes;
extern	uint8_t   temout; 
extern  uint32_t UltrTestCount;
extern  OS_FLAG_GRP * flags_event;	

static void RTC_NVIC_Config(void)
{	
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
 
		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//先占优先级1位,从优先级3位
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//先占优先级0位,从优先级4位
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
		NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
			
	  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  //
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //RTC
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	  NVIC_Init(&NVIC_InitStructure);
}

//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码

u8 RTC_Init(uint32_t time_h)
{
	    u8 temp=0;
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR和BKP外设时钟   
	    PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
		  BKP_DeInit();	//复位备份区域 	
			RCC_LSICmd(ENABLE);
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{
					temp++;
					delay_ms(10);	
			}
		  if(temp >= 250)return 1;//初始化时钟失败,晶振有问题	    
      RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);				
			RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
			RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
			RTC_WaitForSynchro();		//等待RTC寄存器同步  
			RTC_ITConfig(RTC_IT_ALR, ENABLE);
			RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
			RTC_EnterConfigMode();/// 允许配置	
		  RTC_SetPrescaler(39999);//1HZ
			RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
			RTC_SetCounter(0);
			RTC_WaitForLastTask();
			RTC_SetAlarm(time_h);
			RTC_WaitForLastTask();
			RTC_ExitConfigMode(); //退出配置模式  
			RTC_WaitForLastTask();
	    RTC_NVIC_Config();//RCT中断分组设置		    				     
	    return 0; //ok
}		 

void RTCAlarm_IRQHandler(void)
{
		u8 err=0;
  	u32 count;
		EXTI_ClearITPendingBit(EXTI_Line17);
  	SET_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);
	  OSIntEnter() ;
		OSFlagPost(flags_event,RtcDectectBit,OS_FLAG_SET,&err);
	  OSIntExit();
	  count = RTC_GetCounter();
	  RTC_SetAlarm(count+10);
    UltrTestCount++;
}

//RTC时钟中断
//每秒触发一次  

void RTC_IRQHandler(void)
{		 
		if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
		{
			RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	   
		} 				
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断
		RTC_WaitForLastTask();	 		
}

