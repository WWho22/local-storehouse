#include "includes.h"

//����2��ʼ��
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART2_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIO����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//��������ʱ��ʹ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//�������Ÿ��ö���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
	
	USART2_InitStructure.USART_BaudRate = 115200;
	USART2_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART2_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART2_InitStructure.USART_Parity = USART_Parity_No;
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
		
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIO��ʼ��
	USART_Init(USART2,&USART2_InitStructure);//����2�ṹ���ʼ��
	USART_Cmd(USART2,ENABLE);//����2����ʹ��
	
	#if EN_USART2_R_IT
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	#endif
	
}

//����4��ʼ��
void UART4_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART4_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//GPIO����ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//��������ʱ��ʹ��
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);//�������Ÿ��ö���
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	USART4_InitStructure.USART_BaudRate = 115200;
	USART4_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
	USART4_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART4_InitStructure.USART_Parity = USART_Parity_No;
	USART4_InitStructure.USART_StopBits = USART_StopBits_1;
	USART4_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);//GPIO��ʼ��
	USART_Init(UART4,&USART4_InitStructure);//����4�ṹ���ʼ��
	USART_Cmd(UART4,ENABLE);//����4����ʹ��
	
	#if EN_UART4_R_IT
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);//�жϳ�ʼ��
	#endif
}


int Usart_SendByte(USART_TypeDef* pUSARTx, char ch ,int timeout)
{
	int T_count = 0;
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)!=SET)
	{
		T_count++;
		if(T_count>=timeout)return T_count;
	}
	return T_count;
}

int Usart_SendString(USART_TypeDef* pUSARTx, char* str,int timeout)
{
	int res;
	unsigned int str_count = 0;
	while(str[str_count]!='\0')
	{
		res += Usart_SendByte(pUSARTx,str[str_count],timeout);
		str_count++;
		if(res>=timeout) return TIMEOUT;
	}
	return OK;
}

void USART2_IRQHandler(void)
{
	char res;
//	static int BufferCount = 0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
		res = USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ������ݣ���ȡUSART���ݼĴ�����USART_DR��ͨ��USART_ReceiveData()��������Ӳ�����Զ���������жϱ�־λ��RXNE��
		RingBuffer_WriteByte(&test_RingBuffer,res);
//		USART_SendData(USART2,res);
//		if(BufferCount<=(USART4_REC_LEN-1))
//		{
//			if(res!='\0')
//			{
//				usart4_buffer[BufferCount] = res;
//				BufferCount++;
//			}
//			else 
//			{
//				usart4_buffer[BufferCount] = res;
//				BufferCount = 0;
//			}
//		}
//		else
//		{
//			BufferCount = 0;
//		}
	}
}

void UART4_IRQHandler(void)
{
	char res;
	static char last_char;
	static BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
//	static int BufferCount = 0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!= RESET)
	{
		res = USART_ReceiveData(UART4);//(USART2->DR);	//��ȡ���յ������ݣ���ȡUSART���ݼĴ�����USART_DR��ͨ��USART_ReceiveData()��������Ӳ�����Զ���������жϱ�־λ��RXNE��
		RingBuffer_WriteByte(&test_RingBuffer,res);
		if((res == '\n')&&(last_char == '\r'))
		{
			xSemaphoreGiveFromISR(Esp8266_ParseHandler,&xHigherPriorityTaskWoken);
		}
		last_char = res;
     }
}


//int fputc(int ch, FILE *f)
//{ 	
//	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
//	USART2->DR = (u8) ch;      
//	return ch;
//}

