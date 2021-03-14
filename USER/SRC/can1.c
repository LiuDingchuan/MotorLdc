#include "can1.h"

static s16 s16TempData[4]; u16 u16TempData[4];

void CAN1_Configuration(void) 
{
  CAN_InitTypeDef CAN_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通道模式
  CAN_InitStructure.CAN_ABOM = DISABLE; //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
  CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式由软件唤醒
  CAN_InitStructure.CAN_NART = DISABLE; //报文自动发送
  CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定，新的覆盖旧的
  CAN_InitStructure.CAN_TXFP = DISABLE; //发送FIFO的优先级由标识符决定
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; // CAN硬件工作在正常模式

  /* Seting BaudRate */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用8个时间单位
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用7个时间单位
  CAN_InitStructure.CAN_Prescaler = 3;     //分频系数（Fdiv）
  CAN_Init(CAN1, &CAN_InitStructure);      //初始化CAN1

//   CAN_FilterInitStructure.CAN_FilterNumber = 2; //屏蔽位模式
//   CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//   CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//   CAN_FilterInitStructure.CAN_FilterIdHigh = ((0x01314152 << 3) & 0xFFFF0000) >> 16;
//   CAN_FilterInitStructure.CAN_FilterIdLow = (0x01314152 << 3) & 0xFFFF;
//   CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xffff00 << 3) >> 16;
//   CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xfff0	<< 3) & 0xFFFF;
//   CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//   CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//   CAN_FilterInit(&CAN_FilterInitStructure);

  CAN_FilterInitStructure.CAN_FilterNumber = 2; //列表模式
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x100<<5;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x101<<5;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x532<<5;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x533<<5;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);


//  CAN_FilterInitStructure.CAN_FilterNumber = 3; //屏蔽位模式
//  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//  CAN_FilterInitStructure.CAN_FilterIdHigh =((BROADCAST_ID << 3) & 0xFFFF0000) >> 16;
//  CAN_FilterInitStructure.CAN_FilterIdLow = (BROADCAST_ID << 3) & 0xFFFF;
//  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xFFFF00 << 3) >> 16;
//  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xFFFF << 3) & 0xFFFF;
//  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//  CAN_FilterInit(&CAN_FilterInitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}
///*0号电机负责捡箭旋转，1号电机负责捡箭手爪旋转，2号负责摩擦轮餐盘，3号负责曲柄摇杆，4号负责摩擦轮抬升，5号负责转箭手柄的角度*/
void CAN1_RX0_IRQHandler()
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive (CAN1,CAN_FIFO0 ,&rx_message);
		CAN_ClearITPendingBit (CAN1,CAN_IT_FMP0);

		if(rx_message.IDE==CAN_Id_Extended&&rx_message.RTR==CAN_RTR_DATA)
		{
			//STOP ALL
			if(rx_message.ExtId==0x01314150)
				answer_master(&rx_message);
			#ifdef PICK_UP_ARROW
			//捡箭报文
				if(rx_message.ExtId==0x01314152)
				{
					//自检
					if(rx_message.Data[0]=='Z'&&rx_message.Data[1]=='J')
						answer_master(&rx_message);
					//手爪张开或关闭
					else if(rx_message.Data[0]=='J'&&rx_message.Data[1]=='Q')
					{
						CAN2_Valve(B1,rx_message.Data[2]);
						answer_master(&rx_message);
					}
					//电机寻零
					else if(rx_message.Data[0]=='F'&&rx_message.Data[1]=='Z')
					{
						motor[0].enable=ENABLE;
						motor[0].mode=zero;
						answer_master(&rx_message);
					}
					//手爪电机旋转至相应角度
					else if(rx_message.Data[0]=='T'&&rx_message.Data[1]=='U')
					{
						s16 Motor_angle;
						motor[0].enable=ENABLE;
						DecodeS16Data(&Motor_angle,&rx_message.Data[2]);
						motor[0].valueSet.angle=Motor_angle/50.f;
						answer_master(&rx_message);
					}
				}
			#endif

			#ifdef Shooting
			//摩擦轮发射报文
				else if(rx_message.ExtId==0x01314156)
				{
					//餐盘角度
					if(rx_message.Data[0]=='X'&&rx_message.Data[1]=='Z')
					{
						s16 TurnTable_angle;
						motor[2].enable=ENABLE;
						DecodeS16Data(&TurnTable_angle,&rx_message.Data[2]);
						motor[2].valueSet.angle=TurnTable_angle/10;
						answer_master(&rx_message);
					}
					//摩擦轮速度并发射
					else if(rx_message.Data[0]=='F'&&rx_message.Data[1]=='S')
					{
						u16 Shoot_speed;
						// MO(1,SetData,ENABLE);
						// MO(2,SetData,ENABLE);
						DecodeU16Data(&Shoot_speed,&rx_message.Data[2]);
						VESCmotor[1].valueSet.speed=Shoot_speed/10;
						// control_another(1, VESC);
						VESCmotor[2].valueSet.speed=-Shoot_speed/10;
						VESCmotor[1].enable=ENABLE;
						VESCmotor[2].enable=ENABLE;
						// control_another(2, VESC);
						// JV(1,SetData);
						// JV(2,SetData);
						flag.FlagShooting=1;
						answer_master(&rx_message);
					}
					//摩擦轮抬升角度
					else if(rx_message.Data[0]=='J'&&rx_message.Data[1]=='D')
					{
						s16 Pitch_angle;
						motor[4].enable=ENABLE;
						DecodeS16Data(&Pitch_angle,&rx_message.Data[2]);
						motor[4].valueSet.angle=Pitch_angle/30-30;
						answer_master(&rx_message);
					}
				}
			#endif

			#ifdef Defending
				转箭防守报文
				else if(rx_message.ExtId==0x01314157)
				{
					//转箭速度
					if(rx_message.Data[0]=='Z'&&rx_message.Data[1]=='S')
					{
						// VESCmotor[3].enable=rx_message.Data[2];
						u16 Defense_speed;
						DecodeU16Data(&Defense_speed,&rx_message.Data[3]);
						// VESCmotor[3].valueSet.speed=Defense_speed/10.f;
						ELMOmotor[0].valueSet.speed=Defense_speed/10.f;
						JV(1, SetData);
						// control_another(0, ELMO);
						answer_master(&rx_message);
					}
					//转箭机构旋转角度
					else if(rx_message.Data[0]=='F'&&rx_message.Data[1]=='J')
					{
						motor[5].enable=rx_message.Data[2];
						s16 Handle_angle;
						DecodeS16Data(&Handle_angle,&rx_message.Data[3]);
						motor[5].valueSet.angle=-(Handle_angle/10.f);
						answer_master(&rx_message);
					}
				}
			#endif
			
		}
		if(rx_message.IDE==CAN_ID_STD&&rx_message.RTR==CAN_RTR_DATA)
		{
			if(rx_message.StdId==0x100)
			{
				if(rx_message.Data[1]=='D'&&rx_message.Data[2]=='J')
				{
					if(rx_message.Data[0]!=3)
						DecodeFloatDataLost(&motor[rx_message.Data[0]].valueSet.angle, &rx_message.Data[3]);
					DecodeU16Data(&motor[rx_message.Data[0]].limit.posSPlimit, &rx_message.Data[5]);
				}
				else if(rx_message.Data[1]=='V'&&rx_message.Data[2]=='E')
				{
					DecodeS16Data(&VESCmotor[rx_message.Data[0]].valueSet.speed, &rx_message.Data[3]);
				}
			}
			if(rx_message.StdId==0x101)
			{
				flag.FlagShooting=1;
			}
		}
	}
}

