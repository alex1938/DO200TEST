// GSM 基本驱动AT指令

#include "gsm.h"

GSM_TypeDef GSM_Structure;
extern PROTOCOL_TypeDef PROTOCOL_Structure;
uint8_t GSM_NetErrorCount = 0;

uint8_t GSM_ModeOn(void) {
  uint8_t i = 0;
  if (GPIO_ReadInputDataBit(GSM_STATUS_GPIO, GSM_STATUS_PORT) == 1) {
    GSM_Structure.Module_Power = POWERON;
    GSM_Structure.Module_Status = POWERONSTU;
    return POWERON;
  }
  GPIO_SetBits(GSM_POWER_GPIO, GSM_POWER_PORT);
  delay_ms(2000);
  
  GPIO_ResetBits(GSM_POWER_GPIO, GSM_POWER_PORT);
  delay_ms(300);
  while (i < 20) {
    if (GPIO_ReadInputDataBit(GSM_STATUS_GPIO, GSM_STATUS_PORT) == 1) {
      GSM_Structure.Module_Power = POWERON;
      GSM_Structure.Module_Status = POWERONSTU;
      return POWERON;
    }
    i++;
    delay_ms(300);
  }
  GSM_Structure.Module_Power = POWEROFF;
  GSM_Structure.Module_Status = POWEROFSTU;
  return POWEROFF;
}

uint8_t GSM_ModeOff(void) {
  uint8_t i = 0;
  if (GPIO_ReadInputDataBit(GSM_STATUS_GPIO, GSM_STATUS_PORT) == 0) {
    GSM_Structure.Module_Power = POWEROFF;
    GSM_Structure.Module_Status = POWEROFSTU;
    return POWEROFF;
  }
  GPIO_SetBits(GSM_POWER_GPIO, GSM_POWER_PORT);
  delay_ms(1000);
  delay_ms(700);
  GPIO_ResetBits(GSM_POWER_GPIO, GSM_POWER_PORT);
  delay_ms(200);
  while (i < 20) {
    if (GPIO_ReadInputDataBit(GSM_STATUS_GPIO, GSM_STATUS_PORT) == 0) {
      GSM_Structure.Module_Power = POWEROFF;
      GSM_Structure.Module_Status = POWEROFSTU;
      GSM_Structure.SIM_Status = INVALID;
      GSM_Structure.Operator_Info = UNKNOWN;
      GSM_Structure.GSMNet_Status = INVALID;
      return POWEROFF;
    }
    i++;
    delay_ms(300);
  }
  GSM_Structure.Module_Power = POWERON;
  GSM_Structure.Module_Status = POWERONSTU;
  return POWERON;
}

void GSM_StructInit(void) {
  GSM_Structure.Module_Power = POWEROFF;    //模块电源
  GSM_Structure.Module_Status = POWEROFSTU; //模块状态
  GSM_Structure.Module_Type = SIM_MOD;      //模块型号
  GSM_Structure.SIM_Status = INVALID;       // SIM卡状态
  GSM_Structure.Operator_Info = UNKNOWN;    //运营商信息
  GSM_Structure.GSMNet_Status = INVALID;
}

//检查GSM模块电源信息
uint8_t GSM_CheckPower(void) {
  if (GPIO_ReadInputDataBit(GSM_STATUS_GPIO, GSM_STATUS_PORT) == 1) {
    GSM_Structure.Module_Power = POWERON;
    GSM_Structure.Module_Status = POWERONSTU;
    return POWERON;
  } else {
    GSM_Structure.Module_Power = POWEROFF;
    GSM_Structure.Module_Status = POWEROFSTU;
    GSM_Structure.SIM_Status = INVALID;
    GSM_Structure.Operator_Info = UNKNOWN;
    GSM_Structure.GSMNet_Status = INVALID;
    return POWEROFF;
  }
}

//检查模块通信状态
uint8_t GSM_ReadModStatus(void) { return GSM_Structure.Module_Status; }

//检查SIM卡状态
uint8_t GSM_ReadSIMStatus(void) { return GSM_Structure.SIM_Status; }

//检查运营商信息
uint8_t GSM_ReadOPSInfo(void) { return GSM_Structure.Operator_Info; }

//检查GSM网络状态
uint8_t GSM_ReadNetStatus(void) { return GSM_Structure.GSMNet_Status; }

