#include "lora.h"
#include "usart.h"

extern PROTOCOL_TypeDef PROTOCOL_Structure;
extern uint8_t FLASH_RW_Buf[500];


uint8_t  LoRa_Mode(void)
{//»–«÷∏¡Ó  ƒ£ Ω≈‰÷√
	   uint8_t  i = 0;
	   USART_ClearBuf(&USART3_Data);
	   USART_SendString(USART3, "AT+MODE=LWOTAA\r\n", 16);
	   USART_SendString(USART1, "AT+MODE=LWOTAA\r\n", 16);
		do {
			if (USART_CheckRec("WAKEUP")||(USART_CheckRec("LWOTAA"))) 
			{ 
					return 1;
			}    
					i++;
			delay_ms(100);
		} while (i < 100);
		return 0;
	 
}


uint8_t  LoRaotaajion(void)
{
		 uint8_t  i = 0;
		 USART_ClearBuf(&USART3_Data);
		 USART_SendString(USART3, "AT+JOIN\r\n", 0);
		 USART_SendString(USART1, "AT+JOIN\r\n", 0);
	
		do {
//						USART_SendString(USART1, USART3_Data.DATA_RX,0);
					if (USART_CheckRec("Network joined")||USART_CheckRec("Joined already"))						
					{ 
						 return 1;
					 
					}else if(USART_CheckRec("WAKEUP")) 
					{
						 return 2;
					
					}else if(USART_CheckRec("Join failed")) 
					{
						 return 3;
					}
					 i++;
					 delay_ms(200);		
		} while (i < 200);
		return 0;
}

uint8_t  LoRa_ruixingSend(uint8_t *Data)
{
    uint8_t  i = 0;
	  USART_ClearBuf(&USART3_Data);
	  USART_SendString(USART3, Data,0);
	  USART_SendString(USART1, Data,0);

	 do {
	//		 		USART_SendString(USART1, USART3_Data.DATA_RX,0);
			if (USART_CheckRec("Done")) 
			{
//				 USART_SendString(USART1, "AT+CMSGHEX OK\r\n", 0);
					if (USART_CheckRec("RX: \"")) 
					{ 
							 SetBasicInformat(USART3_Data.DATA_RX, strlen((char *)USART3_Data.DATA_RX));
							 USART_SendString(USART1, "RX DATA OK\r\n", 0);
							 delay_ms(100);
					} 
					 USART_SendString(USART1, "Send Data OK\r\n", 0);
					return 1;
			}
		  i++;
      delay_ms(200);
  } while (i < 200);
	 
    return  0;                     //????
}
uint8_t  LoRasleep(void)
{
	 uint8_t  i = 0;
	 USART_ClearBuf(&USART3_Data);
	 USART_SendString(USART3, "AT+LOWPOWER\r\n",0);
	 USART_SendString(USART1, "AT+LOWPOWER\r\n",0);

		do {
				if (USART_CheckRec("SLEEP")) 
				{ 
						return 1;
				 
				} 
				 i++;
				delay_ms(50);		
		 } while (i < 200);
	
  return 0;
}

uint8_t  LoRaJDC(void)
{
	 uint8_t  i = 0;
	 USART_ClearBuf(&USART3_Data);
	 USART_SendString(USART3, "AT+LW=JDC,OFF\r\n", 0);

		do {
					if (USART_CheckRec("+LW: JDC, OFF")) 
					{ 
							return 1;
					} 
			   i++;
			   delay_ms(50);		
		} while (i < 200);
  return 0;
}