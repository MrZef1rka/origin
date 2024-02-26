#include <iostream>
#include <string>
using namespace std;


class Calculator
{
public:
	double num1;
	double num2;

	Calculator() {
		num1 = 0;
		num2 = 0;
	};

	double add() {
		return num1 + num2;
	}
	double multiply() {
		return num1 * num2;
	}
	double subtract_1_2() {
		return num1 - num2;
	}
	double subtract_2_1() {
		return num2 - num1;
	}
	double divide_1_2() {
		return num1 / num2;
	}
	double divide_2_1() {
		return num2 / num1;
	}

	bool set_num1(double num1) {
		if (num1 != 0)
		{
			this->num1 = num1;
			return true;
		}
		else
			return false;
	};
	bool set_num2(double num2) {
		if (num2 != 0)
		{
			this->num2 = num2;
			return true;
		}
		else
			return false;
	};
};



int main()
{
	setlocale(LC_ALL, "Russian");

	float a, b;
	Calculator calc;

	while (true)
	{

		cout << "Введите num1: ";
		cin >> a;
		while (calc.set_num1(a) != true)
		{
			cout << "Неверный ввод!" << endl;
			cout << "Введите num1: ";
			cin >> a;
		}
		cout << "Введите num2: ";
		cin >> b;
		while (calc.set_num2(b) != true)
		{
			cout << "Неверный ввод!" << endl;
			cout << "Введите num2: ";
			cin >> b;
		}
		
		cout << "num1 + num2 = " << calc.add() << endl;
		cout << "num1 - num2 = " << calc.subtract_1_2() << endl;
		cout << "num1 - num2 = " << calc.subtract_2_1() << endl;
		cout << "num1 * num2 = " << calc.multiply() << endl;
		cout << "num1 / num2 = " << calc.divide_1_2() << endl;
		cout << "num2 / num1 = " << calc.divide_2_1() << endl;

	};

		return 0;
}

