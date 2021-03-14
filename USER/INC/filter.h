#ifndef FILTER_H_
#define FILTER_H_

#include "param.h"
/*低通滤波器*/
typedef struct IIR
{
	float last;        //上一时刻滤波后的值
	float now;         //这一时刻滤波后的值
	float a;           //低通滤波系数
}IIR_struct;

/*卡尔曼滤波*/
typedef struct tag_k_filter
{
	s32	A;					/*系统状态转移矩阵*/
//	float B;						/*噪声输入矩阵，列矩阵*/
	s32 H;					/*观测矩阵，行矩阵*/
											/*前三个矩阵为系统的参数矩阵，计算过程已经将矩阵直接代入，程序运算过程不用此矩阵，仅作记录用*/

	float C;						/*预测过程协方差矩阵     C(k|k)  */
	float C_last;				/*上次预测过程协方差矩阵 C(k|k-1)*/
	float Q;						/*过程噪声*/
	float R;						/*观测噪声*/

	float X;						/*系统状态滤波输出矩阵，列矩阵*/
	float X_last;				/*系统状态预测矩阵，列矩阵*/
	float kg;						/*卡尔曼增益，列矩阵*/
	float zero_point;		/*kalman filter zero_point*/					
	float input;				/*量测值，即Z(k)*/
}k_filter;

extern IIR_struct IIR_flt;
extern k_filter kal_filter; 

void IIR_init(IIR_struct *IIR_flt);
double IIR_filter(IIR_struct *IIR_flt,double input);
double IIR_filter_two(IIR_struct *IIR_flt,double input); 

void kal_flt_init(k_filter* k_flt);
float kalman_filter(k_filter* k_flt , float input);

#endif


