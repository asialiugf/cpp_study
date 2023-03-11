#include <unistd.h>

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <vector>

#include "SigSlot.h"

#define fu2305 100
#define au2306 101

using lSignal::Connection;
using lSignal::Signal;
using lSignal::Slot;

Signal<void()> Update[50];

class Base {
   public:
    // Signal<void()> Update[50];
    Signal<void()>* fun_list[100];

    std::function<void()> onInitHandler;
    std::function<void(int)> onBarsHandler;

    void onInit(std::function<void()> handler) { Base::onInitHandler = handler; }
    void onBars(std::function<void(int)> handler) { Base::onBarsHandler = handler; }
    void onMessages() {
        for (int i = 0; i < 2000; i++) {
            this->onBarsHandler(i);
        }
    };
    void Run() {
        this->onInitHandler();
        this->onMessages();
    }
};

template <int M, int N>
class Future {
   public:
    static int index;  //  index搞定了
    static int hello;
    static std::array<double, 1000000> o;  // ohlcv 搞定了
    static std::array<double, 1000000> h;  // ohlcv 搞定了
    static std::array<double, 1000000> l;  // ohlcv 搞定了
    static std::array<double, 1000000> c;  // ohlcv 搞定了
    static std::array<double, 1000000> v;  // ohlcv 搞定了
    // todo 按照 <M,N>对 来说的话，这里只需要一个 Signal<void()>就可以了，
    // 因为它可以连接所有与M这个合约N这个周期的所有 function。
    static Signal<void()> future_fun_list[50];  // 这里初始化了50个 Signal<void()>， 每个都可以connect 多个function。
    Future(Base* x) {
        this->b = x;
        this->b->fun_list[0] = future_fun_list;
    }
    Base* b;

    //  std::function<void()> handler;
    // static std::vector<std::function<void()>> ffnn;
    // 派生类的执行列表 还没有搞定。
};

template <int M, int N>   // 这里很神奇啊！！！！   由M和N确定的数据，在Future内必须定义为 static,
int Future<M, N>::index;  // 然后在这里进行初始化。
template <int M, int N>   // 这里很神奇啊！！！！
int Future<M, N>::hello;
template <int M, int N>                            // 这里很神奇啊！！！！
std::array<double, 1000000> Future<M, N>::o;       // OHLCV是某一组交易基础数据。它由M、N二者一起才能唯一确定。
template <int M, int N>                            // 这里很神奇啊！！！！
std::array<double, 1000000> Future<M, N>::h;       // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int M, int N>                            // 这里很神奇啊！！！！
std::array<double, 1000000> Future<M, N>::l;       // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int M, int N>                            // 这里很神奇啊！！！！
std::array<double, 1000000> Future<M, N>::c;       // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int M, int N>                            // 这里很神奇啊！！！！
std::array<double, 1000000> Future<M, N>::v;       // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int M, int N>                            // M是指future, N是指1F 5F 等周期
Signal<void()> Future<M, N>::future_fun_list[50];  // 每一个<M,N>对，有一个list,里面可能有，多个 macd() 多个kdj() .....

template <int M, int N>
class MACD : public Future<M, N> {
   public:
    // 最终，还是决定将Base类，作为参数传递给 MACD，KDJ。
    // 模板参数M和N是 instrumentID 和 period。 两者合在一起才能唯一确定：一组交易数据。
    // MACD是Future的派生类。这里调用 Future<M, N>(x) 基类构造函数，初始化。
    MACD(Base* b) : Future<M, N>(b) {
        std::cout << "macd constructor !!! -- :  " << N << std::endl;
        // this->handler = []() { std::cout << "this.N;" << N << std::endl; };
        //   this->handler = MACD<N>::macd;
        // this->index = this->hello + 1;
        Update[0].Connect(this, &MACD<M, N>::macd);
        Future<M, N>::future_fun_list[0].Connect(this, &MACD<M, N>::macd);  //
    }

    void macd() { std::cout << "from macd(): " << M << " " << N << " " << this->o[this->index] << std::endl; };

    // Base* b;
};
template <int M, int N>
class KDJ : public Future<M, N> {
   public:
    KDJ(Base* x) : Future<M, N>(x) {
        Update[0].Connect(this, &KDJ<M, N>::kdj);
        Future<M, N>::future_fun_list[0].Connect(this, &KDJ<M, N>::kdj);  //
        std::cout << "kdj  init !! ---- " << M << " " << N << std::endl;
    };
    void kdj() { std::cout << "from kdj(): " << M << " " << N << " " << this->o[this->index] << std::endl; };
};

