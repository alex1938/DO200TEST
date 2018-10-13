#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "calct.h"
#include "dma.h"
#include "delay.h"
#include "flash.h"
#include "stm32f10x.h"
#include "ultra.h"
#include "usart.h"
#include "led.h"
#include "nb.h"
#include "lora.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gps.h"
#include "os_cpu.h"
#include "power.h"


#define BASIC_OPTION_ADDR 0x08000000 + 1024 * 122 
  
#define   HEXTYPE          0
#define   ASCIITYPE        1
#define   ALLCHK           0        //都检查
#define   NOCKSCHK         0x02 //不检查校验

#define SRCUART            0 //串口

typedef struct {
	
	uint8_t  HisDataBuf[12][200];   // 历史数据缓存
	uint8_t  NowDataBuf[2][200];    //  本次上报数据
	uint8_t  DeviceMode;            //设备模式
	uint8_t  DataType;              //数据类型
	uint8_t  HisData;               //历史数据
	uint16_t Height;                //高度
	int16_t  MagX;                  //X-磁强
	int16_t  MagY;                  //Y-磁强
	int16_t  MagZ;                  //Z-磁强
	uint16_t FRMN;                  //帧计数
	uint16_t TestTimes;             //检测时间
	uint16_t UpdateTime;            //更新时间
	uint16_t Battery;               //电量报警值 ,百分比
	int XXTEAKey[4];                //加密算法盐值
	
} PROTOCOL_TypeDef;


//typedef struct 
//{
//	uint16_t FrameCount;  
//	uint8_t ParkingStatus;
//	uint8_t  UltraStatus;
//	uint16_t Height;
//	uint8_t  MagStatus;
//	uint16_t MagX;
//	uint16_t MagY;
//	uint16_t MagZ;
//	uint32_t magnet;
//	uint8_t PowerStatus;
//	uint16_t FRMN;
//}SENDDATA_TypeDef;


void PROTOCOL_StructInit(void);

//读出上传时间
uint16_t PROTOCOL_ReadUPTime(void);
//设置上传时间
void PROTOCOL_WriteUPTime(uint16_t UPTime);
//读取检测时间
uint16_t PROTOCOL_ReadUTTimes(void);
//将基本配置信息写入FLASH中
uint8_t PROTOCOL_WriteBasicOPt(void);
//从FLASH中读出基本配置的内容
uint8_t PROTOCOL_ReadBasicOPt(void);
//待定
void SendDate_StructInit(void);
//处理接收函数
uint8_t  dealdata(uint8_t* ptr1, uint8_t type);
//区分不同类型指令
void SetBasicInformat(uint8_t  *arr ,uint16_t  len);
//发送数据
uint8_t PROTOCOL_AlarmData(void);
//校验
uint8_t PROTOCOL_CalcCheckSum(uint8_t *Data_In, uint16_t Data_Len);
//校验
uint8_t PROTOCOL_CheckSumCheck(uint8_t *Data_Check);
//接收处理
uint8_t PROTOCOL_InforProcessSet(uint8_t *Set_Data, uint8_t Source);
//接收处理
uint8_t PROTOCOL_InforProcessGPRS(char *G_Data, uint8_t Source);
//接收处理
uint8_t *PROTOCOL_InforProcess(uint8_t *Protocol_Data, uint8_t Para,uint8_t Source);

#endif


