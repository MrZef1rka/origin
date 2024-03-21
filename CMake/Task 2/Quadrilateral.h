#pragma once
#include "Figure.h"

class Quadrilateral : public Figure
{
public:
	Quadrilateral(int a, int b, int c, int d);
	void print_info(Figure* figure) override;
};
