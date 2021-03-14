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

#define F4valveON GPIO_ResetBits(GPIOC, GPIO_Pin_8);//���õ͵�ƽ��ͨ���״̬
#define F4valveOFF GPIO_SetBits(GPIOC, GPIO_Pin_8);//���øߵ�ƽ�ǲ�ͨ���״̬

#define B1 1
#define B0 2
#define A3 3
#define A2 4
/**5.6.7�������8V��ѹ����ŷ�ֻ��ͨ��5V��ѹ���ƣ����5V/8V������**/
//#define A1 5
//#define A6 6
//#define A7 7 
//�¶��������IO�����£�
#define B4 5
#define B5 6
#define B6 7 
#define B7 8

/**
 * @description: serial communication buffer size
 */



//��һλ
#define SYSTEMERROR 0x0000
#define DJERROR 	0x0001
#define ELMOERROR   0x0002
#define VESCERROR   0x0003
#define EPOSERROR   0x0004
//����λ
#define TIMEOUT     0x0100
#define STUCK       0x0200
#define EMERGENCY  	0x0300

//���ģʽ
enum MOTOR_MODE { duty=0, current, RPM, brake, zero, position, handbrake, vesc_position_rpm};

/*������ʾ����*/
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
  USART1_Tx_BufferSize = 256, //����һ�������ݳ���
  USART2_Tx_BufferSize = 256, //���ڶ��������ݳ���
  USART3_Tx_BufferSize = 32,  //�������������ݳ���
  USART1_Rx_BufferSize = 32, //����һ�������ݳ���(�����ж���ʹ��)
  USART2_Rx_BufferSize = 32, //���ڶ��������ݳ���(�����ж���ʹ��)
  USART3_Rx_BufferSize = 32, //�������������ݳ���(�����ж���ʹ��)
} Usart_Enum;

/**
 * @description: serial communication data buffer struct
 */
typedef struct{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //����һ��������
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //����һ��������

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //���ڶ���������
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //���ڶ���������

  u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //��������������
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //��������������
} Usart_Struct;

/****��־λ�ṹ��****/
typedef struct{
	volatile bool Can1CtrlList_Enable; // CAN1���Ŀ��Ʊ�������־λ
	volatile bool Can2CtrlList_Enable; // CAN2���Ŀ��Ʊ�������־λ
	volatile bool Can1SendqueueFULL;   // CAN1���Ͷ�����
	volatile bool Can2SendqueueFULL;   // CAN2���Ͷ�����
	volatile bool VESCSendqueueFULL;   // CAN2���Ͷ�����
	volatile bool CanSendqueueEMPTY;   // CAN����Ϊ��
	volatile bool has_timeout[8];      // CAN���Ľ��ճ�ʱ
	volatile bool Usart1DMASendFinish; // Usart1_DMA����
	volatile bool Usart2DMASendFinish; // Usart2_DMA����
	volatile bool FlagMusic;   //�����־λ
	bool FlagShooting;         //�����־λ
	u8 FlagTakeArrow;  	   //ȡ����־λ
	bool valve[4];                     //��ǰ��ŷ�״̬
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

