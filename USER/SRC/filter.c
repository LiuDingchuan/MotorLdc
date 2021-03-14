/*
 * @Descripttion: 滤波器
 * @version: 第一版
 * @Author: 叮咚蛋
 * @Date: 2021-01-17 10:44:05
 * @LastEditors: 叮咚蛋
 * @LastEditTime: 2021-01-17 16:49:38
 * @FilePath: \MotoPro\USER\SRC\filter.c
 */
#include "filter.h"

IIR_struct IIR_flt={0};
k_filter kal_filter = {0};

/**
 * @author: 叮咚蛋
 * @brief: 低通滤波初始化
 * @param IIR_struct
 */
void IIR_init(IIR_struct *IIR_flt)	//低通滤波初始化
{
	IIR_flt->a=660;      		      //低通滤波系数
	IIR_flt->last=0;
}

/**
 * @author: 叮咚蛋
 * @brief: 进行一阶低通滤波
 * @param IIR_struct
 * @return double output
 */
/*程序中整数运算比小数运算快，为加快程序的处理速度，为计算方便，a取一整数，1-a用1000-a来代替，a则取0~999*/

double IIR_filter(IIR_struct *IIR_flt,double input)	//一阶滞后低通滤波 -- 运行 us
{
	//本次输出值主要取决于上次滤波输出值，当前采样值对本次输出贡献比较小，起到修正作用；
	IIR_flt->now=(1000-IIR_flt->a)/1000.0f*IIR_flt->last+IIR_flt->a*input/1000.0f;	//这一时刻滤波后的值
	IIR_flt->last=IIR_flt->now;		//上一时刻滤波后的值
	
	return IIR_flt->now;
}

/*减少乘、除的运算次数以提高运算速度。 
具体优化办法： 
先将新采样值与上次滤波结果进行比较，然后根据比较采用不同的公式计算，这样程序的运算效率提高了一倍*/
//TODO:实际运用有bug，电机会卡住一样= =

double IIR_filter_two(IIR_struct *IIR_flt,double input)
{
	if(input<IIR_flt->last)
	{
			IIR_flt->now=IIR_flt->last-input;
			IIR_flt->now=IIR_flt->now*IIR_flt->a;  //滤波系数范围0~1000
			IIR_flt->now=IIR_flt->now+500.0f;//+500是为了四舍五入
			IIR_flt->now=IIR_flt->now/1000.0f;
			IIR_flt->now=IIR_flt->last-IIR_flt->now;
	}
	else if(input>IIR_flt->last)
	{
			IIR_flt->now=input-IIR_flt->last;
			IIR_flt->now=IIR_flt->now*IIR_flt->a;
			IIR_flt->now=IIR_flt->now+500.f;//+500是为了四舍五入
			IIR_flt->now=IIR_flt->now/1000.f;
			IIR_flt->now=IIR_flt->last-IIR_flt->now;
	}
	else IIR_flt->now=IIR_flt->last;
	
	IIR_flt->last=IIR_flt->now;		//上一时刻滤波后的值
	
	return IIR_flt->now;
}

/**
 * @author: 叮咚蛋
 * @brief: 卡尔曼滤波初始化
 */
void kal_flt_init(k_filter* k_flt)	//卡尔曼滤波器初始化
{
//	int i=0;
	k_flt->A = 1;						/*系统状态转移矩阵*/
	k_flt->H = 1;						/*观测矩阵，行矩阵*/

	k_flt->C_last = 1;			/*上次预测过程协方差矩阵 C(k|k-1)*/
	
//	for(i=0;i<MAX_NUM;i++)
//		comb_flt.data[i]= Read_AD_Value();	//读取 AD 的值
//	k_flt->X_last=combination_filter(&comb_flt);	/*系统状态预测矩阵，列矩阵*/

//	k_flt->Q = 0.0001;	   	/*过程噪声*/
//	k_flt->R = 0.0211;			/*观测噪声*/
	k_flt->Q = 0.00001;	   		/*过程噪声*/
	k_flt->R = 0.1;						/*观测噪声*/

	k_flt->input = 0;				/*初始量测值, 即Z(k)为0*/
}

/**
 * @author: 叮咚蛋
 * @brief: 卡尔曼计算
 */

float kalman_filter(k_filter* k_flt, float input)   //卡尔曼滤波,	加了一阶滞后滤波 -- 运行 46.26us
{
	k_flt->input=IIR_filter(&IIR_flt,input);                //一阶滞后滤波法获得测量值, 如果滞后很强就用下面的
	/*量测方程的更新，3组方程*/
//	k_flt->input = input;
	k_flt->kg = (k_flt->C_last)/(k_flt->C_last + k_flt->R);	/*计算卡尔曼增益 kg(k) = C(k|k-1)*H(转置)/[H*C(k|k-1)*H(转置)+R] */
	k_flt->X  = k_flt->X_last + k_flt->kg * (k_flt->input - k_flt->X_last); 	/*得到现在状态的最优化估算值 X(k|k) = X(k|k-1)+kg(k)*[Z(k)-H*X(k|k-1)]*/
	k_flt->C =  (1-k_flt->kg)*(k_flt->C_last);							/*更新 k 状态下 X(k|k) 的协方差 C(k|k) = [I-kg(k)*H]*C(k|k-1) */
	/*积分得角度*/
//	k_flt->output = k_flt->output + k_flt->X*SAMP_TIME;
	/*时间更新方程，2组方程*/
	k_flt->X_last = k_flt->X;																/*根据上一状态预测下一状态的结果 X(k+1|k) = A*X(k|k) 没有控制量*/
	k_flt->C_last = k_flt->C + k_flt->Q;										/*更新 X(k|k-1) 的协方差 C(k+1|k) = A*C(k|k)*A(转置)+B*Q*B(转置)*/

	return k_flt->X;	//返回卡尔曼滤波后的值
}
