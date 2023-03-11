#include <iostream>
#include <vector>
class Person {
   public:
    Person(){};
    Person(int a, int b) {
        this->m_A = a;  // 构造函数中的类本身的属性要加this
        this->m_B = b;
    }
    // 成员函数实现 + 号运算符重载
    Person operator+(const Person& p) {
        Person temp;
        temp.m_A = this->m_A + p.m_A;
        temp.m_B = this->m_B + p.m_B;
        return temp;
    }
    friend Person operator+(const Person& p2, int val) {
        Person temp;
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }

    friend Person operator+(int val, const Person& p2) {
        Person temp;
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
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
    Person p3 = p2 + p1;  // 相当于 p2.operaor+(p1)
    std::cout << "mA:" << p3.m_A << " mB:" << p3.m_B << std::endl;

    Person p4 = p3 + 10;  // 相当于 operator+(p3,10)
    std::cout << "mA:" << p4.m_A << " mB:" << p4.m_B << std::endl;
}

int main() {
    test();

    Person x1(1, 2);
    Person x2(2, 3);
    Person x3 = 21 + x1 + 10 + x2 + 10 + 20 + x2 + x1;

    std::cout << x3.m_A << std::endl;

    system("pause");

    return 0;
}

// g++ op.cpp
