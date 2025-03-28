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

//�㷨2.7��ȡ�����е�i��λ�õ�Ԫ�أ���ͨ��data����
Status GetElem_L(LinkList L, unsigned char i, Data_Dmains& data)
{
	unsigned char count = 1;
	Lnode* P = L->next;//��ȡ��Ԫ����ַ
	while ((count < i) && P)//��λ�ò���ͬ����ָ�벻Ϊ��
	{
		count++;
		P = P->next;
	}
	if ((count > i)||!P)return Error;//��i��Ԫ�ز�����
		data = P->data;//ȡ��i��Ԫ��
		return OK;
}

//�㷨2.8.1��ֵ����λ�ã���ֵ���ڣ����ص�ַ�������ڣ����ؿ�ָ��
Lnode* LocateElem_L(LinkList L, Data_Dmains e)
{
	Lnode* P = L->next;
	while (P && (P->data.ID!= e.ID))
	{
		P = P->next;
	}
	return P;

}

//�㷨2.8.2��ֵ����λ�ã���ֵ���ڣ�����λ�ã������ڣ�����0
unsigned char LocateElemPostion_L(LinkList L, Data_Dmains e)
{
	Lnode* P = L->next;
	unsigned char count = 1;
	while (P && (P->data.ID != e.ID))//P��Ҫ�����߼���ǰ�棬��Ϊ���PΪ�գ�P->data.IDΪ�Ƿ�ʹ�ã�����P��ǰ��ֱ�ӽ���while��
	{
		P = P->next;
		count++;
	}
	if (P)return count;
	else return 0;
}

//�㷨2.9�ڵ�i�����֮ǰ��������Ϊe���½ڵ�
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

//�㷨2.10ɾ�������е�i����㣬���������ݲ��ͷ��ڴ�ռ�
Status ListDelete_L(LinkList &L,unsigned char i, Data_Dmains &e)
{
	unsigned char count = 0;
	Lnode *P = L,*Q;
	while (count<i-1 && P->next)//������P->next���ж�������Ϊ����ǰ�ж���һ������Ƿ�Ϊ�գ�Ϊ�վ�û�б�Ҫ����ѭ����
	{
		count++;
		P = P->next;
	}
	if (!(P->next) || (count > i - 1))return Error;//�������i-1����㷢��ָ����Ϊ�գ�����Error��
	Q = P->next;//���ֱ��P��Ϊ�жϣ�ָ����Ϊ�ո���Q��Q��ָ���������ָ��������
	e = Q->data;
	P->next = Q->next;
	free(Q);
	return OK;
}

//�㷨2.11ǰ�巨����������,��������������Ԫ��
void CreateList_L(LinkList &L,unsigned char n)
{
	unsigned char count = 0;
	Lnode *P;
	L = (LinkList)malloc(sizeof(Lnode));//�Ƚ���һ�������ĵ�����
	L->next = NULL;
	for (count = 0;count<n;count++)
	{
		P = (Lnode*)malloc(sizeof(Lnode));//�����½��
		scanf_s("%d",&(P->data));//����Ԫ��ֵ
		P->next = L->next;
		L->next = P;
	}
}

//�㷨2.12β�巨������������������������Ԫ��
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

//�㷨2.13��βָ��ѭ������ĺϲ�
LinkList LinkList_Connect(LinkList Ta, LinkList Tb)
{
	Lnode* P;
	P = Ta->next;
	Ta->next = Tb->next->next;
	free(Tb->next);
	Tb->next = P;

	return Tb;
}

//�㷨2.14˫������Ԫ�ص�λ��
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

//�㷨2.15˫��������Ĳ���
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

//�㷨2.16˫���������ɾ��
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

//�㷨2.17��������ĺϲ�
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

//�㷨2.18��������ĺϲ�
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

//��ҵ1����ʽ����




//��ҵ2ϡ�����ʽ������




//��ҵ3ͼ����Ϣ����ϵͳ
