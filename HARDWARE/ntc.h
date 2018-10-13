/*****************************************************************************
Copyright: 2011-2016, Dingtek Company Co., Ltd.
File name: ntc.c
Description:NTC热敏电阻测量函数。
需要热敏电阻B值，25度时电阻值。
测量过程：
1.得到电压值，若有比较电压，同时得到准确电源电压值
2.将电压值转为电阻值
3.将电阻值套入公式，转为温度值。
注意：需DMA函数配合使用

Author: Zitao Wang
Version: 1.0
Date: 2016-05-18
History:
*******************************/

#ifndef __NTC_H
#define __NTC_H

#include "stm32f10x.h"
#include "dma.h"

uint8_t NTC_GetTemperature(float *BackTemperature);

#endif