void fn(Base* x) {
    MACD<fu2305, 4> d41(x);
    MACD<au2306, 4> d42(x);
    MACD<fu2305, 4> d43(x);
    KDJ<au2306, 60> d60a(x);
    KDJ<fu2305, 60> d61b(x);
    KDJ<fu2305, 60> d62b(x);
    KDJ<fu2305, 60> d63b(x);
    static KDJ<fu2305, 60> d64b(x);
    static KDJ<fu2305, 60> d65b(x);
    static KDJ<fu2305, 60> d66b(x);
    static KDJ<fu2305, 60> d67b(x);
    static KDJ<fu2305, 60> d68b(x);
    static KDJ<fu2305, 60> d69b(x);
    static KDJ<fu2305, 60> d6rb(x);
    static KDJ<fu2305, 60> d6ub(x);
    static KDJ<fu2305, 60> d6eb(x);
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
    static Base bb;

    Future<au2306, 4>::index = 51245;   // Future 会根据模板参数，分别初始化。 这里初始化了两组，分别是 <4> <500>
    Future<au2306, 500>::index = 9999;  // 每组都有自己独立的。静态成员空间。
    MACD<au2306, 4> x(&bb);
    MACD<au2306, 4> x2(&bb);
    KDJ<au2306, 500> y1(&bb);
    KDJ<au2306, 500> y2(&bb);

    std::cout << x.index << std::endl;   // 51425
    std::cout << x2.index << std::endl;  // 51425

    x.index = 1;
    std::cout << x2.index << std::endl;  // 1
    x2.index = 1000;
    std::cout << x.index << std::endl;  // 1000

    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;
    y2.index = 8888;
    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;

    static Future<au2306, 4> b1(&bb);  // 定义了一个对象，所有的 参数为<4>的对象，共享其内部的 static成员。
    static Future<au2306, 60> b2(&bb);

    for (int i = 0; i < 10; i++) {
        b1.hello = i;
        b2.hello = i;
        b1.index = i;
        b2.index = i;
        b1.o[i] = sqrt(i * 2);  //  Future 基类的数据 实时变化，子类可以直接引用。
        b2.o[i] = sqrt(i * 2);  // 基类的 static 变化
        fn(&bb);                // 派生类使用！
    }

    Update[0]();

    // here is the start of a strategy ------------------------------------------------------
    bb.onInit([]() {
        std::cout << "bb.init! " << std::endl;
        // Future<15> f15;
        // Future<30> f30;
        // Future<60> f60;
        /*
        static MACD<au2306, 15> m15_1;
        static MACD<au2306, 15> m15_2;
        static MACD<au2306, 15> m15_3;
        static MACD<au2306, 30> m30_1;
        static MACD<au2306, 30> m30_2;
        static MACD<au2306, 30> m30_3;
        static auto m60_1 = new MACD<au2306, 60>;
        static MACD<au2306, 60> m60_2;
        static MACD<au2306, 60> m60_3;
        static auto m7 = new MACD<au2306, 7>;  // 采用这种方式， 构造函数也只运行一次。
        static auto m15 = new MACD<au2306, 15>;
        static auto m19 = new MACD<au2306, 19>;
        static auto m21 = new MACD<au2306, 21>;
        */
    });
    bb.onBars([](int e) {
        std::cout << " from onBars data: -----------------------------------" << e << std::endl;

        // 下面的这种static定义的变量，均与 instrumentID 和 period相关。
        //  todo !!
        // 但如果是onTick() 里面，如何知道是哪个instrumentID的 Tick呢？？
        static MACD<au2306, 15> m15_1(&bb);
        static MACD<au2306, 15> m15_2(&bb);
        static MACD<au2306, 15> m15_3(&bb);
        static MACD<au2306, 30> m30_1(&bb);
        static MACD<au2306, 30> m30_2(&bb);
        static MACD<au2306, 30> m30_3(&bb);
        static auto m60_1 = new MACD<au2306, 60>(&bb);
        static MACD<au2306, 60> m60_2(&bb);
        static MACD<au2306, 60> m60_3(&bb);
        static auto m7 = new MACD<au2306, 7>(&bb);  // 采用这种方式， 构造函数也只运行一次。
        static auto m15 = new MACD<au2306, 15>(&bb);
        static auto m19 = new MACD<au2306, 19>(&bb);
        static auto m21 = new MACD<au2306, 21>(&bb);

        std::cout << "macd15-1:" << m15_1.index << " " << m15_1.o[m15_1.index] << " -- macd60_1:" << m60_1->index << "  "
                  << m60_1->o[m60_1->index] << std::endl;
        //  Update[0]();
    });

    std::cout << "------------------------------------------------" << std::endl;
    sleep(2);
    bb.Run();
}
