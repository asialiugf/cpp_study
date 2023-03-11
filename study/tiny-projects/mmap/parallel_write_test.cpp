#include <atomic>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "mmapper.h"

#define FILE_SIZE_LIM 1 << 10

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_to_write>\n", argv[ 0 ]);
        exit(0);
    }

    mem::Writer         writer(FILE_SIZE_LIM, std::string(argv[ 1 ]));
    std::atomic<size_t> total(0);

    int THREAD_NUM = std::thread::hardware_concurrency();

    std::cout << "thread num:" << THREAD_NUM << std::endl;

    // 读取要写入的单词
    std::ifstream            infile("words.txt");
    std::vector<std::string> words;
    std::string              line;
    while (getline(infile, line)) {
        words.push_back(std::move(line));
    }

    auto                     start = std::chrono::steady_clock::now();
    std::vector<std::thread> threads;
    for (int i = 0; i < THREAD_NUM; i++) {
        threads.emplace_back([ & ]() {
            std::string word_with_space = words[ i % words.size() ] + " ";
            const char* s               = word_with_space.c_str();
            size_t      len             = word_with_space.size();
            for (int i = 0; i < 1000; i++) {
                writer.write_data(s, len);
            }
            size_t byte_num = len * 1000;
            total += byte_num;
            printf("Thread %08x put %lu bytes to file %s\n", std::this_thread::get_id(), byte_num,
                   argv[ 1 ]);
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    auto                          end  = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    printf("Write complete, %d threads write %lu bytes in %.3f ms\n", THREAD_NUM, total.load(),
           diff * 1000);
    return 0;
}
