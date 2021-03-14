#include "can2.h"

void CAN2_Configuration()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStruture;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_CAN2,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStruture.GPIO_Mode= GPIO_Mode_AF ;
	GPIO_InitStruture.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6 ;
	GPIO_Init(GPIOB,&GPIO_InitStruture);

	GPIO_PinAFConfig (GPIOB,GPIO_PinSource5,GPIO_AF_CAN2);
	GPIO_PinAFConfig (GPIOB,GPIO_PinSource6,GPIO_AF_CAN2);
	
	NVIC_InitStructure.NVIC_IRQChannel =CAN2_RX0_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_InitStructure.CAN_TTCM=DISABLE;  									//非时间触发通道模式
	CAN_InitStructure.CAN_ABOM=DISABLE;  									//软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
	CAN_InitStructure.CAN_AWUM=DISABLE;  									//睡眠模式由软件唤醒
	CAN_InitStructure.CAN_NART=DISABLE;  									//禁止报文自动发送，即只发送一次，无论结果如何
	CAN_InitStructure.CAN_RFLM=DISABLE;  									//报文不锁定，新的覆盖旧的
	CAN_InitStructure.CAN_TXFP=DISABLE;  									//发送FIFO的优先级由标识符决定
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;		        //CAN硬件工作在正常模式
	
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;  //重新同步跳跃宽度为一个时间单位
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq;  //时间段1占用9个时间单位
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;  //时间段2占用4个时间单位
	CAN_InitStructure.CAN_Prescaler=3;	    //分频系数
	CAN_Init(CAN2,&CAN_InitStructure);      //初始化CAN2
	
	//大疆3508或2006过滤器
	CAN_FilterInitStructure.CAN_FilterNumber =14;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =0X201<<5;
	CAN_FilterInitStructure.CAN_FilterIdLow=0X202<<5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =0X203<<5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =0X204<<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);	
	
	CAN_FilterInitStructure.CAN_FilterNumber =15;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =0X205<<5;
	CAN_FilterInitStructure.CAN_FilterIdLow =0X206<<5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =0X207<<5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =0X208<<5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);	
	
	//ELMO过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 16;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor1_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor2_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor3_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor4_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 17;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor5_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor6_RX<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor7_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor8_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
		
	CAN_FilterInitStructure.CAN_FilterNumber = 18;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor1_error<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor2_error<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor3_error << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor4_error << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
		
	CAN_FilterInitStructure.CAN_FilterNumber = 19;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh =Elmo_Motor5_error<< 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Elmo_Motor6_error<< 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =Elmo_Motor7_error << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =Elmo_Motor8_error << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//VESC过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = 20;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ((((uint32_t)CAN_PACKET_STATUS << 8) << 3) & 0xffff0000) >> 16;//0x00000900,CAN_PACKET_STATUS=9
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)CAN_PACKET_STATUS << 8) << 3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xffffff00 << 3) >> 16;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xffffff00 << 3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);
	
	CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);
}

//RX0用于接收大疆电调报文
void CAN2_RX0_IRQHandler()
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive (CAN2,CAN_FIFO0 ,&rx_message);
		CAN_ClearITPendingBit (CAN2,CAN_IT_FMP0);
		CAN_ClearFlag(CAN2,CAN_IT_FMP0);
		#ifdef USE_DJ
		if ((rx_message.StdId >= 0x201) && (rx_message.StdId <= 0x208) && (rx_message.RTR == CAN_RTR_DATA)) //DJ电机报文处理
		{
			u8 id = rx_message.StdId - 0x201;
			motor[id].argum.lastRxTim = OSTimeGet();
			// ChangeData(&rx_message.Data[0],&rx_message.Data[1]);//硬件发送报文时，先发低8位，后发高8位（硬件就是这么设置的，好像涉及微机原理）
			// ChangeData(&rx_message.Data[2],&rx_message.Data[3]);//但是大疆电调接收和返回报文时是反过来的，所以咱们发送和接收报文时也要进行一次高8位和低8位的互换
			// ChangeData(&rx_message.Data[4],&rx_message.Data[5]);
			// DecodeS16Data(&motor[id].valueReal.pulseRead,&rx_message.Data[0]);//用指针格式，从data【0】地址开始往后读16位
			// DecodeS16Data(&motor[id].valueReal.speed,&rx_message.Data[2]);//另一种写法：motor[id].valueReal.pulseRead = (RxMessage.Data[0] << 8) | (RxMessage.Data[1]);
			// DecodeS16Data(&motor[id].valueReal.current,&rx_message.Data[4]);
			motor[id].valueReal.pulseRead = (rx_message.Data[0]<<8)|(rx_message.Data[1]);//"|运算符，相对应位都为0则结果为0，反之则为1"
			motor[id].valueReal.speed = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
			motor[id].valueReal.current = (rx_message.Data[4]<<8)|(rx_message.Data[5]);
			motor[id].valueReal.temperature=rx_message.Data[6];

			pulse_calculate(id);
			if (!motor[id].status.clearFlag) //上电第一次进中断清除位置计算误差。
			{
				motor[id].status.clearFlag = true;
				motor[id].argum.distance = 0;
				motor[id].valueReal.pulse = 0;
			}
		}
		#endif
 	}
}

