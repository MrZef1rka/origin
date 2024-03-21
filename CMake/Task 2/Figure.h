#pragma once

class Figure
{
private:
	int sidesTriangle[3];
	int anglesTriangle[3];
	int sidesQuadrilateral[4];
	int anglesQuadrilateral[4];

public:

	Figure(int a, int b, int c, int A, int B, int C);

	Figure(int a, int b, int c, int d, int A, int B, int C, int D);

	virtual void print_info(Figure* figure);

	int* getTriangleSides();

	int* getTriangleAngles();

	int* getQuadrilateralSides();

	int* getQuadrilateralAngles();

};
