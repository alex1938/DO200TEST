#include "nb.h"
 NB_TypeDef  NB;
 extern PROTOCOL_TypeDef   PROTOCOL_Structure;
 uint8_t NB_Buf[200];
 
 
 //初始化NB配置信息
void NB_StructInit(void)
{
		strcpy((char *)NB.IMEI, "123456789012345"); 
		strcpy((char *)NB.APN, "CTNB"); 
		strcpy((char *)NB.PORT, "5683"); 
		strcpy((char *)NB.IP, "117.60.157.137"); 
		NB.Status = 0;
}
 
 
//写入基本信息
uint8_t NB_WriteBasicOPt(void) 
{
		memset(NB_Buf,0,200);
		sprintf((char *)NB_Buf, "N:%s:%s:%s:%s:",NB.IMEI,NB.IP,NB.PORT,NB.APN);
						 FLASH_WriteString(NB_ADDR, (uint16_t *)NB_Buf,strlen((char *)NB_Buf));   
	
		return RESPONSEOK;
}

//读出基本信息
uint8_t NB_ReadBasicOPt(void) 
{
    NB_StructInit(); 
	  memset(NB_Buf,0,200);
    FLASH_ReadString(NB_ADDR, (uint16_t *)NB_Buf, 100);
		if (NB_Buf[0] == 'N') 
		{ 
				sscanf((char *)NB_Buf, "N:%[^:]:%[^:]:%[^:]:%[^:]:",NB.IMEI,NB.IP,NB.PORT,NB.APN);
				NB_SendBASIC(0);
				return RESPONSEOK;
		} else	
		{          
				NB_WriteBasicOPt(); 
				return ERROR_RSPER;
		}
}

//发送NB配置信息
void NB_SendBASIC( uint8_t OPT ) 
{
		 memset(NB_Buf,0,200);
     sprintf((char *)NB_Buf,"NB_IMEI: %s\r\nNB_IP: %s\r\nNB_PORT: %s\r\nNB_APN: %s\r\n",NB.IMEI,NB.IP,NB.PORT,NB.APN);
     if (OPT == 0) 
		 {
        USART_SendString(USART1, NB_Buf, 0);
     } 
}

/**********************
*   function：Enable or DisAble NB pwer

*   input：  ENABLE     //  Enable NB
*            DISABLE     //  Disable NB
*   Output： null
**********************/
void NB_Power( uint8_t  mode)
{
	

     if( mode )
     {	  
			 if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_10) != 1)
			 {
					 GPIO_SetBits(GPIOC, GPIO_Pin_10);
					 delay_ms(10);
			 }
		 }else
     {
			   GPIO_ResetBits(GPIOC, GPIO_Pin_10);
	       delay_ms(10);
		 }
	 
}

//读取IMEI
uint8_t NB_CGSN(void) 
{
		uint8_t i = 0;
		uint8_t *pri;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+CGSN=1\r\n",0);
	
	  #ifdef DEBUG
	
	     	    USART_SendString(USART1, "AT+CGSN=1\r\n",0);      
	  #endif
		do {
				  #ifdef DEBUG
	     	   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
	        #endif
			     
					if (USART_CheckRec("OK")&&(USART_CheckRec("+CGSN:")))  
					{
								pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "+CGSN:");
								pri += 6;
								sscanf((char *)pri, "%[^\r\n]",NB.IMEI); 
								USART_SendString(USART1,"IMEI: ",0);
								USART_SendString(USART1,(uint8_t*)&NB.IMEI,0);
								USART_SendString(USART1,"\r\n",0);
								NB_WriteBasicOPt();
								return RESPONSEOK;

					}else if(USART_CheckRec("ERROR"))
					{
						    return ERROR_RSPER;
					}
					
					i++;
					delay_ms(50);
		} while (i < 30);
		return ERROR_NORSP; 
}