static void Elmo_Feedback_Deel(MesgControlGrpTypeDef* Can_MesgSentList)
{
	Can_MesgSentList->ReceiveNumber+=1;
	Can_MesgSentList->TimeOut=0;
	Can_MesgSentList->SendSem--;
	Can_MesgSentList->SendQueue.Front=(Can_MesgSentList->SendQueue.Front+1)%CAN_QUEUESIZE;
}

void CAN2_RX1_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP1)!=RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive(CAN2,CAN_FIFO1,&rx_message);
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP1);
		CAN_ClearFlag(CAN2,CAN_IT_FMP1);
		
		#ifdef USE_ELMO
		if(((rx_message.StdId>=0x281)&&(rx_message.StdId<=0x288))&&(rx_message.RTR==CAN_RTR_Data))
		{
			u8 id=rx_message.StdId-0x281;
			s32 ELMO_Recieve_Value;
			ELMOmotor[id].argum.lastRxTim=OSTimeGet();
			if(	(rx_message.Data[0]=='A'&&rx_message.Data[1]=='C'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='B'&&rx_message.Data[1]=='G'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='I'&&rx_message.Data[1]=='Q'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='J'&&rx_message.Data[1]=='V'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='M'&&rx_message.Data[1]=='O'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='P'&&rx_message.Data[1]=='E'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='P'&&rx_message.Data[1]=='X'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='S'&&rx_message.Data[1]=='T'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='T'&&rx_message.Data[1]=='C'&&(rx_message.Data[3]&BIT6)!=1)|
			(rx_message.Data[0]=='U'&&rx_message.Data[1]=='M'&&(rx_message.Data[3]&BIT6)!=1) )
			Elmo_Feedback_Deel(&Can2_MesgSendList[id]);
			if(rx_message.Data[0]=='M'&&rx_message.Data[1]=='O'&&(rx_message.Data[3]&BIT6)!=1)
			{
				ELMOmotor[id].enable=rx_message.Data[4];
			}
			if(rx_message.Data[0]=='V'&&rx_message.Data[1]=='X'&&(rx_message.Data[3]&BIT6)!=1)
			{
				DecodeS32Data(&ELMO_Recieve_Value,&rx_message.Data[4]);
				ELMOmotor[id].valueReal.speed=ELMO_Recieve_Value/(ELMOmotor[id].intrinsic.PULSE/60);
			}
			if(rx_message.Data[0]=='P'&&rx_message.Data[1]=='X'&&(rx_message.Data[3]&BIT6)!=1)
			{
				DecodeS32Data(&ELMOmotor[id].valueReal.pulse,&rx_message.Data[4]);
				ELMOmotor[id].valueReal.angle=ELMOmotor[id].valueReal.pulse*(360.f/ELMOmotor[id].intrinsic.RATIO)/ELMOmotor[id].intrinsic.PULSE;
			}
			if(rx_message.Data[0]=='I'&&rx_message.Data[1]=='Q'&&(rx_message.Data[3]&BIT6)!=1)
			{
				DecodeS32Data(&ELMOmotor[id].valueReal.current,&rx_message.Data[4]);
			}
		}
		#endif
		
		#ifdef USE_VESC
		if ((rx_message.IDE == CAN_ID_EXT) && (rx_message.RTR == CAN_RTR_DATA)) //VESC报文
		{
			int32_t ind = 0;
			u8 id=rx_message.ExtId&0xff;
			VESCmotor[id-1].argum.lastRxTim=OSTimeGet();
			if ((rx_message.ExtId >> 8) == CAN_PACKET_STATUS)
			{
				VESCmotor[id-1].valueReal.speed = (s32)(buffer_32_to_float(rx_message.Data, 1e0, &ind) / VESCmotor[id-1].instrinsic.POLE_PAIRS);
				VESCmotor[id-1].valueReal.current =	buffer_16_to_float(rx_message.Data, 1e1, &ind);
//				VESCmotor[id-1].valueReal.duty =	buffer_16_to_float(rx_message.Data, 1e3, &ind);
				VESCmotor[id-1].valueReal.angle =	buffer_16_to_float(rx_message.Data, 10, &ind);
				//位置计算
				ChangeData(&rx_message.Data[6],&rx_message.Data[7]);
				DecodeU16Data(&VESCmotor[id].valueReal.angleNow,&rx_message.Data[6]);//此时得到的数据是angle*10，当作pulseRead来进行处理
				VESCmotor[id].argum.distance=VESCmotor[id].valueReal.angleNow-VESCmotor[id].valueReal.anglePrv;
				if(VESCmotor[id].argum.firstPos)
				{
					VESCmotor[id].argum.firstPos=false;
					VESCmotor[id].argum.distance=0;//绝对值编码器的特性，上电即有初角度，distance>0，必须要通过此步清零
				}
				VESCmotor[id].valueReal.anglePrv=VESCmotor[id].valueReal.anglePrv;
				if(ABS(VESCmotor[id].argum.distance)>1800)
					VESCmotor[id].argum.distance-=SIG(VESCmotor[id].argum.distance)*3600;
				VESCmotor[id].valueReal.position+=VESCmotor[id].argum.distance;
				//位置残差更新
				VESCmotor[id].argum.difPosition=VESCmotor[id].valueSet.position-VESCmotor[id].valueReal.position;
				if(VESCmotor[id].begin)
				{
					VESCmotor[id].argum.lockAngle=VESCmotor[id].valueReal.angle;
					VESCmotor[id].argum.lockPosition=VESCmotor[id].valueReal.position;
				}
			}
		}		
		#endif
	}
}

