#ifndef _PARAM_H_
#define _PARAM_H_

#include "motor.h"
#include "delay.h"
#include "valve.h"
#include "elmo.h"
#include "vesc.h"


#define RESET_PRO {__set_FAULTMASK(1);NVIC_SystemReset();}

#define Valve_On 1
#define Valve_Off 0
#define Valve_NOW 2

#define F4valveON GPIO_ResetBits(GPIOC, GPIO_Pin_8);//设置低电平是通电的状态
#define F4valveOFF GPIO_SetBits(GPIOC, GPIO_Pin_8);//设置高电平是不通电的状态

#define B1 1
#define B0 2
#define A3 3
#define A2 4
/**5.6.7输入的是8V电压，电磁阀只能通过5V电压控制，舵机5V/8V都可以**/
//#define A1 5
//#define A6 6
//#define A7 7 
//新舵机板新添IO口如下：
#define B4 5
#define B5 6
#define B6 7 
#define B7 8

/**
 * @description: serial communication buffer size
 */



//第一位
#define SYSTEMERROR 0x0000
#define DJERROR 	0x0001
#define ELMOERROR   0x0002
#define VESCERROR   0x0003
#define EPOSERROR   0x0004
//第三位
#define TIMEOUT     0x0100
#define STUCK       0x0200
#define EMERGENCY  	0x0300

//电机模式
enum MOTOR_MODE { duty=0, current, RPM, brake, zero, position, handbrake, vesc_position_rpm};

/*错误显示链表*/
typedef struct _ErrorList{
	u16 code;
	struct _ErrorList* next;
}ErrorList;

typedef struct{
	u8 size;
	ErrorList* head;
}ErrorTypeDef;

typedef enum
{
  USART1_Tx_BufferSize = 256, //串口一发送数据长度
  USART2_Tx_BufferSize = 256, //串口二发送数据长度
  USART3_Tx_BufferSize = 32,  //串口三发送数据长度
  USART1_Rx_BufferSize = 32, //串口一接收数据长度(接收中断内使用)
  USART2_Rx_BufferSize = 32, //串口二接收数据长度(接收中断内使用)
  USART3_Rx_BufferSize = 32, //串口三接收数据长度(接收中断内使用)
} Usart_Enum;

/**
 * @description: serial communication data buffer struct
 */
typedef struct{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //串口一发送数据
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //串口一接收数据

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //串口二发送数据
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //串口二接收数据

  u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //串口三发送数据
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //串口三接收数据
} Usart_Struct;

/****标志位结构体****/
typedef struct{
	volatile bool Can1CtrlList_Enable; // CAN1报文控制表启动标志位
	volatile bool Can2CtrlList_Enable; // CAN2报文控制表启动标志位
	volatile bool Can1SendqueueFULL;   // CAN1发送队列满
	volatile bool Can2SendqueueFULL;   // CAN2发送队列满
	volatile bool VESCSendqueueFULL;   // CAN2发送队列满
	volatile bool CanSendqueueEMPTY;   // CAN队列为空
	volatile bool has_timeout[8];      // CAN报文接收超时
	volatile bool Usart1DMASendFinish; // Usart1_DMA传输
	volatile bool Usart2DMASendFinish; // Usart2_DMA传输
	volatile bool FlagMusic;   //唱歌标志位
	bool FlagShooting;         //发射标志位
	u8 FlagTakeArrow;  	   //取箭标志位
	bool valve[4];                     //当前电磁阀状态
} FlagTypeDef;

extern Usart_Struct usart;
extern FlagTypeDef flag;
extern ErrorTypeDef Error;

void param_Init(void);
void ChangeData(u8* buff2,u8* buff1);
ErrorList* creatError(void);
void deleteError(ErrorList* list, ErrorList* listdel);
void insertError(ErrorList* list, uc16 errorcode);
ErrorList* ErrorFind(ErrorList* list, uc16 errorcode);
void utils_norm_angle(float *angle);

#endif

