// GPRS 联网驱动

#include "gprs.h"

GPRS_TypeDef GPRS_Structure;

uint8_t Send_Buf[200] = {0}; //发送中使用的数组
 u8 gprs_flag = 0;
void GPRS_StructInit(void) {
  GPRS_Structure.GPRSNet_Status = INITIAL;   //网络状态
  GPRS_Structure.Server_Status = DISCONNECT; //服务器状态
  strcpy(GPRS_Structure.Server_IP, "47.90.50.197");    //服务器IP
  strcpy(GPRS_Structure.Server_PORT, "50000");  //服务器端口
  strcpy(GPRS_Structure.Server_Type, "TCP"); //连接类型
  GPRS_Structure.USER_APN = APN_INMOD;       // APN模式
  strcpy(GPRS_Structure.APN, "airtelgprs.com");          // APN信息
  strcpy(GPRS_Structure.USER, "\0");         // APN用户名信息
  strcpy(GPRS_Structure.PASSWORD, "\0");     // APN密码信息
}
//将配置信息打印
char *GPRS_SendAllConf(uint8_t OPT) {
  sprintf((char *)Send_Buf, "Server: %s %s %s\r\n"
                    "APN: %s %s %s\r\n",
          GPRS_Structure.Server_IP, GPRS_Structure.Server_PORT,
          GPRS_Structure.Server_Type, GPRS_Structure.APN, GPRS_Structure.USER,
          GPRS_Structure.PASSWORD);
  if(OPT == 0){
    USART_SendString(USART1,Send_Buf,0);
  }
  return (char *)Send_Buf;
}
//写入信息到内存中
void GPRS_WriteFlashOPt(void) {
  memset(Send_Buf, 0, 200);
  sprintf((char *)Send_Buf, "GPRS;%s;%s;%s;%s;%s;%s;",
          GPRS_Structure.Server_Type, GPRS_Structure.Server_IP,
          GPRS_Structure.Server_PORT, GPRS_Structure.APN, GPRS_Structure.USER,
          GPRS_Structure.PASSWORD);
  FLASH_WriteString(GPRS_OPT_ADDR, (uint16_t *)Send_Buf,strlen((char *)Send_Buf));
}
//从内存中读出GPRS的配置信息
uint8_t GPRS_ReadFlashOPt(void) {
  GPRS_StructInit();
  FLASH_ReadString(GPRS_OPT_ADDR, (uint16_t *)Send_Buf, 200);
  //判断读出正确后再写入
  if (Send_Buf[0] == 'G' && (strstr((char *)Send_Buf, "GPRS;") != NULL)) {
    sscanf((char *)Send_Buf, "GPRS;%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];",
           GPRS_Structure.Server_Type, GPRS_Structure.Server_IP,
           GPRS_Structure.Server_PORT, GPRS_Structure.APN, GPRS_Structure.USER,
           GPRS_Structure.PASSWORD);
		GPRS_SendAllConf(0);
    return 1;
  } else { //没有写入
    GPRS_WriteFlashOPt();
    return 0;
  }
}

//复位网络与服务器状态
uint8_t GPRS_ResetStatus(void) {
  GPRS_Structure.GPRSNet_Status = INITIAL;   //网络状态
  GPRS_Structure.Server_Status = DISCONNECT; //服务器状态
  return RETURNOK;
}

//读取当前GPRS网络状态
uint8_t GPRS_ReadNetStatus(void) { return GPRS_Structure.GPRSNet_Status; }
//读取当前GPRS服务器状态
uint8_t GPRS_ReadServerStatus(void) { return GPRS_Structure.Server_Status; }

//设置APN信息 保证传入参数不超过长度
uint8_t GPRS_SetAPNInfo(char *APNNAME, char *APNUSER, char *APNPASS) {
  if (strlen(APNNAME) > 50 || strlen(APNUSER) > 50 || strlen(APNPASS) > 50) {
    return WRONGDATA;
  }
  GPRS_Structure.USER_APN = APN_USER;
  strcpy(GPRS_Structure.APN, APNNAME);
  strcpy(GPRS_Structure.USER, APNUSER);
  strcpy(GPRS_Structure.PASSWORD, APNPASS);
  GPRS_WriteFlashOPt();
  return RETURNOK;
}

