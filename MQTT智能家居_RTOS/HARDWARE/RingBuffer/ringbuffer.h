#ifndef  __RINGBUFFER_H__
#define  __RINGBUFFER_H__

#define rt_size_t int16_t


#define Rec_SUCCESS 0
#define Rec_FAIL    -1

/* 环形缓冲区控制块 */
typedef struct 
{
    char* buffer_ptr;			//缓冲区指针
    uint16_t read_index : 15;	//读取位置
    uint16_t write_index : 15;	//写入位置
    int16_t buffer_size;			//缓冲区大小
}RingBuffer;



void RingBuffer_Init(RingBuffer* pRingBuffer,char* ptr,int16_t len);
void RingBuffer_WriteByte(RingBuffer* pRingBuffer,unsigned char ch);
int RingBuffer_ReadByte(RingBuffer* pRingBuffer,unsigned char* pch);


#endif



