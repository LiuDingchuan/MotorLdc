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
	bool isPosLimitON;//是否对最大位置进行限制(轴前角度)
	float maxAngle;//最大旋转角度
	bool isPosSPLimitOn;//是否对位置模式进行限速
	u16 posSPlimit;//位置模式下的速度限制
	bool stuckmotion;//是否开启堵转检测
	bool timeoutMotion;//是否开启超时检测
	bool isRealseWhenStuck;//是否堵转时释放电机
	s16 zeroSP;//寻零模式下的速度
	u16 zeroCurrent;//寻零模式下的最大电流
}MotorLimit;

//电机内参
typedef struct{
	u16 PULSE;//编码器线数
	float RATIO;//减速比
	u16 CURRENT_LIMIT;//最大输出电流限制
	float GearRatio;//齿轮减速比
}MotorParam;

//电机外参
typedef struct{
	vs16 current;//电流
	vs16 speed;//速度(rpm)
	vs16 temperature;//温度
	vs32 pulse;//累计脉冲
	vs16 pulseRead;//反馈脉冲
	float angle;//轴前角度
}MotorValue;

//电机状态
typedef  struct {
	bool arrived;//位置模式下到达指定位置
	bool zero;//寻零完成
	bool stuck;//电机堵转
	bool timeout;//can报文反馈超时
	bool overspeed;//超速
	bool isSetZero;//是否设置当前位置为零点
	bool clearFlag;//判断是否为第一次上电，清除误差
}MotorStatus;

//不需要关心的一些参数
typedef struct{
    u32 maxPulse;//最大旋转脉冲(相关于最大旋转角度)
    vs32 lockPulse;//锁位置用当前位置记录(相关于begin)
    s16 distance;//当前反馈脉冲与前次反馈脉冲差值(相关于累计脉冲pulse)!!!!!这个参数挺重要的
    s32 difPulseSet;//当前累计脉冲与设定脉冲差值!!!这个也挺重要的（都是在debug模式下可以查阅的参数）
    u16 zeroCnt;//寻零用计数值，(相关于寻零模式zero)
    u8 stuckCnt;//堵转用计数值
    u8  timeoutCnt;//超时用计数值
    u32 lastRxTim;//上次接受到报文的系统时间，(相关于超时状态timeout)
    u16 timeoutTicks;//超时判断所用的系统节拍数,[相关于超时状态timeout]
		u16 stuckTicks;//堵转判断所用的系统节拍数
}MotorArgum;

//DJ电机总结构体
typedef struct{
	bool enable;//电机使能
	bool begin;//电机启动
	u8 mode;//电机模式
	MotorValue valueSet,valueReal,valuePrv;//外参设定值，实际值，前次值
	MotorStatus status;//电机状态
	PID_setTypeDef PIDx,PIDs;//pid参数
	MotorLimit limit;//限制保护
	MotorArgum argum;//计算所用间值
  MotorParam intrinsic;//电机内参
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



