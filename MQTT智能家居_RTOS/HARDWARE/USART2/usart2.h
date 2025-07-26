#ifndef __USART2_H__
#define __USART2_H__
#include "sys.h"
#include "stm32f4xx.h"                  // Device header
#include "stdio.h"

#define EN_UART4_R_IT    1  //ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART2_R_IT    1  //ʹ�ܣ�1��/��ֹ��0������1����

void USART2_Init(void);
void UART4_Init(void);
void Usart_SendByte(USART_TypeDef* pUSARTx, char ch);
int Usart_SendString(USART_TypeDef* pUSARTx, const char* str);
char* Usart_ReadBuffer(char* pBuffer);
#endif



