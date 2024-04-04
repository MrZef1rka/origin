#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API Quadrilateral : public Figure
{
public:
	Quadrilateral(int a, int b, int c, int d);
	void print_info(Figure* figure) override;
};
