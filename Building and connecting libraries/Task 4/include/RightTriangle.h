#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API RightTriangle : public Figure
{
public:
	RightTriangle(int a, int b, int c);
	void print_info(Figure* figure)override;
};
