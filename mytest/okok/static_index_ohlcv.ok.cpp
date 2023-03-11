#include <array>
#include <cmath>
#include <iostream>
template <int N>
class B {
   public:
    static int index;  //  index搞定了
    static int hello;
    static std::array<double, 100000> o;  // ohlcv 搞定了
    // 派生类的执行列表 还没有搞定。
};

template <int N>  // 这里很神奇啊！！！！
int B<N>::index;
template <int N>  // 这里很神奇啊！！！！
int B<N>::hello;
template <int N>                     // 这里很神奇啊！！！！
std::array<double, 100000> B<N>::o;  // 这里的数组的长度应该怎么搞？？？？ 固定吗?

template <int N>
class D1 : public B<N> {
   public:
    D1() { std::cout << "d1!" << std::endl; }
};
template <int N>
class D2 : public B<N> {
   public:
    D2() { std::cout << "d2! init !!" << std::endl; };
};

void fn() {
    D1<4> d41;
    D1<4> d42;
    D1<4> d43;
    D2<60> d60a;
    D2<60> d61b;
    D2<60> d62b;
    D2<60> d63b;
    static D2<60> d64b;
    static D2<60> d65b;
    static D2<60> d66b;
    static D2<60> d67b;
    static D2<60> d68b;
    static D2<60> d69b;
    static D2<60> d6rb;
    static D2<60> d6ub;
    static D2<60> d6eb;
    std::cout << "d41.hello " << d41.hello << "   y2.hello " << d42.hello << std::endl;
    std::cout << "d41.o[d41.index] " << d41.index << "  " << d41.o[d41.index] << "  d42.o[d42.index] " << d42.index << "  "
              << d42.o[d42.index] << std::endl;

    std::cout << "d60a.hello " << d60a.hello << "   d60b.hello " << d67b.hello << std::endl;
    std::cout << "d6eb.o[d6eb.index] " << d6eb.index << "  " << d6eb.o[d6eb.index] << "  d67b.o[d67b.index] " << d67b.index << "  "
              << d67b.o[d67b.index] << std::endl;
}

int main() {
    B<4>::index = 51245;
    B<500>::index = 9999;
    D1<4> x;
    D1<4> x2;
    // D2<60>
    D2<500> y1;
    D2<500> y2;

    std::cout << x.index << std::endl;   // 51425
    std::cout << x2.index << std::endl;  // 51425

    x.index = 1;
    std::cout << x2.index << std::endl;  // 1
    x2.index = 1000;
    std::cout << x.index << std::endl;  // 1000

    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;
    y2.index = 8888;
    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;

    B<4> b1;
    B<60> b2;

    for (int i = 0; i < 100; i++) {
        b1.hello = i;
        b2.hello = i;
        b1.index = i;
        b2.index = i;
        b1.o[i] = sqrt(i * 2);
        b2.o[i] = sqrt(i * 2);  // 基类的 static 变化

        /*
        B<4>::index = i;
        B<4>::hello = i;
        B<4>::o[i] = i * 2;
        B<60>::index = i;
        B<60>::hello = i;
        B<60>::o[i] = sqrt(i * 2);
        */
        fn();  // 派生类使用！
    }
}
