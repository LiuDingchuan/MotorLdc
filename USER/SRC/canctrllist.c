#include "canctrllist.h"

Can_ProcedureTypeDef CAN_Procedure;

//��ʼ��CAN���Ŀ��Ʊ�
void InitCANControlList(MesgControlGrpTypeDef* CAN_MesgSendList,u8 CAN_x)
{
	u8 CAN_NodeNumber=0;
	if(CAN_x==CAN_1)
	{
		flag.Can1CtrlList_Enable=1;
		CAN_NodeNumber=CAN1_NodeNumber;
	}
	else 
	{
		flag.Can2CtrlList_Enable=1;
		CAN_NodeNumber=CAN2_NodeNumber;
	}
	for(int i=0;i<CAN_NodeNumber;i++)
	{
		CAN_MesgSendList[i].SendSem=0;
		CAN_MesgSendList[i].TimeOut=0;
		CAN_MesgSendList[i].SendNumber = 0;
		CAN_MesgSendList[i].ReceiveNumber = 0;
		CAN_MesgSendList[i].SendQueue.Rear = 0;
		CAN_MesgSendList[i].SendQueue.Front = 0;
		CAN_MesgSendList[i].QueueFullTimeOut = 0;
		CAN_MesgSendList[i].SendQueue.Can_DataSend[0].InConGrpFlag=0;
	}
}

//�����ѷ��ͱ���״̬
void TraversalControlList(MesgControlGrpTypeDef *Can_MesgSendList,Can_QueueTypeDef *can_queue,u8 CAN_x)
{
	u8 CAN_NodeNumber=0;
	if(CAN_1==CAN_x) 
		CAN_NodeNumber=CAN1_NodeNumber;
	else 
		CAN_NodeNumber=CAN2_NodeNumber;
//�������Ŀ��Ʊ�
	for(int i=0;i<CAN_NodeNumber;i++)
	{
		if(Can_MesgSendList[i].SendSem>=1)
		{
			if(Can_MesgSendList[i].TimeOut++>=30)
			{
				if(QueueFull(can_queue))
				{
					Can_MesgSendList[i].QueueFullTimeOut++;
					return;
				}
				else if(QueueEmpty(&Can_MesgSendList[i].SendQueue))
				{
					Can_MesgSendList[i].QueueFullTimeOut=0;
					return;
				}
				else 
				{
					Can_MesgSendList[i].SendSem-=1;
					Can_MesgSendList[i].TimeOut=0;
					Can_MesgSendList[i].QueueFullTimeOut=0;
					can_queue->Can_DataSend[can_queue->Rear].ID=Can_MesgSendList[i].SendQueue.Can_DataSend[Can_MesgSendList[i].SendQueue.Front].ID;
					can_queue->Can_DataSend[can_queue->Rear].DLC=Can_MesgSendList[i].SendQueue.Can_DataSend[Can_MesgSendList[i].SendQueue.Front].DLC;
					can_queue->Can_DataSend[can_queue->Rear].InConGrpFlag=Can_MesgSendList[i].SendQueue.Can_DataSend[Can_MesgSendList[i].SendQueue.Front].InConGrpFlag;
					memcpy(can_queue->Can_DataSend[can_queue->Rear].Data,Can_MesgSendList[i].SendQueue.Can_DataSend[Can_MesgSendList[i].SendQueue.Front].Data,
						sizeof(u8)*can_queue->Can_DataSend[can_queue->Rear].DLC);
				}
				can_queue->Rear=(can_queue->Rear+1)%CAN_QUEUESIZE;
				Can_MesgSendList[i].SendQueue.Front=(Can_MesgSendList[i].SendQueue.Front+1)%CAN_QUEUESIZE;
			}
			if(Can_MesgSendList[i].QueueFullTimeOut>10||(Can_MesgSendList[i].SendNumber-Can_MesgSendList[i].ReceiveNumber)>50)
			{
				Can_MesgSendList[i].SendSem=-10000;
				if(CAN_1==CAN_x)
				{
					switch(i)
					{
						
					}				
				}
				else
				{
					switch(i)
					{
//						case 0:insertError(Error.head,SYSTEMERROR|(1<<4)|(2<<8));
//							break;
//						case 1:insertError(Error.head,SYSTEMERROR|(1<<4)|(2<<8));
//							break;//��ʱ��֪��ΪʲôҪ��ôд����˵������д�ģ������������û�ù�
					}
				}
			}
		}
			
	}
}


//����Ҫ���ӵı��ķ����Ӧ���ӵĿ��ƿ���
void Can_MesgCtrlList(MesgControlGrpTypeDef *CAN_MesgSendList,Can_QueueTypeDef *can_queue,u8 CAN_x)
{
	u8 CAN_NodeNumber=0;
	u16 ControlListID=0xFF;
	
	if(can_queue->Can_DataSend[can_queue->Front].InConGrpFlag==0)
		return;//��ӱ�־λΪ0ʱ��ֱ���˳�
	if(CAN_x==CAN_1)
	{
		CAN_NodeNumber=CAN1_NodeNumber;
	}
	else
	{
		CAN_NodeNumber=CAN2_NodeNumber;
		ControlListID=can_queue->Can_DataSend[can_queue->Front].ID-0x301;//���Ŀ��ƿ�ӳ��(���������ضԽӲ��ֵĴ��룬��ʱ��Ҫ������ط��ı��Ľ�����Ӧ����)
	}
	if(ControlListID<CAN_NodeNumber)//��ʱ����ĳ����ֻ���ڴ˴����� &&ControlListID != X
	{
		if(QueueFull(&CAN_MesgSendList[ControlListID].SendQueue))
		{
			if(CAN_x==CAN_1)
				flag.Can1SendqueueFULL++;
			else 
				flag.Can2SendqueueFULL++;
		}
		else
		{
			CAN_MesgSendList[ControlListID].SendSem++;
			CAN_MesgSendList[ControlListID].SendNumber++;
			CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].ID=can_queue->Can_DataSend[can_queue->Front].ID;
			CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].InConGrpFlag = can_queue->Can_DataSend[can_queue->Front].InConGrpFlag;
			memcpy(CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].Data,can_queue->Can_DataSend[can_queue->Front].Data,
				sizeof(u8)*CAN_MesgSendList[ControlListID].SendQueue.Can_DataSend[CAN_MesgSendList[ControlListID].SendQueue.Rear].DLC);
			CAN_MesgSendList[ControlListID].SendQueue.Rear=(CAN_MesgSendList[ControlListID].SendQueue.Rear+1)%CAN_QUEUESIZE;
		}
	}
	
}



























