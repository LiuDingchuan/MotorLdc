#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f4xx.h"
#include "pid.h"
#include "stdbool.h"
#include "macro.h"
#include "can2.h"
#include "led8.h"
#include "param.h"


typedef struct{
	bool isPosLimitON;//�Ƿ�����λ�ý�������(��ǰ�Ƕ�)
	float maxAngle;//�����ת�Ƕ�
	bool isPosSPLimitOn;//�Ƿ��λ��ģʽ��������
	u16 posSPlimit;//λ��ģʽ�µ��ٶ�����
	bool stuckmotion;//�Ƿ�����ת���
	bool timeoutMotion;//�Ƿ�����ʱ���
	bool isRealseWhenStuck;//�Ƿ��תʱ�ͷŵ��
	s16 zeroSP;//Ѱ��ģʽ�µ��ٶ�
	u16 zeroCurrent;//Ѱ��ģʽ�µ�������
}MotorLimit;

//����ڲ�
typedef struct{
	u16 PULSE;//����������
	float RATIO;//���ٱ�
	u16 CURRENT_LIMIT;//��������������
	float GearRatio;//���ּ��ٱ�
}MotorParam;

//������
typedef struct{
	vs16 current;//����
	vs16 speed;//�ٶ�(rpm)
	vs16 temperature;//�¶�
	vs32 pulse;//�ۼ�����
	vs16 pulseRead;//��������
	float angle;//��ǰ�Ƕ�
}MotorValue;

//���״̬
typedef  struct {
	bool arrived;//λ��ģʽ�µ���ָ��λ��
	bool zero;//Ѱ�����
	bool stuck;//�����ת
	bool timeout;//can���ķ�����ʱ
	bool overspeed;//����
	bool isSetZero;//�Ƿ����õ�ǰλ��Ϊ���
	bool clearFlag;//�ж��Ƿ�Ϊ��һ���ϵ磬������
}MotorStatus;

//����Ҫ���ĵ�һЩ����
typedef struct{
    u32 maxPulse;//�����ת����(����������ת�Ƕ�)
    vs32 lockPulse;//��λ���õ�ǰλ�ü�¼(�����begin)
    s16 distance;//��ǰ����������ǰ�η��������ֵ(������ۼ�����pulse)!!!!!�������ͦ��Ҫ��
    s32 difPulseSet;//��ǰ�ۼ��������趨�����ֵ!!!���Ҳͦ��Ҫ�ģ�������debugģʽ�¿��Բ��ĵĲ�����
    u16 zeroCnt;//Ѱ���ü���ֵ��(�����Ѱ��ģʽzero)
    u8 stuckCnt;//��ת�ü���ֵ
    u8  timeoutCnt;//��ʱ�ü���ֵ
    u32 lastRxTim;//�ϴν��ܵ����ĵ�ϵͳʱ�䣬(����ڳ�ʱ״̬timeout)
    u16 timeoutTicks;//��ʱ�ж����õ�ϵͳ������,[����ڳ�ʱ״̬timeout]
		u16 stuckTicks;//��ת�ж����õ�ϵͳ������
}MotorArgum;

//DJ����ܽṹ��
typedef struct{
	bool enable;//���ʹ��
	bool begin;//�������
	u8 mode;//���ģʽ
	MotorValue valueSet,valueReal,valuePrv;//����趨ֵ��ʵ��ֵ��ǰ��ֵ
	MotorStatus status;//���״̬
	PID_setTypeDef PIDx,PIDs;//pid����
	MotorLimit limit;//���Ʊ���
	MotorArgum argum;//�������ü�ֵ
  MotorParam intrinsic;//����ڲ�
}DJmotor;


extern DJmotor motor[8];
void Motor_Init(void);
void Hook_Motor_Set(s16 id1, s16 id2);
void speed_mode(s16 id);
void position_mode(s16 id);
void zero_mode(s16 id);
void pulse_calculate(u8 id);
u8 ifstuck(u16 id);
void ifDJTimeout(u16 id);

extern MotorParam M3508instrin,M2006instrin;

#endif