//读取IMSI
uint8_t NB_CIMI(void)
{
		uint8_t i = 0;
		uint8_t *pri1;
		USART_ClearBuf(&USART3_Data);
		memset(NB_Buf,0,50);
		USART_SendString(USART3, "AT+CIMI\r\n", 0);
		#ifdef DEBUG
	        	USART_SendString(USART1, "AT+CIMI\r\n", 0);  
	  #endif
		do {
				  #ifdef DEBUG
	     	   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
	        #endif
					if (USART_CheckRec("OK")) 
					{
							  pri1 = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "\r\n");
							  pri1 += 2;//不确定指针
								sscanf((char *)pri1, "%[^\r\n]",NB_Buf); 
								USART_SendString(USART1,"IMSI: ",0);
								USART_SendString(USART1,NB_Buf,0);
								USART_SendString(USART1,"\r\n",0);
							  return RESPONSEOK;
					}else if(USART_CheckRec("ERROR"))
					{
							return ERROR_RSPER;
					}

					i++;
					delay_ms(100);
		} while (i < 200); // 20s
		return ERROR_NORSP;   //??,???
}

//注册网络
uint8_t NB_ReadCGATT(void) 
{
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+CGATT?\r\n",0);
		#ifdef DEBUG
	        	USART_SendString(USART1, "AT+CGATT?\r\n",0);
	  #endif
		do {
				#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
        #endif
				if (USART_CheckRec("OK")) 
				{
							if(USART_CheckRec("+CGATT:1")){
									return RESPONSEOK;
							}else
							{
									return ERROR_FALL;
							}

				}else if(USART_CheckRec("ERROR"))
				{
							return ERROR_RSPER;
				}

			  i++;
				delay_ms(200);
		} while (i < 150); // 20s
		return ERROR_NORSP;   //??,???
}

//查询信号质量
uint8_t NB_CSQ(void) 
{
		uint8_t i = 0,temp;
		uint8_t *pri;
		memset(NB_Buf,0,50);
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+CSQ\r\n", 0);
		#ifdef DEBUG
						USART_SendString(USART1, "AT+CSQ\r\n", 0);
		#endif
		do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
			
			    if(USART_CheckRec("OK")) 
					{
							pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "+CSQ:");
							memcpy(NB_Buf,pri,10);
							sscanf((char *)pri, "%*[^:]:%[^,]",NB_Buf); 
							temp=atoi((char *)NB_Buf);  //读取信号强度
							if(temp == 99||(temp == 0))
							{
							     return ERROR_FALL;
				      }
				      printf("CSQ: %d\r\n",temp);
				      return RESPONSEOK;

			    }else if(USART_CheckRec("ERROR"))
				  {
			        return ERROR_RSPER;
			    }

				  i++;
				  delay_ms(200);
		} while (i < 150); // 20s
		return ERROR_NORSP;   //
}

//附着网络
uint8_t NB_CEREG(void) 
{
	
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+CEREG?\r\n", 0);
		#ifdef DEBUG
						USART_SendString(USART1, "AT+CEREG?\r\n",0);
		#endif
	
		do {
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
						if((USART_CheckRec("OK"))&&(USART_CheckRec("+CEREG:0,1"))) 
						{
								 return RESPONSEOK;

						}else if(USART_CheckRec("ERROR")||((USART_CheckRec("+CEREG:0,2"))))
						{
								 return ERROR_RSPER;
						}

						i++;
						delay_ms(100);
		} while (i < 200); // 20s
		return ERROR_NORSP;   //
}

//获取NB连接状态
uint8_t NB_CSCON(void) 
{
	
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+CSCON?\r\n", 0);
		#ifdef DEBUG
						USART_SendString(USART1, "AT+CSCON?\r\n",0);
		#endif
	
		do {
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
						if((USART_CheckRec("OK"))) 
						{    if(USART_CheckRec("CSCON:0,1"))
							   {
									  return RESPONSEOK;
							   }
							
								

						}else if(USART_CheckRec("ERROR"))
						{
								 return ERROR_RSPER;
						}

						i++;
						delay_ms(100);
		} while (i < 200); // 20s
		return ERROR_NORSP;   //
}


//AT+NRB重启模块
uint8_t NB_NRB(void)
{
			 uint8_t i = 0;
			 USART_ClearBuf(&USART3_Data);
			 USART_SendString(USART3, "AT+NRB\r\n",0);
			 #ifdef DEBUG
							 USART_SendString(USART1, "AT+NRB\r\n",0);
			 #endif
	
			 do {
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
						if ((USART_CheckRec("OK"))) 
						{
								return RESPONSEOK;
						}else if(USART_CheckRec("ERROR"))
					  {
								return ERROR_RSPER;
						}
						i++;
						delay_ms(100);
			} while (i < 20); // 20s
			return ERROR_NORSP;   //

}


	

