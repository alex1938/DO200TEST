
#include "ntc.h"

const float DIV_RES_VALUE = 10.0;
const float NTC_RESON25 = 10.0;
const uint16_t NTC_B_VALUE = 3950;

float Power_Voltage_3V3 = 3.30;
/*****************************************************
计算输入参数的log值
lndata  计算参数
N 计算深度
*****************************************************/
double mylog(double lndata, uint8_t N) {
  int k, nk;
  double x, xx, y;
  x = (lndata - 1) / (lndata + 1);
  xx = x * x;
  nk = 2 * N + 1;
  y = 1.0 / nk;
  for (k = N; k > 0; k--) {
    nk = nk - 2;
    y = 1.0 / nk + xx * y;
  }
  return 2.0 * x * y;
}

uint8_t NTC_GetVoltage(float *BackVoltage) 
{
  *BackVoltage = DMA_GetADCValue(1)/ 4095.0 * Power_Voltage_3V3;
  return 1;
}
/*****************************************************
将AD电压转为对应的电阻值
Pow_Voltage - 电阻的电源电压值 5V 或3.3V
NTC_Voltage - 热敏电阻的AD电压值
Div_Resist - 分压电阻阻值，单位K
BackResist - 返回值，为对应电阻值，单位K
******************************************************/
uint8_t NTC_VoltageToRes(float Pow_Voltage, float NTC_Voltage, float Div_Resist,
                         float *BackResist) {
  float Resist;
  Resist = NTC_Voltage * Div_Resist;
  Resist /= (Pow_Voltage - NTC_Voltage);
  *BackResist = Resist;

  return 1;
}

/******************************************************
返回对应NTC的温度值;
Resist_On25 - NTC在25度时的阻值，单位K
B - NTC的B值
Resist_Now - 当前得到传感器阻值，单位K

******************************************************/
uint8_t NTC_ResToTemperature(float Resist_On25, uint16_t B, float Resist_Now,
                             float *Tem_Now) {
  float Temp;
  Temp = mylog(Resist_Now / Resist_On25, 10);
  Temp = Temp + B / 298.15;
  Temp = B / Temp - 273.15;
  *Tem_Now = Temp - 3.0;
  return 1;
}

uint8_t NTC_GetTemperature(float *BackTemperature) {

  float NTC_Voltage;
  float NTC_Resistance;
  NTC_GetVoltage(&NTC_Voltage); //得到电压值
  NTC_VoltageToRes(Power_Voltage_3V3, NTC_Voltage, DIV_RES_VALUE,
                   &NTC_Resistance); //得到电阻值
  NTC_ResToTemperature(NTC_RESON25, NTC_B_VALUE, NTC_Resistance,
                       BackTemperature); //得到温度值
  return 1;
}
