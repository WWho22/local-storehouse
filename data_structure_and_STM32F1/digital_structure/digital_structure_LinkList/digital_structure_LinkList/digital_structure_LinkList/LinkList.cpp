#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "LinkList.h"

//�㷨2.6����ĳ�ʼ�����ڴ����һ��ռ��ͷ��㣬�ٰ�ͷ���ָ�����ÿ�
Status InitList(LinkList &L)
{
	L = (LinkList)malloc(sizeof(Lnode));
	L->next = NULL;
	return OK;
}

//�����㷨1���ж������Ƿ�Ϊ�ձ�
Status IsEmpty(LinkList L)
{
	//0Ϊ�ǿգ�1Ϊ��
	if (L->next)//�ǿ�
		return 0;
	else
	{
		return 1;
	}
}

//�����㷨2�����ٵ�����
Status DestoryList_Link(LinkList &L)
{
	LinkList P;
	while (L)
	{
		P = L;
		L = L->next;
		free(P);
	}
	return OK;
}

//�����㷨3����յ�����
Status ClearList(LinkList &L)
{
	Lnode *P,*Q;
	P = L->next;
	while (P != NULL)
	{
		Q = P->next;
		free(P);
		P = Q;
	}
	L->next = NULL;
	return OK;
}

//�����㷨4��������ı�
unsigned int ListLength(LinkList L)
{
	Lnode* P;
	unsigned int length = 0;
	P = L->next;
	while (P)
	{
		length++;
		P = P->next;
	}
	return length;
}


