#include "param.h"

FlagTypeDef flag;
ErrorTypeDef Error;
Usart_Struct usart;

void param_Init(void)
{
	#ifdef USE_DJ
	Motor_Init();
	#endif
	
	#ifdef USE_ELMO
	ELMO_Motor_Init();
	ELMO_Init(CAN2);
	#endif
	
	#ifdef USE_VESC
	VESCInit();
	#endif
	
	#ifdef USE_F4VALVE
	Valve_Configuration();
	#endif
	
	Error.head=creatError();
	Beep_Show(1);
}

ErrorList* creatError()
{
	ErrorList* head;
	head = (ErrorList*)malloc(sizeof(ErrorList));
	head->next=NULL;
	return head;
}

void deleteError(ErrorList* list, ErrorList* listdel)
{
	ErrorList* tpre = list;
	while(tpre!=NULL&&(tpre->next)!=listdel)
	{
		tpre=tpre->next;
	}
	if(listdel!=NULL)
	{
		tpre->next=listdel->next;
		free(listdel);
		Error.size--;
	}
}

void insertError(ErrorList* list,uc16 errorcode)
{
	ErrorList* t=list, *in;
	if(ErrorFind(t, errorcode)!=NULL)
		return;
	u8 i=0;
  while(i<Error.size&&t!=NULL)
	{
		t=t->next;
		i++;
	}		
	if(t!=NULL)
	{
		in=(ErrorList*)malloc(sizeof(ErrorList));
		in->code=errorcode;
		in->next=t->next;
		t->next=in;
		Error.size++;
	}
}

ErrorList* ErrorFind(ErrorList* list, uc16 errorcode)
{
	ErrorList* t=list;
	while(t!=NULL)
	{
		if(t->code==errorcode)
		{
			return t;
		}
		else
			t=t->next;
	}
	return NULL;
}


/*改变16位数据高低顺序*/
void ChangeData(u8* buff2,u8* buff1)
{
	u8 temp=0;
  temp= *buff1;
	*buff1 = *buff2;	 
	*buff2 = temp;
}//电调返回报文时，基本上都是按照高8位在前、低8位在后发送的，但是我们在指针类型转换时（不改变内存里的内容，用不同的方式解读同一片内存的内容），由于stm32芯片是小端序的，是将低8位在前、高8位在后进行解读的。
//如0x12345678，进行小端序指针类型转换时，解读出的结果就是0x78563412

void utils_norm_angle(float *angle)
{
	*angle = fmodf(*angle, 360.0f);

	if (*angle < 0.0f) {
		*angle += 360.0f;
	}
}

void EncodeFloatDataLost(float* f, unsigned char* buff)
{
	buff[0] = (u8)(((s16)*f)&0x00ff);
	buff[1] = (u8)(((s16)*f)>>8&0x00ff);
}

void DecodeFloatDataLost(float* f, unsigned char* buff)
{
	*f=(double)((s16)((u16)(buff[1]&0x00ff)<<8)|buff[0]);
}