// 设置IP地址
u8 NB_NCDP(char *ip_addr) 
{
			uint8_t i = 0;
			memset(NB_Buf,0,100);
			USART_ClearBuf(&USART3_Data);
			sprintf((char *)NB_Buf, "AT+NCDP=%s\r\n",ip_addr);
			USART_SendString(USART3, NB_Buf, 0);
			#ifdef DEBUG
							USART_SendString(USART1, NB_Buf, 0);
			#endif
//	 USART_SendString(USART1, NB_Buf, 0);
			do { 
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
				
						if (USART_CheckRec("OK")) 
						{
							    return RESPONSEOK;
							
						}else if(USART_CheckRec("ERROR"))
						{
						     return ERROR_RSPER;
						}
						
				    i++;
				    delay_ms(50);
			 } while (i < 200); // 20s
		   return ERROR_NORSP;   //
}


//设置发送回显
uint8_t NB_NSMI(uint8_t mode)
{	    
	    uint8_t i = 0;
			memset(NB_Buf,0,20);
			USART_ClearBuf(&USART3_Data);
			sprintf((char *)NB_Buf, "AT+NSMI=%d\r\n",mode);
			USART_SendString(USART3, NB_Buf, 0);
			#ifdef DEBUG
							USART_SendString(USART1, NB_Buf, 0);
			#endif
			do {
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
            if ((USART_CheckRec("OK"))) 
						{
                return RESPONSEOK;
            }else if(USART_CheckRec("ERROR"))
					  {
		            return ERROR_RSPER;
		        }
            i++;
		        delay_ms(50);
      } while (i < 200); // 20s
			
      return ERROR_NORSP;   //

}

//设置接收是否回显
uint8_t NB_NNMI(uint8_t mode)
{
	    uint8_t i = 0;
			memset(NB_Buf,0,20);
			USART_ClearBuf(&USART3_Data);
		 	sprintf((char *)NB_Buf, "AT+NNMI=%d\r\n",mode);
			USART_SendString(USART3, NB_Buf, 0);
			#ifdef DEBUG
							USART_SendString(USART1,  NB_Buf, 0);
			#endif
      do {
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
				
					 if ((USART_CheckRec("OK"))) 
					 {
							return RESPONSEOK;
					 }else if(USART_CheckRec("ERROR"))
				   {
							return ERROR_RSPER;
					 }
					 
					 i++;
					 delay_ms(50);
      } while (i < 200); // 20s
			
      return ERROR_NORSP;   //

}


//发送数据
uint8_t NB_NMGS(u16 lenth,char * data1) 
{
      uint16_t i = 0;
		  memset(NB_Buf,0,100);
      USART_ClearBuf(&USART3_Data);
	    sprintf((char *)NB_Buf, "AT+NMGS=%d,%s\r\n",lenth,data1);
      USART_SendString(USART3, NB_Buf, 0);
		  #ifdef DEBUG
							USART_SendString(USART1, NB_Buf,0);
			#endif
	
      do { 
						#ifdef DEBUG
										USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
				
					  if ((USART_CheckRec("OK"))&&(USART_CheckRec("+NSMI:SENT")) )
						{
							  return RESPONSEOK;
					  }else if(USART_CheckRec("ERROR"))
						{
							  return ERROR_RSPER;
					  }
      i++;
		  delay_ms(50);
      } while (i < 300); // 20s
			
      return ERROR_NORSP;   
			
}



//读取NB网络状态
uint8_t NB_GetNUESTATS ( uint8_t* ECL, uint16_t* PCI, float* RSRP, float* SNR) 
{
		uint8_t i = 0;
		uint8_t *pri;
		memset(NB_Buf,0,50);
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+NUESTATS\r\n", 0);
		#ifdef DEBUG
						USART_SendString(USART1, "AT+NUESTATS\r\n", 0);
		#endif
		do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
			
			    if(USART_CheckRec("OK")) 
					{
						  pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "ECL:");
							memcpy(NB_Buf,pri,10);
							sscanf((char *)pri, "%*[^:]:%[^\r\n]",NB_Buf); 
							*ECL = atoi((char *)NB_Buf); 
				      printf("ECL: %d\r\n",*ECL);
						  pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "SNR:");
							memcpy(NB_Buf,pri,10);
							sscanf((char *)pri, "%*[^:]:%[^\r\n]",NB_Buf); 
							*SNR = atof((char *)NB_Buf); 
				      printf("SNR: %.2f\r\n",*SNR);		
						
						  pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "PCI:");
							memcpy(NB_Buf,pri,10);
							sscanf((char *)pri, "%*[^:]:%[^\r\n]",NB_Buf); 
							*PCI = atoi((char *)NB_Buf);  
				      printf("PCI: %d\r\n",*PCI);								
						
						  pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "Signal power:");
