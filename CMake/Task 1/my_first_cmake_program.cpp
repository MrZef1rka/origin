#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	string name;

	cout << "Ведите имя: ";
	cin >> name;
	cout << "Здравствуйте, " << name << "!" << endl;
	system("pause");
	return 0;
}

