#include <iostream>
#include <functional>
#include <string>
#include <chrono>
#include <vector>

template <typename F>
float calc1(F f)
{
    return -1.0f * f(3.3f) + 666.0f;
}
float calc2(std::function<float(float)> f)
{
    return -1.0f * f(3.3f) + 666.0f;
}

//template <typename X>
template <typename F>
int cal1(F f)
{
    return 3+f(10);
}
int cal2(std::function<int(int)> f)
{
    return 3+f(10);
}

typedef void(* EV)(char *) ;

typedef struct {
    int a;
} t1_t;

typedef struct {
    int a;
    double b;
} t2_t;

void t1(char *b)
{
    t1_t *x;
    x = (t1_t *) b;
    std::cout << "in t1 a:: " << x->a <<std::endl;
}

void t2(char *b)
{
    t2_t *x;
    x = (t2_t *) b;
    std::cout << "in t2 a:: " << x->a <<std::endl;
    std::cout << "in t2 b:: " << x->b <<std::endl;
}

int main()
{
    using namespace std::chrono;

    const auto tp1 = system_clock::now();
    for(int i = 0; i < 1e8; ++i) {
        calc1([](float arg) {
            return arg * 0.5f;
        });
    }
    const auto tp2 = high_resolution_clock::now();
    const auto d = duration_cast<milliseconds>(tp2 - tp1);
    std::cout << d.count() << std::endl;

    std::cout << calc1([](float arg) {
        return arg * 0.5f;
    }) << std::endl;
    std::cout << calc2([](float arg) {
        return arg * 0.5f;
    }) << std::endl;

    std::cout << cal1([](int arg) {
        return arg * 15;
    }) << std::endl;
    std::cout << cal2([](int arg) {
        return arg * 15;
    }) << std::endl;

    // --------------------

    std::vector<EV> evv ;
    evv.push_back(t1);
    evv.push_back(t2);

    t2_t ttt;
    ttt.a = 1;
    ttt.b =199.25;

    for(int i=0 ; i<evv.size(); i++) {
        evv[i]((char *)&ttt);
    }




    return 0;
}
