#include <iomanip>
#include <iostream>
#include <sstream>
int main() {
    std::istringstream input(" \"Some text.\" ");
    char               cc = 'b';
    int                m = 40;
    volatile int       n = 42;
    double             f = 3.14;
    bool               b = true;
    std::cout << n                    // int overload
              << ' '                  // non-member overload
              << std::boolalpha << b  // bool overload
              << " "                  // non-member overload
              << std::fixed << f      // double overload
              << input.rdbuf()        // streambuf overload
              << &n                   // bool overload: volatile int* doesn't convert to const void*
              << std::endl;           // function overload
    std::cout.operator<<(&m) << std::endl;
    std::cout.operator<<(&cc) << std::endl;
    std::cout.operator<<(cc) << std::endl;
    std::cout << cc << std::endl;
}
