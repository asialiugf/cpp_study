/** 实现1 **/
#include <ctime>
#include <iostream>

#include "timestamp.h"

using namespace std;

int main() {

    auto   before  = rdtsc();
    time_t now     = time(nullptr);
    auto   latency = rdtsc() - before;
    std::cout << "latency: " << latency << " cycles \n" << std::endl;

    cout << "Now is: " << ctime(&now);
}
