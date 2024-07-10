#include <iostream>
#include <vector>
using namespace std;

template <typename T>
T Squaring(T a) {
    
   return a * a;

};

template<typename T>
void Squaring(vector<T>& vec) {

    for (auto& elem : vec)
    {
        elem = Squaring(elem);
    };
};

int main()
{
    std::cout << Squaring(10) << endl;
    std::cout << Squaring(5) << endl;
    std::cout << Squaring(8) << endl;
    std::cout << Squaring(15) << endl;


    cout << "-----------" << endl;
    
    vector<int> vec = { 10, 5, 8, 15 };
    Squaring(vec);

    for (const auto& elem : vec) 
    {
        cout << elem << endl;
    };

    return 0;
}