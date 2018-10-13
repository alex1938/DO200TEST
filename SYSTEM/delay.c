#include "delay.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					// 
#endif


static u8  fac_us=0;							//us延时倍乘数		   
static u16 fac_ms=0;							//ms延时倍乘数	

	
#if SYSTEM_SUPPORT_OS							//是否使用os
//支持ucos II
#ifdef 	OS_CRITICAL_METHOD						//		支持ucosII	
#define delay_osrunning		OSRunning			//os是否运行标记，1运行，0未运行
#define delay_ostickspersec	OS_TICKS_PER_SEC	//os时钟节拍，每秒的调度次数
#define delay_osintnesting 	OSIntNesting		//
#endif

//us级延时时，关闭任务调度（防止打断us级延时）
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   				//
	OS_ERR err; 
	OSSchedLock(&err);							//
#else											//使用UCOSII
	OSSchedLock();								//UCOSII方式，禁止调度
#endif
}

void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   				//
	OS_ERR err; 
	OSSchedUnlock(&err);						//
#else											//使用UCOSII
	OSSchedUnlock();							//UCOSII方式，恢复调度
#endif
}

//调用os自带的延时函数延时
//ticks:延时的节拍数
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//
#else
	OSTimeDly(ticks);	
#endif	//UCOSII延时
}

//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{	
                                              // Tell uC/OS-II that we are leaving the ISR 
	if(delay_osrunning==1)						//OS运行标志位
	{
		OSIntEnter();							//进入中断
		OSTimeTick();       					//调用ucos的时钟服务函数            
		OSIntExit();       	 					//触发任务切换软中断
	}
}


#endif


//初始化延时函数
//当使用os的时候，此函数会初始化os的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK系统时钟
void delay_init()
{
#if SYSTEM_SUPPORT_OS  							//是否使用os
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;				//系统时钟的1/8  （10^-6÷[1/(SystemCoreClock/8)]）
#if SYSTEM_SUPPORT_OS  							//是否使用os.
	reload=SystemCoreClock/8000000;				//每秒的计数次数，单位k   
	reload*=1000000/delay_ostickspersec;		//根据delay_ostickspersec设定溢出时间
	                                            //reload为24为的寄存器,最大值:16777216,约为1.86s	
	fac_ms=1000/delay_ostickspersec;			//os延时的最少单位  

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//每1/delay_ostickspersec秒中断一次
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    

#else
	fac_ms=(u16)fac_us*1000;					//非OS下,每个ms需要systick的时钟数 
#endif
}								    

#if SYSTEM_SUPPORT_OS  							//是否使用os
//延时nus
//nus要延时的us数		    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD的值 
	ticks=nus*fac_us; 							//需要的节拍数	 
	tcnt=0;
	delay_osschedlock();						//禁止os调度打断us延时
	told=SysTick->VAL; 
while(1)	//刚进入的计数器值
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;		//SYSTICK递减计数器
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//超时或等于退出
		}  
	};
	delay_osschedunlock();						//恢复os调度								    
}
//延时nms
//nms要延时的ms数	
void delay_ms(u16 nms)
{	
	if(delay_osrunning && delay_osintnesting==0)	//os运行并且不再中断里面（中断里面不能任务调度）   
	{		 
		if(nms>=fac_ms)							//延时时间大于os的最小周期
		{ 
   			delay_ostimedly(nms/fac_ms);		//OS延时
		}
		nms%=fac_ms;							// 
	}
	delay_us((u32)(nms*1000));					//普通延时
}
#else //普通延时
//延时nus
//nus要延时的us数	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待计数
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数
	SysTick->VAL =0X00;      					 //清空计数 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器，最大延时：//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK为hz，对于72M，nms<=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载
	SysTick->VAL =0x00;							//清空计数
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达 
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数
	SysTick->VAL =0X00;       					//清空计数	  	    
} 
#endif 














