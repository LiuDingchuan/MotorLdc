#include "main.h"

int main(void)
{
	Delay_ms(3000);//Delay_ms(1)是延时1ms，OSTimeDly（1）是延时0.1ms
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
	LED_Configuration();
	Led8_Configuration();
	Beep_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	USART1_Configuration();
	USART2_Configuration();//会伴随着DJ电机的微微微抖，具体原因未知
	param_Init();  
	TIM2_Init();
	TIM3_Init();
	Can_SendqueueInit();
	static CanTxMsg Can1_tx_message;
	InitCANControlList(Can2_MesgSendList,CAN_2);
	OSInit();//初始化UCOSII    
	OSTaskCreate(Task_Start,(void *)0,&START_TASK_STK[START_TASK_STK_SIZE-1],START_TASK_PRIO);//创建起始任务
	OSStart();//启动UCOSII
	
}

/****起始任务****/
static void Task_Start(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OS_CPU_SysTickInit();//重要！！！不写没有任务调度
	OS_ENTER_CRITICAL();//进入临界区（使得创建任务时无法被中断打断）
	RUN=OSSemCreate(0);
	OSTaskCreate(Task_LCD, (void *)0, (OS_STK *)&LCD_TASK_STK[LCD_STK_SIZE-1], LCD_TASK_PRIO);
	OSTaskCreate(Task_ZERO, (void *)0, (OS_STK *)&ZERO_TASK_STK[ZERO_STK_SIZE-1], ZERO_TASK_PRIO);
	OSTaskCreate(Task_BEEP, (void *)0, (OS_STK *)&BEEP_TASK_STK[BEEP_STK_SIZE-1], BEEP_TASK_PRIO);
	OSTaskCreate(Task_ERROR, (void *)0, (OS_STK *)&ERROR_TASK_STK[ERROR_STK_SIZE-1], ERROR_TASK_PRIO);
//	OSTaskCreate(Task_TAKEARROW, (void *)0, (OS_STK *)&TAKEARROW_TASK_STK[TAKEARROW_TASK_STK_SIZE-1], TAKEARROW_TASK_PRIO);
	#ifdef USE_ELMO
	OSTaskCreate(Task_ELMO,(void *)0,(OS_STK *)&ELMO_TASK_STK[ELMO_STK_SIZE-1],ELMO_TASK_PRIO);
	#endif
	
	#ifdef USE_VESC
	OSTaskCreate(Task_VESC,	(void *)0,	(OS_STK *)&VESC_TASK_STK[VESC_STK_SIZE-1],	VESC_TASK_PRIO);
	#endif 
	
	#ifdef USE_VISUALSCOPE
	OSTaskCreate(Task_VISUALSCOPE,(void *)0,(OS_STK *)&VISUALSCOPE_TASK_STK[VISUALSCOPE_STK_SIZE-1],VISUALSCOPE_TASK_PRIO);
 	#endif
	
	#ifdef PICK_UP_ARROWS
	OSTaskCreate(Task_PICKUPARROW,(void *)0,(OS_STK *)&PICKUPARROW_TASK_STK[PICKUPARROW_STK_SIZE-1],PICKUPARROW_TASK_PRIO);
	#endif
	
	#ifdef Shooting
	OSTaskCreate(Task_SHOOTING,(void *)0,(OS_STK *)&SHOOTING_TASK_STK[SHOOTING_STK_SIZE-1],SHOOTING_TASK_PRIO);
	#endif
	
	Beep_Show(3);
	OSTaskSuspend(START_TASK_PRIO);//挂起起始任务
	OS_EXIT_CRITICAL();//退出临界区（可以被中断打断）
}

static void Task_LCD(void *pdata)
{
	while(1)
	{
		UsartLCDshow();
		OSTimeDly(1000);
	}
}

static void Task_ELMO(void *pdata)
{
	while(1)
	{
		for(int id=1;id<4;id++)
		{
			 VX(id);
			 OSTimeDly(1500);
			 IQ(id);
			 OSTimeDly(1500);
		}
		OSTimeDly(1000);   
	}
}

static void Task_VESC(void *pdata)
{
	while(1)
	{
		Can_DeQueue(CAN2,&VESC_Sendqueue);
		// VESC_Speed_Now[1]=VESCmotor[1].valueReal.speed;
		// VESC_Speed_Now[2]=VESCmotor[2].valueReal.speed;
		OSTimeDly(4);
	}
}

//让电机上电时去除误差
static void Task_ZERO(void *pdata)
{
	for(int id=0;id<8;id++)
	{
		motor[id].status.isSetZero=1;
		OSTimeDly(10);
	}
	OSTaskSuspend(ZERO_TASK_PRIO);//挂起起始任务  
}

static void Task_TAKEARROW(void *pdata)
{
	while (1)
	{
		static float Handle_Position_Prv, angle_first;
		if(flag.FlagTakeArrow==1)
		{
			motor[2].valueSet.angle=TakeArrowPosition;
			motor[3].valueSet.angle=-90-(TakeArrowPosition-Handle_Position_Prv)-motor[4].valueSet.angle-30-angle_first;
			OSTimeDly(50);
			if(motor[2].status.arrived)
				flag.FlagTakeArrow=2;
		}
		else if(flag.FlagTakeArrow==0)
		{
			Handle_Position_Prv=motor[2].valueReal.angle;
			angle_first=motor[3].valueReal.angle;
			OSTimeDly(50);
		}
		else if(flag.FlagTakeArrow==2)
		{
			/*等待下一个命令*/
			OSTimeDly(50);
		}
		else if(flag.FlagTakeArrow==3)
		{
			motor[2].valueSet.angle=Handle_Position_Prv;
			motor[3].valueSet.angle=angle_first;
			OSTimeDly(50);
			if(motor[2].status.arrived){
				flag.FlagTakeArrow=2;
			}
		}
	}
}

