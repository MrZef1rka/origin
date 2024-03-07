#include <iostream>
using namespace std;

class Figure
{
private:

	int parties;

public:
	Figure(int parties) {
		this->parties = parties;
	};

	int getParties() {
		return parties;
	}

	void printParties() {

		cout << getParties() << endl;
	};
};


class Triangle : public Figure 
{
public:
	
	Triangle() : Figure(3) {}
};

class Quadrangle : public Figure 
{
public:
	Quadrangle() : Figure(4) {}
	
};


int main()
{
	setlocale(LC_ALL, "ru");

	Figure figure(0);
	Triangle triangle;
	Quadrangle quadrangle;

	cout << "Колличество сторон:" << endl;
	cout << "Фигрура: "; 
	figure.printParties();
	cout << "Треугольник: ";
	triangle.printParties();
	cout << "Четырёхугольник: ";
	quadrangle.printParties();

	return 0;
}

