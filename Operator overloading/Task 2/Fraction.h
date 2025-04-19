#pragma once
#include <iostream>

class Fraction
{
private:
	int numerator;
	int denominator;

public:
	Fraction(int numerator, int denominator);
	int findLCM(int a, int b);
	int getcd(int a, int b);
	void reduce();

	Fraction operator+(Fraction other);
	Fraction operator-(Fraction other);
	Fraction operator*(Fraction other);
	Fraction operator/(Fraction other);
	Fraction operator++();
	Fraction operator++(int);
	Fraction operator--();
	Fraction operator--(int);
	Fraction operator-() const;
	friend std::ostream& operator<<(std::ostream& out, const Fraction& fraction);

};

