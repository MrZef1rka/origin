#include <iostream>
#include "CreationError.h"
#include <stdexcept>
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

	try
	{

	Triangle triangle(10, 20, 30, 50, 60, 70);
	triangle.print_info(&triangle);

	RightTriangle rightTriangle(10, 20, 30, 50, 40, 90);
	rightTriangle.print_info(&rightTriangle);

	IsoscelesTriangle isoscelesTriangle(10, 20, 10, 60, 60, 60);
	isoscelesTriangle.print_info(&isoscelesTriangle);

	EquilateralTriangle equilateralTriangle(20, 20, 20, 60, 60, 60);
	equilateralTriangle.print_info(&equilateralTriangle);

	Quadrilateral quadrilateral(10, 20, 30, 40, 100, 60, 180, 20);
	quadrilateral.print_info(&quadrilateral);

	Rectangle rectangle(10, 20, 10, 20, 90, 90, 90, 90);
	rectangle.print_info(&rectangle);

	Square square(20, 20, 20, 20, 90, 90, 90, 90);
	square.print_info(&square);

	Parallelogram parallelogram(20, 40, 20, 40, 100, 80, 100, 80);
	parallelogram.print_info(&parallelogram);

	Rhombus rhombus(20, 20, 20, 20, 100, 80, 100, 80);
	rhombus.print_info(&rhombus);

	cout << endl;

	}
	catch (const CreationError& error)
	{
		std::cerr << "Ошибка создания фигуры. Причина: " << error.what() << std::endl;
	}

	return 0;
};


