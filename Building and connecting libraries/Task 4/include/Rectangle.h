#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API Rectangle : public Figure
{
public:
	Rectangle(int a, int b);
	void print_info(Figure* figure) override;
};
