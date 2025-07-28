#include "includes.h"
//const char AT_WIFI_Mode[]          = "AT+CWMODE=3";//配置WIFI模式
////"AT+CWJAP=\"SSID\",\"password\""
//const char AT_WIFI_Connect[]       = "AT+CWJAP=\"";//连接路由器
//const char AT_IP_Query[]           = "AT+CIFSR";//查询ESP8266设备的IP地址
//const char AT_Server_Connect[]     = "AT+CIPSTART=\"TCP\",\"";//ESP8266 设备作为 TCP client 连接到上述服务器
//                                                      //还要补充服务器地址和端口
//AT+CIPSTART=\"TCP\",\"192.168.3.116\",8080
//const char AT_Transmiss_Mode[]     = "AT+CIPMODE=1";//使能透传模式
//const char AT_Send_Server[]        = "AT+CIPSEND";//向服务器发送数据



enum URC_Handle_Status URC_Status;//服务器URC回复的处理状态
URC_handled* Cur_URC;
static platform_mutex_t UsartSend;
static uint8_t recv_buf[Recv_Buf_LENGTH];
static int g_at_status;
static char* g_cur_cmd;

void URC_handler_Init(void)
{
	Cur_URC -> URC_Data_handled_length = URC_LENGTH;
}

void UART4_Lock_Init(void)
{
	 platform_mutex_init(&UsartSend);
	 platform_mutex_lock(&UsartSend);
}

void Esp8266_Parse_Init(void)
{
	Esp8266_ParseHandler = xSemaphoreCreateBinary();
}

void UART4_Unlock(void)
{
	platform_mutex_unlock(&UsartSend);
}

void SetATStatus(int status)
{
	g_at_status = status; 
}

int GetATStatus(void)
{
	return g_at_status;
}

/*
 *参数1：要发送的AT命令
 *参数2：AT命令字符串的长度
 *参数3：超时时间(ms)
 *函数作用：发送AT命令字符串，字符串长度<=0，发送失败FALSE，发送成功TRUE
*/
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
//	 g_cur_cmd = buf;
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
/*
 *参数1：要发送的ATwifi连接命令
 *参数2：要连接的路由器ID
 *参数3：WIFI密码
 *参数4：超时时间(ms)
 *函数作用：发送AT命令字符串，字符串长度<=0，发送失败FALSE，发送成功TRUE
*/
int UART_AT_WIFI_CONNECT_Send(const char *buf,char* SSID,char* password,int timeout)
{
	int PreT_count = 0,NowT_count = 0,len = strlen(buf);
	PreT_count = xTaskGetTickCount();
	if(len<=0) return FALSE;
		if(platform_mutex_lock(&UsartSend) == pdTRUE)
	{
	 if((!Usart_SendString(UART4,buf))&&(!Usart_SendString(UART4,SSID))&&
		 (!Usart_SendString(UART4,"\",\""))&&(!Usart_SendString(UART4,password))&&
	 (!Usart_SendString(UART4,"\""))) 
	 return FALSE;
	}
	NowT_count = xTaskGetTickCount();
	if(pdMS_TO_TICKS(NowT_count - PreT_count) >= timeout)return TIMEOUT;
	return TRUE;
}

/*
 *参数1：要发送的ATserver连接命令
 *参数2：要连接的服务器IP地址
 *参数3：服务器端口
 *参数4：超时时间(ms)
 *函数作用：发送AT命令字符串，字符串长度<=0，发送失败FALSE，发送成功TRUE
*/
int UART_AT_SERVER_CONNECT_Send(const char *buf,char* SAddr ,char* Port,int timeout)
{
	int PreT_count = 0,NowT_count = 0,len = strlen(buf);
	PreT_count = xTaskGetTickCount();
	if(len<=0) return FALSE;
		if(platform_mutex_lock(&UsartSend) == pdTRUE)
	{
	 if((!Usart_SendString(UART4,buf))&&(!Usart_SendString(UART4,SAddr))&&
		 (!Usart_SendString(UART4,"\","))&&(!Usart_SendString(UART4,Port))) 
	 return FALSE;
	}
	NowT_count = xTaskGetTickCount();
	if(pdMS_TO_TICKS(NowT_count - PreT_count) >= timeout)return TIMEOUT;
	return TRUE;
}

/*
 *参数1：用于存储环形缓冲区数据的缓冲区地址
 *参数2：超时时间
 *函数作用：接收环形缓冲区的一个字符数据
*/

int AT_Receive_Char(char *c, TickType_t timeout)
{
	if(RingBuffer_ReadByte(&test_RingBuffer,(unsigned char *)c) == 0)
		return 1;
	else
	{
	xSemaphoreTake(Esp8266_ParseHandler,timeout);
		return 0;
	}
}

/*
 *参数1：用于存储AT命令的缓冲区首地址 
 *参数2：存储AT命令长度的变量地址 
 *参数3：超时时间
 *函数作用：接收一行回复,只保存\r\n前的命令字符串
*/
//	static char last_char;
//		if((res == '\n')&&(last_char == '\r'))
//		{
//			xSemaphoreGiveFromISR(Esp8266_ParseHandler,&xHigherPriorityTaskWoken);
//		}
//		last_char = res;
//     }
//int UART_Receive_Line(char *buf, int* plen, int timeout)
//{
//	int Rec_count = 0,len_count = 0,T_count = 0;
//	char res;
//	 do
//	{
//		switch(RingBuffer_ReadByte(&test_RingBuffer,&res))
//		{
//			case Rec_SUCCESS:
//			if((res!='\r')&&(res!='\n'))//环形缓冲区中接收的字符串数据通常以\r\n结尾
//		   {
//		     buf[Rec_count++] = res;
//		     len_count++;
//		   }
//			case Rec_FAIL:
//				return ERROR;
//		}
//		T_count++;
//		if(T_count>=timeout) return TIMEOUT;
//	}while(res!='\n');
//	*plen = len_count;
//	return TRUE;
//}
/*
 *参数1：用于存储字符串的缓冲区首地址 
 *函数作用：判断是否为AT命令的回复
*/
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

