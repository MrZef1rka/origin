#include "Triangle.h"
#include "CreationError.h"
#include <iostream>
using namespace std;

Triangle::Triangle(int a, int b, int c, int A, int B, int C) : Figure(a, b, c, A, B, C) {

	cout << "�����������:" << endl;
		if (a <= 0 || b <= 0 || c <= 0)
		{
			throw CreationError("������� ������ ���� �������������� �������.");
		}
		else if (A + B + C != 180) {

			throw CreationError("����� ����� ������ ���� 180 ��������.");
		}
	};

    void Triangle::print_info(Figure * figure) {

		cout << "�������: " << getTriangleSides()[0] << ", " << getTriangleSides()[1] << ", " << getTriangleSides()[2] << ";" <<
			" ����: " << getTriangleAngles()[0] << ", " << getTriangleAngles()[1] << ", " << getTriangleAngles()[2] << ";";
    };
