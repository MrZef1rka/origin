#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API Parallelogram : public Figure
{
public:
	Parallelogram(int a, int b);

	void print_info(Figure* figure) override;
};
