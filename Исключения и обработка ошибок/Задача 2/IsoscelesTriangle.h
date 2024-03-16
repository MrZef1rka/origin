#pragma once
#include "Figure.h"

class IsoscelesTriangle : public Figure
{
public:
	IsoscelesTriangle(int a, int b, int c, int A, int B, int C);
	void print_info(Figure* figure)override;
};
