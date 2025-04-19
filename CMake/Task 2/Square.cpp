#include "Square.h"
#include <iostream>

Square::Square(int a) : Figure(a, a, a, a, 90, 90, 90, 90) {};
	void Square::print_info(Figure* figure) {

		std::cout << "Квадрат: " << std::endl;
		std::cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << std::endl;
		std::cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << std::endl;
	};
