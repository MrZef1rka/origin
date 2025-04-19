#include "RightTriangle.h"
#include <iostream>
#include "CreationError.h"
using namespace std;


RightTriangle::RightTriangle(int a, int b, int c, int A, int B, int C) : Figure(a, b, c, A, B, C) {

	cout <<"\n" << "Прямоугольный треугольник:" << endl;

	if (a <= 0 || b <= 0 || c <= 0)
	{
		throw CreationError("Стороны должны быть положительными числами.");
	}
	else if (A + B + C != 180) 
	{
		throw CreationError("Сумма углов должна быть 180 градусов.");
	}
	else if (C != 90)
	{
		throw CreationError("Угол С должен быть 90 градусов.");
	}

};

	void RightTriangle::print_info(Figure* figure){

		cout << "Стороны: " << getTriangleSides()[0] << ", " << getTriangleSides()[1] << ", " << getTriangleSides()[2] << ";" <<
			" Углы: " << getTriangleAngles()[0] << ", " << getTriangleAngles()[1] << ", " << getTriangleAngles()[2] << ";";
};