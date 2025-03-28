# C++

### 头文件

C++头文件不必是.h结尾，C语言中的标准库头文件如math.h,stdio.h在C++被命名为cmath,cstdio

```c++
#include <cmath>
#include <cstdio>
```

### 命名空间

为了防止名字冲突（出现重名），C++引入了名字空间（namespace），通过：：运算符限定某个名字属于哪个名字空间。

```c++
//如“计算机1702”李平
//如“自动化1603”李平
#include <stdio>
namespace first
{
    int a;
    void f();
    int g();
}
namespace second
{
    double a;
    double f();
    char g;    
}
```

![image-20250314215829546](image/image-20250314215829546.png)

### 标准输入输出流

C++的新的输入输出流库（头文件iostream）将输入输出看成一个流，并用输出运算符<<和输入运算符>>对数据（变量和常量进行输入输出）

其中有cout和cin分别代表标准输出流对象（屏幕窗口）和标准输入流对象（键盘）

标准库中的名字都属于标准名字空间std

```c
#include <iostream>
using std::cout;
int main()
{
  double a;
  cout<<"从键盘输入一个数"<<std::end1;//end1表示换行符，并强制输出
  std::cin>>a;//通过名字限定std::cin,
              //cin是代表键盘的输入流对象，>>等待键盘输入一个实数
  a=sin(a);
  cout<<a;    //cout是代表屏幕窗口的输出流对象
  return 0;
}
```

![image-20250314222950157](image/image-20250314222950157.png)

这里由于使用using namespace std，下面的cout和cin都可以直接使用

### 变量及其作用域

变量“即用即定义”，且可用表达式初始化

```c++
#include <iostream>
using namespace std;
int main()
{
	double a = 3.25 * 3;
	double b = a + 1.112;
	cout << "a contains:" << a << endl;
	cout << "b contains:" << b << endl;

	double c = a + b * a;
	cout << "c contains:" << c << endl;
	std::cout << "I love you";
	return 0;
}
```

程序块{}内部作用域可以定义域外部作用域同名的变量，在该块内隐藏了外部变量，也就是外部同名变量在内部不起作用。主要：变量一定都要指明类型

```c++
#include <iostream>
using namespace std;

int main()
{
	double a;
	cout << "Type a number:";
	cin >> a;
	{
		int a = 1;//int a 隐藏了外部作用域的double a
		cout << "Local Number:" << a << endl;
	}
	cout << "You Type Number:" << a << endl;

	return 0;
}
```

for循环语句可以定义局部变量，for循环块跟上一部分一样，外部变量无法影响块内部

```c++
#include <iostream>
using namespace std;

int main()
{
	int i = 0;
	for (int i=0;i<4;i++)
	{
		cout << i << endl;
	}
	cout << "i contains:" << i << endl;

	for (int i = 0;i<4;i++)
	{
		for (int i = 0;i<4;i++)
		{
			cout << i << " ";
		}
		cout << endl;
	}
	return 0;
}
```

这里的i一定都要指明变量的类型，否则会被定义为离你最近的变量的类型，这样操作的是外部变量

访问和内部作用域变量同名的全局变量，要使用全局作用域限定：：

```c++
#include <iostream>
using namespace std;
double a = 3.14;
int main()
{
	for (int a = 0;a<4;a++)//内部变量一定要定义类型，否则会被自动定义为double型，也就是外部的a
	{
		cout << ::a << endl;//::是全局变量限定
	}
	return 0;
}
```

### 引用

C++引入了引用类型，即一个变量时另一个变量的别名

```c++
#include <iostream>
using namespace std;
int main()
{
	double a = 3.14;
	double& b = a;//b是a的别名，b就是a
	b = 90;//也就是a的内存块值为90
	cout << a << endl;
	return 0;
}
```

引用经常用作函数的形参，表示形参和实参实际上是同一个对象，在函数中对形参的修改也就是对实参的修改

例1：为正常的形参和实参使用

```c++
#include <iostream>
using namespace std;
void swap(int x,int y)
{
    cout<<"swap函数内交换前："<< x <<" "<< y <<end1;
    int t = x;x=y;y=t;
    cout<<"swap函数内交换后："<< x <<" "<< y <<end1;
}
int main()
{
    int a = 3,b = 4;
    swap(a,b);
    cout<< a <<", "<< b <<end1;//这里a和b仍然是3和4
    return 0;
}
```

例2：使用指针进行修改实参值，修改的是x,y指向的那两个int型变量中的内容

