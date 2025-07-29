#include "includes.h"
char paser_buf[30];
enum paser_handle_status{Complete_Status,Uncomplete_Status};
enum write_status{WIFISet_Status,WIFIConnect_Status,TCPConnect_Status,IPQuery_Status};
const char AT_Test[]               = "AT" ;//AT��������  
char AT_WIFI_Mode[]          = "AT+CWMODE=3";//����WIFIģʽ
char AT_WIFI_Connect[]       = "AT+CWJAP=\"";//����·����
char AT_IP_Query[]           = "AT+CIFSR";//��ѯESP8266�豸��IP��ַ
char AT_Server_Connect[]     = "AT+CIPSTART=\"TCP\",\"";//ESP8266 �豸��Ϊ TCP client ���ӵ�����������                                                      //��Ҫ�����������ַ�Ͷ˿�
char AT_Transmiss_Mode[]     = "AT+CIPMODE=1";//ʹ��͸��ģʽ
char AT_Send_Server[]        = "AT+CIPSEND";//���������������
enum write_status Write_Status;



void start_task(void* pvParameters)
{
	//�����ٽ���
	taskENTER_CRITICAL();
//	xTaskCreate( (TaskFunction_t) led0_task,
//                            (const char *) "led0",     
//                            (uint16_t  )128,
//                            (void *) NULL,
//                            (UBaseType_t) 2,
//                            (TaskHandle_t *) &led0_task_handle );
//	
//	xTaskCreate( (TaskFunction_t) led1_task,
//                            (const char *) "led1",     
//                            (uint16_t  )128,
//                            (void *) NULL,
//                            (UBaseType_t) 3,
//                            (TaskHandle_t *) &led1_task_handle );

//	xTaskCreate( (TaskFunction_t) float_task,
//                            (const char *) "float",     
//                            (uint16_t  )128,
//                            (void *) NULL,
//                            (UBaseType_t) 4,
//                            (TaskHandle_t *) &float_task_handle );
 	xTaskCreate( (TaskFunction_t) ESP8266_PalseTask,
                            (const char *) "ESP8266_Palse",     
                            (uint16_t  )1024,
                            (void *) NULL,
                            (UBaseType_t) 1,
                            (TaskHandle_t *) &ESP8266_Palse_task_handle );	
 	xTaskCreate( (TaskFunction_t) AT_TestTask,
                            (const char *) "AT_Test",     
                            (uint16_t  )1024,
                            (void *) NULL,
                            (UBaseType_t) 1,
                            (TaskHandle_t *) &AT_Test_task_handle );															
							
//	xTaskCreate( (TaskFunction_t) ESP8266_ReadTask,
//                            (const char *) "ESP8266_ReadTask",     
//                            (uint16_t  )256,
//                            (void *) NULL,
//                            (UBaseType_t) 3,
//                            (TaskHandle_t *) &ESP8266_Read_task_handle );								
							
	vTaskDelete(StartTask_handle);
	//�˳��ٽ���
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

/*
while(1)
{
1.��ȡ���ݴ����ֵ�ź���
2.��ȡ�ɹ����ȡ���λ��������ݣ�֪������\r\n
3.�ж��Ƿ�Ϊ���������ݣ�ʵ��ִ�з��������ݴ�����
4.�ж��Ƿ�Ϊ����ATָ��ظ��������ж���OK����ERROR
5.OK���ͷ�AT����Ͷ�ֵ�ź���

}
*/
//	static char last_char;
//		if((res == '\n')&&(last_char == '\r'))
//		{
//			xSemaphoreGiveFromISR(Esp8266_ParseHandler,&xHigherPriorityTaskWoken);
//		}
//		last_char = res;
//     }

void ESP8266_PalseTask(void* arg)
{
	int paser_count = 0;
	enum paser_handle_status Paser_Status;
	Paser_Status = Uncomplete_Status;
	while(1)
	{
				if(Paser_Status == Complete_Status)
				 {
					 paser_count = 0;//�յ�������ɱ�־λ������������ͷ��ʼ�洢������
					 Paser_Status = Uncomplete_Status;
				 }
				//��ȡ���ݴ����ֵ�ź���,��ȡ���λ�������һ���ֽ�
			  
				if(AT_Receive_Char(&(paser_buf[paser_count]),portMAX_DELAY))
				{
				//��ȡ�ɹ����ȡ���λ��������ݣ�֪������\r\n
			 if(paser_count&&(paser_buf[paser_count-1] == '\r')&&(paser_buf[paser_count] == '\n'))
			 {
				 if(Get_CIPSEND_Result(paser_buf))
				 {
					 return;
				 }
				 //�ж��Ƿ�Ϊ������URCָ��ظ��������ж���TRUE��������FALSE
				 if(strstr(paser_buf,"+IPD,"))
				 {
					 //URCָ�����
					 Get_URC_Obj(paser_buf,200,paser_count+1);
					 Paser_Status = Complete_Status;
				 }
				 //�ж��Ƿ�Ϊ����ATָ��ظ�����OK����ERROR
				 else if(Is_AT_Response(paser_buf) == AT_OK)
				 {
					 SetATStatus(AT_OK);
					 Paser_Status = Complete_Status;
					 UART4_Unlock();//���յ�OK�����ͷŻ�����
				 }
				 else if(Is_AT_Response(paser_buf) == AT_ERROR)
				 {
					 SetATStatus(AT_ERROR);
					 Paser_Status = Complete_Status;
					 UART4_Unlock();//���յ�ERROR�����ͷŻ�����
				 }
				 else 
				 {
					 paser_count++;
				 }
			  }
			 else
			 {
			  paser_count++;
			 }
		 }
				if(paser_count>=30)
				{
					paser_count = 0;
				}
	}
}

void ESP8266_WriteTask(void* arg)
{
	while(1)
	{
			switch(Write_Status)
			{
				case WIFISet_Status:
					UART_AT_Send(AT_WIFI_Mode,strlen(AT_WIFI_Mode),20);
					break;
				
				case WIFIConnect_Status:
					UART_AT_WIFI_CONNECT_Send(AT_WIFI_Connect,"106-02","106xuxieweili*106",20);
					break;
				
				case TCPConnect_Status:
					UART_AT_SERVER_CONNECT_Send(AT_Server_Connect,"192.168.38.1","6666",20);
					break;
				
				case IPQuery_Status:
					UART_AT_Send(AT_IP_Query,strlen(AT_IP_Query),20);
					break;
				default :
					break;
		  }
	}	
}

void AT_TestTask(void* arg)
{
	int ret = 0;
	while(1)
	{
		ret = UART_AT_Send("AT",strlen(AT_Test),1000);
		printf("ret is %d\r\n",ret);
	}
}


