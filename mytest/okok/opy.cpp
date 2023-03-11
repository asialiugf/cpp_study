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
        std::cout << " Person(int,int) constructor ----" << std::endl;
        this->m_A = a;  // 构造函数中的类本身的属性要加this
        this->m_B = b;
    }
    Person(int a) {
        this->m_A = a;
        std::cout << " Person(int)  constructor ----" << std::endl;
    }

    Person(int a, const Person& p) {
        this->m_A = a + p.m_A;
        std::cout << " Person(int, Person)  constructor ----" << std::endl;
    }
    Person(const Person& p, int a) {
        this->m_A = a + p.m_A;
        std::cout << " Person(Person,int)  constructor ----" << std::endl;
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

    friend inline int operator+(const Person p1, const Person p2) {
        std::cout << "p1 & p2  " << std::endl;
        return p1.m_A + p2.m_A;
    }
    friend inline int operator+(int val, const Person p2) {
        std::cout << "num & p2  " << std::endl;
        return val + p2.m_A;
    }
    friend inline int operator+(const Person p2, int val) {
        std::cout << "p2 and num  " << std::endl;
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

void fnfn() {
    static Person x1(1, 2);
    std::cout << "Person x4:";
    static Person x40(x1);
    std::cout << "x40.m_A:" << x40.m_A << std::endl;
}

int main() {
    static Person x1(1001, 2);  //  Person(int,int) constructor ---- 1001,2
    std::cout << "Person x4:" << std::endl;
    // static Person x42{x1};  //  copy !  1001,2
    // static Person x42(x1);  //  copy !  1001,2
    // static Person x42(x1 + x1, 98);  // p1 & p2, Person(int,int)  constructor ----  2002,98
    // static Person x42(67, x1 + x1 + 22);  // p1 & p2, Person(int,int)  constructor ----  67,2024
    // static Person x42(x1 + x1);  // p1 & p2, Person(int)  constructor ----  2002,0
    static Person x42(x1, 45);  //  如果没有定义会报错 ：没有与参数列表匹配的构造函数 "Person::Person" 实例C/C++(289) opy.cpp(140, 23):
                                //  参数类型为: (Person, int)
                                //  如果有定义（上面第23行）： Person(Person,int)  constructor ----  1046, 0 // 这里数字是构造函数产生的。
    std::cout << "x1.m_A:  " << x1.m_A << "       x1.m_B:  " << x1.m_B << std::endl;
    std::cout << "x42.m_A:  " << x42.m_A << "       x42.m_B:  " << x42.m_B << std::endl;
    for (int i = 0; i < 3; i++) {
        std::cout << std::endl;
        std::cout << "-------------------------------" << std::endl;
        // fnfn();
    }

    return 0;
}

// g++ opx.cpp
