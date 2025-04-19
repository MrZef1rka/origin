#include <iostream>
#include <forward_list>
#include <vector>
#include <algorithm>
using namespace std;


int main()
{

    int quantity;
    int num;
 
    cout << "[Quantity of elements]: ";
    cin >> quantity;

    vector<int> list;

    for (int count = 0; count < quantity; count++) {
    
        cout << "[IN]: ";
        cin >> num;
        list.push_back(num);
    }

    sort(list.begin(), list.end());

    auto del = unique(list.begin(), list.end());
    list.erase(del, list.end());

    reverse(list.begin(), list.end());

    cout << "[OUT]: ";

    for (const auto& elem : list) {
    
        cout << elem << " ";
    }

    return 0;
}

