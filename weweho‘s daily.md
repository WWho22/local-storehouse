# weweho‘s daily

## 2025.07.14

- 完善向ESP8266发送命令任务的逻辑（未调试）

  - ```c
    void ESP8266_WriteTask(void* arg)
    {
    	while(1)
    	{
    	if(xSemaphoreTake(Esp8266_SendHandler,portMAX_DELAY) == pdTRUE)
    		{
    			switch(Write_Status)
    			{
    				case WIFISet_Status:
    	UART_AT_Send(AT_WIFI_Mode,strlen(AT_WIFI_Mode),20);
    					break;
    				
    				case WIFIConnect_Status:
    	UART_AT_WIFI_CONNECT_Send(AT_WIFI_Connect,"106-02","106xuxieweili*106",20);
    					break;
    				
    				case TCPConnect_Status:             UART_AT_SERVER_CONNECT_Send(AT_Server_Connect,"192.168.38.1","6666",20);
    					break;
    				
    				case IPQuery_Status:
    		UART_AT_Send(AT_IP_Query,strlen(AT_IP_Query),20);
    					break;
    				default :
    					break;
    			}
    		}
    	}	
    }
    ```

    

- 完善识别URC命令的代码逻辑（未调试）

  - ```c
    /*
     *参数1：处理缓冲区结构体地址
     *参数2：超时时间
     *返回值：1表示为服务器发出的消息，0表示AT命令回复
     *函数作用：判断是否为服务器传递信息：+IPD,2:AB
    */
    	/* 数据格式: +IPD,2:ab */
    int Get_URC_Obj(char* PaserBuf,int timeout)
    {
    	enum URC_Read_Status AT_Status = URC_Init_Status;
    	int i = 0,URC_Len = 0,len = strlen(PaserBuf),PreT_count,NowT_count,m;
    	uint8_t recv_buf[64];
    	if(len<=0) return URC_FALSE;
    	PreT_count = xTaskGetTickCount();
    	while(AT_Status != UnURC_Status)
    	{
    		switch(AT_Status)
    		{
    			case URC_Init_Status:
    				if(i<(strlen("+IPD,")))
    				{
    					recv_buf[i+1] = '\0';
    					if (strstr((char*)recv_buf, "+IPD,"))
    					{
    						AT_Status = URC_Len_Status;
    						i = 0;
    					}
    					else
    					{
    						i++;
    					}
    				}
    				else AT_Status = UnURC_Status;
    				break;
    			
    			case URC_Len_Status:
    				if(PaserBuf[i] == ':')
    				{
    						/*计算长度*/
    						for (m = 0; m < i; m++)
    						{
    							len = len * 10 + PaserBuf[m] - '0';
    						}
    						AT_Status = URC_Data_Status;
    						i = 0;
    				}
    				break;
    			
    			case URC_Data_Status:
    				while(i<URC_Len)
    				{
    //					URC_handled[i] = 1;
    				}
    				AT_Status = URC_Complete_Status;
    				break;
    				
    			default:
    				break;
    		}
    	if(AT_Status == URC_Complete_Status)
    		break;
    	}
    	NowT_count = xTaskGetTickCount();
      if(pdMS_TO_TICKS(NowT_count-PreT_count)>=timeout)return URC_TIMEOUT;
      if(AT_Status == URC_Complete_Status)return URC_TRUE;
      else return URC_FALSE;
    }
    ```

    

- 完成普通AT命令发送函数的编写（未调试）

  - ```c
    int UART_AT_Send(const char *buf, int len, int timeout)
    {
    	int PreT_count = 0,NowT_count = 0;
    	PreT_count = xTaskGetTickCount();
    	if(len<=0) return FALSE;
    	if(xSemaphoreTake(Uart4_Send_Mutexhandle,portMAX_DELAY) == pdTRUE)
    	{
    	 if((!Usart_SendString(UART4,buf))&&(!Usart_SendString(UART4,"\r\n"))) return FALSE;
    	 xSemaphoreGive(Uart4_Send_Mutexhandle);
    	}
    	NowT_count = xTaskGetTickCount();
    	if(pdMS_TO_TICKS(NowT_count - NowT_count) >= timeout)return TIMEOUT;
    	return TRUE;
    }
    ```

    

## 2025.07.15

