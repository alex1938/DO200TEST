//超声波控制等函数

#ifndef __ULTRA_H
#define __ULTRA_H

#include "stm32f10x.h"
#include "SYSCONF.h"
#include "calct.h"
#include "delay.h"
#include "ntc.h"
#include "usart.h"



#if ULTRAFREZ == ULTRAFRE75

#define ULTRARANGE 5500
#define ULTRACOUNT 60    //发送波形次数 需除以2
#define BLIND_TIME 1700  //盲区时间
#define MAX_TIME   30000  //最大量程

#elif ULTRAFREZ == ULTRAFRE112

#define ULTRARANGE 2000
#define ULTRACOUNT 40    //发送波形次数 需除以2
#define BLIND_TIME 750  //盲区时间
#define MAX_TIME 12000  //最大量程
#elif ULTRAFREZ == ULTRAFRE200
#define ULTRARANGE 1000
#define ULTRACOUNT 40    //发送波形次数 需除以2
#define BLIND_TIME 370  //盲区时间
#define MAX_TIME  6000 //最大量程

#else
#define ULTRARANGE 2000
#endif

typedef struct 
{

  uint8_t   Echo_Count;     //回波计数
  uint8_t   IRQFlag;       //本次测量结束
  uint16_t  Average_Time; //平均时间
	uint32_t  Echo_Time[10]; //回波时间 单位us
	
} ULTRA_TypeDef;


//开关超声波电源
void Ultra_Power( uint8_t  mode);
	//初始化超声波参数
void ULTRA_StructInit(void);
//启动超声波次数
void UltraTest( uint8_t  number);
//驱动超声波
void ULTRA_StartDetect(void);
//读取超声波高度
void ULTRA_ReadAirHeight(uint16_t *Air_Height);

#endif
