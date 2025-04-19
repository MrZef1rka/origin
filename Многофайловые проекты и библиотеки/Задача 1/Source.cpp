#include "Source.h"

double addition(double a, double b) {
	return a + b;
};
double subtraction(double a, double b) {
	return a - b;
};
double multiplication(double a, double b) {
	return a * b;
};
double division(double a, double b) {
	return a / b;
};
double degree(double& a, int b) {
	double res = 1;

	for (int i = 1; i <= b; i++)
	{
		res = res * a;
	}
	return res;
};
