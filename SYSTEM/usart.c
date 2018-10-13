#include "usart.h"

#pragma import(__use_no_semihosting)
struct __FILE {
  int handle;
};
uint8_t flag_usart1,flag_usart3=0;	

FILE __stdout;
_sys_exit(int x) { x = x; }
//重定义fputc函数
int fputc(int ch, FILE *f) {

  while ((USART1->SR & 0X40) == 0)
    ;
  USART1->DR = (u8)ch;
  return ch;
}

USART_DataType USART1_Data;
USART_DataType USART3_Data;

void USART_Configuration(void) {
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  USART_DeInit(USART1); //复位串口1
  USART_DeInit(USART3); //复位串口3
  USART_InitStructure.USART_BaudRate = 115200; //
  USART_InitStructure.USART_WordLength =
      USART_WordLength_8b; //字长为8位数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;    //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None; //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //收发模式

  USART_Init(USART1, &USART_InitStructure);      //初始化串口
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启中断
  USART_Cmd(USART1, ENABLE);                     //使能串口
  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;	 //串口中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  USART_InitStructure.USART_BaudRate = 9600;    //
  USART_Init(USART3, &USART_InitStructure);      //初始化串口
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启中断
  USART_Cmd(USART3, ENABLE);                     //使能串口
  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;	 //串口中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//串口发送字符串，可能发送0x00需带字符长度
void USART_SendString(USART_TypeDef *USARTx, uint8_t *Data, uint16_t Long) 
{
  if (Long > 0) 
	{
    while (Long--) 
		{
      while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
      USART_SendData(USARTx, *Data++);
    }
  } else 
	{
    while (*Data) 
		{
      while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
      USART_SendData(USARTx, *Data++);
    }
  }
}


void USART_ClearBuf(USART_DataType *USART_Data) 
{
  memset(USART_Data->DATA_RX, 0,USART_REC_LEN);
  USART_Data->RX_STA = 0;
}

uint8_t USART_CheckRec(uint8_t *ptr)
{
		if (strstr((char *)USART3_Data.DATA_RX, (char *)ptr) == NULL)
		{	
			return 0;
		}
		else
		{
			return 1;
		}
}

void USART1_IRQHandler(void) //串口1中断服务程序
{
	  uint8_t Res;	
		OSIntEnter();  //关中断
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    Res = USART1->DR; //读取接收到的数据
      if (Res == 0x0D) { //\r舍去
      // nothing
    } else if (Res == 0x0A) { // \n进入处理函数
      if (USART1_Data.RX_STA > 5) {
        USART1_Data.DATA_RX[USART1_Data.RX_STA++] = 0;
        PROTOCOL_InforProcess(USART1_Data.DATA_RX, 0, 0);
        USART1_Data.RX_STA = 0;
      } else {
        USART1_Data.RX_STA = 0;
      }
    } else { //非结尾 保存
			    flag_usart1=1;
          USART1_Data.DATA_RX[USART1_Data.RX_STA++] = Res;
      if (USART1_Data.RX_STA == USART_REC_LEN) {
        USART1_Data.RX_STA = 0;
      }
    }
  }
		OSIntExit();
}


void USART3_IRQHandler(void) //串口1中断服务程序
{
    uint8_t Res;
		OSIntEnter();  //关中断
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        Res = USART3->DR;          
         USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
      if (USART3_Data.RX_STA == USART_REC_LEN) {
         USART3_Data.RX_STA = 0;
    }
  }

		OSIntExit();
}
