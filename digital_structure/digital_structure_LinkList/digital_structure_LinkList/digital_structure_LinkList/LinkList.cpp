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
unsigned int ListLength_L(LinkList L)
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

//算法2.7获取链表中第i个位置的元素，并通过data返回
Status GetElem_L(LinkList L, unsigned char i, Data_Dmains& data)
{
	unsigned char count = 1;
	Lnode* P = L->next;//获取首元结点地址
	while ((count < i) && P)//当位置不相同与结点指针不为空
	{
		count++;
		P = P->next;
	}
	if ((count > i)||!P)return Error;//第i个元素不存在
		data = P->data;//取第i个元素
		return OK;
}

//算法2.8.1按值查找位置，若值存在，返回地址，不存在，返回空指针
Lnode* LocateElem_L(LinkList L, Data_Dmains e)
{
	Lnode* P = L->next;
	while (P && (P->data.ID!= e.ID))
	{
		P = P->next;
	}
	return P;

}

//算法2.8.2按值查找位置，若值存在，返回位置，不存在，返回0
unsigned char LocateElemPostion_L(LinkList L, Data_Dmains e)
{
	Lnode* P = L->next;
	unsigned char count = 1;
	while (P && (P->data.ID != e.ID))//P需要放在逻辑与前面，因为如果P为空，P->data.ID为非法使用，所以P在前会直接进入while？
	{
		P = P->next;
		count++;
	}
	if (P)return count;
	else return 0;
}

//算法2.9在第i个结点之前插入数据为e的新节点
Status ElemInsert_L(LinkList &L,unsigned char i, Data_Dmains Data)
{
	unsigned char count = 0;
	Lnode *P = L;
	Lnode *temp = (Lnode*)malloc(sizeof(Lnode));
	temp->data = Data;
	temp->next = NULL;
	
	while ((count<i-1) && P)
	{
		count++;
		P = P->next;
	}
	if (!P||count>i-1) return Error;
	temp->next = P->next;
	P->next = temp;
	return OK;
}

//算法2.10删除链表中第i个结点，返还其数据并释放内存空间
Status ListDelete_L(LinkList &L,unsigned char i, Data_Dmains &e)
{
	unsigned char count = 0;
	Lnode *P = L,*Q;
	while (count<i-1 && P->next)//这里用P->next做判断条件是为了提前判断下一个结点是否为空，为空就没有必要继续循环了
	{
		count++;
		P = P->next;
	}
	if (!(P->next) || (count > i - 1))return Error;//如果到第i-1个结点发现指针域为空，就置Error。
	Q = P->next;//如果直接P作为判断，指针域为空赋给Q，Q再指向数据域和指针域会出错
	e = Q->data;
	P->next = Q->next;
	free(Q);
	return OK;
}

//算法2.11前插法创建单链表,倒序插入想输入的元素
void CreateList_L(LinkList &L,unsigned char n)
{
	unsigned char count = 0;
	Lnode *P;
	L = (LinkList)malloc(sizeof(Lnode));//先建立一个带结点的单链表
	L->next = NULL;
	for (count = 0;count<n;count++)
	{
		P = (Lnode*)malloc(sizeof(Lnode));//生成新结点
		scanf_s("%d",&(P->data));//输入元素值
		P->next = L->next;
		L->next = P;
	}
}

//算法2.12尾插法创建单链表，正序插入想输入的元素
void CreateList_R(LinkList& L, unsigned char n)
{
	unsigned char count = 0;
	Lnode *R,*P;
	L = (Lnode*)malloc(sizeof(Lnode));
	L->next = NULL;
	R = L;
	for (count = 0; count < n; count++)
	{
		P = (Lnode*)malloc(sizeof(Lnode));
		scanf_s("%d",&(P->data));
		P->next = NULL;
		R->next = P;
		R = P;
	}
	
}

//算法2.13带尾指针循环链表的合并
LinkList LinkList_Connect(LinkList Ta, LinkList Tb)
{
	Lnode* P;
	P = Ta->next;
	Ta->next = Tb->next->next;
	free(Tb->next);
	Tb->next = P;

	return Tb;
}

//算法2.14双向链表元素的位置
DuLnode* GetElem_Du(DuLinkList L,unsigned int i)
{
	unsigned int count = 0;
	DuLnode* P = L;
	while (count<i&&P)
	{
		P = P->next;
		count++;
	}
	if (count > i || !P)return Error;
	else return P;
}

//算法2.15双向链表结点的插入
Status Insert_DuLinkList(DuLinkList &L,unsigned int i, Data_Dmains e)
{
	DuLnode* P,*s = (DuLnode*)malloc(sizeof(DuLnode));
	if (!(P = GetElem_Du(L, i)))return Error;
	s->data = e;
	P->prior->next = s;
	s->prior = P->prior;
	s->next = P;
	P->prior = s;
	return OK;

}

//算法2.16双向链表结点的删除
Status Delete_DuLinkList(DuLinkList &L,unsigned int i, Data_Dmains &e)
{
	DuLnode* P;
	if (!(P = GetElem_Du(L, i)))return Error;
	P->prior->next = P->next;
	P->next->prior = P->prior;
	e = P->data;
	free(P);
	return OK;
}

//算法2.17无序链表的合并
void LinkList_Union(LinkList &La, LinkList Lb)
{
	Data_Dmains e,g;
	unsigned int LengthA = 0, LengthB = 0, count_A = 0, count_B = 0;
	LengthA = ListLength_L(La);
	LengthB = ListLength_L(Lb);

	for (count_B = 0; count_B < LengthB; count_B++)
	{
		for (count_A = 0; count_A < LengthA; count_A++)
		{
			GetElem_L(Lb, count_B, e);
			if (!LocateElem_L(La,e))ElemInsert_L(La, ++LengthA,e);
		}
		
	}

}

//算法2.18有序链表的合并
void MergeList_L(LinkList &La, LinkList Lb)
{
	LinkList Lc = La;
	Lnode *pa, *pb,*pc;
	pa = La->next;
	pb = Lb->next;
	pc = Lc;

	while (pa && pb)
	{
		if (pa->data.ID <= pb->data.ID)
		{
			pc->next = pa;
			pc = pa;
			pa = pa->next;
		}
		else
		{
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}
	}
	pc->next = pa ? pa : pb;
	free(Lb);
}

//作业1多项式运算




//作业2稀疏多项式的运算




//作业3图书信息管理系统
