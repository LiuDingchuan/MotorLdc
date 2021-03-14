#include "motor.h"

MotorParam M3508instrin,M2006instrin;
MotorLimit Motorlimit;
MotorArgum Motorargum;
DJmotor motor[8];

void Motor_Init(void)
{
	{//电机内参	 
		M3508instrin.PULSE=8192;
		M3508instrin.RATIO=3591.f/187.f;
		M3508instrin.CURRENT_LIMIT=14745;
		M3508instrin.GearRatio=1.0f;
		
		M2006instrin.PULSE=8192;
		M2006instrin.RATIO=36.f;
		M2006instrin.CURRENT_LIMIT=9000;
		M2006instrin.GearRatio=1.0f;
	}
	{//电机限制保护设置
		// Motorlimit.maxAngle= 185*M3508instrin.GearRatio;//最多旋转正负0.5圈多一点点
		Motorlimit.isPosSPLimitOn=true;
		Motorlimit.isRealseWhenStuck=true;
		Motorlimit.isPosLimitON=false;
		Motorlimit.posSPlimit=500;
		Motorlimit.zeroSP=500;
		Motorlimit.zeroCurrent=2000;
		Motorlimit.stuckmotion=true;
		Motorlimit.timeoutMotion=true;
	}    
	{
		//电机其他参数设置		
		Motorargum.lastRxTim=0;
		Motorargum.timeoutTicks =50;//50ms
		Motorargum.stuckTicks=200;//200ms
	}
	
	for(int id=0;id<8;id++)
	{
		motor[id].argum=Motorargum;
		motor[id].limit=Motorlimit;
	}
		
	 //0号电机  
	motor[0].intrinsic=M3508instrin;
	motor[0].enable=DISABLE;
	motor[0].begin=true;
	motor[0].mode=position;
	motor[0].valueSet.angle=0;
	motor[0].valueSet.speed=100;
	motor[0].valueSet.current=100;
	PID_Init(&motor[0].PIDx,5,0.2,0,0.4,motor[0].valueSet.pulse);
	PID_Init(&motor[0].PIDs,8,0.3,0,1,motor[0].valueSet.speed);
	
	//1号电机
	motor[1].intrinsic=M2006instrin;
	motor[1].enable=DISABLE;
	motor[1].begin=true;
	motor[1].mode=position;
	motor[1].valueSet.angle=0;
	motor[1].valueSet.speed=100;
	motor[1].valueSet.current=100;
	PID_Init(&motor[1].PIDx,5,0.2,0,0.4,motor[1].valueSet.pulse);
	PID_Init(&motor[1].PIDs,8,0.3,0,1,motor[1].valueSet.speed);
	
	//2号电机
	motor[2].intrinsic=M3508instrin;
	motor[2].enable=ENABLE;
	motor[2].begin=true;
	motor[2].mode=position;
	motor[2].valueSet.angle=0;
	motor[2].valueSet.speed=100;
	motor[2].valueSet.current=50;
	motor[2].intrinsic.GearRatio=0.25f;
	motor[2].limit.isPosLimitON=false;
	motor[2].limit.maxAngle=40;
	motor[2].limit.posSPlimit=1500;
	PID_Init(&motor[2].PIDx,5,0.2,0,0.4,motor[2].valueSet.pulse);
	PID_Init(&motor[2].PIDs,8,0.3,0,1,motor[2].valueSet.speed);
	
		//3号电机
	motor[3].intrinsic=M2006instrin;
	motor[3].enable=DISABLE;
	motor[3].begin=true;
	motor[3].mode=position;
	motor[3].valueSet.angle=0;
	motor[3].valueSet.speed=100;
	motor[3].valueSet.current=50;
	motor[3].limit.posSPlimit=15000;
	PID_Init(&motor[3].PIDx,5,0.2,0,0.4,motor[3].valueSet.pulse);
	PID_Init(&motor[3].PIDs,8,0.3,0,1,motor[3].valueSet.speed);
	
		//4号电机
	motor[4].intrinsic=M3508instrin;
	motor[4].enable=ENABLE;
	motor[4].begin=true;
	motor[4].mode=position;
	motor[4].valueSet.angle=0;
	motor[4].valueSet.speed=100;
	motor[4].valueSet.current=50;
	motor[4].intrinsic.GearRatio=17.f/120.f;
	motor[4].limit.isPosLimitON=true;
	motor[4].limit.maxAngle=39;
	motor[4].limit.posSPlimit=1500;
	PID_Init(&motor[4].PIDx,5,0.2,0,0.4,motor[4].valueSet.pulse);
	PID_Init(&motor[4].PIDs,8,0.3,0,1,motor[4].valueSet.speed);
	
		//5号电机
	motor[5].intrinsic=M3508instrin;
	motor[5].enable=DISABLE;
	motor[5].begin=true;
	motor[5].mode=position;
	motor[5].valueSet.angle=0;
	motor[5].valueSet.speed=100;
	motor[5].valueSet.current=50;
	motor[5].limit.isPosLimitON=true;
	motor[5].limit.maxAngle=120;
	motor[5].intrinsic.GearRatio=0.5f;
	motor[5].limit.posSPlimit=1100;
	PID_Init(&motor[5].PIDx,5,0.2,0,0.4,motor[5].valueSet.pulse);
	PID_Init(&motor[5].PIDs,8,0.3,0,1,motor[5].valueSet.speed);
	
		//6号电机
	motor[6].intrinsic=M3508instrin;
	motor[6].enable=DISABLE;
	motor[6].begin=true;
	motor[6].mode=position;
	motor[6].valueSet.angle=0;
	motor[6].valueSet.speed=100;
	motor[6].valueSet.current=50;
	PID_Init(&motor[6].PIDx,5,0.2,0,0.4,motor[6].valueSet.pulse);
	PID_Init(&motor[6].PIDs,8,0.3,0,1,motor[6].valueSet.speed);
	
	 //7号电机 
	motor[7].intrinsic=M3508instrin;
	motor[7].enable=DISABLE;
	motor[7].begin=true;
	motor[7].mode=position;
	motor[7].valueSet.angle=0;
	motor[7].valueSet.speed=100;
	motor[7].valueSet.current=50;
	PID_Init(&motor[7].PIDx,5,0.2,0,0.4,motor[7].valueSet.pulse);
	PID_Init(&motor[7].PIDs,8,0.3,0,1,motor[7].valueSet.speed);
	
	for(int id=0;id<8;id++)
	{
		motor[id].argum.maxPulse=motor[id].limit.maxAngle/motor[id].intrinsic.GearRatio*motor[id].intrinsic.RATIO*motor[id].intrinsic.PULSE/360.f;
		motor[id].valueSet.pulse=motor[id].valueSet.angle/motor[id].intrinsic.GearRatio*motor[id].intrinsic.RATIO*motor[id].intrinsic.PULSE/360.f;
	}

}

