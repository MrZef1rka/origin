#include "Triangle.h"
#include <iostream>

Triangle::Triangle(int a, int b, int c) : Figure(a, b, c, 50, 60, 70) {};

	void Triangle::print_info(Figure* figure) {

		std::cout << "�����������: " << std::endl;
		std::cout << "�������: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << std::endl;
		std::cout << "����: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << std::endl;
	};