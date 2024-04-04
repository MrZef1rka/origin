#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API Square : public Figure
{
public:
	Square(int a);
	void print_info(Figure* figure) override;
};

