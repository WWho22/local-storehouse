#pragma once
#ifndef __LINKLIST_H__
#define __LINKLIST_H__
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
	char ID;
}Data_Dmains;

typedef struct Lnode
{
	Data_Dmains data;
	struct Lnode* next;
}Lnode,*LinkList;


#endif // !__LINKLIST_H__
