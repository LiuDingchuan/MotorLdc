#include "tim3.h"
extern int Pressed;
extern int function[16];
s32 VESC_Speed_Now[5];

void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;//42MHZ，分频系数    
	TIM_TimeBaseStructure.TIM_Period = 1000-1;//1ms，自动重装载值   
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;    
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM3,ENABLE);
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET)
	{
			#ifdef USE_DJ
			for(int id=0;id<8;id++)
			{
				if(motor[id].enable)
				{
					if(motor[id].begin)
					{
						switch(motor[id].mode)
						{
							case current:;break;
							case RPM: 
								speed_mode(id);break;
							case position: 
								position_mode(id);break;
							case zero: 
								zero_mode(id); break;
							default:break;
						}     
					}
					else position_mode(id);
				}
				currentInput(id);
			}
			#endif
			
			#ifdef USE_VESC
			for (int i = 1; i < 3; i++)
			{
				if (VESCmotor[i].enable)
				{
					if (VESCmotor[i].begin)
					{
							switch (VESCmotor[i].mode)
							{ 
									case duty://0
											VESC_Set_Duty_Cycle(i + 1, VESCmotor[i].valueSet.duty, 1);
											break;
									case current://1
											VESC_Set_Current(i + 1, VESCmotor[i].valueSet.current, 1);
											break;
									case RPM://2
											VESC_Set_Speed(i + 1, VESCmotor[i].valueSet.speed * VESCmotor[i].instrinsic.POLE_PAIRS, 1);
											break;
									case brake://3
											VESC_Set_Handbrake_Current(i + 1, VESCmotor[i].limit.breakCurrent, 1);
											break;
									case position://5
											VESC_position_mode_pos(i+1);
											break;
									default:
											break;
							}
					}
					else
						VESC_Set_Handbrake_Current(i + 1, VESCmotor[i].limit.breakCurrent, 1);
				}
				else
						VESC_Set_Current(i + 1, 0.0, 1); //发送电流
			}
			#endif
			
			Can_DeQueue(CAN1,&Can1_Sendqueue);
			Can_DeQueue(CAN2,&Can2_Sendqueue);    
			
//			if(flag.Can2CtrlList_Enable)
//				TraversalControlList(Can2_MesgSendList,&Can2_Sendqueue,CAN_2);
	} 
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}


/*void TIM3_IRQHandler(void)直接用串口屏发送指令吧
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		Key_Ctrl();//检测矩阵键盘按钮
		
		if(Pressed==1)
		{
			if(function[0]==1)
			{
				ELMO_Enable(1);//u8 id，使能电机
				function[0]=0;
				Pressed=0;
			}
			else if(function[1]==1)
			{
				ELMO_Disable(1);//u8 id，失能电机
				function[1]=0;
				Pressed=0;
			}
			else if(function[2]==1)
			{
				ELMO_PositionMode(1);//u8 id	
				ELMO_Enable(1);
				ELMO_PositionMode_Speed(1);//u8 id,speed
		    
				ELMO_PositionMode_Angle(1);//u8 id,3.7*4000(4000转一圈，减速比3.7)
			  
				ELMO_Motor_Begin(1);
				function[2]=0;
				Pressed=0;
			}
			else if(function[3]==1)
			{
				ELMO_SpeedMode(1);//u8 id
				ELMO_Enable(1);
				ELMO_SpeedMode_Speed(1);//u8 id,speed
        
				ELMO_Motor_Begin(1);
				function[3]=0;
				Pressed=0;
			}
			else if(function[4]==1)
			{
				ELMO_SaveData(1);//u8 id
				function[4]=0;
				Pressed=0;
			}
			else if(function[5]==1)//实际运行的时候，它实际接收到的电流可能远没有2A，所以电机跑不起来
			{
				ELMO_CurrentMode(1);
        ELMO_Enable(1);
				ELMO_Current(1);
				function[5]=0;
				Pressed=0;
			}
			else if(function[6]==1)
			{
				ELMO_Motor_Begin(1);
				function[6]=0;
				Pressed=0;
			}
			else if(function[7]==1)
			{
				ELMO_Motor_Stop(1);
				function[7]=0;
				Pressed=0;
			}
			else if(function[8]==1)
			{
				ELMO_SetZero(1);
				function[8]=0;
				Pressed=0;
			}
			else if(function[9]==1)
			{
				ELMO_Position_Feedback(1);
				function[9]=0;
				Pressed=0;
			}
			else if(function[10]==1)
			{
				ELMO_Speed_Feedback(1);
				function[10]=0;
				Pressed=0;
			}
			else if(function[11]==1)
			{
				ELMO_speedplus(&ELMOmotor);
				function[11]=0;
				Pressed=0;
			}
		}
			Can_DeQueue(CAN2,&Can2_Sendqueue);
			if(flag.Can2CtrlList_Enable)
				TraversalControlList(Can2_MesgSendList,&Can2_Sendqueue,CAN_2);
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}*///ELMO
















