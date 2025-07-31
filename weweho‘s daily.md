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

## 2025.07.26

问题排查：

- 将AT_TestTask任务的优先级调整到与ESP8266_PalseTask相同，任务堆栈增加到1024字节。

- 将原本在主函数C文件定义的信号量结构体变量

  ```c
  static platform_mutex_t UsartSend
  ```

  放置到esp8266.c文件，也就是UART_AT_Send函数所定义的位置

  ```c
  int UART_AT_Send(const char *buf, int len, int timeout)
      {
  //	int PreT_count = 0,NowT_count = 0;
  	int ret,err;
  //	PreT_count = xTaskGetTickCount();
  //	if(len<=0) return FALSE;
  //	if((!Usart_SendString(UART4,buf))&&(!Usart_SendString(UART4,"\r\n"))) 
  //	 {
  //		 return FALSE;
  //	 }
  	 Usart_SendString(UART4,buf);
  	 Usart_SendString(UART4,"\r\n");
  	 ret = platform_mutex_lock_timeout(&UsartSend,timeout);
  
  //	NowT_count = xTaskGetTickCount();
  //	if(pdMS_TO_TICKS(NowT_count - PreT_count) >= timeout)return TIMEOUT;
  		if( ret == pdTRUE)
  	{
  		 err = GetATStatus();
       return err;
  	}
  	else return TIMEOUT;
  }
  ```

  此处使用到了UsartSend信号量结构体变量，将其定义为静态变量，然后便可正常进行任务调度。

  串口打印的结果为正常收到信号量，但是AT命令的回复具体是不是OK还没看，需要在调试中查看环形缓冲区的数据

## 2025.07.27

今日目标：

使用已写好的AT命令编写MQTT的socket网络函数

- platform_net_socket_recv_timeout
- platform_net_socket_write_timeout
- platform_net_socket_connect
- platform_net_socket_close

问题排查：

环形缓冲区的数据接收存在问题，每接收一个字符就会有一个间隔，因此无法进入字符串处理函数。

![image-20250727213706709](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250727213706709.png)

![image-20250727213849152](C:\Users\jyq20\Documents\GitHub\local-storehouse\image\image-20250727213849152.png)

正常接收情况应该是这样：

![image-20250727214011257](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250727214011257.png)

昨天可以正常接收，但是今天出了问题，费解

搜了一下貌似是这种情况：

最根本的原因是，您的**微控制器（MCU）的接收逻辑被配置为按16位（半字/half-word）单位来读取数据，但却将这些数据存入了一个8位（字节/byte）的数组中**。

让我们以字符 'A' 为例来解释这个过程：

1. 蓝牙模块通过UART发送了字符 'A'，它的ASCII码是 0x41 (一个8位的字节)。
2. MCU的UART外设接收到这个8位的字节 0x41。
3. 由于配置错误，DMA控制器或者CPU被指令从UART的数据寄存器（Data Register, DR）中读取一个**16位**的数据。此时，硬件会将这个8位的 0x41 扩展成一个16位的值，通常是 0x0041。
4. DMA或CPU现在需要将这个16位的值 0x0041 存入您定义的8位 char 类型的数组 paser_buf 中。
5. 在存储时，这个16位的数据被拆分成两个8位的字节。根据您的系统是小端模式（Little-Endian）还是大端模式（Big-Endian），存储顺序会不同。从您的截图 [0x00, 0x41] 来看，系统将低地址存低位字节，高地址存高位字节，这符合小端模式的特征（*注：也可能是硬件读取和放置的自然结果*）。
   - 
   - 第一个字节 0x00 被写入 paser_buf[i]。
   - 第二个字节 0x41 ('A') 被写入 paser_buf[i+1]。

这个过程对每个接收到的字符都重复一遍，就导致了您看到的每个有效字符前面都插入了一个 \0 的现象。

## 2025.07.28

07.26的代码可以正常接收字符，但是乱序了