static void Task_PICKUPARROW(void *pdata)
{
	while(1)
	{
		if(motor[0].enable==ENABLE&&motor[1].enable==ENABLE)
		{
			if(ABS(motor[0].valueReal.angle)>=60&&ABS(motor[0].valueReal.angle)<=150)
				motor[1].valueSet.angle=ABS(motor[0].valueReal.angle)-60;
			else if(ABS(motor[0].valueReal.angle)>150)
				motor[1].valueSet.angle=90;
			else if(ABS(motor[0].valueReal.angle)<60)
				motor[1].valueSet.angle=0;
		}
		OSTimeDly(50);
	}
}

static void Task_SHOOTING(void *pdata)
{
	while(1)
	{
		static CanTxMsg Can1_tx_message;
		if(flag.FlagShooting==1)
		{
			Beep_Show(3);
			motor[3].enable=ENABLE;
			VESCmotor[1].enable=ENABLE;
			VESCmotor[2].enable=ENABLE;
			F4valveON
			OSTimeDly(30000);
//			motor[3].valueSet.angle+=DJ2006angle_gap;
			motor[3].valueSet.angle+=180;
			for(u8 j=0;j<3;j++)
			{
				Can1_tx_message.ExtId=0x05141316;
				Can1_tx_message.IDE=CAN_ID_EXT;
				Can1_tx_message.RTR=CAN_RTR_DATA;
				Can1_tx_message.DLC=2;
				Can1_tx_message.Data[0]='F';
				Can1_tx_message.Data[1]='N';
				CAN_Transmit(CAN1,&Can1_tx_message);
			}
			OSTimeDly(20000);
			F4valveOFF
			VESCmotor[1].enable=DISABLE;
			VESCmotor[2].enable=DISABLE;
			motor[3].valueSet.angle+=180;
			flag.FlagShooting=0;
		}
		OSTimeDly(100);
	}
}

//新示波器任务
static void Task_VISUALSCOPE(void *pdata)
{
	while (1)
	{
		u8 Send_Count;
		DataScope_Get_Channel_Data( ELMOmotor[0].valueReal.speed ,   1 );   //将数据   写入通道 1
		DataScope_Get_Channel_Data( ELMOmotor[1].valueReal.speed ,   2 );   //将数据   写入通道 2
		DataScope_Get_Channel_Data( VESCmotor[2].valueReal.speed,    3 );   //将数据   写入通道 3
		DataScope_Get_Channel_Data( VESCmotor[2].valueReal.current , 4 );   //将数据   写入通道 4
		DataScope_Get_Channel_Data( motor[2].valueReal.speed ,   5 );       //将数据   写入通道 5
		DataScope_Get_Channel_Data( motor[2].valueReal.current , 6 );       //将数据   写入通道 6
		DataScope_Get_Channel_Data( motor[3].valueReal.speed ,   7 );       //将数据   写入通道 7
		DataScope_Get_Channel_Data( motor[3].valueReal.current , 8 );       //将数据   写入通道 8
		DataScope_Get_Channel_Data( motor[4].valueReal.speed ,   9 );       //将数据   写入通道 9
		DataScope_Get_Channel_Data( motor[4].valueReal.current , 10); //将数据   写入通道 10
		Send_Count = DataScope_Data_Generate(10);    //数据转换，输入有几个通道
		USART1_Send(DataScope_OutPut_Buffer,Send_Count);
		OSTimeDly(400);
	}
}

static void Task_BEEP(void *pdata)
{
	while(1)
	{
		if(Beep_Count>0)
		{
			BEEP_ON;OSTimeDly(1000);
			BEEP_OFF;
			Beep_Count--;
		}
		else
		{
			Beep_Count=0;
			BEEP_OFF;
		}
		if(flag.FlagMusic==1)
		{
			Beep_Music();
			flag.FlagMusic=0;
		}
		OSTimeDly(1000);
	}
}

/*错误显示链表*/
static void Task_ERROR(void *pdata)
{
	ErrorList* _error;
	while(1)
	{
		if(Error.size==0) 
		{
			Led8DisData(8);
			Led_Show();
		}
		else
		{
			LED_RED_ON;
			Beep_Show(1);
			_error=Error.head->next;
			for(u8 i=0;i<Error.size;i++)
			{
				if((_error->code&0x000F)==SYSTEMERROR)
				{
					Led8DisData(10);
				}
				else if((_error->code&0x000F)==DJERROR)
				{
					LED_BLUE_ON;
					Led8DisData((_error->code&0x00F0)>>4);
					OSTimeDly(5000);
					if((_error->code&0x0F00)==STUCK)
					{
						Led8DisData(11);
						OSTimeDly(5000);
					}
					if((_error->code&0x0F00)==TIMEOUT)
					{
						Led8DisData(12);
						OSTimeDly(5000);
					}
				}
				else if((_error->code&0x000F)==ELMOERROR)
				{
					LED_YELLOW_ON;
					Led8DisData((_error->code&0x00F0)>>4);
				}
				else if((_error->code&0x000F)==VESCERROR)
				{
					LED_GREEN_ON;
					Led8DisData((_error->code&0x00F0)>>4);
				}
				_error=_error->next;
				OSTimeDly(1000);
				LED_BLUE_OFF;LED_YELLOW_OFF;LED_GREEN_OFF;
				OSTimeDly(1000);
			}
		}
		OSTimeDly(1000);
	}	
}










