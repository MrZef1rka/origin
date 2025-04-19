#include <iostream>
using namespace std;

template<class T>
class unique_ptr {
public:
    explicit unique_ptr(T* ptr = nullptr) : ptr(ptr) {};
    unique_ptr(const unique_ptr&) = delete;
    ~unique_ptr() {
    
        cout << "Destructor";
        delete ptr;
    }

    unique_ptr& operator=(const unique_ptr&) = delete;

    T& operator*() const {
        return *ptr;
    }

    T* release() {
        T* old_ptr = ptr;
        ptr = nullptr;
        return old_ptr;
    }
private:
    T* ptr;

};

int main()
{
    ::unique_ptr<int> uptr(new int(42));
    std::cout << "Value: " << *uptr << std::endl;

    int* raw_ptr = uptr.release();
    std::cout << "Value after release: " << *raw_ptr << std::endl;

    delete raw_ptr;

    return 0;
}

