#include "stdio.h"
#include "string.h"
#include <iostream>
#include <cmath>
#include <cstdio>

using namespace std;
double a = 3.14;

void swap(int x, int y)
{
	cout << "swap函数内交换前：" << x << " " << y << endl;
	int t = x; x = y; y = t;
	cout << "swap函数内交换后：" << x << " " << y << endl;
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
	//	int a = 1;//int a 隐藏了外部作用域的double a
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
	//	cout << ::a << endl;//::是全局变量限定
	//}



	//double a = 3.14;
	//double& b = a;//b是a的别名，b就是a
	//b = 90;//也就是a的内存块值为90
	//cout << a << endl;
	int a = 3, b = 4;
	swap(a, b);
	cout << a << ", " << b << endl;//这里a和b仍然是3和4
	return 0;


	return 0;
}