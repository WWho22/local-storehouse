#ifndef __INCLUDES_H__
#define __INCLUDES_H__

/*FreeRTOS配置*/
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

/*标准库引用*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"


/*硬件外设配置*/
#include "usart.h"
#include "led.h"
#include "usart2.h"
#include "gpio.h"
#include "dht11.h"
#include "oled.h"
#include "esp8266.h"

/*数据结构配置*/
#include "ringbuffer.h"



/*OLED格式库*/
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
extern TaskHandle_t ESP8266_Read_task_handle;
extern SemaphoreHandle_t mutexhandle;
extern RingBuffer test_RingBuffer;
extern RingBuffer Uart4_RingBuffer;
extern SemaphoreHandle_t Esp8266_ParseHandler;
extern SemaphoreHandle_t Esp8266_SendHandler;
extern int AT_LEN;
//定义所有的FreeRTOS任务
void queuesend_Task(void* pvParameters);
void queuereceive_Task(void* pvParameters);
void start_task(void* pvParameters);
void led0_task(void* arg);
void led1_task(void* arg);
void float_task(void* arg);
void ESP8266_ReadTask(void* arg);
void ESP8266_WriteTask(void* arg);








#endif
