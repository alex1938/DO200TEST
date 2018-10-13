/*****************************************************************************
Copyright:  Dingtek Company Co., Ltd.
File name: lora.c
Description:LoRa module interface。

Author: Jiabinly
Version: 1.0
Date: 2018-07-31
History:
*******************************/

#ifndef __LORA_H
#define __LORA_H

#include "stm32f10x.h"

uint8_t  LoRa_Mode(void);
uint8_t  LoRaotaajion(void);
uint8_t  LoRa_ruixingSend(uint8_t *Data);
uint8_t  LoRasleep(void);
uint8_t  LoRaJDC(void);

#endif
