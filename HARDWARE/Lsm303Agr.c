#include "main.h"
#include "mpuiic.h"
#include "includes.h"	
#include "Lsm303Agr_driver.h"

#define Threshold   15
extern OS_FLAG_GRP * flags_event;	
extern uint32_t rtcc;

//��ʼ��Lis3dhģ��
uint8_t  Lsm303Agr_Init(void) 
{   
	 u8 sta;
	 u8 err;
	 u8 ThreshHoldL;
	 u8 ThreshHoldH ;
	 delay_ms(3000);
	 LSM303AGR_GetWHO_AM_I(&sta);   //��ȡ�豸ID
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X10);     //�͹���ʹ�ܣ���������ģʽ�� ͨ���Ĵ���0x62 INT_MAGλ��ø�״̬
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_B_M,0X01);     //��ͨ�˲�ʹ��
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_C_M,0X01);     // DRDY�����������
	 ThreshHoldL = (u8)Threshold;
	 ThreshHoldH = (u8)(Threshold >> 8);
	 LSM303AGR_WriteReg(INT_THS_L_REG_M,ThreshHoldL);    //��λ��ֵ����
	 LSM303AGR_WriteReg(INT_THS_H_REG_M,ThreshHoldH);    //��λ��ֵ����
	 LSM303AGR_Cale_OffSet();                            //��ȡƫ��ֵ
	 LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X11);     //�͹���ʹ�ܣ����β���ģʽ����DRDY�ø�ʱ���ؿ���ģʽ
	 LSM303AGR_WriteReg(LSM303AGR_INT_CTRL_REG_M,0X00);  //��ʹ���жϹ���
	 LSM303AGR_ReadReg(LSM303AGR_INT_SOURCE_REG_M,&err); //��ȡ�ж�ֵ
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
