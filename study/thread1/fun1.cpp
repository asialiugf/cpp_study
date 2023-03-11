/*
 * File:   main.cpp
 * Author: Vicky.H
 * Email:  eclipser@163.com
 */
#include <iostream>
#include <functional>
#include <typeinfo>
#include <string.h>

int add1(int i, int j, int k)
{
    return i + j + k;
}


class Utils
{
public:
    Utils(const char* name)
    {
        strcpy(_name, name);
    }

    void sayHello(const char* name) const
    {
        std::cout << _name << " say: hello " << name << std::endl;
    }

    static int getId()
    {
        return 10001;
    }

    int operator()(int i, int j, int k) const
    {
        return i + j + k;
    }

private:
    char _name[32];
};


/*
 *
 */
int main(void)
{

    // 绑定全局函数
    auto add2 = std::bind(add1, std::placeholders::_1, std::placeholders::_2, 10);
    // 函数add2 = 绑定add1函数，参数1不变，参数2不变，参数3固定为10.
    std::cout << typeid(add2).name() << std::endl;
    std::cout << "add2(1,2) = " << add2(1, 2) << std::endl;

    std::cout << "\n---------------------------" << std::endl;

    // 绑定成员函数
    Utils utils("Vicky");
    auto sayHello = std::bind(&Utils::sayHello, utils/*调用者*/, std::placeholders::_1/*参数1*/);
    sayHello("Jack");

    auto sayHelloToLucy = std::bind(&Utils::sayHello, utils/*调用者*/, "Lucy"/*固定参数1*/);
    sayHelloToLucy();

    // 绑定静态成员函数
    auto getId = std::bind(&Utils::getId);
    std::cout << getId() << std::endl;

    std::cout << "\n---------------------------" << std::endl;

    // 绑定operator函数
    auto add100 = std::bind(&Utils::operator(), utils, std::placeholders::_1, std::placeholders::_2, 100);
    std::cout << "add100(1, 2) = " << add100(1, 2) << std::endl;

    // 注意：无法使用std::bind()绑定一个重载函数

    return 0;
}
