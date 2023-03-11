#include <iostream>

template <int N>
class Test {
   public:
    const static int perriod = N;

   public:
    int serial;

   public:
    Test(int a = 0) { serial = a; }
    int get_perriod() { return perriod; }
};

template <int N>
class Mytest {
   public:
    static Test<N> t;
    int x;
    int get_serial() { return t.serial; }
};

int main() {
    static Test<10> t{1001};
    Mytest<10> x1;
    std::cout << "" << x1.t.get_perriod() << std::endl;
}