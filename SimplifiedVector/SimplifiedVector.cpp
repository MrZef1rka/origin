#include <iostream>
using namespace std;

template <class T>
class MyVector {
public:
	MyVector() : arr(nullptr), sz(0), cap(0) {}
	MyVector(std::initializer_list<T> initList) : arr(nullptr), sz(0), cap(initList.size()) {

		arr = new T[cap];
	
		for (const T& value : initList) {
		
			arr[sz++] = value;
		}
	
	}

	~MyVector() {
		if (arr != nullptr) {
			delete[] arr;
		}
	}

	const T& at(int index) const {

		if (index >= sz) {

			throw std::out_of_range("Index out of range");
		}

		return arr[index];

	}
	T& at(int index) {

		if (index >= sz) {

			throw std::out_of_range("Index out of range");
		}

		return arr[index];

	}

	const T& operator[](size_t index) const {

		return arr[index];
	}
	T& operator[](size_t index) {
	
		return arr[index];
	}

	void push_back(const T& value) {

		if (sz == cap) {

			size_t newCapacity = cap == 0 ? 1 : cap * 2;
			T* newArray = new T[newCapacity];

			for (size_t i = 0; i < sz; i++)
				{
					newArray[i] = arr[i];
				}

			delete[] arr;

			arr = newArray;
			cap = newCapacity;
		}
		arr[sz++] = value;
	}

	size_t size() const {

		return sz;

	}

	size_t capacity() const {

		return cap;
	}

private:
	T* arr;
	size_t sz;
	size_t cap;
};




int main()
{

	MyVector<int> arr = { 1, 2, 3, 4, 5 };

	cout << "Size: " << arr.size() << endl;
	cout << "Capacity: " << arr.capacity() << endl;
	cout <<"Index[4]: " << arr.at(4) << endl;
	arr.push_back(6);
	arr.push_back(7);
	arr.push_back(8);

	cout << "\n" << "Аfter push_back()" << "\n" << endl;
	cout << "Size: " << arr.size() << endl;
	cout << "Capacity: " << arr.capacity() << endl;
	cout <<"Index[7]: " << arr.at(7) << endl;


	return 0;
}

