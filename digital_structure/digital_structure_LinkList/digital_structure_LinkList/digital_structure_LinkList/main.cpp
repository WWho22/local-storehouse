#include "stdio.h"
#include "string.h"
#include <iostream>
#include <cmath>
#include <cstdio>

using namespace std;
double a = 3.14;

void swap(int x, int y)
{
	cout << "swap�����ڽ���ǰ��" << x << " " << y << endl;
	int t = x; x = y; y = t;
	cout << "swap�����ڽ�����" << x << " " << y << endl;
}
int main()
{
	//double a = 3.25 * 3;
	//double b = a + 1.112;
	//cout << "a contains:" << a << endl;
	//cout << "b contains:" << b << endl;

	//double c = a + b * a;
	//cout << "c contains:" << c << endl;
	//std::cout << "I love you";
	
	//double a;
	//cout << "Type a number:";
	//cin >> a;
	//{
	//	int a = 1;//int a �������ⲿ�������double a
	//	cout << "Local Number:" << a << endl;
	//}
	//cout << "You Type Number:" << a << endl;

	//int i = 0;
	//for (int i=0;i<4;i++)
	//{
	//	cout << i << endl;
	//}
	//cout << "i contains:" << i << endl;

	//for (i = 0;i<4;i++)
	//{
	//	for (int i = 0;i<4;i++)
	//	{
	//		cout << i << " ";
	//	}
	//	cout << endl;
	//}

	//for (int a = 0;a<4;a++)
	//{
	//	cout << ::a << endl;//::��ȫ�ֱ����޶�
	//}



	//double a = 3.14;
	//double& b = a;//b��a�ı�����b����a
	//b = 90;//Ҳ����a���ڴ��ֵΪ90
	//cout << a << endl;
	int a = 3, b = 4;
	swap(a, b);
	cout << a << ", " << b << endl;//����a��b��Ȼ��3��4
	return 0;


	return 0;
}