- 完成回复处理任务的逻辑

  - ```c
    void ESP8266_PalseTask(void* arg)
    {
    	int paser_count = 0;
    	enum paser_handle_status Paser_Status;
    	while(1)
    	{
    		//获取数据处理二值信号量
    		if(xSemaphoreTake(Esp8266_ParseHandler,portMAX_DELAY) == pdTRUE)
    		{
    			//读取环形缓冲区的一个字节
    			if(RingBuffer_ReadByte(&test_RingBuffer,&paser_buf[paser_count]) == 0)
    			{
    			 if(Paser_Status == Complete_Status)
    				 {
    					 paser_count = 0;//收到处理完成标志位，处理缓冲区从头开始存储新数据
    					 Paser_Status = Uncomplete_Status;
    				 }
    				//获取成功则读取环形缓冲区数据，知道读到\r\n
    			 if((paser_buf[paser_count-1] == '\r')&&(paser_buf[paser_count] == '\n'))
    			 {
    				 //判断是否为服务器URC指令回复，是则判断是TRUE，否则是FALSE
    				 if(Get_URC_Obj(paser_buf,200) == URC_TRUE)
    				 {
    					 //URC指令处理函数
    					 URC_handle();
    				 }
    				 //判断是否为常规AT指令回复，是OK还是ERROR
    				 else if(Is_AT_Response(paser_buf) == AT_OK)
    				 {
    					 Paser_Status = Complete_Status;
    					 platform_mutex_unlock(&UsartSend);//若收到OK，则释放互斥量
    				 }
    				 else if(Is_AT_Response(paser_buf) == AT_ERROR)
    				 {
    					 Paser_Status = Complete_Status;
    					 platform_mutex_unlock(&UsartSend);//若收到ERROR，则释放互斥量
    				 }
    			  }
    			 paser_count++;		
    			}
    		}
    	}
    }
    ```

    

- 完成普通AT命令判断函数的逻辑

  - ```c
    int Is_AT_Response(const char* response)
    {
    	char recv_buf[128];
    	int Response_Len = 0;
    	if ((response == NULL) || (strlen(response) == 0)) 
    		{
            return FALSE;
        }
    		while(!isspace(response[Response_Len]))
    		{
    			recv_buf[Response_Len] = response[Response_Len];
    			Response_Len++;
    		}
    		recv_buf[Response_Len] = '\0';
    		if(memcmp(recv_buf,"OK",strlen("OK")) == 0)
    		{
    			return AT_OK;
    		}
    		if(memcmp(recv_buf,"ERROR",strlen("ERROR")) == 0)
    		{
    			return AT_ERROR;
    		}
    		return TRUE;
    }
    ```

## 2025.07.24

freertos互斥量创建时，互斥量为1，二值信号量创建时，初始值为0.







## 2025.07.25

中断函数中调用信号量释放函数不会立即引起休眠任务的调度，而是等中断执行完再调度。

按理来说，中断里释放信号量的函数应该采用带FromISR后缀的函数（释放完信号量会在函数内部进行上下文切换从而进行任务调度），但是可以不用，因为不带FromISR后缀的信号量释放函数不会引起任务阻塞，虽然实时性没那么好（不能快速调度优先级最高的任务）。

使用示例：

![image-20250725145747057](C:\Users\jyq20\AppData\Roaming\Typora\typora-user-images\image-20250725145747057.png)

![image-20250725174358266](C:\Users\jyq20\AppData\Roaming\Typora\typora-user-images\image-20250725174358266.png)

中断分为可屏蔽中断和不可屏蔽中断，其中可屏蔽中断又分为两种中断。紧急中断和一般中断，紧急中断认为FreeRtos的API函数不安全或耗时长，不能使用FreeRtos的API函数，只能自己写，一般中断可以使用。

FreeRtos的API函数被中断调用时，会判断此时中断的优先级，若中断优先级高，会陷入死循环。

![image-20250725175156046](C:\Users\jyq20\AppData\Roaming\Typora\typora-user-images\image-20250725175156046.png)

中断里也不能释放互斥量或获取互斥量，否则也会陷入死循环。

调试的问题：

![image-20250725183411391](C:\Users\jyq20\AppData\Roaming\Typora\typora-user-images\image-20250725183411391.png)

直接硬件错误中断卡死

## 2025.07.25

将AT_TestTask任务的优先级调整到与ESP8266_PalseTask相同，任务堆栈增加到2048字节后，任务可以运行了