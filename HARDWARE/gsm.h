// GSM 基本驱动AT指令

#ifndef __GSM_H
#define __GSM_H

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include <string.h>

// GSM引脚信息
#define GSM_POWER_GPIO GPIOA
#define GSM_STATUS_GPIO GPIOA
#define GSM_POWER_PORT GPIO_Pin_12
#define GSM_STATUS_PORT GPIO_Pin_11

#define SIM800 1
#define SIM808 2

#define SIM_MOD SIM808

//电源信息
#define POWERON 1  //上电
#define POWEROFF 2 //掉电

//模块信息
#define VALID 1   //有效
#define INVALID 0 //无效

//服务商信息
#define UNKNOWN 0
#define CMOBILE 1
#define CUNICOM 2
#define OTHEROP 3

// return 信息
#define RESPONSEOK 1  //回复正常
#define ERROR_NORSP 0 //错误 无回复
#define ERROR_RSPER 2 //回复错误
#define ERROR_FALL 3  //连接失败
#define ALREADYCON 4  //已经连接

//普通函数返回
#define RETURNOK 1  //返回正常
#define WRONGDATA 2 //错误数据
#define POWERERR 3  //模块开机失败
#define STATUERR 4  // AT检测状态失败
#define SIMERROR 5  // SIM卡检测失败
#define NOAPN 6     //没有配置APN
#define NETERROR 7  //联网失败
#define OPERAERR 8  //运营商查询错误
#define SERVRERR 9  //连接服务器错误
#define SDATAERR 10 //发送数据错误
#define JUSTPWON 11 //模块刚刚开机

//模块当前状态
#define POWEROFSTU 0  //关机状态
#define POWERONSTU 1  //开机状态
#define GETATSTU 2  //AT指令回复
#define SIMRDSTU 3  //SIM卡正常
#define NETRDSTU 4  //网络正常
#define OPERTRSTU 5 //运营商正常


// GSM基本信息
typedef struct {
  uint8_t Module_Power;  //模块电源
  uint8_t Module_Status; //模块当前所处状态
  uint8_t Module_Type;   //模块类型
  uint8_t SIM_Status;    // SIM卡状态
  uint8_t Operator_Info; //运营商信息
  uint8_t GSMNet_Status; // GSM网络状态
} GSM_TypeDef;

uint8_t GSM_ModeOn(void);
uint8_t GSM_ModeOff(void);
void GSM_StructInit(void);
//检查GSM模块电源信息
uint8_t GSM_CheckPower(void);
//检查模块通信状态
uint8_t GSM_ReadModStatus(void);
//检查SIM卡状态
uint8_t GSM_ReadSIMStatus(void);
//检查运营商信息
uint8_t GSM_ReadOPSInfo(void);
//检查GSM网络状态
uint8_t GSM_ReadNetStatus(void);
//标准OK ERROR检查函数
uint8_t GSM_OKERRORCheck(uint8_t time);
//发送AT 命令，检查模块状态
uint8_t GSM_SendAT(void);
//发送AT+CCID查询SIM卡
uint8_t GSM_CheckSIMID(void);
//发送AT+CREG?检查联网状态
uint8_t GSM_CheckNetStatus(void);
//发送AT+COPS 检查运营商状态
uint8_t GSM_CheckOPerator(void);
//模块上电后正常检测
uint8_t GSM_ModuleStatusCheck(void);
//错误处理函数
uint8_t GSM_ErrorHandler(void);
uint8_t GSM_CheckIMEI(void);
uint8_t GSM_CIICR(void);
#endif
