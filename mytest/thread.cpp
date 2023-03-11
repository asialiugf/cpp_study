#include <unistd.h>

#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

#include "timestamp.h"

int k[1000]{5};

uint64_t before;
uint64_t latency;

void worker() {
    std::cout << "another thread" << std::endl;
    int i = 0;
    while (1) {
        if (k[i] != 0) {
            latency = rdtsc() - before;
            i = i + 1;

            std::cout << "thread1:  " << k[i - 1] << "   --: " << latency << std::endl;
            if (i == 1000) {
                std::cout << "thread1:  " << k[i - 1] << "   --: " << latency << std::endl;
                i = 0;
            }
            before = rdtsc();
        }
    }
}

int main() {
    for (int x = 0; x < 1000; x++) {
        k[x] = 0;
    }
    std::thread t(worker);
    std::cout << "main thread" << std::endl;
    std::cout << k[100] << std::endl;
    int x = -1;
    while (1) {
        x = x + 1;
        if (x == 1000) {
            x = 0;
        }
        usleep(1000000);
        k[x] = x + 1134;
        before = rdtsc();
    }
    t.join();
    return 0;
}
