//使用组合bind函数，找出集合中大于5小于10的元素个数
#include <iostream>
#include <functional>
#include <algorithm>
#include <set>

using namespace std;

auto f = std::bind(std::logical_and<bool>(),std::bind(std::greater<int>(),1,5),std::bind(std::less_equal<int>(),  1, 10));

int main()
{
    set<int> se= {1,2,3,4,5,6,7,8,9};
    int count = std::count_if(se.begin(), se.end(), f);
    cout << count <<endl;

    return 0;
}
