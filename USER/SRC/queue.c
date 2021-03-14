#include "queue.h"

/***相关结构体的定义*/
Can_ProcedureTypeDef Can_Procedure;
Can_SendtructTypeDef Can_Sendtruct;
MesgControlGrpTypeDef Can1_MesgSendList[CAN1_NodeNumber],Can2_MesgSendList[CAN2_NodeNumber];
Can_QueueTypeDef Can1_Sendqueue,Can2_Sendqueue,VESC_Sendqueue;

//基本数据
u8 CanSendCount;
int Can1_Tx_NoMailBox;
int Can2_Tx_NoMailBox;

//判断队列是否为空
bool QueueEmpty(Can_QueueTypeDef *can_queue)
{
	return (can_queue->Front==can_queue->Rear);
}

//判断队列是否已满
bool QueueFull(Can_QueueTypeDef *can_queue)
{
	return(((can_queue->Rear+1)%CAN_QUEUESIZE)==can_queue->Front);
}

//CAN队列初始化
void Can_SendqueueInit(void)
{
	Can1_Sendqueue.Front=Can1_Sendqueue.Rear=0;
	Can2_Sendqueue.Front=Can2_Sendqueue.Rear=0;
}

//CAN出队函数
void Can_DeQueue(CAN_TypeDef *CANx,Can_QueueTypeDef *can_queue)
{
	CanTxMsg TxMessage;
	CanSendCount=0;
	if(QueueEmpty(can_queue))
		flag.CanSendqueueEMPTY=1;
	else
	{
		if(can_queue->Can_DataSend[can_queue->Front].ID<0x400)
		{
			TxMessage.IDE=CAN_ID_STD;
			TxMessage.StdId=can_queue->Can_DataSend[can_queue->Front].ID;
		}
		else
		{
			if((can_queue->Can_DataSend[can_queue->Front].ID&0xf0000000)!=0)//VESC报文的标志
			{
				can_queue->Can_DataSend[can_queue->Front].ID=can_queue->Can_DataSend[can_queue->Front].ID & 0x0fffffff;			
			}	
			TxMessage.IDE=CAN_ID_EXT;
			TxMessage.ExtId=can_queue->Can_DataSend[can_queue->Front].ID;
		}
		if(CANx==CAN1)
			Can_MesgCtrlList(Can1_MesgSendList,&Can1_Sendqueue,CAN_1);
		else
			Can_MesgCtrlList(Can2_MesgSendList,&Can2_Sendqueue,CAN_2);
		TxMessage.DLC=can_queue->Can_DataSend[can_queue->Front].DLC;
		TxMessage.RTR=CAN_RTR_DATA;
		memcpy(TxMessage.Data,(can_queue->Can_DataSend[can_queue->Front].Data),TxMessage.DLC*sizeof(uint8_t));
		can_queue->Front=(can_queue->Front+1)%CAN_QUEUESIZE;
		CAN_Transmit(CANx,&TxMessage);

		//目前根据实验得出的结论,这一段是导致2006丢位置的主要原因（以及电调过多之后电机疯抖）
		//保护措施
		// while((CAN_Transmit(CANx,&TxMessage))==CAN_TxStatus_NoMailBox)
		// {
		// 	CanSendCount++;
		// 	if(CanSendCount>10)
		// 	{
		// 		if(CANx==CAN1)	Can1_Tx_NoMailBox++;
		// 		else			Can2_Tx_NoMailBox++;
		// 		break;
		// 	}
		// }
		// if(Can1_Tx_NoMailBox>=5)
		// {
		// 	Can1_Tx_NoMailBox=0;
		// }
		// if(Can2_Tx_NoMailBox>=5)
		// {
		// 	Can2_Tx_NoMailBox=0;
		// }
	}
}
	
















