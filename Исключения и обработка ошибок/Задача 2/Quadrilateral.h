#pragma once
#include "Figure.h"

class Quadrilateral : public Figure
{
public:
	Quadrilateral(int a, int b, int c, int d, int A, int B, int C, int D);
	void print_info(Figure* figure) override;
};
