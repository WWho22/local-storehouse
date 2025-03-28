#pragma once
#ifndef __SQLIST_H__
#define __SQLIST_H__
//初始化顺序表长
#define MaxSize 100
//函数结果状态代码
#define True 1
#define False 0
#define OK 1
#define Error 0
#define Infeasible -1
#define Overflow -2
//Status 是函数的类型，其结果是函数结果状态代码
typedef int Status;
typedef char ElemType;

typedef struct
{
	int Coefficient;//系数
	int index;//指数
}Polynomial;

typedef struct
{
	Polynomial* elem;//多项式类型指针
	unsigned int length;//顺序表长
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
