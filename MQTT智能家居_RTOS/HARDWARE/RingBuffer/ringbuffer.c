#include "includes.h"


//���λ�������ʼ��
void RingBuffer_Init(RingBuffer* pRingBuffer,int16_t len)
{
	pRingBuffer->read_index = 0;
	pRingBuffer->write_index = 0;
	pRingBuffer->buffer_size = len;
}


//���λ�����д������
/*
 *����1 �������Ļ��λ���������
 *����2 �������һ���ֽ�����
 *�������ã����λ�����д��һ���ֽڵ����ݣ������λ������������ǿ�ͷ��������
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


//���λ�������������
/*
 *����1 �������Ļ��λ���������
 *����2 ��������ַ
 *�������ã��ӻ��λ���������һ���ֽڵ����ݣ��洢�������Ļ������С�����ȡ�ɹ�����0����ȡʧ�ܣ�����-1
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




