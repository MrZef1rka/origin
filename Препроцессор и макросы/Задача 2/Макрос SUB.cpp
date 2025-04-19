#include <iostream>
using namespace std;
#define SUB(a, b) ((a) - (b))

int main()
{
	setlocale(LC_ALL, "ru");

	int a = 6;
	int b = 5;
	int c = 2;
	cout << SUB(a, b) << std::endl;
	cout << SUB(a, b) * c << std::endl;
	cout << SUB(a, b + c) * c << std::endl;

	return 0;
}

