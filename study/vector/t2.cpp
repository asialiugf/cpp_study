#include <vector>
#include <ctime>
#include <iostream>
#include <numeric>

using namespace std;

int main()
{
    int i = 0;
    vector<int> arrar(1000000, 2);

    clock_t sT1 = clock();
    for(i = 0; i<10; i++) {
        for(size_t i=0; i<arrar.size(); i++) {
            arrar[i]++;
        }
    }
    //int suum=accumulate(arrar.begin(),arrar.end(),0);
    clock_t eT1 = clock();
    cout << eT1-sT1 << endl;

    cout << "kk:  " << arrar[100] << endl;

    //cout << suum << endl ;

    clock_t sT2 = clock();
    for(i = 0; i<10 ; i++) {
        for(vector<int>::iterator p=arrar.begin(); p!=arrar.end(); p++) {
            (*p)++;
        }
    }
    //int kk=0;
    //for ( int i = 0; i<1000000 ; i++ ) {
    //  kk = kk + arrar[i];
    //}
    clock_t eT2 = clock();
    cout << eT2-sT2 << endl;

    //cout << kk << endl ;
    cout << "kk:  " << arrar[100] << endl;

    return 0;
}
