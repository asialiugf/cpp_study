#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <thread>

#define xxx std::cout

void thread_task()
{
    std::cout << "hello world" << std::endl;
    xxx << "kkk" << std::endl;
}

int main(int argc, const char* argv[])
{
    std::thread t(thread_task);
    t.join();
    return EXIT_SUCCESS;
}

int test()
{
}
