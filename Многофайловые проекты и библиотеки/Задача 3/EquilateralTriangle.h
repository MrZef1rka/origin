#pragma once
#include "Figure.h"

class EquilateralTriangle : public Figure
{
public:
	EquilateralTriangle(int a);

	void print_info(Figure* figure)override;
};