static CanTxMsg DJ_tx_message;
void currentInput(u8 id)
{
	PEAK(motor[id].valueSet.current,motor[id].intrinsic.CURRENT_LIMIT);
	if(!motor[id].enable) 
		motor[id].valueSet.current =0;
	if(id<4) DJ_tx_message.StdId =0x200;
	else DJ_tx_message.StdId =0x1FF;
	DJ_tx_message.RTR=CAN_RTR_Data;
	DJ_tx_message.IDE=CAN_ID_STD;
	DJ_tx_message.DLC=8;
	u8 temp=2*(id&0x0B);//0x0B=00001011
	EncodeS16Data(&motor[id].valueSet.current, &DJ_tx_message.Data[temp]);
	ChangeData (&DJ_tx_message.Data[temp], &DJ_tx_message.Data[temp+1]);//因为大疆电调接收报文格式的特殊性，要求要进行高8位和低8位的换位
	if(id==3||id==7)
	  CAN_Transmit (CAN2, &DJ_tx_message);//每4个id发一次报文，因为是tx_message是全局变量数据不会覆盖，每8个data位存储4个id的电流数据
	//如果不加这个语句，一共发8次，邮箱会塞满，会报文溢出,id=5-8的报文都发送不出去
}

/*电磁阀可能要求实时性较高，用队列可能延迟比较高，即发即送可能会好一点*/
void CAN2_Valve(u8 Valve_id, u8 Valve_Flag)
{
	static CanTxMsg tx_message;
	tx_message.ExtId=0x00010400;
	tx_message.IDE=CAN_ID_EXT;
	tx_message.RTR=CAN_RTR_DATA;
	tx_message.DLC=4;
	tx_message.Data[0]=0x04;
	if(Valve_Flag==Valve_On)
	{
		tx_message.Data[1]='O'+0x40;//0x40是防止和驱动器报文有重复
		tx_message.Data[2]='N';
		tx_message.Data[3]=Valve_id;
	}
	else if(Valve_Flag==Valve_Off)
	{
		tx_message.Data[1]='E'+0x40;//0x40是防止和驱动器报文有重复
		tx_message.Data[2]='F';
		tx_message.Data[3]=Valve_id;
	}
	else if(Valve_Flag==Valve_NOW)
	{
		tx_message.Data[1]='N'+0x40;//0x40是防止和驱动器报文有重复
		tx_message.Data[2]='O';
		tx_message.Data[3]='W';
		tx_message.Data[4]=Valve_id;
	}
	CAN_Transmit(CAN2,&tx_message);
}

