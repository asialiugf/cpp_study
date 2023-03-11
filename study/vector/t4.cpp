#include <vector>
#include <ctime>
#include <iostream>
#include <numeric>

using namespace std;

int main()
{
    int i = 0;
    int j = 0;
    int kk = 0;
    int suum = 0;
    vector<int> arrar(1000000, 2);

    // ----------------------------------------------------
    clock_t sT1 = clock();
    for(j = 0; j<10000; j++) {
        suum = 0;
        suum=accumulate(arrar.begin(),arrar.end(),0);
    }
    clock_t eT1 = clock();
    cout << eT1-sT1 << endl;

    cout << suum << endl ;
    // ----------------------------------------------------
    clock_t sT2 = clock();
    for(j = 0; j<10000; j++) {
        kk = 0;
        for(i = 0; i<1000000 ; i++) {
            kk = kk + arrar[i];
        }
    }
    clock_t eT2 = clock();
    cout << eT2-sT2 << endl;

    cout << kk << endl ;

    return 0;
}
