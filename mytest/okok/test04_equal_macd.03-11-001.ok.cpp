#include <string.h>
#include <unistd.h>

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "SigSlot.h"

#define BAR_TK 0
#define BAR_1S 1
#define BAR_2S 2
#define BAR_3S 3
#define BAR_5S 4
#define BAR_10S 5
#define BAR_15S 6
#define BAR_20S 7
#define BAR_30S 8
#define BAR_1F 9
#define BAR_2F 10
#define BAR_3F 11
#define BAR_5F 12
#define BAR_10F 13
#define BAR_15F 14
#define BAR_20F 15
#define BAR_30F 16
#define BAR_1H 17
#define BAR_2H 18
#define BAR_3H 19
#define BAR_4H 20
#define BAR_5H 21
#define BAR_6H 22
#define BAR_8H 23
#define BAR_10H 24
#define BAR_12H 25
#define BAR_1D 26
#define BAR_1W 27
#define BAR_1M 28
#define BAR_1J 29
#define BAR_1Y 30

static const std::map<std::string, int> M_FF = {
    {"BAR_TK", 0},       // 0 tick!!!
    {"BAR_1S", 1},       // 1
    {"BAR_2S", 2},       // 2
    {"BAR_3S", 3},       // 3
    {"BAR_5S", 5},       // 4
    {"BAR_10S", 10},     // 5
    {"BAR_15S", 15},     // 6
    {"BAR_20S", 20},     // 7
    {"BAR_30S", 30},     // 8
    {"BAR_1F", 60},      // 9
    {"BAR_2F", 120},     // 10
    {"BAR_3F", 180},     // 11
    {"BAR_5F", 300},     // 12
    {"BAR_10F", 600},    // 13
    {"BAR_15F", 900},    // 14
    {"BAR_20F", 1200},   // 15
    {"BAR_30F", 1800},   // 16
    {"BAR_1H", 3600},    // 17
    {"BAR_2H", 7200},    // 18
    {"BAR_3H", 10800},   // 19
    {"BAR_4H", 14400},   // 20
    {"BAR_5H", 18000},   // 21
    {"BAR_6H", 21600},   // 22
    {"BAR_8H", 28800},   // 23
    {"BAR_10H", 36000},  // 24
    {"BAR_12H", 43200},  // 25
    {"BAR_1D", -1},      // 26
    {"BAR_1W", -1},      // 27
    {"BAR_1M", -1},      // 28
    {"BAR_1J", -1},      // 29
    {"BAR_1Y", -1},      // 30
    //---------- for custom define 自定义周期---------------
    {"131_cu", -1},
    {"132_cu", -1},
    {"133_cu", -1},
    {"134_cu", -1},
    {"135_xp", 3456},
    {"136_cu", -1},
    {"137_cu", -1},
    {"138_cu", -1},
    {"139_cu", -1},
    {"140_cu", -1},
    {"141_cu", -1},
    {"142_cu", -1},
    {"143_cu", -1},
    {"144_cu", -1},
    {"145_cu", -1},
    {"146_cu", -1},
    {"147_cu", -1},
    {"148_cu", -1},
    {"149_cu", -1},  // 不能超过50个周期！！ 在 base.h <struct Future> 中 写死了。
};

#define FutureNum 10
#define PeriodNum 50

// two types of data_structure:
/*   [type 1:] ********
// *   1 [futureID1 ] ---------------------------------------------------------------------------
//                 0  1   2   3   4    5    6    7    8    9   10  11  13  14 ...  ...
//                 x  1F  x   x  15F   x    x    1H   x    x   4H
//                    O           O              O             O
//                    O           O              O             O
//                                O
// *   2 [futureID1 ] ---------------------------------------------------------------------------
//                 0  1   2   3   4    5    6    7    8    9   10  11  13  14 ...  ...
//                 x  1F  x   x  15F   x    x    1H   x    x   4H
//                    O           O              O             O
//                    O           O              O             O
//                                O
//
//      "O": means: functtion like MACD(1), MACD(2), RSI(), KDJ()...
//      each <futureID,1F>( <M,N>pair below)   pair has a function_list !!!!
//
//      [type 2:] <M,N>  ********
//       1  <futureID1, 1F >  O O ........
//       2  <futureID1, 15F>  O O O O ........
//       3  <futureID1, 1H >  O O ........
//
//       4  <futureID2, 1F >  O O ........
//       5  <futureID2, 10F>  O O O O ........
//       6  <futureID2, 2H >  O O ........
//       7   ... ...
*/

