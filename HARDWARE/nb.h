#ifndef __NB_H
#define __NB_H
#include "stm32f10x.h"
#include "usart.h"
#include "calct.h"
#include <time.h>
//#define DEBUG 1         //����ģʽ

#define ERROR_NORSP    0        //�޻ظ�
#define RESPONSEOK     1        //�ظ���ȷ
#define ERROR_RSPER    2        //�ظ�����
#define ERROR_FALL     3        //ʧ��
#define ALREADYCON     4        //�Ѿ�����
#define DownConfirm    0x10     //�Ѿ�����
#define DownConfig     0x20     //�Ѿ�����
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

 //��ʼ��NB������Ϣ
void NB_StructInit(void);
//д�������Ϣ
uint8_t NB_WriteBasicOPt(void);

//����������Ϣ
uint8_t NB_ReadBasicOPt(void);
//����NB������Ϣ
void NB_SendBASIC( uint8_t OPT ) ;
//����NBģ���Դ
void NB_Power( uint8_t  mode);
//��ȡIMEI
uint8_t NB_CGSN(void) ;
//��ȡIMSI
uint8_t NB_CIMI(void);
//ע������
uint8_t NB_ReadCGATT(void) ;
//��ѯ�ź�����
uint8_t NB_CSQ(void) ;
//��������
uint8_t NB_CEREG(void) ;
//AT+NRB����ģ��
uint8_t NB_NRB(void);
// ����IP��ַ
u8 NB_NCDP(char *ip_addr) ;
//���÷��ͻ���
uint8_t NB_NSMI(uint8_t mode);
//���ý����Ƿ����
uint8_t NB_NNMI(uint8_t mode);
//��������
uint8_t NB_NMGS(u16 lenth,char * data1) ;
//��ȡNB����״̬
uint8_t NB_GetNUESTATS ( uint8_t* ECL, uint16_t* PCI, float* RSRP, float* SNR)  ;
//��ȡ��վʱ��
uint8_t NB_GetCCLK(uint32_t* TimesTamp) ;
//����APN
uint8_t NB_CGDCONT(uint8_t* apn) ;
//��ȡAPN
uint8_t NB_GetCGDCONT(uint8_t* APN) ;
//����豸״̬
uint8_t CheckNBModule(void);
//NBģ�������״̬
uint8_t GSM_ErrorHandler(void) ;
//��ȡģ�鱣��ĵ�ַ���Ա�ģ�鱣�����Ϣ�Ƿ�����õ�IP��ַһ��
uint8_t NB_GetNCDP(uint8_t* IP) ;
//��ȡ��������
uint8_t NB_NMGR(void);
//��ȡ�������ݵĴ���
uint8_t NB_NQMGR( uint8_t *NQMGR ) ;
//��������
uint8_t NB_SendNMGS( uint8_t* Data ,uint16_t Long) ;
uint8_t SendMassege(uint8_t* Massege ,uint16_t MassegeLong);
uint8_t ReceiveMassege(void);
#endif
