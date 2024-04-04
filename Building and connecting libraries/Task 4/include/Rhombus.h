#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API Rhombus : public Figure
{
public:
	Rhombus(int a);
	void print_info(Figure* figure) override;
};

