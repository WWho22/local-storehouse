#include "includes.h"

//串口2初始化
void USART2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART2_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//GPIO总线时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//串口总线时钟使能
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);//串口引脚复用定义
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
		
	GPIO_Init(GPIOA,&GPIO_InitStructure);//GPIO初始化
	USART_Init(USART2,&USART2_InitStructure);//串口2结构体初始化
	USART_Cmd(USART2,ENABLE);//串口2外设使能
	
	#if EN_USART2_R_IT
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	#endif
	
}

//串口4初始化
void UART4_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART4_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//GPIO总线时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//串口总线时钟使能
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);//串口引脚复用定义
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
	
	
	GPIO_Init(GPIOC,&GPIO_InitStructure);//GPIO初始化
	USART_Init(UART4,&USART4_InitStructure);//串口4结构体初始化
	USART_Cmd(UART4,ENABLE);//串口4外设使能
	
	#if EN_UART4_R_IT
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	#endif
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}

/*
 *参数1：要使用的串口指针 
 *参数2：要发送的字符
 *函数作用：指定串口发送一个字节数据
*/
void Usart_SendByte(USART_TypeDef* pUSARTx, char ch)
{
	USART_SendData(pUSARTx,ch);
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)!=SET)
	{
	}
}

/*
 *参数1：要使用的串口指针 
 *参数2：要发送的字符串
 *函数作用：指定串口发送字符串
*/
int Usart_SendString(USART_TypeDef* pUSARTx, const char* str)
{
	unsigned int str_count = 0;
	if(str == NULL)return FALSE;
	while(str[str_count]!='\0')
	{
		Usart_SendByte(pUSARTx,str[str_count++]);
	}
	return TRUE;
}


void USART2_IRQHandler(void)
{
	char res;
//	static int BufferCount = 0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET)
	{
		res = USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据，读取USART数据寄存器（USART_DR或通过USART_ReceiveData()函数），硬件会自动清除接收中断标志位（RXNE）
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
	static BaseType_t xHigherPriorityTaskWoken;
	xHigherPriorityTaskWoken = pdFALSE;
//	static int BufferCount = 0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!= RESET)
	{
		res = USART_ReceiveData(UART4);//(USART2->DR);	//读取接收到的数据，读取USART数据寄存器（USART_DR或通过USART_ReceiveData()函数），硬件会自动清除接收中断标志位（RXNE）
		RingBuffer_WriteByte(&test_RingBuffer,res);
		xSemaphoreGiveFromISR(Esp8266_ParseHandler,&xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}





//int fputc(int ch, FILE *f)
//{ 	
//	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
//	USART2->DR = (u8) ch;      
//	return ch;
//}

