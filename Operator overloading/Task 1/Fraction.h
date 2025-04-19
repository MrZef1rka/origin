#pragma once

class Fraction
{
private:
	int numerator;
	int denominator;

public:
	Fraction(int numerator, int denominator);
	int getcd(int a, int b);
	void reduce();

	bool operator==(Fraction other);
	bool operator!=(Fraction other);
	bool operator<(Fraction other);
	bool operator>(Fraction other);
	bool operator<=(Fraction other);
	bool operator>=(Fraction other);

};
