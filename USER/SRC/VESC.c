#include "vesc.h"
//FIXME:位置模式
VESCParam U10instrin,SunnySkyinstrin,U8instrin,U5instrin;
VESCLimit VESClimit;
VESCArgum VESCargum;
VESC_Motor VESCmotor[5];

static float angle_gap=90.f;

void VESCInit(void)
{
	{ //电机内参
		U10instrin.POLE_PAIRS = 21;
		U10instrin.MAX_CURRENT=60.0f;
		
		SunnySkyinstrin.POLE_PAIRS=7;
		SunnySkyinstrin.MAX_CURRENT=60.0f;
		
		U8instrin.POLE_PAIRS = 21;
		U8instrin.MAX_CURRENT=60.0f;
		
		U5instrin.POLE_PAIRS=7;
		U5instrin.MAX_CURRENT=60.f;
	}
	{
		//电机限制保护
    VESClimit.breakCurrent=10;
		VESClimit.timeoutMotion=1;
	} 
	{								   //间值参数设置
		VESCargum.timeoutTicks = 1000; //1000ms(1s)
		VESCargum.firstPos=true;
	}
	/****0号电机初始化****/
	VESCmotor[0].instrinsic = SunnySkyinstrin;
	VESCmotor[0].enable = false;
	VESCmotor[0].begin = true;
	VESCmotor[0].mode = RPM;
	PID_Init(&VESCmotor[0].PIDx,3.84,0.64,0.54,1,VESCmotor[0].valueSet.position);
	PID_Init(&VESCmotor[0].PIDs,0.007,0.001,1,1,VESCmotor[0].valueSet.speed);
	VESCmotor[0].valueSet.current = 0.0f;
	VESCmotor[0].valueSet.speed = 0;
	VESCmotor[0].valueSet.position = 0;
	VESCmotor[0].valueSet.duty = 0.0f;

	/****1号电机初始化****/
	VESCmotor[1].instrinsic = U5instrin;
	VESCmotor[1].enable = false;
	VESCmotor[1].begin = true;
	VESCmotor[1].mode = RPM;
	VESCmotor[1].valueSet.current = 0.0f;
	VESCmotor[1].valueSet.speed = 0;
	VESCmotor[1].valueSet.position = 0;
	VESCmotor[1].valueSet.duty = 0.0f;

	/****2号电机初始化****/
	VESCmotor[2].instrinsic = U5instrin;
	VESCmotor[2].enable = false;
	VESCmotor[2].begin = true;
	VESCmotor[2].mode = RPM;
	VESCmotor[2].valueSet.current = 0.0f;
	VESCmotor[2].valueSet.speed = 0;
	VESCmotor[2].valueSet.position = 0;
	VESCmotor[2].valueSet.duty = 0.0f;

	/****3号电机初始化****/
	VESCmotor[3].instrinsic = U8instrin;
	VESCmotor[3].enable = false;
	VESCmotor[3].begin = true;
	VESCmotor[3].mode = RPM;
	VESCmotor[3].valueSet.current = 0.0f;
	VESCmotor[3].valueSet.speed = 0;
	VESCmotor[3].valueSet.position = 0;
	VESCmotor[3].valueSet.duty = 0.0f;

	for (int i = 0; i < 4; i++)
	{
		VESCmotor[i].limit = VESClimit;
		VESCmotor[i].argum = VESCargum;
	}
}

void VESC_caculate(VESC_Motor* motor)
{
	if(motor->status.isSetZero)
	{
		motor->status.isSetZero=false;
		motor->valueReal.position=0;
	}
}

//位置模式--通过速度模式实现——存在问题：可以控制速度，但是到后半程速度恢复得很慢
void VESC_position_mode_rpm(u8 id)
{
	VESCmotor[id].PIDx.SetValue=VESCmotor[id].valueSet.position;
	if(!VESCmotor[id].begin)
	  VESCmotor[id].PIDx.SetValue=VESCmotor[id].argum.lockPosition;
	if(VESCmotor[id].limit.isPosLimitOn)
		PEAK(motor->PIDx.SetValue,VESCmotor[id].limit.maxPosition);
	VESCmotor[id].PIDx.CurValue=VESCmotor[id].valueReal.position;
	PID_Operation(&VESCmotor[id].PIDx);
	VESCmotor[id].PIDx.Udlt=IIR_filter(&IIR_flt, VESCmotor[id].PIDx.Udlt);
	if(motor->limit.isPosSPLimitOn)
		PEAK(VESCmotor[id].PIDx.Udlt,motor->limit.posSPlimit);
	VESC_Set_Speed(id+1,VESCmotor[id].PIDx.Udlt*VESCmotor[id].instrinsic.POLE_PAIRS,1);
}

