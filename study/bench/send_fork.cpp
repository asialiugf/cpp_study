#include <time.h>
#include <unistd.h>

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

int        n = 0;
static int x = -1;

struct tick {
    int    i;
    double b;
    char   cc[ 100 ];
};

tick T[ 1000000 ];

void write() {
    for (int i = 0; i < 10000; i++) {
        T[ n ].i = 2 * n;
        T[ n ].b = 2 * n;

        for (int z = 0; z < 100; z++) {
            T[ n ].cc[ z ] = 'a';
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "write n:" << n << std::endl;
        t1 = high_resolution_clock::now();
        n++;
        // std::cout << "n:" << n << std::endl;
    }
}

void read() {
    while (1) {
        std::cout << "read  n:" << n << std::endl;
        if (n > x) {
            t2 = high_resolution_clock::now();
            duration<double, std::milli> ms_double = t2 - t1;
            std::cout << ms_double.count() << "ms\n";
            x = n;
            std::cout << " n:" << n << "       x:" << x << std::endl;
            std::cout << T[ n - 1 ].i << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    // fork函数使用
    pid_t pid = fork();
    if (pid < 0) {
        printf("error\n");
        return 1;
    } else if (pid == 0) {
        write();
    } else {
        read();
    }
    return 0;
}
