// Overloading operators for Data class
#include <cstdlib>
#include <iostream>

using namespace std;

// A class to represent an integer array
class Data {
   private:
    int* ptr;
    int size;

   public:
    Data(int*, int);

    // Overloading [] operator to access elements in array style
    int& operator[](int);

    // Utility function to print contents
    void print() const;
};

// Implementation of [] operator.  This function must return a
// reference as array element can be put on left side
int& Data::operator[](int index) {
    if (index >= size) {
        cout << "Data index out of bound, exiting" << endl;
        exit(0);
    }
    return ptr[index];
}

// constructor for array class
Data::Data(int* p = nullptr, int s = 0) {
    size = s;
    ptr = nullptr;
    if (s != 0) {
        ptr = new int[s];
        for (int i = 0; i < s; i++) ptr[i] = p[i];
    }
}

void Data::print() const {
    for (int i = 0; i < size; i++) cout << ptr[i] << " ";
    cout << endl;
}

// Driver program to test above methods
int main() {
    int a[] = {1, 2, 4, 5};
    Data arr1(a, 4);
    arr1[2] = 6;
    arr1.print();
    arr1[8] = 6;
    return 0;
}
