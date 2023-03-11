
#include <iostream>

using namespace std;

struct Person {
    int  num;
    char name[ 32 ];
};

void test03() {
    //    printf("num = %d\n", num);
    // num = 200;//err num只读

    // C语言中const 修饰变量名 说明变量名为只读（用户不能通过变量名data进行赋值）
    const int data = 100;  //局部只读变量 内存在栈区（内存可读可写）
    // data = 200;//err

    printf("data = %d\n", data);
    //但是：如果知道data的地址 可以通过地址间接的修改data所对应空间的内容
    int *p = (int *)&data;
    *p     = 2000;
    printf("data = %d\n", data);  // ok 200
}

void test04() {

    // 1、c++中 对于基础类型 系统不会给data开辟空间 data放到符号表中
    const int data = 10;
    // data = 100;//err 只读
    cout << "data = " << data << endl;
    // 2、c++中当 对data 取地址的时候 系统就会给data开辟空间
    int *p = (int *)&data;
    *p     = 2000;
    cout << "*p = " << *p << endl;  //空间内容修改成功 2000

    cout << "data = " << data << endl;  // data 还是10为啥？

    // 2、当以变量的形式 初始化 const修饰的变量 系统会为其开辟空间
    int       b = 200;
    const int a = b;  //系统直接为a开辟空间 而不会把a放入符号表中
    p           = (int *)&a;
    *p          = 3000;
    cout << "*p = " << *p << endl;  // 3000
    cout << "a = " << a << endl;    // 3000

    // 3、const 自定义数据类型(结构体、对象) 系统会分配空间
    const Person per = {100, "lucy"};
    // per.num = 1000;//err
    cout << "num = " << per.num << ", name = " << per.name << endl;  // 100 lucy
    Person *p1 = (Person *)&per;
    p1->num    = 2000;
    cout << "num = " << per.num << ", name = " << per.name << endl;  // 2000 lucy
}

int main() {
    test03();
    std::cout << " -----------------\n";
    test04();
    std::cout << " -----------------\n";

    const volatile int t = 8888;
    int                n = 1;
    n                    = 5;

    int d  = 519;
    int d1 = d;
    int d2 = (int &&)d;
    int d3 = (int &)d;

    std::cout << "d d1 d2 d3:" << d << " " << d1 << " " << d2 << " " << d3 << "\n\n";

    int *p = (int *)&t;
    *p     = 9999;

    int *pn = &n;
    // int* pt = &t;

    int &k = n;
    // int& s = x;

    int tx = t + 100;
    std::cout << "tx:" << tx << '\n';
    std::cout << "d:" << d << '\n';

    std::cout << "---    t:" << t << std::endl;
    std::cout << "----  *p:" << *p << std::endl;
    std::cout << "(int&&)t:" << (int &&)t << std::endl;
    std::cout << " (int&)t:" << (int &)t << std::endl;

    std::cout << "n: " << n << std::endl;
    //    std::cout << "*ptr1:" << *ptr1 << std::endl;

    printf("t   值为：OX: %d\n", t);
    printf("t   指针（地址）的值为：OX: %p\n", &t);
    printf("p       （地址）的值为：OX: %p\n", p);

    printf("n   指针（地址）的值为：OX: %p\n", &n);
    // printf("ptr1指针（地址）的值为：OX: %p\n", ptr1);
    printf("k   指针（地址）的值为：OX: %p\n", &k);
    // printf("ptr1指针（地址）的值为：OX: %p\n", ptr1);

    /*
    printf("z1 指针（地址）的值为：OX: %p\n", z1);
    printf("&z1 指针（地址）的值为：OX: %p\n", &z1);
    printf("&z1 指针（地址）的值为：OX: %p\n", &z1);
    printf("z2 的值为： %d\n", z2);
    printf("z2 指针（地址）的值为：OX: %p\n", &z2);
    printf("z3 指针（地址）的值为：OX: %p\n", z3);
    */

    /*
    int& x1 = x;
    x1      = 9;

    const int* p = &x;
    *p           = 9;
    */
}
