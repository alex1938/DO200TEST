#ifndef __CALCT_H
#define __CALCT_H

#include "stm32f10x.h"
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef union  
{  
    float fdata;  
	  int  idata;
    unsigned long ldata;  
}FloatLongType;  


uint32_t Find_Biggest(uint32_t *arr, uint16_t len);
uint32_t Get_Average(uint32_t *arr, uint16_t len);
//
void Bubble_Sort(uint32_t *arr, uint32_t len);
//寻找出现次数最多的数
uint32_t Find_Most(uint32_t *arr, uint32_t len);
//计算绝对值
uint32_t MYabs(uint32_t B_Num, uint32_t L_Num);
//计算圆或椭圆的面积
//传入的都为直径或长短轴长度
//返回double
double CAL_CircleArea(double a, double b);
//计算直角三角形边长
//输入任意两边长，计算另一边长
//例 输入 3 4 0 返回 5
//   输入 0 4 5 返回 3
double CAL_TriAngleSide(double a, double b, double c);
//计算扇形的面积
// r 为半径
// radian为弧度 非角度
double CAL_FanShapedArea(double r, double radian);
//计算弓形的面积
//输入参数 半径与拱高
//拱高即为 分段点高度 或 液面高度
//输出的为弓形面积
uint8_t CAL_ArcuateArea(double Radius, double Arch_Height,
                        double *Arcuate_Area);
void Float_to_Byte(float f,unsigned char byte[])  ;
void int_to_Byte(int i,unsigned char byte[])  ;
void Byte_to_Float(float *f,unsigned char byte[])  ;

int btea(int* v, int n, int* k) ;
#endif
