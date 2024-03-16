#pragma once
#include "Figure.h"

class EquilateralTriangle : public Figure
{
public:
	EquilateralTriangle(int a, int b, int c, int A, int B, int C);

	void print_info(Figure* figure)override;
};
