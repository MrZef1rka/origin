#pragma once
#include "Figure.h"

class IsoscelesTriangle : public Figure
{
public:
	IsoscelesTriangle(int a, int b);
	void print_info(Figure* figure)override;
};
