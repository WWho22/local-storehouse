#ifndef  __ESP8266_H__
#define  __ESP8266_H__

#define OK       1
#define ERROR    0
#define TIMEOUT -1

struct at_urc
{
    const char *cmd_prefix;
    const char *cmd_suffix;
//    void (*func)(struct at_client *client, const char *data, rt_size_t size);
};
typedef struct at_urc *at_urc_t;

int UART_AT_Send(char *buf, int len, int timeout);
int UART_AT_Receive_Line(char *buf, int* plen, int timeout);
int UART_AT_Receive(char *buf, int len, int timeout);
int AT_SendCmd(char *cmd, int timeout);
#endif



