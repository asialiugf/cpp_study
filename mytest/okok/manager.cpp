#include <cmath>
#include <iostream>
#include <vector>

#include "timestamp.h"

#define kkkk static Person

uint64_t before;
uint64_t latency;

class Manager {
   public:
    static int x;
    Manager() { x = 0; }
    //~Manager(){};
    void set_current(int a) { x = a; }
};

class Person : public Manager {
   public:
    Person() { std::cout << "constructor !!" << std::endl; };
    Person(int a, int b) {
        std::cout << " Person(int,int) constructor ----" << std::endl;
        this->m_A = a;  // 构造函数中的类本身的属性要加this
        this->m_B = b;
    }
    Person(int a) {
        this->m_A = a;
        std::cout << " Person(int)  constructor ----:" << Manager::x << std::endl;
    }

    Person(int a, const Person& p) {
        this->m_A = a + p.m_A;
        std::cout << " Person(int, Person)  constructor ----:" << Manager::x << std::endl;
    }
    Person(const Person& p, int a) {
        this->m_A = a + p.m_A;
        std::cout << " Person(Person,int)  constructor ----:" << Manager::x << std::endl;
    }

    /* 这个构造函数不能有！
     * 有了下面这个构造函数之后， 下面的 fnfn()里面的所有的 x1 x2 x3 相关的运算，都会调用这个构造函数。
     * 应该改成  operator=(const Person&p) {}  重载 = 运算符
    Person(const Person& p) {
        this->m_A = p.m_A;
        std::cout << " Persion  init !" << std::endl;
    }
    */

    // 成员函数实现 + 号运算符重载
    /*
    Person operator+(const Person& p) {
        Person temp;
        temp.m_A = this->m_A + p.m_A;
        temp.m_B = this->m_B + p.m_B;
        return temp;
    }
    */
    /*
    friend Person operator+(const Person& p2, int val) {
        Person temp;
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }

    friend Person operator+(int val, const Person& p2) {
        Person temp;  // 有没有性能问题？
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }
    */

    Person& operator=(int x) {
        std::cout << "赋值: " << x << std::endl;
        ;
        m_A = x;
        return *this;
    }
    Person& operator=(const Person& p) {
        std::cout << "赋值 persion:" << x << std::endl;
        m_A = p.m_A * 2;
        return *this;
    }

    friend inline int operator+(const Person p1, const Person p2) {
        std::cout << "p1 & p2 : " << p1.x << std::endl;
        return p1.m_A + p2.m_A;
    }
    friend inline int operator+(int val, const Person p2) {
        std::cout << "num & p2:  " << p2.x << std::endl;
        return val + p2.m_A;
    }
    friend inline int operator+(const Person p2, int val) {
        std::cout << "p2 and num:  " << p2.x << std::endl;
        return val + p2.m_A;
    }

   public:
    int m_A;
    int m_B;
};

/*全局函数实现 + 号运算符重载
Person operator+(const Person& p1, const Person& p2) {
        Person temp(0, 0);
        temp.m_A = p1.m_A + p2.m_A;
        temp.m_B = p1.m_B + p2.m_B;
        return temp;
}
*/

// 运算符重载 可以发生函数重载
/*
Person operator+(const Person& p2, int val) {
    Person temp;
    temp.m_A = p2.m_A + val;
    temp.m_B = p2.m_B + val;
    return temp;
}
*/

void test() {
    Person p1(10, 10);
    Person p2(20, 20);

    // 成员函数方式
    // Person p3 = p2 + p1;  // 相当于 p2.operaor+(p1)
    // std::cout << "mA:" << p3.m_A << " mB:" << p3.m_B << std::endl;

    // Person p4 = p1 + 10;  // 相当于 operator+(p3,10)
    // std::cout << "mA:" << p4.m_A << " mB:" << p4.m_B << std::endl;
};

int add1(int a, int b) { return a + b; };

void fnfn(int i) {
    kkkk x1(i, 2);
    x1 = x1 + i;
    std::cout << "Person x4:";
    static Person x40;
    static Person x41;
    static Person x42;
    static Person x43;

    x40 = x1;
    x41 = x1;
    x42 = x1 + x1 + 1;
    x43 = 1 + x1 + x1 + 1;
    std::cout << "x1:" << x1.m_A << "  x40:" << x40.m_A << "  x41:" << x41.m_A << "  x42:" << x42.m_A << "  x43:" << x43.m_A << std::endl;
}

double sqrt(const Person& p) { return sqrt(p.m_A); }

int main() {
    static Person x1(1001, 2);  //  Person(int,int) constructor ---- 1001,2
    // x1 = x1 + 5;
    // static Person x2 = 339;  // Person(int)  constructor ----  339, 0
    // static Person x2 = x1 + 339;  // ( p2 and num )   Person(int)  constructor ----  1340, 0
    static Person x2 = x1;                             // copy from x1;   1001,2
    x2 = x2 + x1 + 100;                                // ( p1 & p2 ) + 赋值  2102
    x2 = x2 + x2 + 1 + 2 + 3 + x1 + x2 + x1 + 3 + x2;  // (p1 & p2) (num & p2)        (num & p2)  (num & p2)      (num & p2)     10419,2
                                                       //  x2 + x2  4204+x1...+1+2+3... 5210+x2    7312+x1  ...+3... xx+x2
                                                       // 中间部分是 +1+2+3 是数字+数字，没有调用关系！

    x2 = x1;  //  赋值 p   2002,2  请看前面的 operator=(const Person& p){} 这里面是 *2; 所以变成了2002而不是 1001
    // x2 = sqrt(x1);  // 如果不重载 sqrt()的话 报错： 不存在从 "Person" 到 "double" 的适当转换函数C/C++(413)
    x2 = sqrt(x1);  // 135行 重载了 sqrt(const Person&) 所以正常处理了。

    std::cout << "x2.m_A:  " << x2.m_A << "       x2.m_B:  " << x2.m_B << std::endl;

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < 10; i++) {
        fnfn(i);
    }

    return 0;
}

// g++ opequal.cpp