//标准OK ERROR检查函数
uint8_t GSM_OKERRORCheck(uint8_t time){
  uint8_t i = 0;
  do {
    if (USART_CheckRec("OK")) {
      return RESPONSEOK;
    }
    if (USART_CheckRec("ERROR")) {
      return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < time);
  return ERROR_NORSP;
}


//发送AT 命令，检查模块状态
uint8_t GSM_SendAT(void) {
  uint8_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT\r\n", 4);
  do {
    if (USART_CheckRec("OK")) {
      GSM_Structure.Module_Status = GETATSTU;
      return RESPONSEOK;
    }
    i++;
    delay_ms(100);
  } while (i < 20);
  GSM_Structure.Module_Status = POWERONSTU;
  return ERROR_NORSP;
}

//发送AT+CGSN 查询IMEI
uint8_t GSM_CheckIMEI(void) {
  uint8_t i = 0;
	uint8_t *ptr;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CGSN\r\n", 9);
  do {

    if (USART_CheckRec("OK")) {
			  if((ptr = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "AT+CGSN"))!=NULL){
					    ptr+=10;
					    sscanf((char *)ptr, "%[^\r\n]",PROTOCOL_Structure.Imei); 
					 USART_SendString(USART1,(uint8_t*)PROTOCOL_Structure.Imei,0);
					printf("\r\n");
				}
 return RESPONSEOK;
    }else if (USART_CheckRec("ERROR")) {
     return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 50); //
  return ERROR_NORSP;
}

//发送AT+CCID查询SIM卡
uint8_t GSM_CheckSIMID(void) {
  uint8_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CCID\r\n", 9);
  do {
    if (USART_CheckRec("ERROR")) {
      GSM_Structure.SIM_Status = INVALID;
      return ERROR_RSPER;
    }
    if (strlen((char *)USART3_Data.DATA_RX) > 20) {
      return RESPONSEOK;
    }
    i++;
    delay_ms(50);
  } while (i < 20); // 1000ms
  return ERROR_NORSP;
}

//发送AT+CREG?检查联网状态
uint8_t GSM_CheckNetStatus(void) {
  uint16_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CREG?\r\n", 10);
//	 USART_SendString(USART1, "AT+CREG?\r\n", 10);
  do {
//		  USART_SendString(USART1,USART3_Data.DATA_RX,0);
    if (USART_CheckRec("+CREG:")) {                      //回复正常
      if (USART_CheckRec(",1") || USART_CheckRec(",5")) { //已注册上网络
	
        return RESPONSEOK;
      } else {
           return ERROR_RSPER;
      }
    }
    if (USART_CheckRec("ERROR")) { //回复错误

          return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 200); // 10s
		printf("GSM_CheckNetStatus outtime\r\n");
  return ERROR_NORSP;
}


//发送AT+COPS 检查运营商状态
uint8_t GSM_CheckOPerator(void) {
  uint8_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+COPS?\r\n", 10);
  do {
    if (USART_CheckRec("+COPS:")) {
      if (USART_CheckRec("CHINA MOBILE")) {
        GSM_Structure.Operator_Info = CMOBILE;
      } else if (USART_CheckRec("CHN-UNICOM")) {
        GSM_Structure.Operator_Info = CUNICOM;
      } else { //非国内运营商，先判断是否连上网络
        if (GSM_Structure.GSMNet_Status == VALID) {
          GSM_Structure.Operator_Info = OTHEROP;
        } else {
          GSM_Structure.Operator_Info = UNKNOWN;
          return ERROR_RSPER;
        }
      }
      GSM_Structure.Module_Status = OPERTRSTU;
      return RESPONSEOK;
    }
    if (USART_CheckRec("ERROR")) {
      GSM_Structure.Operator_Info = UNKNOWN;
      return ERROR_RSPER;
    }
    i++;
    delay_ms(10);
  } while (i < 50); // 500ms
  GSM_Structure.Operator_Info = UNKNOWN;
  return ERROR_NORSP;
}

//模块状态检测
uint8_t GSM_ModuleStatusCheck(void) {
  uint8_t i = 0;
  uint8_t Error_Count = 0;
  uint8_t Error_Code;
  while (Error_Count < 5) {
    switch (GSM_Structure.Module_Status) {
    case POWEROFSTU: //关机状态
      Error_Code = GSM_ModeOn();
			printf("GSM_ModeOn %d\r\n",Error_Code);
      if(Error_Code == POWEROFF){
        Error_Code = 5;
        break;
      }
      return JUSTPWON;
    case POWERONSTU: //开机状态
      i = 0;
      for (;;) {
        Error_Code = GSM_SendAT();
				printf("GSM_SendAT %d\r\n",Error_Code);
        if (Error_Code == RESPONSEOK) { //得到正常回复
          GSM_Structure.Module_Status = GETATSTU;
          break;
        }
        i++;
        if (i == 5) { //回复错误
          goto OUTSW; //跳出switch
        }
      }
    case GETATSTU: // AT指令回复
      i = 0;
      for (;;) {
        Error_Code = GSM_CheckSIMID();
					printf("GSM_CheckSIMID %d\r\n",Error_Code);
        if (Error_Code == RESPONSEOK) { //正常
          GSM_Structure.Module_Status = SIMRDSTU;
          break;
        }
        i++;
        if (i == 5) {
          goto OUTSW; //跳出switch
        }
        delay_ms(100);
      }
    case SIMRDSTU: // SIM卡正常
      i = 0;
      for (;;) {
        Error_Code = GSM_CheckNetStatus();
				printf("GSM_CheckNetStatus %d\r\n",Error_Code);
        if (Error_Code == RESPONSEOK) { //正常
          GSM_Structure.Module_Status = NETRDSTU;
          break;
        }
        i++;
        if (i == 5) {
          goto OUTSW; //跳出switch
        }
        delay_ms(100);
      }
    case NETRDSTU: //网络正常
      i = 0;
      for (;;) {
        Error_Code = GSM_CheckOPerator();
					printf("GSM_CheckOPerator %d\r\n",Error_Code);
        if (Error_Code == RESPONSEOK) { //正常
          GSM_Structure.Module_Status = NETRDSTU;
          break;
        }
        i++;
        if (i == 5) {
          goto OUTSW; //跳出switch
        }
        delay_ms(100);
      }
    case OPERTRSTU: //都正常
      GSM_Structure.Module_Status = SIMRDSTU;
      GSM_NetErrorCount = 0;
      return RETURNOK;
    default:
      break;
    }
  OUTSW:
    Error_Count++;
  }
  GSM_ErrorHandler();
  return ERROR_NORSP;
}

uint8_t GSM_CIICR(void){
  uint16_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CIICR\r\n", 10);
  do {
    if (USART_CheckRec("OK")) {
			printf("AT+CIICR ok \r\n");
      return RESPONSEOK;
    }
		else if (USART_CheckRec("ERROR")) { //
				printf("AT+CIICR erorr \r\n");
      return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 851); // 100s
  return ERROR_NORSP;
}

//GSM错误处理函数
uint8_t GSM_ErrorHandler(void) {
  switch (GSM_Structure.Module_Status) {
  case POWEROFSTU: //关机状态
    printf("\r\nHARDWARE POWER ERROR\r\n\r\n");
    break;
  case POWERONSTU: //开机状态
    printf("\r\nHARDWARE COMMUNICATE ERROR\r\n\r\n");
    GSM_Structure.Module_Status = POWEROFSTU;
    break;
  case GETATSTU:   // AT指令回复
    printf("\r\nSIM CARD ERROR\r\n\r\n");
    GSM_Structure.Module_Status = POWERONSTU;
    GSM_NetErrorCount++;
    if(GSM_NetErrorCount > 5){
      GSM_NetErrorCount = 0;
      GSM_ModeOff();
      GSM_Structure.Module_Status = POWEROFSTU;
    }
    break;
  case SIMRDSTU:   // SIM卡正常
    printf("\r\nGSM NET ERROR\r\n\r\n");
    GSM_Structure.Module_Status = GETATSTU;
    GSM_NetErrorCount++;
    if(GSM_NetErrorCount > 5){
      GSM_NetErrorCount = 0;
      GSM_ModeOff();
      GSM_Structure.Module_Status = POWEROFSTU;
    }
    break;
  case NETRDSTU:   //网络正常
    printf("\r\nOPERATOR CHECK ERROR\r\n\r\n");
    GSM_Structure.Module_Status = SIMRDSTU;
    break;
  case OPERTRSTU:  //运营商正常
    GSM_NetErrorCount = 0;
    break;
  default:
    break;
  }
  return RETURNOK;
}
