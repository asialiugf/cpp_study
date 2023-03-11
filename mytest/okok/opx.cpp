#include <cmath>
#include <iostream>
#include <vector>

#include "timestamp.h"

uint64_t before;
uint64_t latency;

class Person {
   public:
    Person() { std::cout << "constructor !!" << std::endl; };
    Person(int a, int b) {
        std::cout << " contructor here!!----" << std::endl;
        this->m_A = a;  // 构造函数中的类本身的属性要加this
        this->m_B = b;
    }
    Person(int a) {
        this->m_A = a;
        std::cout << " one int !!" << std::endl;
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

    /*
    friend Person& operator=(int x) {
        std::cout << "复制赋值\n";
        m_A = x;
        return *this;
    }
    */

    friend inline int operator+(const Person p1, const Person p2) { return p1.m_A + p2.m_A; }
    friend inline int operator+(int val, const Person p2) { return val + p2.m_A; }
    friend inline int operator+(const Person p2, int val) { return val + p2.m_A; }

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
int add1(Person p1, Person p2) { return p1.m_A + p2.m_A; };

void fnfn() {
    static Person x1(1, 2);
    static Person x2(2009, 3);
    // Person x3 = 21 + x1 + 10 + x2 + 10 + 20 + x2 + x1 + x1 + x2 + x1 + x2 + x3;

    before = rdtsc();
    int m = 21 + x1 + 10 + x2 + 10 + 20 + x2 + x1 + x1 + x2 + x1 + x2 + 109;
    int kk = 21 + x2 + x1 + 3 + x1 + x2;
    int kk2 = x2 + x1 + x1 + x2;
    double u = 10.1 / (x1 + x2 + 7);
    double u1 = sqrt(x1 + x2);
    latency = rdtsc() - before;
    std::cout << "latency:   --: " << latency << std::endl;

    before = rdtsc();
    int m1 = 21 + x1.m_A + 10 + x2.m_A + 10 + 20 + x2.m_A + x1.m_A + x1.m_A + x2.m_A + x1.m_A + x2.m_A + 109;
    int kk1 = 21 + x2.m_A + x1.m_A + 3 + x1.m_A + x2.m_A;
    int kk21 = x2.m_A + x1.m_A + x1.m_A + x2.m_A;
    double ux1 = 10.1 / (x1.m_A + x2.m_A + 7);
    double u11 = sqrt(x1.m_A + x2.m_A);
    latency = rdtsc() - before;
    std::cout << "latency .m_A :   --: " << latency << std::endl;

    static Person x3(x1 + x2, 8);  // =========> 【采用这种方式，利用旧变量来定义新变量！！！】
    static Person x4(x2);

    x4.m_A = 1011;
    std::cout << " x2 and x4 ::  ";
    std::cout << x2.m_A << "  " << x4.m_A << std::endl;

    int ret = add1(x1, x2);  //  要重载实现
    int ret1 = add1(x1 + 1, x2 + 2);

    std::cout << "ret:" << ret << "      ret1:" << ret1 << std::endl;

    // Person x5 = 3;
    // int er = 21 + x1;
    std::cout << x1.m_A << "   " << kk << std::endl;
    std::cout << add1(1009, x1 + x2) << std::endl;
    std::cout << "u: " << u << std::endl;
    std::cout << "u1: " << u1 << std::endl;
    std::cout << "ux1: " << u << std::endl;
    std::cout << "u11: " << u1 << std::endl;

    std::cout << "x3.m_A:" << x3.m_A << std::endl;
    std::cout << "x4.m_A:" << x4.m_A << std::endl;
}

int main() {
    test();

    static Person x1(1, 2);
    // static Person x2(2, 3);
    // static Person x3(x1 + x2);
    std::cout << "Person x4:";
    static Person x40(x1);

    // std::cout << "x3.m_A:" << x3.m_A << std::endl;
    std::cout << "x40.m_A:" << x40.m_A << std::endl;

    for (int i = 0; i < 3; i++) {
        std::cout << std::endl;
        std::cout << std::endl;
        fnfn();
    }
    system("pause");

    return 0;
}

// g++ opx.cpp
