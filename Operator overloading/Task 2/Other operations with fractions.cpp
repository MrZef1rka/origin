#include <iostream>
#include "Fraction.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");

	int a, b, c, d;
	
	cout << "Введите числитель 1 дроби: ";
	cin >> a;
	cout << "Введите знаменатель 1 дроби: ";
	cin >> b;
	Fraction f1(a, b);

	cout << "Введите числитель 2 дроби: ";
	cin >> c;
	cout << "Введите знаменатель 2 дроби: ";
	cin >> d;
	Fraction f2(c, d);
	
	cout << f1 << " + " << f2 << " = " << (f1 + f2) << endl;
	cout << f1 << " - " << f2 << " = " << (f1 - f2) << endl;
	cout << f1 << " * " << f2 << " = " << (f1 * f2) << endl;
	cout << f1 << " / " << f2 << " = " << (f1 / f2) << endl;

	cout << "Операция с унарным минусом:" << endl;
	cout << "-" << f1 << " * " << f2 << " = " << (-(f1) * f2) << endl;
	
	cout << "Префиксный инкремент:" << endl;
	cout << "Начальное значение дроби 1: " << "++" << f1 << endl;
	cout << ++f1 << " * " << f2 << " = " << f1 * f2 << endl;

	cout << "Префиксный декремент:" << endl;
	cout << "Начальное значение дроби 1: " << "--" << f1 << endl;
	cout << --f1 << " * " << f2 << " = " << f1 * f2 << endl;

	cout << "Постфиксный инкремент:" << endl;
	Fraction temp_inc_post = f1++;
	cout << temp_inc_post << "++" << " * " << f2 << " = " << temp_inc_post * f2 << endl;
	cout << "Знчение дроби 1 после вычесления: " << f1 << endl;

	cout << "Постфиксный декремент:" << endl;
	Fraction temp_dec_post = f1--;
	cout << temp_dec_post << "--" << " * " << f2 << " = " << temp_dec_post * f2 << endl;
	cout << "Знчение дроби 1 после вычесления: " << f1;

	return 0;
}