#include "includes.h"

void queuesend_Task(void* pvParameters)
{
    BaseType_t xStatus;
	TickType_t WaitToSend = pdMS_TO_TICKS(10UL);
	
	for(;;)
	{
	xStatus = xQueueSend(
	           (QueueHandle_t)   x1queuehandle,
               (const void*) pvParameters,
               (TickType_t)       WaitToSend);
			   
	if(xStatus != pdTRUE)
	{
		printf("Send Error");
	}
	}
}

