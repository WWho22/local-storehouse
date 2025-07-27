#ifndef  __ESP8266_H__
#define  __ESP8266_H__

#define URC_LENGTH 20
#define Recv_Buf_LENGTH 64


//命令执行结果
#define TRUE       1
#define FALSE      0
#define TIMEOUT   -1

//URC命令判断结果
#define URC_TRUE       1
#define URC_FALSE      0
#define URC_TIMEOUT   -1

//AT命令判断结果
#define AT_OK         1
#define AT_ERROR      0
#define AT_TIMEOUT   -1

enum URC_Read_Status
    {URC_Init_Status,URC_Len_Status,URC_Data_Status,URC_Complete_Status,UnURC_Status}; 
		
enum General_Status{Gen_OK_Status,Gen_ERROR_Status};

enum URC_Handle_Status
    {URC_Start_Status,URC_OLED_Status,URC_LED_Status,URC_Temperature_Status}; 

enum AT_Read_Status
    {AT_Start_Status,AT_OK_Status,AT_ERROR_Status}; 
		
enum URC_Rec_Status
    {Full_Rec,Missing_Rec};
	
	typedef struct 
{
   char URC_Data_handled[URC_LENGTH];
	 int URC_Data_handled_length;
	 int Effective_word_len;
	 int URC_len;
	 enum URC_Rec_Status URC_Rec;
	 
}URC_handled;


void URC_handler_Init(void);
void UART4_Lock_Init(void);
void UART4_Unlock(void);
void Esp8266_Parse_Init(void);
void SetATStatus(int status);
int GetATStatus(void);
int UART_AT_Send(const char *buf, int len, int timeout);
int UART_AT_WIFI_CONNECT_Send(const char *buf,char* SSID,char* password,int timeout);
int UART_AT_SERVER_CONNECT_Send(const char *buf,char* SAddr ,char* Port,int timeout);
void AT_Receive_Char(char *c, TickType_t timeout);
int UART_Receive_Line(char *buf, int* plen, int timeout);
int UART_AT_Receive(char *buf, int len, int timeout);
int AT_SendCmd(char *cmd, int timeout);
int Get_URC_Obj(char* PaserBuf,int timeout,int len);
int Get_CIPSEND_Result(char* buf);
int Get_URC_Result(char* buf);
int Is_AT_Response(const char* response);
void URC_handle(void);
#endif



