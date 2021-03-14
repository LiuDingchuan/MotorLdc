#ifndef _MACRO_H_
#define _MACRO_H_
//��ָ��

#include "stdbool.h"
#include "stm32f4xx.h"

#define USE_VISUALSCOPE
//#define PICK_UP_ARROWS
#define Shooting
// #define Defending
// #define USE_ELMO
// #define USE_DJ
// #define USE_VESC
// #define USE_F4VALVE

#ifdef PICK_UP_ARROWS
  #define USE_DJ
#endif

#ifdef Shooting
	#define USE_VESC
	#define USE_DJ
	#define USE_F4VALVE
#endif

#ifdef Defending
	#define USE_DJ
	#define USE_ELMO
#endif

// /* BIT��ţ��������̺��ж� ��δ֪���ܣ�*/
// #define BIT0 (0x01<<0)
// #define BIT1 (0x01<<1)
// #define BIT2 (0x01<<2)
// #define BIT3 (0x01<<3)
// #define BIT4 (0x01<<4)
// #define BIT5 (0x01<<5)
// #define BIT6 (0x01<<6)
// #define BIT7 (0x01<<7)

/* math function */
#define ABS(x)  ((x)>0? (x):(-(x)))
#define SIG(x)  ((x)>=0? 1:-1)
#define PEAK(A,B)   if(ABS(A)>B) A=SIG(A)*B; //�Ƚ�A��B�Ĵ�С����ѡȡ��С��ֵ��sig��֤��С��ֵ������ϴ��ֵһ�£�

#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}
#define EncodeS16Data(f,buff)	{*(s16*)buff = *f;}
#define DecodeS16Data(f,buff)	{*f = *(s16*)buff;}
#define EncodeU16Data(f,buff)	{*(u16*)buff = *f;}
#define DecodeU16Data(f,buff)	{*f = *(u16*)buff;}
#define EncodeS32Data(f,buff)	{*(s32*)buff = *f;}
#define DecodeS32Data(f,buff)	{*f = *(s32 *)buff;}
void EncodeFloatDataLost(float* f, unsigned char* buff);
void DecodeFloatDataLost(float* f, unsigned char* buff);

#endif
