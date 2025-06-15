#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <atomic>

using namespace std;

mutex mtx;
condition_variable cv;
bool client_first_step_done = false;
atomic<int> queueCount(0);
const int MAX_QUEUE = 10;

void client_thread() {
    {
        lock_guard<mutex> lock(mtx);
        queueCount.fetch_add(1, memory_order_relaxed);
        cout << "First client added. Queue: " << queueCount << "\n";
        client_first_step_done = true;
    }
    cv.notify_one();

    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        int current = queueCount.load(memory_order_relaxed);
        if (current < MAX_QUEUE) {
            queueCount.fetch_add(1, memory_order_relaxed);
            lock_guard<mutex> lock(mtx);
            cout << "Client added. Queue: " << queueCount << "\n";
        } else {
            lock_guard<mutex> lock(mtx);
            cout << "Maximum queue reached. Client thread stops." << "\n";
            break;
        }
    }
}

void operator_thread() {
    {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return client_first_step_done; });
    }

    while (true) {
        this_thread::sleep_for(chrono::seconds(2));
        int current = queueCount.load(memory_order_relaxed);
        if (current > 0) {
            queueCount.fetch_sub(1, memory_order_relaxed);
            lock_guard<mutex> lock(mtx);
            cout << "Operator served. Queue: " << queueCount << "\n";
        } else {
            lock_guard<mutex> lock(mtx);
            cout << "No more clients. Operator thread stops." << "\n";
            break;
        }
    }
}

int main() {
    thread t1(client_thread);
    thread t2(operator_thread);

    t1.join();
    t2.join();

    cout << "Program finished." << "\n";
    return 0;
}
