#include "EquilateralTriangle.h"
#include <iostream>

EquilateralTriangle::EquilateralTriangle(int a) : Figure(a, a, a, 60, 60, 60) {};

void EquilateralTriangle::print_info(Figure* figure) {

		std::cout << "�������������� �����������: " << std::endl;
		std::cout << "�������: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << std::endl;
		std::cout << "����: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << std::endl;

};
