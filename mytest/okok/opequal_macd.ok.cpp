#include <cmath>
#include <iostream>
#include <vector>

#include "timestamp.h"

#define kkkk static FutureSerial
#define mmmm static MACD

uint64_t before;
uint64_t latency;

class FutureSerial {
   public:
    FutureSerial() { std::cout << "constructor !!" << std::endl; };
    FutureSerial(int a, int b) {
        std::cout << " FutureSerial(int,int) constructor ----" << std::endl;
        this->m_A = a;  // 构造函数中的类本身的属性要加this
        this->m_B = b;
    }
    FutureSerial(int a) {
        this->m_A = a;
        std::cout << " FutureSerial(int)  constructor ----" << std::endl;
    }

    FutureSerial(int a, const FutureSerial& p) {
        this->m_A = a + p.m_A;
        std::cout << " FutureSerial(int, FutureSerial)  constructor ----" << std::endl;
    }
    FutureSerial(const FutureSerial& p, int a) {
        this->m_A = a + p.m_A;
        std::cout << " FutureSerial(FutureSerial,int)  constructor ----" << std::endl;
    }

    /* 这个构造函数不能有！
     * 有了下面这个构造函数之后， 下面的 fnfn()里面的所有的 x1 x2 x3 相关的运算，都会调用这个构造函数。
     * 应该改成  operator=(const FutureSerial&p) {}  重载 = 运算符
    FutureSerial(const FutureSerial& p) {
        this->m_A = p.m_A;
        std::cout << " Persion  init !" << std::endl;
    }
    */

    // 成员函数实现 + 号运算符重载
    /*
    FutureSerial operator+(const FutureSerial& p) {
        FutureSerial temp;
        temp.m_A = this->m_A + p.m_A;
        temp.m_B = this->m_B + p.m_B;
        return temp;
    }
    */
    /*
    friend FutureSerial operator+(const FutureSerial& p2, int val) {
        FutureSerial temp;
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }

    friend FutureSerial operator+(int val, const FutureSerial& p2) {
        FutureSerial temp;  // 有没有性能问题？
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }
    */

    FutureSerial& operator=(int x) {
        std::cout << "赋值\n";
        m_A = x;
        return *this;
    }
    FutureSerial& operator=(const FutureSerial& p) {
        std::cout << "赋值 p\n";
        m_A = p.m_A * 2;
        return *this;
    }

    friend inline int operator+(const FutureSerial p1, const FutureSerial p2) {
        std::cout << "p1 & p2  " << std::endl;
        return p1.m_A + p2.m_A;
    }
    friend inline int operator+(int val, const FutureSerial p2) {
        std::cout << "num & p2  " << std::endl;
        return val + p2.m_A;
    }
    friend inline int operator+(const FutureSerial p2, int val) {
        std::cout << "p2 and num  " << std::endl;
        return val + p2.m_A;
    }

   public:
    int m_A;
    int m_B;
};

class MACD : public FutureSerial {
   public:
    MACD() { std::cout << "constructor !!" << std::endl; };
    MACD(int a, int b) {
        std::cout << " FutureSerial(int,int) constructor ----" << std::endl;
        this->m_A = a;  // 构造函数中的类本身的属性要加this
        this->m_B = b;
    }
    MACD(int a) {
        this->m_A = a;
        std::cout << " FutureSerial(int)  constructor ----" << std::endl;
    }

    MACD(int a, const FutureSerial& p) {
        this->m_A = a + p.m_A;
        std::cout << " FutureSerial(int, FutureSerial)  constructor ----" << std::endl;
    }
    MACD(const FutureSerial& p, int a) {
        this->m_A = a + p.m_A;
        std::cout << " FutureSerial(FutureSerial,int)  constructor ----" << std::endl;
    }
    using FutureSerial::operator=;
};
class KDJ : public FutureSerial {};

void test() {
    FutureSerial p1(10, 10);
    FutureSerial p2(20, 20);
    MACD m1(100, 100);
    //  KDJ m1(100, 100);

    // 成员函数方式
    // FutureSerial p3 = p2 + p1;  // 相当于 p2.operaor+(p1)
    // std::cout << "mA:" << p3.m_A << " mB:" << p3.m_B << std::endl;

    // FutureSerial p4 = p1 + 10;  // 相当于 operator+(p3,10)
    // std::cout << "mA:" << p4.m_A << " mB:" << p4.m_B << std::endl;
};

int add1(int a, int b) { return a + b; };

// here!! is real testing!!!! ----------------
void fnfn(int i) {
    // kkkk x1(i, 2);
    mmmm x1(i, 2);
    x1 = x1 + i;
    std::cout << "FutureSerial x4:";
    /*
    static FutureSerial x40;
    static FutureSerial x41;
    static FutureSerial x42;
    static FutureSerial x43;
    */
    static MACD x40;
    static MACD x41;
    static MACD x42;
    static MACD x43;

    x40 = x1;
    x41 = x1;
    x42 = x1 + x1 + 1;
    x43 = 1 + x1 + x1 + 1;
    std::cout << "x1:" << x1.m_A << "  x40:" << x40.m_A << "  x41:" << x41.m_A << "  x42:" << x42.m_A << "  x43:" << x43.m_A << std::endl;
}

double sqrt(const FutureSerial& p) { return sqrt(p.m_A); }

int main() {
    // static FutureSerial x1(1001, 2);  //  FutureSerial(int,int) constructor ---- 1001,2
    static MACD x1(1001, 2);  //  FutureSerial(int,int) constructor ---- 1001,2
    // x1 = x1 + 5;
    // static FutureSerial x2 = 339;  // FutureSerial(int)  constructor ----  339, 0
    // static FutureSerial x2 = x1 + 339;  // ( p2 and num )   FutureSerial(int)  constructor ----  1340, 0
    static FutureSerial x2 = x1;                       // copy from x1;   1001,2
    x2 = x2 + x1 + 100;                                // ( p1 & p2 ) + 赋值  2102
    x2 = x2 + x2 + 1 + 2 + 3 + x1 + x2 + x1 + 3 + x2;  // (p1 & p2) (num & p2)        (num & p2)  (num & p2)      (num & p2)     10419,2
                                                       //  x2 + x2  4204+x1...+1+2+3... 5210+x2    7312+x1  ...+3... xx+x2
                                                       // 中间部分是 +1+2+3 是数字+数字，没有调用关系！

    x2 = x1;  //  赋值 p   2002,2  请看前面的 operator=(const FutureSerial& p){} 这里面是 *2; 所以变成了2002而不是 1001
    // x2 = sqrt(x1);  // 如果不重载 sqrt()的话 报错： 不存在从 "FutureSerial" 到 "double" 的适当转换函数C/C++(413)
    x2 = sqrt(x1);  // 135行 重载了 sqrt(const FutureSerial&) 所以正常处理了。

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
