#include <iostream>

template <typename T>
T max(T a, T b) {
    return b < a ? a : b;
}

int main1() {
    std::cout << ::max(static_cast<double>(3), 1.4) << std::endl;
    std::cout << ::max(1, 5) << std::endl;
    char a = ::max(65, 56);
    std::cout << a << std::endl;
    std::max(3, 1);
    return 0;
}

template <typename T>
void hello(T x) {
    //    undeclared();   // first-phase compile-time error if undeclared() unknown
    //    undeclared(x);  // second-phase compile-time error if undeclared(T) unknown
    // static_assert(sizeof(int) > 10, "int too small");  // always fails if sizeof(int) <= 10
    // static_assert(sizeof(T) > 1000, "T too small");  // fails if instantiated for T with size <=
    // 10
    std::cout << x << std::endl;
    // return a;
}

template <typename T, typename U>
auto max(T t, U u) {
    return t > u ? t : u;
}

int main2() {
    hello<double>(3);
    return 0;
}

int main() {
    main1();
}