//						   pri += 12;
							memcpy(NB_Buf,pri,20);
							sscanf((char *)pri, "%*[^:]:%[^\r\n]",NB_Buf); 
							*RSRP = atof((char *)NB_Buf);  //读取信号强度
				      printf("RSRP: %.2f\r\n",*RSRP);	
							
				      return RESPONSEOK;

			    }else if(USART_CheckRec("ERROR"))
				  {
			        return ERROR_RSPER;
			    }

				  i++;
				  delay_ms(50);
		} while (i < 100); // 20s
		return ERROR_NORSP;   //
}
/*
+CCLK:18/04/02,23:00:07+00

OK

*/

//获取基站时间
uint8_t NB_GetCCLK(uint32_t* TimesTamp) 
{   
	  struct tm stm1;  
	
		uint8_t i = 0;
		uint8_t *pri;
	  int NBY,NBM,NBD,NBH,NBMI,NBS;
		memset(NB_Buf,0,50);
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+CCLK?\r\n", 0);
	
		#ifdef DEBUG
						USART_SendString(USART1, "AT+CCLK?\r\n", 0);
		#endif
		do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
			
			    if(USART_CheckRec("OK")) 
					{   
						  if((USART_CheckRec("+CCLK:")))
						  { 
									 pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "+CCLK:");
									 pri += 6;
									 NBY = 2000+((*pri)-0x30) * 10;
									 NBY += ((*(++pri))-0x30);
									 if((NBY >= 2038)||( NBY < 2018 ))
									 {
												return ERROR_RSPER;
									 }
									 pri += 2;
									 NBM = (*pri-0x30)*10;
									 NBM  += (*(++pri)-0x30);
									 if( ( NBM > 12 ) ||( NBM < 1 ))
									 {
											return ERROR_RSPER;
									 }
									 
									 pri += 2;
									 NBD = (*pri-0x30)*10;
									 NBD  += (*(++pri)-0x30);
									 if( ( NBD > 31 ) ||( NBD < 1 ))
									 {
											return ERROR_RSPER;
									 }
									 
									 pri += 2;
									 NBH = (*pri-0x30)*10;
									 NBH  += (*(++pri)-0x30);					 
									 if( ( NBH > 23 ) ||( NBH < 0 ))
									 {
											return ERROR_RSPER;
									 }
									 
									 pri += 2;
									 NBMI = (*pri-0x30)*10;
									 NBMI += (*(++pri)-0x30);					
									 if( ( NBMI > 59 ) ||( NBMI < 0 ))
									 {
											return ERROR_RSPER;
									 }
									 
									 pri += 2;
									 NBS = (*pri-0x30)*10;
									 NBS  += (*(++pri)-0x30);					 
									 if( ( NBS > 59 ) ||( NBS < 0 ))
									 {
											return ERROR_RSPER;
									 }
									 
									stm1.tm_year = NBY-1900;  
									stm1.tm_mon = NBM-1;  
									stm1.tm_mday = NBD;  
									stm1.tm_hour = NBH;  
									stm1.tm_min = NBMI;  
									stm1.tm_sec = NBS;  
								 
									 *TimesTamp = mktime(&stm1); 
									printf("TimesTamp %d\r\n",*TimesTamp);
									 
									return RESPONSEOK;
						 }else
             {
						      return ERROR_RSPER;
						 }
			    }else if(USART_CheckRec("ERROR"))
				  {
			        return ERROR_RSPER;
			    }

				  i++;
				  delay_ms(100);
		} while (i < 100); // 20s
		return ERROR_NORSP;   //
}

