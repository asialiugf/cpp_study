#include <iostream>

template <int N>
class A {
   public:
    // static int x = N * 2;
    int x;
    // A(int a = 0) { x = a; };
    void set_index(int a) { x = x + a; };
};

template <int N>
class B : public A<N> {};
template <int N>
class C : public A<N> {};

void fn() {
    static B<4> b;
    static C<5> c;
    std::cout << b.x << std::endl;
    std::cout << b.x << std::endl;
}
// int A::x{0};
int main() {
    static A<4> a1;
    static A<5> a2;
    // int A::x{0};
    // int A<4>::x{0};
    for (int i = 0; i < 10; i++) {
        a1.set_index(i);
        a2.set_index(i);
        fn();
    }
    exit(0);
}
