//协议文件
//包含了参数处理，存储读取等功能

#include "protocol.h"

 PROTOCOL_TypeDef PROTOCOL_Structure;
 extern NB_TypeDef   NB;
 extern OS_CPU_SR cpu_sr;
 extern uint16_t Ttimes;
 
uint8_t  BATTERY = 0;
uint8_t FLASH_RW_Buf[500] = {0};
uint8_t FLASH_Send_Buf[600] = {0};
uint16_t AH,Batt;  
extern uint32_t SendDataCount;
extern uint8_t CarState;
extern uint8_t  UltraCarSate;
extern uint8_t  MagnentCarState;
extern uint8_t  NoMagnetButUltra ;
extern int XYZSumMagn[3] ;

const char Timbuf5[8] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};  //补0用，使用XXTEA最后一个不够进行补0；  
void PROTOCOL_StructInit(void) 
{
	PROTOCOL_Structure.TestTimes = 5;        //检测时间 单位分钟
	PROTOCOL_Structure.UpdateTime = 24;     //默认数据更新时间 小时
	PROTOCOL_Structure.Battery=20;     //电量报警值 单位百分比
}


//输出所有的设置信息
// OPT为选项字，为0的则直接串口输出
void PROTOCOL_SendAllConf(uint8_t OPT) 
{
   sprintf((char *)FLASH_RW_Buf,
		       "Test Times: %d min\r\nUpdateTime: %d h\r\nbattery %d\r\n",
	         PROTOCOL_Structure.TestTimes,PROTOCOL_Structure.UpdateTime,PROTOCOL_Structure.Battery);
	 
		if( OPT == 0 ) 
		{
			USART_SendString(USART1, FLASH_RW_Buf, 0);
		} 
 
}

//将基本配置信息写入FLASH中
uint8_t PROTOCOL_WriteBasicOPt(void) 
{
	sprintf((char *)FLASH_RW_Buf, "B:%d:%d:%d:",PROTOCOL_Structure.TestTimes,PROTOCOL_Structure.Battery,
		     PROTOCOL_Structure.UpdateTime);
	FLASH_WriteString(BASIC_OPTION_ADDR, (uint16_t *)FLASH_RW_Buf,strlen((char *)FLASH_RW_Buf));     
  return 1;
}

//从FLASH中读出基本配置的内容
uint8_t PROTOCOL_ReadBasicOPt(void) 
{
		PROTOCOL_StructInit(); //对结构体初始化
		FLASH_ReadString(BASIC_OPTION_ADDR, (uint16_t *)FLASH_RW_Buf, 200);
	
    if (FLASH_RW_Buf[0] == 'B') //首字母为B 表明读出正常
		{  
				sscanf((char *)FLASH_RW_Buf, "B:%d:%d:%d:",&PROTOCOL_Structure.TestTimes,&PROTOCOL_Structure.Battery,
				       &PROTOCOL_Structure.UpdateTime );
				PROTOCOL_SendAllConf(0);
				return 1;
    } else                         //读出失败
		{                   
				PROTOCOL_WriteBasicOPt(); //写入FLASH 中
				return 0;
    }
}

//读出上传时间
uint16_t PROTOCOL_ReadUPTime(void)
{ 
    return PROTOCOL_Structure.UpdateTime; 
}

//设置上传时间
void PROTOCOL_WriteUPTime(uint16_t UPTime) 
{  
    PROTOCOL_Structure.UpdateTime = UPTime;
    PROTOCOL_WriteBasicOPt();
}


//读出检测时间间隔
uint16_t PROTOCOL_ReadUTTimes(void)
{ 
   return PROTOCOL_Structure.TestTimes;
}

//设置检测时间间隔
void PROTOCOL_WriteTestTimes(uint16_t TTime) 
{ 
    PROTOCOL_Structure.TestTimes = TTime;
    PROTOCOL_WriteBasicOPt();
}

//读出电量
uint16_t PROTOCOL_ReadBatt(void) 
{
  return PROTOCOL_Structure.Battery;
}