//设置APN
uint8_t NB_CGDCONT(uint8_t* apn) 
{
    uint8_t i = 0;
	  memset(NB_Buf,0,50);
	  sprintf((char *)NB_Buf, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n",apn);
	  USART_ClearBuf(&USART3_Data);
    USART_SendString(USART3, NB_Buf, 0);
		#ifdef DEBUG
						USART_SendString(USART1, NB_Buf, 0);
		#endif
    do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if (USART_CheckRec("OK")) 
					{
						return RESPONSEOK;
					}else if(USART_CheckRec("ERROR"))
					{
						return RESPONSEOK;
					}

      i++;
		  delay_ms(100);
    } while (i < 100);
    return ERROR_NORSP;   
}

//获取内部保存的APN
uint8_t NB_GetCGDCONT(uint8_t* APN) 
{
    uint8_t i = 0;
		uint8_t *pri;
    USART_ClearBuf(&USART3_Data);
	  memset(NB_Buf,0,50);
    USART_SendString(USART3,"AT+CGDCONT?", 0);
		#ifdef DEBUG
						USART_SendString(USART1,"AT+CGDCONT?", 0);
		#endif
    do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if (USART_CheckRec("OK")) 
					{
						  pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX,(char *) APN);
						  if(pri != NULL) 
              {
							  return RESPONSEOK;
							}else
							{
						   return ERROR_FALL;
							
							}  
					}else if(USART_CheckRec("ERROR"))
					{
						return ERROR_RSPER;
					}

      i++;
		  delay_ms(100);
    } while (i < 100);
    return ERROR_NORSP;   
}

//模块保存的地址并对比模块保存的信息是否和配置的IP地址一致
uint8_t NB_GetNCDP(uint8_t* IP) 
{
    uint8_t i = 0;
		uint8_t *pri;
    USART_ClearBuf(&USART3_Data);
	  memset(NB_Buf,0,50);
//	  sprintf((char *)NB_Buf, "AT+CGDCONT=1,\"IP\",\"%s\"\r\n",IP);
    USART_SendString(USART3,"AT+NCDP?", 0);
		#ifdef DEBUG
						USART_SendString(USART1,"AT+NCDP?", 0);
		#endif
    do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if (USART_CheckRec("OK")) 
					{
						  pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX,(char *) IP);
						  if(pri != NULL) 
              {
							  return RESPONSEOK;
							}else
							{
						   return ERROR_FALL;
							
							}  
						
					  	
					}else if(USART_CheckRec("ERROR"))
					{
						return ERROR_RSPER;
					}

      i++;
		  delay_ms(100);
    } while (i < 100);
    return ERROR_NORSP;   
}

//设置IP地址
uint8_t NB_SetNCDP(uint8_t* IP) 
{
    uint16_t i = 0;
    USART_ClearBuf(&USART3_Data);
	  memset(NB_Buf,0,50);
	  sprintf((char *)NB_Buf, "AT+NCDP=%s\r\n",IP);
    USART_SendString(USART3,NB_Buf, 0);
		#ifdef DEBUG
					 USART_SendString(USART1,NB_Buf, 0);
		#endif
    do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if (USART_CheckRec("OK")) 
					{  
						  return RESPONSEOK;
	
					}else if(USART_CheckRec("ERROR"))
					{
					 	 return ERROR_RSPER;
					}

      i++;
		  delay_ms(100);
    } while (i < 100);
    return ERROR_NORSP;   
}

//发送数据
uint8_t NB_SendNMGS( uint8_t* Data ,uint16_t Long) 
{
    uint16_t i = 0;
    USART_ClearBuf(&USART3_Data);
	  memset(NB_Buf,0,200);
	  sprintf((char *)NB_Buf, "AT+NMGS=%d,%s\r\n",Long,Data);
    USART_SendString(USART3,NB_Buf, 0);
		#ifdef DEBUG
					 USART_SendString(USART1,NB_Buf, 0);
		#endif
    do {
					#ifdef DEBUG
							   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
//					if (USART_CheckRec("AAAA0000")) 
				  if ((USART_CheckRec("OK"))&&(USART_CheckRec("+NSMI:SENT")) )
					{  
						  return RESPONSEOK;
	
					}else if(USART_CheckRec("ERROR"))
					{
					 	  return ERROR_RSPER;
					}
      i++;
		  delay_ms(100);
    } while (i < 600);
    return ERROR_NORSP;   
}


