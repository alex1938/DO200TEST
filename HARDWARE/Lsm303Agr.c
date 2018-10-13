#include "main.h"
#include "mpuiic.h"
#include "includes.h"	
#include "Lsm303Agr_driver.h"

#define Threshold   15
extern OS_FLAG_GRP * flags_event;	
extern uint32_t rtcc;

//初始化Lis3dh模块
uint8_t  Lsm303Agr_Init(void) 
{   
	 u8 sta;
	 u8 err;
	 u8 ThreshHoldL;
	 u8 ThreshHoldH ;
	 delay_ms(3000);
	 LSM303AGR_GetWHO_AM_I(&sta);   //读取设备ID
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X10);     //低功耗使能，连续测量模式， 通过寄存器0x62 INT_MAG位获得该状态
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_B_M,0X01);     //低通滤波使能
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_C_M,0X01);     // DRDY引脚数字输出
	 ThreshHoldL = (u8)Threshold;
	 ThreshHoldH = (u8)(Threshold >> 8);
	 LSM303AGR_WriteReg(INT_THS_L_REG_M,ThreshHoldL);    //低位阈值设置
	 LSM303AGR_WriteReg(INT_THS_H_REG_M,ThreshHoldH);    //高位阈值设置
	 LSM303AGR_Cale_OffSet();                            //读取偏置值
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X11);     //低功耗使能，单次测量模式，当DRDY置高时返回空闲模式
	 LSM303AGR_WriteReg(LSM303AGR_INT_CTRL_REG_M,0X00);  //不使能中断功能
	 LSM303AGR_ReadReg(LSM303AGR_INT_SOURCE_REG_M,&err); //读取中断值
	 return 1;
	
}


void EXTI3_IRQHandler(void)
{
	 	u8 err;	
	  EXTI->IMR &= ~EXTI_Line3;
		EXTI_ClearITPendingBit(EXTI_Line3);
		SET_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);
	  OSIntEnter();
		OSFlagPost(flags_event,Ext3IntBit,OS_FLAG_SET,&err);
	  OSIntExit();
}
