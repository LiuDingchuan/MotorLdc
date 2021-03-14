/*
 * @Descripttion: �˲���
 * @version: ��һ��
 * @Author: ���˵�
 * @Date: 2021-01-17 10:44:05
 * @LastEditors: ���˵�
 * @LastEditTime: 2021-01-17 16:49:38
 * @FilePath: \MotoPro\USER\SRC\filter.c
 */
#include "filter.h"

IIR_struct IIR_flt={0};
k_filter kal_filter = {0};

/**
 * @author: ���˵�
 * @brief: ��ͨ�˲���ʼ��
 * @param IIR_struct
 */
void IIR_init(IIR_struct *IIR_flt)	//��ͨ�˲���ʼ��
{
	IIR_flt->a=660;      		      //��ͨ�˲�ϵ��
	IIR_flt->last=0;
}

/**
 * @author: ���˵�
 * @brief: ����һ�׵�ͨ�˲�
 * @param IIR_struct
 * @return double output
 */
/*���������������С������죬Ϊ�ӿ����Ĵ����ٶȣ�Ϊ���㷽�㣬aȡһ������1-a��1000-a�����棬a��ȡ0~999*/

double IIR_filter(IIR_struct *IIR_flt,double input)	//һ���ͺ��ͨ�˲� -- ���� us
{
	//�������ֵ��Ҫȡ�����ϴ��˲����ֵ����ǰ����ֵ�Ա���������ױȽ�С�����������ã�
	IIR_flt->now=(1000-IIR_flt->a)/1000.0f*IIR_flt->last+IIR_flt->a*input/1000.0f;	//��һʱ���˲����ֵ
	IIR_flt->last=IIR_flt->now;		//��һʱ���˲����ֵ
	
	return IIR_flt->now;
}

/*���ٳˡ����������������������ٶȡ� 
�����Ż��취�� 
�Ƚ��²���ֵ���ϴ��˲�������бȽϣ�Ȼ����ݱȽϲ��ò�ͬ�Ĺ�ʽ���㣬�������������Ч�������һ��*/
//TODO:ʵ��������bug������Ῠסһ��= =

double IIR_filter_two(IIR_struct *IIR_flt,double input)
{
	if(input<IIR_flt->last)
	{
			IIR_flt->now=IIR_flt->last-input;
			IIR_flt->now=IIR_flt->now*IIR_flt->a;  //�˲�ϵ����Χ0~1000
			IIR_flt->now=IIR_flt->now+500.0f;//+500��Ϊ����������
			IIR_flt->now=IIR_flt->now/1000.0f;
			IIR_flt->now=IIR_flt->last-IIR_flt->now;
	}
	else if(input>IIR_flt->last)
	{
			IIR_flt->now=input-IIR_flt->last;
			IIR_flt->now=IIR_flt->now*IIR_flt->a;
			IIR_flt->now=IIR_flt->now+500.f;//+500��Ϊ����������
			IIR_flt->now=IIR_flt->now/1000.f;
			IIR_flt->now=IIR_flt->last-IIR_flt->now;
	}
	else IIR_flt->now=IIR_flt->last;
	
	IIR_flt->last=IIR_flt->now;		//��һʱ���˲����ֵ
	
	return IIR_flt->now;
}

/**
 * @author: ���˵�
 * @brief: �������˲���ʼ��
 */
void kal_flt_init(k_filter* k_flt)	//�������˲�����ʼ��
{
//	int i=0;
	k_flt->A = 1;						/*ϵͳ״̬ת�ƾ���*/
	k_flt->H = 1;						/*�۲�����о���*/

	k_flt->C_last = 1;			/*�ϴ�Ԥ�����Э������� C(k|k-1)*/
	
//	for(i=0;i<MAX_NUM;i++)
//		comb_flt.data[i]= Read_AD_Value();	//��ȡ AD ��ֵ
//	k_flt->X_last=combination_filter(&comb_flt);	/*ϵͳ״̬Ԥ������о���*/

//	k_flt->Q = 0.0001;	   	/*��������*/
//	k_flt->R = 0.0211;			/*�۲�����*/
	k_flt->Q = 0.00001;	   		/*��������*/
	k_flt->R = 0.1;						/*�۲�����*/

	k_flt->input = 0;				/*��ʼ����ֵ, ��Z(k)Ϊ0*/
}

/**
 * @author: ���˵�
 * @brief: ����������
 */

float kalman_filter(k_filter* k_flt, float input)   //�������˲�,	����һ���ͺ��˲� -- ���� 46.26us
{
	k_flt->input=IIR_filter(&IIR_flt,input);                //һ���ͺ��˲�����ò���ֵ, ����ͺ��ǿ���������
	/*���ⷽ�̵ĸ��£�3�鷽��*/
//	k_flt->input = input;
	k_flt->kg = (k_flt->C_last)/(k_flt->C_last + k_flt->R);	/*���㿨�������� kg(k) = C(k|k-1)*H(ת��)/[H*C(k|k-1)*H(ת��)+R] */
	k_flt->X  = k_flt->X_last + k_flt->kg * (k_flt->input - k_flt->X_last); 	/*�õ�����״̬�����Ż�����ֵ X(k|k) = X(k|k-1)+kg(k)*[Z(k)-H*X(k|k-1)]*/
	k_flt->C =  (1-k_flt->kg)*(k_flt->C_last);							/*���� k ״̬�� X(k|k) ��Э���� C(k|k) = [I-kg(k)*H]*C(k|k-1) */
	/*���ֵýǶ�*/
//	k_flt->output = k_flt->output + k_flt->X*SAMP_TIME;
	/*ʱ����·��̣�2�鷽��*/
	k_flt->X_last = k_flt->X;																/*������һ״̬Ԥ����һ״̬�Ľ�� X(k+1|k) = A*X(k|k) û�п�����*/
	k_flt->C_last = k_flt->C + k_flt->Q;										/*���� X(k|k-1) ��Э���� C(k+1|k) = A*C(k|k)*A(ת��)+B*Q*B(ת��)*/

	return k_flt->X;	//���ؿ������˲����ֵ
}
