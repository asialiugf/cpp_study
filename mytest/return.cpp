#include <iostream>
struct TT {
    int x = 2;
    int y = 0;
    std::string str = "kkkk";
};

struct TT ft() {
    struct TT x;
    return x;
}
struct TT& ftt() {
    static struct TT x;
    // struct TT& y = x;
    return x;
}

int main() {
    struct TT& tx = ftt();
    tx.x = 6;
    std::cout << tx.x << std::endl;
    struct TT txt = ftt();
    std::cout << txt.x << std::endl;
    exit(0);
}

