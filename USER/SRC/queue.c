#include "queue.h"

/***��ؽṹ��Ķ���*/
Can_ProcedureTypeDef Can_Procedure;
Can_SendtructTypeDef Can_Sendtruct;
MesgControlGrpTypeDef Can1_MesgSendList[CAN1_NodeNumber],Can2_MesgSendList[CAN2_NodeNumber];
Can_QueueTypeDef Can1_Sendqueue,Can2_Sendqueue,VESC_Sendqueue;

//��������
u8 CanSendCount;
int Can1_Tx_NoMailBox;
int Can2_Tx_NoMailBox;

//�ж϶����Ƿ�Ϊ��
bool QueueEmpty(Can_QueueTypeDef *can_queue)
{
	return (can_queue->Front==can_queue->Rear);
}

//�ж϶����Ƿ�����
bool QueueFull(Can_QueueTypeDef *can_queue)
{
	return(((can_queue->Rear+1)%CAN_QUEUESIZE)==can_queue->Front);
}

//CAN���г�ʼ��
void Can_SendqueueInit(void)
{
	Can1_Sendqueue.Front=Can1_Sendqueue.Rear=0;
	Can2_Sendqueue.Front=Can2_Sendqueue.Rear=0;
}

//CAN���Ӻ���
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
			if((can_queue->Can_DataSend[can_queue->Front].ID&0xf0000000)!=0)//VESC���ĵı�־
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

		//Ŀǰ����ʵ��ó��Ľ���,��һ���ǵ���2006��λ�õ���Ҫԭ���Լ��������֮�����趶��
		//������ʩ
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
	
















