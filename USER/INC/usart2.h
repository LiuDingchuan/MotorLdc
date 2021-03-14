#ifndef __USART2_H
#define __USART2_H

#include "dma.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "param.h"
#include "stm32f4xx_usart.h"
#include "beep.h"
#include "can1.h"
#include "stdbool.h"
#include "macro.h"

void USART2_Configuration(void);
void UsartLCDshow(void);

extern float DJ2006angle_gap;
extern float TakeArrowPosition;
#endif