//设置电量
void PROTOCOL_WriteBatt(uint16_t temp) 
{
    PROTOCOL_Structure.Battery = temp;   
    PROTOCOL_WriteBasicOPt();
}


/****************************************************************************
 *
 * Function Name:    SetBasicInformat
 * Input:            Array size，length
 * Output:           None
 * Returns:          
 * Description:      Initialize ADC channels 5 and 6
 * Note:             
 ***************************************************************************/

void SetBasicInformat(uint8_t  *arr ,uint16_t  len)
{  
		uint16_t i;
		uint8_t *ptr1;
		/*ascii*/
		if(((ptr1 = (uint8_t *)strstr((char *)arr, "9999"))!=NULL))
		{  
			 dealdata(ptr1,ASCIITYPE);
		}
		
		/*hexadecimal */
		for(i=0;i<len;i++)
		{
			if((*arr)==0x99)
			{
				dealdata(arr,HEXTYPE);
				break;
			}
			arr++;
		}
}
 
uint8_t dealdata(uint8_t  *ptr1,uint8_t type)
{
		uint16_t temp;
		uint8_t  databuf[3]={0},i;
		if( type == HEXTYPE )
		{
				ptr1++;
				if(( *ptr1 ) == 0x99 )
				{
					ptr1++;
					switch(*ptr1)
					{
							case 0x01:  
									 ptr1++;
									 temp =  ((*ptr1 >> 4) * 10);
									 temp += (((*ptr1 << 4) & 0xf0 )/16);
									 if(( temp > 0 ) && (temp <= 24))
									 {
											PROTOCOL_WriteUPTime(temp);
											printf("Heartbeat Time: %d h\r\n",temp);
									 }else
									 {
											printf("Null\r\n");
									 }
									 
                   break;
						 
						  case 0x02:   //

				           break;
									 
						  case 0x03:  //flame alarm setting

									 break;
								 
						  case 0x04:   //fall alarm setting
						 
									 printf("null\r\n");
						 
									 break;
			 
						  case 0x05:   
							
									 ptr1++;
									 temp = ((*ptr1 >> 4) * 10);
									 temp+= (((*ptr1 << 4) & 0xf0 ) / 16);
									 if( ( temp >= 5 ) && ( temp <= 99 ) )
									 {
											 PROTOCOL_WriteBatt(temp);
											 printf("Battery Status: %d\r\n",temp);
									 }
									 break;
						  case 0x06:   //set ip addr
									 ptr1++;
									 memset(FLASH_RW_Buf, 0, 50);
									 temp += (((*ptr1) >> 4) * 16);
									 temp += ((((*ptr1) << 4) & 0xf0 ) / 16);
									 sprintf((char *)FLASH_RW_Buf, "%d.",temp);
							
									 ptr1++;
									 temp = (((*ptr1) >> 4) * 16);
									 temp += ((((*ptr1) << 4) & 0xf0 ) / 16);
									 sprintf((char *)FLASH_RW_Buf, "%s%d.",FLASH_RW_Buf,temp);
							
									 ptr1++;
									 temp = (((*ptr1) >> 4) * 16);
									 temp += ((((*ptr1) << 4) & 0xf0 ) / 16);
									 sprintf((char *)FLASH_RW_Buf, "%s%d.",FLASH_RW_Buf,temp);
						
									 ptr1++;
									 temp = (((*ptr1) >> 4) * 16);
									 temp += ((((*ptr1) << 4) & 0xf0 ) / 16);
									 sprintf((char *)FLASH_RW_Buf, "%s%d",FLASH_RW_Buf,temp);
									 strcpy((char *)NB.IP,(char *)FLASH_RW_Buf);
									 
									 NB_WriteBasicOPt();
									 printf("\r\nIP ADDR:");
									 USART_SendString(USART1, (uint8_t *)NB.IP, 0);
									 printf("\r\n");
									 
									 break;
						  case 0x07:  
							
									 ptr1++;
									 memset(FLASH_RW_Buf, 0, 100);
									 temp =((*ptr1)>> 4) * 16 * 16 * 16;
									 temp +=(((*ptr1)<< 4) & 0xf0 ) * 16;
									 ptr1++;
						
									 temp += (((*ptr1) >> 4) * 16);
									 temp += ((((*ptr1) << 4) & 0xf0 ) / 16);
									 sprintf((char *)NB.PORT, "%d",temp);
									 NB_WriteBasicOPt();
						
									 printf("\r\nIP PORT:");
									 USART_SendString(USART1, (uint8_t *)NB.PORT, 0);
									 printf("\r\n");
									 break;   
						
					    case 0x08: 
									 ptr1++;
									 temp = (*ptr1 >> 4) * 10;
									 temp+= ((*ptr1 << 4 & 0xf0 ) / 16);
									 if(( temp >= 1 ) && ( temp <= 60 ))
									 { 
											PROTOCOL_WriteTestTimes(temp);
											printf("Testing time is %d min\r\n",temp);
									 }
									 break;
									
						  case 0x09:   //GPS Mode setting  
										
									 ptr1++;
									 temp = (*ptr1 >> 4)*10;
									 temp+= (*ptr1 << 4&0xf0 )/16;
                   break;
									 
					     default :
								   break;
			     }
		    }
	  }else if(type==ASCIITYPE)
    {
				ptr1+=5;
				switch(*ptr1)
	      {
			         case 0x31:   
										ptr1++;
										memcpy(databuf,ptr1,2);
										temp=atoi((char *)databuf);
										if((temp > 0) && (temp <= 24) )
										{
												PROTOCOL_WriteUPTime(temp);
												printf("Heartbeat Time: %d h\r\n",temp);
										}else
										{
												printf("Null\r\n");	
										}	  
										break;
										
							 case 0x32:  
										ptr1++;
										memcpy(databuf,ptr1,2);
										temp = atoi((char *)databuf);
										printf("Null\r\n");		
								    break;
							 
							 case 0x33: 
										ptr1++;
										memcpy(databuf,ptr1,2);
										temp=atoi((char *)databuf);
										printf("Null\r\n");	
									  break;
							 case 0x34:  
									  printf("Null\r\n");	
							      break;
							 case 0x35: 
										ptr1++;
										memcpy(databuf,ptr1,2);
										temp=atoi((char *)databuf);
										if( ( temp >= 5 ) && ( temp <= 99 ) )
										{
												PROTOCOL_WriteBatt(temp);
												printf("battery alarm is %d \r\n",temp);
										}else
										{
												printf("Null\r\n");	
										}			
							      break;
				       case 0x36:   //set ip addr
										memset(FLASH_RW_Buf, 0, 100);
										for(i=0;i<=3;i++)
										{
												ptr1++;
												if(*ptr1 <= 0x39) 
												{
													temp=(*ptr1-0x30)*16;
												}else
												{
													temp=(((*ptr1-0x31)/16*10)+((*ptr1-0x31)%16))*16;
												} 
												
												ptr1++;
												if(*ptr1 <= 0x39) 
												{
													temp+=(*ptr1-0x30);
												}else
												{
													temp+=(((*ptr1-0x31)/16*10)+((*ptr1-0x31)%16));
												} 
												
												if((temp < 0)||(temp > 255))	
												{     //输入值为0-255
													printf("Null\r\n");	
													break;
												}		
												
												if(i==0)
												{
													sprintf((char *)FLASH_RW_Buf, "%d.",temp);
												}else if(i==3)
												{
													sprintf((char *)FLASH_RW_Buf, "%s%d",FLASH_RW_Buf,temp);
												}else{
													sprintf((char *)FLASH_RW_Buf, "%s%d.",FLASH_RW_Buf,temp);
												}
									 }
										strcpy((char *)NB.IP,(char *)FLASH_RW_Buf);
										NB_WriteBasicOPt();
										printf("\r\n IP ADDR:  ");
										USART_SendString(USART1, (uint8_t *)NB.IP, 0);
										printf("\r\n");
										break;	
				       case 0x37:   
									  ptr1++;
									  if(*ptr1 <= 0x39) 
									  {
											temp=(*ptr1 - 0x30) * 16 * 16 * 16;
									  }else
									  {
											temp=(((*ptr1-0x31) / 16 * 10)+((*ptr1 - 0x31) % 16)) * 16 * 16 * 16;
									  } 		
									  ptr1++;
									  if(*ptr1 <= 0x39) 
									  {
											temp += (*ptr1-0x30) * 16 * 16;
									  }else
									  {
											temp+=(((*ptr1-0x31)/16*10)+((*ptr1-0x31)%16)) * 16 * 16;
									  } 	
									  ptr1++;
									  if(*ptr1 <= 0x39) 
									  {
											temp += (*ptr1-0x30)*16;
									  }else
									  {
											temp += (((*ptr1-0x31) / 16 * 10)+((*ptr1-0x31) % 16)) * 16;
									  } 
									  ptr1++;
									  if( *ptr1 <= 0x39 ) 
									  {
											temp += (*ptr1-0x30);
									  }else
									  {
											temp += (((*ptr1-0x31)/16*10)+((*ptr1-0x31)%16));
									  } 
									  if(( temp >= 0 )&&( temp <= 65535 ))
									  {
											sprintf((char *)NB.PORT, "%d",temp);
											NB_WriteBasicOPt();
											printf("\r\n IP PORT:  ");
											USART_SendString(USART1, (uint8_t *)NB.PORT, 0);
											printf("\r\n");
									  }else
									  {
											printf("Null\r\n");		
									  }
									  break;	
				       case 0x38:   
									  ptr1++;
									  memcpy(databuf,ptr1,2);
									  temp=atoi((char *)databuf);
									  if(temp >= 1 && temp <= 60)
									  {
											PROTOCOL_WriteTestTimes(temp);
											printf("Testing time is %d min\r\n",temp);
									  }
			              break;
							 case 0x39:
								    ptr1++;
								    memcpy(databuf,ptr1,2);
							      temp=atoi((char *)databuf);
                    break;
				    default :
                     return  0;
			  }
	 }
	 return  1;
}



