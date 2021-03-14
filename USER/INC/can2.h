#ifndef __CAN2_H
#define __CAN2_H

#include "param.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "motor.h"
#include "elmo.h"
#include "queue.h"

void CAN2_Configuration(void);
void currentInput(u8 id);
void valveCtrl(u8 ID,bool status);
void CAN2_Valve(u8 id,u8 Valve_Flag);

#endif
