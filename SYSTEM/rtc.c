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
 
		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�1λ,�����ȼ�3λ
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��ռ���ȼ�0λ,�����ȼ�4λ
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
		NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
			
	  NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  //
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //RTC
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	  NVIC_Init(&NVIC_InitStructure);
}

//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������

u8 RTC_Init(uint32_t time_h)
{
	    u8 temp=0;
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	    PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
		  BKP_DeInit();	//��λ�������� 	
			RCC_LSICmd(ENABLE);
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{
					temp++;
					delay_ms(10);	
			}
		  if(temp >= 250)return 1;//��ʼ��ʱ��ʧ��,����������	    
      RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);				
			RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
			RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
			RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
			RTC_ITConfig(RTC_IT_ALR, ENABLE);
			RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
			RTC_EnterConfigMode();/// ��������	
		  RTC_SetPrescaler(39999);//1HZ
			RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
			RTC_SetCounter(0);
			RTC_WaitForLastTask();
			RTC_SetAlarm(time_h);
			RTC_WaitForLastTask();
			RTC_ExitConfigMode(); //�˳�����ģʽ  
			RTC_WaitForLastTask();
	    RTC_NVIC_Config();//RCT�жϷ�������		    				     
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

//RTCʱ���ж�
//ÿ�봥��һ��  

void RTC_IRQHandler(void)
{		 
		if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
		{
			RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	   
		} 				
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�
		RTC_WaitForLastTask();	 		
}

