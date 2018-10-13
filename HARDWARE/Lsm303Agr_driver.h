
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LSM303AGR_DRIVER__H
#define __LSM303AGR_DRIVER__H
#include <stdio.h>
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#include "delay.h"


//these could change accordingly with the architecture

#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef signed char i8_t;

#endif /*__ARCHDEP__TYPES*/

typedef u8_t LSM303AGR_IntPinConf_t;
typedef u8_t LSM303AGR_Axis_t;
typedef u8_t LSM303AGR_Int1Conf_t;


//define structure
#ifndef __SHARED__TYPES
#define __SHARED__TYPES

typedef enum 
{
  MEMS_SUCCESS				=		0x01,
  MEMS_ERROR				=		0x00	
} status_t;

typedef enum 
{
  MEMS_ENABLE				=		0x01,
  MEMS_DISABLE				=		0x00	
} State_t;

typedef struct 
{
  i16_t AXIS_X;
  i16_t AXIS_Y;
  i16_t AXIS_Z;
} AxesRaw_t;

#endif /*__SHARED__TYPES*/

typedef enum {  
  LSM303AGR_ODR_10Hz                      =		0x00,
  LSM303AGR_ODR_20Hz		        =		0x01,
  LSM303AGR_ODR_50Hz		        =		0x02,
  LSM303AGR_ODR_100Hz		        =		0x053	
} LSM303AGR_ODR_t;

typedef enum {
  LSM303AGR_Continuous            =		0x00,
  LSM303AGR_SIGNAL  			  =		0x01,
  LSM303AGR_IDLE			      =		0x02
} LSM303AGR_Mode_t;

typedef enum {
  LSM303AGR_HPM_NORMAL_MODE_RES           =               0x00,
  LSM303AGR_HPM_REF_SIGNAL                =               0x01,
  LSM303AGR_HPM_NORMAL_MODE               =               0x02,
  LSM303AGR_HPM_AUTORESET_INT             =               0x03
} LSM303AGR_HPFMode_t;

typedef enum {
  LSM303AGR_HPFCF_0                       =               0x00,
  LSM303AGR_HPFCF_1                       =               0x01,
  LSM303AGR_HPFCF_2                       = 		0x02,
  LSM303AGR_HPFCF_3                       =               0x03
} LSM303AGR_HPFCutOffFreq_t;

typedef struct {
  u16_t AUX_1;
  u16_t AUX_2;
  u16_t AUX_3;
} LSM303AGR_Aux123Raw_t;

typedef enum {
  LSM303AGR_FULLSCALE_2                   =               0x00,
  LSM303AGR_FULLSCALE_4                   =               0x01,
  LSM303AGR_FULLSCALE_8                   =               0x02,
  LSM303AGR_FULLSCALE_16                  =               0x03
} LSM303AGR_Fullscale_t;

typedef enum {
  LSM303AGR_BLE_LSB			=		0x00,
  LSM303AGR_BLE_MSB			=		0x01
} LSM303AGR_Endianess_t;

typedef enum {
  LSM303AGR_SELF_TEST_DISABLE             =               0x00,
  LSM303AGR_SELF_TEST_0                   =               0x01,
  LSM303AGR_SELF_TEST_1                   =               0x02
} LSM303AGR_SelfTest_t;

typedef enum {
  LSM303AGR_FIFO_BYPASS_MODE              =               0x00,
  LSM303AGR_FIFO_MODE                     =               0x01,
  LSM303AGR_FIFO_STREAM_MODE              =               0x02,
  LSM303AGR_FIFO_TRIGGER_MODE             =               0x03,
  LSM303AGR_FIFO_DISABLE                  =               0x04
} LSM303AGR_FifoMode_t;

typedef enum {
  LSM303AGR_TRIG_INT1                     =		0x00,
  LSM303AGR_TRIG_INT2 			=		0x01
} LSM303AGR_TrigInt_t;

typedef enum {
  LSM303AGR_SPI_4_WIRE                    =               0x00,
  LSM303AGR_SPI_3_WIRE                    =               0x01
} LSM303AGR_SPIMode_t;

