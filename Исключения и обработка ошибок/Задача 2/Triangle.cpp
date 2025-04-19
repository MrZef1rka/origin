#include "Triangle.h"
#include "CreationError.h"
#include <iostream>
using namespace std;

Triangle::Triangle(int a, int b, int c, int A, int B, int C) : Figure(a, b, c, A, B, C) {

	cout << "Треугольник:" << endl;
		if (a <= 0 || b <= 0 || c <= 0)
		{
			throw CreationError("Стороны должны быть положительными числами.");
		}
		else if (A + B + C != 180) {

			throw CreationError("Сумма углов должна быть 180 градусов.");
		}
	};

    void Triangle::print_info(Figure * figure) {

		cout << "Стороны: " << getTriangleSides()[0] << ", " << getTriangleSides()[1] << ", " << getTriangleSides()[2] << ";" <<
			" Углы: " << getTriangleAngles()[0] << ", " << getTriangleAngles()[1] << ", " << getTriangleAngles()[2] << ";";
    };