//位置模式--通过VESC自带位置模式实现——存在问题：速度仍不能很好地控制
void VESC_position_mode_pos(u8 id)
{
	//反转最大允许15000转速
	if(SIG(VESCmotor[id].argum.difPosition)==SIG(VESCmotor[id].valueReal.speed))//判断是否当前速度方向与位置误差同向
		angle_gap=170.f;//锟锟找出的最合适分度角
	else
		angle_gap=90.f;
	if(ABS(VESCmotor[id].argum.difPosition)>(angle_gap*10))
		VESCmotor[id].valueSet.angle=VESCmotor[id].valueReal.angle+SIG(VESCmotor[id].argum.difPosition)*angle_gap;
	else
	{
		float difangle=VESCmotor[id].argum.difPosition/10.f;
		VESCmotor[id].valueSet.angle=VESCmotor[id].valueReal.angle+difangle;
	}
	utils_norm_angle(&VESCmotor[id].valueSet.angle);
	VESC_Set_Position(id+1,VESCmotor[id].valueSet.angle,1);
}
/** 
  * @brief 设定VESC占空比
  */
void VESC_Set_Duty_Cycle(u8 controller_ID, float duty_cycle, u8 InConGrpFlag)
{
	int32_t send_index = 0;
	if (VESCRear == VESC_Sendqueue.Front)
	{
		flag.VESCSendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (int32_t)(duty_cycle * 100000), &send_index);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_DUTY << 8);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;
	}
		VESC_Sendqueue.Rear = VESCRear;
}

/** 
	* @brief 设定VESC转速
	*/
void VESC_Set_Speed(u8 controller_ID, s32 speed, u8 InConGrpFlag)
{
	int32_t send_index = 0;
	if (VESCRear == VESC_Sendqueue.Front)
	{
		flag.VESCSendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, speed, &send_index);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_RPM << 8);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;
	}
	VESC_Sendqueue.Rear = VESCRear;
}

/** 
	* @brief 设定VESC电流
	*/
void VESC_Set_Current(u8 controller_ID, float current, u8 InConGrpFlag)
{
	int32_t send_index = 0;
	if (VESCRear == VESC_Sendqueue.Front)
	{
		flag.VESCSendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (int32_t)(current * 1000), &send_index);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;
	}
	VESC_Sendqueue.Rear = VESCRear;
}

/** 
	* @brief 设定VESC刹车电流
	*/
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 InConGrpFlag)
{
	int32_t send_index = 0;
	if (VESCRear == VESC_Sendqueue.Front)
	{
		flag.VESCSendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (int32_t)(brake_current * 1000), &send_index);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;
	}
	VESC_Sendqueue.Rear = VESCRear;
}

/** 
	* @brief 设定VESC锁电机电流
	*/
void VESC_Set_Handbrake_Current(u8 controller_ID, float handbrake_current, u8 InConGrpFlag)
{
	int32_t send_index = 0;
	if (VESCRear == VESC_Sendqueue.Front)
	{
		flag.VESCSendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (int32_t)(handbrake_current * 1000), &send_index);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;
	}
	VESC_Sendqueue.Rear = VESCRear;
}

void VESC_Set_Position(u8 controller_ID,float angle, u8 InConGrpFlag)
{
	int32_t send_index = 0;
	if (VESCRear == VESC_Sendqueue.Front)
	{
		flag.VESCSendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].Data, (int32_t)(angle*1e6f), &send_index);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_POS << 8);
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].DLC = send_index;
		VESC_Sendqueue.Can_DataSend[VESC_Sendqueue.Rear].InConGrpFlag = InConGrpFlag;
	}
	VESC_Sendqueue.Rear = VESCRear;
}








