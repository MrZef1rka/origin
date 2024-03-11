#pragma once
#include "Figure.h"

class Square : public Figure
{
public:
	Square(int a);
	void print_info(Figure* figure) override;
};

