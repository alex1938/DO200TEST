#ifndef __NB_H
#define __NB_H
#include "stm32f10x.h"
#include "usart.h"
#include "calct.h"
#include <time.h>
//#define DEBUG 1         //调试模式

#define ERROR_NORSP    0        //无回复
#define RESPONSEOK     1        //回复正确
#define ERROR_RSPER    2        //回复错误
#define ERROR_FALL     3        //失败
#define ALREADYCON     4        //已经连接
#define DownConfirm    0x10     //已经连接
#define DownConfig     0x20     //已经连接
#define NB_ADDR 0x08000000 + 1024 * 121

typedef struct 
{ 
  uint8_t  IMEI[20];   
  uint8_t  IP[20];
	uint8_t  PORT[6]; 
	uint8_t  APN[50];
	uint8_t  Status;
	uint8_t  ECL;
	uint16_t PCI;
	float    RSRP;
	float    SNR;
	uint32_t TimesTamp;
	
} NB_TypeDef;

 //初始化NB配置信息
void NB_StructInit(void);
//写入基本信息
uint8_t NB_WriteBasicOPt(void);

//读出基本信息
uint8_t NB_ReadBasicOPt(void);
//发送NB配置信息
void NB_SendBASIC( uint8_t OPT ) ;
//开关NB模块电源
void NB_Power( uint8_t  mode);
//读取IMEI
uint8_t NB_CGSN(void) ;
//读取IMSI
uint8_t NB_CIMI(void);
//注册网络
uint8_t NB_ReadCGATT(void) ;
//查询信号质量
uint8_t NB_CSQ(void) ;
//附着网络
uint8_t NB_CEREG(void) ;
//AT+NRB重启模块
uint8_t NB_NRB(void);
// 设置IP地址
u8 NB_NCDP(char *ip_addr) ;
//设置发送回显
uint8_t NB_NSMI(uint8_t mode);
//设置接收是否回显
uint8_t NB_NNMI(uint8_t mode);
//发送数据
uint8_t NB_NMGS(u16 lenth,char * data1) ;
//读取NB网络状态
uint8_t NB_GetNUESTATS ( uint8_t* ECL, uint16_t* PCI, float* RSRP, float* SNR)  ;
//获取基站时间
uint8_t NB_GetCCLK(uint32_t* TimesTamp) ;
//设置APN
uint8_t NB_CGDCONT(uint8_t* apn) ;
//读取APN
uint8_t NB_GetCGDCONT(uint8_t* APN) ;
//检测设备状态
uint8_t CheckNBModule(void);
//NB模块和网络状态
uint8_t GSM_ErrorHandler(void) ;
//获取模块保存的地址并对比模块保存的信息是否和配置的IP地址一致
uint8_t NB_GetNCDP(uint8_t* IP) ;
//获取接收数据
uint8_t NB_NMGR(void);
//获取接收数据的次数
uint8_t NB_NQMGR( uint8_t *NQMGR ) ;
//发送数据
uint8_t NB_SendNMGS( uint8_t* Data ,uint16_t Long) ;
uint8_t SendMassege(uint8_t* Massege ,uint16_t MassegeLong);
uint8_t ReceiveMassege(void);
#endif
