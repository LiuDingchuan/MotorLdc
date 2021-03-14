====================
中断优先级：|
====================
TIM1                 PWM
TIM2            	 3   1
TIM3       			 1   0	
CAN1_RX0     	 	 3   0	
CAN2_RX0         	 2   0	
CAN2_RX1         	 2   1	
USART2_IRQn     	 4   1	手柄
USART2_IRQn(DMA)     7   2	手柄
USART1_IRQn_Dma 	 5   1 	虚拟示波器


/*错误反馈查询：
一切正常时，数码管显示数字8 
当出现错误时，红灯常亮报警
SYSTEM ERROR ：数码管显示A（可能是电源线没有接好或者是板子自身问题）
DJERROR：蓝灯常亮， 超时：数码管交替显示故障电机id和字母B  堵转：数码管交替显示故障电机id和字母C
ELMOERROR：超时：黄灯常亮，数码管显示故障elmoid
vescERROR：超时：绿灯常亮，数码管显示故障vescid
*/
	
/*
DJmotor-id号：
0号电机负责捡箭旋转
1号电机负责捡箭手爪旋转
2号负责摩擦轮餐盘
3号负责曲柄摇杆
4号负责摩擦轮抬升
5号负责转箭手柄的角度

VESCmotor-id号：
1号ELMO负责转箭

ELMOmotor-id号：
注意！ELMOidg和VESCid号不能设置成0
ELMOid1号对应我们的0号电机
VESCid1号对应我们的0号电机

id2号VESC负责摩擦轮左（ 即VESC【1】）
id3号VESC负责摩擦轮右（ 即VESC【2】）

*/

时钟（总线）频率
SYSCLK(Hz)                             | 168000000
AHB Prescaler                          | 1
HCLK(Hz)                               | 168000000
APB1 Prescaler                         | 4
PCLK1(Hz)                              | 42000000
APB2 Prescaler                         | 2
PCLK2(Hz)                              | 84000000
对于定时器部分，如果所在APB总线的分频系数为 1，那么就不倍频，如果不为 1（比如 2/4/8/16），那么就会 2 倍频后，作为定时器时钟输入