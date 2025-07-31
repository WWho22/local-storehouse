#include "includes.h"
enum URC_Handle_Status URC_Status;//������URC�ظ��Ĵ���״̬
URC_handled* Cur_URC;
static platform_mutex_t UsartSend;
static platform_mutex_t AT_DataRead;
static uint8_t recv_buf[Recv_Buf_LENGTH];
static int g_at_status;
static char* g_cur_cmd;

void URC_handler_Init(void)
{
	Cur_URC -> URC_Data_handled_length = URC_LENGTH;
}

void AT_DataRead_Lock_Init(void)
{
	 platform_mutex_init(&AT_DataRead);
	 platform_mutex_lock(&AT_DataRead);
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

void AT_DataRead_Unlock(void)
{
	platform_mutex_unlock(&AT_DataRead);
}

void SetATStatus(int status)
{
	g_at_status = status; 
}

int GetATStatus(void)
{
	return g_at_status;
}

int AT_Data_Read(unsigned char *buf, int timeout)
{
	int err;
	do
	{
		if(RingBuffer_ReadByte(&AT_Data_RingBuffer,buf) == 0)
			return TRUE;
	  else 
		{
			err = platform_mutex_lock_timeout(&AT_DataRead,timeout);
			if(err == pdFALSE)
				return TIMEOUT;
		}
	}while(1);
	
}

/*
 *����1��Ҫ���͵����ݰ�
 *����2����ʱʱ��(ms)
 *�������ã�����AT�����ַ������ַ�������<=0������ʧ��FALSE�����ͳɹ�TRUE����ʱ����TIMEOUT
*/
int UART_AT_Data_Send(const char *buf, int timeout)
{
	int ret,err,len;
	len = strlen(buf);
	if(len<=0) return FALSE;
   Usart_SendString(UART4,buf);
	 ret = platform_mutex_lock_timeout(&UsartSend,timeout);

		if( ret == pdTRUE)
	{
		 err = GetATStatus();
     return err;
	}
	else return TIMEOUT;
}


/*
 *����1��Ҫ���͵�AT����
 *����2����ʱʱ��(ms)
 *�������ã�����AT�����ַ������ַ�������<=0������ʧ��FALSE�����ͳɹ�TRUE����ʱ����TIMEOUT
*/
int UART_AT_Send(const char *buf, int timeout)
{
	int ret,err,len;
	len = strlen(buf);
	if(len<=0) return FALSE;
   Usart_SendString(UART4,buf);
	 Usart_SendString(UART4,"\r\n");
	 strcpy(g_cur_cmd,buf);
	 ret = platform_mutex_lock_timeout(&UsartSend,timeout);

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
 *����1�����ڴ洢�ַ����Ļ������׵�ַ 
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
 *����1�����ڴ洢�ַ����Ļ������׵�ַ 
 *�������ã��ж��Ƿ�ΪAT��������Ļظ�
*/
int Get_CIPSEND_Result(char* buf)
{
	if(g_cur_cmd&&(strstr(g_cur_cmd,"AT+CIPSEND="))&&(buf[0] == '>'))
		return TRUE;
	else return FALSE;
}

/*
 *����1�����ڴ洢�ַ����Ļ������׵�ַ 
 *�������ã��ж��Ƿ�ΪAT����Ļظ�
*/
int Get_URC_Result(char* buf)
{
	if(strstr(buf,"+IPD,"))
		return TRUE;
	else return FALSE;
}

/*
 *����1��AT�����ַ���
 *����2����ʱʱ��
 *�������ã�����AT����,���ͳɹ�����ֵΪ1������ʧ�ܷ���ֵΪ0
*/
int AT_SendCmd(char *cmd, int timeout)
{
	if(UART_AT_Send(cmd,timeout) != TRUE)
		return ERROR;
	return TRUE;
}

/*
 *����1�����������ṹ���ַ
 *����2����ʱʱ��
 *����3���յ����ַ������ȣ�����ò�������ΪĳЩ����Ļظ����̲�һ��ǰһ��̫�����ַ���û����ȫ����һ�̵���գ�
 *����ֵ��1��ʾΪ��������������Ϣ��0��ʾAT����ظ�
 *�������ã��ж��Ƿ�Ϊ������������Ϣ��+IPD,2:AB
*/
	/* ���ݸ�ʽ: +IPD,2:ab */
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
			//�ȶ��Ƿ����ݰ���ǰ׺Ϊ+IPD,
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
			//��ȡ���ݰ�����
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
			//��ȡ���ݰ��ڵ�����
			case URC_Data_Status:
				if((i<Cur_URC->URC_len))
				{
					RingBuffer_WriteByte(&AT_Data_RingBuffer,PaserBuf[i]);
					AT_DataRead_Unlock();
					i++;
				}				
				else
				{					
					AT_Status = URC_Complete_Status;
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



