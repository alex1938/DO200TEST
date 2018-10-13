//存放一些计算函数

#include "calct.h"
 FloatLongType IntToByte;  
 FloatLongType FloatToByte;
const float PI = 3.1415926;

/* 
将浮点数f转化为4个字节数据存放在byte[4]中 
*/  
void int_to_Byte(int i,unsigned char byte[])  
{  
   
    IntToByte.idata = i;  
//	  printf("fl.fdata is %f\r\n",fl.fdata);
    byte[0]=(char)IntToByte.idata;  
//	printf("%c",byte[0]);	
    byte[1]=(char)(IntToByte.idata>>8);  
//	printf("%c",byte[1]);
    byte[2]=(char)(IntToByte.idata>>16);
//  printf("%c",byte[2]);
    byte[3]=(char)(IntToByte.idata>>24); 
//  printf("%c",byte[3]);	
}  


/*
将4个字节数据byte[4]转化为浮点数存放在*f中 
*/  
void Byte_to_Int(int* i,unsigned char byte[])  
{  
    FloatToByte.ldata=0;  
    FloatToByte.ldata=byte[3];  
    FloatToByte.ldata=((FloatToByte.ldata<<8)|byte[2]);  
    FloatToByte.ldata=((FloatToByte.ldata<<8)|byte[1]);  
    FloatToByte.ldata=((FloatToByte.ldata<<8)|byte[0]);  
    *i =FloatToByte.fdata;  
}

void Float_to_Byte(float f,unsigned char byte[])  
{  
  
    FloatToByte.fdata = f;  
//	  printf("fl.fdata is %f\r\n",fl.fdata);
    byte[0]=(char)FloatToByte.ldata;  
//	printf("%c",byte[0]);	
    byte[1]=(char)(FloatToByte.ldata>>8);  
//	printf("%c",byte[1]);
    byte[2]=(char)(FloatToByte.ldata>>16);
//  printf("%c",byte[2]);
    byte[3]=(char)(FloatToByte.ldata>>24); 
//  printf("%c",byte[3]);	
}  

/* 
将4个字节数据byte[4]转化为浮点数存放在*f中 
*/  
void Byte_to_Float(float *f,unsigned char byte[])  
{  
    FloatToByte.ldata=0;  
    FloatToByte.ldata=byte[3];  
    FloatToByte.ldata=((FloatToByte.ldata<<8)|byte[2]);  
    FloatToByte.ldata=((FloatToByte.ldata<<8)|byte[1]);  
    FloatToByte.ldata=((FloatToByte.ldata<<8)|byte[0]);  
    *f=FloatToByte.fdata;  
}  

  int btea(int* v, int n, int* k) 
{
        int z=v[n-1], y=v[0], sum=0, e, DELTA=0x9e3779b9;
	
        int p, q,fffbuf[4];
        if (n > 1) {          /* Coding Part */
            q = 6 + 52/n;
            while (q-- > 0) {
                sum += DELTA;
                e = (sum >> 2) & 3;
                for (p=0; p<n-1; p++){ 
									y = v[p+1];
									z = v[p] += (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);
									fffbuf[p] =z;
								}
                y = v[0];
                z = v[n-1] += (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);
								fffbuf[n-1] =z;
            }
            return 0 ;
        } else if (n < -1) {  /* Decoding Part */
            n = -n;
            q = 6 + 52/n;
            sum = q*DELTA ;
            while (sum != 0) {
                e = (sum >> 2) & 3;
                for (p=n-1; p>0; p--){
									z = v[p-1];
									y = v[p] -= (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);
								}
                z = v[n-1];
                y = v[0] -= (z>>5^y<<2) + (y>>3^z<<4)^(sum^y) + (k[p&3^e]^z);
                sum -= DELTA;
            }
            return 0;
        }
        return 1;
}
    

