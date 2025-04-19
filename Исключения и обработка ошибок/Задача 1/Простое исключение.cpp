#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;


class bad_length {};

int function(std::string str, int forbidden_length) {

	int length = str.length();

	if (length != forbidden_length) {
	
		return length;
	}
	else if (length == forbidden_length) {
	
		throw bad_length();
	}
};


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	string str;
	int forbiddenLength = 0;
	
	cout << "Введите запретную длину: ";
	cin >> forbiddenLength;
	cin.ignore();

	while (true)
	{
		cout << "Введите слово: ";
		getline(cin, str);

		try
		{
			int length = function(str, forbiddenLength);
			cout << "Длина слова \"" << str << "\" = " << length << endl;

		}
		catch (const bad_length&)
		{
			cout << "Ошибка: длина слова равна запретной длине!" << endl;
			break;
		}
	}
	return 0;
}