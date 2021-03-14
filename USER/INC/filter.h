#ifndef FILTER_H_
#define FILTER_H_

#include "param.h"
/*��ͨ�˲���*/
typedef struct IIR
{
	float last;        //��һʱ���˲����ֵ
	float now;         //��һʱ���˲����ֵ
	float a;           //��ͨ�˲�ϵ��
}IIR_struct;

/*�������˲�*/
typedef struct tag_k_filter
{
	s32	A;					/*ϵͳ״̬ת�ƾ���*/
//	float B;						/*������������о���*/
	s32 H;					/*�۲�����о���*/
											/*ǰ��������Ϊϵͳ�Ĳ������󣬼�������Ѿ�������ֱ�Ӵ��룬����������̲��ô˾��󣬽�����¼��*/

	float C;						/*Ԥ�����Э�������     C(k|k)  */
	float C_last;				/*�ϴ�Ԥ�����Э������� C(k|k-1)*/
	float Q;						/*��������*/
	float R;						/*�۲�����*/

	float X;						/*ϵͳ״̬�˲���������о���*/
	float X_last;				/*ϵͳ״̬Ԥ������о���*/
	float kg;						/*���������棬�о���*/
	float zero_point;		/*kalman filter zero_point*/					
	float input;				/*����ֵ����Z(k)*/
}k_filter;

extern IIR_struct IIR_flt;
extern k_filter kal_filter; 

void IIR_init(IIR_struct *IIR_flt);
double IIR_filter(IIR_struct *IIR_flt,double input);
double IIR_filter_two(IIR_struct *IIR_flt,double input); 

void kal_flt_init(k_filter* k_flt);
float kalman_filter(k_filter* k_flt , float input);

#endif


