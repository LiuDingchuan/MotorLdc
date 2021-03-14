#include "tim1.h"

//��ʱ��1Ϊ�߼���ʱ�������pwm�����𳪸�

void TIM1_PWM_Init(u32 arr, u32 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//84MHZ����Ƶϵ��    
		TIM_TimeBaseStructure.TIM_Period = arr;//1s���Զ���װ��ֵ   
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ģʽ PWM1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //������Ե�
    TIM_OC1Init(TIM1, &TIM_OCInitStructure); //����Tָ���Ĳ�����ʼ������TIM1 OC1
    
		TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);//�߼���ʱ�����У����pwm���������
		TIM_Cmd(TIM1,ENABLE);
}



