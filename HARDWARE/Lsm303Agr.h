#ifndef __LIS3DH_H
#define __LIS3DH_H	    
#include "stm32f10x.h"

#include "delay.h"
#include "usart.h"
u8 Lsm303Agr_Init(void);
u8 Lis3dhReadInterrupt(u8 data);
u8 Lis3dhReadIoStatus(void);
#define UltrasoundBit             0x01  //超声波信号量集过滤器，请求第一位信号
#define ExtIntBit                 0x02  //信号量集第一位
#define RtcDectectBit             0x04	//信号量集第二位
#define SendDataStartBit          0x08  //对信号量集中的第3位进行操作，

#define SendDataFinishBit         0x10   //信号量集第四位

#define DirectSendBit             0x20   //信号量集第五位
#define BeginRepeatMagnent        0x40    //地磁检测开始第六位
#define Ext3IntBit                0x80    //地磁检测开始第六位
#endif
