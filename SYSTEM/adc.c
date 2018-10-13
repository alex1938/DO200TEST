#include "adc.h"

//通道9是温度通道

void ADC_Configuration(void) {

   ADC_InitTypeDef ADC_InitStructure;

  RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC CLK
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_DeInit(ADC1);                                  // RESET ADC1
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;       //扫描模式开
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
  ADC_InitStructure.ADC_ExternalTrigConv =
      ADC_ExternalTrigConv_None;                         //外部触发关
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 2; // 16;        //通道数目
  ADC_Init(ADC1, &ADC_InitStructure);     //初始化ADC1

  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_239Cycles5);
	
 ADC_DMACmd(ADC1, ENABLE); // ADC DMA SETUP
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1); //复位ADC1校准器
  while (ADC_GetResetCalibrationStatus(ADC1))
    ;                         //等待状态
  ADC_StartCalibration(ADC1); //校准ADC1
  while (ADC_GetCalibrationStatus(ADC1))
    ; //等待完成
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