//返回APN信息
void GPRS_ReadAPNInfo(char *APNNAME, char *APNUSER, char *APNPASS) {
  uint8_t i;
  if (GPRS_Structure.USER_APN == APN_INMOD) { //未配置APN，使用内部APN
    i = GSM_ReadOPSInfo();                    //读出运营商代码
    if (i == CMOBILE) {                       //移动APN
      strcpy(GPRS_Structure.APN, "CMNET");
    } else if (i == CUNICOM) { //联通APN
      strcpy(GPRS_Structure.APN, "UNINET");
    }
  }
  strcpy(APNNAME, GPRS_Structure.APN);
  strcpy(APNUSER, GPRS_Structure.USER);
  strcpy(APNPASS, GPRS_Structure.PASSWORD);
}
//设置传输协议
uint8_t GPRS_SetSendProtocol(char *Ser_Type) {
  if ((strcmp(Ser_Type, "TCP") != 0) && (strcmp(Ser_Type, "UDP") != 0)) {
    return WRONGDATA;
  }
  strcpy(GPRS_Structure.Server_Type, Ser_Type); //连接类型
  GPRS_WriteFlashOPt();
  return RETURNOK;
}
//返回传输协议
void GPRS_ReadSendProtocol(char *Ser_Type) {
  strcpy(Ser_Type, GPRS_Structure.Server_Type); //连接类型
}
//设置服务器信息
uint8_t GPRS_SetServerInfo(char *Ser_IP, char *Ser_Port) {
  if (strlen(Ser_IP) > 16 || strlen(Ser_Port) > 5) {
    return WRONGDATA;
  }
  strcpy(GPRS_Structure.Server_IP, Ser_IP);     //服务器IP
  strcpy(GPRS_Structure.Server_PORT, Ser_Port); //服务器端口
  GPRS_WriteFlashOPt();
  return RETURNOK;
}
//返回服务器信息
void GPRS_ReadServerInfo(char *Ser_IP, char *Ser_Port) {
  strcpy(Ser_IP, GPRS_Structure.Server_IP);     //服务器IP
  strcpy(Ser_Port, GPRS_Structure.Server_PORT); //服务器端口
}

/******************************
从这里开始，是发送字符串所需完整函数
******************************/
//设置APN信息
uint8_t GPRS_SetAPN(void) {
  uint16_t i = 0;
  memset(Send_Buf, 0, 200);
  sprintf((char *)Send_Buf, "AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n",GPRS_Structure.APN, GPRS_Structure.USER, GPRS_Structure.PASSWORD);
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, Send_Buf, 0);
    do {
    if (USART_CheckRec("OK")) {
				printf("GPRS_SetAPN ok\r\n");
      return RESPONSEOK;
    }
		else if (USART_CheckRec("ERROR")) { //
				printf("GPRS_SetAPN ERROR\r\n");
      return ERROR_RSPER;
    }
    i++;
    delay_ms(50);
  } while (i < 200); // 10s
				printf("GPRS_SetAPN outtime\r\n");
  return ERROR_NORSP;
}


//发送AT+CIICR 连网
uint8_t GPRS_ConnectNet(void) {
  uint8_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CIICR\r\n", 10);
  i = GSM_OKERRORCheck(200);
  if (i == RESPONSEOK) {
    GPRS_Structure.GPRSNet_Status = GPRSACT;
  }
  return i;
}

