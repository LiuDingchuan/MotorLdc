#ifndef _MATRIXKEY_H
#define _MATRIXKEY_H

#include "led8.h"
#include "delay.h"
#include "sys.h"
#include "elmo.h"

//列线
#define C1_PIN		GPIO_Pin_6
#define C1_GPIO_PORT	GPIOC
#define	C1_GPIO_CLK		RCC_AHB1Periph_GPIOC

#define C2_PIN		GPIO_Pin_7
#define C2_GPIO_PORT	GPIOC
#define	C2_GPIO_CLK		RCC_AHB1Periph_GPIOC

#define C3_PIN		GPIO_Pin_8
#define C3_GPIO_PORT	GPIOC
#define	C3_GPIO_CLK		RCC_AHB1Periph_GPIOC

#define C4_PIN		GPIO_Pin_9
#define C4_GPIO_PORT	GPIOC
#define	C4_GPIO_CLK		RCC_AHB1Periph_GPIOC

//行数
#define R1_PIN		GPIO_Pin_12
#define	R1_GPIO_PORT	GPIOB
#define R1_GPIO_CLK		RCC_AHB1Periph_GPIOB

#define R2_PIN		GPIO_Pin_13
#define	R2_GPIO_PORT	GPIOB
#define R2_GPIO_CLK		RCC_AHB1Periph_GPIOB

#define R3_PIN		GPIO_Pin_14
#define	R3_GPIO_PORT	GPIOB
#define R3_GPIO_CLK		RCC_AHB1Periph_GPIOB

#define R4_PIN		GPIO_Pin_15
#define	R4_GPIO_PORT	GPIOB
#define R4_GPIO_CLK		RCC_AHB1Periph_GPIOB

//检测	uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
#define DETECT_C1 	GPIO_ReadInputDataBit(C1_GPIO_PORT,C1_PIN)
#define DETECT_C2 	GPIO_ReadInputDataBit(C2_GPIO_PORT,C2_PIN)
#define DETECT_C3 	GPIO_ReadInputDataBit(C3_GPIO_PORT,C3_PIN)
#define DETECT_C4 	GPIO_ReadInputDataBit(C4_GPIO_PORT,C4_PIN)

#define DETECT_R1 	GPIO_ReadInputDataBit(R1_GPIO_PORT,R1_PIN)
#define DETECT_R2 	GPIO_ReadInputDataBit(R2_GPIO_PORT,R2_PIN)
#define DETECT_R3 	GPIO_ReadInputDataBit(R3_GPIO_PORT,R3_PIN)
#define DETECT_R4 	GPIO_ReadInputDataBit(R4_GPIO_PORT,R4_PIN)
/********* Matrixkey **********/
#define S1    0x11
#define S2    0x12
#define S3    0x14
#define S4    0x18
#define S5    0x21
#define S6    0x22
#define S7    0x24
#define S8    0x28
#define S9    0x41
#define S10   0x42
#define S11   0x44
#define S12   0x48
#define S13   0x81
#define S14   0x82
#define S15   0x84
#define S16   0x88
/*********************************/

void Key_Configuration(void);
u8 Check_Matrixkey(void);
void Key_Ctrl(void);

#endif



