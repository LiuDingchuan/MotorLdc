#include "usart2.h"

void USART2_Configuration()
{
  USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //接受中断

  //配置NVIC
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  DMA_DeInit(DMA1_Stream6);
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(usart.TxBuffer_USART2);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //以下为f4特有
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);

  DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
  DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
  DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE); //关闭了DMA传输完成中断，导致数据无法传输。10/14/2018

  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);

  //使能USART
  USART_Cmd(USART2, ENABLE);
/*寄存器写法（我实在看不懂）*/
//   RCC->APB1ENR |= 1<<17;             //使能USART2时钟
//   RCC->AHB1ENR |= 1<<21;             //使能DMA1时钟
//   GPIO_Set(GPIOA,PIN2|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
//   GPIO_AF_Set(GPIOA,2,7);            //复用PA2,PA3到USART2
//   GPIO_AF_Set(GPIOA,3,7);
//   USART2->BRR = (u32)0x016C;         //Bound rate set, 115200,72Mhz
//   USART2->CR1 = (u32)0x000C;         //USART2 setting
//   USART2->SR &= ~(1<<5);             //清除RXNE标志位
//   USART2->CR1 |= 1<<5;               //使能接收中断
//   MY_NVIC_Init(4,2,USART2_IRQn,3);
//   MYDMA_Config(DMA1_Stream6,4,(u32)&USART2->DR,(u32)usart.TxBuffer_USART2,USART2_Tx_BufferSize);
//   DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);//使能传输完成中断
//   MY_NVIC_Init(7,2,DMA1_Stream6_IRQn,3);
//   USART2->CR3 |= 1<<7;               //DMA1使能发送接收器
//   USART2->CR1 |= 1<<13;              //使能串口1



}

void DMA1_Stream6_IRQHandler(void) //数据传输完成,产生中断,检查是否还有没有传输的数据，继续传输
{
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET) 
  {
    DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    flag.Usart2DMASendFinish = 0;
  }
}

//串口屏
vu8 interface=0;
u8 USART2_RX_STA = 0;
static u8 ELMO_id=1;
static u8 VESC_id1,VESC_id2;
float DJ2006angle_gap;
float TakeArrowPosition;