![image-20250728113519252](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728113519252.png)

更老的代码也有问题

![image-20250728115517388](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728115517388.png)

目前代码中，环形缓冲区的数据是这样的

![image-20250728150406126](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728150406126.png)

但是处理缓冲区读取的数据是这样的

![image-20250728150429115](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728150429115.png)

并且串口打印出来的数据貌似也没有问题

![image-20250728150521601](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728150521601.png)

问题发现：

之前AT_Receive_Char(&(paser_buf[paser_count]),portMAX_DELAY)单纯只用来接收数据，但是无论有没有接收到数据，paser_count都会加1。而paser_buf[]初始化后，内部数组单元的值都为0x00，因此可以判断为在环形缓冲区没有收到新数据时，paser都会加1，导致paser_buf[paser_count]会出现间隔有0x00的问题。

![image-20250728170126032](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728170126032.png)

问题解决：

![image-20250728170010627](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250728170010627.png)

新问题：

![image-20250729150224556](C:\Users\jyq20\Documents\GitHub\local-storehouse\image\image-20250729150224556.png)

![image-20250729150201611](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250729150201611.png)



刚开始时，可能会出现ERROR和busy等待的情况，需要继续改进发送逻辑和接收逻辑。

## 2025.07.29

今日目标：

使用已写好的AT命令编写MQTT的socket网络函数

nettype_tcp.c

- platform_net_socket_recv_timeout
- platform_net_socket_write_timeout
- platform_net_socket_connect
- platform_net_socket_close

真完蛋，今天装了个项目的环境和跑了一下就没时间了啊啊啊啊啊。

## 2025.07.30

连接服务器的AT命令流程：

```shell
AT

// 1. 配置 WiFi 模式
AT+CWMODE=3						//	softAP+station	mode

// 2. 连接路由器
AT+CWJAP="SSID","password"		//	SSID	and	password	of	router

// 3. 查询 ESP8266 设备的 IP 地址
AT+CIFSR

// 响应
+CIFSR:APIP,"192.168.4.1"
+CIFSR:APMAC,"1a:fe:34:a5:8d:c6"
+CIFSR:STAIP,"192.168.3.133"
+CIFSR:STAMAC,"18:fe:34:a5:8d:c6"
OK

// 4. ESP8266 设备作为 TCP client 连接到服务器
AT+CIPSTART="TCP","192.168.3.116",8080			 //protocol,	server	IP	and	port

// 5. ESP8266 设备向服务器器发送数据
AT+CIPSEND=4				//	set	date	length	which	will	be	sent,		such	as	4	bytes	
>test						//	enter	the	data,		no	CR

// 响应
Recv	4	bytes
SEND	OK

// 6. 当 ESP8266 设备接收到服务器器发来的数据，将提示如下信息：
+IPD,n:xxxxxxxxxx				//	received	n	bytes,		data=xxxxxxxxxxx	
```

## 2025.07.31

今天完成了四个网络读写、连接、断开函数

nettype_tcp.c

- platform_net_socket_recv_timeout
- platform_net_socket_write_timeout
- platform_net_socket_connect
- platform_net_socket_close

新内容：

创建MQTT_Client_Task任务，内部是订阅话题，不断接收话题信息

![image-20250731134952300](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250731134952300.png)

目前的通信流程：

手机发信息给MQTT服务器->MQTT服务器发消息给esp8266WIFI模块->esp8266WIFI模块通过串口发送数据包给MCU->串口中断接收数据，将数据存入环形缓冲区，接收一个数据释放一次数据处理任务的信号量->数据处理任务，如果处理状态是数据包状态，则将数据存入环形缓冲区，释放数据接收任务信号量->数据接收任务读取环形缓冲区。

![image-20250731141718306](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250731141718306.png)

![image-20250731141803701](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250731141803701.png)

![image-20250731141835670](C:\Users\jyq20\Documents\GitHub\local-storehouse\weweho‘s daily.assets\image-20250731141835670.png)