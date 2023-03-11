#include <iostream>

#include "timestamp.h"

template <typename T>
T max(T a, T b) {
    return b < a ? a : b;
}

template <typename T, std::size_t n>
int RSI(T a) {
    T x;
    x     = a;
    T *tt = new T[ n ];
    std::cout << sizeof(tt);
    return 0;
}

int m = 0;

constexpr int f(int a) {
    return a + 1;
}

int main() {

    int         x;
    std::size_t n;
    std::cin >> n;

    n = 199;

    x = RSI<double, 100>(3.14);
    x = RSI<int, 100>(30);
    x = RSI<long, 100>(3);
    x = RSI<char, n>('a');

    auto before  = rdtsc();
    x            = f(3);
    auto latency = rdtsc() - before;
    std::cout << "latency: " << latency << " cycles \n" << std::endl;

    std::cout << "x:" << x << "\n";

    int t;
    std::cin >> t;

    before  = rdtsc();
    x       = x + 1;
    latency = rdtsc() - before;
    std::cout << "latency: " << latency << " cycles \n" << std::endl;

    std::cout << "x:" << x << "\n";

    return 0;
}
