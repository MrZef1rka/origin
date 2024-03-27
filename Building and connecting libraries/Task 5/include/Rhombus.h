#pragma once
#include "Figure.h"

class Rhombus : public Figure
{
public:
	Rhombus(int a);
	void print_info(Figure* figure) override;
};