// static const std::map<std::string, std::set<int>> future_map = {
static const std::map<std::string, std::vector<int>> future_map = {
    {"au2033", {0, 15, 25, 33, 45}},
    {"au2034", {0, 15, 25, 33, 45}},
};

#define fu2305 100
#define au2306 101

using lSignal::Connection;
using lSignal::Signal;
using lSignal::Slot;

Signal<void()> Update[50];

template <int... N>
class Future;

struct Unit {
    int index;  //  index搞定了
    char instrumentID[81];
    int period;
    int period_index;
    char period_name[3];
    std::array<double, 1000000> o;  // ohlcv 搞定了
    std::array<double, 1000000> h;  // ohlcv 搞定了
    std::array<double, 1000000> l;  // ohlcv 搞定了
    std::array<double, 1000000> c;  // ohlcv 搞定了
    std::array<double, 1000000> v;  // ohlcv 搞定了
    double* O;
    double* H;
    double* L;
    double* C;
    double* V;
    // todo 按照 <M,N>对 来说的话，这里只需要一个 Signal<void()>就可以了，
    // 因为它可以连接所有与M这个合约N这个周期的所有 function。
    Signal<void()> future_fun_list[1];  // 这里初始化了50个 Signal<void()>， 每个都可以connect 多个function。
    Signal<void()> sig;
};

class Base {
   public:
    // todo 这里应该是 每一个<M,N>对，有一个 Signal<void()> 对象。
    Signal<void()>* fun_list[100];
    Unit* unit_array1[100];                   // index by future_period_index_
    Unit* unit_array_[FutureNum][PeriodNum];  // index by future_period_index_
    std::map<std::vector<int>, int> future_period_index;
    char future_list[FutureNum][81];  //

    // template <int M, int N>
    // Future<int M, int N>* fu_list[100];
    // template <int M, int N>
    // std::map<int, Future<int M, int N>> mTestf;
    std::map<std::vector<int>, void*> bmap;
    std::function<void()> onInitHandler;
    std::function<void(int)> onBarsHandler;

    int FutureInit(const std::map<std::string, std::vector<int>>&, bool);
    void onInit(std::function<void()> handler) { Base::onInitHandler = handler; }
    void onBars(std::function<void(int)> handler) { Base::onBarsHandler = handler; }
    void onMessages() {
        for (int i = 0; i < 2000; i++) {
            unit_array_[0][0]->o[i] = sqrt(i * 100);
            unit_array_[0][0]->h[i] = sqrt(i * 100);
            unit_array_[0][0]->l[i] = sqrt(i * 100);
            unit_array_[0][0]->c[i] = sqrt(i * 100);
            unit_array_[0][0]->v[i] = sqrt(i * 100);
            this->onBarsHandler(i);
        }
    };
    void Run() {
        this->onInitHandler();
        this->onMessages();
    }
};

/*
 //   vector 第一个值是序号，每个品种分配一个，不能重复，从0开始， 在后面的 Future<序号，周期> 时必须使用这个序号。
 //   std::map<std::string, std::vector<int>> fuMap;
 //   fuMap["xu1801"] = {0, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
 //   fuMap["tu1801"] = {1, BAR_TK, BAR_30F, BAR_2H};
 //   fuMap["ru1805"] = {2, BAR_1S, BAR_5F, BAR_2H};
 //   fuMap["ru1809"] = {3, BAR_TK, BAR_15S, BAR_1F, BAR_5F};
 //   fuMap["xu1807"] = {4, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
 //   fuMap["zz1805"] = {5, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
 //   fuMap["ag1912"] = {6, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
 */
