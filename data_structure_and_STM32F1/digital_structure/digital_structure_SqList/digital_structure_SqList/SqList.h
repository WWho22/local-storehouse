#pragma once
#ifndef __SQLIST_H__
#define __SQLIST_H__
//��ʼ��˳���
#define MaxSize 100
//�������״̬����
#define True 1
#define False 0
#define OK 1
#define Error 0
#define Infeasible -1
#define Overflow -2
//Status �Ǻ��������ͣ������Ǻ������״̬����
typedef int Status;
typedef char ElemType;

typedef struct
{
	int Coefficient;//ϵ��
	int index;//ָ��
}Polynomial;

typedef struct
{
	Polynomial* elem;//����ʽ����ָ��
	unsigned int length;//˳���
}SqList;


Status InitList_Sq(SqList& L);
Status DestroyList_Sq(SqList& L);
Status ClearList_Sq(SqList& L);
int GetLength_Sq(SqList L);
Status IsEmpty_Sq(SqList L);
Status DestroyList_Sq(SqList L, int i, Polynomial& e);
Status LocateElem(SqList L, Polynomial e);
Status ListInsert_Sq(SqList& L, int i, Polynomial e);


#endif // !1
