#include <iostream>
#include "Source.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");

	double a = 0;
	double b = 0;
	int num = 0;

	cout << "Введите первое число: ";
	cin >> a;
	cout << "Введите второе число: ";
	cin >> b;

	cout << "Выберите операцию:" << endl;
	cout << "1 - сложение" << "\n" << "2 - вычитание" << "\n" << "3 - умножение" << "\n" << "4 - деление" << "\n" << "5 - возведение в степень" << endl;
	cout << "Номер: ";
	cin >> num;

	switch (num)
	{
	case 1: 
		cout << a << " + " << b << " = " << addition(a, b);
		break;
	case 2:
		cout << a << " - " << b << " = " << subtraction(a, b);
		break;
	case 3:
		cout << a << " * " << b << " = " << multiplication(a, b);
		break;
	case 4:
		if (b != 0)
		{
			cout << a << " / " << b << " = " << division(a, b);
		break;
		}
	default:
		cout << "Делить на 0 нельзя." << endl;
		break;
	case 5:
		cout << a << " в степени " << b << " = " << degree(a, b);
		break;
	}

	return 0;
}
