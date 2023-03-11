#include <bits/stdint-intn.h>
#include <time.h>
//#include <unistd.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>

using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

high_resolution_clock::time_point t1, t2;

inline uint64_t rdtsc() {
    return __builtin_ia32_rdtsc();
}
inline uint64_t now1() {
    return rdtsc();
}

int        n = 0;
int        r = 0;
static int x = -1;

struct tick {
    int     i;
    int64_t b;
    char    cc[ 100 ];
};

tick T[ 1000000 ];

void write() {
    for (int i = 0; i < 10000; i++) {
        T[ n ].i = 2 * n;
        for (int z = 0; z < 100; z++) {
            T[ n ].cc[ z ] = 'a';
        }
        T[ n ].b = now1();
        n++;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // t1 = high_resolution_clock::now();
        // std::cout << "n:" << n << std::endl;
    }
}

void read() {
    while (1) {
        if (n > x) {

            // t2 = high_resolution_clock::now();
            // duration<double, std::milli> ms_double = t2 - t1;
            // std::cout << ms_double.count() << "ms\n";
            int64_t y = now1() - T[ n - 1 ].b;
            x = n;
            std::cout << " n:" << n << "       x:" << x << "   -- circle:" << y << std::endl;
        }
    }
}

int main() {

    cpu_set_t cpuset;

    int64_t w;
    for (int i = 0; i < 50; i++) {
        w = now1();
        r++;
        r = r + i;
        r = r -n;
        std::cout << "sy:" << now1() - w << std::endl;
    }

    // 设置进程的 CPU 亲和性
    /*
    if (sched_setaffinity(0, sizeof(cpuset), &cpuset) == -1) {
        printf("Set CPU affinity failed, error: %d\n", errno);
        return -1;
    }
    */

    // int n = 0;
    // std::thread t1;               // t1 is not a thread
    std::thread t2(write);  // pass by value
    std::thread t3(read);   // pass by reference
    CPU_ZERO(&cpuset);      // 初始化CPU集合，将 cpuset 置为空
    CPU_SET(1, &cpuset);    // 将本进程绑定到 CPU2 上
    int rc = pthread_setaffinity_np(t2.native_handle(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
        std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
    }
    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);
    rc = pthread_setaffinity_np(t3.native_handle(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0) {
        std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
    }
    // std::thread t4(std::move(t3));  // t4 is now running f2(). t3 is no longer a thread
    t2.join();
    t3.join();
    std::cout << "Final value of n is " << n << '\n';
}
