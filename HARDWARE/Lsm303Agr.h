#ifndef __LIS3DH_H
#define __LIS3DH_H	    
#include "stm32f10x.h"

#include "delay.h"
#include "usart.h"
u8 Lsm303Agr_Init(void);
u8 Lis3dhReadInterrupt(u8 data);
u8 Lis3dhReadIoStatus(void);
#define UltrasoundBit             0x01  //�������ź������������������һλ�ź�
#define ExtIntBit                 0x02  //�ź�������һλ
#define RtcDectectBit             0x04	//�ź������ڶ�λ
#define SendDataStartBit          0x08  //���ź������еĵ�3λ���в�����

#define SendDataFinishBit         0x10   //�ź���������λ

#define DirectSendBit             0x20   //�ź���������λ
#define BeginRepeatMagnent        0x40    //�شż�⿪ʼ����λ
#define Ext3IntBit                0x80    //�شż�⿪ʼ����λ
#endif
