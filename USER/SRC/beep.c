#include "beep.h"

u8 Beep_Count;

int tune[] =  
{  
//   /*蝴蝶的时间*/
 M5,M3,
 M2,M3,M5,M2,half_M2,ZERO,M5,M3,
 M2,M3,M6,M3,ZERO,M2,M1,
 M2,M3,M7,H1,M7,M5,
 M6,M5,M6,M6,M3,M2,M1,
 M2,M1,M2,L6,ZERO,L6,M6,
 M6,M5,M2,M3,ZERO,L5,
 L6,M1,M1,L6,M6,M5,M6,
 M2,M3,ZERO,
 M5,M3,
 M2,M3,M5,M2,half_M2,ZERO,M5,M3,
 M2,M3,M6,M3,ZERO,
 M2,M1,
 M2,M3,M7,H1,H2,H3,H2,
 M7,M6,M6,M5,M6,M5,
 M6,M5,M6,M1,ZERO,M2,M1,
 M1,M5,ZERO,M6,M3,M2,ZERO,L5,
 L6,M1,M1,L6,M3,M1,ZERO,L6,M1,L7

/*天空之城*/
//  M6,M7,H1,M7,H1,H3,M7,M7,M7,M3,M3,

//  M6,M5,M6,H1,M5,M5,M5,M3,M4,M3,M4,H1,

//  M3,M3,ZERO,H1,H1,H1,M7,half_M4,M4,M7,M7,M7,ZERO,M6,M7,

//  H1,M7,H1,H3,M7,M7,M7,M3,M3,M6,M5,M6,H1,

//  M5,M5,M5,M2,M3,M4,H1,M7,M7,H1,H1,H2,H2,H3,H1,H1,H1,

//  H1,M7,M6,M6,M7,half_M5,M6,M6,M6,H1,H2,H3,H2,H3,H5,

//  H2,H2,H2,M5,M5,H1,M7,H1,H3,H3,H3,H3,H3,

//  M6,M7,H1,M7,H2,H2,H1,M5,M5,M5,H4,H3,H2,H1,

//   H3,H3,H3,H3,H6,H6,H5,H5,H3,H2,H1,H1,ZERO,H1,

//   H2,H1,H2,H2,H5,H3,H3,H3,H3,H6,H6,H5,H5,

//   H3,H2,H1,H1,ZERO,H1,H2,H1,H2,H2,M7,M6,M6,M6,M6
};

float duration[]= 
{  
	0.5,0.5, 
  0.5,0.5,0.5,0.25,0.25,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  0.5,0.5,0.5,1,0.5,0.5,
  1,0.5,0.5,0.5,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  0.5,0.5,0.5,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  1,1,1,
  0.5,0.5,
  0.5,0.5,0.5,0.25,0.25,1,0.5,0.5,
  0.5,0.5,0.5,0.5,1,
  0.5,0.5,
  0.5,0.5,1,0.5,0.5,1,
  0.5,0.5,0.5,0.5,0.5,0.5,
  0.5,0.5,0.5,0.5,1,0.5,0.5,
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
  1,1


//  0.5,0.5,     1.5,0.5,1,1,     1,1,1,0.5,0.5,
//  1.5,0.5,1,1,     1,1,1,1,          1.5,0.5,1,1, 
//  1,1,0.5,0.5,0.5,0.5,    1+0.5,0.5,1,1,     1,1,1,0.5,0.5,
//  1+0.5,0.5,1,1,    1,1,1,0.5,0.5,     1+0.5,0.5,1,1,
//  1,1,1,0.5,0.5,    1,0.5,0.25,0.25,0.25,0.5,    0.5,0.5,0.5,0.25,0.5,1,
//  0.5,0.5,0.5,0.5,1,1,    1,1,1,0.5,0.5,    1+0.5,0.5,1,1,
//  1,1,1,0.5,0.5,    1.5,0.5,1,1,    1,1,1,1,
//  0.5,0.5,1,1,0.5,0.5,    1.5,0.25,0.5,1,    1,1,1,1,
//   1,1,1,1,    1,1,1,1,    0.5,0.5,1,1,0.5,0.5,
//   1,0.5,0.5,1,1,    1,1,1,1,    1,1,1,1,
//   0.5,0.5,1,1,0.5,0.5,    1,0.5,0.5,1.5,1,    1,1,1,0.5
};//这部分是整首曲子的节拍部分，也定义个序列duration，浮点（数组的个数和前面音符的个数是一样的，一一对应）

int length = sizeof(tune)/sizeof(tune[0]);//这里用了一个sizeof函数， 可以查出tone序列里有多少个音符

void Beep_Configuration(void)
{
	GPIO_Set(GPIOA,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_NONE);  
}

void Beep_Show(u8 num)
{
	Beep_Count+=num;
}

void Beep_Music()
{
	GPIO_Set(GPIOA,PIN8,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_AF_Set(GPIOA,8,1); //PA8,AF1
	for(int x=0;x<length;x++)//循环音符的次数
	{  
		//设置成复用模式（此处就是PWM端口），输出pwm波
		TIM1_PWM_Init(tune[x],8400-1);  //（arr,psc）
		TIM_SetCompare1(TIM1,tune[x]*0.5);  // 修改占空比
		OSTimeDly(8000*duration[x]);//否则延时会很长
	}
	GPIO_Set(GPIOA,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_2M,GPIO_PUPD_PU);//把PA8设置成普通IO口输入就不会有噪声了.
}



