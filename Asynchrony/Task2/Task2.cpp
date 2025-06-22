#include <iostream>
#include <vector>
#include <algorithm>
#include <future>
#include <iterator>

using namespace std;

template <typename Iterator, typename Func>
void parallel_for_each(Iterator begin, Iterator end, Func func)
{
    const size_t size = distance(begin, end);
    if (size <= 1)
    {
        if (size == 1) func(*begin);
        return;
    }

    Iterator mid = begin + (size / 2);
    auto left = async(launch::async, [=, &func]
    {
        parallel_for_each(begin, mid, func);
    });
    parallel_for_each(mid, end, func);
    left.get();
}

int main()
{
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    parallel_for_each(vec.begin(), vec.end(), [](int& i)
    {
        i *= 2;
    });

    for (int x : vec) cout << x << " ";
    
    return 0;
}
