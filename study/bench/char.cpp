#include <stdio.h>
#include <string.h>

#include <cstddef>
#include <iostream>

using namespace std;

int main() {
    int  z = 1;
    int *z1;
    int &z2 = z;

    int        e  = 100;
    int        e1 = 100;
    int        i  = 2;
    const int &i1 = i;
    const int *e2 = &e;
    const int *e3 = &e1;

    int &iii = i;

    int *const e4 = &z;

    e2 = &z;

    char        a   = '?';
    char *      p1  = "==============";
    const char *pp  = "asialine";
    const char *pp1 = "--------------------------------------";
    const char *pp2 = "-------------------------------------";
    const char *pp3 = "hello world";
    char        c[] = "abcdasdffffffff";

    char *p;
    p = &a;

    z1       = &z;
    int *&z3 = z1;  // z1必须初始化。

    std::cout << "--------------------------------" << std::endl;
    printf("z 指针（地址）的值为：OX: %p\n", &z);
    printf("z1 指针（地址）的值为：OX: %p\n", z1);
    printf("&z1 指针（地址）的值为：OX: %p\n", &z1);
    printf("&z1 指针（地址）的值为：OX: %p\n", &z1);
    printf("z2 的值为： %d\n", z2);
    printf("z2 指针（地址）的值为：OX: %p\n", &z2);
    printf("z3 指针（地址）的值为：OX: %p\n", z3);
    std::cout << "z:" << z << std::endl;
    std::cout << "&z:" << &z << std::endl;
    std::cout << "z1:" << z1 << std::endl;
    std::cout << "* z1:" << *z1 << std::endl;
    std::cout << "& z1:" << &z1 << std::endl;
    std::cout << "(void *)&z:" << (void *)&z << std::endl;
    std::cout << "(void *)z1:" << (void *)z1 << std::endl;
    std::cout << "(void *)&z1:" << (void *)&z1 << std::endl;
    std::cout << "--------------------------------" << std::endl;

    printf("p 指针（地址）的值为：OX: %p\n", p);
    printf("p1 指针（地址）的值为：OX: %p\n", p1);
    printf("pp 指针（地址）的值为：OX: %p\n", pp);
    printf("pp1 指针（地址）的值为：OX: %p\n", pp1);
    printf("pp2 指针（地址）的值为：OX: %p\n", pp2);
    printf("pp3 指针（地址）的值为：OX: %p\n", pp3);
    printf("a& 指针（地址）的值为：OX: %p\n", &a);
    printf("c 指针（地址）的值为：OX: %p\n", c);
    printf("p1 is: %s\n", p1);
    printf("pp is: %s\n", pp);
    printf("c is: %s\n", c);
    printf("*p1 is: %c\n", *p1);
    printf("*pp is: %c\n", *pp);
    printf("c[0] is: %c\n", c[ 0 ]);

    std::cout << &z << "-------&z\n";
    std::cout << (void *)&z << "-------void * &z\n";
    std::cout << &a << "-------&a\n";
    std::cout << &p1 << "-------&p1\n";
    std::cout << &c << "-------&c\n";
    std::cout << &p << "----p = &a   ---&p\n";
    std::cout << (void *)&a << "------void * &a" << std::endl;
    std::cout << (void *)p << "------void * p" << std::endl;
    std::cout << (void *)pp3 << "------void * pp3" << std::endl;

    std::cout << "*c:" << *c << std::endl;
    std::cout << "*c+2:" << *(c + 2) << std::endl;

    //    pp = &a;
    //   pp = p;

    const char *px;
    px = pp3;
    std::cout << "(void *)px:" << (void *)px << std::endl;
    std::cout << "px:" << px << std::endl;
    std::cout << "*px:" << *px << std::endl << std::endl;
    px = c;
    std::cout << "(void *)px:" << (void *)px << std::endl;
    std::cout << "px:" << px << std::endl;
    std::cout << "*px:" << *px << std::endl;

    std::cout << "a:" << a << std::endl;
    std::cout << "&a:" << &a << std::endl;
    std::cout << "(void *) a:" << (void *)&a << std::endl;

    std::cout << "*p:" << *p << std::endl;
    std::cout << "p:" << p << std::endl;
    std::cout << "&p:" << &p << std::endl;
    std::cout << "(void *) p:" << (void *)p << std::endl;

    std::cout << "*pp:" << *pp << std::endl;
    std::cout << "pp:" << pp << std::endl;
    std::cout << "&pp:" << &pp << std::endl;

    std::cout << "*p1:" << *p1 << std::endl;
    std::cout << "p1:" << p1 << std::endl;
    std::cout << "&p1:" << &p1 << std::endl;

    p = p1;
    std::cout << "*p:" << *p << std::endl;
    std::cout << "p:" << p << std::endl;
    std::cout << "&p:" << &p << std::endl;

    std::cout << "a:" << a << std::endl;
    std::cout << "&a:" << &a << std::endl;

    std::cout << sizeof(char) << std::endl;
    std::cout << (void *)"Hello World"
              << "         : hello \n";
    std::cout << (void *)"Hello World"
              << "         : hello \n";
    std::cout << (void *)"Hello World"
              << "         : hello \n";
    std::cout << (void *)"Hello World"
              << "         : hello \n";
    std::cout << (void *)"Hello World"
              << "         : hello \n";

    // char x = "a";
    // const char *b = "aaaa";
    const char *b = "-xyzt-";

    p = c;
    std::cout << "after p =c p:" << p << std::endl;

    char d = a;
    // char d = *(b + 1);

    char *m = &d;
    char *n = &a;
    *n      = 'b';
    d       = 'c';
    memcpy(c + 4, b + 1, 5);

    std::cout << *b << std::endl;
    std::cout << b << std::endl;
    std::cout << "*(b+1): " << *(b + 1) << std::endl;
    std::cout << c << std::endl;
    std::cout << "d:" << d << std::endl;
    std::cout << "a:" << a << std::endl;

    std::cout << &b << std::endl;
    std::cout << static_cast<void *>(c) << std::endl;
    std::cout << static_cast<void *>(&d) << std::endl;
    std::cout << &c << std::endl;
    std::cout << &m << std::endl;
    std::cout << static_cast<void *>(n) << std::endl;
    std::cout << static_cast<void *>(&a) << std::endl;
    std::cout << *(&a) << std::endl;
    std::cout << "-----------------\n";
    std::cout << b;

    char *str = "abcdasdffffffff";
    std::cout << str << std::endl;         //"abcdasdffffffff"
    std::cout << *str << std::endl;        // a
    std::cout << *(str + 2) << std::endl;  // c
    std::cout << str[ 0 ] << std::endl;    // a
    std::cout << str[ 2 ] << std::endl;    // c
    std::cout << &str << std::endl;        // c这个
    std::cout << (void *)str << std::endl;
    std::cout << static_cast<void *>(str) << std::endl;
    printf("str 指针（地址）的值为：OX: %p\n", str);

    char str1[] = "asialine";
    std::cout << str1 << std::endl;         //"abcdasdffffffff"
    std::cout << *str1 << std::endl;        // a
    std::cout << *(str1 + 2) << std::endl;  // c
    std::cout << str1[ 0 ] << std::endl;    // a
    std::cout << str1[ 2 ] << std::endl;    // c
    std::cout << &str1 << std::endl;        // c这个
    std::cout << (void *)str1 << std::endl;
    std::cout << static_cast<void *>(str1) << std::endl;
    printf("str1 指针（地址）的值为：OX: %p\n", str1);
}
