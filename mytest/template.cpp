#include <iostream>

template <int N>
class Test {
   public:
    const static int perriod = N;

   protected:
    int serial;

   public:
    Test(int a = 0) { serial = a; }
    int get_perriod() { return perriod; }
};

template <int N>
class Mtest : public Test<N> {
   public:
    int x;

   public:
    Mtest(int a) : x(100) {
        x = a;
        Test<N>::serial = a * 2;
    };
    int foo() {
        std::cout << Test<N>::perriod << std::endl;
        return Test<N>::perriod + 1;
    };
};

int main() {
    Test<10> t10;
    Test<20> t20;
    std::cout << t10.get_perriod() << std::endl;
    std::cout << t20.get_perriod() << std::endl;

    for (int i = 0; i < 10; i++) {
        static Mtest<98> m1{i};
        Mtest<108> m2{i * 10};
        std::cout << m1.perriod << std::endl;
        std::cout << m2.foo() << std::endl;
        std::cout << m1.x << std::endl;
        std::cout << m2.x << std::endl;
    }
    exit(0);
}