#include <fmt/chrono.h>
#include <stdlib.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

inline uint64_t rdtsc() { return __builtin_ia32_rdtsc(); }

#define PRINTF_2D_WITH_TWO_DIGIT(buff, num) \
    {                                       \
        int32_t tmp2 = (num) / 10;          \
        int32_t tmp = (num)-tmp2 * 10;      \
        *buff++ = (char)('0' + tmp2);       \
        *buff++ = (char)('0' + tmp);        \
    }

int MakeTime(char *caT, int T) {
    int h, m, s;
    h = T / 3600;
    m = (T - h * 3600) / 60;
    s = T % 60;
    char *hh = caT;
    char *mm = caT + 3;
    char *ss = caT + 6;
    // see_memzero(caT, 9);
    // snprintf(caT, 9, "%02d:%02d:%02d", h, m, s);
    PRINTF_2D_WITH_TWO_DIGIT(hh, h);
    PRINTF_2D_WITH_TWO_DIGIT(mm, m);
    PRINTF_2D_WITH_TWO_DIGIT(ss, s);
    caT[2] = ':';
    caT[5] = ':';
    caT[8] = 0;
    return 0;
}

int main() {
    // uint64_t rdtsc();
    uint64_t before;
    uint64_t latency;
    char hms[9];
    before = rdtsc();
    MakeTime(hms, 7129);
    latency = rdtsc() - before;
    fmt::print(" {} , MakeTime latency: {} \n", hms, latency);

    char ca_errmsg[2048];
#define ERR_MSG_LEN 2048
    before = rdtsc();
    snprintf(ca_errmsg, ERR_MSG_LEN, "A:%s %s %06d S:%d T:%s H:%g L:%g LP:%g AP:%g AV:%d BP:%g ", "20220202", "12:23:12", 500 * 1000, 0,
             "20220202", 80.1, 90.1, 100.2, 11.11, 100, 22.22);
    latency = rdtsc() - before;
    fmt::print(" {} \n  latency: {} \n", ca_errmsg, latency);

    memset(ca_errmsg, 0, 2048);
    before = rdtsc();
    fmt::format_to(ca_errmsg, "A:{} {} {} S:{} T:{} H:{} L:{} LP:{} AP:{:.2f} AV:{} BP:{}", "20220202", "12:23:12", 500 * 1000, 0,
                   "20220202", 80.1, 90.1, 100.2, 11.1101010101, 100, 22.22);
    latency = rdtsc() - before;
    fmt::print(" {} \n  latency: {} \n", ca_errmsg, latency);

    using namespace std::literals::chrono_literals;
    fmt::print("Default format: {} {}\n", 42s, 100ms);
    fmt::print("strftime-like format: {:%H:%M:%S}\n", 3h + 15min + 30s);

    double pi = 3.14159265358979;
    printf("%f\n", pi);                    // 输出3.141593，后面的被四舍五入了
    std::cout << fmt::format("{}\n", pi);  // format，输出完整的3.14159265358979

    // 与snprintf函数对应的是format_to_n
    char buff[50];
    sprintf(buff, "%f", pi);
    std::cout << buff << "\n";  // 与printf一样，输出3.141593
    memset(buff, 0, 50);
    fmt::format_to_n(buff, 2, "{}", pi);
    std::cout << "pi: " << buff << "\n";  // 3.14159265358979

    char buffer[12];
    fmt::format_to_n(buffer, 5, "aaaa{}", "bbbb");

    fmt::print("name is {:.2f}\n", 1.2356);

    double x;
    x = sqrt(100);
    memset(buff, 0, 50);
    fmt::format_to(buff, "name is {:.2f}\n", 1.2356);
    std::cout << "buff:" << buff << "\n";  // 与printf一样，输出3.141593
    memset(buff, 0, 50);
    fmt::format_to(buff, "name is {:.2f}\n", x);
    std::cout << "buff:" << buff << "\n";  // 与printf一样，输出3.141593

    auto out = fmt::memory_buffer();
    fmt::format_to(std::back_inserter(out), "For a moment, {} happened.", "nothing");
    auto data = out.data();  // pointer to the formatted data
    auto size = out.size();  // size of the formatted data

    std::cout << buffer;
}