//获取接收数据的次数
uint8_t NB_NQMGR( uint8_t *NQMGR ) 
{
		uint8_t i = 0;
		uint8_t *pri;
		uint8_t tempbuf[10];
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, "AT+NQMGR\r\n",0);
		#ifdef DEBUG
				   USART_SendString(USART1, "AT+NQMGR\r\n",0);
		#endif	
    do {
					#ifdef DEBUG
							   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if (USART_CheckRec("OK")) 
					{
						   pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "BUFFERED");
						   memcpy(tempbuf,pri,10);
						   sscanf((char *)pri, "%*[^=]=%[^,]",tempbuf); 
						   *NQMGR = atoi((char* )tempbuf); //截取接收的次数
						   return RESPONSEOK;
					}else if(USART_CheckRec("ERROR"))
					{
					   return ERROR_RSPER;
					}
					i++;
					delay_ms(50);
   } while (i < 100); 
		
   return ERROR_NORSP;  
}

//获取接收数据
uint8_t NB_NMGR(void)
{
    uint8_t i = 0;
    USART_ClearBuf(&USART3_Data);
    USART_SendString(USART3, "AT+NMGR\r\n",0);
		#ifdef DEBUG
					 USART_SendString(USART1, "AT+NMGR\r\n",0);
		#endif		
    do {
					#ifdef DEBUG
							   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if ((USART_CheckRec("OK"))) 
					{
							if((USART_CheckRec("9999")))
							{  
									SetBasicInformat(USART3_Data.DATA_RX,USART_REC_LEN);
							}
							else if((USART_CheckRec("AAAA0000")))
							{
									return DownConfirm;
							}
							return ERROR_NORSP;
					}else if(USART_CheckRec("ERROR"))
					{
							 return ERROR_NORSP;
					}
          i++;
		      delay_ms(50);
  } while (i < 100); // 20s
		
  return ERROR_NORSP;   //

}

//检测设备状态
uint8_t CheckNBModule(void)
{
		uint8_t i = 0;
		uint8_t Statua_Count = 0;
		uint8_t Statua_Code = 0;
	  uint8_t Status = 0;
	  
 	  while( Statua_Count < 1 )
    {     
			    switch(Statua_Code)
					{
						case 0x00:                           //打开模块电源
								NB.Status = 0x00;
								NB_Power(ENABLE);
								Statua_Code = 0x01;				
						
						case 0x01:	                        	//读取IMEI号
							   NB.Status = 0x01;
							   for( i = 0; i < 10; i++ )
						     {
								     Status = NB_CGSN();
								     if(Status == 1) 
                     {
											   break;
										 }
										 delay_ms(1000);
								 }
								 if(Status != 1)
								 {
								     goto OUTSW; 
								 }
						
		        case 0x02:	                             //查询IMSI		
							   NB.Status = 0x02;
							   for( i = 0; i < 10; i++ )
						     {
								     Status = NB_CIMI();
								     if(Status == 1) 
                     {
											   break;
										 }
										 delay_ms(1000);
								 }
								 if(Status != 1)
								 {
								     goto OUTSW;    
								 }
								  
						case 0x03:                                     //设置APN
							   NB.Status = 0x03;
						     for( i = 0; i < 3; i++ )
						     {
								     Status = NB_CGDCONT(NB.APN);
								     if((Status == 1)) 
                     {
											   break;
										 }
										 delay_ms(1000);
								 }
								 
								 if( Status != 1 )
								 {
								     goto OUTSW;   
								 }						 
						    
						case 0x04:                                    //读取信号强度
						     NB.Status = 0x04;
								 for( i = 0; i < 60; i++ )
						     {
								     Status = NB_CSQ();
								     if(Status == 1) 
                     {
											   break;
											 
										 }
										 delay_ms(1000);
								 }
								 
								 if(Status != 1)
								 {
								     goto OUTSW;   
								 }	
						
            case 0x05:                                //注册网络
                 NB.Status = 0x05;							
							   for( i = 0; i < 60; i++ )
						     {
								     Status = NB_ReadCGATT();
								     if(Status == 1) 
                     {
											   break;
										 }
										 delay_ms(1000);
								 }
								 
								 if(Status != 1)
								 {
								     goto OUTSW;  
								 }									 
            case 0x06:                                  //附着网络
						     NB.Status = 0x06;		
							   for( i = 0; i < 10; i++ )
						     {
								     Status = NB_CEREG();
								     if(Status == 1) 
                     {
											   break;
										 }
										 delay_ms(500);
								 }
								 if(Status != 1)
								 {
								     goto OUTSW;    
								 }	
								 
							   
            case 0x07:                                    //获取NB网络状态
							   NB.Status = 0x07;
							   for( i = 0; i < 2; i++ )
						     {
								     Status = NB_GetNUESTATS(&NB.ECL,&NB.PCI,&NB.RSRP,&NB.SNR);
								     if(Status == 1) 
                     {
											   break;
										 }
										 delay_ms(100);
								 }	
								 if(Status != 1)
								 {
								     goto OUTSW;   
								 }	
								 
            case 0x08:                                  	 //网络正常
							   NB.Status = 0x08;
							   for( i = 0; i < 5; i++ )
						     {
								     Status = NB_GetCCLK(&NB.TimesTamp);
								     if(Status == 1) 
                     {
											   break;
										 }
										 delay_ms(2000);
								 }						 
								 if(Status != 1)
								 {
								     goto OUTSW;    
								 }									 
//							   break;
            case 0x09:	
                 NB.Status = 0x09;	
						     break;
						default:
										break;				
					}						
		
		  OUTSW:
				    Statua_Count++;
		}
	  return GSM_ErrorHandler();

}