int Base::FutureInit(const std::map<std::string, std::vector<int>>& future_map, bool is_local_data) {
    if (future_map.empty()) {
        std::cout << " future map is empty !" << std::endl;
        return -1;
    }

    memset(future_list, 0, sizeof(future_list));

    int i = 0;
    for (auto it = (future_map).begin(); it != (future_map).end(); ++it) {  // for strategy ... future
        if (it->second.size() < 2) {
            std::cout << " period is not set !" << std::endl;
            return -1;
        }

        strncpy(future_list[i], it->first.c_str(), it->first.length());

        auto iter = it->second.cbegin();
        iter++;
        // todo

        int j = 0;
        for (; iter != it->second.cend(); iter++) {
            std::cout << it->first << " " << *iter << std::endl;
            Unit* x = new Unit();
            if (!is_local_data) {
                x->O = new double[100000];
                x->H = new double[100000];
                x->L = new double[100000];
                x->C = new double[100000];
                x->V = new double[100000];
            } else {
                //  采用本地的数据！
                // todo  这里的数据是谁来分配，如何传递到这里来？
                // x->O = ??
                // x->H ==??
            }
            x->period = *iter;
            // todo 找到 period 对应的index 值
            unit_array_[i][j] = x;
            j++;
        }

        i++;
    }
    /*
    for (i = 0; i < FutureNum; i++) {
        std::cout << future_list[i] << " ";
    }
    std::cout << std::endl;
    */
    return 0;
}

template <int... N>
class FutureBase {
   public:
    static Unit* unit;  // 这里不用static也可以， 因为 Base里只申请了一份。其它所有的Future均挂接到它即可。
    static int ohlcv_len;
    static int index;  //  index搞定了
    static char instrumentID[81];
    static int period;
    static int period_index;
    static char period_name[3];
    static std::array<double, 1000000> o;  // ohlcv 搞定了
    static std::array<double, 1000000> h;  // ohlcv 搞定了
    static std::array<double, 1000000> l;  // ohlcv 搞定了
    static std::array<double, 1000000> c;  // ohlcv 搞定了
    static std::array<double, 1000000> v;  // ohlcv 搞定了

    // todo 按照 <M,N>对 来说的话，这里只需要一个 Signal<void()>就可以了，
    // 因为它可以连接所有与M这个合约N这个周期的所有 function。
    static Signal<void()> future_fun_list[1];  // 这里初始化了1个 Signal<void()>， 每个都可以connect 多个function。
    static Signal<void()> sig;

    //  std::function<void()> handler;
    // static std::vector<std::function<void()>> ffnn;
    // 派生类的执行列表 还没有搞定。
};

template <int... N>
class FutureSerial : public FutureBase<N...> {
   public:
    FutureSerial() {
        this->data = new double[this->ohlcv_len];
        std::cout << "constructor !! futureSerial -------------------------------------------------*****" << std::endl;
    };
    FutureSerial(int a) {
        this->data = new double[this->ohlcv_len];
        this->data[this->index] = a;
        std::cout << " FutureSerial(int)  constructor ----" << std::endl;
    }

    FutureSerial(int a, const FutureSerial& p) {
        // this->m_A = a + p.m_A;
        std::cout << " FutureSerial(int, FutureSerial)  constructor ----" << std::endl;
    }
    FutureSerial(const FutureSerial& p, int a) {
        // this->m_A = a + p.m_A;
        std::cout << " FutureSerial(FutureSerial,int)  constructor ----" << std::endl;
    }

    /* 这个构造函数不能有！
     * 有了下面这个构造函数之后， 下面的 fnfn()里面的所有的 x1 x2 x3 相关的运算，都会调用这个构造函数。
     * 应该改成  operator=(const FutureSerial&p) {}  重载 = 运算符
    FutureSerial(const FutureSerial& p) {
        this->m_A = p.m_A;
        std::cout << " Persion  init !" << std::endl;
    }
    */

    // 成员函数实现 + 号运算符重载
    /*
    FutureSerial operator+(const FutureSerial& p) {
        FutureSerial temp;
        temp.m_A = this->m_A + p.m_A;
        temp.m_B = this->m_B + p.m_B;
        return temp;
    }
    */
    /*
    friend FutureSerial operator+(const FutureSerial& p2, int val) {
        FutureSerial temp;
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }

    friend FutureSerial operator+(int val, const FutureSerial& p2) {
        FutureSerial temp;  // 有没有性能问题？
        temp.m_A = p2.m_A + val;
        temp.m_B = p2.m_B + val;
        return temp;
    }
    */

