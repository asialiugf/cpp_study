#include <iostream>
#include <map>

template <typename Val>
using int_map_t = std::map<int, Val>;

using map = std::map<int, double>;

class Base {
protected:
    int bn1;
    int bn2;
    int get() {
        return bn1;
    }
};

using kk = Base;

class Derived : private kk {
public:
    using Base::bn1;
    using kk::get;
};

class test : public kk {
public:
    using kk::bn2;
};

class DerivedAgain : public Derived {};

int main() {
    int_map_t<double>     mm;
    std::map<int, double> tt;
    mm.insert(std::pair<int, double>(1, 3.14));
    tt.insert(std::pair<int, double>(1, 3.14));
    Derived      d;
    DerivedAgain da;
    d.bn1 = 1;
    // d.bn2  = 2;  // error, 'bn2' is a private member of 'Base'
    da.bn1 = 3;  // ok
    std::cout << d.bn1 << std::endl;

    da.bn1 = 100;
    std::cout << da.bn1 << " --- ok !!\n" << std::endl;
    std::cout << da.get() << " --- ok !!\n" << std::endl;

    return 0;
}