void USART2_IRQHandler(void) 
{
  u8 temp = 0;
  if(USART_GetITStatus(USART2, USART_IT_ORE_RX) != RESET) 
  {
    USART_ReceiveData(USART2);
    USART2_RX_STA = 0;
    memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
  }
  else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) 
  {
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    temp = USART_ReceiveData(USART2);
    if ((USART2_RX_STA & 0x40) != 0) 
    {
      if ((USART2_RX_STA & 0x80) == 0)
      {
        if (temp == 0xff)  USART2_RX_STA |= 0x80;
        else 
        {
          usart.RxBuffer_USART2[USART2_RX_STA & 0X3F] = temp;
          USART2_RX_STA++;
          if ((USART2_RX_STA & 0X3F) > 40)  USART2_RX_STA = 0;
        }
      }
    }
    else if (temp == 0xee)   USART2_RX_STA |= 0x40;
    if (((USART2_RX_STA & 0x80) != 0))//串口屏指令判断
    {
		if(usart.RxBuffer_USART2[0]==0x12)//真实矩阵键盘
		{
			Beep_Show(1);
			switch (usart.RxBuffer_USART2[1])
			{
			case 0x00:
			flag.FlagTakeArrow=1;
				break;
			case 0x01:
			flag.FlagTakeArrow=3;
				break;
			case 0x02:
			flag.FlagTakeArrow=0;
				break;
			case 0x03:
				F4valveON
				break;
			case 0x04:
				F4valveOFF
				break;
			case 0x05:
				break;
			case 0x06:
				break;
			case 0x08:
				break;
			case 0x09:
				break;
			case 0x0A:
				break;
			case 0x0B:
				break;
			case 0x0C:
				break;
			case 0x0D:
				break;
			case 0x0E:
				break;
			case 0x0F:
				break;
			default:
				break;
			}
		}
      	if(usart.RxBuffer_USART2[1] == 0x01)
      	{
        	interface=usart.RxBuffer_USART2[3];
      	}
     	switch (usart.RxBuffer_USART2[3]) 
    	{
        case 0x00: //待机界面
        {
          switch (usart.RxBuffer_USART2[5])
          {
            case 0x05:
						RESET_PRO
            	break;
			case 0x09:
				flag.FlagMusic=1;
				break;
			case 0x0A:
				TakeArrowPosition=atof((char*)(&usart.RxBuffer_USART2[7]));
				Beep_Show(1);
				break;
            default:break;
          }
        }break;
        case 0x01: //DJ界面
        {
			switch (usart.RxBuffer_USART2[5])
			{
			case 0x1B:
				for(int i=0;i<8;i++) motor[i].enable=false;
			break;
			case 0x09:
				motor[0].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x0D:
				motor[0].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x11:
				motor[0].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x15:
				motor[0].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x01:
				motor[0].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x05:
				motor[0].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1C:
				motor[0].status.isSetZero=1;
			break;
			case 0x0A:
				motor[1].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x0E:
				motor[1].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x12:
				motor[1].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x16:
				motor[1].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x02:
				motor[1].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x06:
				motor[1].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1D:
				motor[1].status.isSetZero=1;
			break;
			case 0x0B:
				motor[2].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x0F:
				motor[2].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x13:
				motor[2].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x17:
				motor[2].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x03:
				motor[2].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x07:
				motor[2].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1E:
				motor[2].status.isSetZero=1;
			break;
			case 0x0C:
				motor[3].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x10:
				motor[3].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x14:
				motor[3].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x18:
				motor[3].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x04:
				motor[3].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x08:
				motor[3].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1F:
				motor[3].status.isSetZero=1;
			break;
            default:break;
			}
		} break;
				
        case 0x02: //DJ界面副
        {
			switch (usart.RxBuffer_USART2[5])
			{
			case 0x1B:
				for(int i=0;i<8;i++) motor[i].enable=false;
			break;
			case 0x09:
				motor[4].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x0D:
				motor[4].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x11:
				motor[4].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x15:
				motor[4].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x01:
				motor[4].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x05:
				motor[4].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1C:
				motor[4].status.isSetZero=1;
			break;
			case 0x0A:
				motor[5].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x0E:
				motor[5].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x12:
				motor[5].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x16:
				motor[5].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x02:
				motor[5].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x06:
				motor[5].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1D:
				motor[5].status.isSetZero=1;
			break;
			case 0x0B:
				motor[6].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x0F:
				motor[6].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x13:
				motor[6].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x17:
				motor[6].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x03:
				motor[6].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x07:
				motor[6].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1E:
				motor[6].status.isSetZero=1;
			break;
			case 0x0C:
				motor[7].mode=atof((char*)(&usart.RxBuffer_USART2[7])); 
			break;
			case 0x10:
				motor[7].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x14:
				motor[7].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x18:
				motor[7].limit.posSPlimit=atof((char*)(&usart.RxBuffer_USART2[7]));
			break;
			case 0x04:
				motor[7].enable=usart.RxBuffer_USART2[8];
			break;
			case 0x08:
				motor[7].begin=usart.RxBuffer_USART2[8];
			break;
			case 0x1F:
				motor[7].status.isSetZero=1;
			break;
			default:break;
			}
		} break;
				
        case 0x03://ELMO界面
        {
            switch(usart.RxBuffer_USART2[5])
            {
			case 0x12: ELMO_id=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
				break;
			case 0x02: MO(ELMO_id+1, SetData,1);
						MO(ELMO_id+1, GetData, 1);
				break;
			case 0x03: ELMOmotor[ELMO_id].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
				break;
			case 0x04: ELMOmotor[ELMO_id].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
				break;
			case 0x06: JV(ELMO_id+1,SetData);Beep_Show(1);
				break;
			case 0x07: PA(ELMO_id+1,SetData);Beep_Show(1);
				break;
			case 0x09: SP(ELMO_id+1,SetData);Beep_Show(1);
				break;
			case 0x0B: BG(ELMO_id+1);Beep_Show(1);
				break;
			case 0x0C: MO(ELMO_id+1,SetData,0);
				break;
			case 0x0D: ST(ELMO_id+1);
				break;
			case 0x0E: PX(ELMO_id+1,SetData);
				break;               
			case 0x0F: ELMO_PR(ELMO_id+1,SetData);
				break;
			default:break;
            }
        }break;
				
		case 0x04:
		{
			switch(usart.RxBuffer_USART2[5])
			{
			case 0x02: 
				VESCmotor[VESC_id1].enable=usart.RxBuffer_USART2[8];Beep_Show(1);
			break;
			case 0x03:
				VESCmotor[VESC_id2].enable=usart.RxBuffer_USART2[8];Beep_Show(1);
			break;
			case 0x04:
				VESCmotor[VESC_id1].begin=usart.RxBuffer_USART2[8];Beep_Show(1);
			break;
			case 0x05:
				VESCmotor[VESC_id2].begin=usart.RxBuffer_USART2[8];Beep_Show(1);
			break;
			case 0x06:
				VESC_id1=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x07:
				VESC_id2=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x08:
				VESCmotor[VESC_id1].mode=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x09:
				VESCmotor[VESC_id2].mode=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x0A:
				VESCmotor[VESC_id1].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x0B:
				VESCmotor[VESC_id2].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x0C:
				VESCmotor[VESC_id1].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x0D:
				VESCmotor[VESC_id2].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x0E:
				VESCmotor[VESC_id1].valueSet.duty=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x0F:
				VESCmotor[VESC_id2].valueSet.duty=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x10:
				VESCmotor[VESC_id1].valueSet.current=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x11:
				VESCmotor[VESC_id2].valueSet.current=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			break;
			case 0x12:
				flag.FlagShooting=1;
			break;
			case 0x13:
				DJ2006angle_gap=atof((char*)(&usart.RxBuffer_USART2[7])); Beep_Show(1);
			break;
			default:break;
			}
		}break;
        
        case 0x06://电磁阀
        {
			if(usart.RxBuffer_USART2[5]!=0)
           		CAN2_Valve(usart.RxBuffer_USART2[5],usart.RxBuffer_USART2[8]);
        }break;
        
        case 0x07://发射机构1
        {
            static int angle_for_picking;
			static int Time_Dly;
            switch(usart.RxBuffer_USART2[5])
            {
			case 0x02:
			{
				angle_for_picking=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			}break;
			case 0x03:
			{
				CAN2_Valve(1,1);
				CAN2_Valve(1,0);
			}break;
			case 0x04:
			{
									flag.FlagShooting=1;
			}break;
			case 0x05: //3508捡箭
			{
				CAN2_Valve(1,1); 
				motor[2].valueSet.angle=angle_for_picking;
				CAN2_Valve(1,0);
				motor[2].valueSet.angle=0;   
				CAN2_Valve(1,1); 
			}break; 
			case 0x06:
			{
				VESCmotor[2].valueSet.duty=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
				VESCmotor[1].valueSet.duty=-atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
			}break;
			case 0x07:
			{
				VESCmotor[2].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
				VESCmotor[1].valueSet.speed=-atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
//                    VESCmotor[1].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));Beep_Show(1);
//                    VESCmotor[2].valueSet.speed=-VESCmotor[1].valueSet.speed;Beep_Show(1);
			}break;
			default:break;
            }
        }break; 
									
        default:break;
      }     
      USART2_RX_STA = 0;
      memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));
    }
  }
}