    FutureSerial& operator=(int x) {
        std::cout << "赋值\n";
        this->data[this->index] = x;
        return *this;
    }
    FutureSerial& operator=(const FutureSerial& p) {
        std::cout << "赋值 p\n";
        this->data[this->index] = p.data[this->index];
        return *this;
    }

    friend inline int operator+(const FutureSerial p1, const FutureSerial p2) {
        std::cout << "p1 & p2  " << std::endl;
        return p1.data[p1.index] + p2.data[p2.index];
        // return p1.m_A + p2.m_A;
    }
    friend inline int operator+(int val, const FutureSerial p) {
        std::cout << "num & p2  " << std::endl;
        return val + p.data[p.index];
    }
    friend inline int operator+(const FutureSerial p, int val) {
        std::cout << "p2 and num  " << std::endl;
        return val + p.data[p.index];
    }

   public:
    // int m_A;
    double* data;
};

// Future是继承 FutureBase<M,N> 这里<M,N>是实例化了
// #define 5F 300
// Future<M,N>( base* bb, "ru2303", 5F)
template <int... N>
class Future : public FutureBase<N...> {
   public:
    Future(Base* base, std::string s, int period) {
        // 这里要做几件事情：
        // 【一】根据<M,N>计算出 它的号，可以根据初始化的顺序，直接采用 0,1,2,3,4,5......
        //  比如   <ru2305,5>  0
        //        <ru2306,10> 1
        //        <au2306,5>  2
        //        <au2307,15> 3
        // 【二】 把 OHLCV数组以及索引号（当前数组内容的长度）， 传给 Base ;
        // 【三】 把 function_list 传给Base ;
        //  完成以上事情后，Base在收到message时，会自动计算OHLCV，索引号+1，并放在相应的 Future所需的地方。然后调用 func_list。

        this->b = base;
        // todo 这里要根据相应的<M,N> 到 i的应对关系 ，挂接到不同的  fun_list[i] 上去。
        // 后面的执行只能是 fun_list[i]0]();
        this->b->fun_list[0] = this->future_fun_list;
        // todo 这里为什么不能& sig ?
        // this->b->Update[0] = &this->sig;

        // 把Future 强塞给 b->bmap里了。
        std::pair<std::vector<int>, void*> kkk = {{N...}, (void*)this};
        this->b->bmap.insert(kkk);
        // b->unit_array[0] = &this->unit;
        Future<N...>::unit = b->unit_array1[0];

        if constexpr (0 == sizeof...(N)) {
            std::cout << " no input in template <> !!  " << std::endl;
        } else if constexpr (1 == sizeof...(N)) {
            std::cout << " one input in template <> !!  " << std::endl;
            int tt = getone<N...>();
            std::cout << tt << "hhhhhhhhhhhhh -----one ------" << std::endl;

        } else if constexpr (2 == sizeof...(N)) {
            std::cout << " two input in template <> !!  " << std::endl;
            int tt = getone<N...>();
            std::cout << tt << "hhhhhhhhhhhhh --- two --------" << std::endl;
        }
    }

    template <int N1>
    int getone() {
        return N1;
    };

    template <int N1, int N2>
    int getone() {
        return N2;
    }
    // --- https://blog.tartanllama.xyz/if-constexpr/
    // With if constexpr
    /*
    template <int N1, int N2, int... Ns>
    int getone() {
        return N2;
    }
    */

    Future(Base* base) {
        if constexpr (0 == sizeof...(N)) {
            std::cout << " no input in template <> !!  " << std::endl;
        } else if constexpr (1 == sizeof...(N)) {
            std::cout << " one input in template <> !!  " << std::endl;
            int tt = getone<N...>();
            std::cout << tt << "hhhhhhhhhhhhh -----one ------" << std::endl;

        } else if constexpr (2 == sizeof...(N)) {
            std::cout << " two input in template <> !!  " << std::endl;
            int tt = getone<N...>();
            std::cout << tt << "hhhhhhhhhhhhh --- two --------" << std::endl;
        }
        this->b = base;
        // todo 这里要根据相应的<M,N> 到 i的应对关系 ，挂接到不同的  fun_list[i] 上去。
        // 后面的执行只能是 fun_list[i]0]();
        this->b->fun_list[0] = this->future_fun_list;
        // todo 这里为什么不能& sig ?
        // this->b->Update[0] = &this->sig;

        // 把Future 强塞给 b->bmap里了。
        std::pair<std::vector<int>, void*> kkk = {{N...}, (void*)this};
        this->b->bmap.insert(kkk);
    }
    Base* b;
};

