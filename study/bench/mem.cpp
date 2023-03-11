#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
//#include <cstdint>
//  #include <thread>

int main() {

    struct m {
        char c[ 408 ];
    };

    struct m mem[ 3600 * 24 * 2 ];

    mem[ 1 ].c[ 1 ] = 'a';

    while (1) {
        usleep(1000000);
    }
}
