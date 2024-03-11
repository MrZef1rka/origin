#pragma once
#include "Figure.h"

class Rectangle : public Figure
{
public:
	Rectangle(int a, int b);
	void print_info(Figure* figure) override;
};
