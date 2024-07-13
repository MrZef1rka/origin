#include <iostream>
#include <vector>
using namespace std;

template <typename T>
T Squaring(T a) {
    
   return a * a;

};

template<typename T>
vector<T> Squaring(const vector<T>& vec) {

    vector<T> res = vec;
    for (auto& elem : res)
    {
        elem = Squaring(elem);
    };
    return res;
};

int main()
{
    int a = 5; int x = Squaring(a); //Переменная a по прежнему равна 5  
    std::vector<int> b = { -1,4,8 };
    std::vector<int> y = Squaring(b);
    
    for (const auto& elem : y) 
    {
        cout << elem << endl;
    };

    cout << "-----------" << endl;

    for (const auto& elem : b)
    {
        cout << elem << endl;
    };

    return 0;
};