void CAN1_RX1_IRQHandler()
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive (CAN1,CAN_FIFO1 ,&rx_message);
		CAN_ClearITPendingBit (CAN1,CAN_IT_FMP1);
	}
}

void answer_master(CanRxMsg *rx_message)
{
	static CanTxMsg Can1_tx_message;
	u8 CAN1_Command=rx_message->ExtId&0x0000000f;
	Can1_tx_message.ExtId=(0x05141310|CAN1_Command);
	Can1_tx_message.IDE=CAN_ID_EXT;
	Can1_tx_message.RTR=CAN_RTR_DATA;
	switch(CAN1_Command)
	{
		case 0:
			for(u8 i=0;i<8;i++)
				motor[i].enable=DISABLE;
			for(u8 j=0;j<5;j++)
			{
				VESCmotor[j].enable=DISABLE;
				MO(j, SetData, DISABLE);
			}
			break;
		case 2:
			Can1_tx_message.DLC=rx_message->DLC;
			memcpy(&Can1_tx_message.Data[0],&rx_message->Data[0],rx_message->DLC);
			break;
		case 6:
			Can1_tx_message.DLC=rx_message->DLC;
			if(rx_message->Data[0]=='X'&&rx_message->Data[1]=='Z')
			{
				Can1_tx_message.Data[0]=rx_message->Data[0];
				Can1_tx_message.Data[1]=rx_message->Data[1];
				Can1_tx_message.Data[2]=motor[2].status.arrived;
				Can1_tx_message.Data[3]=0;
			}
			else if(rx_message->Data[0]=='F'&&rx_message->Data[1]=='S')
			{
				memcpy(&Can1_tx_message.Data[0],&rx_message->Data[0],rx_message->DLC);
			}
			else if(rx_message->Data[0]=='J'&&rx_message->Data[1]=='D')
			{
				Can1_tx_message.Data[0]=rx_message->Data[0];
				Can1_tx_message.Data[1]=rx_message->Data[1];
				Can1_tx_message.Data[2]=motor[4].status.arrived;
				Can1_tx_message.Data[3]=0;
			}
			break;
		case 7:
			Can1_tx_message.DLC=2;
			Can1_tx_message.Data[0]=rx_message->Data[0];
			Can1_tx_message.Data[1]=rx_message->Data[1];
			break;
		default:break;
	}
	static u16 flag_can1;
	if(CAN_Transmit(CAN1,&Can1_tx_message)==0x04)
	{
		flag_can1++;
	}
	else 
		flag_can1=0;
}






















