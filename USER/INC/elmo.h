#ifndef _ELMO_H
#define _ELMO_H

#include "param.h"
#include "stdarg.h"
#include "queue.h"

#define GetData	1
#define	SetData 2

/****ELMO CAN报文ID****/
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

/****ELMO内参****/
typedef struct{
    u16 PULSE;//编码器线数
    float  RATIO;//减速比
}ELMOParam;

/****ELMO外参****/
typedef struct{
	vs16 angle;//轴前角度
	vs16 current;//电流
	vs16 speed;//速度(rpm,后面计算时转换为JV值与SP值)
	vs32 pulse;
}ELMOValue;

/****电机状态****/
typedef struct{
	bool arrived;//位置模式下到达指定模式
	bool zero;//寻零完成
	bool stuck;//电机堵转
	bool timeout;//can报文反馈超时
}ELMOStatus;

/****ELMO限制保护结构体****/
typedef struct{
    bool isPosLimitON;//是否对最大位置进行限制，（轴前角度）
    float maxAngle;//最大旋转角度
    bool isPosSPLimitOn;//是否对位置模式进行限速
    u16 posSPlimit;//位置模式下的速度限制
	  bool timeoutMotion;//是否开启超时检测
    bool isRealseWhenStuck;//是否堵转时释放电机
    u16 zeroSP;//寻零模式下的速度
    u16 zeroCurrent;//寻零模式下的最大电流
}ELMOLimit;

/****不需要关心的一些参数****/
typedef struct{
    u32 maxPulse;//最大旋转脉冲，（相关于最大旋转角度）
    u16 zeroCnt;//寻零用计数值，（相关于寻零状态zero）
    vu16 stuckCnt;//堵转用计数值，（相关于堵转状态stuck）
    u16 timeoutCnt;//超时用计数值（相关于超时状态timeout）
    u32 lastRxTim;//上次接收到报文的系统时间（相关于超时状态timeout）
    u16 timeoutTicks;//判断超时用系统节拍数，（相关于超时状态timeout）
}ELMOArgum;

/****ELMO总结构体****/
typedef struct{
  u8 mode;//电机模式
  bool enable;//电机使能
  ELMOValue valueSet,valueReal;//外参设定值，实际值
  ELMOStatus status;//电机状态
  ELMOLimit limit;//限制保护
  ELMOArgum argum;//计算所用间值
  ELMOParam intrinsic;//电机内参
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



