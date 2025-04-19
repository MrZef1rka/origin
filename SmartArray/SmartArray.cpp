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