// 下面这一堆玩意， 必须要， 不然的话，前面【 Future<N...>::unit = b->unit_array[0]; 】这种操作会出问题。

template <int... N>           // 这里很神奇啊！！！！   由M和N确定的数据，在Future内必须定义为 static,
int FutureBase<N...>::index;  // 然后在这里进行初始化。
template <int... N>           // 这里很神奇啊！！！！
int FutureBase<N...>::period;
template <int... N>  // 这里很神奇啊！！！！
Unit* FutureBase<N...>::unit;
template <int... N>  // 这里很神奇啊！！！！
int FutureBase<N...>::ohlcv_len;
template <int... N>                                   // 这里很神奇啊！！！！
std::array<double, 1000000> FutureBase<N...>::o;      // OHLCV是某一组交易基础数据。它由M、N二者一起才能唯一确定。
template <int... N>                                   // 这里很神奇啊！！！！
std::array<double, 1000000> FutureBase<N...>::h;      // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int... N>                                   // 这里很神奇啊！！！！
std::array<double, 1000000> FutureBase<N...>::l;      // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int... N>                                   // 这里很神奇啊！！！！
std::array<double, 1000000> FutureBase<N...>::c;      // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int... N>                                   // 这里很神奇啊！！！！
std::array<double, 1000000> FutureBase<N...>::v;      // 这里的数组的长度应该怎么搞？？？？ 固定吗?
template <int... N>                                   // M是指future, N是指1F 5F 等周期
Signal<void()> FutureBase<N...>::future_fun_list[1];  // 每一个<M,N>对，有一个list,里面可能有，多个 macd() 多个kdj() .....

template <int... N>
class MACD : public FutureBase<N...> {
   public:
    // 最终，还是决定将Base类，作为参数传递给 MACD，KDJ。
    // 模板参数M和N是 instrumentID 和 period。 两者合在一起才能唯一确定：一组交易数据。
    // MACD是Future的派生类。这里调用 Future<M, N>(x) 基类构造函数，初始化。
    MACD() {
        if constexpr (0 == sizeof...(N)) {
            std::cout << "" << std::endl;
        } else if constexpr (1 == sizeof...(N)) {
        } else if constexpr (2 == sizeof...(N)) {
        }
        std::cout << "macd constructor !!! -- :  " << 5 << std::endl;  // N
        // this->handler = []() { std::cout << "this.N;" << N << std::endl; };
        //   this->handler = MACD<N>::macd;
        // this->index = this->period + 1;
        Update[0].Connect(this, &MACD<N...>::macd);
        Future<N...>::future_fun_list[0].Connect(this, &MACD<N...>::macd);  //
    }

    void macd() { std::cout << "from macd(): " << 3 << " " << 3 << " " << this->o[this->index] << std::endl; };

   public:
    FutureSerial<N...> x1;
    FutureSerial<N...> x2;

    // Base* b;
};
template <int... N>
class KDJ : public FutureBase<N...> {
   public:
    KDJ() {
        Update[0].Connect(this, &KDJ<N...>::kdj);
        Future<N...>::future_fun_list[0].Connect(this, &KDJ<N...>::kdj);  //
        std::cout << "kdj  init !! ---- " << 5 << " " << 5 << std::endl;
    };
    void kdj() { std::cout << "from kdj(): " << 5 << " " << 5 << " " << this->o[this->index] << std::endl; };
};

