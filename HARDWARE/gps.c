#include "gps.h"
GPS_TypeDef  GPS_Structure;
char DH_id_sep[24];     //
char GPS_Data_Buf[200]; //
char Longitudebuf[20],Latitudebuf[20];
void GPS_StructInit(void) {

  GPS_Structure.Fix_Status = 0;    
  GPS_Structure.Fix_Mode = 0;            //
  strcpy(GPS_Structure.UTC_Date, "\0");  //
  strcpy(GPS_Structure.UTC_Time, "\0");  //
  GPS_Structure.Latitude =  0.000000;  // 纬度
  GPS_Structure.Longitude = 0.000000; //经度

}


void Creat_DH_Index(char *buffer) {
  uint16_t i, len;
  uint8_t idj;
  memset(DH_id_sep, 0, sizeof(DH_id_sep));
  len = strlen(buffer);
  for (i = 0, idj = 0; i < len; i++) {
    if (buffer[i] == ',') {
      DH_id_sep[idj] = i;
      idj++;
      buffer[i] = 0x00;
    }
  }
}


char *Real_Process_DH(char *buffer, uint8_t num) {
  if (num < 1)
    return &buffer[0];
  return &buffer[DH_id_sep[num - 1] + 1];
}

//读取定位状态
uint8_t GPS_ReadFixStatus(void) {
//  if (GPS_Structure.Fix_Status == 0) {
    return GPS_Structure.Fix_Status;
//  } else {
//    return 1;
//  }
}

//
//$GPGGA,031847.000,4002.3637,N,11630.5601,E,1,8,1.15,60.1,M,-5.7,M,,*70
//uint8_t  GetGpsInformation(void){
//  uint8_t i = 0,*ptr;
//	uint8_t  GpsBuf[100];
//  uint8_t  LOCATION[4];
//	float Latitude,Longitude;
//	do {
//					USART_SendString(USART1, USART2_Data.DATA_RX, 0);
//    if((ptr = (uint8_t *)strstr((char *)USART2_Data.DATA_RX, "$GPGGA"))!=NULL){
//       sscanf((char *)ptr, "%[^*]*", GPS_Data_Buf);
//       Creat_DH_Index(GPS_Data_Buf);
//      if (*Real_Process_DH(GPS_Data_Buf, 6) == '0') { 
//				printf("no fix\r\n");
//        GPS_Structure.Fix_Status = 0;
//				return ResponsError; 
//      } else {
//					printf("fix ok\r\n");
//				  GPS_Structure.Fix_Status = 1; 
//				  memset((char *)GpsBuf,0,20);
//				  memset((char *)Latitudebuf,0,20);  //gps缓存数据纬度
//				 ptr = (uint8_t *)Real_Process_DH(GPS_Data_Buf, 2);
//				     memcpy(GpsBuf, ptr, 9);
//					   memset((char *)LOCATION,0,4);
//				     memcpy(LOCATION, GpsBuf, 2);
//				     Latitude = atof((char *)LOCATION);
//				     GPS_Structure.Latitude =(double)(Latitude+atof((char *)&GpsBuf[2])/60);
//				if(GPS_Structure.Latitude>=90){
//				GPS_Structure.Latitude=90;
//				}else if(GPS_Structure.Latitude<=0){
//					GPS_Structure.Latitude=0;
//				}
//       if(*Real_Process_DH(GPS_Data_Buf, 3)=='N'){
//			 GPS_Structure.Latitude=(-GPS_Structure.Latitude);
//			 }
//			 printf("GPS_Structure.Latitudeis %f\r\n",GPS_Structure.Latitude);
//			 
//			    memset((char *)GpsBuf,0,20);
//			    memset((char *)Longitudebuf,0,20);  //gps缓存数据经度
//			  ptr = (uint8_t *)Real_Process_DH(GPS_Data_Buf, 4);
//			  
//			
//			   memcpy(GpsBuf, ptr, 10);
//			   memset((char *)LOCATION,0,4);
//			  memcpy(LOCATION, GpsBuf, 3);
//			  Longitude = atof((char *)LOCATION);
//			  GPS_Structure.Longitude =(double)(Longitude+atof((char *)&GpsBuf[3])/60);
//			 	if(GPS_Structure.Longitude>=180){
//				GPS_Structure.Longitude=180;
//				}else if(GPS_Structure.Longitude<=0){
//					GPS_Structure.Longitude=0;
//				}
//			   if(*Real_Process_DH(GPS_Data_Buf, 5)=='W'){
//			 GPS_Structure.Longitude = (-GPS_Structure.Longitude);
//			 }
//			  printf("GPS_Structure.Longitude %f\r\n",GPS_Structure.Longitude);
//			   return ResponsOk; 
//        }

//	 
//		}
//      i++;
//		  delay_ms(50);
//  } while (i < 10); // 20s
//  return ResponsOoutTime;   //

//}


//设置相应的输出指令默认输出GGA，间隔2s
//uint8_t SetNMEAOut(void){
//  uint8_t i = 0;
//  USART_ClearBuf(&USART2_Data);
//	USART_SendString(USART2, "$PMTK314,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*2A\r\n", 0);
////		USART_SendString(USART1, "$PMTK314,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*2A\r\n", 0);
//  do {
//			USART_SendString(USART1, USART2_Data.DATA_RX, 0);
//   if (USART2_CheckRec("$PMTK001,314")) {
//      return RESPONSEOK;
//    }
//      i++;
//		  delay_ms(10);
//  } while (i < 10); // 20s
//  return ERROR_NORSP;   //
//}
