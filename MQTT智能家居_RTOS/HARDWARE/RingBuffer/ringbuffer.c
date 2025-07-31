#include "includes.h"


//环形缓冲区初始化
void RingBuffer_Init(RingBuffer* pRingBuffer,int16_t len)
{
	pRingBuffer->read_index = 0;
	pRingBuffer->write_index = 0;
	pRingBuffer->buffer_size = len;
}


//环形缓冲区写入数据
/*
 *参数1 所操作的环形缓冲区对象
 *参数2 所填入的一个字节数据
 *函数作用：向环形缓冲区写入一个字节的数据，若环形缓冲区满，覆盖开头已有数据
 */
void RingBuffer_WriteByte(RingBuffer* pRingBuffer,unsigned char ch)
{
	int i = (pRingBuffer->write_index+1)%pRingBuffer->buffer_size;
	if(i!=pRingBuffer->read_index)
	{
		pRingBuffer->buffer_ptr[pRingBuffer->write_index] = ch;
		pRingBuffer->write_index = i;
	}
}


//环形缓冲区读出数据
/*
 *参数1 所操作的环形缓冲区对象
 *参数2 缓冲区地址
 *函数作用：从环形缓冲区读出一个字节的数据，存储到给定的缓冲区中。若读取成功返回0，读取失败，返回-1
 */
int RingBuffer_ReadByte(RingBuffer* pRingBuffer,unsigned char* pch)
{
	if(pRingBuffer->read_index == pRingBuffer->write_index)
	{
		return -1;
	}
	else
	{
		*pch = pRingBuffer->buffer_ptr[pRingBuffer->read_index];
//		printf("%c",*pch);
		pRingBuffer->read_index = ((pRingBuffer->read_index)+1)%(pRingBuffer->buffer_size);
		return 0;
	}

}




