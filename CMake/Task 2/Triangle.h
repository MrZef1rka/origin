#pragma once
#include "Figure.h"

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c);
    void print_info(Figure* figure)override;
};

