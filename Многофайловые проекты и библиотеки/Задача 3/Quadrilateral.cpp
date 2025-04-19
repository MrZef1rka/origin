#include "Quadrilateral.h"
#include <iostream>

Quadrilateral::Quadrilateral(int a, int b, int c, int d) : Figure(a, b, c, d, 50, 60, 70, 80) {};

	void Quadrilateral::print_info(Figure* figure) {

		std::cout << "Четырёхугольник: " << std::endl;
		std::cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << std::endl;
		std::cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << std::endl;
	};
