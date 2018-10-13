#ifndef __GPS_H
#define __GPS_H

#include "delay.h"
#include "stm32f10x.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>

#define  ResponsOk           1
#define  ResponsError        2
#define  ResponsOoutTime     3


typedef struct {
  uint8_t Fix_Status;         
  uint8_t Fix_Mode;           
  char UTC_Date[7];       
  char UTC_Time[7];        
  double Latitude;      
  double Longitude;     

} GPS_TypeDef;

void GPS_StructInit(void);
uint8_t SetNMEAOut(void);
uint8_t  GetGpsInformation(void);
uint8_t GPS_ReadFixStatus(void);

#endif
