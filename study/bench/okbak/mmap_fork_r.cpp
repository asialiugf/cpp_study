#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <iostream>

#include "shm_common.h"
#include "timestamp.h"

#define random(x) (rand() % x)

#define BUF_SIZE 10000
struct tick {
    int      haha;
    int      num;
    uint64_t before;
    double   h;
    double   o;
    double   c;
    double   l;
    char     a[ 100 ];
};

int main(int argc, char **argv) {

    /*
    if (fork() == 0) {
        sleep(1);

        tick *tt = shmmap<tick>("/WFMPMC.shm");
        if (tt == nullptr)
            return 1;
        tt->haha = 0;

        tt->num = 0;
        while (1) {
            if (tt->num == 10) {
                exit(0);
            }
            sleep(2);
            tt->h = (double)random(1000);
            tt->o = (double)random(1000);
            tt->c = (double)random(1000);
            tt->l = (double)random(1000);

            std::cout << "sun: num:" << tt->num << "    " << tt->h << " " << tt->o << " " << tt->c
                      << " " << tt->l << std::endl;

            std::cout << tt->haha << "-------------" << std::endl;

            tt->num    = tt->num + 1;
            tt->before = rdtsc();
        }
        exit(0);
    }
    */

    if (fork() == 0) {
        sleep(2);
        int      t = -1;
        uint64_t latency;

        volatile tick *hh = shmmap<tick>("/WFMPMC.shm");
        if (hh == nullptr)
            return 1;
        volatile int *ty = &hh->num;

        hh->haha = 1000;

        while (true) {
            // std::cout << "f:" << hh->num << " " << t << std::endl;
            //  sleep(1);
            if (*ty > t) {
                latency = rdtsc() - hh->before;
                // printf("parent got a message: %d ", hh->num);
                std::cout << "latency: " << latency << " cycles" << std::endl;
                std::cout << "father-------------: num:" << hh->num << " t:" << t << " " << hh->h
                          << " " << hh->l << std::endl;
                t        = hh->num;
                hh->haha = hh->haha + 10;
            }

            if (hh->num == 10) {
                exit(0);
            }
            // std::cout << "f:" << hh->num << " " << t << std::endl;
        }

        return 0;
    }

    sleep(15);
    return 0;
}
