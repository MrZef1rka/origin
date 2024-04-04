#pragma once
#include "LibraryExport.h"
#include "Figure.h"

class MY_LIBRARY_API Triangle : public Figure {
public:
    Triangle(int a, int b, int c);
    void print_info(Figure* figure)override;
};

