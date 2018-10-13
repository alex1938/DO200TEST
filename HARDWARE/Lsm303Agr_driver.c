#include "Lsm303Agr_driver.h"
#include "mpuiic.h"
#include <math.h>

#define ZDirect    120
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
int CarDectectConst = ZDirect*ZDirect;
u8 CarState = 0;
u8 PreCarState = 0;
i16_t CarZOffSet = 100;
int AllmMag = 0;  //������ܵĴ�ǿ

i16_t XYZOffSet[3] = {0};
int XYZSumMagn[3] = {0};
/*******************************************************************************
* Function Name		: LSM303AGR_ReadReg
* Description		: Generic Reading function. It must be fullfilled with either
*			: I2C or SPI reading functions					
* Input			: Register Address
* Output		: Data REad
* Return		: None
*******************************************************************************/
u8_t LSM303AGR_ReadReg(u8_t Reg, u8_t* Data) {
  
   	MPU_IIC_Start();  	
		MPU_IIC_Send_Byte(LSM303AGR_ADDR_W);//����������ַ+д����	
	  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	  MPU_IIC_Send_Byte(Reg);	//д�Ĵ�����ַ
	  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
	  MPU_IIC_Send_Byte(LSM303AGR_ADDR_R);//����������ַ+������	
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
   *Data =MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
    MPU_IIC_Stop();			//����һ��ֹͣ���� 
	  return 1;
	
}


/*******************************************************************************
* Function Name		: LSM303AGR_WriteReg
* Description		: Generic Writing function. It must be fullfilled with either
*			: I2C or SPI writing function
* Input			: Register Address, Data to be written
* Output		: None
* Return		: None
*******************************************************************************/
u8_t LSM303AGR_WriteReg(u8_t WriteAddr, u8_t Data) 
{
  
	 	MPU_IIC_Start(); 
		MPU_IIC_Send_Byte(LSM303AGR_ADDR_W);//����������ַ+д����	
   	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();		 
		return 1;		
	}
	  MPU_IIC_Send_Byte(WriteAddr);//����������ַ+д��?
	  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 	
	  MPU_IIC_Send_Byte(Data);
		if(MPU_IIC_Wait_Ack())	//�ȴ�ACK
	{
		MPU_IIC_Stop();	 
		return 1;		 
	}	
	   MPU_IIC_Stop();	 
		return 0;
}


u8_t LSM303AGR_Write_OffSet(i16_t *pXYZ)
{
		u8 *pData = (u8_t*)pXYZ;
		u8_t i;
		MPU_IIC_Start(); 
		MPU_IIC_Send_Byte(LSM303AGR_ADDR_W);//����������ַ+д����	
   	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
		{
			MPU_IIC_Stop();		 
			return 1;		
		}
	  MPU_IIC_Send_Byte(LSM303AGR_OFFSET_X_REG);//����������ַ+д��?
	  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 	
		for(i = 0; i < 6;i++)
		{
			MPU_IIC_Send_Byte(*pData++);
			if(MPU_IIC_Wait_Ack())	//�ȴ�ACK
			{
				MPU_IIC_Stop();	 
				return 1;		 
			}	
		}
	  MPU_IIC_Stop();	 
		return 0;
}
void LSM303AGR_Read_XYZ_Magnent(u8 RegAddr,i16_t *pXYZ)
{
	  u8_t *pData = (u8_t*)pXYZ;
	  u8_t i;
	  MPU_IIC_Start();  	
		MPU_IIC_Send_Byte(LSM303AGR_ADDR_W);//����������ַ+д����	
	  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	  MPU_IIC_Send_Byte(RegAddr);	//д�Ĵ�����ַ
	  MPU_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MPU_IIC_Start();
	  MPU_IIC_Send_Byte(LSM303AGR_ADDR_R);//����������ַ+������	
    MPU_IIC_Wait_Ack();		//�ȴ�Ӧ�� 
	  for(i = 0; i < 5; i++)
		{
			*pData++ = MPU_IIC_Read_Byte(1);//��ȡ����,����nACK 
		}
		*pData++ =MPU_IIC_Read_Byte(0);//��ȡ����,����nACK 
    MPU_IIC_Stop();			
}

