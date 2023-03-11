#include <iostream>

#include "timestamp.h"

template <typename T>
T max(T a, T b) {
    return b < a ? a : b;
}

int m = 0;

constexpr int f(int a) {
    return a + 1;
}

int main() {
    int x;

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
