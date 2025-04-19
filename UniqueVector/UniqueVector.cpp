#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

void removeDuplicates(vector<int>& vec) {

    unordered_set<int> rem;
    auto it = remove_if(vec.begin(), vec.end(),
        [&rem](const int& value) {
            if (rem.find(value) != rem.end()) {
                return true;
            }
            else {
                rem.insert(value);
                return false;
            }
        });
    vec.erase(it, vec.end());
};

int main()
{
    vector<int> vec = { 1, 1, 2, 5, 6, 1, 2, 4 };

    cout << "[IN]: ";
    for (const auto& res : vec) {
        cout << res << " ";
    }
    cout << endl;

    removeDuplicates(vec);
    sort(vec.begin(), vec.end());

    cout << "[OUT]: ";
    for (const auto& res : vec) {
        cout << res << " ";
    }
    return 0;
}