//NB模块和网络状态
uint8_t GSM_ErrorHandler(void) 
{
	
  switch (NB.Status) 
	{
         case 0x00: 
              USART_SendString(USART1, "\r\nModule Power Error\r\n",0);
              return  ERROR_RSPER;
         case 0x01:
              USART_SendString(USART1, "\r\nReadIMEI Error\r\n",0);
				      return  ERROR_RSPER;
         case 0x02: 
              USART_SendString(USART1, "\r\nRead IMSI Error\r\n",0);
              return  ERROR_RSPER;
         case 0x03:
              USART_SendString(USART1, "\r\nSet APN Error\r\n",0);
              return  ERROR_RSPER;
         case 0x04: 
              USART_SendString(USART1, "\r\nNo Signal\r\n",0);
              return  ERROR_RSPER;
         case 0x05:
              USART_SendString(USART1, "\r\nInactive Network\r\n",0);
				      return  ERROR_RSPER;
         case 0x06: 
              USART_SendString(USART1, "\r\nNetwork Registration Failed\r\n",0);
              return  ERROR_RSPER;
         case 0x07:
              USART_SendString(USART1, "\r\nGet Network Status Failed\r\n",0);
				      return  ERROR_RSPER;
         case 0x08:
              USART_SendString(USART1, "\r\nGet Network Time Error\r\n",0);
              return  ERROR_RSPER;
         case 0x09:
              USART_SendString(USART1, "\r\nAll Normal \r\n",0);
				      return  RESPONSEOK;
         case 10:
              USART_SendString(USART1, "\r\nSet CDP Error\r\n",0);
				      return  ERROR_RSPER;		
         case 11:
              USART_SendString(USART1, "\r\nSet Send Show Error\r\n",0);
				      return  ERROR_RSPER;		
         case 12:
              USART_SendString(USART1, "\r\nSend Data Error\r\n",0);
				      return  ERROR_RSPER;
         case 13:
              USART_SendString(USART1, "\r\nReceive Data Error\r\n",0);
              return  ERROR_RSPER;	
         case 14:
              USART_SendString(USART1, "\r\nSend Data Error\r\n",0);
				      return  ERROR_RSPER;
				 case 15:
              USART_SendString(USART1, "\r\nResend Data Error\r\n",0);
				      return  ERROR_FALL;		
         case 16:
              USART_SendString(USART1, "\r\nSend Data Ok\r\n",0);
				      return  RESPONSEOK;		
        default:
					     return  ERROR_RSPER;
              
  }
//  return 0x01;
}


