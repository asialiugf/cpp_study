#include <cstdlib>
#include <iostream>
#include <vector>
#include <numeric>
using namespace std;
typedef struct _NODE {
    int id;
    int val;
    int operator()(int nod, _NODE &node)
    {
        return nod * (node.val);
    }
    static int nadd(int x, int y)
    {
        return x + y ;
    }

} NODE;

int main(int argc, char *argv[])
{
    vector<NODE> vec;

    NODE node;

    vector<int> aa = {1,1,1,1,1};

    for(int i=1; i<5; i++) {
        NODE temp;
        temp.id = i;
        temp.val = i;

        vec.push_back(temp);
    }

    long long sum = accumulate(vec.begin(), vec.end(), 1, NODE());
    cout << "sum is: " << sum << endl;

    // 注意：下面是对 aa 而不是 vec 进行的操作
    sum = accumulate(aa.begin(), aa.end(), 1, NODE::nadd); // 这里必须是静态成员变量
    sum = accumulate(aa.begin(), aa.end(), 1, node.nadd); // 这里必须是静态成员变量

    cout << "sum is: " << sum << endl;

    return EXIT_SUCCESS;
}

/*
需要为自定义的结构类型重载()运算符， 而且第一个参数的类型应该和
调用accumulate()的第三个参数(也就是初始化值)类型一致,其他就没了...
*/
