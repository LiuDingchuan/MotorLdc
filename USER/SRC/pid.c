#include "pid.h"

PID_setTypeDef SunnySkyPID, U10PID, DJ3508PID, DJ2006PID;

void PID_Init(PID_setTypeDef* PID,float KP,float KI,float KD,float KS,s32 Set)
{
  	PID->CurValue=0;
	PID->SetValue=Set;
	PID->liKkValue[0]=0;
	PID->liKkValue[1]=0;
  	PID->liKkValue[2]=0;
	PID->KP=KP;
	PID->KI=KI;
	PID->KD=KD;
	PID->KS=KS;
}

void PID_Operation(PID_setTypeDef* PID)
{
	PID->liKkValue[0]=PID->SetValue-PID->CurValue;
	PID->Udlt=PID->KP*(PID->liKkValue[0]-PID->liKkValue[1])+PID->KI*PID->liKkValue[0]+PID->KD*(PID->liKkValue[0]-2*PID->liKkValue[1]+PID->liKkValue[2]);
	PID->liKkValue[2]=PID->liKkValue[1];
	PID->liKkValue[1]=PID->liKkValue[0];
}