//查找数组中最大的数
uint32_t Find_Biggest(uint32_t *arr, uint16_t len) 
{
  uint16_t i;
  uint16_t Big_Number = 0;
  for (i = 0; i < len; i++) 
	{
    if (Big_Number < arr[i]) 
		{
      Big_Number = arr[i];
    }
  }
  return Big_Number;
}

//求得对应数组的平均值
uint32_t Get_Average(uint32_t *arr, uint16_t len) {
  uint16_t i;
  uint32_t Sum = 0;
  for (i = 0; i < len; i++) {
    Sum += arr[i];
  }
  Sum /= len;
  return Sum;
}

//交换位置
void swap(uint32_t *x, uint32_t *y) {
  uint32_t t = *x;
  *x = *y;
  *y = t;
}

//冒泡排序
void Bubble_Sort(uint32_t *arr, uint32_t len) 
{
  uint32_t i, j;
  for (i = 0; i < len - 1; i++)
    for (j = 0; j < len - 1 - i; j++)
      if (arr[j] > arr[j + 1]) {
        swap(&arr[j], &arr[j + 1]);
      }
}

//寻找出现次数最多的数
uint32_t Find_Most(uint32_t *arr, uint32_t len) {
  uint32_t i, temp = 0, Most_Number;
  uint32_t time1 = 0, time2 = 0;
  temp = arr[0];
  Most_Number = temp;
  for (i = 1; i < len; i++) {
    if (arr[i] == temp)
      time2++;
    else {
      if (time2 > time1) {
        time1 = time2;
        time2 = 0;
        Most_Number = temp;
        if (time1 > len / 2)
          break;
      }
      temp = arr[i];
    }
  }
  return Most_Number;
}

//计算整数的绝对值
uint32_t MYabs(uint32_t B_Num, uint32_t L_Num) {
  if (B_Num < L_Num)
    return (L_Num - B_Num);
  else
    return (B_Num - L_Num);
}

//计算圆或椭圆的面积
//传入的都为直径或长短轴长度
//返回double
double CAL_CircleArea(double a, double b) {
  double tt;
  tt = a / 2 * b / 2;
  return tt * PI;
}

//计算直角三角形边长
//输入任意两边长，计算另一边长
//例 输入 3 4 0 返回 5
//   输入 0 4 5 返回 3
double CAL_TriAngleSide(double a, double b, double c) {
  if (a == 0) {
    return sqrt((c + b) * (c - b));
  } else if (b == 0) {
    return sqrt((c + a) * (c - a));
  } else if (c == 0) {
    return sqrt(a * a + b * b);
  } else {
    return 0;
  }
}

//计算扇形的面积
// r 为半径
// radian为弧度 非角度
double CAL_FanShapedArea(double r, double radian) { return radian * r * r / 2; }

//计算弓形的面积
//输入参数 半径与拱高
//拱高即为 分段点高度 或 液面高度
//输出的为弓形面积
uint8_t CAL_ArcuateArea(double Radius, double Arch_Height,
                        double *Arcuate_Area) {
  double Cathetus = 0.0;
  if (Arch_Height < Radius) {                      //劣弧
    Arch_Height = Radius - Arch_Height;                   //求得边心距
    Cathetus = CAL_TriAngleSide(Arch_Height, Cathetus, Radius); //求得底边长
    *Arcuate_Area =
        CAL_FanShapedArea(Radius, 2.0 * acos(Arch_Height / Radius)); //求出扇形面积
    *Arcuate_Area -= Arch_Height * Cathetus; //与三角面积相减得弓形面积
  } else {                                   //优弧
    Arch_Height -= Radius;                   //边心距
    Cathetus = CAL_TriAngleSide(Arch_Height, Cathetus, Radius); //求得底边长
    *Arcuate_Area = CAL_FanShapedArea(
        Radius, 2.0 * (PI - acos(Arch_Height / Radius))); //求出扇形面积
    *Arcuate_Area += Arch_Height * Cathetus; //与三角面积相加得弓形面积
  }
  return 1;
}
