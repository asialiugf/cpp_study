#include <vector>
#include <numeric>
#include <functional>
#include <iostream>

using namespace std;

int main()
{

    vector <int> v1, v2(20);
    vector <int>::iterator Iter1, Iter2;

    int i;
    for(i = 1 ; i < 21 ; i++) {
        v1.push_back(i);
    }

    cout << "最初向量v1中个元素的值为:\n ( " ;
    for(Iter1 = v1.begin() ; Iter1 != v1.end() ; Iter1++)
        cout << *Iter1 << " ";
    cout << ")." << endl;

    // accumulate函数的第一个功能，求和
    int total;
    total = accumulate(v1.begin() , v1.end() , 0);

    cout << "整数从1到20的和为: "
         << total << "." << endl;

    // 构造一个前n项和的向量
    int j = 0, partotal;
    for(Iter1 = v1.begin() + 1; Iter1 != v1.end() + 1 ; Iter1++) {
        partotal = accumulate(v1.begin() , Iter1 , 0);
        v2 [ j ] = partotal;
        j++;
    }

    cout << "前n项和分别为:\n ( " ;
    for(Iter2 = v2.begin() ; Iter2 != v2.end() ; Iter2++)
        cout << *Iter2 << " ";
    cout << ")." << endl << endl;

    // accumulate函数的第二个功能，计算连乘积
    vector <int> v3, v4(10);
    vector <int>::iterator Iter3, Iter4;

    int s;
    for(s = 1 ; s < 11 ; s++) {
        v3.push_back(s);
    }

    cout << "向量v3的初始值分别为:\n ( " ;
    for(Iter3 = v3.begin() ; Iter3 != v3.end() ; Iter3++)
        cout << *Iter3 << " ";
    cout << ")." << endl;

    int ptotal;
    ptotal = accumulate(v3.begin() , v3.end() , 1 , multiplies<int>());

    cout << "整数1到10的连乘积为: "
         << ptotal << "." << endl;

    // 构造一个前n项积的向量
    int k = 0, ppartotal;
    for(Iter3 = v3.begin() + 1; Iter3 != v3.end() + 1 ; Iter3++) {
        ppartotal = accumulate(v3.begin() , Iter3 , 1 , multiplies<int>());
        v4 [ k ] = ppartotal;
        k++;
    }

    cout << "前n项积分别为:\n ( " ;
    for(Iter4 = v4.begin() ; Iter4 != v4.end() ; Iter4++)
        cout << *Iter4 << " ";
    cout << ")." << endl;
}
