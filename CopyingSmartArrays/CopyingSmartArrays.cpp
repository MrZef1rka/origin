#include <iostream>
#include <stdexcept>
#include <algorithm>
using namespace std;

class smartArray {
private:
	int* arr;
	int size;
	int capacity;

public:
	smartArray(int size) : size(0), capacity(size) {
		arr = new int[capacity];
	}

	~smartArray() {
		delete[] arr;
	}

	smartArray(const smartArray& other) : size(other.size), capacity(other.capacity) {
		arr = new int[capacity];
		std::copy(other.arr, other.arr + size, arr);
	}

	// Оператор присваивания
	smartArray& operator=(const smartArray& other) {
		if (this == &other) {
			return *this;
		}

		delete[] arr;

		arr = new int[other.capacity];
		size = other.size;
		capacity = other.capacity;
		std::copy(other.arr, other.arr + size, arr);

		return *this;
	}

	// Получение элемента по индексу
	int get_element(int index) const {
		if (index < 0 || index >= size) {
			throw out_of_range("Выход за пределы массива.");
		}
		return arr[index];
	}

	// Добавление элемента
	void add_element(int value) {
		if (size >= capacity) {
			throw overflow_error("Массив заполнен, добавление невозможно.");
		}
		arr[size++] = value;
	}
};

int main() {
	setlocale(LC_ALL, "ru");

	try {
		smartArray arr(2);

		arr.add_element(10);
		arr.add_element(20);

		cout << "Элемент с индексом 0 в arr: " << arr.get_element(0) << endl;
		cout << "Элемент с индексом 1 в arr: " << arr.get_element(1) << endl;

		smartArray new_arr(2);

		new_arr.add_element(44);
		new_arr.add_element(34);

		arr = new_arr;

		cout << "Операция присваивания выполнилась!" << endl;

		cout << "Элемент с индексом 0 в arr: " << arr.get_element(0) << endl;
		cout << "Элемент с индексом 1 в arr: " << arr.get_element(1) << endl;

	}
	catch (const out_of_range& ex) {
		cout << "Ошибка! " << ex.what() << endl;
	}
	catch (const overflow_error& ex) {
		cout << "Ошибка! " << ex.what() << endl;
	}

	return 0;
}
