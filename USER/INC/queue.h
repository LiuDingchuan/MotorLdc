#ifndef _QUEUE_H
#define _QUEUE_H

#include "param.h"
#include "string.h"
#include "stm32f4xx_can.h"
#include "VESC.h"

#define CAN_1 (u8)(0x01<<0)
#define CAN_2 (u8)(0x01<<1)

/*���Ŀ��Ʊ�*/
#define CAN_QUEUESIZE 30  //�ѷ��͵ı��Ķ��г���
#define CAN_HAVESENDQUEUESIZE 5 //�ѷ��ͱ��Ķ��г���
#define CAN1_NodeNumber 6u //��Ҫ����8�����ϱ�־λֻ��8λ��
#define CAN2_NodeNumber 6u //��Ҫ����8�����ϱ�־λֻ��8λ��
#define Rear1 ((Can1_Sendqueue.Rear+1)%CAN_QUEUESIZE)
#define Rear2 ((Can2_Sendqueue.Rear+1)%CAN_QUEUESIZE)
#define VESCRear ((VESC_Sendqueue.Rear+1)%CAN_QUEUESIZE)

//CAN���ķ��ͽṹ��
typedef struct
{
	u32 ID;
	u8 DLC;
	u8 Data[8];
	u8 InConGrpFlag;
}Can_SendtructTypeDef;

//CAN���Ķ���
typedef struct
{
	u16 Front,Rear;
	Can_SendtructTypeDef Can_DataSend[CAN_QUEUESIZE];
}Can_QueueTypeDef;

//���Ŀ��ƿ�
typedef struct
{
	s32 SendNumber;
	s32 ReceiveNumber;
	u32 QueueFullTimeOut;
	u8 TimeOut;
	s32 SendSem;
	Can_QueueTypeDef SendQueue;
}MesgControlGrpTypeDef;

//CAN���̺�
typedef struct
{
	volatile u8 order_valid;//��ǰ��Ч�������̺�
	volatile u8 order_now;//��ǰ�������̺�
	volatile u8 status;//��ǰ����״̬
}Can_ProcedureTypeDef;

extern Can_ProcedureTypeDef 	Can_Procedure;
extern Can_SendtructTypeDef 	Can_Sendtruct;
extern Can_QueueTypeDef 			Can1_Sendqueue,Can2_Sendqueue,VESC_Sendqueue;
extern MesgControlGrpTypeDef 	Can1_MesgSendList[CAN1_NodeNumber],Can2_MesgSendList[CAN2_NodeNumber];

void Can_SendqueueInit(void);
bool QueueEmpty(Can_QueueTypeDef *can_queue);
bool QueueFull(Can_QueueTypeDef *can_queue);
void Can_DeQueue(CAN_TypeDef *CANx,Can_QueueTypeDef *can_queue);
void InitCANControlList(MesgControlGrpTypeDef* CAN_MesgSendList,u8 CAN_x);
void TraversalControlList(MesgControlGrpTypeDef *Can_MesgSendList,Can_QueueTypeDef *can_queue,u8 CAN_x);
void Can_MesgCtrlList(MesgControlGrpTypeDef *CAN_MesgSendList,Can_QueueTypeDef *can_queue,u8 CAN_x);


#endif