static void setZero(DJmotor* motor)
{
	motor->status.isSetZero=false;
	motor->valueReal.pulse=0;
	motor->valueReal.angle=0;
	motor->argum.distance=0;
}

void speed_mode(s16 id)
{
	motor[id].PIDs.SetValue=motor[id].PIDs.KS*motor[id].valueSet.speed;
	motor[id].PIDs.CurValue=motor[id].valueReal.speed;
	PID_Operation(&motor[id].PIDs);
	motor[id].valueSet.current+=motor[id].PIDs.Udlt;
}

void position_mode(s16 id)
{
	if(motor[id].intrinsic.GearRatio!=1.0f)
		motor[id].valueSet.pulse=motor[id].valueSet.angle/motor[id].intrinsic.GearRatio*motor[id].intrinsic.RATIO*motor[id].intrinsic.PULSE/360.f;
	else
		motor[id].valueSet.pulse=motor[id].valueSet.angle*motor[id].intrinsic.RATIO*motor[id].intrinsic.PULSE/360.f;	
	motor[id].PIDx.SetValue=motor[id].valueSet.pulse;
	if(!motor[id].begin) 
		motor[id].PIDx.SetValue = motor[id].argum.lockPulse;//如果为锁电机状态
	if(motor[id].limit.isPosLimitON) 
		PEAK(motor[id].PIDx.SetValue,motor[id].argum.maxPulse);
	motor[id].PIDx.CurValue=motor[id].valueReal.pulse;
	PID_Operation(&motor[id].PIDx);
	motor[id].PIDs.SetValue=motor[id].PIDx.KS*motor[id].PIDx.Udlt;
	if(motor[id].limit.isPosSPLimitOn) 
		PEAK(motor[id].PIDs.SetValue,motor[id].limit.posSPlimit);
	motor[id].PIDs.CurValue=motor[id].valueReal.speed;
	PID_Operation(&motor[id].PIDs);
	motor[id].valueSet.current+=motor[id].PIDs.Udlt;
	if(ABS(motor[id].argum.difPulseSet)<60) 
		motor[id].status.arrived=true;//到达指定位置
	else 
		motor[id].status.arrived=false;
}

