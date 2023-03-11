#include <initializer_list>
#include <iostream>
#include <vector>
class MagicFoo {
public:
    std::vector<int> vec;
    MagicFoo(std::initializer_list<int> list) {
        for (std::initializer_list<int>::iterator it = list.begin(); it != list.end(); ++it)
            vec.push_back(*it);
    }
};
int main() {
    using std::cout;
    using cc = MagicFoo;
    cout << "test";
    // after C++11
    MagicFoo magicFoo = {1, 2, 3, 333, 333, 33333, 4, 5};
    std::cout << "magicFoo: " << std::endl;
    for (std::vector<int>::iterator it = magicFoo.vec.begin(); it != magicFoo.vec.end(); ++it)
        std::cout << *it << std::endl;
}
