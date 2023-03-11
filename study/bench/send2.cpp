#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>

int n = 0;
int x = 0;

struct tick {
    int    i;
    double b;
    char   cc[ 100 ];
};

tick T[ 1000000 ];

void write(int u) {
    std::cout << "kkk" << std::endl;
}

void read() {
}

int main() {
    int n = 0;
    // std::thread t1;               // t1 is not a thread
    std::thread t2(write);  // pass by value
    std::thread t3(read);   // pass by reference
    // std::thread t4(std::move(t3));  // t4 is now running f2(). t3 is no longer a thread
    t2.join();
    t3.join();
    std::cout << "Final value of n is " << n << '\n';
}
