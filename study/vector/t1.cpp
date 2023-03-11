#include<iostream>
#include<vector>
#include<functional>
#include <numeric>


typedef std::vector<int> EE;

typedef int(*mmx)();

int kkmm()
{
    std::cout<< "tetesttttt " << std::endl;
    return 15;
}

int main()
{

    std::vector<std::function<int()>> func;
    std::vector<mmx> myfuncs;
    std::vector<int(*)()> myfuncs1;
    myfuncs.push_back(kkmm) ;
    myfuncs1.push_back(kkmm) ;
    myfuncs[0]() ;
    myfuncs1[0]() ;

    std::vector<int> a(10, 100) ;      //大小为10初值为100的向量a
    EE b(20,1000);
    std::cout << " EE typedef : " << *(b.end()-1) << b[2] << std::endl;

    //对其中部分元素进行输入
    int j = 0;
    for(j = 0; j<10 ; j++) {
        a[j] = j;
    }
    std::cout << a[0] << std::endl;
    std::cout << a.at(5) << std::endl;

    std::vector <int> :: iterator itb = a.begin();
    std::vector <int> :: iterator ite = a.end()-1;  //最后一个值需要-1
    auto itm = a.begin()+a.size()/2;

    std::cout << "itb : " << *itb << std::endl;  // 要用* iterator来取值
    std::cout << "ite : " << *ite << std::endl;
    std::cout << "itm : " << *itm << std::endl;

    a.push_back(555);
    std::cout << "last: " << *(a.end()-1) << std::endl;
    std::cout << "a.size(): " << a.size() << std::endl;


    int summ = accumulate(itb,ite,0);
    std::cout <<"x:"<< summ << std::endl;

    //全部输出
    //int i ;
    unsigned i;
    for(i=0; i<a.size(); i++)
        std::cout<<a[i]<<" " << std::endl;

    return 0 ;
}
