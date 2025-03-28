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
		free(L.elem);//if(L.elem) = if(L.elem != NULL)是一个判空操作，后续释放L.elem的内存空间
		L.elem = NULL;//不赋值空指针，elem会变成野指针
	}
	return OK;
}

Status ClearList_Sq(SqList& L)
{
	L.length = 0;//清空线性表
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

//算法2.2顺序表的取值（根据位置i取顺序表中相应位置的元素）,随机取元素，链表不能随机取元素
Status DestroyList_Sq(SqList L,unsigned int i, Polynomial &e)
{
	if (i<1 || i>L.length)
		return Error;
	e = L.elem[i-1];
	return	OK;
}

//算法2.3顺序表的查找（在顺序表中查找与指定值e相同的元素的位置）
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

//算法2.4顺序表的插入
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

//算法2.5顺序表的删除,删除位序为i的元素，并将其存放在e中
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

//算法2.6合并两个有序表
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

//作业1多项式运算



//作业2稀疏多项式的运算


//作业3图书信息管理系统