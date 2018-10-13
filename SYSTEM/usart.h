#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
#include "string.h"
#include "protocol.h"
#include "os_cpu.h"
#include "includes.h"	
#include "os_cfg.h"
#define USART_REC_LEN 500 //定义最大接收字节数 500

typedef struct 
{
  uint8_t DATA_RX[USART_REC_LEN];
  uint16_t RX_STA;
} USART_DataType;

extern USART_DataType USART1_Data;
extern USART_DataType USART3_Data;

void USART_Configuration(void);
void USART_SendString(USART_TypeDef *USARTx, uint8_t *Data, uint16_t Long);
uint8_t USART_CheckRec(uint8_t *ptr);
void USART_ClearBuf(USART_DataType *USART_Data);
#endif