/*
 * Data               高度
 * Temperature        温度
 * Angle              角度
 * Voltage            电压
 * FrameCount         数据类型，1或者2

*/
uint8_t NBSendDataType1(uint16_t* Data,  int16_t* MagX, int16_t*  MagY, int16_t* MagZ,uint8_t* Battery, uint16_t* VOLT)
{
		uint8_t* XXTEAPRI;  //数据转换指针
		uint8_t i,j;        
		uint8_t cout;       
		char BufFest[2],Timbuf[10],Timbuf1[4];  
		int XXTEAData[20]={0};        //加密算法数组
		
	  memset((char* )PROTOCOL_Structure.NowDataBuf[0],0,200);
		memset((char* )PROTOCOL_Structure.NowDataBuf[1],0,200);
		
		PROTOCOL_Structure.XXTEAKey[0]=0x01;//盐值数
    PROTOCOL_Structure.XXTEAKey[1]=0x02;
    PROTOCOL_Structure.XXTEAKey[2]=0x03;
	  PROTOCOL_Structure.XXTEAKey[3]=0x04;	
		
		//  高度转换
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%04X",(char* )PROTOCOL_Structure.NowDataBuf[1],*Data);
					
    //4个状态位，电量状态位没做处理
		memset(Timbuf,0,10);
		Timbuf[0] = 0x30+CarState;
		Timbuf[1] = 0x30+UltraCarSate;
		Timbuf[2] = 0x30+MagnentCarState;
		if( * Battery <= PROTOCOL_Structure.Battery )
		{
		   Timbuf[3] = 0x31;   
    }else
    {
		 Timbuf[3] = 0x30;   
		}			
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%s",(char* )PROTOCOL_Structure.NowDataBuf[1],Timbuf);
	
		//  ECL和电压值
//		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%02X%04X",(char* )PROTOCOL_Structure.NowDataBuf[1],NB.ECL,*Battery);
    sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%04X",(char* )PROTOCOL_Structure.NowDataBuf[1],*VOLT);  
	
		// RSRP值
		memset(Timbuf1,0,4);
		memset(Timbuf,0,10);
		Float_to_Byte(NB.RSRP,(uint8_t *)Timbuf1);	  //浮点型转字符串
		for(i=0; i < 4; i++)
		{
				sprintf(BufFest, "%02X",Timbuf1[i]);
				Timbuf[(2*i)] = BufFest[0];
				Timbuf[2*i+1] = BufFest[1];
		}
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%s",(char* )PROTOCOL_Structure.NowDataBuf[1],Timbuf);
		
		// SNR值	
		memset(Timbuf1,0,4);
		memset(Timbuf,0,10);
		Float_to_Byte(NB.SNR,(uint8_t *)Timbuf1);
		for(i=0; i < 4; i++)
		{
				sprintf(BufFest, "%02X",Timbuf1[i]);
				Timbuf[(2*i)] = BufFest[0];
				Timbuf[2*i+1] = BufFest[1];
		}
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%s",(char* )PROTOCOL_Structure.NowDataBuf[1],Timbuf);
	
		//  PCI值
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%04X",(char* )PROTOCOL_Structure.NowDataBuf[1],NB.PCI);
	
		// 获取网络时间
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%08X",(char* )PROTOCOL_Structure.NowDataBuf[1],NB.TimesTamp);
	
		//X/Y/Zz轴数据
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%04X%04X%04X",(char* )PROTOCOL_Structure.NowDataBuf[1],*MagX,*MagY,*MagZ);		

		// 帧计数器 ,每次整合自++
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%04X",(char* )PROTOCOL_Structure.NowDataBuf[1],PROTOCOL_Structure.FRMN);
		PROTOCOL_Structure.FRMN++;	
		// 求本数据包大小，不够的增加0
		j = strlen((char* )PROTOCOL_Structure.NowDataBuf[1]);
//		printf("j %d\r\n",j);
	  if(( j % 8 ) == 0 )
		{
			 cout = ( j / 8 );
		}else
		{
			 cout = ( j / 8 );
			 cout += 1;
		}
				
		memset(Timbuf,0,10);
		memcpy(Timbuf,Timbuf5,(8 - ( j % 8 )));
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[1],"%s%s",(char* )PROTOCOL_Structure.NowDataBuf[1],(uint8_t *)Timbuf);//补足数量，默认在后面补足
		USART_SendString(USART1,(uint8_t *) PROTOCOL_Structure.NowDataBuf[1], 0);	
		printf("\r\n");			
		XXTEAPRI = PROTOCOL_Structure.NowDataBuf[1];
		
		for(i=0; i < strlen((char* )PROTOCOL_Structure.NowDataBuf[1])/8; i++ )
		{    
			 for(j = 0; j < 8; j++ )
			 {   
					if(*XXTEAPRI <= 0x39) 
					{
							 XXTEAData[i] += ( ( ( *XXTEAPRI ) - 0x30 ) << ( 28 - ( 4 * j ) ) );
					}else
					{
							 XXTEAData[i] += (  ( ( ( ( *XXTEAPRI ) - 0x31 ) / 16 ) * 10 ) + ( ( ( *XXTEAPRI ) - 0x31 ) % 16 ) ) << ( 28 -( 4 * j ) );
					} 
						
					XXTEAPRI++;
			 }
//					 printf("%08X\r\n",XXTEAData[i]);
		}
		
		//数据加密XXTEAKey
		btea(XXTEAData,(strlen((char* )PROTOCOL_Structure.NowDataBuf[1]) / 8 ),PROTOCOL_Structure.XXTEAKey);
		
		if( PROTOCOL_Structure.UpdateTime >= 0 )
		{
		    PROTOCOL_Structure.DataType = 0x01;
		}else
    {
		    PROTOCOL_Structure.DataType = 0x02;
		}
		
		// 帧头，设备类型，数据类型，数据包长度
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[0],"%s%02X%02X","800002",PROTOCOL_Structure.DataType,(strlen((char* )PROTOCOL_Structure.NowDataBuf[1])/2));
		
		for( j = 0; j < (strlen((char* )PROTOCOL_Structure.NowDataBuf[1]) / 8); j++)
		{				
				sprintf((char* )&PROTOCOL_Structure.NowDataBuf[0],"%s%08X",(char* )PROTOCOL_Structure.NowDataBuf[0],XXTEAData[j]);	
		} 
		
		sprintf((char* )&PROTOCOL_Structure.NowDataBuf[0],"%s%s",(char* )PROTOCOL_Structure.NowDataBuf[0],"0081");
		USART_SendString(USART1,(uint8_t *) PROTOCOL_Structure.NowDataBuf[0], 0);
		printf("\r\n");
    return 1;
}

