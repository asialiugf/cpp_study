#include <iostream>
#include <string>
using namespace std;

template <int N>
class Pow3 {
public:
    static const int result = 3 * Pow3<N - 1>::result;
};

//用于结束递归的全局特化
template <>
class Pow3<0> {
public:
    static const int result = 1;
};

int main(int argc, char *argv[]) {
    int n;
    std::cin >> n;
    cout << "Pow3<2>::result= " << Pow3<2>::result << endl;
    cout << "Pow3<3>::result= " << Pow3<4>::result << endl;
    cout << "Pow3<3>::result= " << Pow3<n>::result << endl;
    return 0;
}
