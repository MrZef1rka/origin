#include <iostream>
using namespace std;

class Figure
{
private:
	int sidesTriangle[3];
	int anglesTriangle[3];
	int sidesQuadrilateral[4];
	int anglesQuadrilateral[4];

public:

	Figure(int a, int b, int c, int A, int B, int C)
	{
		sidesTriangle[0] = a;
		sidesTriangle[1] = b;
		sidesTriangle[2] = c;
		anglesTriangle[0] = A;
		anglesTriangle[1] = B;
		anglesTriangle[2] = C;
	};

	Figure(int a, int b, int c, int d, int A, int B, int C, int D)
	{
		sidesQuadrilateral[0] = a;
		sidesQuadrilateral[1] = b;
		sidesQuadrilateral[2] = c;
		sidesQuadrilateral[3] = d;
		anglesQuadrilateral[0] = A;
		anglesQuadrilateral[1] = B;
		anglesQuadrilateral[2] = C;
		anglesQuadrilateral[3] = D;
	};

	virtual void print_info(Figure* figure){
		cout << "Информация о фигуре." << endl;

	};
	
	const int* getTriangleSides() const 
	{ return sidesTriangle; }
	const int* getTriangleAngles() const 
	{ return anglesTriangle; }
	const int* getQuadrilateralSides() const 
	{ return sidesQuadrilateral; }
	const int* getQuadrilateralAngles() const 
	{ return anglesQuadrilateral; }

};

class Triangle : public Figure
{
public:

	Triangle(int a, int b, int c) : Figure(a, b, c, 50, 60, 70) {}; // Сумма углов == 180 градусам

	void print_info(Figure* figure)override {

		cout << "Треугольник: " << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class RightTriangle : public Figure
{
public:
	RightTriangle(int a, int b, int c) : Figure(a, b, c, 50, 40, 90) {};

	void print_info(Figure* figure)override {

		cout << "Прямоугольный треугольник: " << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class IsoscelesTriangle : public Figure
{
public:
	IsoscelesTriangle(int a, int b) : Figure(a, b, a, 50, 80, 50) {};

	void print_info(Figure* figure)override {

		cout << "Равнобедренный треугольник: " << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class EquilateralTriangle : public Figure
{
public:
	EquilateralTriangle(int a) : Figure(a, a, a, 60, 60, 60) {};

	void print_info(Figure* figure)override {

		cout << "Равносторонний треугольник: " << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class Quadrilateral : public Figure
{
public:
	Quadrilateral(int a, int b, int c, int d) : Figure(a, b, c, d, 50, 60, 70, 80) {};

	void print_info(Figure* figure) override {

		cout << "Четырёхугольник: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] 
			<< "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] 
			<< "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Rectangle : public Figure
{
public:
	Rectangle(int a, int b) : Figure(a, b, a, b, 90, 90, 90, 90){};

	void print_info(Figure* figure) override {

		cout << "Прямоугольник: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1]
			<< "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1]
			<< "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Square : public Figure
{
public:
	Square(int a) : Figure(a, a, a, a, 90, 90, 90, 90) {};
	void print_info(Figure* figure) override {

		cout << "Квадрат: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1]
			<< "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;

		cout << "Углы: " << "A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1]
			<< "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Parallelogram : public Figure
{
public:
	Parallelogram(int a, int b) : Figure(a, b, a, b, 30, 40, 30, 40) {};

	void print_info(Figure* figure) override {
		cout << "Параллелограмм: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1]
			<< "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;

		cout << "Углы: " << "A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1]
			<< "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Rhombus : public Figure
{
public:
	Rhombus(int a) : Figure(a, a, a, a, 30, 40, 30, 40) {};

	void print_info(Figure* figure) override {
		cout << "Ромб: " << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1]
			<< "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;

		cout << "Углы: " << "A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1]
			<< "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << endl;
	};
};


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

	Rectangle rectangle(10, 20);
	rectangle.print_info(&rectangle);

	Square square(20);
	square.print_info(&square);

	Parallelogram parallelogram(10, 20);
	parallelogram.print_info(&parallelogram);

	Rhombus rhombus(20);
	rhombus.print_info(&rhombus);

	return 0;
};