//У׼�ų�
void LSM303AGR_Cale_OffSet(void)
{
		i16_t XYZMagn[10][3] = {0};
	  i16_t TXYZOffSet[3] = {0};
		int XYZSumMagn[3] = {0};
		uint8_t i,j,sta;

//		LSM303AGR_Write_OffSet(TXYZOffSet);  //ƫ�ò���
		
		for(i = 0; i < 10; i++)
		{
				while(1)
				{
						delay_ms(100);
						LSM303AGR_ReadReg(LSM303AGR_STATUS_REG_M,&sta);  //��ȡ�������״̬��
						if((sta & 0x08) != 0)
						{
								LSM303AGR_Read_XYZ_Magnent(LSM303AGR_OUT_DATA,&XYZMagn[i][0]);  //��ȡ����
								break;
						}
				}
		}
		
		for(i = 0; i < 10; i++)
		{
				for(j = 0; j < 3; j++)
				{
						XYZSumMagn[j]  += XYZMagn[i][j];   //�����ĺ�
				}
		}
		
		for(j = 0; j < 3; j++)
		{
				TXYZOffSet[j]  = ( XYZSumMagn[j] / 10 );   //У׼ֵ
		}

		for( j = 0; j < 3; j++ )
		{
				XYZOffSet[j]  = TXYZOffSet[j];
			 
		} 


		printf("Mag Offset value: %d : %d : %d\r\n",XYZOffSet[0],XYZOffSet[1],XYZOffSet[2]);
}

u8 LSM303AGR_Detect_Query_Car(u8 *carsate,int XYZ[3])
{
		static u8 count = 0;   //�ų�ǿ�ȼ���
		u8 i,j,retVal = 0;
		static i16_t XYZMagn[3][3] = {0};  //������������
		
		if( count < 3 )   //
		{
				LSM303AGR_Read_XYZ_Magnent(LSM303AGR_OUT_DATA,&XYZMagn[count][0]);  //��ȡ����
				LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X11);// �͹��ģ���һ����ģʽ
				retVal = 0;
				count++;
		}
		
		if(count == 3)
		{
				LSM303AGR_WriteReg(LSM303AGR_CFG_REG_A_M,0X13);   //�͹��ģ�����ģʽ
			
			  for(i = 0; i  < 3; i++)
				{
						for(j = 0; j < 3; j++)
						{
							XYZSumMagn[j]  += XYZMagn[i][j];          //��3�εľ�ֵ
						}
				}
				retVal = 1;
				count = 0;                                     //������0
				
				for(j = 0; j < 3; j++)
				{
						XYZSumMagn[j]  = ( (XYZSumMagn[j] / 3) - XYZOffSet[j] );  //��ȥƫ��ֵ
						XYZ[j] = XYZSumMagn[j];
				}
				
				
				AllmMag = ((0.6 * XYZSumMagn[0]*XYZSumMagn[0] + 0.6 * XYZSumMagn[1]*XYZSumMagn[1] + 1.8 * XYZSumMagn[2]*XYZSumMagn[2]));
				printf("Now Mag : %04d : %04d : %04d %06d\r\n",XYZSumMagn[0],XYZSumMagn[1],XYZSumMagn[2],AllmMag);
				
				if(AllmMag > CarDectectConst)  //����ų�ֵ�����趨��ֵ���β�Ϊ1
				{		
						*carsate = 1;	
						if( AllmMag > CarDectectConst * 2 )
						{
								retVal = 2;
						}
				}
				else
				{ 
						*carsate = 0;
				}
				
				if( ( AllmMag > 50*50 ) && ( AllmMag < 80*80 ) )
				{
					retVal = 3;
				}
		}
		return retVal;	
}

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : LSM303AGR_GetWHO_AM_I
* Description    : Read identification code by WHO_AM_I register
* Input          : Char to empty by Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
status_t LSM303AGR_GetWHO_AM_I(u8_t* val){
  
  if( !LSM303AGR_ReadReg(LSM303AGR_WHO_AM_I, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;
}


/*******************************************************************************
* Function Name  : LSM303AGR_GetStatusAUX
* Description    : Read the AUX status register
* Input          : Char to empty by status register buffer
* Output         : None
* Return         : Status [MEMS_ERROR, MEMS_SUCCESS]
*******************************************************************************/
status_t LSM303AGR_GetStatusAUX(u8_t* val) {
  
  if( !LSM303AGR_ReadReg(LSM303AGR_STATUS_AUX, val) )
    return MEMS_ERROR;
  
  return MEMS_SUCCESS;  
}



