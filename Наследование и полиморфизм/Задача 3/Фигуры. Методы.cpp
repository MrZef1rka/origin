#include <iostream>
using namespace std;

class Figure
{
private:

	int sides;
	int sidesTriangle[3];
	int anglesTriangle[3];
	int sidesQuadrilateral[4];
	int anglesQuadrilateral[4];

public:

	Figure() {

		sides = 0;
	};

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

	virtual void print_info(Figure* figure) {
		cout << "Фигура:" << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << "\n" << endl;
	};

	virtual void chekFigure(Figure* figure) {

		if (sides != 0) {

			cout << "Неправильная" << endl;

		}
		else {
			cout << "Правильная" << endl;

		};
	};


	virtual int getSides(Figure* figure)
	{
		return sides;
	};


	int* getTriangleSides()
	{
		return sidesTriangle;
	}
	int* getTriangleAngles()
	{
		return anglesTriangle;
	};
	int* getQuadrilateralSides()
	{
		return sidesQuadrilateral;
	};
	int* getQuadrilateralAngles()
	{
		return anglesQuadrilateral;
	};

};

class Triangle : public Figure
{
public:

	Triangle(int a, int b, int c, int A, int B, int C) : Figure(a, b, c, A, B, C) {};

	void chekFigure(Figure* figure) override
	{
		if (getTriangleAngles()[0] + getTriangleAngles()[1] + getTriangleAngles()[2] != 180)
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	int getSides(Figure* figure) override {

		int sides = 3;
		return sides;
	};


	void print_info(Figure* figure)override {

		cout << "Треугольник: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class RightTriangle : public Triangle
{
public:
	RightTriangle(int a, int b, int c, int A, int B, int C) : Triangle(a, b, c, A, B, C) {};

	void chekFigure(Figure* figure) override
	{
		if (getTriangleAngles()[0] + getTriangleAngles()[1] + getTriangleAngles()[2] != 180 || getTriangleAngles()[2] != 90)
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	void print_info(Figure* figure)override {
		cout << "Прямоугольный треугольник: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class IsoscelesTriangle : public Triangle
{
public:
	IsoscelesTriangle(int a, int b, int c, int A, int B, int C) : Triangle(a, b, c, A, B, C) {};

	void chekFigure(Figure* figure) override
	{
		if (getTriangleAngles()[0] + getTriangleAngles()[1] + getTriangleAngles()[2] != 180) {

			cout << "Неправильная" << endl;
		}
		else if (getTriangleSides()[0] != getTriangleSides()[2] || getTriangleAngles()[0] != getTriangleAngles()[2])
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};


	void print_info(Figure* figure)override {
		cout << "Равнобедренный треугольник: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class EquilateralTriangle : public Triangle
{
public:
	EquilateralTriangle(int a, int b, int c, int A, int B, int C) : Triangle(a, b, c, A, B, C) {};

	void chekFigure(Figure* figure) override
	{
		if (getTriangleAngles()[0] != 60 || getTriangleAngles()[1] != 60 || getTriangleAngles()[2] != 60) {

			cout << "Неправильная" << endl;
		}
		else if (getTriangleSides()[0] != getTriangleSides()[1] || getTriangleSides()[0] != getTriangleSides()[2] || getTriangleSides()[1] != getTriangleSides()[2])
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	void print_info(Figure* figure)override {
		cout << "Равносторонний треугольник: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getTriangleSides()[0] << "\tb = " << getTriangleSides()[1] << "\tc = " << getTriangleSides()[2] << endl;
		cout << "Углы: " << "\t A = " << getTriangleAngles()[0] << "\tB = " << getTriangleAngles()[1] << "\tC = " << getTriangleAngles()[2] << "\n" << endl;
	};
};

class Quadrilateral : public Figure
{
public:
	Quadrilateral(int a, int b, int c, int d, int A, int B, int C, int D) : Figure(a, b, c, d, A, B, C, D) {};

	void chekFigure(Figure* figure) override
	{
		if (getQuadrilateralAngles()[0] + getQuadrilateralAngles()[1] + getQuadrilateralAngles()[2] + getQuadrilateralAngles()[3] != 360)
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	int getSides(Figure* figure) override {

		int sides = 4;
		return sides;
	};


	void print_info(Figure* figure) override {

		cout << "Четырёхугольник: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Rectangle : public Quadrilateral
{
public:
	Rectangle(int a, int b, int c, int d, int A, int B, int C, int D) : Quadrilateral(a, b, c, d, A, B, C, D) {};

	void chekFigure(Figure* figure) override
	{
		if (getQuadrilateralAngles()[0] + getQuadrilateralAngles()[1] + getQuadrilateralAngles()[2] + getQuadrilateralAngles()[3] != 360)
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralSides()[0] != getQuadrilateralSides()[2] || getQuadrilateralSides()[1] != getQuadrilateralSides()[3])
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralAngles()[0] != 90 || getQuadrilateralAngles()[1] != 90 || getQuadrilateralAngles()[2] != 90 || getQuadrilateralAngles()[3] != 90)
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	void print_info(Figure* figure) override {

		cout << "Прямоугольник: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Square : public Quadrilateral
{
public:
	Square(int a, int b, int c, int d, int A, int B, int C, int D) : Quadrilateral(a, b, c, d, A, B, C, D) {};

	void chekFigure(Figure* figure) override
	{
		if (getQuadrilateralAngles()[0] + getQuadrilateralAngles()[1] + getQuadrilateralAngles()[2] + getQuadrilateralAngles()[3] != 360)
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralSides()[0] != getQuadrilateralSides()[1] || getQuadrilateralSides()[0] != getQuadrilateralSides()[2] || getQuadrilateralSides()[0] != getQuadrilateralSides()[3])
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralAngles()[0] != 90 || getQuadrilateralAngles()[1] != 90 || getQuadrilateralAngles()[2] != 90 || getQuadrilateralAngles()[3] != 90)
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	void print_info(Figure* figure) override {

		cout << "Квадрат: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Parallelogram : public Quadrilateral
{
public:
	Parallelogram(int a, int b, int c, int d, int A, int B, int C, int D) : Quadrilateral(a, b, c, d, A, B, C, D) {};

	void chekFigure(Figure* figure) override
	{
		if (getQuadrilateralAngles()[0] + getQuadrilateralAngles()[1] + getQuadrilateralAngles()[2] + getQuadrilateralAngles()[3] != 360)
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralSides()[0] != getQuadrilateralSides()[2] || getQuadrilateralSides()[1] != getQuadrilateralSides()[3])
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralAngles()[0] != getQuadrilateralAngles()[2] || getQuadrilateralAngles()[1] != getQuadrilateralAngles()[3])
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	void print_info(Figure* figure) override {
		cout << "Параллелограмм: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << "\n" << endl;
	};
};

class Rhombus : public Quadrilateral
{
public:
	Rhombus(int a, int b, int c, int d, int A, int B, int C, int D) : Quadrilateral(a, b, c, d, A, B, C, D) {};

	void chekFigure(Figure* figure) override
	{
		if (getQuadrilateralAngles()[0] + getQuadrilateralAngles()[1] + getQuadrilateralAngles()[2] + getQuadrilateralAngles()[3] != 360)
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralSides()[0] != getQuadrilateralSides()[1] || getQuadrilateralSides()[0] != getQuadrilateralSides()[2] || getQuadrilateralSides()[0] != getQuadrilateralSides()[3])
		{
			cout << "Неправильная" << endl;
		}
		else if (getQuadrilateralAngles()[0] != getQuadrilateralAngles()[2] || getQuadrilateralAngles()[1] != getQuadrilateralAngles()[3])
		{
			cout << "Неправильная" << endl;
		}
		else
			cout << "Правильная" << endl;
	};

	int getSides(Figure* figure) override
	{
		int size = 4;
		return size;
	};


	void print_info(Figure* figure) override {
		cout << "Ромб: " << endl;
		chekFigure(figure);
		cout << "Количество сторон: " << getSides(figure) << endl;
		cout << "Стороны: " << "a = " << getQuadrilateralSides()[0] << "\tb = " << getQuadrilateralSides()[1] << "\tc = " << getQuadrilateralSides()[2] << "\td = " << getQuadrilateralSides()[3] << endl;
		cout << "Углы: " << "\t A = " << getQuadrilateralAngles()[0] << "\tB = " << getQuadrilateralAngles()[1] << "\tC = " << getQuadrilateralAngles()[2] << "\tD = " << getQuadrilateralAngles()[3] << endl;
	};
};


int main()
{
	setlocale(LC_ALL, "ru");

	Figure figure;
	figure.print_info(&figure);

	Triangle triangle(10, 20, 30, 50, 60, 70);
	triangle.print_info(&triangle);

	RightTriangle rightTriangle(10, 20, 30, 50, 40, 90);
	rightTriangle.print_info(&rightTriangle);

	IsoscelesTriangle isoscelesTriangle(10, 20, 10, 50, 80, 50);
	isoscelesTriangle.print_info(&isoscelesTriangle);

	EquilateralTriangle equilateralTriangle(20, 20, 20, 60, 60, 60);
	equilateralTriangle.print_info(&equilateralTriangle);

	Quadrilateral quadrilateral(10, 20, 30, 40, 90, 90, 90, 90);
	quadrilateral.print_info(&quadrilateral);

	Rectangle rectangle(10, 20, 10, 20, 90, 90, 90, 90);
	rectangle.print_info(&rectangle);

	Square square(15, 15, 15, 15, 90, 90, 90, 90);
	square.print_info(&square);

	Parallelogram parallelogram(10, 20, 10, 20, 90, 90, 90, 90);
	parallelogram.print_info(&parallelogram);

	Rhombus rhombus(20, 20, 20, 10, 90, 90, 90, 90);
	rhombus.print_info(&rhombus);

	return 0;
};

