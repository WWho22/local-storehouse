#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "LinkList.h"

//算法2.6链表的初始化，内存分配一块空间给头结点，再把头结点指针域置空
Status InitList(LinkList &L)
{
	L = (LinkList)malloc(sizeof(Lnode));
	L->next = NULL;
	return OK;
}

//补充算法1，判断链表是否为空表
Status IsEmpty(LinkList L)
{
	//0为非空，1为空
	if (L->next)//非空
		return 0;
	else
	{
		return 1;
	}
}

//补充算法2，销毁单链表
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

//补充算法3，清空单链表
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

//补充算法4，求单链表的表长
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


