#include "Square.h"
#include <iostream>
#include "CreationError.h"
using namespace std;

Square::Square(int a, int b, int c, int d, int A, int B, int C, int D) : Figure(a, b, c, d, A, B, C, D) {

	cout << "\n" << "�������:" << endl;

	if (A + B + C + D != 360)
	{
		throw CreationError("����� ����� ������ ���� 360 ��������.");
	}
	else if (a != b || a != c || a != d || A != 90 || B != 90 || C != 90 || D != 90)
	{
		throw CreationError("������� ������ ���� �����. ��� ���� ����� 90.");
	}

};
void Square::print_info(Figure* figure) {

	cout << "�������: " << getQuadrilateralSides()[0] << ", " << getQuadrilateralSides()[1] << ", " << getQuadrilateralSides()[2] << ", " << getQuadrilateralSides()[3] << ";";
	cout << " ����: " << getQuadrilateralAngles()[0] << ", " << getQuadrilateralAngles()[1] << ", " << getQuadrilateralAngles()[2] << ", " << getQuadrilateralAngles()[3] << ";";
};