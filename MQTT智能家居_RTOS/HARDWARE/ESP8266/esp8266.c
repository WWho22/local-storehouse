#include "includes.h"
//const char AT_WIFI_Mode[]          = "AT+CWMODE=3";//����WIFIģʽ
////"AT+CWJAP=\"SSID\",\"password\""
//const char AT_WIFI_Connect[]       = "AT+CWJAP=\"";//����·����
//const char AT_IP_Query[]           = "AT+CIFSR";//��ѯESP8266�豸��IP��ַ
//const char AT_Server_Connect[]     = "AT+CIPSTART=\"TCP\",\"";//ESP8266 �豸��Ϊ TCP client ���ӵ�����������
//                                                      //��Ҫ�����������ַ�Ͷ˿�
//AT+CIPSTART=\"TCP\",\"192.168.3.116\",8080
//const char AT_Transmiss_Mode[]     = "AT+CIPMODE=1";//ʹ��͸��ģʽ
//const char AT_Send_Server[]        = "AT+CIPSEND";//���������������



enum URC_Handle_Status URC_Status;//������URC�ظ��Ĵ���״̬
URC_handled* Cur_URC;
static uint8_t recv_buf[Recv_Buf_LENGTH];
static int g_at_status;

void SetATStatus(int status)
{
	g_at_status = status; 
}

int GetATStatus(void)
{
	return g_at_status;
}

/*
 *����1��Ҫ���͵�AT����
 *����2��AT�����ַ����ĳ���
 *����3����ʱʱ��(ms)
 *�������ã�����AT�����ַ������ַ�������<=0������ʧ��FALSE�����ͳɹ�TRUE
*/
int UART_AT_Send(const char *buf, int len, int timeout)
{
//	int PreT_count = 0,NowT_count = 0;
	int ret,err;
//	PreT_count = xTaskGetTickCount();
	if(len<=0) return FALSE;
	if((!Usart_SendString(UART4,buf))&&(!Usart_SendString(UART4,"\r\n"))) 
	 {
		 return FALSE;
	 }
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
 *����1��Ҫ���͵�ATwifi��������
 *����2��Ҫ���ӵ�·����ID
 *����3��WIFI����
 *����4����ʱʱ��(ms)
 *�������ã�����AT�����ַ������ַ�������<=0������ʧ��FALSE�����ͳɹ�TRUE
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
 *����1��Ҫ���͵�ATserver��������
 *����2��Ҫ���ӵķ�����IP��ַ
 *����3���������˿�
 *����4����ʱʱ��(ms)
 *�������ã�����AT�����ַ������ַ�������<=0������ʧ��FALSE�����ͳɹ�TRUE
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
 *����1�����ڴ洢���λ��������ݵĻ�������ַ
 *����2����ʱʱ��
 *�������ã����ջ��λ�������һ���ַ�����
*/

void AT_Receive_Char(char *c, TickType_t timeout)
{
	xSemaphoreTake(Esp8266_ParseHandler,timeout);
	RingBuffer_ReadByte(&test_RingBuffer,c);
}

/*
 *����1�����ڴ洢AT����Ļ������׵�ַ 
 *����2���洢AT����ȵı�����ַ 
 *����3����ʱʱ��
 *�������ã�����һ�лظ�,ֻ����\r\nǰ�������ַ���
*/
//	static char last_char;
//		if((res == '\n')&&(last_char == '\r'))
//		{
//			xSemaphoreGiveFromISR(Esp8266_ParseHandler,&xHigherPriorityTaskWoken);
//		}
//		last_char = res;
//     }
int UART_Receive_Line(char *buf, int* plen, int timeout)
{
	int Rec_count = 0,len_count = 0,T_count = 0;
	char res;
	 do
	{
		switch(RingBuffer_ReadByte(&test_RingBuffer,&res))
		{
			case Rec_SUCCESS:
			if((res!='\r')&&(res!='\n'))//���λ������н��յ��ַ�������ͨ����\r\n��β
		   {
		     buf[Rec_count++] = res;
		     len_count++;
		   }
			case Rec_FAIL:
				return ERROR;
		}
		T_count++;
		if(T_count>=timeout) return TIMEOUT;
	}while(res!='\n');
	*plen = len_count;
	return TRUE;
}
//
int IS_RecOK(char* buf,int len)
{
	buf[len] = '\0';
	return (strstr(buf,"OK"))?TRUE:FALSE;
}

int IS_RecERROR(char* buf,int len)
{
	buf[len] = '\0';
	return (strstr(buf,"ERROR"))?TRUE:FALSE;
}


/*
 *����1�����ڴ洢�ַ����Ļ������׵�ַ 
 *����2
 *�������ã��ж��Ƿ�ΪAT����Ļظ�
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
 *����1��AT�����ַ���
 *����2����ʱʱ��
 *�������ã�����AT����,���ͳɹ�����ֵΪ1������ʧ�ܷ���ֵΪ0
*/
int AT_SendCmd(char *cmd, int timeout)
{
	if(UART_AT_Send(cmd,strlen(cmd),timeout) != TRUE)
		return ERROR;
	return TRUE;
}

/*
 *����1�����������ṹ���ַ
 *����2����ʱʱ��
 *����ֵ��1��ʾΪ��������������Ϣ��0��ʾAT����ظ�
 *�������ã��ж��Ƿ�Ϊ������������Ϣ��+IPD,2:AB
*/
	/* ���ݸ�ʽ: +IPD,2:ab */
int Get_URC_Obj(char* PaserBuf,int timeout)
{
	enum URC_Read_Status AT_Status = URC_Init_Status;
	int i = 0,len = strlen(PaserBuf),PreT_count,NowT_count,Cur_Len_Pos,m;
	if(len<=0) return URC_FALSE;
	PreT_count = xTaskGetTickCount();
	while(AT_Status != UnURC_Status)
	{
		switch(AT_Status)
		{
			case URC_Init_Status:
				if(i<Recv_Buf_LENGTH)
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
						/*���㳤��*/
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
				if((i<Cur_URC->URC_len)&&(m<Cur_URC->URC_len))
				{
					Cur_URC->URC_Data_handled[m] =  PaserBuf[i];
					m++;
					i++;
				}				
				else AT_Status = URC_Complete_Status;
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
////���� WiFi ģʽ
////AT+CWMODE=3	 softAP+station	mode
//void ESP8266_Mode_Set(void)
//{
//	Usart_SendString(UART4,"AT+CWMODE=3");
//}

////����·����
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

////ESP8266 �豸��Ϊ TCP client ���ӵ�������
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



