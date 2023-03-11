#include <iostream>
#include <type_traits>
using namespace std;

//斐波那契数列
template<int T>
struct Fibonacci_1
{
	enum { res = Fibonacci_1<T - 1>::res + Fibonacci_1<T - 2>::res };
};
template<>
struct Fibonacci_1<1>
{
	enum {res = 1 };
};
template<>
struct Fibonacci_1<2>
{
	enum {res = 2 };
};


//c11 定义整型数据 - 关键字 integral_constant

template<class T>
struct one_type : std::integral_constant<int, 1>{};

//斐波那契数列
template<int arg>
struct Fibonacci_2 : std::integral_constant<int, Fibonacci_2<arg - 1>::value + Fibonacci_2<arg - 2>::value>{};

template<>
struct Fibonacci_2<1> :std::integral_constant<int, 1>{};

template<>
struct Fibonacci_2<2> :std::integral_constant<int, 2>{};




//求取最大的整数
template <size_t arg, size_t... rest>
struct IntegerMax_1
{
	//enum{ res };
};

template <size_t arg>
struct IntegerMax_1<arg>
{
	enum { res = arg };
};

template <size_t arg1, size_t arg2, size_t... rest>
struct IntegerMax_1<arg1, arg2, rest...>
{
	enum{ res = ( arg1 >= arg2 ? IntegerMax_1<arg1, rest...>::res : IntegerMax_1<arg2, rest...>::res) };
};


//c11 定义整型数据 - 关键字 integral_constant

template <size_t arg, size_t... rest>
struct IntegerMax_2;

template <size_t arg>
struct IntegerMax_2<arg> : std::integral_constant<size_t, arg>{};

template <size_t arg1, size_t arg2, size_t... rest>
struct IntegerMax_2<arg1, arg2, rest...>
	: std::integral_constant< size_t, arg1 >= arg2 ? IntegerMax_2<arg1, rest...>::value : IntegerMax_2<arg2, rest...>::value >{};

//可变参数模板

//递归方式展开
//通过递归函数展开参数包，需要提供一个参数包展开的函数和一个递归终止函数

void printargs()//递归终止函数-等同于递归出口 即没有参数时的返回
{	
}

template<class T>
void printargs(T t)//参数包展开函数
{
	cout << t << endl;
}

template<class T,class ...Args>
void printargs(T t,Args... args)
{
	//cout << sizeof...(args) << endl;//sizeof...()获取参数个数
	printargs(t);		//重载调用输出
	printargs(args...); //递归调用自身
}



int getdata(int n)
{
	if (n == 1)
	{
		return 1;
	}
	else if (n == 2)
	{
		return 2;
	}
	else
	{
		return getdata(n - 1) + getdata(n - 2);
	}
}
int main()
{
	
	cout << Fibonacci_1<50>::res << endl;
	cout << Fibonacci_2<50>::value << endl;
	
	//cout << getdata(50)<<endl;

	cout << IntegerMax_1<10, 13, 15, 16>::res << endl;//必须为常量
	cout << IntegerMax_2<10, 13, 15, 16>::value << endl;//必须为常量
	
	//printargs<int>(1,2,3,4,5,6);
	
	cin.get();
	return 0;
}
