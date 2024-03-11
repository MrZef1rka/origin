#pragma once
#include "Figure.h"

class RightTriangle : public Figure
{
public:
	RightTriangle(int a, int b, int c);
	void print_info(Figure* figure)override;
};
