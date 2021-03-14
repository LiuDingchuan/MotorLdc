#include "elmo.h"

ELMOParam EC_4P_22,U10,U8,U5;
ELMOLimit ELMOlimit[5];
ELMOArgum ELMOargum;
ELMO_Motor ELMOmotor[5];

/*ELMO电机参数初始化*/
void ELMO_Motor_Init(void)
{
	{
		EC_4P_22.PULSE=4000;
		EC_4P_22.RATIO=3.7f;//MAXON Gear 166930
        
		U10.PULSE=4096*4;
		U10.RATIO=1;
		
		U8.PULSE=4096*4;
		U8.RATIO=1;
		
		U5.PULSE=1000*4;
		U5.RATIO=1;
	}
	{//电机限制保护
		ELMOlimit[0].isPosLimitON=false;
		ELMOlimit[0].maxAngle=720.f;
		ELMOlimit[0].isPosSPLimitOn=false;
		ELMOlimit[0].posSPlimit=1000;
		ELMOlimit[0].isRealseWhenStuck=false;
		ELMOlimit[0].zeroSP=500;
		ELMOlimit[0].zeroCurrent=1000;
		ELMOlimit[0].timeoutMotion=1;
		
		ELMOlimit[1].isPosLimitON=false;
		ELMOlimit[1].maxAngle=720.f;
		ELMOlimit[1].isPosSPLimitOn=false;
		ELMOlimit[1].posSPlimit=500;
		ELMOlimit[1].isRealseWhenStuck=false;
		ELMOlimit[1].zeroSP=500;
		ELMOlimit[1].zeroCurrent=1000;
		ELMOlimit[1].timeoutMotion=1;
		
		ELMOlimit[2].isPosLimitON=false;
		ELMOlimit[2].maxAngle=720.f;
		ELMOlimit[2].isPosSPLimitOn=false;
		ELMOlimit[2].posSPlimit=500;
		ELMOlimit[2].isRealseWhenStuck=false;
		ELMOlimit[2].zeroSP=500;
		ELMOlimit[2].zeroCurrent=1000;
		ELMOlimit[2].timeoutMotion=1;
	}
	{//电机其他参数设置
		ELMOargum.lastRxTim=0;
		ELMOargum.timeoutTicks=1000;//2000ms
	}
	ELMOmotor[0].intrinsic=U5;
	ELMOmotor[0].enable=DISABLE;
	ELMOmotor[0].mode=RPM;
	ELMOmotor[0].valueSet.angle=0;
	ELMOmotor[0].valueSet.speed=0;
	ELMOmotor[0].valueSet.current=0;
	
	ELMOmotor[0].limit=ELMOlimit[0];
	ELMOmotor[0].argum=ELMOargum;
    
	ELMOmotor[1].intrinsic=U5;
	ELMOmotor[1].enable=DISABLE;
	ELMOmotor[1].mode=RPM;
	ELMOmotor[1].valueSet.angle=360;
	ELMOmotor[1].valueSet.speed=200;
	ELMOmotor[1].valueSet.current=2;
	
	ELMOmotor[1].limit=ELMOlimit[0];
	ELMOmotor[1].argum=ELMOargum;
	
	ELMOmotor[2].intrinsic=U8;
	ELMOmotor[2].enable=DISABLE;
	ELMOmotor[2].mode=RPM;
	ELMOmotor[2].valueSet.angle=360;
	ELMOmotor[2].valueSet.speed=200;
	ELMOmotor[2].valueSet.current=2;
	
	ELMOmotor[2].limit=ELMOlimit[0];
	ELMOmotor[2].argum=ELMOargum;
}

//ELMO初始化
void ELMO_Init(CAN_TypeDef* CANx)
{
	CanTxMsg tx_message;
	tx_message.StdId=0x000;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.IDE = CAN_Id_Standard;
	tx_message.DLC=8;
	tx_message.Data[0]=0x01;
	tx_message.Data[1]=0x00;
	tx_message.Data[2]=0x00;
	tx_message.Data[3]=0x00;
	tx_message.Data[4]=0x00;
	tx_message.Data[5]=0x00;
	tx_message.Data[6]=0x00;
	tx_message.Data[7]=0x00;
	CAN_Transmit(CANx,&tx_message);
}

