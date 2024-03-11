#include <iostream>
#include <string>
#include "Class.h"
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");

	int num = 0;
	string ans;
	Counter myCounter;

	while (true) {
		cout << "Вы хотите указать начальное значение счётчика? Введите yes или no: ";
		std::cin >> ans;


		if (ans == "yes") {
			while (true) {
				cout << "Введите начальное значение счётчика: ";
				std::cin >> num;

				if (std::cin.fail()) {
					std::cin.clear();
					std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Ошибка! Введите целое число." << endl;
					continue;
				}
				else {
					myCounter.setNum(num);
					cout << "Начальное значение счётчика = " << myCounter.meaning() << endl;
					break;
				}
			}
			break;
		}
		else if (ans == "no") {
			myCounter.setOne();
			cout << "Начальное значение счётчика = " << myCounter.meaning() << endl;
			break;
		}
		else {
			cout << "Некорректная команда. Попробуйте снова." << endl;
		}
	}


	char sym = ' ';

	while (sym != 'x') {
		cout << "Введите команду ('+', '-', '=' или 'x'): ";
		std::cin >> sym;

		if (sym == '+') {
			myCounter.plus();
		}
		else if (sym == '-') {
			myCounter.minus();
		}
		else if (sym == '=') {
			cout << myCounter.meaning() << endl;
		}
		else if (sym == 'x') {
			cout << "До свидания!";
		}
		else {
			cout << "Некорректная команда. Попробуйте снова." << endl;
		}
	}

	return 0;
}
