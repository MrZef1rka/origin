#include "Figure.h"
#include <iostream>

Figure::Figure(int a, int b, int c, int A, int B, int C)
	{
		sidesTriangle[0] = a;
		sidesTriangle[1] = b;
		sidesTriangle[2] = c;
		anglesTriangle[0] = A;
		anglesTriangle[1] = B;
		anglesTriangle[2] = C;
	};

Figure::Figure(int a, int b, int c, int d, int A, int B, int C, int D)
	{
		sidesQuadrilateral[0] = a;
		sidesQuadrilateral[1] = b;
		sidesQuadrilateral[2] = c;
		sidesQuadrilateral[3] = d;
		anglesQuadrilateral[0] = A;
		anglesQuadrilateral[1] = B;
		anglesQuadrilateral[2] = C;
		anglesQuadrilateral[3] = D;
	};

	void Figure::print_info(Figure* figure) {
		std::cout << "Информация о фигуре." << std::endl;

	};

	int* Figure::getTriangleSides() {
		return sidesTriangle;
	}
	int* Figure::getTriangleAngles() {
		return anglesTriangle;
	}
	int* Figure::getQuadrilateralSides()
	{
		return sidesQuadrilateral;
	}
	int* Figure::getQuadrilateralAngles()
	{
		return anglesQuadrilateral;
	}