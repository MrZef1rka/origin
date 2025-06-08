#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

void vector_sum(const vector<int>& a, const vector<int>& b, vector<int>& result, 
                size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

double benchmark(size_t size, int num_threads) {
    vector<int> a(size), b(size), result(size);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 100);
    
    for (size_t i = 0; i < size; ++i) {
        a[i] = dist(gen);
        b[i] = dist(gen);
    }

    vector<thread> threads;
    size_t chunk_size = size / num_threads;
    size_t remaining = size % num_threads;
    size_t start = 0, end = 0;

    auto start_time = high_resolution_clock::now();
    
    for (int i = 0; i < num_threads; ++i) {
        end = start + chunk_size + (i < remaining ? 1 : 0);
        threads.emplace_back(vector_sum, cref(a), cref(b), ref(result), start, end);
        start = end;
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = high_resolution_clock::now();
    duration<double> duration = end_time - start_time;
    
    return duration.count();
}

int main() {
    cout << "Cores available: " << thread::hardware_concurrency() << "\n\n";

    vector<size_t> sizes = {1000, 10000, 100000, 1000000};
    vector<int> thread_counts = {1, 2, 4, 8, 16};
    
    cout << "\t\t";
    for (size_t size : sizes) {
        cout << size << "\t\t";
    }
    cout << "\n";
    
    for (int threads : thread_counts) {
        cout << threads << " Streams\t";
        for (size_t size : sizes) {
            double time = benchmark(size, threads);
            cout << fixed << setprecision(6) << time << "s\t";
        }
        cout << "\n";
    }

    return 0;
}
