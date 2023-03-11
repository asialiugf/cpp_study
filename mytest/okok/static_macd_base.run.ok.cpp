#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "SigSlot.h"

using lSignal::Connection;
using lSignal::Signal;
using lSignal::Slot;

Signal<void()> Update[50];

class Base {
   public:
    Signal<void()> Update[50];

    std::function<void()> onInitHandler;
    std::function<void(int)> onBarsHandler;

    void onInit(std::function<void()> handler) { Base::onInitHandler = handler; }
    void onBars(std::function<void(int)> handler) { Base::onBarsHandler = handler; }
    void onMessages() {
        for (int i = 0; i < 100; i++) {
            this->onBarsHandler(i);
        }
    };
    void Run() {
        this->onInitHandler();
        this->onMessages();
    }
};

template <int N>
class Future {
   public:
    static int index;  //  index搞定了
    static int hello;
    static std::array<double, 1000000> o;  // ohlcv 搞定了
    static std::array<double, 1000000> h;  // ohlcv 搞定了
    static std::array<double, 1000000> l;  // ohlcv 搞定了
    static std::array<double, 1000000> c;  // ohlcv 搞定了
    static std::array<double, 1000000> v;  // ohlcv 搞定了
    Signal<void()> Update[50];

    //  std::function<void()> handler;
    // static std::vector<std::function<void()>> ffnn;
    // 派生类的执行列表 还没有搞定。
};

template <int N>  // 这里很神奇啊！！！！
int Future<N>::index;
template <int N>  // 这里很神奇啊！！！！
int Future<N>::hello;
template <int N>                           // 这里很神奇啊！！！！
std::array<double, 1000000> Future<N>::o;  // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int N>                           // 这里很神奇啊！！！！
std::array<double, 1000000> Future<N>::h;  // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int N>                           // 这里很神奇啊！！！！
std::array<double, 1000000> Future<N>::l;  // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int N>                           // 这里很神奇啊！！！！
std::array<double, 1000000> Future<N>::c;  // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int N>                           // 这里很神奇啊！！！！
std::array<double, 1000000> Future<N>::v;  // 这里的数组的长度应该怎么搞？？？？ 固定吗?

template <int N>
class MACD : public Future<N> {
   public:
    MACD() {
        std::cout << "d1!" << std::endl;
        // this->handler = []() { std::cout << "this.N;" << N << std::endl; };
        //   this->handler = MACD<N>::macd;
        // this->index = this->hello + 1;
        Update[0].Connect(this, &MACD<N>::macd);
    }

    void macd() { std::cout << "from macd(): " << N << " " << this->o[this->index] << std::endl; };
};
template <int N>
class KDJ : public Future<N> {
   public:
    KDJ() { std::cout << "d2! init !!" << std::endl; };
};

void fn() {
    MACD<4> d41;
    MACD<4> d42;
    MACD<4> d43;
    KDJ<60> d60a;
    KDJ<60> d61b;
    KDJ<60> d62b;
    KDJ<60> d63b;
    static KDJ<60> d64b;
    static KDJ<60> d65b;
    static KDJ<60> d66b;
    static KDJ<60> d67b;
    static KDJ<60> d68b;
    static KDJ<60> d69b;
    static KDJ<60> d6rb;
    static KDJ<60> d6ub;
    static KDJ<60> d6eb;
    std::cout << "d41.hello " << d41.hello << "   y2.hello " << d42.hello << std::endl;
    std::cout << "d41.o[d41.index] " << d41.index << "  " << d41.o[d41.index] << "  d42.o[d42.index] " << d42.index << "  "
              << d42.o[d42.index] << std::endl;

    std::cout << "d60a.hello " << d60a.hello << "   d60b.hello " << d67b.hello << std::endl;
    std::cout << "d6eb.o[d6eb.index] " << d6eb.index << "  " << d6eb.o[d6eb.index] << "  d67b.o[d67b.index] " << d67b.index << "  "
              << d67b.o[d67b.index] << std::endl;

    // d42.macd();
}

void tt() { std::cout << " tt " << std::endl; };
int main() {
    Future<4>::index = 51245;   // Future 会根据模板参数，分别初始化。 这里初始化了两组，分别是 <4> <500>
    Future<500>::index = 9999;  // 每组都有自己独立的。静态成员空间。
    MACD<4> x;
    MACD<4> x2;
    // KDJ<60>
    KDJ<500> y1;
    KDJ<500> y2;

    std::cout << x.index << std::endl;   // 51425
    std::cout << x2.index << std::endl;  // 51425

    x.index = 1;
    std::cout << x2.index << std::endl;  // 1
    x2.index = 1000;
    std::cout << x.index << std::endl;  // 1000

    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;
    y2.index = 8888;
    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;

    static Future<4> b1;  // 定义了一个对象，所有的 参数为<4>的对象，共享其内部的 static成员。
    static Future<60> b2;

    for (int i = 0; i < 10; i++) {
        b1.hello = i;
        b2.hello = i;
        b1.index = i;
        b2.index = i;
        b1.o[i] = sqrt(i * 2);  //  Future 基类的数据 实时变化，子类可以直接引用。
        b2.o[i] = sqrt(i * 2);  // 基类的 static 变化

        fn();  // 派生类使用！
    }
    std::function<void()> f;
    // f = []() { std::cout << " hh " << std::endl; };
    f = &tt;
    f();
    Update[0]();

    static Base bb;
    bb.onInit([]() {
        std::cout << "bb.init! " << std::endl;
        Future<15> f15;
        Future<30> f30;
        Future<60> f60;
    });
    bb.onBars([](int e) {
        std::cout << " from onBars data:" << e << std::endl;
        static MACD<15> m15_1;
        static MACD<15> m15_2;
        static MACD<15> m15_3;
        static MACD<30> m30_1;
        static MACD<30> m30_2;
        static MACD<30> m30_3;
        static MACD<60> m60_1;
        static MACD<60> m60_2;
        static MACD<60> m60_3;
        std::cout << "macd15-1:" << m15_1.index << " " << m15_1.o[m15_1.index] << " -- macd60_1:" << m60_1.index << "  "
                  << m60_1.o[m60_1.index] << std::endl;
    });
    bb.Run();
}
