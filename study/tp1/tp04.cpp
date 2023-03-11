
#include <cstdint>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>

#if USE_BOOST
#include <boost/function.hpp>
#include <boost/bind.hpp>
#endif


class FunctorClass
{
public:
    // The required constructor for this example.
    explicit FunctorClass(uint64_t& evenCount)
        : m_evenCount(evenCount)
    {
    }

    int get_count()
    {
        return m_evenCount;
    }

    // The function-call operator prints whether the number is
    // even or odd. If the number is even, this method updates
    // the counter.
    void operator()(int n) const
    {
        //std::cout << n << std::endl;
        m_evenCount += n;
        //std::cout << m_evenCount << std::endl;
    }

private:
    // Default assignment operator to silence warning C4512.
    FunctorClass& operator=(const FunctorClass&);

    uint64_t &m_evenCount; // the number of even variables in the vector.  这里是不是有问题？
};

class timer
{
public:
    typedef std::chrono::high_resolution_clock clock;
    typedef clock::time_point                  time_point;
    typedef clock::duration                    duration;

public:
    timer()
    {
        reset();
    }

    void reset()
    {
        _starttime = clock::now();
    }

    duration elapsed() const
    {
        return clock::now() - _starttime;
    }
protected:
    time_point _starttime;
};

bool test_timer()
{
    using std::chrono::milliseconds;
    typedef timer::duration duration;

    const milliseconds sleep_time(500);

    timer t;
    std::this_thread::sleep_for(sleep_time);
    duration recorded = t.elapsed();

    // make sure the clock and this_thread::sleep_for is precise within one millisecond (or at least in agreement as to
    // how inaccurate they are)
    return (recorded - milliseconds(1) < sleep_time)
           && (recorded + milliseconds(1) > sleep_time);
}

template <typename T>
void volatile_write(const T& x)
{
    volatile T* p = new T;
    *p = x;
    delete p;
}

template <typename Function>
void run_test(const std::string& name, Function func)
{
    std::cout << name;
    timer t;
    volatile_write(func());
    timer::duration duration = t.elapsed();
    std::cout << '\t' << duration.count() << std::endl;
}

template <typename Function>
void do_test_loop(Function func, const uint64_t upper_limit = 1ULL)
{
    uint64_t i;
    for(i = 0; i < upper_limit; ++i)
        func(i);
    if(i == upper_limit) {
        std::cout<<i;
    }
}

uint64_t x1 = 0;
uint64_t x2 = 0;

uint64_t low_level( uint64_t x) {
   x1 = x;
   x2 = x1 + x ; 
}

uint64_t test_low_level()
{
    uint64_t x = 0;
    do_test_loop(low_level);
    return x;
}

uint64_t test_accumulate_lambda()
{
    uint64_t x = 0;
    auto accumulator = [&x](uint64_t i) {
        x += i;
    };
    do_test_loop(accumulator);
    return x;
}

void test_accumulate_bind_function(uint64_t& x, uint64_t i)
{
    x += i;
}

uint64_t test_accumulate_bind()
{
    namespace arg = std::placeholders;

    uint64_t x = 0;
    std::function<void (uint64_t)> accumulator = std::bind(&test_accumulate_bind_function, std::ref(x), arg::_1);
    do_test_loop(accumulator);
    return x;
}

uint64_t test_accumulate_bound_lambda()
{
    uint64_t x = 0;
    std::function<void (uint64_t)> accumulator = [&x](uint64_t i) {
        x += i;
    };
    do_test_loop(accumulator);
    return x;
}


uint64_t test_accumulate_class_function()
{
    uint64_t x = 0;

    do_test_loop(FunctorClass(x));
    // for_each(v.begin(), v.end(), FunctorClass(x));
    //uint64_t mmm = FunctorClass::get_count() ; 
    //std:: cout << mmm;
    return x;
}

uint64_t test_accumulate_bind_auto()
{
    namespace arg = std::placeholders;

    uint64_t x = 0;
    auto accumulator = std::bind(&test_accumulate_bind_function, std::ref(x), arg::_1);
    do_test_loop(accumulator);
    return x;
}

#if USE_BOOST
uint64_t test_accumulate_boost_bind()
{
    uint64_t x = 0;

    boost::function<void (uint64_t)> accumulator = boost::bind(&test_accumulate_bind_function, boost::ref(x), _1);
    do_test_loop(accumulator);
    return x;
}

uint64_t test_accumulate_boost_bound_lambda()
{
    uint64_t x = 0;
    boost::function<void (uint64_t)> accumulator = [&x](uint64_t i) {
        x += i;
    };
    do_test_loop(accumulator);
    return x;
}
#endif

int main()
{
    if(!test_timer()) {
        std::cout << "Failed timer test." << std::endl;
        return -1;
    }

    run_test("Accumulate (lambda)            ", &test_accumulate_lambda);
    run_test("Accumulate (bind)              ", &test_accumulate_bind);
    run_test("Accumulate (bound lambda)      ", &test_accumulate_bound_lambda);
    run_test("Accumulate (Function Object)    ", &test_accumulate_class_function);
    run_test("Accumulate (bind auto)    ", &test_accumulate_bind_auto);
    run_test("Accumulate (lowlevel auto)    ", &test_low_level);
    std::cout << "x2 is:  " << x2 << std::endl;
#if USE_BOOST
    run_test("Accumulate (boost bind)        ", &test_accumulate_boost_bind);
    run_test("Accumulate (boost bound lambda)", &test_accumulate_bound_lambda);
#endif
}
