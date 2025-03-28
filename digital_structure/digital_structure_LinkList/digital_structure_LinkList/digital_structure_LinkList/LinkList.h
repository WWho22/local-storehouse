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


Status InitList(LinkList& L);//�㷨2.6����ĳ�ʼ�����ڴ����һ��ռ��ͷ��㣬�ٰ�ͷ���ָ�����ÿ�
Status IsEmpty(LinkList L);//�����㷨1���ж������Ƿ�Ϊ�ձ�
Status DestoryList_Link(LinkList& L);//�����㷨2�����ٵ�����
Status ClearList(LinkList& L);//�����㷨3����յ�����
unsigned int ListLength_L(LinkList L);//�����㷨4��������ı�
Status GetElem_L(LinkList L, unsigned char i, Data_Dmains& data);//�㷨2.7��ȡ�����е�i��λ�õ�Ԫ�أ���ͨ��data����
Lnode* LocateElem_L(LinkList L, Data_Dmains e);//�㷨2.8.1��ֵ����λ�ã���ֵ���ڣ����ص�ַ�������ڣ����ؿ�ָ��
unsigned char LocateElemPostion_L(LinkList L, Data_Dmains e);//�㷨2.8.2��ֵ����λ�ã���ֵ���ڣ�����λ�ã������ڣ�����0
Status ElemInsert_L(LinkList& L, unsigned char i, Data_Dmains Data);//�㷨2.9�ڵ�i�����֮ǰ��������Ϊe���½ڵ�
Status ListDelete_L(LinkList& L, unsigned char i, Data_Dmains& e);//�㷨2.10ɾ�������е�i����㣬���������ݲ��ͷ��ڴ�ռ�
void CreateList_L(LinkList& L, unsigned char n);//�㷨2.11ǰ�巨����������,��������������Ԫ��
void CreateList_R(LinkList& L, unsigned char n);//�㷨2.12β�巨������������������������Ԫ��
LinkList LinkList_Connect(LinkList Ta, LinkList Tb);//�㷨2.13��βָ��ѭ������ĺϲ�
DuLnode* GetElem_Du(DuLinkList L, unsigned int i);//�㷨2.14˫������Ԫ�ص�λ��
Status Insert_DuLinkList(DuLinkList& L, unsigned int i, Data_Dmains e);//�㷨2.15˫������Ĳ���
Status Delete_DuLinkList(DuLinkList& L, unsigned int i, Data_Dmains& e);//�㷨2.16˫�������ɾ��

#endif // !__LINKLIST_H__
