// GPRS 联网驱动

#ifndef __GPRS_H
#define __GPRS_H

#include "delay.h"
#include "gsm.h"
#include "stm32f10x.h"
#include "usart.h"
#include <string.h>

#define GPRS_OPT_ADDR 0x08000000 + 0x400 * 102 // 102k位置

//连接信息
#define DISCONNECT 0
#define CONNECTING 1
#define CONNECTFAIL 2
#define CONNECTED 3
#define DISCONNECTING 4

//联网状态信息
#define INITIAL 1  //初始化，需配置APN
#define IPSTART 2  //配置好APN，可联网
#define GPRSACT 3  //连上网，需先查IP地址
#define IPSTATUS 4 //可连服务器
#define CONECTOK 5 //连接上服务器
#define NETUNKON 6 //未知信息

//用户配置APN关键字
#define APN_INMOD 1 //使用模块内部APN
#define APN_USER 2  //使用用户配置的APN

#define SEND_KEEP 1  //保持连接
#define SEND_CLOSE 2 //关闭服务器连接
#define SEND_SHUT 3  //关闭网络

typedef struct {
  uint8_t GPRSNet_Status; // GPRS网络状态
  uint8_t Server_Status;  //服务器状态
  char Server_IP[16];     //服务器IP
  char Server_PORT[6];    //服务器端口
  char Server_Type[4];    //连接类型
  uint8_t USER_APN;       //用户配置APN
  char APN[50];           // APN信息
  char USER[50];          // APN用户名信息
  char PASSWORD[50];      // APN密码信息
} GPRS_TypeDef;

//结构体初始化
void GPRS_StructInit(void);
//将配置信息打印
char *GPRS_SendAllConf(uint8_t OPT);
//写入信息到内存中
void GPRS_WriteFlashOPt(void);
//从内存中读出GPRS的配置信息
uint8_t GPRS_ReadFlashOPt(void);
//复位网络与服务器状态
uint8_t GPRS_ResetStatus(void);
//读取当前GPRS网络状态
uint8_t GPRS_ReadNetStatus(void);
//读取当前GPRS服务器状态
uint8_t GPRS_ReadServerStatus(void);

//设置APN信息 保证传入参数不超过长度
uint8_t GPRS_SetAPNInfo(char *APNNAME, char *APNUSER, char *APNPASS);
//返回APN信息
void GPRS_ReadAPNInfo(char *APNNAME, char *APNUSER, char *APNPASS);
//设置传输协议
uint8_t GPRS_SetSendProtocol(char *Ser_Type);
//返回传输协议
void GPRS_ReadSendProtocol(char *Ser_Type);
//设置服务器信息
uint8_t GPRS_SetServerInfo(char *Ser_IP, char *Ser_Port);
//返回服务器信息
void GPRS_ReadServerInfo(char *Ser_IP, char *Ser_Port);

//关闭与服务器的连接
uint8_t GPRS_CloseServer(void);
//关闭网络连接
uint8_t GPRS_CloseNet(void);

//发送数据
uint8_t GPRS_SendString(uint8_t *Data_String, uint16_t len, uint8_t Mode);
// GPRS网络错误处理
uint8_t GPRS_ErrorHandler(void);

#endif