void fn() {
    MACD<fu2305, 4> d41;
    MACD<au2306, 4> d42;
    MACD<fu2305, 4> d43;
    KDJ<au2306, 60> d60a;
    KDJ<fu2305, 60> d61b;
    KDJ<fu2305, 60> d62b;
    KDJ<fu2305, 60> d63b;
    static KDJ<fu2305, 60> d64b;
    static KDJ<fu2305, 60> d65b;
    static KDJ<fu2305, 60> d66b;
    static KDJ<fu2305, 60> d67b;
    static KDJ<fu2305, 60> d68b;
    static KDJ<fu2305, 60> d69b;
    static KDJ<fu2305, 60> d6rb;
    static KDJ<fu2305, 60> d6ub;
    static KDJ<fu2305, 60> d6eb;
    std::cout << "d41.period " << d41.period << "   y2.period " << d42.period << std::endl;
    std::cout << "d41.o[d41.index] " << d41.index << "  " << d41.o[d41.index] << "  d42.o[d42.index] " << d42.index << "  "
              << d42.o[d42.index] << std::endl;

    std::cout << "d60a.period " << d60a.period << "   d60b.period " << d67b.period << std::endl;
    std::cout << "d6eb.o[d6eb.index] " << d6eb.index << "  " << d6eb.o[d6eb.index] << "  d67b.o[d67b.index] " << d67b.index << "  "
              << d67b.o[d67b.index] << std::endl;

    // d42.macd();
}

void test_zero() {
    // static <> 没有参数的测试！ begin!
    static MACD<> m01;
    static MACD<> m02;
    static KDJ<> kk01;
    static KDJ<> kk02;
    std::cout << "macd<> 1 is : --------- " << m01.period << " " << m01.o[m01.period] << std::endl;
    std::cout << "macd<> 2 is : --------- " << m02.period << " " << m02.o[m01.period] << std::endl;
    std::cout << "kdj<> 1 is : --------- " << kk01.period << " " << kk01.o[m01.period] << std::endl;
    std::cout << "kdj<> 2 is : --------- " << kk02.period << " " << kk02.o[m01.period] << std::endl;
    // static <> 没有参数的测试！ begin!
}

void test_one() {
    // static  <10> 只有一个参数的测试！ begin!
    static MACD<10> m01;
    static MACD<20> m02;
    static KDJ<10> k01;
    static KDJ<20> k02;
    std::cout << "macd<> 1 is : --------- " << m01.period << " " << m01.o[m01.period] << std::endl;
    std::cout << "macd<> 2 is : --------- " << m02.period << " " << m02.o[m01.period] << std::endl;
    std::cout << "kdj<> 1 is : --------- " << k01.period << " " << k01.o[m01.period] << std::endl;
    std::cout << "kdj<> 2 is : --------- " << k02.period << " " << k02.o[m01.period] << std::endl;
    // static <> 没有参数的测试！ begin!
}

void test_two() {
    // static  <10,20> 两个参数的测试！ begin!
    static MACD<10, 20> m01;
    static MACD<10, 20> m02;
    static MACD<20, 20> m03;
    static MACD<20, 20> m04;
    static MACD<20, 30> m05;
    static MACD<20, 30> m06;
    static KDJ<10, 20> k01;
    static KDJ<10, 20> k02;
    static KDJ<20, 20> k03;
    static KDJ<20, 20> k04;
    static KDJ<20, 30> k05;
    static KDJ<20, 30> k06;
    std::cout << "macd<> 1 is : --------- " << m01.period << " " << m01.o[m01.period] << std::endl;
    std::cout << "macd<> 2 is : --------- " << m02.period << " " << m02.o[m02.period] << std::endl;
    std::cout << "macd<> 3 is : --------- " << m03.period << " " << m03.o[m03.period] << std::endl;
    std::cout << "macd<> 4 is : --------- " << m04.period << " " << m04.o[m04.period] << std::endl;
    std::cout << "macd<> 5 is : --------- " << m05.period << " " << m05.o[m05.period] << std::endl;
    std::cout << "macd<> 6 is : --------- " << m06.period << " " << m06.o[m06.period] << std::endl;
    std::cout << "kdj<> 1 is : --------- " << k01.period << " " << k01.o[k01.period] << std::endl;
    std::cout << "kdj<> 2 is : --------- " << k02.period << " " << k02.o[k02.period] << std::endl;
    std::cout << "kdj<> 3 is : --------- " << k03.period << " " << k03.o[k03.period] << std::endl;
    std::cout << "kdj<> 4 is : --------- " << k04.period << " " << k04.o[k04.period] << std::endl;
    std::cout << "kdj<> 5 is : --------- " << k05.period << " " << k05.o[k05.period] << std::endl;
    std::cout << "kdj<> 6 is : --------- " << k06.period << " " << k06.o[k06.period] << std::endl;
}

