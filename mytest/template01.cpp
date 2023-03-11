#include <array>
#include <iostream>

std::array<double, 1000> O{0};
std::array<double, 1000> H{0};
std::array<double, 1000> L{0};
std::array<double, 1000> C{0};
std::array<double, 1000> V{0};

template <int N>
class Future {
   public:
    const static int perriod = N;
    static int serial;

   public:
    Future(int a = 0) {
        std::cout << "Future init !" << std::endl;
        // serial = a;
    };
    int get_perriod() { return perriod; }
    void set_serial(int a) { serial = a; }
    int get_serial() { return serial; }
};

// template <int N>
class Future1 {
   public:
    const static int perriod = 10;
    int serial;

   public:
    Future1(int a = 0) {
        std::cout << "Future1 init !" << std::endl;
        serial = a;
    };
    int get_perriod() { return perriod; }
};

class Serial {
   public:
    int x;
    std::array<double, 1000> s{0};
};

// template <Future<10> F, Serial S, int M = 1, int N = 2, int D = 3> // 这里的 Future<10>没有搞定！
template <int M = 1, int N = 2, int D = 3, int X = 1>
class MACD {
   public:
    // template <int X>
    Future<X> f;
    const static int m = M;
    const static int n = N;
    const static int d = D;
    const static Serial s1;
    const static Serial s2;
    const static Serial s3;
    // template <int X>
    MACD(const Future<X>& f1) {
        std::cout << "here !" << std::endl;
        f = f1;
    }
    // const const static Serial& s1 =
    // const Serial& s = S;
};

void fnfn() {
    static Future<15> f{0};
    static Future<15> f1{0};
    static Future1 f3{0};
    static Future1 f4{0};
}

int main() {
    static Future<15> f{0};
    static Future<15> f1{0};
    static Future1 f3{0};
    static Future1 f4{0};
    static Serial s1;
    static MACD<26, 9, 9, 15> m1{f};  //
    static MACD<26, 9, 9, 15> m2(f);  // 这里最好用 {} 进行初始化！
    std::cout << m1.f.get_perriod() << std::endl;
    // Future<15>::serial = 1001;
    // std::cout << "------f serial: " << m1.f.get_serial() << std::endl;
    // std::cout << "------f1 serial: " << f1.get_serial() << std::endl;
    // std::cout << m1.f.get_perriod() << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < 5; i++) {
        fnfn();
    }
    exit(0);
}