//刷新发送数据  
uint8_t PROTOCOL_AlarmData(void) 
{
	    uint8_t  DataType,i,Batt;	
		  uint16_t Volt;
			BatLevel(&Batt,&Volt);
	    printf("Height: %d\r\nBatt: %d\r\nVolt: %d\r\n",PROTOCOL_Structure.Height,Batt,Volt);
	
		  // 读取模块状态
			if( CheckNBModule() != 1 )//读取错误时，相关信息归0
			{
			}
			PROTOCOL_Structure.MagX = XYZSumMagn[0];
			PROTOCOL_Structure.MagY = XYZSumMagn[1];
			PROTOCOL_Structure.MagZ = XYZSumMagn[2];
			
			//整合数据
			for( i = 0; i < 2; i++ )
			{
			    DataType = NBSendDataType1(&PROTOCOL_Structure.Height, 
				                             &PROTOCOL_Structure.MagX, 
				                             &PROTOCOL_Structure.MagY,
				                             &PROTOCOL_Structure.MagZ,
				                             &Batt,
				                             &Volt
				                             );
					if( DataType != 1 )
					{ 
							return  0;
					}else
          {
					    break;
					}
		  }	
	//发送数据

		 DataType = SendMassege( PROTOCOL_Structure.NowDataBuf[0],strlen((char* )PROTOCOL_Structure.NowDataBuf[0])/2);
			
		 if( DataType != 1 )
     {
			 if(DataType != 0x03)
				{
						 memcpy(PROTOCOL_Structure.HisDataBuf[PROTOCOL_Structure.HisData],PROTOCOL_Structure.NowDataBuf[0],
									 strlen((char* )PROTOCOL_Structure.NowDataBuf[0]));
					
					   USART_SendString(USART1,PROTOCOL_Structure.HisDataBuf[PROTOCOL_Structure.HisData], 0);
			       printf("\r\n");
					
					   PROTOCOL_Structure.HisData++;
					  if(PROTOCOL_Structure.HisData >= 12)
					  {
							 PROTOCOL_Structure.HisData = 12;
					  }
						printf("HisData1 %X\r\n",PROTOCOL_Structure.HisData);
					  return 0;
			 }
		 }
		 			
			
    return 1;
}