typedef enum {
  LSM303AGR_X_ENABLE                      =               0x01,
  LSM303AGR_X_DISABLE                     =               0x00,
  LSM303AGR_Y_ENABLE                      =               0x02,
  LSM303AGR_Y_DISABLE                     =               0x00,
  LSM303AGR_Z_ENABLE                      =               0x04,
  LSM303AGR_Z_DISABLE                     =               0x00    
} LSM303AGR_AXISenable_t;

typedef enum {
  LSM303AGR_INT1_6D_4D_DISABLE            =               0x00,
  LSM303AGR_INT1_6D_ENABLE                =               0x01,
  LSM303AGR_INT1_4D_ENABLE                =               0x02 
} LSM303AGR_INT_6D_4D_t;

typedef enum {
  LSM303AGR_UP_SX                         =               0x44,
  LSM303AGR_UP_DX                         =               0x42,
  LSM303AGR_DW_SX                         =               0x41,
  LSM303AGR_DW_DX                         =               0x48,
  LSM303AGR_TOP                           =               0x60,
  LSM303AGR_BOTTOM                        =               0x50
} LSM303AGR_POSITION_6D_t;

typedef enum {
  LSM303AGR_INT_MODE_OR                   =               0x00,
  LSM303AGR_INT_MODE_6D_MOVEMENT          =               0x01,
  LSM303AGR_INT_MODE_AND                  =               0x02,
  LSM303AGR_INT_MODE_6D_POSITION          =               0x03  
} LSM303AGR_Int1Mode_t;


//interrupt click response
//  b7 = don't care   b6 = IA  b5 = DClick  b4 = Sclick  b3 = Sign  
//  b2 = z      b1 = y     b0 = x
typedef enum {
LSM303AGR_DCLICK_Z_P                      =               0x24,
LSM303AGR_DCLICK_Z_N                      =               0x2C,
LSM303AGR_SCLICK_Z_P                      =               0x14,
LSM303AGR_SCLICK_Z_N                      =               0x1C,
LSM303AGR_DCLICK_Y_P                      =               0x22,
LSM303AGR_DCLICK_Y_N                      =               0x2A,
LSM303AGR_SCLICK_Y_P                      =               0x12,
LSM303AGR_SCLICK_Y_N			=		0x1A,
LSM303AGR_DCLICK_X_P                      =               0x21,
LSM303AGR_DCLICK_X_N                      =               0x29,
LSM303AGR_SCLICK_X_P                      =               0x11,
LSM303AGR_SCLICK_X_N                      =               0x19,
LSM303AGR_NO_CLICK                        =               0x00
} LSM303AGR_Click_Response; 

//TODO: start from here and manage the shared macros etc before this

/* Exported constants --------------------------------------------------------*/

#ifndef __SHARED__CONSTANTS
#define __SHARED__CONSTANTS

#define MEMS_SET                                        0x01
#define MEMS_RESET                                      0x00

#endif /*__SHARED__CONSTANTS*/


//Register Definition
#define LSM303AGR_OFFSET_X_REG     0x45
#define LSM303AGR_WHO_AM_I				 0x4F  // device identification register
#define LSM303AGR_ADDR_R				   0x3D  // device identification register
#define LSM303AGR_ADDR_W				   0x3C  // device identification register

// CONTROL REGISTER 1
#define LSM303AGR_CFG_REG_A_M				0x60
#define LSM303AGR_LPEN					BIT(4)
#define LSM303AGR_ZEN					BIT(2)
#define LSM303AGR_ODR					BIT(2)
#define LSM303AGR_MD					BIT(0)

//CONTROL REGISTER 2
#define LSM303AGR_CFG_REG_B_M				0x61
#define LSM303AGR_HPM     				BIT(6)
#define LSM303AGR_HPCF					BIT(4)
#define LSM303AGR_FDS					BIT(3)
#define LSM303AGR_HPCLICK					BIT(2)
#define LSM303AGR_HPIS2					BIT(1)
#define LSM303AGR_HPIS1					BIT(0)

//CONTROL REGISTER 3
#define LSM303AGR_CFG_REG_C_M				0x62
#define LSM303AGR_I1_CLICK				BIT(7)
#define LSM303AGR_I1_AOI1					BIT(6)
#define LSM303AGR_I1_AOI2				        BIT(5)
#define LSM303AGR_I1_DRDY1				BIT(4)
#define LSM303AGR_I1_DRDY2				BIT(3)
#define LSM303AGR_I1_WTM					BIT(2)
#define LSM303AGR_I1_ORUN					BIT(1)

