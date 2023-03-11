#include <thread>
#include <iostream>
#include<stdio.h>

#define func auto 

int fun()
{
    auto f = [](std::string s1,std::string s2) {
        std::cout << s1 << s2;
    };
    f("hello ","world\n");
    auto f2 = [](std::string s1,std::string s2="hahah\n") {
        std::cout << s1 << s2;
    };
    f2("eric ");
}

func test()   // func is auto
{
    int local = 3;
    return [=](int x) {      // 相当于返回一个函数。
        return x + local ;
    };
}

int main()
{
    func f=[](int input) {
        int local=3;//这个local变量的生命周期，是在lambda声明末尾，还是整个main函数?
        return [=](int x) {
            return input+local+x;
        };
    };

    auto nn= []() {
        return 100;
    };
    std::cout << "nnnn  is:  " << nn() << std::endl;

    auto line = [](int a,int b) {
        return [a,b](int x) {     // [a,b] 可以改成[=] 表示值传递
            return  a*x + b ;
        };
    };
    auto line1 = line(3,4);
    auto line2 = line(5,6);
    std::cout << "line1 is:  " << line1(5) << std::endl;
    std::cout << "line1 is:  " << line2(5) << std::endl;

    auto f1=f(3);
    auto f2=f(4);

    auto m = test();
    int xx = m(3);
    std::cout << "xxxx is:  " << xx << std::endl;

    printf("%d,%d\n",f1(2),f2(2));

    int n1 = 500;
    int n2 = 600;
    std::thread t([&](int addNum) {
        n1 += addNum;
        n2 += addNum;
    },500);
    t.join();
    std::cout << n1 << ' ' << n2 << std::endl;

    fun();

}