void test_one_x(Base* bb) {
    // template zero parameter test -------------------------------------------------------------------
    static Future<10> one1(bb);  // 产生一个type  Future<10> 和 Future<20>是不同的类型。
    static Future<10> one2(bb);  // 产生第二个type Future<20>
    static Future<20> one3(bb, "au2316", 110);
    static Future<20> one4(bb, "au2316", 110);
    for (int i = 0; i < 10; i++) {
        one1.period = i;
        one1.index = i;
        one1.o[i] = sqrt(i * 17);
        std::cout << one1.period << "  " << one1.index << " one1.period --------------------------------" << std::endl;
        std::cout << one2.period << "  " << one2.index << " one2.period --------------------------------" << std::endl;
        std::cout << one3.period << "  " << one3.index << " one3.period --------------------------------" << std::endl;
        std::cout << one4.period << "  " << one4.index << " one4.period --------------------------------" << std::endl;
        std::cout << "after one3 set data !!!" << std::endl;
        one3.period = i * 3;
        one3.index = i * 3;
        one3.o[i] = sqrt(i * 34);
        std::cout << one1.period << "  " << one1.index << " noe1.period --------------------------------" << std::endl;
        std::cout << one2.period << "  " << one2.index << " noe2.period --------------------------------" << std::endl;
        std::cout << one3.period << "  " << one3.index << " noe3.period --------------------------------" << std::endl;
        std::cout << one4.period << "  " << one4.index << " noe4.period --------------------------------" << std::endl;
        test_one();
    }
}
void test_two_x(Base* bb) {
    static Future<10, 20> two1(bb);  // 产生一个type  Future<10,20>
    static Future<10, 20> two2(bb);
    static Future<20, 20> two3(bb);  // 产生第二个type Future<20,20>
    static Future<20, 20> two4(bb);
    static Future<20, 30> two5(bb);  // 产生第三个type Future<20,30>
    static Future<20, 30> two6(bb);
    for (int i = 0; i < 10; i++) {
        two1.period = i;
        two1.index = i;
        two1.o[i] = sqrt(i * 17);
        two3.period = i * 3;
        two3.index = i * 3;
        two3.o[i * 3] = sqrt(i * 17);
        two5.period = i * 6;
        two5.index = i * 6;
        two5.o[i * 6] = sqrt(i * 17);
        std::cout << two1.period << "  " << two1.index << " two1.period --------------------------------" << std::endl;
        std::cout << two2.period << "  " << two2.index << " two2.period --------------------------------" << std::endl;
        std::cout << two3.period << "  " << two3.index << " two3.period --------------------------------" << std::endl;
        std::cout << two4.period << "  " << two4.index << " two4.period --------------------------------" << std::endl;
        std::cout << two5.period << "  " << two5.index << " two5.period --------------------------------" << std::endl;
        std::cout << two6.period << "  " << two6.index << " two6.period --------------------------------" << std::endl;
        test_two();
    }
}