/****寻零模式****/
void zero_mode(s16 id)
{
    motor[id].PIDs.SetValue=motor[id].limit.zeroSP;
    motor[id].PIDs.CurValue=motor[id].valueReal.speed;
    PID_Operation(&motor[id].PIDs);
    motor[id].valueSet.current+=motor[id].PIDs.Udlt;
    PEAK(motor[id].valueSet.current,Motorlimit.zeroCurrent);
    if(ABS(motor[id].argum.distance)<50) 
		motor[id].argum.zeroCnt++;
    else    
		motor[id].argum.zeroCnt=0;
    if(motor[id].argum.zeroCnt>250)//寻零完成，初始化零点，转换为位置模式锁零点
    {
        motor[id].argum.zeroCnt=0; 
	    motor[id].valueReal.pulse=0;//这个很重要
        motor[id].mode=position; 
		motor[id].status.zero=1;
    }
}

void pulse_calculate(u8 id)
{
	motor[id].argum.distance=motor[id].valueReal.pulseRead-motor[id].valuePrv.pulseRead;
	motor[id].valuePrv=motor[id].valueReal;
	if(ABS(motor[id].argum.distance)>4085)
		motor[id].argum.distance-=SIG(motor[id].argum.distance)*motor[id].intrinsic.PULSE;//电调返回的值在0-8192之间，当数值突变时，代表圈数增加，需要通过SIG函数加一圈pulse值
	motor[id].valueReal.pulse+=motor[id].argum.distance;//累计脉冲计算
	motor[id].argum.difPulseSet=motor[id].valueSet.pulse-motor[id].valueReal.pulse;//更新误差，判断电机状态是否为arrived
	motor[id].valueReal.angle=motor[id].valueReal.pulse*360.f/motor[id].intrinsic.RATIO/motor[id].intrinsic.PULSE;

	/*判断是否堵转*/
	if(ifstuck(id)&&motor[id].limit.isRealseWhenStuck)
	{
		motor[id].enable=DISABLE;
	}
	/* 锁点位置记录 */
	if(motor[id].begin) motor[id].argum.lockPulse=motor[id].valueReal.pulse;
	/* 判断是否需要重置零点 */
	if(motor[id].status.isSetZero) setZero(&motor[id]);

}

u8 ifstuck(u16 id)
{
	if(motor[id].enable==DISABLE)
		return 0;
	if(motor[id].limit.stuckmotion)
	{
		if(motor[id].mode==RPM)
		{
			if(ABS(motor[id].valueReal.speed)>=0&&ABS(motor[id].valueReal.speed)<100)//电机速度小于阈值
			{
				motor[id].argum.stuckCnt++;
			}
			else
			{
				motor[id].argum.stuckCnt=0;
			}
		  if(motor[id].argum.stuckCnt>motor[id].argum.stuckTicks)
			{
				motor[id].status.stuck=1;
				insertError(Error.head,DJERROR|(id<<4)|STUCK);
			}
			else
			{
				motor[id].status.stuck=0;
				deleteError(Error.head,ErrorFind(Error.head,DJERROR|(id<<4)|STUCK));
			}
			if(&motor[id].intrinsic==&M3508instrin)
			{
				if(motor[id].status.stuck==1&&motor[id].valueReal.temperature>=40)
				{
					BEEP_ON;//一直报警
					return 1;
				}
			}
			else if(&motor[id].intrinsic==&M2006instrin)//C610电调不返回温度信息
			{
				if(motor[id].status.stuck==1)
				{
					BEEP_ON;
					return 1;
				}
			}			
		}
		
		else if(motor[id].mode==position)
		{
			if(ABS(motor[id].valueReal.pulse-motor[id].valuePrv.pulse<50))
			{
				if(ABS(motor[id].PIDs.CurValue)<100&&motor[id].status.arrived==false)//电机转速小于阈值并且没有到达位置
				{
					motor[id].argum.stuckCnt++;
				}
				else
				{
					motor[id].argum.stuckCnt=0;
				}
				if(motor[id].argum.stuckCnt>motor[id].argum.stuckTicks)
				{
					motor[id].status.stuck=1;
				}
				else
				{
					motor[id].status.stuck=0;
				}
				if(&motor[id].intrinsic==&M3508instrin)
				{
					if(motor[id].status.stuck==1&&motor[id].valueReal.temperature>=40)
					{
						BEEP_ON;
						return 1;
					}
				}			
				else if(&motor[id].intrinsic==&M2006instrin)
				{
					if(motor[id].status.stuck==1)
					{
						BEEP_ON;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

	
	
	
	
	
	
	
	
	






























