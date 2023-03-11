#include <iostream>
class Base {
protected:
    int bn1;
    int bn2;
    int get() {
        std::cout << "world" << std::endl;
        return bn1;
    };

public:
    int kk;
    int rout() {
        std::cout << "hello" << std::endl;
        return 0;
    }
};

class Derived : public Base {
public:
    using Base::bn1;
    using Base::get;
    // using Base::kk;
    using Base::rout;
};

class DerivedAgain : public Derived {};

int main() {
    Derived      d;
    DerivedAgain da;
    d.bn1 = 1;
    //>> d.bn2 = 2;  // error, 'bn2' is a private member of 'Base'
    da.bn1 = 3;  // ok
    da.kk  = 8;
    std::cout << d.bn1 << std::endl;
    std::cout << da.kk << std::endl;
    da.rout();
    da.get();
    return 0;
}
