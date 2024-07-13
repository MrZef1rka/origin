#include <iostream>
#include <vector>
#include <utility>
using namespace std;

template <class T>
class Array {
private:
    T** array;
    int rows;
    int cols;
public:
    Array() : array(nullptr), rows(0), cols(0) {}

    Array(int a, int b) : rows(a), cols(b) {
    
        array = new T*[rows];

        for (int i = 0; i < rows; i++) 
        {
            array[i] = new T[cols];
        }
    
    };

    Array(const Array& other) = delete;

    ~Array() {

        for (int i = 0; i < rows; i++) {
        
            delete[] array[i];
        }
    
        delete[] array;
        
    };


    Array& operator=(const Array& other) = delete;

    const T* operator =() const {
    
    
    }

    const T* operator[](int index) const {
    
        return array[index];
    }

    T* operator[](int index) {

        return array[index];
    }

    pair<int, int> Size() const {

        return { rows, cols };
 
    }
};



int main()
{
    
    Array<int> arr(2, 3);

    arr[0][0] = 4;

    auto size = arr.Size();
    
    cout << arr[0][0] << endl;
    cout << "Size table arr: " << size.first << "x" << size.second << endl;

    return 0;
}

