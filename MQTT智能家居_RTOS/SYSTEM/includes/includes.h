#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/*FreeRTOS����*/
#include "sys.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "list.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"
#include "semphr.h"

/*��׼������*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  
#include "stm32f4xx.h"


/*Ӳ����������*/
#include "usart.h"
#include "led.h"
#include "usart2.h"
#include "gpio.h"
#include "dht11.h"
#include "oled.h"
#include "esp8266.h"

/*���ݽṹ����*/
#include "ringbuffer.h"

/*ƽ̨�ź�������*/
#include "platform_mutex.h"



/*OLED��ʽ��*/
//#include "OLED_Font.h"

#define START_TASK_PRIO 1
#define START_TASK_STACK_SIZE 128

#define LED0_TASK_PRIO 1
#define LED0_TASK_STACK_SIZE 128

#define LED1_TASK_PRIO 1
#define LED1_TASK_STACK_SIZE 128

#define FLOAT_TASK_PRIO 1
#define FLOAT_TASK_STACK_SIZE 128

#define USART4_REC_LEN 100


extern u8 usart4_buffer[USART4_REC_LEN];
extern u8 usart_test;
extern TaskHandle_t queuesend_Task_Handle;
extern TaskHandle_t queuesend_Task2_Handle;
extern TaskHandle_t queuesend_Task3_Handle;
extern QueueHandle_t x1queuehandle;
extern QueueHandle_t x2queuehandle;
extern TaskHandle_t StartTask_handle;
extern TaskHandle_t led0_task_handle;
extern TaskHandle_t led1_task_handle;
extern TaskHandle_t float_task_handle;
extern TaskHandle_t ESP8266_Palse_task_handle;
extern TaskHandle_t ESP8266_Write_task_handle;
extern TaskHandle_t AT_Test_task_handle;
extern SemaphoreHandle_t mutexhandle;
extern RingBuffer test_RingBuffer;
extern RingBuffer Uart4_RingBuffer;
extern platform_mutex_t UsartSend;
extern SemaphoreHandle_t Esp8266_ParseHandler;//Esp8266�Ļظ������ź���
extern SemaphoreHandle_t Esp8266_SendHandler;//Esp8266��AT������ź���
extern int AT_LEN;
//AT����

extern enum URC_Handle_Status URC_Status;//������URC�ظ��Ĵ���״̬
extern enum write_status Write_Status;

//�������е�FreeRTOS����
void queuesend_Task(void* pvParameters);
void queuereceive_Task(void* pvParameters);
void start_task(void* pvParameters);
void led0_task(void* arg);
void led1_task(void* arg);
void float_task(void* arg);
void ESP8266_PalseTask(void* arg);
void ESP8266_WriteTask(void* arg);
void AT_TestTask(void* arg);







#endif
