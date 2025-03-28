#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "SqList.h"





Status InitList_Sq(SqList& L)
{
	L.elem = (Polynomial*)malloc(sizeof(Polynomial)* MaxSize);
	if (!(L.elem))exit(Overflow);
	L.length = 0;
	return OK;
}

Status DestroyList_Sq(SqList& L)
{
	if (L.elem)
	{
		free(L.elem);//if(L.elem) = if(L.elem != NULL)��һ���пղ����������ͷ�L.elem���ڴ�ռ�
		L.elem = NULL;//����ֵ��ָ�룬elem����Ұָ��
	}
	return OK;
}

Status ClearList_Sq(SqList& L)
{
	L.length = 0;//������Ա�
	return OK;
}

int GetLength_Sq(SqList L)
{
	return L.length;
}

Status IsEmpty_Sq(SqList L)
{
	if (L.length == 0)
		return True;
	else return False;
}

//�㷨2.2˳����ȡֵ������λ��iȡ˳�������Ӧλ�õ�Ԫ�أ�,���ȡԪ�أ����������ȡԪ��
Status DestroyList_Sq(SqList L,unsigned int i, Polynomial &e)
{
	if (i<1 || i>L.length)
		return Error;
	e = L.elem[i-1];
	return	OK;
}

//�㷨2.3˳���Ĳ��ң���˳����в�����ָ��ֵe��ͬ��Ԫ�ص�λ�ã�
Status LocateElem(SqList L, Polynomial e)
{
	unsigned int count = 0;
	for (count = 0; count < L.length; count++)
	{
		if (L.elem[count].Coefficient == e.Coefficient && L.elem[count].index == e.index)
			return count+1;
	}
	return False;
}

//�㷨2.4˳���Ĳ���
Status ListInsert_Sq(SqList &L,unsigned int i, Polynomial e)
{
	unsigned int count = 0;
	if (i<1 || i>L.length) return Error;
	if (L.length == MaxSize) return Error;
	for (count = L.length-1;count>=i-1;count--)
	{
		L.elem[count+1].Coefficient = L.elem[count].Coefficient;
		L.elem[count+1].index = L.elem[count].index;
	}
	L.elem[i - 1].Coefficient = e.Coefficient;
	L.elem[i - 1].index = e.index;
	L.length++;
	return OK;
}

//�㷨2.5˳����ɾ��,ɾ��λ��Ϊi��Ԫ�أ�����������e��
Status ListDelete_Sq(SqList &L,unsigned int i, Polynomial &e)
{
	unsigned int count = 0;
	if (i<1 || i>L.length) return Error;
	if (L.length == 0)return Error;
	e.Coefficient = L.elem[i - 1].Coefficient;
	e.index = L.elem[i - 1].index;
	for (count = i-1;count<L.length-1;count++)
	{
		L.elem[count].Coefficient = L.elem[count+1].Coefficient;
		L.elem[count].index = L.elem[count+1].index;
	}
	L.length--;
	return OK;
}

//�㷨2.6�ϲ����������
void MergeList_Sq(SqList La, SqList Lb, SqList &Lc)
{
	Polynomial* pa, * pb, * pc,*pa_Last,*pb_Last;
	Lc.length = La.length + Lb.length;
	Lc.elem = (Polynomial*)malloc(sizeof(Polynomial) * Lc.length);
	pa = La.elem;
	pb = Lb.elem;
	pc = Lc.elem;
	pa_Last = pa + (La.length - 1);
	pb_Last = pb + (Lb.length - 1);
	while ((pa <= pa_Last)&& (pb <= pb_Last))
	{
		if (pa->index <= pb->index)
		{
			pc->Coefficient = pa->Coefficient;
			pc->index = pa->index;
			pc++;
			pa++;
		}
		else if (pa->index > pb->index)
		{
			pc->Coefficient = pb->Coefficient;
			pc->index = pc->index;
			pc++;
			pb++;
		}
	}
	while (pa <= pa_Last)
	{
		pc->Coefficient = pa->Coefficient;
		pc->index = pa->index;
		pc++;
		pa++;
	}
	while (pb <= pb_Last)
	{
		pc->Coefficient = pb->Coefficient;
		pc->index = pc->index;
		pc++;
		pb++;
	}
}

//��ҵ1����ʽ����



//��ҵ2ϡ�����ʽ������


//��ҵ3ͼ����Ϣ����ϵͳ