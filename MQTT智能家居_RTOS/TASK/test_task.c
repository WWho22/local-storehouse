#include "includes.h"


void start_task(void* pvParameters)
{
	//进入临界区
	taskENTER_CRITICAL();
	xTaskCreate( (TaskFunction_t) led0_task,
                            (const char *) "led0",     
                            (uint16_t  )128,
                            (void *) NULL,
                            (UBaseType_t) 2,
                            (TaskHandle_t *) &led0_task_handle );
	
	xTaskCreate( (TaskFunction_t) led1_task,
                            (const char *) "led1",     
                            (uint16_t  )128,
                            (void *) NULL,
                            (UBaseType_t) 3,
                            (TaskHandle_t *) &led1_task_handle );

	xTaskCreate( (TaskFunction_t) float_task,
                            (const char *) "float",     
                            (uint16_t  )128,
                            (void *) NULL,
                            (UBaseType_t) 4,
                            (TaskHandle_t *) &float_task_handle );	
							
//	xTaskCreate( (TaskFunction_t) ESP8266_ReadTask,
//                            (const char *) "ESP8266_ReadTask",     
//                            (uint16_t  )256,
//                            (void *) NULL,
//                            (UBaseType_t) 3,
//                            (TaskHandle_t *) &ESP8266_Read_task_handle );								
							
	vTaskDelete(StartTask_handle);
	//退出临界区
	taskEXIT_CRITICAL();
}

//void Usart_test(void* arg)
//{
//	while(1)
//	{
//		if();
//	}
//}

void led0_task(void* arg)
{
	while(1)
	{
		GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
		vTaskDelay(pdMS_TO_TICKS(500UL));
	}
}

void led1_task(void* arg)
{
	mutexhandle = xSemaphoreCreateMutex();
	while(1)
	{
	xSemaphoreTake(mutexhandle,portMAX_DELAY);
	GPIO_SetBits(GPIOF,GPIO_Pin_11);
	vTaskDelay(pdMS_TO_TICKS(200UL));
	GPIO_ResetBits(GPIOF,GPIO_Pin_11);
	vTaskDelay(pdMS_TO_TICKS(800UL));	
	xSemaphoreGive(mutexhandle);
	}
}

void float_task(void* arg)
{
	static float float_num = 0.00;
	while(1)
	{
		float_num+=0.01f;
		printf("The float_num is %f\r\n",float_num);
		vTaskDelay(pdMS_TO_TICKS(2000UL));
	}
}

void ESP8266_PalseTask(void* arg)
{
	
	while(1)
	{
		if(xSemaphoreTake(Esp8266_ParseHandler,portMAX_DELAY) == pdTRUE)
		{
          			
		}
	   vTaskDelay(5UL);
	}
}

void ESP8266_WriteTask(void* arg)
{
	while(1)
	{
		if(xSemaphoreTake(Esp8266_SendHandler,portMAX_DELAY) == pdTRUE)
		{
			
		}
	}	
}