//传入的协议信息处理
//参数Para为是否判断ID 校验和等。
//参数Source为判断来源，有些指令来源错误不判断
//先判断$ 后判断是否校验和ID 都正常才进流程
//若处理正常，会返回$所在的地址，否则返回NULL
uint8_t *PROTOCOL_InforProcess(uint8_t *Protocol_Data, uint8_t Para,
                               uint8_t Source) 
{
		uint8_t *ptr1;
		uint8_t i = 0;

		ptr1 = (uint8_t *)strchr((char *)Protocol_Data, '$'); //查找起始符

		if ((Para & NOCKSCHK) == 0) 
		{              //需要检查校验和
				if (PROTOCOL_CheckSumCheck(ptr1) == 0) 
				{ //检查校验和
					return NULL;                           // CHKWRNG;
				}
		}

		ptr1++;

		switch (*ptr1) 
		{
			case 'S': //设置指令
							i = PROTOCOL_InforProcessSet(ptr1, Source);
							if (i == 1) 
							{
								return ptr1;
							} else 
							{
								return NULL;
							}
			case 'G': // GPRS设置指令
							i = PROTOCOL_InforProcessGPRS((char *)ptr1, Source);
							if (i == 1) 
							{
								return ptr1;
							} else 
							{
								return NULL;
							}
			default:
				return NULL;
		}
}
															 

