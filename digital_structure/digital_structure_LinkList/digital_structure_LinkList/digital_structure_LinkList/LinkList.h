#pragma once
#ifndef __LINKLIST_H__
#define __LINKLIST_H__
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
    int ID;
}Data_Dmains;

typedef struct Lnode
{
	Data_Dmains data;
	struct Lnode* next;
}Lnode,*LinkList;

typedef struct DuLnode
{
	Data_Dmains data;
	struct DuLnode* next,*prior;
}DuLnode,*DuLinkList;


Status InitList(LinkList& L);//算法2.6链表的初始化，内存分配一块空间给头结点，再把头结点指针域置空
Status IsEmpty(LinkList L);//补充算法1，判断链表是否为空表
Status DestoryList_Link(LinkList& L);//补充算法2，销毁单链表
Status ClearList(LinkList& L);//补充算法3，清空单链表
unsigned int ListLength_L(LinkList L);//补充算法4，求单链表的表长
Status GetElem_L(LinkList L, unsigned char i, Data_Dmains& data);//算法2.7获取链表中第i个位置的元素，并通过data返回
Lnode* LocateElem_L(LinkList L, Data_Dmains e);//算法2.8.1按值查找位置，若值存在，返回地址，不存在，返回空指针
unsigned char LocateElemPostion_L(LinkList L, Data_Dmains e);//算法2.8.2按值查找位置，若值存在，返回位置，不存在，返回0
Status ElemInsert_L(LinkList& L, unsigned char i, Data_Dmains Data);//算法2.9在第i个结点之前插入数据为e的新节点
Status ListDelete_L(LinkList& L, unsigned char i, Data_Dmains& e);//算法2.10删除链表中第i个结点，返还其数据并释放内存空间
void CreateList_L(LinkList& L, unsigned char n);//算法2.11前插法创建单链表,倒序插入想输入的元素
void CreateList_R(LinkList& L, unsigned char n);//算法2.12尾插法创建单链表，正序插入想输入的元素
LinkList LinkList_Connect(LinkList Ta, LinkList Tb);//算法2.13带尾指针循环链表的合并
DuLnode* GetElem_Du(DuLinkList L, unsigned int i);//算法2.14双向链表元素的位置
Status Insert_DuLinkList(DuLinkList& L, unsigned int i, Data_Dmains e);//算法2.15双向链表的插入
Status Delete_DuLinkList(DuLinkList& L, unsigned int i, Data_Dmains& e);//算法2.16双向链表的删除

#endif // !__LINKLIST_H__
