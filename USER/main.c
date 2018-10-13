#include "main.h"
extern  PROTOCOL_TypeDef PROTOCOL_Structure;
OS_CPU_SR cpu_sr=0;
uint32_t Ttimes=0,CapCount;        //检测时间间隔
uint8_t   temout=0,CapFlag; 
u8 MagnentCarState = 0;
u8 UltraCarSate = 0;
u8 NoMagnetButUltra = 0;//地磁不工作状态位
u8 NoMagnetButUltraCarState = 0;
u8 MagneDistur = 0;//地磁干扰标志位
uint32_t SendDataCount = 0;
extern u8 CarState ;
extern u8 PreCarState;
extern int XYZSumMagn[3];
u32 UltrTestCount = 0;      //超声波检测计数器
OS_FLAG_GRP  *flags_event;	//信号量级
/*设置任务优先级*/
#define MainManager_Pri   10    // 设置优先级
#define UltraStart_Pri    3     // 设置优先级
#define EchoBack_Pri      4     // 设置优先级
#define DealUsart_Pri     2     // 设置优先级
#define Sleep_Pri         5     // 设置优先级

/*设置任务栈的大小*/
#define MainManager_Size  512 //设置堆栈大小
#define UltraStart_Size   512 //设置堆栈大小
#define EchoBack_Size     512 //设置堆栈大小
#define DealUsart_Size    512 //设置堆栈大小
#define Sleep_Size        512 //设置堆栈大小

/*设置堆栈*/
__align(8) OS_STK  MainManager_Stack[MainManager_Size];   //静态分配方式
__align(8) OS_STK  UltraStart_Stack[UltraStart_Size];     //静态分配方式,8字节对齐
__align(8) OS_STK  EchoBack_Stack[EchoBack_Size];         //静态分配方式,8字节对齐
__align(8) OS_STK  DealUsart_Stack[DealUsart_Size];       //静态分配方式,8字节对齐
__align(8) OS_STK  Sleep_Stack[Sleep_Size];               //静态分配方式,8字节对齐
  
/*设置任务函数*/
void MainManager_Task(void *p);  //主任务
void UltraStart_Task(void *p);   //启动探头
void EchoBack_Task(void *p);     //关闭终端
void DealUsart_Task(void *p);    //上传数据
void Sleep_Task(void *p);        //低功耗函数

int main(void) 
{
 
  RCC_Configuration();   //时钟配置
	NVIC_Configuration();
  USART_Configuration(); //串口配置
  GPIO_Configuration();  // gpio配置
  EXTI_Configuration();  //外部中断配置
  delay_init();          // systick时钟配置
  TIM_Configuration();   //定时器配置
  ADC_Configuration();   //adc配置
	DMA_Configuration();   //dma配置
	NB_ReadBasicOPt();
	PROTOCOL_ReadBasicOPt();
  GPIO_ON();//引脚启动
	
	
	printf("\r\nDO200-90V1.1.1\r\n");
  OSInit(); //系统初始化  
  OSTaskCreate(MainManager_Task,(void*)0,
  (OS_STK*)&MainManager_Stack[MainManager_Size-1],MainManager_Pri);//创建任务。任务代码指针；参数指针；堆栈栈顶指针；优先级
  OSStart();//系统开始
 
  } 

//主任务用于管理所有的任务
void MainManager_Task(void*p) 
{

		u8 err;	 
	  for(;;)
	  { 
				flags_event=OSFlagCreate(0,&err); 
				OS_ENTER_CRITICAL();  //关中断
				OSTaskCreate(UltraStart_Task,(void*)0,(OS_STK*)&UltraStart_Stack[UltraStart_Size-1],UltraStart_Pri); 
				OSTaskCreate(EchoBack_Task,(void*)0,(OS_STK*)&EchoBack_Stack[EchoBack_Size-1],EchoBack_Pri);    
				OSTaskCreate(DealUsart_Task,(void*)0,(OS_STK*)&DealUsart_Stack[DealUsart_Size-1],DealUsart_Pri);   
				OSTaskCreate(Sleep_Task,(void*)0,(OS_STK*)&Sleep_Stack[Sleep_Size-1],Sleep_Pri);    
				OSTaskSuspend(MainManager_Pri);
				OS_EXIT_CRITICAL();  //开中断   
	  }
}
    