//计算传入数据的校验和
//直接返回计算的8位数值 不处理！
uint8_t PROTOCOL_CalcCheckSum(uint8_t *Data_In, uint16_t Data_Len) 
{
		uint16_t i = 0;
		uint8_t Sum_Data = 0;
		if (Data_Len < 5) 
		{
			return 0; //太短 不计算
		}
		for (i = 0; i < Data_Len; i++) 
		{
			Sum_Data = (uint8_t)(Sum_Data + Data_In[i]);
		}
		return Sum_Data;
}

//协议的校验和检查
//只能输入$************XX格式 XX为校验和
uint8_t PROTOCOL_CheckSumCheck(uint8_t *Data_Check) 
{
  uint16_t Data_Len = 0;
  uint8_t Sum_Data[2] = {0};
  Data_Len = strlen((char *)Data_Check);
  if (Data_Len < 7 || Data_Len > 200) 
	{ //短或长了
    return 0;
  }
  Sum_Data[0] = PROTOCOL_CalcCheckSum(Data_Check, Data_Len - 2);
  sprintf((char *)Sum_Data, "%02X", Sum_Data[0]); //转到数组里
  if ((Sum_Data[0] == Data_Check[Data_Len - 2]) &&
      (Sum_Data[1] == Data_Check[Data_Len - 1])) 
	{
    return 1;
  }
  USART_SendString(USART1, "WRONG CHECKSUM   ", 0);
  USART_SendString(USART1, Sum_Data, 2);
  USART_SendString(USART1, "\r\n", 2);
  return 0;
}

