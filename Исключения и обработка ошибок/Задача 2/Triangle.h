#pragma once
#include "Figure.h"

class Triangle : public Figure {
public:
    Triangle(int a, int b, int c, int A, int B, int C);
    bool isValidTriangle(int a, int b, int c, int A, int B, int C);

    void print_info(Figure* figure)override;
};