static void USART2_Send(u8 count)
{
    if(!flag.Usart2DMASendFinish)//若上一次传输未完成，则舍弃本次传输
    {
      DMA_SetCurrDataCounter(DMA1_Stream6,count);
      DMA_Cmd(DMA1_Stream6, ENABLE);
      flag.Usart2DMASendFinish=1;
    }
}

char str_temp[32];
void UsartLCDshow(void)
{
    u8 i = 0;//用于串口数据包的下标

      /*****主界面****/
    usart.TxBuffer_USART2[i++]=0xee;
    usart.TxBuffer_USART2[i++]=0xb1;
    usart.TxBuffer_USART2[i++]=0x10;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x00;
    usart.TxBuffer_USART2[i++]=0x06;
    usart.TxBuffer_USART2[i++]=0x01;

    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xfc;
    usart.TxBuffer_USART2[i++]=0xff;
    usart.TxBuffer_USART2[i++]=0xff;//以上为判断蓝牙是否连接上的代码
  
    switch(interface)
    {
    case 0x01:/****DJ界面****/
    {
        usart.TxBuffer_USART2[i++]=0xee;
        usart.TxBuffer_USART2[i++]=0xb1;	
        usart.TxBuffer_USART2[i++]=0x12;	
        usart.TxBuffer_USART2[i++]=0x00;	
        usart.TxBuffer_USART2[i++]=0x01;
	
		if(motor[0].begin!=0)
		{	
			if(motor[0].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x09;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[0].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0D;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[0].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x11;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[0].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x15;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[0].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
						
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[0].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x05;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[0].begin;
			}
		}
		
		if(motor[1].begin!=false)
		{
			if(motor[1].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0A;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[1].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0E;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[1].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
			
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x12;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[1].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x16;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[1].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x02;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[1].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x06;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[1].begin;
			}
		}
		
		if(motor[2].begin!=0)
		{		
			if(motor[2].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0B;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[2].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0F;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[2].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);   
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x13;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[2].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x17;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[2].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x03;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[2].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x07;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[2].begin;
			}
		}
		
		if(motor[3].begin!=0)
		{		
			if(motor[3].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0C;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[3].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x10;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[3].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);  
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x14;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[3].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x18;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[3].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x04;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[3].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x08;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[3].begin;
			}
		}
	}break;
		
   case 0x02:
    {
        usart.TxBuffer_USART2[i++]=0xee;
        usart.TxBuffer_USART2[i++]=0xb1;	
        usart.TxBuffer_USART2[i++]=0x12;	
        usart.TxBuffer_USART2[i++]=0x00;	
        usart.TxBuffer_USART2[i++]=0x02;
    
		if(motor[4].begin!=0)
		{	
			if(motor[4].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x09;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[4].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0D;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[4].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x11;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[4].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x15;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[4].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
						
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[4].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x05;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[4].begin;
			}
		}
			
		if(motor[5].begin!=0)
		{
			if(motor[5].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0A;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[5].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0E;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[5].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
			
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x12;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[5].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x16;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[5].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x02;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[5].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x06;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[5].begin;
			}
		}
					
		if(motor[6].begin!=0)
		{	
			if(motor[6].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0B;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[6].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0F;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[6].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);   
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x13;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[6].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x17;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[6].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x03;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[6].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x07;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[6].begin;
			}
		}
		
		if(motor[7].begin!=false)
		{				
			if(motor[7].mode != 0)
			{
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x0C;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[7].mode);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);    
					
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x10;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[7].valueReal.speed);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);  
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x14;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%.1f",motor[7].valueReal.angle);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x18;
				usart.TxBuffer_USART2[i++]=0x00;
				sprintf(str_temp,"%d",motor[7].limit.posSPlimit);
				usart.TxBuffer_USART2[i++]=strlen(str_temp);
				strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
				i += strlen(str_temp);
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x04;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[7].enable;
				
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x08;
				usart.TxBuffer_USART2[i++]=0x00;
				usart.TxBuffer_USART2[i++]=0x01;
				usart.TxBuffer_USART2[i++]=motor[7].begin;
			}
		}

	 }break;
		case 0x03:
		{
			usart.TxBuffer_USART2[i++]=0xee;
			usart.TxBuffer_USART2[i++]=0xb1;	
			usart.TxBuffer_USART2[i++]=0x12;	
			usart.TxBuffer_USART2[i++]=0x00;	
			usart.TxBuffer_USART2[i++]=0x03;
			if(ELMOmotor[ELMO_id].mode!=0)
			{              
					usart.TxBuffer_USART2[i++]=0x00;
					usart.TxBuffer_USART2[i++]=0x10;
					usart.TxBuffer_USART2[i++]=0x00;
					sprintf(str_temp,"%d",ELMOmotor[ELMO_id].valueReal.speed);
					usart.TxBuffer_USART2[i++]=strlen(str_temp);
					strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
					i += strlen(str_temp);  
					
					usart.TxBuffer_USART2[i++]=0x00;
					usart.TxBuffer_USART2[i++]=0x11;
					usart.TxBuffer_USART2[i++]=0x00;
					sprintf(str_temp,"%d",ELMOmotor[ELMO_id].valueReal.angle);
					usart.TxBuffer_USART2[i++]=strlen(str_temp);
					strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
					i += strlen(str_temp);
				
					usart.TxBuffer_USART2[i++]=0x00;
					usart.TxBuffer_USART2[i++]=0x13;
					usart.TxBuffer_USART2[i++]=0x00;
					sprintf(str_temp,"%d",ELMOmotor[ELMO_id].valueReal.current);
					usart.TxBuffer_USART2[i++]=strlen(str_temp);
					strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
					i += strlen(str_temp);  
				}
		}break;
			
		case 0x04:
		{
			usart.TxBuffer_USART2[i++]=0xee;
			usart.TxBuffer_USART2[i++]=0xb1;	
			usart.TxBuffer_USART2[i++]=0x12;	
			usart.TxBuffer_USART2[i++]=0x00;	
			usart.TxBuffer_USART2[i++]=0x04;

			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x02;
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x01;
			usart.TxBuffer_USART2[i++]=VESCmotor[VESC_id1].enable;
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x03;
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x01;
			usart.TxBuffer_USART2[i++]=VESCmotor[VESC_id2].enable;
		
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x04;
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x01;
			usart.TxBuffer_USART2[i++]=VESCmotor[VESC_id1].enable;
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x05;
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x01;
			usart.TxBuffer_USART2[i++]=VESCmotor[VESC_id2].enable;
		
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x0A;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%d",VESCmotor[VESC_id1].valueReal.speed);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x0B;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%d",VESCmotor[VESC_id2].valueReal.speed);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x0C;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%.2lf",VESCmotor[VESC_id1].valueReal.angle);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x0D;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%.2lf",VESCmotor[VESC_id2].valueReal.angle);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp); 

			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x0E;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%.2lf",VESCmotor[VESC_id1].valueReal.duty);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x0F;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%.2lf",VESCmotor[VESC_id2].valueReal.duty);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x10;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%.2lf",VESCmotor[VESC_id1].valueReal.current);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  
			
			usart.TxBuffer_USART2[i++]=0x00;
			usart.TxBuffer_USART2[i++]=0x11;
			usart.TxBuffer_USART2[i++]=0x00;
			sprintf(str_temp,"%.2lf",VESCmotor[VESC_id2].valueReal.current);
			usart.TxBuffer_USART2[i++]=strlen(str_temp);
			strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
			i += strlen(str_temp);  	
	}break;
		
    default:break;
  }
   USART2_Send(i);
}









