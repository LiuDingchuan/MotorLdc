#include "delay.h"

/** ∫¡√Î°¢Œ¢√Ó 
  **/
	
void Delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/**   —” ±°¢Œ¢√Î
  */
void Delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

