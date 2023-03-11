#include <array>
#include <iostream>

#define F1 60
#define S1 1

class Future {
   public:
    int period_;
    int index_;
    std::string future_id_;
    Future(const std::string& s, int p) {
        std::cout << "stock future init !" << std::endl;
        future_id_ = s;
        period_ = p;
        index_ = 0;
    }
    
};

class MACD {
   public:
    Future f;
    std::array<double, 1000> s;
    MACD() {}
};
/*
static Future& FutureInit(const std::string& s, int p) {
    static Future x(s, p);
    return x;
}
*/

int main() {
    static Future x1("aa", 1);
    static Future x2("bb", 1);
    static Future x3("cc", 1);

    x2.index_ = 100;
    std::cout << x1.index_ << std::endl;
    std::cout << x2.future_id_ << std::endl;

    exit(0);
}