#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>

using namespace std;
using namespace chrono;

mutex cout_mutex;
const int num_threads = 5;
const int progress_length = 30;

// Генератор случайных чисел для разной скорости потоков
random_device rd;
mt19937 gen(rd());
uniform_int_distribution<> speed_dist(50, 300);

void draw_progress(int thread_num, int start_line) {
    auto start = high_resolution_clock::now();
    int delay = speed_dist(gen);
    
    for (int i = 0; i <= progress_length; i++) {
        this_thread::sleep_for(milliseconds(delay));
        
        lock_guard<mutex> lock(cout_mutex);
        cout << "\033[" << start_line + 1 << ";0H";
        cout << "Thread " << thread_num << " [";
        
        // Разные стили прогресс-баров для каждого потока
        char symbol;
        switch(thread_num) {
        case 1: symbol = '='; break;
        case 2: symbol = '*'; break;
        case 3: symbol = '>'; break; 
        case 4: symbol = '-'; break;
        case 5: symbol = '+'; break;
        default: symbol = '|'; break;
        }
        
        cout << string(i, symbol) << string(progress_length - i, ' ') << "] " 
             << (i * 100 / progress_length) << "%";
        cout.flush();
    }
    
    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    
    lock_guard<mutex> lock(cout_mutex);
    cout << "\033[" << start_line + 1 << ";0H";
    cout << "Thread " << thread_num << " [";
    cout << string(progress_length, '=') << "] Done in " << duration.count() << "ms";
    cout << string(10, ' ');
}

int main() {
    system("cls");
    // Создаем пустые строки для прогресс-баров
    for (int i = 0; i < num_threads; i++) {
        cout << "Thread " << i+1 << " [";
        cout << string(progress_length, ' ') << "]\n";
    }
    
    vector<thread> threads;
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(draw_progress, i+1, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    cout << "\033[" << num_threads + 1 << ";0H";
    cout << "All threads completed!\n";
    
    return 0;
}