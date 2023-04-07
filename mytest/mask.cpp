#include <string.h>
#include <unistd.h>

#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "timestamp.h"
int main() {
// #define MASK 0b11111111111111111111111100000000;
#define MASK 0b00000000000000000000001111111111;
    char32_t x;
    char32_t z = MASK;
    for (x = 0; x < 200000; x++) {
        char32_t y = x & z;
        std::cout << x << " " << y << std::endl;
        ;
    }

    x = MASK;
    std::cout << x << std::endl;
}
