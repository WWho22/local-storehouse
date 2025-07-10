#include "includes.h"


char AT_WIFI_MODE[11] = "AT+CWMODE=3";
char AT_CONNECT[10];

enum General_Status{OK_Status,ERROR_Status};

/*
 *����1��Ҫ���͵��ַ��� 
 *����2���ַ�����ָ������
 *����3����ʱʱ��
 *�������ã�����ָ�����ȵ��ַ���
*/
int UART_AT_Send(char *buf, int len, int timeout)
{
	int send_count = 0,T_count = 0;
	if(len<=0) return ERROR;
	while(send_count<len)
	{
	  T_count += Usart_SendByte(UART4,buf[send_count],timeout);
	  if(T_count >= timeout)return TIMEOUT;
	  send_count++;
	}
	return OK;
}

/*
 *����1�����ڴ洢AT����Ļ������׵�ַ 
 *����2���洢AT����ȵı�����ַ 
 *����3����ʱʱ��
 *�������ã�����һ��AT����,ֻ����\r\nǰ�������ַ���
*/
int UART_AT_Receive_Line(char *buf, int* plen, int timeout)
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
	return OK;
}

int IS_RecOK(char* buf,int len)
{
//	if(strcmp("OK",*str) == 0)
//		return 1;
//	else return 0;
	buf[len] = '\0';
	return (strstr(buf,"OK"))?1:0;
}

int IS_RecERROR(char* buf,int len)
{
	buf[len] = '\0';
	return (strstr(buf,"ERROR"))?1:0;
}


/*
 *����1�����ڴ洢�ַ����Ļ������׵�ַ 
 *����2
 *�������ã�����ָ�����ȵ��ַ���
*/
int UART_AT_Receive(char *buf, int len, int timeout)
{
 	int Rec_count = 0,T_count = 0;
	while(Rec_count<len)
	{
		if(RingBuffer_ReadByte(&test_RingBuffer,&buf[Rec_count++])!=Rec_SUCCESS)
			return ERROR;
		if((++T_count)>=timeout)
			return TIMEOUT;
	}
	return OK;
}

/*
 *����1��AT�����ַ���
 *����2����ʱʱ��
 *�������ã�����AT����,���ͳɹ�����ֵΪ1������ʧ�ܷ���ֵΪ0
*/
int AT_SendCmd(char *cmd, int timeout)
{
	if(UART_AT_Send(cmd,strlen(cmd),timeout) != OK)
		return ERROR;
	return OK;
}

/*
 *����1�����λ������ṹ���ַ
 *����2����ʱʱ��
 *�������ã��ж��Ƿ�Ϊ������������Ϣ��+IPD,2:AB
*/
int Get_URC_Obj(RingBuffer* pRingBuffer,int timeout)
{
	enum URC_Read_Status
    {Init_Status,Len_Status,Data_Status}; 
	
	while(timeout)
	{
		
	}
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