void tt() { std::cout << " tt " << std::endl; };
int main() {
    static Base bb;

    Future<au2306, 4> f01(&bb, "au2306", 4);
    Future<au2306, 500> f02(&bb, "au2306", 500);
    f01.index = 51245;  // Future 会根据模板参数，分别初始化。 这里初始化了两组，分别是 <4> <500>
    f02.index = 9999;   // 每组都有自己独立的。静态成员空间。
    MACD<au2306, 4> x;
    MACD<au2306, 4> x2;
    KDJ<au2306, 500> y1;
    KDJ<au2306, 500> y2;

    std::cout << x.index << std::endl;   // 51425
    std::cout << x2.index << std::endl;  // 51425

    x.index = 1;
    std::cout << x2.index << std::endl;  // 1
    x2.index = 1000;
    std::cout << x.index << std::endl;  // 1000

    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;
    y2.index = 8888;
    std::cout << "y1.index " << y1.index << "   y2.index " << y2.index << std::endl;

    static Future<au2306, 4> b1(&bb, "au2306", 4);  // 定义了一个对象，所有的 参数为<4>的对象，共享其内部的 static成员。
    static Future<au2306, 60> b2(&bb, "au2306", 60);

    for (int i = 0; i < 10; i++) {
        b1.period = i;
        b2.period = i;
        b1.index = i;
        b2.index = i;
        b1.o[i] = sqrt(i * 2);  //  Future 基类的数据 实时变化，子类可以直接引用。
        b2.o[i] = sqrt(i * 2);  // 基类的 static 变化
        fn();                   // 派生类使用！
    }

    // template zero parameter test -------------------------------------------------------------------
    static Future<> no1(&bb, "au2306", 4);  //  no1和no2 是同一个类型
    static Future<> no2(&bb, "au2316", 100);
    Future<> no3(&bb, "au2316", 110);
    for (int i = 0; i < 10; i++) {
        no1.period = i;
        no1.index = i;
        no1.o[i] = sqrt(i * 17);
        std::cout << no2.period << "  " << no2.index << " no2.period --------------------------------" << std::endl;
        std::cout << no3.period << "  " << no3.index << " no3.period --------------------------------" << std::endl;
        test_zero();
    }

    test_one_x(&bb);
    test_two_x(&bb);

    Update[0]();
    bb.fun_list[0][0]();  // 用这种方式实现对各个<M,N>对的函数进行调用。 前面的[0]需要根据<M,N>转换成相应的值。

    int m = au2306;
    int n = 60;
    auto it = bb.bmap.find({m, n});
    if (it != bb.bmap.end()) {
        std::cout << " Find -------------------------------------------------- " << std::endl;
        // 完成转换
        auto bp = static_cast<Future<au2306, 60>*>(it->second);  // todo  这里 M，N 不能是变量！！
        bp->future_fun_list[0]();
        std::cout << bp->index << " : " << bp->o[9] << " bbbbbbbbbbbbbbbbbbbbbbpppppppppppppppppppp" << std::endl;
        // bp->o[9]

    } else {
        std::cout << "not find xiaoqiang's score" << std::endl;  // 应该输出该行
    }

    // ******************  主程序开始的地方 ***********************

    // vector 第一个值是序号，每个品种分配一个，从0开始，不能重复，不能间隔。 在后面的 Future<序号，周期> 时必须使用这个序号。

#define xu1801 0
#define tu1801 1
#define ru1805 2
#define ru1809 3
#define xu1807 4
#define zz1805 5
#define ag1912 6

#define LOCAL_DATA false

    std::map<std::string, std::vector<int>> fuMap;
    fuMap["xu1801"] = {xu1801, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
    fuMap["tu1801"] = {tu1801, BAR_TK, BAR_30F, BAR_2H};
    fuMap["ru1805"] = {ru1805, BAR_1S, BAR_5F, BAR_2H};
    fuMap["ru1809"] = {3, BAR_TK, BAR_15S, BAR_1F, BAR_5F};
    fuMap["xu1807"] = {4, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
    fuMap["zz1805"] = {5, BAR_TK, BAR_1S, BAR_1F, BAR_2H};
    fuMap["ag1912"] = {6, BAR_TK, BAR_1S, BAR_1F, BAR_2H};

    bb.FutureInit(fuMap, LOCAL_DATA);

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

    Future<au2306, 30> xp(&bb);

    bb.onBars([](int e) {
        std::cout << " from onBars data: -----------------------------------" << e << std::endl;

        // 下面的这种static定义的变量，均与 instrumentID 和 period相关。
        //  todo !!
        // 但如果是onTick() 里面，如何知道是哪个instrumentID的 Tick呢？？
        static MACD<> m0_0;
        static MACD<15> m15_0;
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

        m30_1.x1 = 10;
        m30_1.x2 = 10;

        std::cout << " *********************  " << m30_1.x1 + m30_1.x2 << std::endl;

        std::cout << "macd15-1:" << m15_1.index << " " << m15_1.o[m15_1.index] << " -- macd60_1:" << m60_1->index << "  "
                  << m60_1->o[m60_1->index] << std::endl;
        //  Update[0]();
    });

    Future<> ty(&bb);
    Future<10> ty1(&bb);
    Future<15, 20> ty2(&bb);

    std::cout << "------------------------------------------------" << std::endl;
    sleep(2);
    bb.Run();
    /*
    double* oo;
    oo = new double[1000];
    std::fill_n(oo, 1000, -3);
    for (int i = 0; i < 1000; i++) {
        std::cout << oo[i] << std::endl;
        oo[i] = i * 2;
    }
    for (int i = 0; i < 1000; i++) {
        std::cout << oo[i] << std::endl;
    }
    */
}
