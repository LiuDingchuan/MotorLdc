#ifndef _ELMO_H
#define _ELMO_H

#include "param.h"
#include "stdarg.h"
#include "queue.h"

#define GetData	1
#define	SetData 2

/****ELMO CAN����ID****/
#define Elmo_Motor1_RX	0x281
#define Elmo_Motor2_RX	0x282
#define Elmo_Motor3_RX	0x283
#define Elmo_Motor4_RX	0x284
#define Elmo_Motor5_RX	0x285
#define Elmo_Motor6_RX	0x286
#define Elmo_Motor7_RX	0x287
#define Elmo_Motor8_RX	0x288
#define Elmo_Motor1_TX	0x301
#define Elmo_Motor2_TX	0x302
#define Elmo_Motor3_TX	0x303
#define Elmo_Motor4_TX	0x304
#define Elmo_Motor5_TX	0x305
#define Elmo_Motor6_TX	0x306
#define Elmo_Motor7_TX	0x307
#define Elmo_Motor8_TX	0x308
#define Elmo_Motor1_error 0x81
#define Elmo_Motor2_error 0x82
#define Elmo_Motor3_error 0x83
#define Elmo_Motor4_error 0x84
#define Elmo_Motor5_error 0x85
#define Elmo_Motor6_error 0x86
#define Elmo_Motor7_error 0x87
#define Elmo_Motor8_error 0x88

/****ELMO�ڲ�****/
typedef struct{
    u16 PULSE;//����������
    float  RATIO;//���ٱ�
}ELMOParam;

/****ELMO���****/
typedef struct{
	vs16 angle;//��ǰ�Ƕ�
	vs16 current;//����
	vs16 speed;//�ٶ�(rpm,�������ʱת��ΪJVֵ��SPֵ)
	vs32 pulse;
}ELMOValue;

/****���״̬****/
typedef struct{
	bool arrived;//λ��ģʽ�µ���ָ��ģʽ
	bool zero;//Ѱ�����
	bool stuck;//�����ת
	bool timeout;//can���ķ�����ʱ
}ELMOStatus;

/****ELMO���Ʊ����ṹ��****/
typedef struct{
    bool isPosLimitON;//�Ƿ�����λ�ý������ƣ�����ǰ�Ƕȣ�
    float maxAngle;//�����ת�Ƕ�
    bool isPosSPLimitOn;//�Ƿ��λ��ģʽ��������
    u16 posSPlimit;//λ��ģʽ�µ��ٶ�����
	  bool timeoutMotion;//�Ƿ�����ʱ���
    bool isRealseWhenStuck;//�Ƿ��תʱ�ͷŵ��
    u16 zeroSP;//Ѱ��ģʽ�µ��ٶ�
    u16 zeroCurrent;//Ѱ��ģʽ�µ�������
}ELMOLimit;

/****����Ҫ���ĵ�һЩ����****/
typedef struct{
    u32 maxPulse;//�����ת���壬������������ת�Ƕȣ�
    u16 zeroCnt;//Ѱ���ü���ֵ���������Ѱ��״̬zero��
    vu16 stuckCnt;//��ת�ü���ֵ��������ڶ�ת״̬stuck��
    u16 timeoutCnt;//��ʱ�ü���ֵ������ڳ�ʱ״̬timeout��
    u32 lastRxTim;//�ϴν��յ����ĵ�ϵͳʱ�䣨����ڳ�ʱ״̬timeout��
    u16 timeoutTicks;//�жϳ�ʱ��ϵͳ��������������ڳ�ʱ״̬timeout��
}ELMOArgum;

/****ELMO�ܽṹ��****/
typedef struct{
  u8 mode;//���ģʽ
  bool enable;//���ʹ��
  ELMOValue valueSet,valueReal;//����趨ֵ��ʵ��ֵ
  ELMOStatus status;//���״̬
  ELMOLimit limit;//���Ʊ���
  ELMOArgum argum;//�������ü�ֵ
  ELMOParam intrinsic;//����ڲ�
}ELMO_Motor;

extern ELMO_Motor ELMOmotor[5];

void EcodeFloatData_to_4byte(float *f, u8 *buff);
void ELMO_Motor_Init(void);
void ELMO_Init(CAN_TypeDef* CANx);
void AC(u32 id,u8 cmd,u32 valueSet);
void BG(u32 id);
void DC(u32 id,u8 cmd,u32 valueSet);
void IQ(u32 id);
void JV(u32 id,u8 cmd);
void MO(u32 id,u8 cmd,u32 status);
void PA(u32 id,u8 cmd);
void ELMO_PR(u32 id,u8 cmd);
void PE(u32 id);
void PX(u32 id,u8 cmd);
void SP(u32 id,u8 cmd);
void TC(u32 id);
void UM(u32 id,u8 cmd,u32 status);
void ST(u32 id);
void VX(u8 id);
void ifElmoTimeout(u8 id);	
#endif