//驱动超声波
void UltraStart_Task(void*p)
{    
		u8 err;
		for(;;)
	  {    
			OSFlagPend( flags_event,UltrasoundBit,OS_FLAG_WAIT_SET_ANY,0,&err);    //等待信号量0x01 ;           		
			ULTRA_ReadAirHeight(&PROTOCOL_Structure.Height);   //高度检测
			printf("High: %d\r\n",PROTOCOL_Structure.Height);
			if( ( PROTOCOL_Structure.Height > 0 ) && ( PROTOCOL_Structure.Height < 500 ) )//高度在范围内
			{
					printf("Have Car\r\n");
					UltraCarSate = 1;     //超声波状态位
					if( MagnentCarState == 0 )    //地磁状态位
					{
							NoMagnetButUltra = 1;
							if( NoMagnetButUltra != NoMagnetButUltraCarState )
							{
									NoMagnetButUltraCarState = NoMagnetButUltra; 
									OSFlagPost(flags_event,SendDataStartBit,OS_FLAG_SET,&err);//发送信号量0x08
									OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);  //等待信号量0x10
									OSTaskSuspend(UltraStart_Pri);//自挂起
							}
							else
							{
								  OSFlagPost( flags_event,SendDataFinishBit,OS_FLAG_SET,&err );//发送信号量0x08
								  OSTaskSuspend(UltraStart_Pri);//自挂起
							}
					}
					else
					{
							CarState = 1;
						  printf("UltraCarSate:%d\r\n", UltraCarSate);
						  printf("Send Car State: %d\r\n", CarState);
							OSFlagPost(flags_event,SendDataStartBit,OS_FLAG_SET,&err);//发送信号量0x08
							OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);//等待信号量0x10
							OSTaskSuspend(UltraStart_Pri);
					}
			}else        //超声波不在范围内
			{
				  //如果地磁检测到有车，超声波在这个高度范围，则视为地磁干扰，视为无车
					printf("No Car\r\n");
					if( MagnentCarState == 1 )
					{
							UltraCarSate = 0;
							CarState = 1;
							OSFlagPost(flags_event,SendDataStartBit,OS_FLAG_SET,&err);//发送信号量0x08
							OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);//等待信号量0x10
							OSTaskSuspend(UltraStart_Pri);//自挂起
					}
					
					if( MagnentCarState == 0 )//如果地磁检测到：有车->无车，超声波确认,在该范围，则视为车正常离开；
					{
							MagneDistur = 0 ;//地磁干扰状态消除，
							printf("The Car Left\r\n\r\n");
							UltraCarSate = 0;
							CarState = UltraCarSate;
							NoMagnetButUltra = 0;
						
							if((NoMagnetButUltra!= NoMagnetButUltraCarState))
						  {
									NoMagnetButUltraCarState = NoMagnetButUltra; 
									OSFlagPost(flags_event,SendDataStartBit,OS_FLAG_SET,&err);//发送信号量0x08
									OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);//等待信号量0x10
									OSTaskSuspend(UltraStart_Pri);//自挂起
						  }else
							{
									OSFlagPost(flags_event,SendDataFinishBit,OS_FLAG_SET,&err);
									OSTaskSuspend(UltraStart_Pri);//自挂起
						  }
							
				}
			 }
				OSTimeDly(100);
    }
}
	 
// 上传数据任务  
void EchoBack_Task(void *p)
{
	  u8 err;	
		for(;;)     
		{  
				 OSFlagPend(flags_event,SendDataStartBit,OS_FLAG_WAIT_SET_ANY,0,&err);//  等待信号0x08 
         PROTOCOL_AlarmData();
			   OSFlagPost(flags_event,SendDataFinishBit,OS_FLAG_SET,&err);//发送信号量10  ，发送数据结束信号
				 OSTimeDly(100);
		}
}
	
void DealUsart_Task(void *p)
{
	  uint8_t  ledStatus = 0;
		for(;;)
	  {
			 LED(ledStatus);
			 ledStatus = !ledStatus;
			
		   if(flag_usart1==1)//处理usart1指令
			 {
			   SetBasicInformat(USART1_Data.DATA_RX,USART_REC_LEN);
			   USART1_Data.RX_STA = 0;
			   flag_usart1 = 0;
		   }	
			 
	     OSTimeDly(500);
	  }
}
	
