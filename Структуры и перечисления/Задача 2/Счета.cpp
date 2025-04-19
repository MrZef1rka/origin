#include <iostream>
#include <string>
using namespace std;


struct bankAccount {
	int check;
	std::string FistName;
	float money;
};

void changeBankAccount(bankAccount& account, float newMoney) {
	account.money = newMoney;
};

int main()
{
	setlocale(LC_ALL, "Russian");

	bankAccount account;

	float newMoney = 0;

	cout << "Введите номер счёта: ";
	cin >> account.check;
	cout << "Введите имя владельца: ";
	cin >> account.FistName;
	cout << "Введите баланс: ";
	cin >> account.money;
	cout << "Введите новый баланс: ";
	cin >> newMoney;

	changeBankAccount(account, newMoney);

	cout << "Ваш счёт: " << account.FistName << ", " << account.check << ", " << account.money << endl;
}


