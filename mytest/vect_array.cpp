#include <iostream>
// #include<windows.h>
// #include< algorithm >
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <array>
#include <vector>

int main() {
    std::array<int, 1000000> arr1{};
    arr1[100000] = 30;
    std::cout << "value: " << arr1[100] << std::endl;
    std::cout << "value: " << arr1[100000] << std::endl;

    std::vector<int> vect;
    vect[100] = 32;
    //    vect[10000] = 55;
    std::cout << "vect value: " << arr1[100] << std::endl;
    //   std::cout << "vect value: " << arr1[10000] << std::endl;

    /*
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
    */

    return 0;
}
