#include <iostream>
#include <string>

template <std::string S>
class Message {
   public:
    Message() : length{S.len} {}

   private:
    std::size_t length{0};
};

extern const char hello[] = "external linkage";  // external linkage
const char hello11[] = "internal linkage!";      // internal linkage

int main() {
    Message<hello> msg;      // OK (all C++ versions)
    Message<hello11> msg11;  // OK since C++11

    static const char hello17[] = "no linkage";  // no linkage

    Message<hello17> msg17;  // OK since C++17

    return 0;
}
