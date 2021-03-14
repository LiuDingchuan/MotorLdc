#include "tim2.h"

//定时器2负责超时检测

void TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseInitStructure.TIM_Prescaler = 168-1;//42MHZ，分频系数
	TIM_TimeBaseInitStructure.TIM_Period = 10-1; //200ms
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)//溢出中断
	{
		#ifdef USE_DJ
		for(u8 i=0;i<8;i++)		
		  ifDJTimeout(i);//DJ电机超时检测
		#endif
		
		#ifdef USE_ELMO
		for(u8 i=0;i<5;i++)
			ifElmoTimeout(i);//Elmo电机超时检测
		#endif
		
		#ifdef USE_VESC
		for(u8 i=0;i<4;i++)
		  ifVESCTimeout(i);//VESC电机超时检测
		#endif		
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清除中断标志位
}

void ifDJTimeout(u16 id)
{
	if(motor[id].enable==ENABLE)
	{
		if(motor[id].limit.timeoutMotion)
		{
			if(OSTimeGet()-motor[id].argum.lastRxTim>=1500)
			{
				motor[id].argum.timeoutCnt++;
			}
			else
			{
				motor[id].argum.timeoutCnt=0;
			}
			if(motor[id].status.timeout==0&&motor[id].argum.timeoutCnt>motor[id].argum.timeoutTicks)
			{
				motor[id].status.timeout=1;//超时标志位
				insertError(Error.head,DJERROR|(id<<4)|TIMEOUT);
			}
			else
			{
				motor[id].status.timeout=0;
				deleteError(Error.head,ErrorFind(Error.head,DJERROR|(id<<4)|TIMEOUT));
			}
		}
	}
}

void ifElmoTimeout(u8 id)
{	
	if(ELMOmotor[id].limit.timeoutMotion)//是否开启超时判断
	{
		if((OSTimeGet()-ELMOmotor[id].argum.lastRxTim)>ELMOmotor[id].argum.timeoutTicks)
			ELMOmotor[id].argum.timeoutCnt++;
		else
			ELMOmotor[id].argum.timeoutCnt=0;
		if(ELMOmotor[id].argum.timeoutCnt>1000)
		{
			ELMOmotor[id].status.timeout=1;
			insertError(Error.head,ELMOERROR|((id+1)<<4)|TIMEOUT);
		}
		else
		{
			ELMOmotor[id].status.timeout=0;
			deleteError(Error.head,ErrorFind(Error.head,ELMOERROR|((id+1)<<4)|TIMEOUT));
		}
	}	
}

void ifVESCTimeout(u16 id)
{
	if (VESCmotor[id].enable)
	{
		if(VESCmotor[id].limit.timeoutMotion)
		{
			if((OSTimeGet() - VESCmotor[id].argum.lastRxTim) > VESCmotor[id].argum.timeoutTicks)
				VESCmotor[id].argum.timeoutCnt++; //反馈超时判断		
			else
				VESCmotor[id].argum.timeoutCnt = 0;
			if (VESCmotor[id].argum.timeoutCnt > 100)
			{
				VESCmotor[id].status.timeout = 1;
				insertError(Error.head,VESCERROR|(id<<4)|TIMEOUT);
			}
			else
			{
				VESCmotor[id].status.timeout = 0;
				deleteError(Error.head,ErrorFind(Error.head, VESCERROR|(id<<4)|TIMEOUT));
			}
		}
	}
}








