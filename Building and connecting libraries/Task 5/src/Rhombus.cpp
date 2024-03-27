#include "../include/Rhombus.h"
#include <iostream>
using namespace std;

Rhombus::Rhombus(int a) : Figure(a, a, a, a, 30, 40, 30, 40) {};

	void Rhombus::print_info(Figure* figure){

		cout << "Ромб: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << endl;
};
