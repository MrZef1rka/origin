#include "IsoscelesTriangle.h"
#include <iostream>

IsoscelesTriangle::IsoscelesTriangle(int a, int b) : Figure(a, b, a, 50, 80, 50) {};

	void IsoscelesTriangle::print_info(Figure* figure) {

		std::cout << "Равнобедренный треугольник: " << std::endl;
		std::cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << std::endl;
		std::cout << "Углы: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << std::endl;
	};
