#include <future>
#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;


int findAsyncIndex(const vector<int>& arr, int start, int end)
{
    int minIndex = start;
    for (int i = start + 1; i < end; i++)
    {
        if (arr[i] < arr[minIndex])
        {
            minIndex = i;
        }
    }
    return minIndex;
}

void selectionSort(vector<int>& arr)
{
    int n = arr.size();
    for (int i = 0; i < n-1; i++)
    {
        promise<int> prom;
        future<int> future = prom.get_future();

        auto search = async(launch::async, [&arr, i, n, &prom]()
        {
            int minIndex = findAsyncIndex(arr, i, n);
            prom.set_value(minIndex);
        });

        int minIndex = future.get();

        if (minIndex != i)
        {
            swap(arr[i], arr[minIndex]);
        }
    }            
}
int main()
{
    vector<int> arr = {64, 25, 12, 22, 11, 15};

    for (int num : arr)
    {
        cout << num << " ";
    }
    cout << endl;
    
    selectionSort(arr);
    
    for (int num : arr)
    {
        cout << num << " ";
    }
    cout << endl;
    
    return 0;
}
