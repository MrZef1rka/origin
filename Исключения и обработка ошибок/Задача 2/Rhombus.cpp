#include "Rhombus.h"
#include <iostream>
#include "CreationError.h"
using namespace std;

Rhombus::Rhombus(int a, int b, int c, int d, int A, int B, int C, int D) : Figure(a, b, c, d, A, B, C, D) {

		cout << "\n" << "Ромб: " << endl;

		if (A + B + C + D != 360)
		{
			throw CreationError("Сумма углов должна быть 360 градусов.");
		}
		else if (a != b || a != c || a != d || A != C || B != D)
		{
			throw CreationError("Стороны должны быть равны. Углы A:C, B:D должны быть попарно равны.");
		}

};

	void Rhombus::print_info(Figure* figure){

		cout << "Стороны: " << getQuadrilateralSides()[0] << ", " << getQuadrilateralSides()[1] << ", " << getQuadrilateralSides()[2] << ", " << getQuadrilateralSides()[3] << ";";
		cout << " Углы: " << getQuadrilateralAngles()[0] << ", " << getQuadrilateralAngles()[1] << ", " << getQuadrilateralAngles()[2] << ", " << getQuadrilateralAngles()[3] << ";";
};