/*
 *参数1：用于存储字符串的缓冲区首地址 
 *函数作用：判断是否为AT发送命令的回复
*/
int Get_CIPSEND_Result(char* buf)
{
	if(g_cur_cmd&&(strstr(g_cur_cmd,"AT+CIPSEND="))&&(buf[0] == '>'))
		return TRUE;
	else return FALSE;
}

/*
 *参数1：用于存储字符串的缓冲区首地址 
 *函数作用：判断是否为AT命令的回复
*/
int Get_URC_Result(char* buf)
{
	if(strstr(buf,"+IPD,"))
		return TRUE;
	else return FALSE;
}

/*
 *参数1：AT命令字符串
 *参数2：超时时间
 *函数作用：发送AT命令,发送成功返回值为1，发送失败返回值为0
*/
int AT_SendCmd(char *cmd, int timeout)
{
	if(UART_AT_Send(cmd,strlen(cmd),timeout) != TRUE)
		return ERROR;
	return TRUE;
}

/*
 *参数1：处理缓冲区结构体地址
 *参数2：超时时间
 *参数3：收到的字符串长度（引入该参数是因为某些命令的回复长短不一，前一刻太长的字符串没有完全被后一刻的清空）
 *返回值：1表示为服务器发出的消息，0表示AT命令回复
 *函数作用：判断是否为服务器传递信息：+IPD,2:AB
*/
	/* 数据格式: +IPD,2:ab */
int Get_URC_Obj(char* PaserBuf,int timeout,int len)
{
	enum URC_Read_Status AT_Status = URC_Init_Status;
	int i = 0,PreT_count,NowT_count,Cur_Len_Pos,m;
	URC_handler_Init();
	if(len<=0) return URC_FALSE;
	PreT_count = xTaskGetTickCount();
	while(AT_Status != UnURC_Status)
	{
		switch(AT_Status)
		{
			case URC_Init_Status:
				if((i<Recv_Buf_LENGTH)&&(i<len))
				{
					recv_buf[i] = PaserBuf[i];
					recv_buf[i+1] = '\0';
					if (strstr((char*)recv_buf, "+IPD,"))
					{
						AT_Status = URC_Len_Status;
						Cur_Len_Pos = i+1;
					}
					else
					{
						i++;
					}
				}
				else AT_Status = URC_Complete_Status;
				break;
			
			case URC_Len_Status:
				if(i<Recv_Buf_LENGTH)
				{
				if(PaserBuf[i] == ':')
				{
						/*计算长度*/
						for (m = Cur_Len_Pos ; m < i ; m++)
						{
							Cur_URC->Effective_word_len = Cur_URC->Effective_word_len * 10 + (PaserBuf[m] - '0');
						}
						m = 0;
						Cur_URC->URC_len = i+ Cur_URC->Effective_word_len+1;
						AT_Status = URC_Data_Status;
				}
				else 
				{
					i++;
				}
			  }
				else AT_Status = URC_Complete_Status;
				break;
			
			case URC_Data_Status:
				//这里应该补充是否完整接收到数据，如果URC处理结构体的数组长度小于Effective_word_len，则接收缺失
			  //这里的判断条件是因为存入处理数组既要满足不超过处理数组的最大长度，
			  //也要满足接收所能容纳的URC的最大长度，并且i也不能超过接收缓冲区的最大长度
				if((m<Cur_URC->URC_Data_handled_length)&&(i<Recv_Buf_LENGTH)&&(m<Cur_URC->URC_len))
				{
					Cur_URC->URC_Data_handled[m] =  PaserBuf[i];
					m++;
					i++;
				}				
				else
				{					
					AT_Status = URC_Complete_Status;
				  if(m == (Cur_URC->URC_len-1))
						Cur_URC ->URC_Rec = Full_Rec;
					else 
						Cur_URC->URC_Rec = Missing_Rec;
				}
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


void URC_handle(void)
{
	
}
////配置 WiFi 模式
////AT+CWMODE=3	 softAP+station	mode
//void ESP8266_Mode_Set(void)
//{
//	Usart_SendString(UART4,"AT+CWMODE=3");
//}

////连接路由器
////AT+CWJAP="SSID","password"
//void Wifi_Connect(char* SSID,char* Password )
//{ 
//	
//    char* command = "AT+CWJAP=\"";
//    char* ssid = SSID;
//    char* password = Password;

//    strcat(command, ssid);
//    strcat(command, "\",\"");
//    strcat(command, password);
//    strcat(command, "\"\r\n");
//	Usart_SendString(UART4,command);
//	printf(command);
//}

//void Query_ESP8266_Address(void)
//{
//	
//}

////ESP8266 设备作为 TCP client 连接到服务器
////AT+CIPSTART="TCP","192.168.3.116",8080
//void Connect_To_Server(char* Server_IP,char* Server_Port)
//{
//	char* command = "AT+CIPSTART=\"TCP\",\"";
//	char* server_ip = Server_IP;
//    char* server_port = Server_Port;
//	
//	strcat(command, server_ip);
//    strcat(command, "\",");
//    strcat(command, server_port);
//    strcat(command, "\r\n");
//	Usart_SendString(UART4,command);
//	printf(command);
//}



