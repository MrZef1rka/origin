#include "../include/Parallelogram.h"
#include <iostream>
using namespace std;

Parallelogram::Parallelogram(int a, int b) : Figure(a, b, a, b, 30, 40, 30, 40) {};

	void Parallelogram::print_info(Figure* figure) {
		cout << "Параллелограмм: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
