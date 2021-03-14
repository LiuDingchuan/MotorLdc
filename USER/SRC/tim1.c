#include "tim1.h"

//定时器1为高级定时器，输出pwm波负责唱歌

void TIM1_PWM_Init(u32 arr, u32 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//84MHZ，分频系数    
		TIM_TimeBaseStructure.TIM_Period = arr;//1s，自动重装载值   
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择模式 PWM1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性低
    TIM_OC1Init(TIM1, &TIM_OCInitStructure); //根据T指定的参数初始化外设TIM1 OC1
    
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);//高级定时器才有，输出pwm必须有这句
		TIM_Cmd(TIM1,ENABLE);
}