//连接服务器
uint8_t GPRS_ConnectServer(void) {
  uint16_t i = 0;
    memset(Send_Buf, 0, 100);
    sprintf((char *)Send_Buf, "AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n",
            GPRS_Structure.Server_Type, GPRS_Structure.Server_IP,GPRS_Structure.Server_PORT);
    USART_ClearBuf(&USART3_Data);
    USART_SendString(USART3, Send_Buf, 0);
		 delay_ms(20);
  do {
//			USART_SendString(USART1,USART3_Data.DATA_RX, 0);
    if (USART_CheckRec("CONNECT")) {
      if (USART_CheckRec("ALREADY CONNECT")) { //已经建立连接
					printf("GPRS_ConnectServer ALREADY CONNECT\r\n");
        return RESPONSEOK;
      }
      if (USART_CheckRec("CONNECT OK")) { //连接正常
				printf("GPRS_ConnectServer CONNECT OK\r\n");
        return RESPONSEOK;
      }
      if (USART_CheckRec("CONNECT FAIL")) { //连接失败
				printf("GPRS_ConnectServer CONNECT FAIL\r\n");
        return ERROR_FALL;
      }
    }
    if (USART_CheckRec("ERROR")) {
			printf("GPRS_ConnectServer ERROR\r\n");
      return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 751); // 10s
	printf("GPRS_ConnectServer outtime\r\n");
  return ERROR_NORSP;
}
//发送数据，必须带长度，不可为0！
uint8_t GPRS_SendData(uint8_t *Data_Send, uint16_t Data_Len) {
  uint16_t i = 0;
  memset(Send_Buf,0,50);
  USART_ClearBuf(&USART3_Data);
  sprintf((char *)Send_Buf, "AT+CIPSEND=%d\r\n", Data_Len);
  USART_SendString(USART3, (uint8_t *)Send_Buf, 0);
  do {
//			USART_SendString(USART1,USART3_Data.DATA_RX, 0);
    if (USART_CheckRec(">")) {
				printf("AT+CIPSEND > ok\r\n");
      break;
    }
    if (USART_CheckRec("ERROR")) {
			printf("AT+CIPSEND > ERROR\r\n");
      return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 645); // 200ms 准备发送
  i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, Data_Send, Data_Len);
	  USART_SendString(USART1, Data_Send, Data_Len);
  do {
//		USART_SendString(USART1,USART3_Data.DATA_RX, 0);
    if (USART_CheckRec("SEND OK")) {
				printf("GPRS_SendData SEND OK\r\n");
      return RESPONSEOK;
    }
    if (USART_CheckRec("ERROR\r\n")) {
				printf("GPRS_SendData ERROR\r\n");
      return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 300); // 
		printf("GPRS_SendData outtime\r\n");
  return ERROR_NORSP;
}

//关闭与服务器的连接
uint8_t GPRS_CloseServer(void) {
  uint8_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CIPCLOSE\r\n", 13);
  do {
    if (USART_CheckRec("CLOSE OK")) {

      return RESPONSEOK;
    }
    if (USART_CheckRec("ERROR")) {
      return ERROR_RSPER;
    }
    i++;
    delay_ms(50);
  } while (i < 100); // 1s
  return ERROR_NORSP;
}


uint8_t GPRS_CloseNet(void) {
  uint16_t i = 0;
  USART_ClearBuf(&USART3_Data);
  USART_SendString(USART3, "AT+CIPSHUT\r\n", 12);
  do {
    if (USART_CheckRec("SHUT OK")) {
				printf("GPRS_CloseNet SHUT OK\r\n");
      return RESPONSEOK;
    }
    if (USART_CheckRec("ERROR")) {
				printf("GPRS_CloseNet ERROR\r\n");
      return ERROR_RSPER;
    }
    i++;
    delay_ms(100);
  } while (i < 651); // 10s
	printf("GPRS_CloseNet outtime\r\n");
  return ERROR_NORSP;
}

//使用指定的GPRS结构发送字符串
// Mode 参数设置发送完成后动作，
uint8_t GPRS_SendString(uint8_t *Data_String, uint16_t len, uint8_t Mode) {
    u8 gprs_count1;
  switch (gprs_flag) { //根据状态不同语句
	      case 0: 
			      GPRS_CloseNet();  
						if( GPRS_SetAPN() == RESPONSEOK)  
					{
						
						  gprs_flag = 1;
						  gprs_count1=0;
					}else{
						
							gprs_flag = 0;
							GPRS_CloseNet();
					}
break;
		    case 1: 
						if( GSM_CheckNetStatus() == RESPONSEOK)  
					{    
						   gprs_flag = 2;
						   gprs_count1=0;
					}else{
						   gprs_flag = 1;
							 gprs_count1++;
//						if(gprs_count1 >= 10){
//						   gprs_flag=0xff;
//						   }
					}
break;	//状态未知，先关闭
		    case 2: 
						 if( GSM_CIICR() == RESPONSEOK) 
							{
									gprs_flag = 3;
									gprs_count1=0;
							}else{
									gprs_flag = 0;
									GPRS_CloseNet();  //关闭网络
							}
break;	//状态未知，先关闭
			  case 3: 
//							GPRS_CloseNet();  //关闭网络
							if( GPRS_ConnectServer() == RESPONSEOK)  // 连接服务器，失败分两种，一种服务器连接失败，一种模块返回错误
							{
								  gprs_flag = 4;
									gprs_count1=0;
							}else {
							  	gprs_flag = 3;
									GPRS_CloseNet(); 
//								 	GPRS_CloseServer();
								
							}
break;	//状态未知，先关闭
			 case 4: 
							if( GPRS_SendData(Data_String,len) == RESPONSEOK)  // 发送数据
							{
								
								gprs_flag = 5;
								gprs_count1=0;
							}else{
								  gprs_flag = 0;
									GPRS_CloseNet();
							}
break;	//状态未知，先关闭
			case 5: 
           if(Mode==SEND_KEEP){
						   delay_ms(1000);
						   SetBasicInformat(USART3_Data.DATA_RX,500);
						   PROTOCOL_InforProcess(USART3_Data.DATA_RX,3,0);
				       delay_ms(500);
					 }else if(Mode==SEND_SHUT){
				
					 	GPRS_CloseNet();
					 }else{
				
					 }
					 	 	gprs_flag = 0xff;
break;	//状态未知，先关闭
  default:
    break;
  }
	return gprs_flag;
}

// GPRS网络错误处理