//处理S开头的指令，配合PROTOCOL_InforProcess函数用，不可单独用
//默认传入的是S所在的地址
//若传入的来源为非串口，则会改写从S开始的内容为传出的内容。
uint8_t PROTOCOL_InforProcessSet(uint8_t *Set_Data, uint8_t Source) {
  Set_Data++;            //到下一位
  switch (*Set_Data) 
	{
				case 'P': //设置服务器
						    Set_Data += 3;
						    memset(FLASH_RW_Buf, 0, 200);
						    sscanf((char *)Set_Data, "%[^;];%[^;];",NB.IP,NB.PORT);
						    NB_WriteBasicOPt();
						    NB_ReadBasicOPt();
						    sprintf((char *)FLASH_RW_Buf, "Server Info: %s : %s \r\n",NB.IP, NB.PORT);
						    if (Source == SRCUART) 
							  {
							    USART_SendString(USART1, FLASH_RW_Buf, 0);
							    break;
						    } else 
								{
									Set_Data -= 4;
									strcpy((char *)Set_Data, (char *)FLASH_RW_Buf);
									break;
						    }
								
			   default:
						    return 0;
  }
  return 1;
}


//处理G开头的设置GPRS信息的指令
//配合PROTOCOL_InforProcess函数用，不可单独用
//传入的是G的地址
//若传入的来源为非串口，则会改写从G开始的内容为传出的内容。
uint8_t PROTOCOL_InforProcessGPRS(char *G_Data, uint8_t Source) 
{
  G_Data++;
  switch (*G_Data) 
	{
				case 'A': //设置APN
								memset(FLASH_RW_Buf, 0, 400);
								G_Data += 3;
								sscanf(G_Data, "%50[^;];", NB.APN);
								NB_WriteBasicOPt();
								sprintf((char *)FLASH_RW_Buf, "APN Info: %s\r\n",NB.APN);
								if (Source == SRCUART) 
								{
									USART_SendString(USART1, FLASH_RW_Buf, 0);
									break;
								} else 
								{
									G_Data -= 4;
									strcpy((char *)G_Data, (char *)FLASH_RW_Buf);
									break;
						    }
         default:
                return 0;
  }
  return 1;
}

