// accumulate example
#include <iostream>     // std::cout
#include <functional>   // std::minus
#include <numeric>      // std::accumulate

int myfunction(int x, int y)
{
    return x+2*y;
}

struct kkk    // class is same as struct  using public:
{
public:
    static int fun1(int x, int y)
    {
        return x + y;
    }
    int myfunction(int x, int y)
    {
        return x+2*y;
    }
};

struct myclass {
    int operator()(int x, int y)
    {
        return x+3*y;
    }
} myobject;

int main()
{
    int init = 100;
    int numbers[] = {10,20,30};

    kkk aam;

    std::cout << "using default accumulate: ";
    std::cout << std::accumulate(numbers,numbers+3,init);
    std::cout << '\n';

    std::cout << "using functional's minus: ";
    std::cout << std::accumulate(numbers, numbers+3, init, std::minus<int>());
    std::cout << '\n';

    std::cout << "using custom function: ";
    std::cout << std::accumulate(numbers, numbers+3, init, myfunction) << std::endl;

    int tete = aam.myfunction(1,2);
    std::cout << tete << std:: endl;
    tete = kkk::fun1(1,2);    // 只有静态成员才能如此调用 static function can be call by class
    std::cout << tete << std:: endl;

    std::cout << std::accumulate(numbers, numbers+3, init, kkk::fun1) << std:: endl ;
    std::cout << std::accumulate(numbers, numbers+3, init, aam.fun1) << std:: endl ;
    // 这里的 accumulate（）第四个参数，必须是静态成员变量才行。
    std::cout << '\n';

    std::cout << "using custom object: ";
    std::cout << std::accumulate(numbers, numbers+3, init, myobject) << std::endl;
    std::cout << std::accumulate(numbers, numbers+3, init, myclass()) << std::endl;
    std::cout << '\n';

    return 0;
}
