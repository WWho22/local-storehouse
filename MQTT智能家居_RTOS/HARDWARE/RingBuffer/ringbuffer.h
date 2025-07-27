#ifndef  __RINGBUFFER_H__
#define  __RINGBUFFER_H__

#define rt_size_t int16_t


#define Rec_SUCCESS 0
#define Rec_FAIL    -1

/* ���λ��������ƿ� */
typedef struct 
{
    char* buffer_ptr;			//������ָ��
    uint16_t read_index : 15;	//��ȡλ��
    uint16_t write_index : 15;	//д��λ��
    int16_t buffer_size;			//��������С
}RingBuffer;



void RingBuffer_Init(RingBuffer* pRingBuffer,char* ptr,int16_t len);
void RingBuffer_WriteByte(RingBuffer* pRingBuffer,unsigned char ch);
int RingBuffer_ReadByte(RingBuffer* pRingBuffer,unsigned char* pch);


#endif