//CONTROL REGISTER 6
#define LSM303AGR_INT_CTRL_REG_M				0x63
#define LSM303AGR_XIEN				BIT(7)
#define LSM303AGR_YIEN				BIT(6)
#define LSM303AGR_ZIEN				BIT(5)
#define LSM303AGR_IEA       	BIT(2)
#define LSM303AGR_IEL				  BIT(1)
#define LSM303AGR_IEN				  BIT(0)
//TEMPERATURE CONFIG REGISTER
#define LSM303AGR_INT_SOURCE_REG_M				0x64


//CONTROL REGISTER 4
#define INT_THS_L_REG_M				0x65
#define LSM303AGR_BDU					BIT(7)
#define LSM303AGR_BLE					BIT(6)
#define LSM303AGR_FS					BIT(4)
#define LSM303AGR_HR					BIT(3)
#define LSM303AGR_ST       				BIT(1)
#define LSM303AGR_SIM					BIT(0)

//CONTROL REGISTER 5
#define INT_THS_H_REG_M				0x66
#define LSM303AGR_BOOT                                    BIT(7)
#define LSM303AGR_FIFO_EN                                 BIT(6)
#define LSM303AGR_LIR_INT1                                BIT(3)
#define LSM303AGR_D4D_INT1                                BIT(2)

//REFERENCE/DATA_CAPTURE
#define LSM303AGR_REFERENCE_REG		                0x26
#define LSM303AGR_REF		                	BIT(0)

//STATUS_REG_AXIES
#define LSM303AGR_STATUS_REG_M			                      	0x67
#define LSM303AGR_OUT_DATA				                         0x68
#define LSM303AGR_ZYXOR                                   BIT(7)
#define LSM303AGR_ZOR                                     BIT(6)
#define LSM303AGR_YOR                                     BIT(5)
#define LSM303AGR_XOR                                     BIT(4)
#define LSM303AGR_ZYXDA                                   BIT(3)
#define LSM303AGR_ZDA                                     BIT(2)
#define LSM303AGR_YDA                                     BIT(1)
#define LSM303AGR_XDA                                     BIT(0)

//STATUS_REG_AUX
#define LSM303AGR_STATUS_AUX				0x07

//INTERRUPT 1 CONFIGURATION
#define LSM303AGR_INT1_CFG				0x30
#define LSM303AGR_ANDOR                                   BIT(7)
#define LSM303AGR_INT_6D                                  BIT(6)
#define LSM303AGR_ZHIE                                    BIT(5)
#define LSM303AGR_ZLIE                                    BIT(4)
#define LSM303AGR_YHIE                                    BIT(3)
#define LSM303AGR_YLIE                                    BIT(2)
#define LSM303AGR_XHIE                                    BIT(1)
#define LSM303AGR_XLIE                                    BIT(0)

//FIFO CONTROL REGISTER
#define LSM303AGR_FIFO_CTRL_REG                           0x2E
#define LSM303AGR_FM                                      BIT(6)
#define LSM303AGR_TR                                      BIT(5)
#define LSM303AGR_FTH                                     BIT(0)

//CONTROL REG3 bit mask
#define LSM303AGR_CLICK_ON_PIN_INT1_ENABLE                0x80
#define LSM303AGR_CLICK_ON_PIN_INT1_DISABLE               0x00
#define LSM303AGR_I1_INT1_ON_PIN_INT1_ENABLE              0x40
#define LSM303AGR_I1_INT1_ON_PIN_INT1_DISABLE             0x00
#define LSM303AGR_I1_INT2_ON_PIN_INT1_ENABLE              0x20
#define LSM303AGR_I1_INT2_ON_PIN_INT1_DISABLE             0x00
#define LSM303AGR_I1_DRDY1_ON_INT1_ENABLE                 0x10
#define LSM303AGR_I1_DRDY1_ON_INT1_DISABLE                0x00
#define LSM303AGR_I1_DRDY2_ON_INT1_ENABLE                 0x08
#define LSM303AGR_I1_DRDY2_ON_INT1_DISABLE                0x00
#define LSM303AGR_WTM_ON_INT1_ENABLE                      0x04
#define LSM303AGR_WTM_ON_INT1_DISABLE                     0x00
#define LSM303AGR_INT1_OVERRUN_ENABLE                     0x02
#define LSM303AGR_INT1_OVERRUN_DISABLE                    0x00

