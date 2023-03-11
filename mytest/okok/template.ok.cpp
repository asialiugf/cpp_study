#include <iostream>

template <int N>
class Test {
   public:
    const static int perriod = N;  // 这里要加 const

   protected:
    int serial;

   public:
    Test(int a = 0) { serial = a; }
    int get_perriod() { return perriod; }
};

template <int N>
class Mtest : public Test<N> {  // 这里主要测试带模板参数的继承。 Mtest 希望能够 继承 Test类的 常量。
   public:
    int x;

   public:
    Mtest(int a) : x(100) {
        x = a;
        Test<N>::serial = a * 2;  // 模板参数类的继承，两段式查找，所以这里需要加上 Test<N> !!
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