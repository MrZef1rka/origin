#include <iostream>
#include <string>
using namespace std;

enum class months 
{
	exit = 0,
	Январь,
	Февраль,
	Март,
	Апрель,
	Май,
	Июнь,
	Июль,
	Август,
	Сентябрь,
	Октябрь,
	Ноябрь,
	Декабрь,
};

string monthToString(months month) {
	switch (month) {
	case months::Январь:return "Январь";
	case months::Февраль:return "Февраль";
	case months::Март:return "Март";
	case months::Апрель:return "Апрель";
	case months::Май:return "Май";
	case months::Июнь:return "Июнь";
	case months::Июль:return "Июль";
	case months::Август:return "Август";
	case months::Сентябрь:return "Сентябрь";
	case months::Октябрь:return "Октябрь";
	case months::Ноябрь:return "Ноябрь";
	case months::Декабрь:return "Декабрь";
	}
}

months monthRefund(int number) {
	if (number >= 1 && number <= 12) {
		return months(number);
	}
	else {
		return months::exit;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	int number = 0;

	while (true) {
		cout << "Введите номер месяца: ";
		cin >> number;

		if (number > 12 || number < 0) {
			cout << "Неправильный номер!" << endl;
			continue;
		}
		months month = monthRefund(number);
		if (month == months::exit) {
			cout << "До свидания!" << endl;
			break;
		}
		else {

			cout << "Название месяца: " << monthToString(month) << endl;
		}
	}

	return 0;
}