//CONTROL REG6 bit mask
#define LSM303AGR_CLICK_ON_PIN_INT2_ENABLE                0x80
#define LSM303AGR_CLICK_ON_PIN_INT2_DISABLE               0x00
#define LSM303AGR_I2_INT1_ON_PIN_INT2_ENABLE              0x40
#define LSM303AGR_I2_INT1_ON_PIN_INT2_DISABLE             0x00
#define LSM303AGR_I2_INT2_ON_PIN_INT2_ENABLE              0x20
#define LSM303AGR_I2_INT2_ON_PIN_INT2_DISABLE             0x00
#define LSM303AGR_I2_BOOT_ON_INT2_ENABLE                  0x10
#define LSM303AGR_I2_BOOT_ON_INT2_DISABLE                 0x00
#define LSM303AGR_INT_ACTIVE_HIGH                         0x00
#define LSM303AGR_INT_ACTIVE_LOW                          0x02

//INT1_CFG bit mask
#define LSM303AGR_INT1_AND                                0x80
#define LSM303AGR_INT1_OR                                 0x00
#define LSM303AGR_INT1_ZHIE_ENABLE                        0x20
#define LSM303AGR_INT1_ZHIE_DISABLE                       0x00
#define LSM303AGR_INT1_ZLIE_ENABLE                        0x10
#define LSM303AGR_INT1_ZLIE_DISABLE                       0x00
#define LSM303AGR_INT1_YHIE_ENABLE                        0x08
#define LSM303AGR_INT1_YHIE_DISABLE                       0x00
#define LSM303AGR_INT1_YLIE_ENABLE                        0x04
#define LSM303AGR_INT1_YLIE_DISABLE                       0x00
#define LSM303AGR_INT1_XHIE_ENABLE                        0x02
#define LSM303AGR_INT1_XHIE_DISABLE                       0x00
#define LSM303AGR_INT1_XLIE_ENABLE                        0x01
#define LSM303AGR_INT1_XLIE_DISABLE                       0x00

//INT1_SRC bit mask
#define LSM303AGR_INT1_SRC_IA                             0x40
#define LSM303AGR_INT1_SRC_ZH                             0x20
#define LSM303AGR_INT1_SRC_ZL                             0x10
#define LSM303AGR_INT1_SRC_YH                             0x08
#define LSM303AGR_INT1_SRC_YL                             0x04
#define LSM303AGR_INT1_SRC_XH                             0x02
#define LSM303AGR_INT1_SRC_XL                             0x01

//INT1 REGISTERS
#define LSM303AGR_INT1_THS                                0x32
#define LSM303AGR_INT1_DURATION                           0x33

//INTERRUPT 1 SOURCE REGISTER
#define LSM303AGR_INT1_SRC				0x31

//FIFO Source Register bit Mask
#define LSM303AGR_FIFO_SRC_WTM                            0x80
#define LSM303AGR_FIFO_SRC_OVRUN                          0x40
#define LSM303AGR_FIFO_SRC_EMPTY                          0x20
  
//INTERRUPT CLICK REGISTER
#define LSM303AGR_CLICK_CFG				0x38
//INTERRUPT CLICK CONFIGURATION bit mask
#define LSM303AGR_ZD_ENABLE                               0x20
#define LSM303AGR_ZD_DISABLE                              0x00
#define LSM303AGR_ZS_ENABLE                               0x10
#define LSM303AGR_ZS_DISABLE                              0x00
#define LSM303AGR_YD_ENABLE                               0x08
#define LSM303AGR_YD_DISABLE                              0x00
#define LSM303AGR_YS_ENABLE                               0x04
#define LSM303AGR_YS_DISABLE                              0x00
#define LSM303AGR_XD_ENABLE                               0x02
#define LSM303AGR_XD_DISABLE                              0x00
#define LSM303AGR_XS_ENABLE                               0x01
#define LSM303AGR_XS_DISABLE                              0x00

