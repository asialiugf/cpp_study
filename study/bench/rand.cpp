
#include <bits/c++config.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <thread>

using namespace std;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

#define random(x) (rand() % x)

double aa[ 10000000 ];
double bb[ 10000000 ];

int main() {
    auto t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 1000000; i++) {
        aa[ i ] = random(100);
    }
    auto                         t2 = high_resolution_clock::now();
    auto                         ms_int = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";

    size_t j = 0;
    t1 = high_resolution_clock::now();
    for (size_t i = 0; i < 1000000; i++) {
        if (aa[ i ] == 5) {
            j++;
            bb[ j ] = aa[ i ];
        }
    }
    t2 = high_resolution_clock::now();
    ms_int = duration_cast<milliseconds>(t2 - t1);
    ms_double = t2 - t1;
    std::cout << ms_int.count() << "ms select \n";
    std::cout << ms_double.count() << "ms   select \n";

    for (size_t i = 0; i < 10; i++) {
        std::cout << bb[ i ] << std::endl;
    }

    return 0;
}
