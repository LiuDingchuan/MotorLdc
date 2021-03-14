#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "param.h"
#include "pid.h"
#include "can2.h"
#include "macro.h"
#include "elmo.h"
#include "delay.h"
#include "matrixkey.h"
#include "queue.h"

void TIM3_Init(void);
extern s32 VESC_Speed_Now[5];


#endif
