/*
 * @Author: jiejie
 * @Github: https://github.com/jiejieTop
 * @Date: 2020-01-10 23:45:59
 * @LastEditTime: 2020-04-25 17:50:58
 * @Description: the code belongs to jiejie, please keep the author information and source code according to the license.
 */
 
#include "mqtt_log.h"
#include "nettype_tcp.h"
#include "platform_net_socket.h"
#include "includes.h"

/*
return <0 err
return =0 ok
*/



int platform_net_socket_connect(const char *host, const char *port, int proto)
{
	char cmd[30];
	//配置 WiFi 模式
	if(UART_AT_Send("AT+CWMODE=3",2000)!= 1)
	{
		return -1;
	}
	//连接路由器
	if(UART_AT_Send("AT+CWJAP=\""WIFI_Name"\",\""WIFI_Password"\"",2000)!=1)
	{
		return -1;
	}
	//判断是TCP模式还是UDP模式
	if(proto == PLATFORM_NET_PROTO_TCP)
	{
		sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%s",host,port);
	}
	else
	{
		sprintf(cmd,"AT+CIPSTART=\"UDP\",\"%s\",%s",host,port);
	}
	//ESP8266 设备作为 TCP client 连接到服务器
	if(UART_AT_Send(cmd,2000)!=1)
	{
		return -1;
	}
    return 0;
}

#if 0
int platform_net_socket_recv(int fd, void *buf, size_t len, int flags)
{
    return 0;
}
#endif

int platform_net_socket_recv_timeout(int fd, unsigned char *buf, int len, int timeout)
{
	int recv_count = 0;
	  while(recv_count<len)
		{
			if(AT_Data_Read(buf,timeout) == TIMEOUT)
			{
				return 0;
			}
			recv_count++;
		}
    return len;
}

#if 0
int platform_net_socket_write(int fd, void *buf, size_t len)
{
    return 0;
}
#endif

int platform_net_socket_write_timeout(int fd, unsigned char *buf, int len, int timeout)
{
	int err;
	char cmd[30];	
	sprintf(cmd,"AT+CIPSEND=%d",len);
	err = UART_AT_Send(cmd,timeout)!= 1;
		if(err)
	{
		printf("err is %d",err);
		return err;
	}
		if(UART_AT_Data_Send((char *)buf,timeout)!= 1)
	{
		return -1;
	}
    return 0;
}

int platform_net_socket_close(int fd)
{
	//断开TCP连接
    return UART_AT_Send("AT+CIPCLOSE",2000);
}

#if 0
int platform_net_socket_set_block(int fd)
{
    return 0;
}

int platform_net_socket_set_nonblock(int fd)
{
    return 0;
}

int platform_net_socket_setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
    return 0;
}
#endif