void Sleep_Task(void *p)
{	
		u8 err;	
		OS_FLAGS uBits = 0;//
		u32 count1 = 0;//车进入计数
		u32 count2 = 0;//车离开计数
		u32 count3 = 0;//复位后发送状态标记
		u32 count4 = 0;//地磁扰动计数
		u8 tempCurState = 0;//
		int XYZ[3] = {0};
		MPU_IIC_Init(); 
		Lsm303Agr_Init();
		RTC_Init(10);		
	  for(;;)
	  {
				uBits = OSFlagPend(flags_event,ExtIntBit|RtcDectectBit|Ext3IntBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err); //等待信号量 0x02 、0x04
			  if(( uBits & ExtIntBit ) != 0||( uBits & Ext3IntBit ) != 0 )
				{
					
						if( ( LSM303AGR_Detect_Query_Car( &tempCurState,XYZ ) < 3) )//地磁检测到车,XYZ值悬空不用
						{
							  if( tempCurState == 1 )  //表示磁强超过阈值，去除扰动
								{
										if(count1 < 2)
										{
											count1++;
										}
										  count2 = 0;
								}
								else
								{
										if(count2 < 2)
										{
											count2++;
										}
										count1 = 0;
								}
								
								if((count1 == 2) ||( count2 == 2 ) )//车离开或者车进入
								{
									
									  MagnentCarState = tempCurState;
									  count1 = 0;
									  count2 = 0;
									 if( MagnentCarState != CarState )
									 {
											if(MagnentCarState == 0)//有车变无车
											{
													CarState = 0;
													UltraCarSate = 0;
												  printf("MagCarFlag: %d\r\n", MagnentCarState); 
													OSFlagPost(flags_event,SendDataStartBit,OS_FLAG_SET,&err);//
													OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);
											}
											else											//无车->有车
											{	
													RCC_Configuration();  
													NVIC_Configuration();
													USART_Configuration();
													GPIO_Configuration();
													EXTI_Configuration();  
													TIM_Configuration();
													GPIO_ON();
												  printf("MagCarFlag: %d\r\n", MagnentCarState);
													OSFlagPost(flags_event,UltrasoundBit,OS_FLAG_SET,&err);//
													OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);
											}
									 }
									 count3 ++;
									 if( count3 == 2 )//复位上传初始状态
									 {
											    CarState = 0;
													RCC_Configuration();  
													NVIC_Configuration();
													USART_Configuration();
													GPIO_Configuration();
													EXTI_Configuration();  
													TIM_Configuration();
													GPIO_ON();
												  printf("MagCarFlag: %d\r\n", MagnentCarState);
													OSFlagPost(flags_event,UltrasoundBit,OS_FLAG_SET,&err);//
													OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);
									 }
										
								}
								EXTI->IMR |= EXTI_Line3;//开放来自线0的中断请求
						}
					  else  //如果地磁有扰动
					  {
							  if(count4 < 2)
								{
									count4++;
								}
								if(count4 == 2)
								{
									printf("Enter MagnetDistur!\r\n");
									MagneDistur = 1;  //地磁扰动标志位置1
									count4 = 0;
							  }	
					 }	
				}
				
				if(( uBits & RtcDectectBit ) != 0 ) //周期唤醒开启磁强中断功能
			  {
					LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X11);
			  }
				
				if(UltrTestCount > 60)   //周期启动超声波检测判断
				{
						UltrTestCount = 0;
						RCC_Configuration();  
						NVIC_Configuration();
						USART_Configuration();
						GPIO_Configuration();
						EXTI_Configuration();  
						TIM_Configuration();
						GPIO_ON();
						OSFlagPost(flags_event,UltrasoundBit,OS_FLAG_SET,&err);
						OSFlagPend(flags_event,SendDataFinishBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);
				}

			 OSFlagPost(flags_event,0xFFFF,OS_FLAG_CLR,&err);  //清空所有信号量
			 OSTaskResume(UltraStart_Pri);//恢复挂起任务
			 MCU_SLEEP();   //对相关外设进行停机模式配置，
			 GPIO_OFF();    //关闭已开的引脚
			 PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI); // 停机模式
			 MPU_IIC_Init(); 
			 USART_Configuration();
			 GPIO_Configuration();
			 
	  }
	
}
