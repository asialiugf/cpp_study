#include <iostream>
template <int N>
class B {
   public:
    static int a;
};

template <int N>  // 这里很神奇啊！！！！
int B<N>::a;

template <int N>
class D1 : public B<N> {
   public:
    D1() { std::cout << "d1!" << std::endl; }
};
template <int N>
class D2 : public B<N> {};

void fn() {
    D1<4> d1;
    D2<5> d2;
}

int main() {
    B<4>::a = 51245;
    B<500>::a = 9999;
    D1<4> x;
    D1<4> x2;
    D2<500> y1;
    D2<500> y2;

    std::cout << x.a << std::endl;   // 51425
    std::cout << x2.a << std::endl;  // 51425

    x.a = 1;
    std::cout << x2.a << std::endl;  // 1
    x2.a = 1000;
    std::cout << x.a << std::endl;  // 1000

    std::cout << "y1.a: " << y1.a << "   y2.a: " << y2.a << std::endl;
    y2.a = 8888;
    std::cout << "y1.a: " << y1.a << "   y2.a: " << y2.a << std::endl;
}
