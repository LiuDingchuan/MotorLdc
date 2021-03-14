#ifndef __VESC_H
#define __VESC_H

#include "stdint.h"
#include "param.h"
#include "queue.h"
#include "canctrllist.h"
#include "buffer.h"
#include "motor.h"
#include "filter.h"

/****VESC�ڲ�****/
typedef struct {
	u8 POLE_PAIRS; //���������
	float MAX_CURRENT;//���������������������
} VESCParam;

/****VESC���****/
typedef struct {
	vs16 speed;				//�ٶ�
	vs32 position;
	volatile float current; //����
	volatile float angle;			//λ��
  	float duty;	//ռ�ձ�
	vu16 angleNow;
	vu16 anglePrv;
} VESCValue;

/****VESC״̬****/
typedef struct {
	bool stuck;	  //�����ת
	bool timeout; //can���ķ�����ʱ
	bool arrived; //�ж��Ƿ񵽴�λ��
	bool isSetZero;//���õ�ǰλ��Ϊ���
} VESCStatus;

/****VESC���Ʊ���****/
typedef struct {
	float breakCurrent; //ɲ������
	bool timeoutMotion;//�Ƿ���г�ʱ���
	bool isPosLimitOn;//�Ƿ����λ�����ƣ�һЩ�����Ͽ�����Ҫ��
	s32 maxPosition;//λ�����Ƶ����λ��
	bool isPosSPLimitOn;//�Ƿ����λ�û��ٶ�����
	s32 posSPlimit;//λ�û��ٶȵ����ֵ
} VESCLimit;

/****һЩ��־λ����****/
typedef struct {
	u8 timeoutCnt;	  //��ʱ�ü���ֵ��[����ڳ�ʱ״̬timeout]
	u32 lastRxTim;	  //�ϴν��յ����ĵ�ϵͳʱ�䣬[����ڳ�ʱ״̬timeout]
	u16 timeoutTicks; //�жϳ�ʱ��ϵͳ��������[����ڳ�ʱ״̬timeout]
	vs16 distance;//���νǶȵĲ�ֵ
	bool firstPos;//�Ƿ��ǵ�һ���ϵ�Ƕ�
	float lockAngle;
	s32 lockPosition;
	vs32 difPosition;
} VESCArgum;

/****VESC����ṹ��****/
typedef struct {
	bool enable;			 //���ʹ��
	bool begin;				 //�������
	bool Encoder_DIR_inverted;//��ת������ת
	u8 mode;				 //���ģʽ
	VESCValue valueSet, valueReal; //����趨ֵ��ʵ��ֵ
	PID_setTypeDef PIDx,PIDs;//PID����
	VESCStatus status;		 //���״̬
	VESCArgum argum;		 //��ֵ����
	VESCLimit limit;		 //������Ʊ���
	VESCParam instrinsic;	 //����ڲ�
} VESC_Motor;

/****VESC��can��������ö����****/
typedef enum {
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5
} CAN_PACKET_ID_Enum;

extern VESC_Motor VESCmotor[5];

void VESCInit(void);
void VESC_caculate(VESC_Motor* motor);
void VESC_position_mode_rpm(u8 id);
void VESC_position_mode_pos(u8 id);
void VESC_Set_Duty_Cycle(u8 controller_ID, float duty_cycle, u8 InConGrpFlag);
void VESC_Set_Speed(u8 controller_ID, s32 speed, u8 InConGrpFlag);
void VESC_Set_Current(u8 controller_ID, float current, u8 InConGrpFlag);
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 InConGrpFlag);
void VESC_Set_Handbrake_Current(u8 controller_ID, float handbrake_current, u8 InConGrpFlag);
void VESC_Set_Position(u8 controller_ID,float angle, u8 InConGrpFlag);
void ifVESCTimeout(u16 id);

#endif

