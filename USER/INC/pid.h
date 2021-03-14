#ifndef _PID_H
#define _PID_H

#include "stm32f4xx.h"

typedef struct
{
	float KP;//比例系数
	float KI;//积分系数
	float KD;//微分常数
	float KS;//速度降系
  	volatile s32 SetValue;//设定值
	volatile s32 Udlt;
	volatile s32 CurValue;
	volatile s32 liKkValue[3];//差值保存，给定和反馈的差值
}PID_setTypeDef;

void PID_Init(PID_setTypeDef *PID,float KP,float KI,float KD,float KS,s32 Set);
void PID_Operation(PID_setTypeDef *PID);

extern PID_setTypeDef SunnySkyPID, U10PID, DJ3508PID, DJ2006PID;

#endif
