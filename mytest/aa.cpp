
#include <iostream>

int b = 0;
// int &a = b;

int fn(int &a) {
    return ++a;
}

int main() {
    // a = 5;
    std::cout << fn(b) << std::endl;

    if (__cplusplus > 201703L)
        std::cout << "C++2a\n";
    else if (__cplusplus == 201703L)
        std::cout << "C++17\n";
    else if (__cplusplus == 201402L)
        std::cout << "C++14\n";
    else if (__cplusplus == 201103L)
        std::cout << "C++11\n";
    else if (__cplusplus == 199711L)
        std::cout << "C++98\n";
}
 