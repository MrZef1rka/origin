#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API IsoscelesTriangle : public Figure
{
public:
	IsoscelesTriangle(int a, int b);
	void print_info(Figure* figure)override;
};