//发送函数
uint8_t SendMassege(uint8_t* Massege ,uint16_t MassegeLong)
{
			uint8_t i = 0;
			uint8_t  Massegeflag = 0;
	    uint8_t  MassegeCount=0;
		  uint8_t  SendStatus = 0;
	
      Massegeflag = 10;
	 	  while( MassegeCount < 1 )
      {    
            switch( Massegeflag )	
						{
									case 10:                            //设置IP地址
												NB.Status = 10;
												for( i = 0; i < 3; i++ )
												{
														SendStatus  =	NB_SetNCDP(NB.IP);
												
														if( SendStatus == 1 )
														{
															  Massegeflag = 11;
																break;
														}else 
                            {
														    delay_ms(1000);
														}
												}
												
											  if(SendStatus != 1)
											  {
													 goto OUTSW;    
											  }													
												
									case 11:                            //设置发送回显
										
											  NB.Status = 11;
												for( i = 0; i < 3; i++ )
												{
														SendStatus  =	NB_NSMI(1);
														if( SendStatus == 1 )
														{
															  Massegeflag = 12;
																break;
														}else 
                            {
														    delay_ms(1000);
														}
												}
												
											  if(SendStatus != 1)
											  {
													 goto OUTSW;    
											  }												
									case 12:                                //发送信息
											  NB.Status = 12;
												for( i = 0; i < 3; i++ )
												{
														SendStatus  =	NB_SendNMGS(Massege,MassegeLong);
														if( SendStatus == 1 )
														{
															  Massegeflag = 13;
																break;
														}else 
                            {
														    delay_ms(5000);
														}
												}

											  if(SendStatus != 1)
											  {
													 goto OUTSW;    
											  }												
									
									case 13:                              //接收信息
												NB.Status = 13;
											  SendStatus = 0;
												for( i = 0; i < 30; i++ )
												{
														SendStatus +=	ReceiveMassege(); 
														if( ( SendStatus & 0x10 ) != 0 )
														{
															  Massegeflag = 13;
																break;
														}else 
                            {
														    delay_ms(2000);
														}
												}	
												
                        if( (SendStatus & 0x10) != 0)
                        {
													  Massegeflag = 14;
												}		
												
                  case 14:                                  //接收下行确认
//										
												NB.Status = 14;
									      if( PROTOCOL_Structure.HisData > 0 )
                        {    
													  PROTOCOL_Structure.HisData--;
													  printf("rHisData %d\r\n",PROTOCOL_Structure.HisData);
														for( i = 0; i < 3; i++ )
														{
															   printf("HisDataBuf[%d] %d\r\n",PROTOCOL_Structure.HisData,strlen((char* )PROTOCOL_Structure.HisDataBuf[PROTOCOL_Structure.HisData])/2);
																 SendStatus  =	NB_SendNMGS((uint8_t* )PROTOCOL_Structure.HisDataBuf[PROTOCOL_Structure.HisData],
															                            strlen((char* )PROTOCOL_Structure.HisDataBuf[PROTOCOL_Structure.HisData])/2);
																if( SendStatus == 1 )
																{
																		Massegeflag = 15;
																		break;
																}else 
																{
																		delay_ms(5000);
																}
														}
											  }else                           //没有历史缓存数据，退出
                        {
													  NB.Status = 16;
												    break;
												}
												
											  if(SendStatus != 1)
											  {
													 NB.Status = 15;
													 PROTOCOL_Structure.HisData++;  //重传失败，恢复原来的计数功能
													 goto OUTSW;    
											  }												
									
									case 15:                              //接收信息
												NB.Status = 15;
											  SendStatus = 0;
												for( i = 0; i < 30; i++ )
												{
														SendStatus +=	ReceiveMassege(); 
														if( SendStatus == 0x10 )
														{
															  if( PROTOCOL_Structure.HisData == 0)
                                {     
																		NB.Status = 16;
																		break;
																}else
                                {
																		Massegeflag = 14;
																		break;
																}
														}else 
                            {
														    delay_ms(2000);
														}
												}	
                        if( (SendStatus & 0x30) != 0)
                        {   
//													
												    Massegeflag = 14;
												}												
						}
				OUTSW:
							MassegeCount++;
		 }
     return GSM_ErrorHandler();
}


//接收数据函数
uint8_t ReceiveMassege(void)
{
		uint8_t RMassegeFlag = 0;
		uint8_t RMCount = 0;
	
		//获取接收的次数
		NB_NQMGR(&RMCount);
		while( RMCount-- )
		{    
					//读取数据
			   RMassegeFlag +=	NB_NMGR();
		  #ifdef DEBUG
			   printf("RMassegeFlag %X\r\n",RMassegeFlag);
			#endif		
				 delay_ms(1000);
				 //获取接收次数
				 NB_NQMGR(&RMCount);
		}
		return RMassegeFlag;		
}
