/** 实现2 **/
#include <chrono>
#include <ctime>
#include <iostream>

#include "timestamp.h"

using namespace std;

int main() {

    auto before  = rdtsc();
    auto now     = chrono::system_clock::now();
    auto latency = rdtsc() - before;
    std::cout << "latency: " << latency << " cycles \n" << std::endl;

    before      = rdtsc();
    time_t time = chrono::system_clock::to_time_t(now);
    latency     = rdtsc() - before;
    std::cout << "latency: " << latency << " cycles \n" << std::endl;

    cout << "Now is: " << ctime(&time) << endl;
    return 0;
}
