#include <fstream>
#include <iostream>

int main() {
    std::cout << "Hello, world"        // the const char* overload
              << '\n';                 // the char overload
    std::ofstream("test.txt") << 1.2;  // rvalue overload
}
