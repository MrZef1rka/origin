#include <iostream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
void move_vectors(vector<T>& vec1, vector<T>& vec2) {

    if (&vec1 == &vec2) {
    
        return;
    }
    vector<T> tmp = move(vec1);
    vec1 = move(vec2);
    vec2 = move(tmp);
}

template <typename T>
void print_vector(const vector<T>& vec) {
    cout << "{";
    for (size_t i = 0; i < vec.size(); ++i) {
    
        cout << vec[i];
        if (i < vec.size() - 1) {
            cout << ", ";
        }
    }
    cout << "}";
};

int main()
{
    vector <string> one = { "test_string1", "test_string2" };
    vector <string> two;

    cout << "one = ";
    print_vector(one);
    cout <<"\n" << "two = ";
    print_vector(two);

    move_vectors(one, two);

    cout <<"\n" << "After moving:" << endl;
    cout << "one = ";
    print_vector(one);
    cout << "\n" << "two = ";
    print_vector(two);
    cout << endl;
    return 0;
}
