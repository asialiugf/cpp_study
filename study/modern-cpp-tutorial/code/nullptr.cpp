#include <iostream>
#include <type_traits>

constexpr int fibonacci(const int n) {
    if (n == 1)
        return 1;
    if (n == 2)
        return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
int fib(const int n) {
    if (n == 1)
        return 1;
    if (n == 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

// void foo(char *);
void foo(int n) {
    std::cout << n << std::endl;
}
void foo(int *n) {
    int i = 5;
    n     = &i;
    int x = i + *n;
    std::cout << *n << " " << x << std::endl;
}
int main() {

    int  i = 10;
    char aa[ i ];
    aa[ 0 ] = 'a';

    const int ii = 100;
    char      bb[ ii ];
    bb[ 0 ] = 'b';

    if (std::is_same<decltype(NULL), decltype(0)>::value)
        std::cout << "NULL == 0" << std::endl;
    if (std::is_same<decltype(NULL), decltype((void *)0)>::value)
        std::cout << "NULL == (void *)0" << std::endl;
    if (std::is_same<decltype(NULL), std::nullptr_t>::value)
        std::cout << "NULL == nullptr" << std::endl;
    foo(0);  // 调用 foo(int)
    // foo(NULL); // 该行不能通过编译
    foo(nullptr);  // 调用 foo(char*)
    foo(&i);

    std::cout << fibonacci(40) << std::endl;
    std::cout << fib(40) << std::endl;

    return 0;
}
