#include "Rectangle.h"
#include <iostream>
#include "CreationError.h"
using namespace std;

Rectangle::Rectangle(int a, int b, int c, int d, int A, int B, int C, int D) : Figure(a, b, c, d, A, B, C, D) {

	cout << "\n" << "Прямоугольник:" << endl;

	if (A + B + C + D != 360)
	{
		throw CreationError("Сумма углов должна быть 360 градусов.");
	}
	else if (a != c || b != d || A != 90 || B != 90 || C != 90 || D != 90)
	{
		throw CreationError("Стороны a:c и b:d должны быть попарно равны. Все углы равны 90.");
	}

};

void Rectangle::print_info(Figure* figure) {

	cout << "Стороны: " << getQuadrilateralSides()[0] << ", " << getQuadrilateralSides()[1] << ", " << getQuadrilateralSides()[2] << ", " << getQuadrilateralSides()[3] << ";";
	cout << " Углы: " << getQuadrilateralAngles()[0] << ", " << getQuadrilateralAngles()[1] << ", " << getQuadrilateralAngles()[2] << ", " << getQuadrilateralAngles()[3] << ";";
};