#include "includes.h"

u8 usart4_buffer[USART4_REC_LEN];
u8 usart_test;
TaskHandle_t queuesend_Task1_Handle;
TaskHandle_t queuesend_Task2_Handle;
TaskHandle_t queuesend_Task3_Handle;
TaskHandle_t StartTask_handle;
TaskHandle_t led0_task_handle;
TaskHandle_t led1_task_handle;
TaskHandle_t float_task_handle;
TaskHandle_t ESP8266_Palse_task_handle;
TaskHandle_t ESP8266_Write_task_handle;
TaskHandle_t AT_Test_task_handle;
QueueHandle_t x1queuehandle;
QueueHandle_t x2queuehandle;
SemaphoreHandle_t mutexhandle;
//FreeRtos的互斥量没有实现谁使用谁释放的功能，只能靠用户自己遵守这个特性
platform_mutex_t UsartSend;
SemaphoreHandle_t Esp8266_ParseHandler;
SemaphoreHandle_t Esp8266_SendHandler;
RingBuffer test_RingBuffer;
RingBuffer Uart4_RingBuffer;

char Public_buffer[20];
char Uart4_buffer[20];
int AT_LEN = 0;
//ALIENTEK 探索者STM32F407开发板 实验1
//跑马灯实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK

void prvHardware_Init(void)
{
//	RCC_DeInit();
//	RCC_HSEConfig(RCC_HSE_ON);
	LED_Init();		        //初始化LED端口
	uart_init(115200);
	KEY_Init();
	USART2_Init();
	UART4_Init();
	RingBuffer_Init(&test_RingBuffer,Public_buffer,20);
	RingBuffer_Init(&Uart4_RingBuffer,Uart4_buffer,20);
	OLED_Init();
}	


int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);	
	//初始化延时函数
   prvHardware_Init();
	 UART4_Lock_Init(UsartSend);
	 Esp8266_ParseHandler = xSemaphoreCreateBinary();
	 Usart_SendString(USART2,"Initilize OK\r\n");
//   Usart_SendString(UART4,"Initialize Finish");
//   Usart_SendString(UART4,"AT+CWMODE=3\r\n",40000);
//	 AT_SendCmd("AT+CWMODE=3\r\n",40000);
//	Usart_SendString(USART2,"AT");
   xTaskCreate( (TaskFunction_t) start_task,
                            (const char *) "start",     /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                            (uint16_t  )128,
                            (void *) 100,
                            (UBaseType_t) 1,
                            (TaskHandle_t *) &StartTask_handle );
	
	 vTaskStartScheduler();
	 return 0;
}


/**
*******************下面注释掉的代码是通过 位带 操作实现IO口控制**************************************
	
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
  while(1)
	{
     LED0=0;			  //LED0亮
	   LED1=1;				//LED1灭
		 delay_ms(500);
		 LED0=1;				//LED0灭
		 LED1=0;				//LED1亮
		 delay_ms(500);
	 }
}
**************************************************************************************************
 **/	
	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOF->BSRRH=GPIO_Pin_9;//LED0亮
	   GPIOF->BSRRL=GPIO_Pin_10;//LED1灭
		 delay_ms(500);
     GPIOF->BSRRL=GPIO_Pin_9;//LED0灭
	   GPIOF->BSRRH=GPIO_Pin_10;//LED1亮
		 delay_ms(500);

	 }
 }	 
**************************************************************************************************
**/	
 

/*

#define __def_class(__name, ...)                                                \
    typedef struct __name __name;                                               \
    struct __name {                                                             \
        __VA_ARGS__                                                             \
    };   

#define def_class(__name, ...)          __def_class(__name, __VA_ARGS__)

def_class(message_handlers_t,
    private_member(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;
    )
)

	
	__def_class(message_handlers_t,     private_member(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;
    ))
	
	
	  typedef struct message_handlers_t message_handlers_t;                                               \
    struct message_handlers_t {                                                             \
        struct  {                                                               \
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;                                                         \
        }PLOOC_ALIGN(PLOOC_ALIGNOF_STRUCT(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;));                                                            \
    };  
	
#define private_member                          ,_private_member



#   define private_member(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;)              
		
		PLOOC_VISIBLE(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;)



#       define PLOOC_VISIBLE(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;)                                               \
        struct  {                                                               \
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;                                                         \
        }PLOOC_ALIGN(PLOOC_ALIGNOF_STRUCT(
        mqtt_list_t         list;
        mqtt_qos_t          qos;
        const char*         topic_filter;
        message_handler_t   handler;));
*/

