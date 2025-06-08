#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
bool client_first_step_done = false;
int queueCount = 0;
const int MAX_QUEUE = 10;

void client_thread() {
    {
        lock_guard<mutex> lock(mtx);
        queueCount++;
        cout << "First client added. Queue: " << queueCount << endl;
        client_first_step_done = true;
    }
    cv.notify_one();

    while (true) {
        this_thread::sleep_for(chrono::seconds(1));
        lock_guard<mutex> lock(mtx);
        if (queueCount < MAX_QUEUE) {
            queueCount++;
            cout << "Client added. Queue: " << queueCount << endl;
        } else {
            cout << "Maximum queue reached. Client thread stops." << endl;
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
        lock_guard<mutex> lock(mtx);
        if (queueCount > 0) {
            queueCount--;
            cout << "Operator served. Queue: " << queueCount << endl;
        } else {
            cout << "No more clients. Operator thread stops." << endl;
            break;
        }
    }
}

int main() {
    thread t1(client_thread);
    thread t2(operator_thread);

    t1.join();
    t2.join();

    cout << "Program finished." << queueCount << endl;
    return 0;
}