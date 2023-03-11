#include <vector>

using namespace std;

template <typename T, typename U>
class Base {
public:
    void func(T& container) {
        m_it = container.begin();
    }
    void set_m(U n) {
        m = n;
    }

private:
    static U m;
    // typename T::iterator m_it;  //注意这里
    // auto m_it;  //注意这里
    static decltype(T().begin()) m_it;  //注意这里
    // decltype(T().begin()) m_it;  //注意这里
};

int main() {
    const vector<int>       v;
    Base<const vector<int>> obj;
    obj.func(v);
    return 0;
}
