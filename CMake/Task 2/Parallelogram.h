#pragma once
#include "Figure.h"

class Parallelogram : public Figure
{
public:
	Parallelogram(int a, int b);

	void print_info(Figure* figure) override;
};
