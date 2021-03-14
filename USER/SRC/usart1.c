//ʾ����(�洫���룬ֱ��copy����)
#include "usart1.h"

void USART1_Configuration(void)
{
    RCC->APB2ENR |= 1<<4;              //ʹ��USART1ʱ��
    RCC->AHB1ENR |= 1<<22;             //ʹ��DMA2ʱ��
    GPIO_Set(GPIOA,PIN9|PIN10,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);
    GPIO_AF_Set(GPIOA,9,7);            //����PA9,PA10��USART1
    GPIO_AF_Set(GPIOA,10,7);
    USART1->BRR = (u32)0x02D9;         //����������,115200
    USART1->CR1 = (u32)0x020C;         //USART1 setting, ʹ�ܽ����ж�
    USART1->SR &= ~(1<<5);             //���RXNE��־λ
    MY_NVIC_Init(6,1,USART1_IRQn,3);
    MYDMA_Config(DMA2_Stream7,4,(u32)&USART1->DR,(u32)usart.TxBuffer_USART1,USART1_Tx_BufferSize);
    DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);//ʹ�ܴ�������ж�
    MY_NVIC_Init(7,0,DMA2_Stream7_IRQn,3);
    USART1->CR3 |= 1<<7;               //DMA2ʹ�ܷ��ͽ�����
    USART1->CR1 |= 1<<13;              //ʹ�ܴ���1
}

void USART1_Send(INT8U *data,INT8U count)
{
	if(!flag.Usart1DMASendFinish)//����һ�δ���δ��ɣ����������δ���
	{
		memcpy(usart.TxBuffer_USART1,data,count);
		DMA_SetCurrDataCounter(DMA2_Stream7,count);
		DMA_Cmd(DMA2_Stream7, ENABLE);
		flag.Usart1DMASendFinish=1;
	}
}

void DMA2_Stream7_IRQHandler(void)//���ݴ�����ɣ������жϣ�����Ƿ���û�д�������ݣ���������
{
	if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7)==SET)
	{
		DMA_ClearFlag (DMA2_Stream7,DMA_IT_TCIF7);//����жϱ�־
		DMA_ClearITPendingBit(DMA2_Stream7,DMA_IT_TCIF7);
		flag.Usart1DMASendFinish=0;
	}
}