void AC(u32 id,u8 cmd,u32 valueSet)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&valueSet,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		Can2_Sendqueue.Rear=Rear2;
}

//启动电机
void BG(u32 id)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
	 	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='B';
	  	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='G';
	  	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
	  	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
	  	Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		Can2_Sendqueue.Rear=Rear2;
}

void DC(u32 id,u8 cmd,u32 valueSet)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='D';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='D';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&valueSet,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
	Can2_Sendqueue.Rear=Rear2;
}

void IQ(u32 id)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='I';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='Q';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x80;//float
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
	Can2_Sendqueue.Rear=Rear2;
}

//设定速度模式下的持续速度
void JV(u32 id,u8 cmd)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='J';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='V';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
		}
		else if(cmd==SetData)
		{
			s32 speed=ELMOmotor[id-1].valueSet.speed*ELMOmotor[id-1].intrinsic.PULSE/60;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='J';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='V';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&speed,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
	  Can2_Sendqueue.Rear=Rear2;
}

//使能、失能电机
void MO(u32 id,u8 cmd,u32 status)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='O';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x40;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='O';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=status;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0;
		}
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		Can2_Sendqueue.Rear=Rear2;
}

//设定位置模式下转的角度
void PA(u32 id,u8 cmd)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
	    Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
			if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			s32 position_set=ELMOmotor[id-1].valueSet.angle/360.f*ELMOmotor[id-1].intrinsic.PULSE*ELMOmotor[id-1].intrinsic.RATIO;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='A';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&position_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
        Can2_Sendqueue.Rear=Rear2;
}

/*获得相对位置*/
void ELMO_PR(u32 id,u8 cmd)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
        if(cmd==GetData)
        {
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='R';
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
        }
        else if(cmd==SetData)
        {
            s32 position_set=ELMOmotor[id-1].valueSet.angle/360.f*ELMOmotor[id-1].intrinsic.PULSE*ELMOmotor[id-1].intrinsic.RATIO;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='R';
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
            EncodeS32Data(&position_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
        }
            Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
        Can2_Sendqueue.Rear=Rear2;
}

/*获取位置误差*/
void PE(u32 id)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='E';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
	Can2_Sendqueue.Rear=Rear2;
}

//查询电机主位置
void PX(u32 id,u8 cmd)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='X';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			s32 position_set=ELMOmotor[id-1].valueSet.angle/360.f*ELMOmotor[id-1].intrinsic.PULSE*ELMOmotor[id-1].intrinsic.RATIO;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='X';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&position_set,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
		 Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		 Can2_Sendqueue.Rear=Rear2;
}


//设置位置模式下的转速SP
void SP(u32 id,u8 cmd)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x40;
		}
		else if(cmd==SetData)
		{
			s32 speed_position;
			speed_position=ELMOmotor[id-1].valueSet.speed*ELMOmotor[id-1].intrinsic.PULSE/60;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='P';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			EncodeS32Data(&speed_position,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
		}
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		    Can2_Sendqueue.Rear=Rear2;
}

//设定电流
void TC(u32 id)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
      float current=ELMOmotor[id-1].valueSet.current;
      Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='T';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='C';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
      EncodeS32Data(&current,&Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]);
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		  Can2_Sendqueue.Rear=Rear2;
}


//运动模式
void UM(u32 id,u8 cmd,u32 status)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
    Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;		
		if(cmd==GetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='U';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		}
		else if(cmd==SetData)
		{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x08;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='U';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='M';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[4]=status;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[5]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[6]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[7]=0;
		}
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
	    Can2_Sendqueue.Rear=Rear2;
}

//制动电机
void ST(u32 id)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='S';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='T';
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0;
			Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		  Can2_Sendqueue.Rear=Rear2;
}

//查询电机转速(VX)
void VX(u8 id)
{
	if(Rear2==Can2_Sendqueue.Front)
	{
		flag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].ID=0x300+id;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].DLC=0x04;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[0]='V';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[1]='X';
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[2]=0;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].Data[3]=0x00;
		Can2_Sendqueue.Can_DataSend[Can2_Sendqueue.Rear].InConGrpFlag=1;
	}
		Can2_Sendqueue.Rear=Rear2;
}














