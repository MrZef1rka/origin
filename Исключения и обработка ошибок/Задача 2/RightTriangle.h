#pragma once
#include "Figure.h"

class RightTriangle : public Figure
{
public:
	RightTriangle(int a, int b, int c, int A, int B, int C);
	void print_info(Figure* figure)override;
};
