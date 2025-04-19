#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API EquilateralTriangle : public Figure
{
public:
	EquilateralTriangle(int a);

	void print_info(Figure* figure)override;
};
