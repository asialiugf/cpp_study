#include <iostream>
// #include<windows.h>
// #include< algorithm >
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <array>
#include <vector>
using namespace std;

int getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
int main() {
    int temp;
    std::array<int, 100000> arr1{};
    arr1[100000] = 30;
    // int start_time2 = getTime();
    for (int i = 0; i < 100000000; i++) {
        arr1[i % 10000] += 1;
        // if (arr2[i % 9]) temp++;
    }
    // int end_time2 = getTime();
    // std::cout << "int[] run time is " << (end_time2 - start_time2) << "ms." << endl;
    std::cout << "arr1[100] run time is " << arr1[100000] << " ms." << endl;

    vector<int> vect(1);
    vect[100000] = 32;
    std::cout << "adsfasd" << std::endl;
    // int start_time1 = getTime();
    for (int i = 0; i < 100000000; i++) {
        vect[i % 10000] += 1;
        // if(vect[i%9]) temp++;
    }
    // int end_time1 = getTime();
    // std::cout << "vector run time is " << (end_time1 - start_time1) << "ms." << endl;
    printf("-------------------------%d\n", vect[100000]);

    // exit(0);
    int *arr3 = new int[100]();
    arr3[0] = 1001;
    arr3[1] = 1001;
    arr3[2] = 1001;
    arr3[3] = 1001;
    arr3[4] = 1001;
    // std::cout << "arr3[100] run time is " << (int)arr3[0] << " ms." << std::endl;
    printf("%d\n", arr3[0]);
    exit(0);
    // start_time1 = getTime();
    for (int i = 0; i < 100000; i++) {
        arr3[i % 10000]++;
        // if(vect[i%9]) temp++;
    }
    // end_time1 = getTime();
    // std::cout << "new int[] run time is " << (end_time1 - start_time1) << "ms." << endl;

    return 0;
}
