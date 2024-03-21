#include <iostream>
#include "Figure.h"
#include "Triangle.h"
#include "RightTriangle.h"
#include "IsoscelesTriangle.h"
#include "EquilateralTriangle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Parallelogram.h"
#include "Rhombus.h"
#include "Quadrilateral.h"
using namespace std;


int main()
{
	setlocale(LC_ALL, "ru");

	Triangle triangle(10, 20, 30);
	triangle.print_info(&triangle);

	RightTriangle rightTriangle(10, 20, 30);
	rightTriangle.print_info(&rightTriangle);

	IsoscelesTriangle isoscelesTriangle(10, 20);
	isoscelesTriangle.print_info(&isoscelesTriangle);

	EquilateralTriangle equilateralTriangle(20);
	equilateralTriangle.print_info(&equilateralTriangle);

	Quadrilateral quadrilateral(10, 20, 30, 40);
	quadrilateral.print_info(&quadrilateral);

	Rectangle rectangle(10, 20);
	rectangle.print_info(&rectangle);

	Square square(20);
	square.print_info(&square);

	Parallelogram parallelogram(10, 20);
	parallelogram.print_info(&parallelogram);

	Rhombus rhombus(20);
	rhombus.print_info(&rhombus);

	system("pause");

	return 0;
};

