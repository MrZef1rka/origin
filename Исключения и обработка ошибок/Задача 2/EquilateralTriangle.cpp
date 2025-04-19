#include "EquilateralTriangle.h"
#include <iostream>
#include "CreationError.h"
using namespace std;

EquilateralTriangle::EquilateralTriangle(int a, int b, int c, int A, int B, int C) : Figure(a, b, c, A, B, C) {

	cout << "\n" << "Равносторонний треугольник:" << endl;

	if (a <= 0 || b <= 0 || c <= 0)
	{
		throw CreationError("Стороны должны быть положительными числами.");
	}
	else if (A + B + C != 180)
	{
		throw CreationError("Сумма углов должна быть 180 градусов.");
	}
	else if (a != b || a != c || b != c || A != 60 || B != 60 || C != 60)
	{
		throw CreationError("Все стороны должны быть равны. Углы должны быть равны 60.");
	}

};

void EquilateralTriangle::print_info(Figure* figure) {

	cout << "Стороны: " << getTriangleSides()[0] << ", " << getTriangleSides()[1] << ", " << getTriangleSides()[2] << ";" <<
		" Углы: " << getTriangleAngles()[0] << ", " << getTriangleAngles()[1] << ", " << getTriangleAngles()[2] << ";";
};