```c++
#include <iostream>
using namespace std;
void swap(int *x,int *y)
{
    cout<<"swap函数内交换前："<< *x <<" "<< *y <<end1;
    int t = *x;*x = *y;*y = t;
    cout<<"swap函数内交换后："<< *x <<" "<< *y <<end1;
}
int main()
{
    int a = 3,b = 4;
    swap(&a,&b);//这里&是取a和b的地址
    cout<< a <<", "<< b <<end1;//这里a和b是4和3
    return 0;
}
```

例3：使用引用类型做形参，从而修改实参内部值

```c++
#include <iostream>
using namespace std;
void swap(int &x,int &y)
{
    cout<<"swap函数内交换前："<< x <<" "<< y <<end1;
    int t = x;x = y;y = t;
    cout<<"swap函数内交换后："<< x <<" "<< y <<end1;
}
int main()
{
    int a = 3,b = 4;
    swap(a,b);//这里&x = a，&y = b，x和y分别是a和b的引用，即x就是a，y就是b
    cout<< a <<", "<< b <<end1;//这里a和b是4和3
    return 0;
}
```

当实参栈内存大是，用引用代替传值（需要把值从一个内存块复制到另一个内存块）可提高效率，如果不希望因此无意中修改实参，可以用const修改符。如：

```c++
#include <iostream>
using namespace std;
void change(double &x,const double &y,double &z)
{
    x=100;
    y=200;//会报错，y是不可修改的，因为为const double &
    z=300;
}
int main()
{
    double a,b,c;//内在类型变量未提供初始化式，默认初始化为0
    change(a,b,c);
    return 0;
}
```

### 内联函数

- 对于不包含循环的简单函数，建议用inline关键字声明为“inline内联函数”，编译器将内联函数调用，用其代码展开。
- 也就是在调用函数时，用函数的内容替换函数调用，称为“内联展开”，避免函数调用开销，提高程序执行效率
- 当函数内部有循环体或者函数内容较多，可能不会执行内联展开

```c++
#include <iostream>
#include <cmath>
using namespace std;
inline double distance(double e,double b)
{
    return sqrt(a*a+b*b);
}
int main()
{
    double k = 6,m = 9;
    //下面两行将产生同样的代码：
    cout<<distance（k,m）<<end1;//即把distance()用其函数内容替代
    cout<<return sqrt(k*k+m*m)<<end1;
    return 0;
}
```

### 默认形参

函数的形参可带有默认值，必须一律在最右边

```c++
#include <iostream>
using namespace std;
double test(double a,double b = 7)
{
    return a-b;
}
int main()
{
    cout<<test（14,5）<<end1;
    cout<<test(14)<<end1;
    return 0;
}
/*错：默认参数一律靠右*/
double test(double a,double b = 7,double c)
{
    return a-b;
}
```

### 函数重载

- C++允许函数同名，只要他们的形参不一样（个数或对象参数类型），调用函数时将根据实参和形参的匹配选择最佳函数。
- 如果有多个难以区分的最佳函数，则变化一起报错
- 注意：不能根据返回类型区分同名函数

```c++
#include <iostream>
using namespace std;
double add(double a,double b)
{
    return a-b;
}
int add(int a,int b)
{
    return a-b;
}
//错：编译器无法区分int add(int a,int b),void add(int a,int b)
void add(int a,int b)
{
    return a-b;
}
```

### 模板template函数

比方说对每种类型求最小值

```c++
#include <iostream>
using namespace std;
double minValue(double a,double b)
{
    if(a<b)return a;//return a<b?a:b
    else return b;
}
int minValue(int a,int b)
{
    if(a<b)return a;//return a<b?a:b
    else return b;
}

//如果是结构体类型的变量，就又要重写一次，现在可以利用模板函数直接定义取最小值的模板函数
//可以对任何能比较大小(<)的类型使用该模板让编译器自动生成一个针对该数据类型的具体函数
template<class T>
T minValue(T a,T b)
{
    if(a<b)return a;//return a<b?a:b
    else return b;   
}

//但是该模板函数还有一个问题是，当实参传入的是两个不同类型的数据时，会无法确定输出什么类型的参数。因此，再设定可以以不同类型参数输入的模板函数
template <class T1，class T2>
T1 minValue(T1 a,T2 b)
{
    if(a<b)return a;//return a<b?a:b
    else return (T1)b;   
}
int main()
{
    int i = 3,j = 4;
    cout<<"min of"<<i<<"and"<<j<<"is"<<minValue(i.j)<<end1;
    double x = 3.5,y = 10;
    cout<<"min of"<<x<<"and"<<y<<"is"<<minValue(x.y)<<end1;    
    return 0;
}
```

### 动态内存分配

