#include <mutex>
#include <utility>

class Data {
public:
    std::mutex mtx;
    int a = 0;
    int b = 0;
    
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    
    Data() = default;
    Data(int a, int b) : a(a), b(b) {}
};

void swap_with_lock(Data& lhs, Data& rhs) {
    if (&lhs == &rhs) return;
    
    lock(lhs.mtx, rhs.mtx);
    
    std::swap(lhs.a, rhs.a);
    std::swap(lhs.b, rhs.b);
    
    lhs.mtx.unlock();
    rhs.mtx.unlock();
}

void swap_with_scoped_lock(Data& lhs, Data& rhs) {
    if (&lhs == &rhs) return;
    
    std::scoped_lock lock(lhs.mtx, rhs.mtx);
    
    std::swap(lhs.a, rhs.a);
    std::swap(lhs.b, rhs.b);
}

void swap_with_unique_lock(Data& lhs, Data& rhs) {
    if (&lhs == &rhs) return;
    
    std::unique_lock<std::mutex> lock_lhs(lhs.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock_rhs(rhs.mtx, std::defer_lock);
    
    lock(lock_lhs, lock_rhs);
    
    std::swap(lhs.a, rhs.a);
    std::swap(lhs.b, rhs.b);
    
}


int main() {
    Data d1(1, 2);
    Data d2(3, 4);
    
    swap_with_lock(d1, d2);
    swap_with_scoped_lock(d1, d2);
    swap_with_unique_lock(d1, d2);
    
    return 0;
}