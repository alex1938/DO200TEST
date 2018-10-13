/*****************************************************************************
Copyright: 2011-2016, Dingtek Company Co., Ltd.
File name: dac.c
Description: dac输出相关驱动
Author: Zitao Wang
Version: 1.0
Date: 2016-05-23
History:
*******************************/

#include "dac.h"

/*************************************************
Function: DAC_Configuration();
Description: 初始化DAC
Input:  NumOfChannel 配置输出的通道数
Output: 无
Return: 0 配置失败   1 配置成功
Others: 输出通道只能选择 1通道  或1 2 通道，不能单独选择2通道。.
*************************************************/
void DAC_Configuration(void) {
  DAC_InitTypeDef DAC_InitType;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  DAC_InitType.DAC_Trigger = DAC_Trigger_None;
  DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

  DAC_Init(DAC_Channel_1, &DAC_InitType);
  DAC_Cmd(DAC_Channel_1, ENABLE);
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);
}

/*************************************************
Function: DAC_SetOutData();
Description: 配置DAC输出数据
Input:  NumOfChannel 配置输出的通道
        OutType       输出类型 电压 电流
Output: 无
Return: 0 配置失败   1 配置成功
Others: 输出通道只能选择 1通道  或1 2 通道，不能单独选择2通道。
        输入Chalx为百分比数值。
        电压电流为默认外置1倍放大电路。
      例：
          通道1 想输出 1V 电压值，则Chal1应输入 20.0 即20%。
                这样输入DAC寄存器值为620.芯片输出电压为0.5V
                经过放大，为1V
          通道1 想输出 10mA 电流值，则Chal1应输入37.5即37.5%
                这样输入DAC寄存器值为1550.芯片输出电压为1.25V
                经过放大为2.5V 经过转换对应电流为 10mA
*************************************************/
uint8_t DAC_SetOutData(float Chal1) {
  uint16_t Chal1_Data;
  if (DAC_OUT_TYPE == OUT_VOLTAGE) {
    Chal1_Data = (uint16_t)(Chal1 * 31);
  } else if (DAC_OUT_TYPE == OUT_CURRENT) {
    Chal1_Data = (uint16_t)(Chal1 * 24.82 + 620);
  }
  DAC_SetChannel1Data(DAC_Align_12b_R, Chal1_Data);
  // DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);

  return 1;
}
