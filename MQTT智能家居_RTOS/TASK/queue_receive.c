#include "includes.h"



void queuereceive_Task(void* pvParameters)
{
	int32_t QueueData;
	BaseType_t xStatus;
	TickType_t WaitToReceive = pdMS_TO_TICKS(10UL);
	
	for(;;)
	{
	xStatus = xQueueReceive((QueueHandle_t) x1queuehandle,
				  (void *)&QueueData,
				  (TickType_t) WaitToReceive);
	if(xStatus == pdTRUE)
	{
		printf("ReceiveData = %d\r\n",&QueueData);
	}
	else printf("Receive Error");
	}
}