//INTERRUPT CLICK SOURCE REGISTER
#define LSM303AGR_CLICK_SRC                               0x39
//INTERRUPT CLICK SOURCE REGISTER bit mask
#define LSM303AGR_IA                                      0x40
#define LSM303AGR_DCLICK                                  0x20
#define LSM303AGR_SCLICK                                  0x10
#define LSM303AGR_CLICK_SIGN                              0x08
#define LSM303AGR_CLICK_Z                                 0x04
#define LSM303AGR_CLICK_Y                                 0x02
#define LSM303AGR_CLICK_X                                 0x01

//Click-click Register
#define LSM303AGR_CLICK_THS                               0x3A
#define LSM303AGR_TIME_LIMIT                              0x3B
#define LSM303AGR_TIME_LATENCY                            0x3C
#define LSM303AGR_TIME_WINDOW                             0x3D

//OUTPUT REGISTER
#define LSM303AGR_OUT_X_L					0x28
#define LSM303AGR_OUT_X_H					0x29
#define LSM303AGR_OUT_Y_L					0x2A
#define LSM303AGR_OUT_Y_H					0x2B
#define LSM303AGR_OUT_Z_L					0x2C
#define LSM303AGR_OUT_Z_H					0x2D

//AUX REGISTER
#define LSM303AGR_OUT_1_L					0x08
#define LSM303AGR_OUT_1_H					0x09
#define LSM303AGR_OUT_2_L					0x0A
#define LSM303AGR_OUT_2_H					0x0B
#define LSM303AGR_OUT_3_L					0x0C
#define LSM303AGR_OUT_3_H					0x0D

//STATUS REGISTER bit mask
#define LSM303AGR_STATUS_REG_ZYXOR                        0x80    // 1	:	new data set has over written the previous one
							// 0	:	no overrun has occurred (default)	
#define LSM303AGR_STATUS_REG_ZOR                          0x40    // 0	:	no overrun has occurred (default)
							// 1	:	new Z-axis data has over written the previous one
#define LSM303AGR_STATUS_REG_YOR                          0x20    // 0	:	no overrun has occurred (default)
							// 1	:	new Y-axis data has over written the previous one
#define LSM303AGR_STATUS_REG_XOR                          0x10    // 0	:	no overrun has occurred (default)
							// 1	:	new X-axis data has over written the previous one
#define LSM303AGR_STATUS_REG_ZYXDA                        0x08    // 0	:	a new set of data is not yet avvious one
                                                        // 1	:	a new set of data is available 
#define LSM303AGR_STATUS_REG_ZDA                          0x04    // 0	:	a new data for the Z-Axis is not availvious one
                                                        // 1	:	a new data for the Z-Axis is available
#define LSM303AGR_STATUS_REG_YDA                          0x02    // 0	:	a new data for the Y-Axis is not available
                                                        // 1	:	a new data for the Y-Axis is available
#define LSM303AGR_STATUS_REG_XDA                          0x01    // 0	:	a new data for the X-Axis is not available

#define LSM303AGR_DATAREADY_BIT                           LSM303AGR_STATUS_REG_ZYXDA


//STATUS AUX REGISTER bit mask
#define LSM303AGR_STATUS_AUX_321OR                         0x80
#define LSM303AGR_STATUS_AUX_3OR                           0x40
#define LSM303AGR_STATUS_AUX_2OR                           0x20
#define LSM303AGR_STATUS_AUX_1OR                           0x10
#define LSM303AGR_STATUS_AUX_321DA                         0x08
#define LSM303AGR_STATUS_AUX_3DA                           0x04
#define LSM303AGR_STATUS_AUX_2DA                           0x02
#define LSM303AGR_STATUS_AUX_1DA                           0x01

#define LSM303AGR_MEMS_I2C_ADDRESS			        0x33

//FIFO REGISTERS
#define LSM303AGR_FIFO_CTRL_REG			        0x2E
#define LSM303AGR_FIFO_SRC_REG			        0x2F


/* Exported macro ------------------------------------------------------------*/

#ifndef __SHARED__MACROS

#define __SHARED__MACROS
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#define BIT(x) ( (x) )

#endif /*__SHARED__MACROS*/

