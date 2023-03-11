
#include <iostream>

int i = 5;
int n = 105;
// const int* p = &i;
int* p = &i;

int main() {

    *p = 6;  // 不可以；
    std::cout << "*p:" << *p << '\n';
    i = 7;  // 完全可以，而且那个“const”的“*p”也跟着变成了7。

    std::cout << "*p:" << *p << '\n';

    printf("i   指针（地址）的值为：OX: %p\n", &i);
    printf("p       （地址）的值为：OX: %p\n", p);

    p = &n;

    std::cout << "*p:" << *p << '\n';
    printf("n   指针（地址）的值为：OX: %p\n", &n);
    printf("p       （地址）的值为：OX: %p\n", p);
}
