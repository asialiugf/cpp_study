#include <iostream>
#include <string>

// 类模版
template <typename T>
class man {
   public:
    man(T t) : name(t) {}

    void printName() { std::cout << name << std::endl; }

    // 类模版的友元函数声明
    template <typename Q>
    friend void fprintMan(man<Q>& m);

    // 类模版的友元类声明
    template <typename Q>
    friend class printClass;

   private:
    T name;
};

// 类模版的友元函数定义
template <typename Q>
void fprintMan(man<Q>& m)  // 注意: 模板类必须实例化 内部实例化 man<Q>
{
    m.printName();
}

// 类模版的友元类定义
template <typename Q>
class printClass {
   public:
    void print(man<Q>& my)  // 注意: 模板类必须实例化 内部实例化 man<Q>
    {
        my.printName();
    }
};

// 类模版作为函数参数
template <typename T>
void printMan(man<T>& m)  // 注意: 模板类必须实例化 内部实例化 man<T>
{
    m.printName();
}

// 类模版作为模版参数
template <typename T1, template <typename T> class T2>
class people {
   public:
    void printPeople(T2<T1> t1)  // 注意: 模板类必须实例化 内部实例化 T2<T1>
    {
        t1.printName();
    }
};

// 函数模版
template <typename T>
T addTemplate(T x, T y) {
    return (x + y);
}

// 函数模版作为函数的参数
template <typename T>
void printAdd(T (*p)(T, T), T x, T y)  // 注意: 模版函数的指针必须实例化 外部实例化
{
    std::cout << (*p)(x, y) << std::endl;
}

// 函数模版作为类的参数
template <typename T>
class addclass {
   public:
    void printAddFun(T (*p)(T, T), T x, T y)  // 注意: 模版函数的指针必须实例化 外部实例化
    {
        std::cout << (*p)(x, y) << std::endl;
    }
};

int main() {
    man<std::string> man1("hello");  // 类模版实例化对象
    man1.printName();

    fprintMan(man1);  // 类模版的友元函数

    printClass<std::string> print1;  // 类模版的友元类
    print1.print(man1);

    printMan(man1);  // 类模版作为函数参数

    people<std::string, man> people1;
    people1.printPeople(man1);  // 类模版作为模版参数

    std::cout << addTemplate(3, 4) << std::endl;  // 函数模版实例化
    std::cout << addTemplate(7.7, 9.5) << std::endl;

    printAdd<int>(addTemplate, 10, 54);  // 函数模版必须实例化

    addclass<int> addclass1;
    addclass1.printAddFun(addTemplate, 10, 54);  // 函数模版作为类的参数

    system("pause");
    return 0;
}