/* Exported functions --------------------------------------------------------*/
//Sensor Configuration Functions
void LSM303AGR_Read_XYZ_Magnent(u8 RegAddr,i16_t *pXYZ);
u8_t LSM303AGR_Write_OffSet(i16_t *pXYZ);
void LSM303AGR_Cale_OffSet(void);
u8 LSM303AGR_Detect_Car(u8 *carsate);
u8 LSM303AGR_Detect_Query_Car(u8 *carsate,int XYZ[3]);
status_t LSM303AGR_SetODR(LSM303AGR_ODR_t ov);
status_t LSM303AGR_SetMode(LSM303AGR_Mode_t md);
status_t LSM303AGR_SetAxis(LSM303AGR_Axis_t axis);
status_t LSM303AGR_SetFullScale(LSM303AGR_Fullscale_t fs);
status_t LSM303AGR_SetBDU(State_t bdu);
status_t LSM303AGR_SetBLE(LSM303AGR_Endianess_t ble);
status_t LSM303AGR_SetSelfTest(LSM303AGR_SelfTest_t st);
status_t LSM303AGR_SetTemperature(State_t state);
status_t LSM303AGR_SetADCAux(State_t state);

//Filtering Functions
status_t LSM303AGR_HPFClickEnable(State_t hpfe);
status_t LSM303AGR_HPFAOI1Enable(State_t hpfe);
status_t LSM303AGR_HPFAOI2Enable(State_t hpfe);
status_t LSM303AGR_SetHPFMode(LSM303AGR_HPFMode_t hpf);
status_t LSM303AGR_SetHPFCutOFF(LSM303AGR_HPFCutOffFreq_t hpf);
status_t LSM303AGR_SetFilterDataSel(State_t state);

//Interrupt Functions
status_t LSM303AGR_SetInt1Pin(LSM303AGR_IntPinConf_t pinConf);
status_t LSM303AGR_SetInt2Pin(LSM303AGR_IntPinConf_t pinConf);
status_t LSM303AGR_Int1LatchEnable(State_t latch);
status_t LSM303AGR_ResetInt1Latch(void);
status_t LSM303AGR_SetIntConfiguration(LSM303AGR_Int1Conf_t ic);
status_t LSM303AGR_SetInt1Threshold(u8_t ths);
status_t LSM303AGR_SetInt1Duration(LSM303AGR_Int1Conf_t id);
status_t LSM303AGR_SetIntMode(LSM303AGR_Int1Mode_t ic);
status_t LSM303AGR_SetClickCFG(u8_t status);
status_t LSM303AGR_SetInt6D4DConfiguration(LSM303AGR_INT_6D_4D_t ic);
status_t LSM303AGR_GetInt1Src(u8_t* val);
status_t LSM303AGR_GetInt1SrcBit(u8_t statusBIT, u8_t* val);

//FIFO Functions
status_t LSM303AGR_FIFOModeEnable(LSM303AGR_FifoMode_t fm);
status_t LSM303AGR_SetWaterMark(u8_t wtm);
status_t LSM303AGR_SetTriggerInt(LSM303AGR_TrigInt_t tr);
status_t LSM303AGR_GetFifoSourceReg(u8_t* val);
status_t LSM303AGR_GetFifoSourceBit(u8_t statusBIT, u8_t* val);
status_t LSM303AGR_GetFifoSourceFSS(u8_t* val);

//Other Reading Functions
status_t LSM303AGR_GetStatusReg(u8_t* val);
status_t LSM303AGR_GetStatusBit(u8_t statusBIT, u8_t* val);
status_t LSM303AGR_GetStatusAUXBit(u8_t statusBIT, u8_t* val);
status_t LSM303AGR_GetStatusAUX(u8_t* val);
status_t LSM303AGR_GetAccAxesRaw(AxesRaw_t* buff);
status_t LSM303AGR_GetAuxRaw(LSM303AGR_Aux123Raw_t* buff);
status_t LSM303AGR_GetClickResponse(u8_t* val);
status_t LSM303AGR_GetTempRaw(i8_t* val);
status_t LSM303AGR_GetWHO_AM_I(u8_t* val);
status_t LSM303AGR_Get6DPosition(u8_t* val);
u8_t LSM303AGR_WriteReg(u8_t WriteAddr, u8_t Data);
u8_t LSM303AGR_ReadReg(u8_t Reg, u8_t* Data);
//Generic
// i.e. u8_t LSM303AGR_ReadReg(u8_t Reg, u8_t* Data);
// i.e. u8_t LSM303AGR_WriteReg(u8_t Reg, u8_t Data);


#endif /* __LSM303AGR_H */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/



