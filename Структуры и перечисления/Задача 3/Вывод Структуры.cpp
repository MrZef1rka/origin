#include <iostream>
#include <string>
using namespace std;


struct adressStorage {
	std::string city;
	std::string streret;
	int house;
	int apartment;
	int index;
};

void outputStructure(adressStorage& info) {
	cout << "Город: " << info.city << endl;
	cout << "Улица: " << info.streret << endl;
	cout << "Дом: " << info.house << endl;
	cout << "Квартира: " << info.apartment << endl;
	cout << "Индекс: " << info.index << endl;
	cout << endl;
};

int main()
{
	setlocale(LC_ALL, "Russian");

	adressStorage oneCopy = { "Москва", "Арбат", 3, 125, 125489};
	adressStorage twoCopy = { "Барнаул", "Пушкина", 1, 6, 759853 };
	outputStructure(oneCopy);
	outputStructure(twoCopy);
}