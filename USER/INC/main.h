#ifndef _MAIN_H
#define _MAIN_H

#include "led.h"
#include "elmo.h"
#include "beep.h"
#include "param.h"
#include "led8.h"
#include "usart1.h"
#include "usart2.h"
#include "tim2.h"
#include "tim3.h"
#include "can1.h"
#include "visual_scope.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "VisualScope.h"

extern bool motorSwitchOn;
/**UCOSII任务设置**/
//START任务
#define START_TASK_PRIO          5            //设置任务优先级
#define START_TASK_STK_SIZE      256          //设置堆栈大小
__align(8) OS_STK START_TASK_STK[START_TASK_STK_SIZE];
static void Task_Start(void *pdata);

//ZERO任务（上电时位置清零）
#define ZERO_TASK_PRIO 6 //设置任务优先级
#define ZERO_STK_SIZE 64 //设置堆栈大小
__align(8) OS_STK  ZERO_TASK_STK[ZERO_STK_SIZE]; //任务堆栈
static void Task_ZERO(void *pdata); //任务函数

//LCD任务
#define LCD_TASK_PRIO 10               //设置任务优先级
#define LCD_STK_SIZE 512               //设置堆栈大小
__align(8) OS_STK LCD_TASK_STK[LCD_STK_SIZE];
static void Task_LCD(void *pdata);

//ELMO任务
#define ELMO_TASK_PRIO 11 //设置任务优先级
#define ELMO_STK_SIZE 512 //设置堆栈大小
__align(8) OS_STK  ELMO_TASK_STK[ELMO_STK_SIZE]; //任务堆栈
static void Task_ELMO(void *pdata); //任务函数

//Task_PICKUPARROW
#define TAKEARROW_TASK_PRIO 14
#define TAKEARROW_TASK_STK_SIZE 128
__align(8) OS_STK TAKEARROW_TASK_STK[TAKEARROW_TASK_STK_SIZE];
static void Task_TAKEARROW(void *pdata);

//Task_PICKUPARROW
#define PICKUPARROW_TASK_PRIO 15
#define PICKUPARROW_STK_SIZE 128
__align(8) OS_STK PICKUPARROW_TASK_STK[PICKUPARROW_STK_SIZE];
static void Task_PICKUPARROW(void *pdata);

//Task_SHOOTING
#define SHOOTING_TASK_PRIO 16
#define SHOOTING_STK_SIZE 128
__align(8) OS_STK SHOOTING_TASK_STK[SHOOTING_STK_SIZE];
static void Task_SHOOTING(void *pdata);

//Task_ERROR
#define ERROR_TASK_PRIO 17
#define ERROR_STK_SIZE 128
__align(8) OS_STK ERROR_TASK_STK[ERROR_STK_SIZE];
static void Task_ERROR(void *pdata);

//Task_VisualScope
#define VISUALSCOPE_TASK_PRIO 19 //设置任务优先级
#define VISUALSCOPE_STK_SIZE 128 //设置堆栈大小
__align(8) OS_STK  VISUALSCOPE_TASK_STK[VISUALSCOPE_STK_SIZE]; //任务堆栈
static void Task_VISUALSCOPE(void *pdata); //任务函数

//VESC任务
#define VESC_TASK_PRIO 20 //设置任务优先级
#define VESC_STK_SIZE 512 //设置堆栈大小
__align(8) OS_STK  VESC_TASK_STK[VESC_STK_SIZE]; //任务堆栈
static void Task_VESC(void *pdata); //任务函数

#define BEEP_TASK_PRIO 21	//设置任务优先级
#define BEEP_STK_SIZE 64 //设置堆栈大小
__align(8) OS_STK  BEEP_TASK_STK[BEEP_STK_SIZE]; //任务堆栈
static void Task_BEEP(void *pdata); //任务函数

OS_EVENT *RUN;

#endif
