#include <omp.h>
#include <time.h>  //记录程序运行时间

#include <chrono>
#include <iostream>
using namespace std;

int main() {
    int     Matrix1[ 3 ][ 4 ] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    int     Matrix2[ 4 ][ 2 ] = {2, 3, 5, 4, 7, 9, 8, 0};
    int     Matrix[ 3 ][ 2 ];
    clock_t start, end;  //声明类型为clock_t

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    cout << "Matrix1:\n";
    int i, j, k;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            cout << Matrix1[ i ][ j ] << '\t';
        }
        cout << endl;
    }

    cout << "Matrix2:\n";
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            cout << Matrix2[ i ][ j ] << '\t';
        }
        cout << endl;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            Matrix[ i ][ j ] = 0;
        }
    }

    omp_set_num_threads(3);
    int pnum = omp_get_num_procs();
    cout << "Thread_pnum =" << pnum << endl;

    start   = clock();  //开始计时
    auto t1 = high_resolution_clock::now();
#pragma omp parallel shared(Matrix1, Matrix2, Matrix) private(j, k)
    {
#pragma omp for schedule(dynamic)
        for (i = 0; i < 3; i++) {
            cout << "Thread_num:" << omp_get_thread_num() << '\n';
            for (j = 0; j < 2; j++) {
                for (k = 0; k < 4; k++) {
                    Matrix[ i ][ j ] = Matrix[ i ][ j ] + Matrix1[ i ][ k ] * Matrix2[ k ][ j ];
                }
            }
        }
    }
    end                                    = clock();
    auto                         t2        = high_resolution_clock::now();
    auto                         ms_int    = duration_cast<milliseconds>(t2 - t1);
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";

    cout << "Matrix multiply time:" << (double(end - start)) / CLOCKS_PER_SEC
         << endl;  // CLOCKS_PER_SEC为每秒敲钟数，end-start是从开始到程序结束的总敲钟数
    cout << "The result is:\n";

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 2; j++) {
            cout << Matrix[ i ][ j ] << '\t';
        }
        cout << endl;
    }

    return 0;
}
